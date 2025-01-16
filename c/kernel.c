#include "kernel.h"
#include "common.h"
#include "sbi.h"

extern char __bss[], __bss_end[];
extern char __free_ram[], __free_ram_end[];

__attribute__((naked)) __attribute__((aligned(sizeof(ulong_t)))) void
kernel_entry(void) {
  __asm__ __volatile__("csrw sscratch, sp\n"
                       "addi sp, sp, -31 * %[size]\n"
                       "sd ra,  0  * %[size](sp)\n"
                       "sd gp,  1  * %[size](sp)\n"
                       "sd tp,  2  * %[size](sp)\n"
                       "sd t0,  3  * %[size](sp)\n"
                       "sd t1,  4  * %[size](sp)\n"
                       "sd t2,  5  * %[size](sp)\n"
                       "sd t3,  6  * %[size](sp)\n"
                       "sd t4,  7  * %[size](sp)\n"
                       "sd t5,  8  * %[size](sp)\n"
                       "sd t6,  9  * %[size](sp)\n"
                       "sd a0,  10 * %[size](sp)\n"
                       "sd a1,  11 * %[size](sp)\n"
                       "sd a2,  12 * %[size](sp)\n"
                       "sd a3,  13 * %[size](sp)\n"
                       "sd a4,  14 * %[size](sp)\n"
                       "sd a5,  15 * %[size](sp)\n"
                       "sd a6,  16 * %[size](sp)\n"
                       "sd a7,  17 * %[size](sp)\n"
                       "sd s0,  18 * %[size](sp)\n"
                       "sd s1,  19 * %[size](sp)\n"
                       "sd s2,  20 * %[size](sp)\n"
                       "sd s3,  21 * %[size](sp)\n"
                       "sd s4,  22 * %[size](sp)\n"
                       "sd s5,  23 * %[size](sp)\n"
                       "sd s6,  24 * %[size](sp)\n"
                       "sd s7,  25 * %[size](sp)\n"
                       "sd s8,  26 * %[size](sp)\n"
                       "sd s9,  27 * %[size](sp)\n"
                       "sd s10, 28 * %[size](sp)\n"
                       "sd s11, 29 * %[size](sp)\n"

                       "csrr a0, sscratch\n"
                       "sd a0, 30 * %[size](sp)\n"

                       "mv a0, sp\n"
                       "call handle_trap\n"

                       "ld ra,  0 * %[size](sp)\n"
                       "ld gp,  1 * %[size](sp)\n"
                       "ld tp,  2 * %[size](sp)\n"
                       "ld t0,  3 * %[size](sp)\n"
                       "ld t1,  4 * %[size](sp)\n"
                       "ld t2,  5 * %[size](sp)\n"
                       "ld t3,  6 * %[size](sp)\n"
                       "ld t4,  7 * %[size](sp)\n"
                       "ld t5,  8 * %[size](sp)\n"
                       "ld t6,  9 * %[size](sp)\n"
                       "ld a0,  10 * %[size](sp)\n"
                       "ld a1,  11 * %[size](sp)\n"
                       "ld a2,  12 * %[size](sp)\n"
                       "ld a3,  13 * %[size](sp)\n"
                       "ld a4,  14 * %[size](sp)\n"
                       "ld a5,  15 * %[size](sp)\n"
                       "ld a6,  16 * %[size](sp)\n"
                       "ld a7,  17 * %[size](sp)\n"
                       "ld s0,  18 * %[size](sp)\n"
                       "ld s1,  19 * %[size](sp)\n"
                       "ld s2,  20 * %[size](sp)\n"
                       "ld s3,  21 * %[size](sp)\n"
                       "ld s4,  22 * %[size](sp)\n"
                       "ld s5,  23 * %[size](sp)\n"
                       "ld s6,  24 * %[size](sp)\n"
                       "ld s7,  25 * %[size](sp)\n"
                       "ld s8,  26 * %[size](sp)\n"
                       "ld s9,  27 * %[size](sp)\n"
                       "ld s10, 28 * %[size](sp)\n"
                       "ld s11, 29 * %[size](sp)\n"
                       "ld sp,  30 * %[size](sp)\n"
                       "sret\n"
                       :
                       : [size] "i"(sizeof(ulong_t)));
}

void
handle_trap(__attribute__((unused)) struct trap_frame *f) {
  ulong_t scause  = READ_CSR(scause);
  ulong_t stval   = READ_CSR(stval);
  ulong_t user_pc = READ_CSR(sepc);

  PANIC(
      "unexpected trap scause=%x, stval=%x, sepc=%x\n", scause, stval, user_pc);
}

void
putchar(char ch) {
  sbi_putchar(ch);
}

void
shutdown() {
  printf("shutting down\n");
  sbi_shutdown();
  PANIC("shutdown failed!");
}

paddr_t
alloc_pages(uint32_t n) {
  static paddr_t next_paddr = (paddr_t)__free_ram;
  paddr_t        paddr      = next_paddr;
  next_paddr += n * PAGE_SIZE;

  if (next_paddr > (paddr_t)__free_ram_end)
    PANIC("out of memory");

  memset((void *)paddr, 0, n * PAGE_SIZE);
  return paddr;
}

void
kernel_main(void) {
  memset(__bss, 0, (size_t)__bss_end - (size_t)__bss);

  paddr_t paddr0 = alloc_pages(2);
  paddr_t paddr1 = alloc_pages(1);
  printf("alloc_pages test: paddr0=%x\n", paddr0);
  printf("alloc_pages test: paddr1=%x\n", paddr1);

  printf("booted!\n");

  shutdown();
}
