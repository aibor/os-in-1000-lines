#pragma once

#define SBI_CONSOLE_PUTCHAR_EXTENSION 0x1
#define SBI_SYSTEM_RESET_EXTENSION    0x53525354

struct sbiret {
  long error;
  long value;
};

struct sbiret sbi_putchar(char c);
struct sbiret sbi_shutdown();
