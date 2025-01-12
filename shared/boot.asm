  .section .text.boot
  .globl boot
boot:
  la sp, __stack_top
  call kernel_main
