use core::arch::asm;

// https://github.com/riscv-non-isa/riscv-sbi-doc/blob/master/src/ext-debug-console.adoc#function-listing
const EXTENSION_DEBUG_CONSOLE: usize = 0x4442434e;
const FUNCTION_CONSOLE_WRITE_BYTE: usize = 0x02;

// https://github.com/riscv-non-isa/riscv-sbi-doc/blob/master/src/ext-sys-reset.adoc#function-system-reset-fid-0
const EXTENSION_SYSTEM_RESET: usize = 0x53525354;
const SYSTEM_RESET_TYPE_SHUTDOWN: usize = 0x00;
const SYSTEM_RESET_REASON_NONE: usize = 0x00;

#[allow(dead_code)]
struct Ret {
    error: isize,
    value: isize,
}

#[inline]
fn ecall(
    extension: usize,
    function: usize,
    arg0: usize,
    arg1: usize,
    arg2: usize,
    arg3: usize,
    arg4: usize,
    arg5: usize,
) -> Ret {
    let (error, value);
    unsafe {
        asm!(
            "ecall",
            inlateout("a0") arg0 => error,
            inlateout("a1") arg1 => value,
            in("a2") arg2,
            in("a3") arg3,
            in("a4") arg4,
            in("a5") arg5,
            in("a6") function,
            in("a7") extension,
        )
    }
    Ret { error, value }
}

#[inline]
fn debug_console(function: usize, arg0: usize, arg1: usize, arg2: usize) -> Ret {
    ecall(EXTENSION_DEBUG_CONSOLE, function, arg0, arg1, arg2, 0, 0, 0)
}
#[inline]
fn sys_reset(typ: usize, reason: usize) -> Ret {
    ecall(EXTENSION_SYSTEM_RESET, 0, typ, reason, 0, 0, 0, 0)
}

/// Write data present in `char` to debug console.
#[inline]
pub fn console_putchar(char: usize) {
    debug_console(FUNCTION_CONSOLE_WRITE_BYTE, char, 0, 0);
}

#[inline]
pub fn shutdown() -> ! {
    sys_reset(SYSTEM_RESET_TYPE_SHUTDOWN, SYSTEM_RESET_REASON_NONE);
    panic!("failed to shutdown");
}
