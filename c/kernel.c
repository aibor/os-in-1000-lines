#include "kernel.h"
#include "common.h"
#include "sbi.h"

extern char __bss[], __bss_end[];
extern char __free_ram[], __free_ram_end[];

__attribute__((naked)) __attribute__((aligned(sizeof(ulong_t)))) void
kernel_entry(void) {
  __asm__ __volatile__("csrrw sp, sscratch, sp\n"
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

                       // Retrieve and save the sp at the time of exception.
                       "csrr a0, sscratch\n"
                       "sd a0, 30 * %[size](sp)\n"

                       // Reset the kernel stack.
                       "addi a0, sp, 31 * %[size]\n"
                       "csrw sscratch, a0\n"

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

__attribute__((naked)) void
switch_context(vaddr_t *prev_sp, vaddr_t *next_sp) {
  // Save callee-saved registers onto the current process's stack.
  __asm__ __volatile__("addi sp, sp, -13 * %[size]\n"
                       "sd ra,  0  * %[size](sp)\n"
                       "sd s0,  1  * %[size](sp)\n"
                       "sd s1,  2  * %[size](sp)\n"
                       "sd s2,  3  * %[size](sp)\n"
                       "sd s3,  4  * %[size](sp)\n"
                       "sd s4,  5  * %[size](sp)\n"
                       "sd s5,  6  * %[size](sp)\n"
                       "sd s6,  7  * %[size](sp)\n"
                       "sd s7,  8  * %[size](sp)\n"
                       "sd s8,  9  * %[size](sp)\n"
                       "sd s9,  10 * %[size](sp)\n"
                       "sd s10, 11 * %[size](sp)\n"
                       "sd s11, 12 * %[size](sp)\n"

                       // Switch the stack pointer.
                       "sd sp, (a0)\n" // *prev_sp = sp;
                       "ld sp, (a1)\n" // Switch stack pointer (sp) here

                       // Restore callee-saved registers from the next
                       // process's stack.
                       "ld ra,  0  * %[size](sp)\n"
                       "ld s0,  1  * %[size](sp)\n"
                       "ld s1,  2  * %[size](sp)\n"
                       "ld s2,  3  * %[size](sp)\n"
                       "ld s3,  4  * %[size](sp)\n"
                       "ld s4,  5  * %[size](sp)\n"
                       "ld s5,  6  * %[size](sp)\n"
                       "ld s6,  7  * %[size](sp)\n"
                       "ld s7,  8  * %[size](sp)\n"
                       "ld s8,  9  * %[size](sp)\n"
                       "ld s9,  10 * %[size](sp)\n"
                       "ld s10, 11 * %[size](sp)\n"
                       "ld s11, 12 * %[size](sp)\n"
                       "addi sp, sp, 13 * %[size]\n"
                       "ret\n"
                       :
                       : [size] "i"(sizeof(ulong_t)));
}

void
handle_trap(__attribute__((unused)) struct trap_frame *f) {
  ulong_t scause = READ_CSR(scause);
  ulong_t stval = READ_CSR(stval);
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
  paddr_t        paddr = next_paddr;
  next_paddr += n * PAGE_SIZE;

  if (next_paddr > (paddr_t)__free_ram_end)
    PANIC("out of memory");

  memset((void *)paddr, 0, n * PAGE_SIZE);
  return paddr;
}

struct process procs[PROCS_MAX]; // All process control structures.

struct process *
create_process(ulong_t pc) {
  // Find an unused process control structure.
  struct process *proc = NULL;
  int             i;
  for (i = 0; i < PROCS_MAX; i++) {
    if (procs[i].state == PROC_UNUSED) {
      proc = &procs[i];
      break;
    }
  }

  if (!proc) {
    PANIC("no free process slots");
  }

  // Stack callee-saved registers. These register values will be restored in
  // the first context switch in switch_context.
  ulong_t *sp = (ulong_t *)&proc->stack[sizeof(proc->stack)];
  for (int j = 0; j < CALLEE_SAVED_REGS_MAX; j++) {
    *--sp = 0;
  }
  *--sp = pc; // ra

  // Initialize fields.
  proc->pid = i + 1;
  proc->state = PROC_RUNNABLE;
  proc->sp = (vaddr_t)sp;
  return proc;
}

struct process *current_proc;
struct process *idle_proc;

void
yield(void) {
  // Search for a runnable process
  struct process *next = idle_proc;
  for (int i = 0; i < PROCS_MAX; i++) {
    struct process *proc = &procs[(current_proc->pid + i) % PROCS_MAX];
    if (proc->state == PROC_RUNNABLE && proc->pid > 0) {
      next = proc;
      break;
    }
  }

  // If there's no runnable process other than the current one, return and continue processing
  if (next == current_proc) {
    return;
  }

  __asm__ __volatile__(
      "csrw sscratch, %[sscratch]\n"
      :
      : [sscratch] "r"((ulong_t)&next->stack[sizeof(next->stack)]));

  // Context switch
  struct process *prev = current_proc;
  current_proc = next;
  switch_context(&prev->sp, &next->sp);
}

void
proc_a_entry(void) {
  printf("starting process A\n");
  for (int i = 0; i < 10; i++) {
    putchar('A');
    yield();
  }
  printf("\n");
  shutdown();
}

void
proc_b_entry(void) {
  printf("starting process B\n");
  for (int i = 0; i < 10; i++) {
    putchar('B');
    yield();
  }
  printf("\n");
  shutdown();
}

void
kernel_main(void) {
  memset(__bss, 0, (size_t)__bss_end - (size_t)__bss);

  WRITE_CSR(stvec, (ulong_t)kernel_entry);

  idle_proc = create_process((ulong_t)NULL);
  idle_proc->pid = -1; // idle
  current_proc = idle_proc;

  create_process((ulong_t)proc_a_entry);
  create_process((ulong_t)proc_b_entry);

  yield();

  PANIC("unreachable here!");
}
