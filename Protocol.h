#include "Setup.h"


#ifndef _PROTOCOL_DEFINE_
#define _PROTOCOL_DEFINE_

EFI_STATUS getHandleByProtocol(
    IN EFI_HANDLE imageHandle,
    IN EFI_GUID *type,
    OUT EFI_HANDLE **handleBuffer);

EFI_STATUS openHandleByBuffer(
    IN EFI_HANDLE imageHandle,
    IN EFI_HANDLE buffer,
    IN EFI_GUID *type,
    OUT void **handle);
#endif //_PROTOCOL_DEFINE_
