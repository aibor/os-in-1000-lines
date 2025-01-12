# Operating System in 1,000 lines

This project follows the tutorial [OS in 1,000 lines][tutorial] and its 
[github repo][repo].

As in the original tutorial it is implemented in C, but with a few changes:
* RISCV64 instead of RISCV32
* Makefile used
* bootstrap assembly code extracted
* basic functions with additional optimizations

Additionally, the idea is adapted to an implementation in Rust. It tries to 
as close as possible to the C implementation and shares same basic assembler
and linker code. How to build a minimal Rust binary that can be used as kernel
has been taken from the following projects:
* https://github.com/xiaoyang-sde/rust-kernel-riscv
* https://docs.rust-embedded.org/embedonomicon/smallest-no-std.html

[tutorial]: https://operating-system-in-1000-lines.vercel.app
[repo]: https://github.com/nuta/operating-system-in-1000-lines
