#include <Uefi.h>

EFI_STATUS LogStatus(EFI_STATUS status,CHAR16 *message);
EFI_STATUS LogInit(EFI_HANDLE handle);
void LogError(CHAR16 *message);