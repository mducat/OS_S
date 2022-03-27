
#include <efi.h>
#include <efilib.h>

#include "elf.h"
#include "boot.h"

#define MAX_FILE_INFO_SIZE 1024

UINTN cstrlen(CHAR16 *str)
{
    UINTN r = 0;

    while(*str++)
        r++;
    return (r);
}

#define strlen cstrlen
#define size_t uint64_t

//copy names only (CHAR16 to CHAR8)
void *namecpy(void *dest, const void *src, size_t n)
{
    uint8_t *p_dest = (uint8_t *) dest;
    uint8_t *p_src  = (uint8_t *) src;

    while (n--) {
        *p_dest++ = *p_src++;
        p_src++;
    }
    return dest;
}

void *cmemcpy(void *dest, const void *src, size_t n)
{
    uint8_t *p_dest = (uint8_t *) dest;
    uint8_t *p_src  = (uint8_t *) src;

    while (n--)
        *p_dest++ = *p_src++;
    return dest;
}

#define memcpy cmemcpy

void *cmemset(void *dest, char c, size_t size)
{
    uint8_t *ptr = (uint8_t *) dest;

    while (size--)
        *ptr++ = c;
    return dest;
}

#define memset cmemset

EFI_STATUS append_file(file_t ***all_files, file_t *file)
{
    EFI_STATUS status;

    file_t **list = *all_files;
    file_t **new_list;
    UINTN list_len = 0;

    while (list[list_len++]) {
        //Print(list[list_len]->name);
        //Print("\r\n");
    }

    list_len --;

    //Print(L"len:%d\r\n", list_len);
    //Print(L"file: 0x%llx\r\n", file);

    status = uefi_call_wrapper(BS->AllocatePool, 3,
                               EfiLoaderData,
                               sizeof(file_t *) * (list_len + 2),
                               (void **) &new_list);
    CHK_STATUS (status, L"Alloc fail\r\n");

    memset(new_list, 0, sizeof(file_t *) * (list_len + 2));
    memcpy(new_list, list, sizeof(file_t *) * list_len);
    new_list[list_len] = file;
    new_list[list_len + 1] = 0;
    
    status = uefi_call_wrapper(BS->FreePool, 1, (void *) list);
    CHK_STATUS (status, L" free fail\r\n");

    *all_files = new_list;

    return (EFI_SUCCESS);
}

EFI_STATUS load_files(EFI_FILE *root, boot_t *data)
{
    EFI_STATUS status;
    CHAR16 *files_path = L"\\EFI\\OS_S\\";
    EFI_FILE *parent_dir;
    file_t **all_files;

    status = uefi_call_wrapper(root->Open, 5,
                               root,
                               &parent_dir,
                               files_path,
                               EFI_FILE_MODE_READ,
                               EFI_FILE_READ_ONLY);

    CHK_STATUS (status, L"Could not find folder file\r\n");

    status = uefi_call_wrapper(BS->AllocatePool, 3,
                               EfiLoaderData,
                               sizeof(file_t *),
                               (void **) &all_files);
    CHK_STATUS (status, L"Alloc fail\r\n");

    all_files[0] = 0;


    EFI_FILE_INFO *info;

    while (1) {
        UINTN inf_size = MAX_FILE_INFO_SIZE + sizeof(EFI_FILE_INFO);
        file_t *file;
        EFI_FILE *efi_file;

        status = uefi_call_wrapper(BS->AllocatePool, 3,
                                   EfiLoaderData,
                                   inf_size,
                                   (void **) &info);
        CHK_STATUS (status, L"Alloc fail\r\n");

        status = uefi_call_wrapper(parent_dir->Read, 3,
                                   parent_dir,
                                   &inf_size,
                                   (void *) info);
        CHK_STATUS (status, L"Read fail\r\n");

        if (info->FileSize > 1024 * 1024 * 1024)
            break;

        status = uefi_call_wrapper(BS->AllocatePool, 3,
                                   EfiLoaderData,
                                   sizeof(file_t),
                                   (void **) &file);
        CHK_STATUS (status, L"Alloc fail\r\n");

        file->size = info->FileSize;
        UINTN name_size = strlen(info->FileName) * 2 + 2;

        status = uefi_call_wrapper(BS->AllocatePool, 3,
                                   EfiLoaderData,
                                   name_size,
                                   (void **) &file->name);
        CHK_STATUS (status, L"Alloc fail\r\n");

        memset(file->name, 0, name_size);
        namecpy(file->name, info->FileName, name_size - 2);
        file->name[name_size - 1] = 0;
        
        //Print(file->name);
        //Print(L"\r\n");

        status = uefi_call_wrapper(BS->AllocatePool, 3,
                                   EfiLoaderData,
                                   info->FileSize,
                                   (void **) &file->content);
        CHK_STATUS (status, L"Alloc fail\r\n");

        CHAR16 *file_loc;
        UINTN loc_len = strlen(info->FileName) + strlen(files_path) + 1;

        loc_len *= 2;

        status = uefi_call_wrapper(BS->AllocatePool, 3,
                                   EfiLoaderData,
                                   loc_len,
                                   (void **) &file_loc);
        CHK_STATUS (status, L"Alloc fail\r\n");

        memset(file_loc, 0, loc_len);
        memcpy(file_loc, files_path, strlen(files_path) * 2);
        memcpy(file_loc + strlen(files_path), info->FileName, strlen(info->FileName) * 2);

        //Print(file_loc);
        //Print(L"\r\n");

        status = uefi_call_wrapper(root->Open, 5,
                                   root,
                                   &efi_file,
                                   file_loc,
                                   EFI_FILE_MODE_READ,
                                   EFI_FILE_READ_ONLY);

        CHK_STATUS (status, L"open file fail\r\n");

        //status = uefi_call_wrapper(BS->FreePool, 1, (void *)file_loc);
        //CHK_STATUS (status, L" free fail\r\n");
        

        UINTN read_size = info->FileSize;
       
        status = uefi_call_wrapper(efi_file->Read, 3,
                                   efi_file,
                                   &read_size,
                                   (void *) file->content);
        CHK_STATUS (status, L"Read fail\r\n");

        
        /*Print(L"------------------\r\n");
        Print(L"FileName: %s\r\n", info->FileName);
        Print(L"InfoSize: %llu\r\n", info->Size);
        Print(L"FileSize: %llu\r\n", info->FileSize);
        Print(L"PhysicalSize: %llu\r\n", info->PhysicalSize);
        Print(L"Attribute: %llx\r\n", info->Attribute);
        Print(L"------------------\r\n");*/
        
        status = uefi_call_wrapper(root->Close, 1,
                                   efi_file);

        CHK_STATUS (status, L"close file fail\r\n");


        status = uefi_call_wrapper(BS->FreePool, 1, info);
        CHK_STATUS (status, L"info file free fail\r\n");

        status = append_file(&all_files, file);
        CHK_FWD (status);
    }

    data->files = all_files;
    return (EFI_SUCCESS);
}

EFI_STATUS get_kernel(EFI_HANDLE handle, EFI_FILE **kernel_ptr, boot_t *data)
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
    CHAR16 *kernel_path = L"\\EFI\\OS_S\\OS_S_KERNEL";
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

    status = load_files(root, data);
    CHK_FWD(status);

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
