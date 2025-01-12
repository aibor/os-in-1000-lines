#pragma once

#define PANIC(fmt, ...) \
  do { \
    printf("PANIC: %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
    while (1) { \
    } \
  } while (0)

#define READ_CSR(reg) \
  ({ \
    long __tmp; \
    __asm__ __volatile__("csrr %0, " #reg : "=r"(__tmp)); \
    __tmp; \
  })

#define WRITE_CSR(reg, value) \
  do { \
    long __tmp = (value); \
    __asm__ __volatile__("csrw " #reg ", %0" ::"r"(__tmp)); \
  } while (0)

struct trap_frame {
  long ra;
  long gp;
  long tp;
  long t0;
  long t1;
  long t2;
  long t3;
  long t4;
  long t5;
  long t6;
  long a0;
  long a1;
  long a2;
  long a3;
  long a4;
  long a5;
  long a6;
  long a7;
  long s0;
  long s1;
  long s2;
  long s3;
  long s4;
  long s5;
  long s6;
  long s7;
  long s8;
  long s9;
  long s10;
  long s11;
  long sp;
} __attribute__((packed));
