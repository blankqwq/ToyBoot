#include <Setup.h>


void LogWarning(void){

}


void LogError(CHAR16 *message){
    Print(L"error:%s\n",message);
}
void LogInfo(CHAR16 *message){

}

EFI_STATUS LogInit(EFI_HANDLE handle){
    return EFI_SUCCESS;
}

EFI_STATUS LogStatus(EFI_STATUS status,CHAR16 *message){
    CHAR16 *statusMessage;
    UINT8 flag=0;
    switch (status)
    {
        case 0:
            statusMessage=L"info:\0";
            break;
        default:
            flag=1;
            statusMessage=L"warning:\0";
            break;
    }
    #ifdef LOG
        Print(L"%s:%s --status:%d\n",statusMessage,message,status);
    #else
        if (flag)
        {
            Print(L"%s:%s --status:%d\n",statusMessage,message,status);
        }
        // Log(statusMessage);
    #endif //LOG
    
    return status;
}




