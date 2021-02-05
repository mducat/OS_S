
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

    status = uefi_call_wrapper(BS->AllocatePool, 3,
                               EfiLoaderData,
                               sizeof(screen_t),
                               (void **)&(data->screen));
    Print(L"Allocated screen info buf at 0x%llx\r\n", data->screen);

    
    
    EFI_GUID gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
 
    status = uefi_call_wrapper(BS->LocateProtocol, 3, &gopGuid, NULL, (void**)&gop);
    CHK_STATUS (status, L"GOP getter failed\r\n");
    
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info;
    UINTN SizeOfInfo;
    UINTN numModes;

    INTN chosen = -1;
    
    status = uefi_call_wrapper(gop->QueryMode, 4, gop, gop->Mode==NULL?0:gop->Mode->Mode, &SizeOfInfo, &info);
    // this is needed to get the current video mode
    if (status == EFI_NOT_STARTED)
        status = uefi_call_wrapper(gop->SetMode, 2, gop, 0);
    if(EFI_ERROR(status)) {
        Print(L"Unable to get native mode");
    } else {
        numModes = gop->Mode->MaxMode;
    }

    for (UINTN i = 0; i < numModes; i++) {
        status = uefi_call_wrapper(gop->QueryMode, 4, gop, i, &SizeOfInfo, &info);
        /*Print(L"mode %3d width %d height %d format %x\r\n",
                i,
                info->HorizontalResolution,
                info->VerticalResolution,
                info->PixelFormat
            );*/
        CHK_STATUS (status, L"Failed to query mode info\r\n");
        if (info->HorizontalResolution == 1920 && info->VerticalResolution == 1080)
            chosen = i;
    }

    if (chosen < 0)
        chosen = numModes - 1;

    status = uefi_call_wrapper(gop->SetMode, 2, gop, chosen);
    CHK_STATUS (status, L"GOP set mode failed\r\n");

    status = uefi_call_wrapper(gop->QueryMode, 4, gop, gop->Mode->Mode, &SizeOfInfo, &info);
    CHK_STATUS (status, L"Query info on current mode failed\r\n");

    data->screen->x_len = info->HorizontalResolution;
    data->screen->y_len = info->VerticalResolution;
    data->screen->format = info->PixelFormat;

    data->screen->version = info->Version;
    data->screen->pix_per_line = info->PixelsPerScanLine;

    data->screen->buf_size = gop->Mode->FrameBufferSize;
    data->screen->p_loc = (void *) gop->Mode->FrameBufferBase;
    
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
    CHK_STATUS (status, L"Error starting the kernel, got code : %E0x%04llx, %r%N\r\n", status, status);

    while (1) // DEBUG: if crash wait
        __asm__ __volatile__("hlt");
    return (status);
}
