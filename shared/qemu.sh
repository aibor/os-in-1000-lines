#!/bin/bash
set -ue

: ${QEMU:=qemu-system-riscv64}

run_qemu() {
  local kernel="$1"

  if [[ -z $kernel ]]; then
    echo "no kernel file given" >&2
    return 1
  fi

  if [[ ! -r $kernel ]]; then
    echo "can't read kernel file '$kernel'" >&2
    return 1
  fi

  # Start QEMU
  $QEMU \
    -machine virt \
    -bios default \
    -nographic \
    -serial mon:stdio \
    --no-reboot \
    -kernel "$kernel"
}
