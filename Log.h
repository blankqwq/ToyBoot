#include <Uefi.h>
#ifndef _LOG_DEFINE_
#define _LOG_DEFINE_

EFI_STATUS LogStatus(EFI_STATUS status,CHAR16 *message);
EFI_STATUS LogInit(EFI_HANDLE handle);
void LogError(CHAR16 *message);
#endif //_LOG_DEFINE_
