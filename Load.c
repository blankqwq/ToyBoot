
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
    bmp.size = fileInfo->FileSize;
    // 对角翻转
    status = ReserveMemory(&addr, fileInfo->FileSize, bmp.width, bmp.height, bmp.offset);
     if (RETRURN_IF_ERROR(status, L"Read memory by logo"))
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
    IN EFI_HANDLE ImageHandle
)
{
    EFI_STATUS Status = EFI_SUCCESS;
    Print(L"\nBye BS.\n");
    MEMORY_MAP MemMap = {4096 * 4, NULL,4096,0,0,0};

    Status = gBS->AllocatePool(
        EfiLoaderData,
        MemMap.BufferSize,
        &MemMap.Buffer
    );
    if(EFI_ERROR(Status))
    {
        Print(L"Allocate pool for memory map failed.\n");
        return Status;
    }    
  
    Status = gBS->GetMemoryMap(
        &MemMap.BufferSize,
        (EFI_MEMORY_DESCRIPTOR *)MemMap.Buffer,
        &MemMap.MapKey,
        &MemMap.DescriptorSize,
        &MemMap.DescriptorVersion
    );

    if(EFI_ERROR(Status))
    {
        Print(L"Get memory map error.\n");
        return Status;
    }
 
    Status = gBS->ExitBootServices(
        ImageHandle, MemMap.MapKey
    );

    if(EFI_ERROR(Status))
    {
        Print(L"ExitBootServices error.\n");
        return Status;
    }

    return Status;
}



//加载ELF
EFI_STATUS LoadElf64ToMemroy(EFI_HANDLE ImageHandle,EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *file, CHAR16 *filename, ADDRESS *load, Elf64_Ehdr *elf)
{
    // 分析和载入
    #ifdef LOG
    Print(L"log:addr:%d\n",*load);
    #endif //LOG
    
    EFI_STATUS status = LoadFile(file, filename, load);
     #ifdef LOG
    Print(L"log:addr:%d\n",*load);
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
    UINT8 *temp=(UINT8 *)(*load+0x1000);
    for(int i=0;i<0x50;i++){
        if(i%16==0){
            Print(L"\n");
        }
        Print(L"%X|",*temp);
        temp++;
    }
    //开始加载？
    // status = ByeBootServices(ImageHandle);
    return CHECK_ERROR_BEFORELOG(status, L"exit load buffer");
}
