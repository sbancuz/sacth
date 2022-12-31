//
// Created by sbancuz on 12/17/22.
//

#ifndef ELF_PATCHER_ELF_SECTION_HEADER_H
#define ELF_PATCHER_ELF_SECTION_HEADER_H

#include <elf.h>
#include <bits/types/FILE.h>

#include "macros.h"
#include "elf_sections.h"

typedef struct elfw_shdr {
    ElfW(Shdr) hdr;
    char name[17];
    ElfW_Sec section;
} ElfW_Shdr;

void print_section_header(const ElfW_Shdr *hdr);

ElfW_Shdr read_section_header(FILE *src, ElfW(Xword) off);

char *section_type_to_string(ElfW(Word) type);

char *section_flags_to_string(ElfW(Xword) flags);

#endif //ELF_PATCHER_ELF_SECTION_HEADER_H
