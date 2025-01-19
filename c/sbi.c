#include "sbi.h"
#include "common.h"

static struct sbiret
sbi_ecall(ulong_t eid,
          ulong_t fid,
          ulong_t arg0,
          ulong_t arg1,
          ulong_t arg2,
          ulong_t arg3,
          ulong_t arg4,
          ulong_t arg5) {
  register ulong_t a0 __asm__("a0") = arg0;
  register ulong_t a1 __asm__("a1") = arg1;
  register ulong_t a2 __asm__("a2") = arg2;
  register ulong_t a3 __asm__("a3") = arg3;
  register ulong_t a4 __asm__("a4") = arg4;
  register ulong_t a5 __asm__("a5") = arg5;
  register ulong_t a6 __asm__("a6") = fid;
  register ulong_t a7 __asm__("a7") = eid;

  __asm__ __volatile__(
      "ecall"
      : "=r"(a0), "=r"(a1)
      : "r"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a5), "r"(a6), "r"(a7)
      : "memory");
  return (struct sbiret){ .error = a0, .value = a1 };
}

static struct sbiret
sbi_debug_console(enum sbi_func_console function,
                  ulong_t               arg0,
                  ulong_t               arg1,
                  ulong_t               arg2) {
  return sbi_ecall(SBI_EXT_DEBUG_CONSOLE, function, arg0, arg1, arg2, 0, 0, 0);
}

static struct sbiret
sbi_system_reset(enum sbi_sys_reset_type   type,
                 enum sbi_sys_reset_reason reason) {
  return sbi_ecall(SBI_EXT_SYSTEM_RESET, type, reason, 0, 0, 0, 0, 0);
}

struct sbiret
sbi_putchar(char ch) {
  return sbi_debug_console(SBI_FUNC_CONSOLE_WRITE_BYTE, (ulong_t)ch, 0, 0);
}

struct sbiret
sbi_shutdown() {
  return sbi_system_reset(SBI_SYS_RESET_TYPE_SHUTDOWN,
                          SBI_SYS_RESET_REASON_NONE);
}
