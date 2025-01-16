#pragma once

#include "common.h"

// https://github.com/riscv-non-isa/riscv-sbi-doc/blob/master/src/ext-debug-console.adoc#function-listing
// https://github.com/riscv-non-isa/riscv-sbi-doc/blob/master/src/ext-sys-reset.adoc#function-system-reset-fid-0

#define SBI_EXT_DEBUG_CONSOLE 0x4442434E
#define SBI_EXT_SYSTEM_RESET  0x53525354

enum sbi_func_console {
  SBI_FUNC_CONSOLE_WRITE_BYTE = 2,
};

enum sbi_sys_reset_type {
  SBI_SYS_RESET_TYPE_SHUTDOWN = 0,
};

enum sbi_sys_reset_reason {
  SBI_SYS_RESET_REASON_NONE = 0,
};

struct sbiret {
  ulong_t error;
  ulong_t value;
};

struct sbiret sbi_putchar(char c);
struct sbiret sbi_shutdown();
