
#include <efi.h>
#include <efilib.h>

#include "elf.h"
#include "boot.h"

void jump_kernel(EFI_PHYSICAL_ADDRESS address, boot_t *parameters)
{
    void (*kernel_main)(boot_t *) = (void(*)(boot_t *)) address;

    kernel_main(parameters);

/*    __asm__ __volatile__ ("push %1\n\t"
                            "call *%0"
                            :
                            : "r" (address), "r" (parameters)
                            : );*/
}

EFI_STATUS exit_boot(EFI_HANDLE handle)
{
    EFI_MEMORY_DESCRIPTOR *mem_map = NULL;
    EFI_STATUS status;

    UINTN map_size = 0;
    UINTN map_key;
    UINTN desc_size;
    UINT32 desc_ver;

    uefi_call_wrapper(BS->GetMemoryMap, 5,
                      &map_size, mem_map,
                      NULL, &desc_size, NULL);

    map_size += desc_size;
    
    status = uefi_call_wrapper(BS->AllocatePool, 3,
                               EfiLoaderData,
                               map_size,
                               (void **)&mem_map);
    CHK_STATUS (status, L"mem_map AllocatePool fail\r\n");
    
    status = uefi_call_wrapper(BS->GetMemoryMap, 5,
                               &map_size, mem_map,
                               &map_key, &desc_size,
                               &desc_ver);

    CHK_STATUS (status, L"get mem_map failed\r\n");

    //Print makes ExitBootServices fail ??
    //Print(L"About to exit boot services!\r\n");

    status = uefi_call_wrapper(BS->ExitBootServices, 2, handle, map_key);
    CHK_STATUS (status, L"ExitBootServices failed\r\n");

    return (EFI_SUCCESS);
}

EFI_STATUS start_kernel(EFI_HANDLE handle)
{
    EFI_STATUS status;
    EFI_PHYSICAL_ADDRESS kernel_entry_pt;

    boot_t *data;

    
    status = load_kernel(handle, &kernel_entry_pt);
    CHK_FWD (status);

    status = uefi_call_wrapper(BS->AllocatePool, 3,
                               EfiLoaderData,
                               sizeof(boot_t),
                               (void **)&data);
    Print(L"Allocated boot data buf at 0x%llx\r\n", data);
    data->sig = 0x42;

    exit_boot(handle);
    jump_kernel(kernel_entry_pt, data);

    return (status);
}

void print_debug_efi_header(void)
{
    Print(L"EFI System Table Info\r\n"          \
          "   Signature: 0x%lx\r\n"             \
          "   UEFI Revision: 0x%08x\r\n"        \
          "   Header Size: %u Bytes\r\n"        \
          "   CRC32: 0x%08x\r\n"                \
          "   Reserved: 0x%x\r\n"               \
          "   Firmware Vendor: %s\r\n"          \
          "   Firmware Revision: 0x%08x\r\n\n",

          ST->Hdr.Signature,
          ST->Hdr.Revision,
          ST->Hdr.HeaderSize,
          ST->Hdr.CRC32,
          ST->Hdr.Reserved,
          ST->FirmwareVendor,
          ST->FirmwareRevision);
}

EFIAPI
EFI_STATUS
efi_main(EFI_HANDLE handle, EFI_SYSTEM_TABLE *system)
{
    InitializeLib(handle, system);
    EFI_STATUS status;

    status = uefi_call_wrapper(ST->ConOut->ClearScreen, 1, ST->ConOut);
    CHK_FWD (status);

    Print(L"----OS S kernel bootloader----\r\n");
    print_debug_efi_header();

    status = start_kernel(handle);

    // if code gets up to here, the bootloader failed
    CHK_STATUS (status, L"Error starting the kernel, got code : 0x%04llx\r\n", status);

    while (1) // DEBUG: if crash wait
        __asm__ __volatile__("hlt");
    return (status);
}
