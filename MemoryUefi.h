#include "Setup.h"

EFI_STATUS AllocatePage(UINT64 pages,address *addr);

EFI_STATUS AllocateM(UINT64 size, void **addr);

EFI_STATUS FreeM(VOID *addr);

EFI_STATUS ReserveMemory(address *addr,UINT64 size,UINTN width,UINTN height,UINTN offset);