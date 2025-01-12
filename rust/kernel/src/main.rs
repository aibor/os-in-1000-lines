#![no_std]
#![no_main]
#![feature(naked_functions)]

#[macro_use]
mod sbi;

use core::arch::global_asm;

use core::panic::PanicInfo;

#[panic_handler]
fn panic(_panic: &PanicInfo<'_>) -> ! {
    let message = _panic.message();
    let string = message.as_str().unwrap();
    for char in string.bytes() {
        sbi::console_putchar(char.into());
    }
    loop {}
}

global_asm!(include_str!("boot.asm"));

#[no_mangle]
pub fn kernel_main() {
    clear_bss();

    let string = "booted\n";

    for char in string.bytes() {
        sbi::console_putchar(char.into());
    }

    sbi::shutdown();
}

fn clear_bss() {
    extern "C" {
        fn __bss();
        fn __bss_end();
    }

    (__bss as usize..__bss_end as usize)
        .for_each(|address| unsafe { (address as *mut u8).write_volatile(0) })
}
