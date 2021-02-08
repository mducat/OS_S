
KERNEL		=		OS_S_kernel
BOOTLOADER	=		bootx64.efi

ISO_NAME	=		system.iso
ISO_DIR		=		iso
FAT			=		system.img
BIOS		=		bootloader/OVMF.fd

VMFLAGS		=		-bios $(BIOS) -cdrom $(ISO_NAME)

all:
	$(MAKE) -C bootloader
	$(MAKE) -C kernel

clean:
	$(MAKE) -C bootloader clean
	$(MAKE) -C kernel     clean

	rm -f  $(FAT)
	rm -f  $(ISO_NAME)
	rm -rf $(ISO_DIR)

fclean: clean
	$(MAKE) -C bootloader fclean
	$(MAKE) -C kernel     fclean

re:	fclean all


##################
#### VM RULES ####
##################


vm:	iso
	qemu-system-x86_64 $(VMFLAGS) -serial stdio

monitor:	iso
	qemu-system-x86_64 $(VMFLAGS) -monitor stdio -serial file:out.dbg

# this one is accessible through gdb with gdb -ex 'target remote localhost:1234'
# does not start CPU at startup
debug:
	$(MAKE) -C kernel debug
	$(MAKE) iso
	qemu-system-x86_64 $(VMFLAGS) -monitor stdio -serial file:out.dbg -S -s


###################
#### ISO BUILD ####
###################


iso:	img
	mkdir -p $(ISO_DIR)
	mv $(FAT) $(ISO_DIR)
	xorriso -as mkisofs -R -f -e $(FAT) -no-emul-boot -o $(ISO_NAME) $(ISO_DIR) -volid "OS_S" 2> /dev/null

img:	all
	dd if=/dev/zero of=$(FAT) bs=1k count=1440 2> /dev/null
	mformat -i $(FAT) -f 1440 ::
	mmd -i $(FAT) ::/efi
	mmd -i $(FAT) ::/efi/boot
	mmd -i $(FAT) ::/efi/OS_S
	mcopy -i $(FAT) bootloader/$(BOOTLOADER) ::/efi/boot/$(BOOTLOADER)
	mcopy -i $(FAT) kernel/$(KERNEL) ::/efi/OS_S/$(KERNEL)
	mattrib -i $(FAT) -a ::/efi/OS_S/$(KERNEL)
	mlabel -i $(FAT) ::/:"OS_S"


#################
#### INSTALL ####
#################


install:
	$(MAKE) -C bootloader install
	$(MAKE) -C kernel     install

uninstall:
	$(MAKE) -C bootloader uninstall
	$(MAKE) -C kernel     uninstall
