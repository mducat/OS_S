/*
** EPITECH PROJECT, 2019
** SHIELD_OS
** File description:
** Address pointer modifier
*/

.set MB_MAGIC, 0x1badb002
.set MB_FLAGS, 0x0
.set MB_CHECKSUM, -(MB_MAGIC + MB_FLAGS)
    
.section .text
.extern kernel_main         // entry point

.align 4
.int    MB_MAGIC
.int    MB_FLAGS
.int    MB_CHECKSUM

.global _start
_start:
    // init stack
    mov $kernel_stack, %esp

    call kernel_main

_stop:                          // in case the kernel stop, wait
    cli
    hlt
    jmp _stop

// space for the mutliboot header
.section bss
.space 8192
kernel_stack:   
