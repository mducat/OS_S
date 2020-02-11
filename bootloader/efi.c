/*
** EPITECH PROJECT, 2019
** SHIELD_OS
** File description:
** UEFI bootloader
*/

#include <efi.h>
#include <efilib.h>

void call_kernel(void)
{
    __asm__("cli\n"                               \
            "mov %rsi, %rbx\n"                    \
            "jmp 0x1000000\n");
}

void exit_boot(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE *systemTable)
{
    EFI_STATUS result;
    UINTN mapSize = 0, mapKey, descriptorSize;
    EFI_MEMORY_DESCRIPTOR *memoryMap = NULL;
    UINT32 descriptorVersion;

    result = uefi_call_wrapper((void *)systemTable->BootServices->GetMemoryMap,
                               5, &mapSize, &memoryMap, NULL, &descriptorSize,
                               NULL);
    ErrorCheck(result, EFI_BUFFER_TOO_SMALL);

    mapSize += 2 * descriptorSize;

    result = uefi_call_wrapper((void *)systemTable->BootServices->AllocatePool,
                               3, EfiLoaderData, mapSize, (void **)&memoryMap);
    ErrorCheck(result, EFI_SUCCESS);

    result = uefi_call_wrapper((void *)systemTable->BootServices->GetMemoryMap,
                               5, &mapSize, &memoryMap, &mapKey, &descriptorSize,
                               &descriptorVersion);
    ErrorCheck(result, EFI_SUCCESS);

    result = uefi_call_wrapper((void *)systemTable->BootServices->ExitBootServices,
                               2, imageHandle, mapKey);
    ErrorCheck(result, EFI_SUCCESS);
}

void load_kernel(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
    EFI_HANDLE_PROTOCOL get_protocol = SystemTable->BootServices->HandleProtocol;
    EFI_GUID image_protocol_guid = EFI_LOADED_IMAGE_PROTOCOL_GUID;
    EFI_GUID filesystem_guid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
    EFI_LOADED_IMAGE_PROTOCOL *loaded_image;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *fs;
    EFI_FILE *root;
    EFI_STATUS status;
    EFI_FILE *kernel;

    Print(L"load_kernel()\n");
    uefi_call_wrapper(get_protocol, 3, ImageHandle, &image_protocol_guid, (void **)&loaded_image);
    Print(L"loading protocols (1/2)\n");
    get_protocol(loaded_image->DeviceHandle, &filesystem_guid, (void **)&fs);
    Print(L"loading protocols (2/2)\n");
    fs->OpenVolume(fs, &root);
    Print(L"Volume opened, root set.\n");
    status = root->Open(root, &kernel, L"shield_kernel", EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);
    if (status != EFI_SUCCESS)
        Print(L"Could not load kernel file !\n");
    Print(L"Kernel loaded.\n");
}

EFI_STATUS
EFIAPI
efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
    InitializeLib(ImageHandle, SystemTable);
    SystemTable->ConOut->ClearScreen(SystemTable->ConOut);

    Print(L"Loading SHIELD Kernel....\n");
    load_kernel(ImageHandle, SystemTable);

    Print(L"Kernel loaded. Exiting boot...\n");
    exit_boot(ImageHandle, SystemTable);

    Print(L"Boot sucessfully exited. Starting kernel...\n");
    call_kernel();

    return EFI_SUCCESS;
}
