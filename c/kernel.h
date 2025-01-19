#pragma once

#include "common.h"

#define PANIC(fmt, ...)                                                   \
  do {                                                                    \
    printf("PANIC: %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
    while (1) {                                                           \
    }                                                                     \
  } while (0)

#define READ_CSR(reg)                                     \
  ({                                                      \
    long __tmp;                                           \
    __asm__ __volatile__("csrr %0, " #reg : "=r"(__tmp)); \
    __tmp;                                                \
  })

#define WRITE_CSR(reg, value)                               \
  do {                                                      \
    long __tmp = (value);                                   \
    __asm__ __volatile__("csrw " #reg ", %0" ::"r"(__tmp)); \
  } while (0)

struct trap_frame {
  ulong_t ra;
  ulong_t gp;
  ulong_t tp;
  ulong_t t0;
  ulong_t t1;
  ulong_t t2;
  ulong_t t3;
  ulong_t t4;
  ulong_t t5;
  ulong_t t6;
  ulong_t a0;
  ulong_t a1;
  ulong_t a2;
  ulong_t a3;
  ulong_t a4;
  ulong_t a5;
  ulong_t a6;
  ulong_t a7;
  ulong_t s0;
  ulong_t s1;
  ulong_t s2;
  ulong_t s3;
  ulong_t s4;
  ulong_t s5;
  ulong_t s6;
  ulong_t s7;
  ulong_t s8;
  ulong_t s9;
  ulong_t s10;
  ulong_t s11;
  ulong_t sp;
} __attribute__((packed));

#define PROCS_MAX 8 // Maximum number of processes

#define PROC_UNUSED   0 // Unused process control structure
#define PROC_RUNNABLE 1 // Runnable process

struct process {
  int      pid;         // Process ID
  int      state;       // Process state: PROC_UNUSED or PROC_RUNNABLE
  vaddr_t  sp;          // Stack pointer
  ulong_t *page_table;  // Process page table
  uint8_t  stack[8192]; // Kernel stack
};

#define CALLEE_SAVED_REGS_MAX 12

#define SATP_SV39 (8ull << 60)
#define PAGE_V    (1 << 0) // "Valid" bit (entry is enabled)
#define PAGE_R    (1 << 1) // Readable
#define PAGE_W    (1 << 2) // Writable
#define PAGE_X    (1 << 3) // Executable
#define PAGE_U    (1 << 4) // User (accessible in user mode)
