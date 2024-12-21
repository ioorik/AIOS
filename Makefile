all: ai_os.iso

temp/kernel.o: os/src/kernel.c
	mkdir -p temp
	i686-elf-gcc -c -ffreestanding -o temp/kernel.o os/src/kernel.c

temp/multiboot_header.o: os/src/multiboot_header.asm
	i686-elf-as os/src/multiboot_header.asm -o temp/multiboot_header.o

os/boot/kernel.bin: temp/kernel.o temp/multiboot_header.o
	i686-elf-gcc -T linker.ld -o os/boot/kernel.bin -ffreestanding -nostdlib temp/multiboot_header.o temp/kernel.o -lgcc

ai_os.iso: os/boot/kernel.bin os/boot/grub/grub.cfg
	grub-mkrescue -o ai_os.iso os/

clean:
	rm -f kernel.o os/boot/kernel.bin ai_os.iso 
	rm -rf temp

run-kernel:
	qemu-system-i386 -kernel os/boot/kernel.bin -vga std

run:
	qemu-system-x86_64 -cdrom ai_os.iso

run-all: clean all run