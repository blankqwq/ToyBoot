#include <Setup.h>
#include <Tools.h>

EFI_STATUS AllocatePage(UINT64 pages, address *addr)
{
    EFI_STATUS status = gBS->AllocatePages(AllocateAnyPages, EfiBootServicesData, pages, addr);

    return CHECK_ERROR_BEFORELOG(status, L"Allocate page");
}

EFI_STATUS FreePage(UINT64 pages, address addr)
{
    EFI_STATUS status = gBS->FreePages(addr, pages);

    return CHECK_ERROR_BEFORELOG(status, L"Free page");
}


EFI_STATUS ReserveMemory(address *addr, UINT64 size, UINTN width, UINTN height, UINTN offset)
{
#ifdef LOG
    Print(L"get size to reserve: %d\n", size);
#endif //LOG
    address old = *addr + offset;
    size = size - offset;
    address new;

    UINT64 pageSize = calPage(size);
    EFI_STATUS status = AllocatePage(pageSize, &new);

    if (RETRURN_IF_ERROR(status, L"Reserve"))
    {
        return status;
    }
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL *PixelToBuffer = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)new;
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL *PixelFromFile = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)(old + size);
    for (UINTN i = 0; i < height; i++)
    {
        PixelFromFile -= width;
        for (UINTN j = 0; j < width; j++)
        {
            *PixelToBuffer = *PixelFromFile;
            PixelToBuffer++;
            PixelFromFile++;
        }
        PixelFromFile -= width;
    }
    *addr = new;
    //free
    status = FreePage(pageSize, old);
    if (RETRURN_IF_ERROR(status, L"Free old"))
    {
        return status;
    }
    return status;
}