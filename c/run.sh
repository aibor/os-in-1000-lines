#!/bin/bash
set -xue

. ../shared/qemu.sh

make kernel

run_qemu kernel
