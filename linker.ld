ENTRY(kernel_main)

SECTIONS {
    /* Start the kernel at the address 1MB */
    . = 0x100000;

    /* Multiboot header section */
    .multiboot_header : {
        *(.multiboot)  /* Include the .multiboot section (the header) */
    }

    /* Code section (.text) */
    .text : {
        *(.text)  /* All the code from the .text section */
    }

    /* Data section (.data) */
    .data : {
        *(.data)  /* All initialized data */
    }

    /* BSS section (.bss) */
    .bss : {
        *(.bss)  /* All uninitialized data */
    }

    /* Ensure that the kernel ends up in memory after the sections */
    . = ALIGN(0x1000);  /* Align to the next page boundary (4KB) for better memory management */

    /* End of the kernel binary (optional, but good for alignment) */
    _end = .;
}
