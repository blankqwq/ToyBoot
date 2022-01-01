#include <Setup.h>

EFI_STATUS getFileProtocol(
    IN EFI_HANDLE imageHandle,
    IN UINT8 index,
    OUT EFI_SIMPLE_FILE_SYSTEM_PROTOCOL **file);

EFI_STATUS OpenFile(
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *driver,
    CHAR16 *filename,
    EFI_FILE_PROTOCOL **result,
    IN UINT64 openMode,
    IN UINT64 attrs);

EFI_FILE_INFO *GetFileInfo(EFI_FILE_PROTOCOL *target, UINT64 *size);
EFI_STATUS ReadToAddr(IN EFI_FILE_PROTOCOL *target, IN OUT UINT64 *size, OUT address *addr);
EFI_STATUS ReadToMemoryPage(IN EFI_FILE_PROTOCOL *target, IN OUT UINT64 *size, OUT address *addr);