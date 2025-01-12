use core::arch::asm;

const CONSOLE_PUTCHAR_EXTENSION: usize = 0x01;
const SYSTEM_RESET_EXTENSION: usize = 0x53525354;

#[inline]
fn sbi_call(extension: usize, function: usize, arg0: usize, arg1: usize) -> (isize, isize) {
    let (error, value);
    unsafe {
        asm!(
            "ecall",
            inlateout("a0") arg0 => error,
            inlateout("a1") arg1 => value,
            in("a6") function,
            in("a7") extension,
        )
    }
    (error, value)
}

/// Write data present in `char` to debug console.
#[inline]
pub fn console_putchar(char: usize) {
    sbi_call(CONSOLE_PUTCHAR_EXTENSION, 0, char, 0);
}

#[inline]
pub fn shutdown() -> ! {
    //    info!("shutdown");
    sbi_call(SYSTEM_RESET_EXTENSION, 0, 0, 0);
    panic!("failed to shutdown");
}
