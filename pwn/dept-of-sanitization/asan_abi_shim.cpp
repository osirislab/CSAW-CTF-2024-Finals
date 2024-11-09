//===-asan_abi_shim.cpp - ASan Stable ABI Shim-----------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include <stdbool.h>
#include <stddef.h>
#include <sys/cdefs.h>
#include <sys/types.h>
#include <assert.h>

#include "asan_interface_internal.h"
#include "asan_abi.h"

extern "C" {

// Functions concerning instrumented global variables
void __asan_register_image_globals(uintptr_t *flag) {
  __asan_abi_register_image_globals(flag);
}
void __asan_unregister_image_globals(uintptr_t *flag) {
  __asan_abi_unregister_image_globals(flag);
}
void __asan_register_elf_globals(uintptr_t *flag, void *start, void *stop) {
  bool bFlag = *flag;
  __asan_abi_register_elf_globals(&bFlag, start, stop);
  *flag = bFlag;
}
void __asan_unregister_elf_globals(uintptr_t *flag, void *start, void *stop) {
  bool bFlag = *flag;
  __asan_abi_unregister_elf_globals(&bFlag, start, stop);
  *flag = bFlag;
}
void __asan_register_globals(__asan_global *globals, uintptr_t n) {
  __asan_abi_register_globals(globals, n);
}
void __asan_unregister_globals(__asan_global *globals, uintptr_t n) {
  __asan_abi_unregister_globals(globals, n);
}

// Functions concerning dynamic library initialization
void __asan_before_dynamic_init(const char *module_name) {
  __asan_abi_before_dynamic_init(module_name);
}
void __asan_after_dynamic_init(void) { __asan_abi_after_dynamic_init(); }


// Functions concerning block memory destinations
void *__asan_memcpy(void *dst, const void *src, uintptr_t size) {
  return __asan_abi_memcpy(dst, src, size);
}
void *__asan_memset(void *s, int c, uintptr_t n) {
  return __asan_abi_memset(s, c, n);
}
void *__asan_memmove(void *dest, const void *src, uintptr_t n) {
  return __asan_abi_memmove(dest, src, n);
}

// Functions concerning RTL startup and initialization
void __asan_init(void) {
  static_assert(sizeof(uintptr_t) == 8 || sizeof(uintptr_t) == 4);
  static_assert(sizeof(uint64_t) == 8);
  static_assert(sizeof(uint32_t) == 4);

  __asan_abi_init();
}

void __asan_handle_no_return(void) { __asan_abi_handle_no_return(); }

// Functions concerning memory load and store
void __asan_load1(uintptr_t addr) { __asan_abi_load_n((void *)addr, 1, true); }
void __asan_load2(uintptr_t addr) { __asan_abi_load_n((void *)addr, 2, true); }
void __asan_load4(uintptr_t addr) { __asan_abi_load_n((void *)addr, 4, true); }
void __asan_load8(uintptr_t addr) { __asan_abi_load_n((void *)addr, 8, true); }
void __asan_load16(uintptr_t addr) { __asan_abi_load_n((void *)addr, 16, true); }
void __asan_loadN(uintptr_t addr, uintptr_t size) {
  __asan_abi_load_n((void *)addr, size, true);
}
void __asan_store1(uintptr_t addr) { __asan_abi_store_n((void *)addr, 1, true); }
void __asan_store2(uintptr_t addr) { __asan_abi_store_n((void *)addr, 2, true); }
void __asan_store4(uintptr_t addr) { __asan_abi_store_n((void *)addr, 4, true); }
void __asan_store8(uintptr_t addr) { __asan_abi_store_n((void *)addr, 8, true); }
void __asan_store16(uintptr_t addr) { __asan_abi_store_n((void *)addr, 16, true); }
void __asan_storeN(uintptr_t addr, uintptr_t size) {
  __asan_abi_store_n((void *)addr, size, true);
}

// Functions concerning memory load and store (experimental variants)
void __asan_exp_load1(uintptr_t addr, uint32_t exp) {
  __asan_abi_exp_load_n((void *)addr, 1, exp, true);
}
void __asan_exp_load2(uintptr_t addr, uint32_t exp) {
  __asan_abi_exp_load_n((void *)addr, 2, exp, true);
}
void __asan_exp_load4(uintptr_t addr, uint32_t exp) {
  __asan_abi_exp_load_n((void *)addr, 4, exp, true);
}
void __asan_exp_load8(uintptr_t addr, uint32_t exp) {
  __asan_abi_exp_load_n((void *)addr, 8, exp, true);
}
void __asan_exp_load16(uintptr_t addr, uint32_t exp) {
  __asan_abi_exp_load_n((void *)addr, 16, exp, true);
}
void __asan_exp_loadN(uintptr_t addr, uintptr_t size, uint32_t exp) {
  __asan_abi_exp_load_n((void *)addr, size, exp, true);
}
void __asan_exp_store1(uintptr_t addr, uint32_t exp) {
  __asan_abi_exp_store_n((void *)addr, 1, exp, true);
}
void __asan_exp_store2(uintptr_t addr, uint32_t exp) {
  __asan_abi_exp_store_n((void *)addr, 2, exp, true);
}
void __asan_exp_store4(uintptr_t addr, uint32_t exp) {
  __asan_abi_exp_store_n((void *)addr, 4, exp, true);
}
void __asan_exp_store8(uintptr_t addr, uint32_t exp) {
  __asan_abi_exp_store_n((void *)addr, 8, exp, true);
}
void __asan_exp_store16(uintptr_t addr, uint32_t exp) {
  __asan_abi_exp_store_n((void *)addr, 16, exp, true);
}
void __asan_exp_storeN(uintptr_t addr, uintptr_t size, uint32_t exp) {
  __asan_abi_exp_store_n((void *)addr, size, exp, true);
}

// Functions concerning memory load and store (noabort variants)
void __asan_load1_noabort(uintptr_t addr) {
  __asan_abi_load_n((void *)addr, 1, false);
}
void __asan_load2_noabort(uintptr_t addr) {
  __asan_abi_load_n((void *)addr, 2, false);
}
void __asan_load4_noabort(uintptr_t addr) {
  __asan_abi_load_n((void *)addr, 4, false);
}
void __asan_load8_noabort(uintptr_t addr) {
  __asan_abi_load_n((void *)addr, 8, false);
}
void __asan_load16_noabort(uintptr_t addr) {
  __asan_abi_load_n((void *)addr, 16, false);
}
void __asan_loadN_noabort(uintptr_t addr, uintptr_t size) {
  __asan_abi_load_n((void *)addr, size, false);
}
void __asan_store1_noabort(uintptr_t addr) {
  __asan_abi_store_n((void *)addr, 1, false);
}
void __asan_store2_noabort(uintptr_t addr) {
  __asan_abi_store_n((void *)addr, 2, false);
}
void __asan_store4_noabort(uintptr_t addr) {
  __asan_abi_store_n((void *)addr, 4, false);
}
void __asan_store8_noabort(uintptr_t addr) {
  __asan_abi_store_n((void *)addr, 8, false);
}
void __asan_store16_noabort(uintptr_t addr) {
  __asan_abi_store_n((void *)addr, 16, false);
}
void __asan_storeN_noabort(uintptr_t addr, uintptr_t size) {
  __asan_abi_store_n((void *)addr, size, false);
}

}
