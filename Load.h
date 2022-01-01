#include <File.h>
#include <MemoryUefi.h>
#include <Tools.h>
//加载到elf
#include <ELF/elf_common.h>
#include <ELF/elf64.h>


// typedef struct{
//     CHAR8 e_ident[EI_NIDENT];
//     // 执行类型
//     UINT16 e_type;
//     //指令集
//     UINT16 e_machine;
//     //ELF版本 4
//     UINT32 e_version;
//     // 64位置与32位开始不同 入口地址
//     UINT64 e_entry;
//     // program 偏移地址，偏移! 直接+e_poff然后开始初始化表头
//     UINT64 e_phoff;
//     // 节头表 
//     UINT64 e_shoff;
//     // 目标整体架构
//     INT32 e_flags;
//     // 文件头大小
//     UINT16 e_ehsize;
//     //程序头表入口
//     UINT16 e_phentsize;
//     // 头表数量
//     UINT16 e_phnum;
//     //section 表入口大小
//     UINT16 e_shentsize;
//     //section 表数量 
//     UINT16 e_shnum;
//     //section name
//     UINT16 e_shstrndx;
// } Elf64_Ehdr;

EFI_STATUS LoadElf(EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *file,CHAR16 *filename,ADDRESS *addr);


BmpStruct LoadBmpFileToMemory(
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *file,
    CHAR16 *filename
);

EFI_STATUS LoadElf64ToMemroy(EFI_HANDLE ImageHandle,EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *file,CHAR16 *filename,ADDRESS *load,Elf64_Ehdr *elf);


#ifndef _MEM_MAP_STRUCT_
#define _MEM_MAP_STRUCT_
typedef struct
{
    UINTN BufferSize;
    VOID *Buffer;
    UINTN MapSize;
    UINTN MapKey;
    UINTN DescriptorSize;
    UINT32 DescriptorVersion;
} MEMORY_MAP;
#endif //_MEM_MAP_STRUCT_



// typedef struct{
//     CHAR8 e_ident[EI_NIDENT];
//     // 执行类型
//     UINT16 e_type;
//     //指令集
//     UINT16 e_machine;
//     //ELF版本 4
//     UINT32 e_version;
//     // 64位置与32位开始不同
//     UINT32 e_entry;
//     UINT32 e_phoff;
//     UINT32 e_shoff;
//     INT32 e_flags;
//     UINT16 e_ehsize;
//     UINT16 e_phentsize;
//     UINT16 e_phnum;
//     UINT16 e_shentsize;
//     UINT16 e_shnum;
//     UINT16 e_shstrndx;
// } Elf32_Ehdr32;
