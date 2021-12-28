#include <Setup.h>
#include <File.h>
#include <MemoryUefi.h>
#include <Tools.h>
//加载到elf
EFI_STATUS LoadElf(EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *file,CHAR16 *filename,ADDRESS *addr);


BmpStruct LoadBmpFileToMemory(
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *file,
    CHAR16 *filename
);