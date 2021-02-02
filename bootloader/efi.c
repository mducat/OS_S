
#include <efi.h>
#include <efilib.h>

#include "elf.h"
#include "loader.h"

void jump_kernel(void *address, void *parameters)
{
    void (*kernel_main)(void *) = (void(*)(void *)) (address);

    kernel_main(parameters);

    __asm__ __volatile__ ("push %1\n\t"
                          "call *%0"
                          :
                          : "r" (address), "r" (parameters)
                          : );
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

UINT8 compare(const void *a, const void *b, UINT64 len)
{
    const UINT8 *one = a, *two = b;

    for (UINT64 i = 0; i < len; i++)
        if (one[i] != two[i])
            return (1);
    return (0);
}

UINT8 check_header(Elf64_Ehdr *hdr)
{
    if (compare(&hdr->e_ident[EI_MAG0], ELFMAG, SELFMAG)) {
        Print(L"Kernel file read not ELF\r\n");
        return (1);
    }
    
    if (hdr->e_ident[EI_CLASS] != ELFCLASS64 || hdr->e_machine != EM_X86_64) {
        Print(L"Kernel not x86_64!\r\n");
        return (1);
    }
    
    if (hdr->e_type != ET_DYN) {
        Print(L"Kernel not PIE!\r\n");
        return (1);
    }
    
    return (0);
}

UINT8 v_memz(UINT64 addr, UINT64 size)
{
    const UINT8 *cur = (UINT8 *)addr;

    for (UINT64 i = 0; i < size; i++)
        if (cur[i] != 0)
            return (1);
    return (0);
}

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

EFI_STATUS start_kernel(EFI_HANDLE handle)
{
    EFI_STATUS status;
    EFI_FILE *kernel_file;
    Elf64_Ehdr hdr;
    UINTN size = sizeof(hdr);

    EFI_FILE_INFO *k_info;

    status = get_kernel(handle, &kernel_file);
    CHK_FWD (status);

    status = get_file_info(kernel_file, &k_info); // print debug info
    CHK_FWD (status);

    status = uefi_call_wrapper(BS->FreePool, 1, k_info);
    CHK_STATUS (status, L"Kernel info file free fail\r\n");
    
    Print(L"Hdr size : %lld\r\n", size);
    status = uefi_call_wrapper(kernel_file->Read, 3,
                               kernel_file,
                               &size,
                               &hdr);

    CHK_STATUS (status, L"Kernel read failed\r\n");
    if (check_header(&hdr))
        return (EFI_LOAD_ERROR);

    Print(L"Kernel header checked.\r\n");
    Print(L"Headers count: %u\r\n", hdr.e_phnum);
    Print(L"Header size: %u, Phdr_size = %u\r\n", hdr.e_phentsize, sizeof(Elf64_Phdr));
    Print(L"Headers tot size: %llu\r\n", hdr.e_phnum * hdr.e_phentsize);
    Print(L"hdr.e_phoff: %d\r\n", hdr.e_phoff);

    Elf64_Phdr *h_table;

    size = hdr.e_phnum * hdr.e_phentsize;
    status = uefi_call_wrapper(BS->AllocatePool, 3,
                               EfiBootServicesData,
                               size,
                               (void **) &h_table);
    CHK_STATUS (status, L"AllocatePool failed (elf header read)\r\n");

    status = uefi_call_wrapper(kernel_file->SetPosition, 2,
                               kernel_file,
                               hdr.e_phoff);
    CHK_STATUS (status, L"Kernel setpos failed\r\n");

    status = uefi_call_wrapper(kernel_file->Read, 3,
                               kernel_file,
                               &size,
                               &h_table[0]);
    CHK_STATUS (status, L"Kernel read failed\r\n");

    UINT64 v_min  = ~ 0ULL;
    UINT64 v_size =   0ULL;

    for (UINT64 i = 0; i < hdr.e_phnum; i++) {
        Elf64_Phdr *p_header = &h_table[i];

        if (p_header->p_type == PT_LOAD) {

            Elf64_Addr adr = p_header->p_vaddr;
            UINT64 ofs     = adr + p_header->p_memsz;

            v_size = (v_size > ofs ? v_size : ofs);
            v_min  = (v_min  < adr ? v_min  : adr);
        }
        
        //Print(L"Header %d is type : 0x%llx\r\n", i, p_header->p_type);
    }

    UINT64 k_pages = EFI_SIZE_TO_PAGES(v_size - v_min); // << 12 => 4096 = 1 page
    EFI_PHYSICAL_ADDRESS kernel_loc;

    status = uefi_call_wrapper(BS->AllocatePages, 4,
                               AllocateAnyPages,
                               EfiLoaderData,
                               k_pages,
                               &kernel_loc);
    CHK_STATUS (status, L"Could not alloc memory space for the kernel.\r\n");

    Print(L"Allocated kernel space at : 0x%llx of %u pages\r\n", kernel_loc, k_pages);

    ZeroMem((void*) kernel_loc, (k_pages << EFI_PAGE_SHIFT));

    if (v_memz(kernel_loc, k_pages << EFI_PAGE_SHIFT))
        TRIGGER (EFI_UNSUPPORTED, L"Firware buggy ? need mem check !\r\n");

    for (UINT64 i = 0; i < hdr.e_phnum; i++) {

        Elf64_Phdr *p_header = &h_table[i];
        Elf64_Xword data_size = p_header->p_filesz;

        // virtual to physical conversion
        EFI_PHYSICAL_ADDRESS m_seg_loc = kernel_loc + p_header->p_vaddr; 

        if (!data_size)
            continue;
                    
        status = uefi_call_wrapper(kernel_file->SetPosition, 2,
                                   kernel_file,
                                   p_header->p_offset);
        CHK_STATUS (status, L"Kernel setpos to seg failed\r\n");

        if (p_header->p_type == PT_LOAD) {

            Print(L"Check:\r\nSectionAddress: 0x%llx\r\nData there: 0x%016llx%016llx (should be 0)\r\n", m_seg_loc, *(EFI_PHYSICAL_ADDRESS*)(m_seg_loc + 8), *(EFI_PHYSICAL_ADDRESS *)m_seg_loc);
            
            status = uefi_call_wrapper(kernel_file->Read, 3,
                                       kernel_file,
                                       &data_size,
                                       (void *) m_seg_loc);
            CHK_STATUS (status, L"Kernel memory load failed\r\n");

            Print(L"loaded %llu at 0x%llx\r\n", data_size, m_seg_loc);

        } else if (p_header->p_type == PT_DYNAMIC) {

            Print(L"Found dynamic segment\r\n");
            // oof
            
            if (p_header->p_memsz != data_size) // abort, should not be possible
                TRIGGER (EFI_LOAD_ERROR, L"PT_DYN ELF segment read error\r\n");

            Elf64_Dyn *dyn_arr;

            status = uefi_call_wrapper(BS->AllocatePool, 3,
                                       EfiBootServicesData,
                                       data_size,
                                       (void **) &dyn_arr);
            CHK_STATUS (status, L"AllocatePool failed (dynamic entries array)\r\n");

            status = uefi_call_wrapper(kernel_file->Read, 3,
                                       kernel_file,
                                       &(p_header->p_filesz),
                                       (void *) dyn_arr);
            CHK_STATUS (status, L"Kernel dynamic relocation segment read failed\r\n");

            
            Elf64_Rela *r_table = (Elf64_Rela *) ~0ULL;
            UINT64 t_size = 0;
            UINT64 t_entry_size = 0;

            // lookup relocation info
            for (Elf64_Dyn *entry = dyn_arr; entry < (Elf64_Dyn *)(dyn_arr + data_size); entry++) {

                switch (entry->d_tag) {

                case DT_RELA:
                    r_table = (Elf64_Rela *) (kernel_loc + entry->d_un.d_ptr);
                    break;

                case DT_RELASZ:
                    t_size = entry->d_un.d_val;
                    break;

                case DT_RELAENT:
                    t_entry_size = entry->d_un.d_val;
                    break;

                default: break;
                }
                
            }

            Print(L"Reloc data: 0x%llx size: %u  ent_size: %u\r\n", r_table, t_size, t_entry_size);

            if (r_table == (Elf64_Rela *) ~0ULL)
                continue; // no reloc to do

            if (!t_size || !t_entry_size)
                TRIGGER (EFI_LOAD_ERROR, L"Incomplete ELF relocation table\r\n");

            UINT64 n_rela = t_size / t_entry_size;

            for (UINT64 j = 0; j < n_rela; j++) {

                if (ELF64_R_TYPE(r_table[j].r_info) != R_X86_64_RELATIVE)
                    TRIGGER (EFI_UNSUPPORTED, L"Unsupported relocation type\r\n");

                *(UINT64 *)(kernel_loc + r_table[j].r_offset) = kernel_loc + r_table[j].r_addend;
            }

            status = uefi_call_wrapper(BS->FreePool, 1, r_table);
            CHK_STATUS (status, L"Dynamic Relocation table free fail\r\n");
        }
    }

    status = uefi_call_wrapper(BS->FreePool, 1, h_table);
    CHK_STATUS (status, L"ELF program header free fail\r\n");

    EFI_PHYSICAL_ADDRESS kernel_entry_pt = kernel_loc + hdr.e_entry;
    Print(L"Kernel loaded ! Entry point at 0x%llx\r\n", kernel_entry_pt);

    Print(L"Data at krnl (first 16 bytes): 0x%016llx%016llx\r\n", *(EFI_PHYSICAL_ADDRESS*)(kernel_entry_pt + 8), *(EFI_PHYSICAL_ADDRESS*)kernel_entry_pt);
    
    exit_boot(handle);
    jump_kernel((void *)kernel_entry_pt, (void *) 0x43);

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
 
    CHK_STATUS (status, L"Error starting the kernel, got code : 0x%04llx\r\n", status);
    
    Print(L"Success !\r\n");
    while (1) // DEBUG: if crash wait
        __asm__ __volatile__("hlt");
    return (status);
}
