#!/bin/bash
set -xue

. ../shared/qemu.sh

cargo build

run_qemu target/riscv64gc-unknown-none-elf/debug/kernel
