
#include <types.h>
#include <fs.h>
#include <string.h>
#include <shell.h>

#include <dev/serial.h>

#include <elf.h>

void do_reloc(void *file)
{
    Elf64_Ehdr *hdr = (Elf64_Ehdr *) file;
    Elf64_Phdr *h_table = (Elf64_Phdr *) (file + sizeof(Elf64_Ehdr));

    for (int i = 0; i < hdr->e_phnum; i++) {
        Elf64_Phdr *cur = &h_table[i];

        if (cur->p_type != PT_DYNAMIC)
            continue;

        if (cur->p_memsz != cur->p_filesz)
            // wot
            return;

        Elf64_Dyn *d_table = (Elf64_Dyn *) (file + cur->p_offset);
        Elf64_Dyn *d_end = (Elf64_Dyn *) (d_table + cur->p_filesz);
        Elf64_Rela *r_table = 0;
        size_t t_size = 0;
        size_t t_entry_size = 0;

        for (; d_table < d_end; d_table++) {
            
            switch (d_table->d_tag) {
                
            case DT_RELA:
                r_table = (Elf64_Rela *) (file + d_table->d_un.d_ptr);
                break;
                
            case DT_RELASZ:
                if (t_size == 0)
                    t_size = d_table->d_un.d_val;
                break;
                
            case DT_RELAENT:
                t_entry_size = d_table->d_un.d_val;
                break;
                
            default: break;
            }
            
        }

        if (!r_table)
            continue;

        if (!t_size || !t_entry_size)
            return;

        size_t n_rela = t_size / t_entry_size;

        for (uint64_t i = 0; i < n_rela; i++) {
            
            if (ELF64_R_TYPE(r_table[i].r_info) != R_X86_64_RELATIVE)
                continue;
            
            *(uint64_t *)(file + r_table[i].r_offset) = (uint64_t) file + r_table[i].r_addend;
        }

    }
}

void jump_elf(void *address, int ac, char **av)
{
    void (*entry)(int, char **) = (void(*)(int, char**)) address;

    entry(ac, av);
}

int exec_eoss(void *file, int ac, char **av)
{
    Elf64_Ehdr *hdr = (Elf64_Ehdr *) (file);

    if (hdr->e_ident[EI_CLASS] != ELFCLASS64 ||
        hdr->e_machine != EM_X86_64)
        return 1;

    if (hdr->e_type != ET_DYN)
        return 1;

    do_reloc(file);

    jump_elf(file + hdr->e_entry, ac, av);

    do_reloc(file);

    return (0);
}

int exec(file_t *file, int ac, char **av)
{
    if (strncmp(file->content, OSS_HDR, OSS_HDR_LEN))
        return 1;

    void *hdr_chk = file->content + OSS_HDR_LEN;

    if (!strncmp(hdr_chk, EOSS_HDR, EOSS_HDR_LEN))
        return exec_eoss(hdr_chk, ac, av);

    void (*entry)(int, char **) = (void(*)(int, char**)) hdr_chk;
    entry(ac, av);

    return 0;
}
