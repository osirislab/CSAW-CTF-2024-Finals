#define _GNU_SOURCE
#include <stdint.h>
#include <stdlib.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>

#include "asan_interface_internal.h"
#include "xxhash.h"
#include "interpose.h"

#define STR_(X) #X
#define STR(X) STR_(X)

static const char flag[] = STR(FLAG);

#if DEBUG >= 1
__attribute__((format(printf, 1, 2))) 
void debug(const char *format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fprintf(stderr, "\n");
}
#else
void debug(const char *format, ...) {}
#endif

#if DEBUG >= 2
#define trace debug
#else
void trace(const char *format, ...) {}
#endif

#define SHADOW_SIZE (128 * 1024)
#define HEAP_SIZE (128 * 1024 * 1024)

char *shadow;
char *heap;
char *heap_base;
void poison_n(uintptr_t p, size_t n);
void unpoison_n(uintptr_t p, size_t n);

void early_init() {
    shadow = (char*)mmap(NULL, SHADOW_SIZE, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
    if (shadow == MAP_FAILED) {
        abort();
    }

    heap_base = (char*)mmap(NULL, HEAP_SIZE, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
    if (heap_base == MAP_FAILED) {
        abort();
    }
    heap = heap_base;
}

#define GRANULARITY_SHIFT 3
#define GRANULARITY_SIZE (1ULL << GRANULARITY_SHIFT)

void load_n(uintptr_t p, size_t n) {
    if (!shadow) {
        early_init();
    }
    p &= ~(GRANULARITY_SIZE - 1);
    for (uintptr_t addr = p; addr < p + n; addr += GRANULARITY_SIZE) {
        XXH64_hash_t hash = XXH64(&addr, sizeof(addr), 0);
        if (shadow[hash % SHADOW_SIZE]) {
            errx(EXIT_FAILURE, "bad load of %p", (void*)addr);
        }
    }
}

void store_n(uintptr_t p, size_t n) {
    if (!shadow) {
        early_init();
    }
    p &= ~(GRANULARITY_SIZE - 1);
    for (uintptr_t addr = p; addr < p + n; addr += GRANULARITY_SIZE) {
        XXH64_hash_t hash = XXH64(&addr, sizeof(addr), 0);
        if (shadow[hash % SHADOW_SIZE]) {
            errx(EXIT_FAILURE, "bad store to %p", (void*)addr);
        }
    }
}

void poison_n(uintptr_t p, size_t n) {
    if (!shadow) {
        early_init();
    }
    p &= ~(GRANULARITY_SIZE - 1);
    for (uintptr_t addr = p; addr < p + n; addr += GRANULARITY_SIZE) {
        trace("poison %p", addr);
        XXH64_hash_t hash = XXH64(&addr, sizeof(addr), 0);
        shadow[hash % SHADOW_SIZE] = 1;
    }
}

void unpoison_n(uintptr_t p, size_t n) {
    if (!shadow) {
        early_init();
    }
    p &= ~(GRANULARITY_SIZE - 1);
    for (uintptr_t addr = p; addr < p + n; addr += GRANULARITY_SIZE) {
        trace("unpoison %p", addr);
        XXH64_hash_t hash = XXH64(&addr, sizeof(addr), 0);
        shadow[hash % SHADOW_SIZE] = 0;
    }
}

INTERPOSE_C(void*, malloc, (size_t size), (size)) {
    if (!heap) {
        early_init();
    }
    if (!size) {
        return NULL;
    }
    size = (size + GRANULARITY_SIZE - 1) & ~(GRANULARITY_SIZE - 1);
    if (heap + size > heap_base + HEAP_SIZE) {
        abort();
    }
    char *p = heap;
    heap += GRANULARITY_SIZE + size + GRANULARITY_SIZE;
    bzero(p, GRANULARITY_SIZE + size + GRANULARITY_SIZE);
    *(size_t*)p = size;
    poison_n((uintptr_t)p, GRANULARITY_SIZE);
    p += GRANULARITY_SIZE;
    unpoison_n((uintptr_t)p, size);
    poison_n((uintptr_t)p + size, GRANULARITY_SIZE);
    debug("malloc %zu = %p", size, p);
    return p;
}

#ifdef __APPLE__
#include <malloc/malloc.h>
#define malloc_usable_size malloc_size
#else
#include <malloc.h>
#endif

INTERPOSE_C(size_t, malloc_usable_size, (void *ptr), (ptr)) {
    if (!heap) {
        early_init();
    }
    if (!ptr) {
        return 0;
    }
    return *(size_t*)((uintptr_t)ptr - GRANULARITY_SIZE);
}

INTERPOSE_C_VOID(free, (void* ptr), (ptr)) {
    if (!heap) {
        early_init();
    }
    // can't log anything here since all the printf's free() internally so we'd recurse
    if (!ptr || !(ptr >= heap_base && ptr <= (heap_base + HEAP_SIZE))) {
        return;
    }
    size_t size = *(size_t*)((uintptr_t)ptr - GRANULARITY_SIZE);
    poison_n((uintptr_t)ptr, size);
    for (off_t i = 0; i < size; i++) {
        ((char*)ptr)[i] = flag[i % sizeof(flag)];
    }
}

INTERPOSE_C(void*, calloc, (size_t count, size_t size), (count, size)) {
    if (!heap) {
        early_init();
    }
    debug("calloc %zu x %zu", count, size);
    void *result = __interpose_malloc(count * size);
    memset(result, 0, count * size);
    unpoison_n((uintptr_t)result, count * size);
    return result;
}

INTERPOSE_C(void*, realloc, (void* ptr, size_t size), (ptr, size)) {
    if (!heap) {
        early_init();
    }

    if (!ptr) {
        return __interpose_malloc(size);
    }

    size_t old_size = *(size_t*)((uintptr_t)ptr - sizeof(size_t));
    debug("realloc %p 0x%lx (old 0x%lx)", ptr, size, old_size);

    if (old_size <= size) {
        return ptr;
    }

    void *result = __interpose_malloc(size);
    memcpy(result, ptr, old_size);

    poison_n((uintptr_t)ptr, old_size);
    unpoison_n((uintptr_t)result, size);

    return result;
}

void __asan_abi_init() {}

void __asan_abi_load_n(void *p, size_t n, bool abort) {
    trace("load %p 0x%lx", p, n);
    load_n((uintptr_t)p, n);
}
void __asan_abi_exp_load_n(void *p, size_t n, int exp, bool abort) {
    trace("expload %p 0x%lx", p, n);
    load_n((uintptr_t)p, n);
}
void __asan_abi_store_n(void *p, size_t n, bool abort) {
    trace("store %p 0x%lx", p, n);
    store_n((uintptr_t)p, n);
}
void __asan_abi_exp_store_n(void *p, size_t n, int exp, bool abort) {
    trace("expstore %p 0x%lx", p, n);
    store_n((uintptr_t)p, n);
}


// Functions concerning instrumented global variables:
void __asan_abi_register_image_globals(uintptr_t *flag) {}
void __asan_abi_unregister_image_globals(uintptr_t *flag) {}
void __asan_abi_register_elf_globals(bool *flag, void *start, void *stop) {}
void __asan_abi_unregister_elf_globals(bool *flag, void *start, void *stop) {}
void __asan_abi_register_globals(struct __asan_global *globals, uintptr_t n) {}
void __asan_abi_unregister_globals(struct __asan_global *globals, uintptr_t n) {}

// Functions concerning dynamic library initialization
void __asan_abi_before_dynamic_init(const char *module_name) {}
void __asan_abi_after_dynamic_init() {}

// Functions concerning block memory destinations
void *__asan_abi_memcpy(void *dst, const void *src, uintptr_t size) {
    trace("memcpy %p %p 0x%lx", dst, src, size);
    load_n((uintptr_t)src, size);
    store_n((uintptr_t)dst, size);
    return memcpy(dst, src, size);
}
void *__asan_abi_memset(void *s, int c, uintptr_t n) {
    trace("memset %p 0x%x 0x%lx", s, c, n);
    store_n((uintptr_t)s, n);
    return memset(s, c, n);
}
void *__asan_abi_memmove(void *dest, const void *src, uintptr_t n) {
    trace("memmove %p %p 0x%lx", dest, src, n);
    load_n((uintptr_t)src, n);
    store_n((uintptr_t)dest, n);
    return memmove(dest, src, n);
}

void __asan_abi_handle_no_return() {}

void __sanitizer_annotate_contiguous_container(const void *beg_p,
                                               const void *end_p,
                                               const void *old_mid_p,
                                               const void *new_mid_p) {
    trace("contiguous_container %p %p %p %p", beg_p, end_p, old_mid_p, new_mid_p);
    if (old_mid_p > new_mid_p) {
        poison_n((uintptr_t)new_mid_p, ((uintptr_t)old_mid_p - (uintptr_t)new_mid_p));
    } else {
        unpoison_n((uintptr_t)old_mid_p, ((uintptr_t)new_mid_p - (uintptr_t)old_mid_p));
    }
}

#ifdef __APPLE__
void __asan_version_mismatch_check_apple_clang_1600() {}
#else
void __asan_version_mismatch_check_v8() {}
#endif
