#include "kernel.h"
#include "common.h"
#include "sbi.h"

extern char __bss[], __bss_end[];
extern char __free_ram[], __free_ram_end[];

__attribute__((naked)) __attribute__((aligned(sizeof(long)))) void
kernel_entry(void) {
  __asm__ __volatile__("csrw sscratch, sp\n"
                       "addi sp, sp, -%[size] * 31\n"
                       "sw ra,  %[size] * 0(sp)\n"
                       "sw gp,  %[size] * 1(sp)\n"
                       "sw tp,  %[size] * 2(sp)\n"
                       "sw t0,  %[size] * 3(sp)\n"
                       "sw t1,  %[size] * 4(sp)\n"
                       "sw t2,  %[size] * 5(sp)\n"
                       "sw t3,  %[size] * 6(sp)\n"
                       "sw t4,  %[size] * 7(sp)\n"
                       "sw t5,  %[size] * 8(sp)\n"
                       "sw t6,  %[size] * 9(sp)\n"
                       "sw a0,  %[size] * 10(sp)\n"
                       "sw a1,  %[size] * 11(sp)\n"
                       "sw a2,  %[size] * 12(sp)\n"
                       "sw a3,  %[size] * 13(sp)\n"
                       "sw a4,  %[size] * 14(sp)\n"
                       "sw a5,  %[size] * 15(sp)\n"
                       "sw a6,  %[size] * 16(sp)\n"
                       "sw a7,  %[size] * 17(sp)\n"
                       "sw s0,  %[size] * 18(sp)\n"
                       "sw s1,  %[size] * 19(sp)\n"
                       "sw s2,  %[size] * 20(sp)\n"
                       "sw s3,  %[size] * 21(sp)\n"
                       "sw s4,  %[size] * 22(sp)\n"
                       "sw s5,  %[size] * 23(sp)\n"
                       "sw s6,  %[size] * 24(sp)\n"
                       "sw s7,  %[size] * 25(sp)\n"
                       "sw s8,  %[size] * 26(sp)\n"
                       "sw s9,  %[size] * 27(sp)\n"
                       "sw s10, %[size] * 28(sp)\n"
                       "sw s11, %[size] * 29(sp)\n"

                       "csrr a0, sscratch\n"
                       "sw a0, %[size] * 30(sp)\n"

                       "mv a0, sp\n"
                       "call handle_trap\n"

                       "lw ra,   %[size] * 0(sp)\n"
                       "lw gp,   %[size] * 1(sp)\n"
                       "lw tp,   %[size] * 2(sp)\n"
                       "lw t0,   %[size] * 3(sp)\n"
                       "lw t1,   %[size] * 4(sp)\n"
                       "lw t2,   %[size] * 5(sp)\n"
                       "lw t3,   %[size] * 6(sp)\n"
                       "lw t4,   %[size] * 7(sp)\n"
                       "lw t5,   %[size] * 8(sp)\n"
                       "lw t6,   %[size] * 9(sp)\n"
                       "lw a0,   %[size] * 10(sp)\n"
                       "lw a1,   %[size] * 11(sp)\n"
                       "lw a2,   %[size] * 12(sp)\n"
                       "lw a3,   %[size] * 13(sp)\n"
                       "lw a4,   %[size] * 14(sp)\n"
                       "lw a5,   %[size] * 15(sp)\n"
                       "lw a6,   %[size] * 16(sp)\n"
                       "lw a7,   %[size] * 17(sp)\n"
                       "lw s0,   %[size] * 18(sp)\n"
                       "lw s1,   %[size] * 19(sp)\n"
                       "lw s2,   %[size] * 20(sp)\n"
                       "lw s3,   %[size] * 21(sp)\n"
                       "lw s4,   %[size] * 22(sp)\n"
                       "lw s5,   %[size] * 23(sp)\n"
                       "lw s6,   %[size] * 24(sp)\n"
                       "lw s7,   %[size] * 25(sp)\n"
                       "lw s8,   %[size] * 26(sp)\n"
                       "lw s9,   %[size] * 27(sp)\n"
                       "lw s10,  %[size] * 28(sp)\n"
                       "lw s11,  %[size] * 29(sp)\n"
                       "lw sp,   %[size] * 30(sp)\n"
                       "sret\n"
                       :
                       : [size] "i"(sizeof(long)));
}

void
handle_trap(__attribute__((unused)) struct trap_frame *f) {
  long scause  = READ_CSR(scause);
  long stval   = READ_CSR(stval);
  long user_pc = READ_CSR(sepc);

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
