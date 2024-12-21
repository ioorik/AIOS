.section .multiboot
.align 4
.long 0x1BADB002  # Multiboot header magic number
.long 0b00000100  # Flags
.long -(0x1BADB002 + 0b00000100) # Checksum (magic + flags + checksum == 0)

.section .text
.global kernel_main
