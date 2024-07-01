/* ************************************************************************
> File Name:     elfparse.c
> Author:        niu0217
> Created Time:  Mon 01 Jul 2024 11:28:42 AM CST
> Description:   
 ************************************************************************/

/* gcc elfparse.c -o elfparse */

/*
  https://www.cnblogs.com/bunner/p/14865893.html
*/

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <elf.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdint.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char **argv) {
    int fd, i;
    uint8_t *mem;
    struct stat st;
    char *StringTable, *interp;

    Elf64_Ehdr *ehdr;
    Elf64_Shdr *shdr;
    Elf64_Phdr *phdr;

    if (argc < 2) {
        printf("Usage: %s <executable>\n", argv[0]);
        exit(0);
    }

    if ((fd = open(argv[1], O_RDONLY)) < 0) {
        perror("open");
        exit(-1);
    }

    if (fstat(fd, &st) < 0) {
        perror("fstat");
        exit(-1);
    }

    /*
    ** 使用mmap函数将可执行文件映射到内存中
    ** NULL:        由系统决定映射区的起始地址
    ** st.st_size:  映射区的大小
    ** PROT_READ:   映射区的保护标志
    ** MAP_PRIVATE: 建立一个写时拷贝的私有映射
    ** fd:          文件描述符
    ** 0:           将从此偏移处的文件位置开始读取
    */
    mem = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (mem == MAP_FAILED) {
        perror("mmap");
        exit(-1);
    }

    /* 若上述步骤成功执行，则此时mem就是可执行文件映射的起始位置 */

    /* 获取文件头 */
    ehdr = (Elf64_Ehdr *) mem;

    /* 获取段表 */
    phdr = (Elf64_Phdr *) (&mem[ehdr->e_phoff]);

    /* 获取节表 */
    shdr = (Elf64_Shdr *) (&mem[ehdr->e_shoff]);
    
    /* 检查该文件是否为ELF映像 */
    if (mem[0] != 0x7f || strncmp(&mem[1], "ELF", 3)) {
        fprintf(stderr, "%s is not an ELF file\n", argv[1]);
        exit(-1);
    }

    /* 确保为可执行文件 */
    if (ehdr->e_type != ET_EXEC) {
        fprintf(stderr, "%s is not an executable\n", argv[1]);
        exit(-1);
    }

    printf("ELF header:\n\n");
    printf("%-36s 0x%x\n", " Program Entry point:", ehdr->e_entry);
    printf("%-36s %d (bytes)\n", " Size of program header:", ehdr->e_phentsize);
    printf("%-36s %d\n", " Number of Program headers:", ehdr->e_phnum);
    printf("%-36s %d (bytes)\n", " Size of section header:", ehdr->e_shentsize);
    printf("%-36s %d\n", " Number of Section headers:", ehdr->e_shnum);
    printf("%-36s %d\n\n", " Section header string table index:", ehdr->e_shstrndx);

    /* 存储节名的字符串表 */
    StringTable = &mem[shdr[ehdr->e_shstrndx].sh_offset];

    printf("Section header list: \n\n");
    for (i = 1; i < ehdr->e_shnum; i++)
        printf("%-26s 0x%x\n", &StringTable[shdr[i].sh_name], shdr[i].sh_addr);

    printf("\n Program header list: \n\n");
    for (i = 0; i < ehdr->e_phnum; i++) {
        switch (phdr[i].p_type) {
            case PT_LOAD:
                /* 一般PT_LOAD类型的段有代码段和数据段，可执行的为代码段 */
                if (phdr[i].p_flags & PF_X)
                    printf("Text segment: 0x%x\n", phdr[i].p_vaddr);
                else 
                    printf("Data segment: 0x%x\n", phdr[i].p_vaddr);
                break;
            case PT_INTERP:
                /* 程序解释器的位置，例如 ld-linux.so.2 */
                interp = strdup((char *)&mem[phdr[i].p_offset]);
                printf("Interpreter: %s\n", interp);
                break;
            case PT_NOTE:
                printf("Note segment: 0x%x\n", phdr[i].p_vaddr);
                break;
            case PT_DYNAMIC:
                printf("Dynamic segment: 0x%x\n", phdr[i].p_vaddr);
                break;
            case PT_PHDR:
                printf("Phdr segment: 0x%x\n", phdr[i].p_vaddr);
                break;
        }
    }

    free(interp);

    exit(0);
}
