CC = clang

CFLAGS = -std=c11 \
		 -O2 \
		 -g3 \
		 -Wall \
		 -Wextra \
		 --target=riscv64 \
		 -ffreestanding \
		 -mcmodel=medany \
		 -nostdlib

kernel: kernel.c common.c sbi.c boot.asm $(wildcard *.h) kernel.ld Makefile
	$(CC) $(CFLAGS) \
		-Wl,-Tkernel.ld \
		-Wl,-Map=kernel.map \
		-o $@ \
		$(filter %.c,$^) \
		boot.asm
