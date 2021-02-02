
#include <efi.h>
#include <efilib.h>

#include "elf.h"
#include "boot.h"

EFI_STATUS get_kernel(EFI_HANDLE handle, EFI_FILE **kernel_ptr)
{
    EFI_STATUS status;
    EFI_HANDLE_PROTOCOL proto = BS->HandleProtocol;
    EFI_OPEN_PROTOCOL open = BS->OpenProtocol;
    EFI_LOADED_IMAGE_PROTOCOL *loaded;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *fs;

    status = uefi_call_wrapper(proto, 3,
                               handle,
                               &gEfiLoadedImageProtocolGuid,
                               (void **) &loaded);
    CHK_STATUS (status, L"Error loading the Loaded Image Protocol\r\n");

    status = uefi_call_wrapper(open, 6,
                               loaded->DeviceHandle,
                               &gEfiSimpleFileSystemProtocolGuid,
                               (void **) &fs,
                               handle,
                               NULL,
                               EFI_OPEN_PROTOCOL_GET_PROTOCOL);

    CHK_STATUS (status, L"Error loading the FileSystem\r\n");

    
    CHAR16 *loc = ((FILEPATH_DEVICE_PATH*)loaded->FilePath)->PathName;
    CHAR16 *kernel_path = L"\\EFI\\BOOT\\OS_S\\OS_S_KERNEL";
    EFI_FILE *root;
    EFI_FILE *kernel_file;

    Print(L"Bootloader FilePath: %s\r\n", loc);
    Print(L"Kernel FilePath: %s\r\n", kernel_path);

    status = uefi_call_wrapper(fs->OpenVolume, 2,
                               fs, &root);

    CHK_STATUS (status, L"Error opening current Volume\r\n");

    status = uefi_call_wrapper(root->Open, 5,
                               root,
                               &kernel_file,
                               kernel_path,
                               EFI_FILE_MODE_READ,
                               EFI_FILE_READ_ONLY);

    CHK_STATUS (status, L"Could not find kernel file\r\n");

    *kernel_ptr = kernel_file;

    return (EFI_SUCCESS);
}

EFI_STATUS get_file_info(EFI_FILE *file, EFI_FILE_INFO **info_ptr)
{
    EFI_STATUS status;
    UINTN info_size;
    EFI_FILE_INFO *file_info;

    if (!file)
        return (0);

    uefi_call_wrapper(file->GetInfo, 4,
                      file,
                      &gEfiFileInfoGuid,
                      &info_size,
                      NULL);

    status = uefi_call_wrapper(BS->AllocatePool, 3,
                               EfiLoaderData,
                               info_size,
                               (void **) &file_info);

    CHK_STATUS (status, L"Allocate Pool failed (file info data)\r\n");
    
    status = uefi_call_wrapper(file->GetInfo, 4,
                               file,
                               &gEfiFileInfoGuid,
                               &info_size,
                               file_info);

    CHK_STATUS (status, L"Could not load file info data\r\n");

    Print(L"------------------\r\n");
    Print(L"FileName: %s\r\n", file_info->FileName);
    Print(L"InfoSize: %llu\r\n", file_info->Size);
    Print(L"FileSize: %llu\r\n", file_info->FileSize);
    Print(L"PhysicalSize: %llu\r\n", file_info->PhysicalSize);
    Print(L"Attribute: %llx\r\n", file_info->Attribute);
    Print(L"------------------\r\n");

    *info_ptr = file_info;

    return (EFI_SUCCESS);
}
