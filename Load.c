
#include <Load.h>
#include <Library/MemoryAllocationLib.h>

EFI_STATUS LoadFile(
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *file, CHAR16 *filename, ADDRESS *addr)
{
    EFI_FILE_PROTOCOL *target = NULL;
    EFI_STATUS status = OpenFile(
        file, filename, &target, EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE,
        EFI_OPEN_PROTOCOL_GET_PROTOCOL);
    if (RETRURN_IF_ERROR(status, L"read"))
    {
        return status;
    }
    UINT64 infoBuffer = sizeof(EFI_FILE_INFO) + 128;
    EFI_FILE_INFO *fileInfo = GetFileInfo(target, &infoBuffer);
    UINT64 fileSize = fileInfo->FileSize;
    status = FreeM((VOID *)fileInfo);
    if (RETRURN_IF_ERROR(status, L"Free memory by file info"))
    {
        return status;
    }
    status = ReadToMemoryPage(target, &fileSize, addr);
    return CHECK_ERROR_BEFORELOG(status, L"load elf");
}

EFI_STATUS LoadElf(
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *file, CHAR16 *filename, ADDRESS *addr)
{
    EFI_STATUS status = LoadFile(file, filename, addr);
    return CHECK_ERROR_BEFORELOG(status, L"load elf");
}

BmpStruct LoadBmpFileToMemory(
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *file,
    CHAR16 *filename)
{
    BmpStruct bmp;
    EFI_FILE_PROTOCOL *target = NULL;
    address addr;
    EFI_STATUS status = OpenFile(
        file, filename, &target, EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE,
        EFI_OPEN_PROTOCOL_GET_PROTOCOL);
    if (RETRURN_IF_ERROR(status, L"read logo"))
    {
        return bmp;
    }
    UINT64 infoBuffer = 1000;
    EFI_FILE_INFO *fileInfo = GetFileInfo(target, &infoBuffer);
    UINT64 fileSize = fileInfo->FileSize;
    status = ReadToMemoryPage(target, &fileSize, &addr);
    if (RETRURN_IF_ERROR(status, L"Read memory by logo"))
    {
        return bmp;
    }
    //读取宽高
    bmp = ParseBmpHeader(addr);
    bmp.size = fileSize;
    // 对角翻转
    status = ReserveMemory(&addr, bmp.size, bmp.width, bmp.height, bmp.offset);
    if (RETRURN_IF_ERROR(status, L"Reserve memory by logo"))
    {
        return bmp;
    }
    //剧中显示
    bmp.start = addr + bmp.offset;
    status = FreeM((void *)fileInfo);
    if (RETRURN_IF_ERROR(status, L"Free memory by logo-bmpstruct"))
    {
        return bmp;
    }
//设置开始地址
#ifdef LOG
    Print(L"read start:%d\n", bmp.start);
#endif //LOG
    //打印
    return bmp;
}

EFI_STATUS ByeBootServices(
    IN EFI_HANDLE ImageHandle)
{
    EFI_STATUS Status = EFI_SUCCESS;
    Print(L"\nBye BootService.\n");
    UINTN MemoryMapSize;
    EFI_MEMORY_DESCRIPTOR MemoryMap;
    UINTN MapKey;
    UINTN DescriptorSize;
    UINT32 DescriptorVersion;
    // MEMORY_MAP MemMap;
    //结束前先打印memory 结构

    Status = gBS->GetMemoryMap(
        &MemoryMapSize,
        &MemoryMap,
        &MapKey,
        &DescriptorSize,
        &DescriptorVersion);

    if (EFI_ERROR(Status))
    {
        Print(L"Get memory map error.\n");
        return Status;
    }

    Status = gBS->ExitBootServices(
        ImageHandle, MapKey);

    if (EFI_ERROR(Status))
    {
        Print(L"ExitBootServices error.\n");
        return Status;
    }
    // 退出后内存地址变化了
    return Status;
}

/**
 * @brief 
 * 将加载的段放入指定位置
 * @param elf 
 * @return EFI_STATUS 
 */
EFI_STATUS copyElfToEntry(Elf64_Ehdr *elf,ADDRESS start){
    // 遍历所有段
    ADDRESS PHeader;
    Elf64_Phdr *Phdr;
    UINT8 Index;
    
    PHeader = start + elf->e_phoff;
    for ( Index = 0; Index < elf->e_phnum; Index++)
    {
        
        Phdr = (Elf64_Phdr *)PHeader;
        if (Phdr->p_type == PT_LOAD)
        {
            VOID *MemStart;
            VOID *SegmentStart;
            VOID *ExtraZeroes;
            UINTN ExtraZeroesCount;
            #ifdef LOG
            Print(L"Load:PT_LOAD [%d]\n",Index);
            #endif //LOG
            /* code */
            MemStart=(VOID *)((ADDRESS *)Phdr->p_vaddr);
            SegmentStart=(VOID *)(start+Phdr->p_paddr);
            // load
            gBS->CopyMem(MemStart,SegmentStart,Phdr->p_filesz);
            // 缺少中间段段填充
            ExtraZeroes = (VOID *)((ADDRESS)MemStart + Phdr->p_filesz);
            ExtraZeroesCount = Phdr->p_memsz - Phdr->p_filesz;
            if (ExtraZeroesCount > 0)
            {
                gBS->SetMem(ExtraZeroes, 0x00, ExtraZeroesCount);
            }
        }
        PHeader += elf->e_phentsize;
    }
    
    return EFI_SUCCESS;
}

//加载ELF
EFI_STATUS LoadElf64ToMemroy(EFI_HANDLE ImageHandle, EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *file, CHAR16 *filename, ADDRESS *load, Elf64_Ehdr *elf)
{
// 分析和载入
#ifdef LOG
    Print(L"log:addr:%d\n", *load);
#endif //LOG

    EFI_STATUS status = LoadFile(file, filename, load);
#ifdef LOG
    Print(L"log:addr:%d\n", *load);
#endif //LOG
    //解析ELF格式
    if (RETRURN_IF_ERROR(status, L"load elf"))
    {
        return status;
    }
    *elf = *((Elf64_Ehdr *)*load);
#ifdef LOG
    Print(L"read entry point:%X,type:%X,flag:%X\n", elf->e_entry, elf->e_type, elf->e_flags);
#endif
    // 拷贝内存
    UINT8 IdentMagic[4] = {0x7f, 0x45, 0x4c, 0x46};
    int i = 0;
    for (i = 0; i < 4; i++)
    {
        if (elf->e_ident[i] != IdentMagic[i])
        {
            return EFI_INVALID_PARAMETER;
        }
    }
    copyElfToEntry(elf,*load);
    //开始加载？
    status = ByeBootServices(ImageHandle);
    //退出后没有打印服务,且栈清空了？
    return status;
}
