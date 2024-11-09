#include <sys/types.h>
#include <stdint.h>

#ifndef ASAN_INTERFACE_INTERNAL__H
#define ASAN_INTERFACE_INTERNAL__H

// This structure is used to describe the source location of a place where
// global was defined.
struct __asan_global_source_location {
  const char *filename;
  int line_no;
  int column_no;
};

// This structure describes an instrumented global variable.
struct __asan_global {
  uintptr_t beg;                // The address of the global.
  uintptr_t size;               // The original size of the global.
  uintptr_t size_with_redzone;  // The size with the redzone.
  const char *name;        // Name as a C string.
  const char *module_name; // Module name as a C string. This pointer is a
                           // unique identifier of a module.
  uintptr_t has_dynamic_init;   // Non-zero if the global has dynamic initializer.
  struct __asan_global_source_location *gcc_location;  // Source location of a global,
                                                // used by GCC compiler. LLVM uses
                                                // llvm-symbolizer that relies
                                                // on DWARF debugging info.
  uintptr_t odr_indicator;      // The address of the ODR indicator symbol.
};

#endif
