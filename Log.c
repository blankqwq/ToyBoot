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
    switch (status)
    {
        case 0:
            statusMessage=L"info:\0";
            break;
        default:
            statusMessage=L"warning:\0";
            break;
    }
    #ifdef LOG
        Print(L"%s:%s --status:%d\n",statusMessage,message,status);
    #else
        LogInfo(statusMessage);
    #endif //LOG
    
    return status;
}




