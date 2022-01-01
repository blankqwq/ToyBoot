#include "Setup.h"
#include <Tools.h>
#include <MemoryUefi.h>
#include <Protocol.h>
#define NAX_BUFFER 50 * 1024 * 1024

EFI_STATUS getFileProtocol(
    IN EFI_HANDLE imageHandle,
    IN UINT8 index,
    OUT EFI_SIMPLE_FILE_SYSTEM_PROTOCOL **file)
{
    EFI_HANDLE *fileHandle = NULL;
    EFI_STATUS status = getHandleByProtocol(
        imageHandle,
        &gEfiSimpleFileSystemProtocolGuid,
        &fileHandle);

    if (RETRURN_IF_ERROR(status, L"Get file-handle"))
    {
        return status;
    }
    Print(L"get handle size:%d\n", sizeof(fileHandle));
    status = openHandleByBuffer(imageHandle, fileHandle[index], &gEfiSimpleFileSystemProtocolGuid, (void **)file);
    return CHECK_ERROR_BEFORELOG(status, L"Open file-handle");
}

EFI_STATUS OpenFile(
    IN EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *driver,
    IN CHAR16 *filename,
    OUT EFI_FILE_PROTOCOL **result,
    IN UINT64 openMode,
    IN UINT64 attrs)
{
    EFI_FILE_PROTOCOL *root;
    EFI_STATUS status = driver->OpenVolume(driver, &root);
    if (RETRURN_IF_ERROR(status, L"Open Volume"))
    {
        return status;
    }
    status = root->Open(root, result, filename, openMode, attrs);
    return CHECK_ERROR_BEFORELOG(status, L"Open File");
}

EFI_FILE_INFO *GetFileInfo(EFI_FILE_PROTOCOL *target, UINT64 *size)
{
    EFI_FILE_INFO *file_info=NULL;
    EFI_GUID infomationGuid = EFI_FILE_INFO_ID;
    // 需要指定最大操作
    EFI_STATUS status = EFI_SUCCESS;
    status = AllocateM(*size, (void **)&file_info);
    if (RETRURN_IF_ERROR(status, L"Allocate file-info"))
    {
        /* code */
        LogError(L"read info faild");
    }
    status = target->GetInfo(target, &infomationGuid, size, file_info);
    if (RETRURN_IF_ERROR(status, L"Get file-info"))
    {
        /* code */
        LogError(L"read info faild");
    }
    return file_info;
}

EFI_STATUS ReadToAddr(IN EFI_FILE_PROTOCOL *target, IN OUT UINT64 *size, OUT address addr)
{
    EFI_STATUS status = target->Read(target, size, (void *)addr);
    return CHECK_ERROR_BEFORELOG(status, L"Read file-content");
}

EFI_STATUS ReadToMemoryPage(IN EFI_FILE_PROTOCOL *target, IN OUT UINT64 *size, OUT address *addr)
{
    UINT64 page_size = calPage(*size);
#ifdef LOG
    Print(L"get page %d\n", page_size);
#endif //LOG
    EFI_STATUS status = AllocatePage(page_size, addr);
    if (RETRURN_IF_ERROR(status, L"Allocate read page"))
    {
        return status;
    }
#ifdef LOG
    Print(L"get addr %d\n", addr);
#endif //LOG
    status = ReadToAddr(target, size, *addr);
    return CHECK_ERROR_BEFORELOG(status, L"Read file to memory");
}

// EFI_STATUS WriteFromAddr(IN EFI_FILE_PROTOCOL *target,IN CHAR16 *filename,IN void *data)
// {

// }

// EFI_STATUS Write(IN EFI_FILE_PROTOCOL *target,IN CHAR16 *filename,IN void *data)
// {

// }
