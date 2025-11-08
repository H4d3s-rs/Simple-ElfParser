#include <fcntl.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>

typedef struct {
    unsigned char e_ident[16]; 
    uint16_t e_type; 
    uint16_t e_machine; 
    uint32_t e_version; 
    uint64_t e_entry; 
    uint64_t e_phoff; 
    uint64_t e_shoff; 
    uint32_t e_flags; 
    uint16_t e_ehsize; 
    uint16_t e_phentsize;
    uint16_t e_phnum; 
    uint16_t e_shentsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;

} Elf64_Ehdr;


Elf64_Ehdr read_elf_header(const char *path){
    int fd = open(path, O_RDONLY);

    if (fd == -1){
        perror("[-] Couldn't open the file");
        exit(EXIT_FAILURE);

    }

    Elf64_Ehdr header;
    ssize_t bytes = read(fd, &header, sizeof(Elf64_Ehdr));

    if (bytes < sizeof(Elf64_Ehdr)){
        perror("[-] Couldn't read the file");
        close(fd);
        exit(EXIT_FAILURE);

    }

    close(fd);
    return header;
}


void print_elf_header(Elf64_Ehdr *header){
    printf("[+] Magic: 0x%02x 0x%02x 0x%02x 0x%02x\n", header->e_ident[0], header->e_ident[1], header->e_ident[2], header->e_ident[3]);
    
    if(header->e_ident[4] == 0) {
        printf("[+] ElfClass: Invalid Class\n");

    }else if(header->e_ident[4] == 1){
        printf("[+] ElfClass: 32 Bits\n");

    }else if(header->e_ident[4] == 2){
        printf("[+] ElfClass: 64 Bits\n");

    }

    if (header->e_ident[5] == 0){
        printf("[+] Data Encoding: Invalid Data Encoding\n");

    }else if(header->e_ident[5] == 1){
        printf("[+] Data Encoding: 2's complement, little endian\n");
    
    }else if(header->e_ident[5] == 2){
        printf("[+] Data Encoding: 2's complement, big endian\n");

    }

    if(header->e_ident[7] == 1){
        printf("[+] ABI: HP-UX (In fact, probably System V)\n");

    }else {
        printf("[+] ABI: checkout for file /usr/include/elf.h the code is 0x%02x\n", header->e_ident[6]);
        // Yeah i'm too lazy to finish this part... =P
    }

   
    printf("[+] Type: 0x%04hx\n", header->e_type);
    printf("[+] Machine Code: 0x%04hx\n", header->e_machine);
    printf("[+] Version: 0x%08x\n", header->e_version);
    printf("[+] Entrypoint: 0x%016lx\n", header->e_entry);
    printf("[+] Program Header Offset: 0x%016lx\n", header->e_phoff);
    printf("[+] Section Header Offset: 0x%016lx\n", header->e_shoff);
    printf("[+] Flags: 0x%08x\n", header->e_flags);
    printf("[+] Header size: %hu Bytes\n", header->e_ehsize);
    printf("[+] Program Header Table Entry Size: 0x%u\n", header->e_phentsize);
    printf("[+] Program Header Table Entry Count: 0x%u\n", header->e_phnum);
    printf("[+] Section Header Table Entry Size: 0x%u\n", header->e_shentsize);
    printf("[+] Section Header Table Entry Count: 0x%u\n", header->e_shnum);
    printf("[+] Section Header String Table Index: 0x%u\n", header->e_shstrndx);

}   


int main(int argc, char **argv){
    if (argc != 2) {
        perror("[?] ./elfParser <file-path>");
        return 1;

    }

    Elf64_Ehdr header = read_elf_header(argv[1]);
    print_elf_header(&header);
    return 0;
}