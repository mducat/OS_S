
#include <efi.h>
#include <efilib.h>

#include "elf.h"
#include "boot.h"

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

UINT64 get_kernel_psize(Elf64_Ehdr *hdr, Elf64_Phdr *h_table)
{
    UINT64 v_min  = ~ 0ULL;
    UINT64 v_size =   0ULL;

    for (UINT64 i = 0; i < hdr->e_phnum; i++) {
        Elf64_Phdr *p_header = &h_table[i];

        if (p_header->p_type == PT_LOAD) {

            Elf64_Addr adr = p_header->p_vaddr;
            UINT64 ofs     = adr + p_header->p_memsz;

            v_size = (v_size > ofs ? v_size : ofs);
            v_min  = (v_min  < adr ? v_min  : adr);
        }
    }

    return EFI_SIZE_TO_PAGES(v_size - v_min); // << 12 => 4096 = 1 page
}

EFI_STATUS load_segment(EFI_FILE *kernel_file, Elf64_Phdr *p_header, EFI_PHYSICAL_ADDRESS kernel_loc)
{
    Elf64_Xword data_size = p_header->p_filesz;
    EFI_STATUS status = EFI_SUCCESS;

    // virtual to physical conversion
    EFI_PHYSICAL_ADDRESS m_seg_loc = kernel_loc + p_header->p_vaddr; 

    if (!data_size)
        return status;

    status = uefi_call_wrapper(kernel_file->SetPosition, 2,
                               kernel_file,
                               p_header->p_offset);
    CHK_STATUS (status, L"Kernel setpos to seg failed\r\n");

    if (p_header->p_type == PT_LOAD) {
        
        status = uefi_call_wrapper(kernel_file->Read, 3,
                                   kernel_file,
                                   &data_size,
                                   (void *) m_seg_loc);
        CHK_STATUS (status, L"Kernel memory load failed\r\n");
        
    } else if (p_header->p_type == PT_DYNAMIC) {

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
                if (t_size == 0)
                    t_size = entry->d_un.d_val;
                break;
                
            case DT_RELAENT:
                t_entry_size = entry->d_un.d_val;
                break;
                
            default: break;
            }
            
        }
        
        if (r_table == (Elf64_Rela *) ~0ULL)
            return status; // no reloc to do
        
        if (!t_size || !t_entry_size)
            TRIGGER (EFI_LOAD_ERROR, L"Incomplete ELF relocation table\r\n");
        
        UINT64 n_rela = t_size / t_entry_size;
        
        for (UINT64 i = 0; i < n_rela; i++) {
            
            if (ELF64_R_TYPE(r_table[i].r_info) != R_X86_64_RELATIVE)
                TRIGGER (EFI_UNSUPPORTED, L"Unsupported relocation type\r\n");
            
            *(UINT64 *)(kernel_loc + r_table[i].r_offset) = kernel_loc + r_table[i].r_addend;
        }
        
        status = uefi_call_wrapper(BS->FreePool, 1, dyn_arr);
        CHK_STATUS (status, L"Dynamic segment free fail\r\n");
    }
    
    return status;
}

EFI_STATUS load_kernel(EFI_HANDLE handle, EFI_PHYSICAL_ADDRESS *entry_point, boot_t *data)
{
    EFI_STATUS status;
    EFI_FILE *kernel_file;
    Elf64_Ehdr hdr;
    UINTN size = sizeof(hdr);

    EFI_FILE_INFO *k_info;

    status = get_kernel(handle, &kernel_file, data);
    CHK_FWD (status);


    status = get_file_info(kernel_file, &k_info); // print debug info
    CHK_FWD (status);

    status = uefi_call_wrapper(BS->FreePool, 1, k_info);
    CHK_STATUS (status, L"Kernel info file free fail\r\n");
    

    status = uefi_call_wrapper(kernel_file->Read, 3,
                               kernel_file,
                               &size,
                               &hdr);

    CHK_STATUS (status, L"Kernel read failed\r\n");

    if (check_header(&hdr))
        return (EFI_LOAD_ERROR);

    
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

    
    UINT64 k_pages = get_kernel_psize(&hdr, h_table);
    EFI_PHYSICAL_ADDRESS kernel_loc;

    status = uefi_call_wrapper(BS->AllocatePages, 4,
                               AllocateAnyPages,
                               EfiLoaderData,
                               k_pages,
                               &kernel_loc);
    CHK_STATUS (status, L"Could not alloc memory space for the kernel.\r\n");

    
    ZeroMem((void*) kernel_loc, (k_pages << EFI_PAGE_SHIFT));

    if (v_memz(kernel_loc, k_pages << EFI_PAGE_SHIFT))
        TRIGGER (EFI_UNSUPPORTED, L"Firware buggy ? need mem check !\r\n");

    
    for (UINT64 i = 0; i < hdr.e_phnum; i++) {
        status = load_segment(kernel_file, &h_table[i], kernel_loc);

        CHK_FWD (status);
    }

    
    status = uefi_call_wrapper(BS->FreePool, 1, h_table);
    CHK_STATUS (status, L"ELF program header free fail\r\n");

    Print(L"%u Kernel pages loaded at 0x%llx\r\n", k_pages, kernel_loc);
    Print(L"Entry point at 0x%llx\r\n", kernel_loc + hdr.e_entry);


    *entry_point = kernel_loc + hdr.e_entry;
    return status;
}
