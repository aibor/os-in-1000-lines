#include "sbi.h"

static struct sbiret
sbi_call(long arg0,
         long arg1,
         long arg2,
         long arg3,
         long arg4,
         long arg5,
         long fid,
         long eid) {
  register long a0 __asm__("a0") = arg0;
  register long a1 __asm__("a1") = arg1;
  register long a2 __asm__("a2") = arg2;
  register long a3 __asm__("a3") = arg3;
  register long a4 __asm__("a4") = arg4;
  register long a5 __asm__("a5") = arg5;
  register long a6 __asm__("a6") = fid;
  register long a7 __asm__("a7") = eid;

  __asm__ __volatile__(
      "ecall"
      : "=r"(a0), "=r"(a1)
      : "r"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a5), "r"(a6), "r"(a7)
      : "memory");
  return (struct sbiret){.error = a0, .value = a1};
}

struct sbiret
sbi_putchar(char ch) {
  return sbi_call(ch, 0, 0, 0, 0, 0, 0, SBI_CONSOLE_PUTCHAR_EXTENSION);
}

struct sbiret
sbi_shutdown() {
  return sbi_call(0, 0, 0, 0, 0, 0, 0, SBI_SYSTEM_RESET_EXTENSION);
}