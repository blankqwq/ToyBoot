#include <Protocol.h>



EFI_STATUS getHandleByProtocol(
    IN EFI_HANDLE imageHandle,
    IN EFI_GUID *type,
    OUT EFI_HANDLE **handleBuffer)
{
    UINTN no_handle = 0;
    EFI_STATUS status = gBS->LocateHandleBuffer(
        ByProtocol,                        //通过Protocol
        type, // GUID
        NULL,
        &no_handle,
        handleBuffer);
#ifdef LOG
    if (EFI_ERROR(status))
    {
        Print(L"Faild to LocateHandleBuffer.err = %d\n", status);
    }
#endif ///*define*/

    return status;
}

EFI_STATUS openHandleByBuffer(
    IN EFI_HANDLE imageHandle,
    IN EFI_HANDLE buffer,
    IN EFI_GUID *type,
    OUT void **handle
)
{
    EFI_STATUS status = gBS->OpenProtocol(
        buffer,
        type,
        handle,
        imageHandle,
        NULL,
        EFI_OPEN_PROTOCOL_GET_PROTOCOL);
#ifdef LOG
    if (EFI_ERROR(status))
    {
        Print(L"buffer count buffer=%d", sizeof(buffer));
        Print(L"Faild to open&getHandle err code=%d\n", status);
    }
#endif
    return status;
}
