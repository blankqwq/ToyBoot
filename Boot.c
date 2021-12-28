#include "Setup.h"
#include "File.h"
#include "Video.h"
#include "Prograss.h"
#include "Log.h"


EFI_STATUS EFIAPI UefiMain(
    EFI_HANDLE ImageHandle,
    EFI_SYSTEM_TABLE *systemTable
)
{
    EFI_STATUS status = EFI_SUCCESS;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *file=NULL;
    EFI_GRAPHICS_OUTPUT_PROTOCOL *video=NULL;
    status = getFileProtocol(ImageHandle,0,&file);
    if (RETRURN_IF_ERROR(status,L"Boot getFileprotoco"))
    {
        return status;
    }

    status =LogInit(file);
    if (RETRURN_IF_ERROR(status,L"Boot log"))
    {
        return status;
    }
   
    status = getVideoProtocol(ImageHandle,0,&video);
    if (RETRURN_IF_ERROR(status,L"Boot getVideo"))
    {
        return status;
    }

    if(file==NULL||video==NULL){
        Print(L"Error!\n");
    }
    //分辨率设置
    status = ResizeVideo(video);
    if (RETRURN_IF_ERROR(status,L"Resize logo loading"))
    {
        return status;
    }
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL black = {0,0,0,0};
    status = FillColorToVideo(video,&black,0,0,780,600);
    if (RETRURN_IF_ERROR(status,L"Fill color"))
    {
        return status;
    }
    status = LoadBmpToVideo(video,file,L"\\logo2.bmp",0,50);
    if (RETRURN_IF_ERROR(status,L"Boot logo loading"))
    {
        return status;
    }
    prograss_t t;
    // //进度条初始化
    PrograssInit(&t,video,300,450,5,10,50,&black);
    int i=0;
    while (i<100)
    {
        /* code */
        PrograssIncrement(&t);
    }
    //加载efi
    
    return status;
}