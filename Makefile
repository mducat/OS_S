
KERNEL		=		OS_S_kernel
BOOTLOADER	=		bootx64.efi

FILES		=		files/output

ISO_NAME	=		system.iso
ISO_DIR		=		iso
FAT_NAME	=		system.img
FAT_DIR		=		efi
BIOS		=		bootloader/OVMF.fd

VMFLAGS		=		-bios $(BIOS) -cdrom $(ISO_NAME)

all:
	$(MAKE) -C oss_lib
	$(MAKE) -C files

	$(MAKE) -C bootloader
	$(MAKE) -C kernel

clean:
	$(MAKE) -C oss_lib    clean
	$(MAKE) -C files      clean

	$(MAKE) -C bootloader clean
	$(MAKE) -C kernel     clean

	rm -f  $(FAT)
	rm -f  $(ISO_NAME)
	rm -rf $(ISO_DIR)

fclean: 
	$(MAKE) -C oss_lib    fclean
	$(MAKE) -C files      fclean

	$(MAKE) -C bootloader fclean
	$(MAKE) -C kernel     fclean

	rm -f  $(FAT_NAME)
	rm -f  $(ISO_NAME)
	rm -rf $(FAT_DIR)
	rm -rf $(ISO_DIR)

re:	fclean all


##################
#### VM RULES ####
##################


vm:	iso
	qemu-system-x86_64 $(VMFLAGS) -serial stdio -enable-kvm

monitor:	iso
	qemu-system-x86_64 $(VMFLAGS) -monitor stdio -serial file:out.dbg

# this one is accessible through gdb with gdb -ex 'target remote localhost:1234'
# does not start CPU at startup
debug:
	$(MAKE) -C oss_lib    debug
	$(MAKE) -C files      debug      

	$(MAKE) -C bootloader 
	$(MAKE) -C kernel     debug
	$(MAKE) iso
	qemu-system-x86_64 $(VMFLAGS) -monitor stdio -serial file:out.dbg -S -s


###################
#### ISO BUILD ####
###################


iso:	img
	mkdir -p $(ISO_DIR)
	mv $(FAT_NAME) $(ISO_DIR)
	xorriso -as mkisofs -R -f -e $(FAT_NAME) -no-emul-boot -o $(ISO_NAME) $(ISO_DIR) -volid "OS_S" 2> /dev/null

# 5760
img:	all
	mkdir -p $(FAT_DIR)/boot $(FAT_DIR)/OS_S
	cp bootloader/$(BOOTLOADER) $(FAT_DIR)/boot/$(BOOTLOADER)
	cp kernel/$(KERNEL) $(FAT_DIR)/OS_S/$(KERNEL)
	cp $(FILES)/* $(FAT_DIR)/OS_S/

	dd if=/dev/zero of=$(FAT_NAME) bs=1k count=2880 2> /dev/null

	mformat -i $(FAT_NAME) -f 2880 ::
	mcopy -i $(FAT_NAME) -s $(FAT_DIR) ::/
	mattrib -i $(FAT_NAME) -a ::/efi/OS_S/$(KERNEL)
	mlabel -i $(FAT_NAME) ::/:"OS_S"


.PHONY: img iso

#################
#### INSTALL ####
#################


install:
	$(MAKE) -C bootloader install
	$(MAKE) -C kernel     install

uninstall:
	$(MAKE) -C bootloader uninstall
	$(MAKE) -C kernel     uninstall
