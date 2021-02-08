
ARCH            = $(shell uname -m | sed s,i[3456789]86,ia32,)
TARGET          = bootx64.efi
INSTALLED		= OS_S
OBJS			= src/load.o	\
				  src/files.o	\
				  src/boot.o

EFIINC          = lib/inc
EFIINCS         = -I$(EFIINC) -I$(EFIINC)/$(ARCH) -I$(EFIINC)/protocol
LIB             = lib/$(ARCH)/lib
EFILIB          = lib/$(ARCH)/gnuefi
EFI_CRT_OBJS    = $(EFILIB)/crt0-efi-$(ARCH).o
EFI_LDS         = lib/gnuefi/elf_$(ARCH)_efi.lds

SELFINCS		= -Iinc

CFLAGS          = $(EFIINCS)-fno-stack-protector -fpic \
		  -fshort-wchar -mno-red-zone -Wall $(SELFINCS) -Wextra
ifeq ($(ARCH),x86_64)
  CFLAGS += -DEFI_FUNCTION_WRAPPER
endif

LDFLAGS         = -nostdlib -znocombreloc -T $(EFI_LDS) -shared \
		  -Bsymbolic -L $(EFILIB) -L $(LIB) $(EFI_CRT_OBJS)

all: $(TARGET)

%.so: $(OBJS)
	$(MAKE) -C lib
	ld $(LDFLAGS) $(OBJS) -o $@ -lefi -lgnuefi

%.efi: %.so
	objcopy -j .text -j .sdata -j .data -j .dynamic \
		-j .dynsym  -j .rel -j .rela -j .reloc \
		--target=efi-app-$(ARCH) $^ $@

clean:
	rm -f *.so
	rm -f $(OBJS)

fclean:	clean
	$(MAKE) -C lib clean
	rm -f $(TARGET)

re:	fclean all

install:all
	sudo mkdir -p /boot/EFI/OS_S
	sudo cp $(TARGET) /boot/EFI/OS_S/$(INSTALLED)

uninstall:
	sudo rm /boot/efi/OS_S/$(INSTALLED)
