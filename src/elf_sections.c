//
// Created by sbancuz on 12/22/22.
//
#include <stdio.h>
#include <stdlib.h>
#include "headers/elf_sections.h"

void print_symtab(Elf64_Sym *sym);

ElfW_Sec get_section(FILE *src, const ElfW(Shdr) *hdr) {
    ElfW_Sec sec = {0};
    switch (hdr->sh_type) {
        case SHT_NULL:
            break;
        case SHT_PROGBITS:
            break;
        case SHT_SYMTAB:
            sec.sym = calloc(hdr->sh_size / sizeof(ElfW(Sym)), sizeof(ElfW(Sym)));
            for(size_t i = 0; i < hdr->sh_size / sizeof(ElfW(Sym)); i++) {
                sec.sym[i] = get_section_symtab(src, hdr->sh_offset + sizeof(ElfW(Sym)) * i);
            }
            break;
        case SHT_STRTAB:
            break;
        case SHT_RELA:
            break;
        case SHT_HASH:
            break;
        case SHT_DYNAMIC:
            break;
        case SHT_NOTE:
            sec.note = get_section_note(src, hdr->sh_offset);
            break;
        case SHT_NOBITS:
            break;
        case SHT_REL:
            break;
        case SHT_SHLIB:
            break;
        case SHT_DYNSYM:
            break;
        case SHT_INIT_ARRAY:
            break;
        case SHT_FINI_ARRAY:
            break;
        case SHT_PREINIT_ARRAY:
            break;
        case SHT_GROUP:
            break;
        case SHT_SYMTAB_SHNDX:
            break;
        case SHT_NUM:
            break;
        case SHT_LOOS:
            break;
    }

    return sec;
}

void print_section(const ElfW_Sec *sec, const ElfW(Shdr) *hdr, const char *name) {
    switch (hdr->sh_type) {
        case SHT_NULL:
            break;
        case SHT_PROGBITS:
            break;
        case SHT_SYMTAB:
            printf("Num:           Value: Size:   Type:  Assoc:       Vis: Ind: Name:\n");
            for (size_t i = 0; i < hdr->sh_size / sizeof(ElfW(Sym)); i++) {
                printf("%3zu: ", i);
                print_symtab(&sec->sym[i]);
            }
            break;
        case SHT_STRTAB:
            break;
        case SHT_RELA:
            break;
        case SHT_HASH:
            break;
        case SHT_DYNAMIC:
            break;
        case SHT_NOTE:
            print_note(&sec->note, name);
            printf("\n");
            break;
        case SHT_NOBITS:
            break;
        case SHT_REL:
            break;
        case SHT_SHLIB:
            break;
        case SHT_DYNSYM:
            break;
        case SHT_INIT_ARRAY:
            break;
        case SHT_FINI_ARRAY:
            break;
        case SHT_PREINIT_ARRAY:
            break;
        case SHT_GROUP:
            break;
        case SHT_SYMTAB_SHNDX:
            break;
        case SHT_NUM:
            break;
        case SHT_LOOS:
            break;
    }
}

void free_section(ElfW_Sec *sec, const ElfW(Shdr) *hdr) {
    switch (hdr->sh_type) {
        case SHT_NULL:
            break;
        case SHT_PROGBITS:
            break;
        case SHT_SYMTAB:
            free(sec->sym);
            break;
        case SHT_STRTAB:
            break;
        case SHT_RELA:
            break;
        case SHT_HASH:
            break;
        case SHT_DYNAMIC:
            break;
        case SHT_NOTE:
            free(sec->note.name);
            free(sec->note.desc);
            break;
        case SHT_NOBITS:
            break;
        case SHT_REL:
            break;
        case SHT_SHLIB:
            break;
        case SHT_DYNSYM:
            break;
        case SHT_INIT_ARRAY:
            break;
        case SHT_FINI_ARRAY:
            break;
        case SHT_PREINIT_ARRAY:
            break;
        case SHT_GROUP:
            break;
        case SHT_SYMTAB_SHNDX:
            break;
        case SHT_NUM:
            break;
        case SHT_LOOS:
            break;
    }
}

void print_note(const ElfW_Note *note, const char *name) {
    // TODO: Properly print all the note info
    printf("%s:\n", name);
    printf("Note:\n");

    printf("\tNamesz:        %zu\n", note->namesz);
    printf("\tName:          %s\n", note->name);
    printf("\tDescsz:        %zu\n", note->descsz);
    printf("\tDesc:          %s\n", note->desc);
    printf("\tType:          %zu\n", note->type);
}


// https://docs.oracle.com/cd/E23824_01/html/819-0690/chapter6-18048.html
ElfW_Note get_section_note(FILE *src, ElfW(Off) off) {
    // points to the beginning of the *note* section
    fseek(src, off, SEEK_SET);
    ElfW_Note note = {0};
    char c = '\0';

    // namesz 4-bytes
    read_bytes(src, c, 1);
    fseek(src, 3, SEEK_CUR);
    note.namesz = (size_t)c;

    // descsz 4-bytes
    read_bytes(src, c, 1);
    fseek(src, 3, SEEK_CUR);
    note.descsz = (size_t)c;

    // type 4-bytes
    read_bytes(src, c, 1);
    fseek(src, 3, SEEK_CUR);
    note.type = (size_t)c;

    int align = 0;
    // name
    note.name = calloc(1, note.namesz);
    for (size_t i = 0; i < note.namesz; i++, align++ % 4) {
        read_bytes(src, note.name[i], 1);
    }
    // keep 4 bytes aligned
    if (align != 0) {
        fseek(src, 4 - align, SEEK_CUR);
    }

    align = 0;
    // desc
    note.desc = calloc(1, note.descsz);
    for (size_t i = 0; i < note.descsz; i++, align++ % 4) {
        read_bytes(src, note.desc[i], 1);
    }
    // keep 4 bytes aligned
    if (align != 0) {
        fseek(src, 4 - align, SEEK_CUR);
    }
    print_note(&note, "ciao\0");

    return note;
}

// https://docs.oracle.com/en/operating-systems/solaris/oracle-solaris/11.4/linkers-libraries/symbol-table-section.html
ElfW(Sym) get_section_symtab(FILE *src, ElfW(Off) off) {
    // points to the beginning of the *symtab* section
    fseek(src, off, SEEK_SET);
    ElfW(Sym) sym = {0};

    read_bytes(src, sym.st_name, ElfWs);
    read_bytes(src, sym.st_value, ElfWs);
    read_bytes(src, sym.st_size, ElfWs);
    read_bytes(src, sym.st_info, 1);
    read_bytes(src, sym.st_other, 1);
    // Clion has an outdated ELF.h version...
    read_bytes(src, sym.st_shndx, sizeof (sym.st_shndx));

    return sym;
}

void print_symtab(ElfW(Sym) *sym) {
    // TODO: Properly print all the .symtab info
    printf("%016lx  %4zu %7d %7d %10d  %3d %d\n",
           sym->st_value, sym->st_size, ELFW(ST_TYPE(sym->st_info)),
           ELFW(ST_BIND(sym->st_info)), sym->st_other, sym->st_shndx, sym->st_name);
}