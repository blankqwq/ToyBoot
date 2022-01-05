#include "Setup.h"

#ifndef _MEMORY_UEFI_DEFINE_
#define _MEMORY_UEFI_DEFINE_
EFI_STATUS AllocatePage(UINT64 pages,address *addr);

EFI_STATUS AllocateM(UINT64 size, void **addr);

EFI_STATUS FreeM(VOID *addr);

EFI_STATUS ReserveMemory(address *addr,UINT64 size,UINTN width,UINTN height,UINTN offset);
#endif //_MEMORY_UEFI_DEFINE_
