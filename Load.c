#include <Setup.h>
#include <File.h>
#include <MemoryUefi.h>
#include <Tools.h>

EFI_STATUS LoadElf(
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *file, CHAR16 *filename, ADDRESS *addr)
{
    EFI_FILE_PROTOCOL *target = NULL;
    EFI_STATUS status = OpenFile(
        file, filename, &target, EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE,
        EFI_OPEN_PROTOCOL_GET_PROTOCOL);
    if (RETRURN_IF_ERROR(status, L"read logo"))
    {
        return status;
    }
    UINT64 infoBuffer = sizeof(EFI_FILE_INFO) + 128;
    EFI_FILE_INFO fileInfo = GetFileInfo(target, &infoBuffer);
    UINT64 fileSize = fileInfo.FileSize;
    status = ReadToMemoryPage(target, &fileSize, addr);
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
    EFI_FILE_INFO fileInfo = GetFileInfo(target, &infoBuffer);
    UINT64 fileSize = fileInfo.FileSize;
    status = ReadToMemoryPage(target, &fileSize, &addr);
    if (RETRURN_IF_ERROR(status, L"Read memory by logo"))
    {
        return bmp;
    }
    //读取宽高
    bmp = ParseBmpHeader(addr);
    bmp.size = fileInfo.FileSize;
    // 对角翻转
    status = ReserveMemory(&addr, fileInfo.FileSize, bmp.width, bmp.height, bmp.offset);
    //剧中显示
    bmp.start = addr + bmp.offset;
    if (RETRURN_IF_ERROR(status, L"Read memory by logo"))
    {
        return bmp;
    }
//设置开始地址
#ifdef LOG
    Print(L"read start:%d\n", bmp.start);
#endif //LOG
    return bmp;
}
