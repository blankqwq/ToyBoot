#include <File.h>
#include <Prograss.h>
#include <Load.h>
#include <Type.h>

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

   // config
    BOOT_CONFIG ttt;
    ttt.VideoBufferBase=video->Mode->FrameBufferBase;
    ttt.VideoBufferSzie = video->Mode->FrameBufferSize;

    prograss_t t;
    // //进度条初始化
    PrograssInit(&t,video,300,450,5,10,50,&black);
    int i=0;
    while (i<100000)
    {
        /* code */
        PrograssIncrement(&t);
        i++;
    }
    //加载efi
    ADDRESS bin;
    Elf64_Ehdr elf;

  
    status =LoadElf64ToMemroy(ImageHandle,file,L"kernel.elf",&bin,&elf);
    if (RETRURN_IF_ERROR(status,L"Boot load ELF"))
    {
        return status;
    }
    Print(L"load ff:%X,%X\n",bin,elf.e_entry);
    ADDRESS entry=elf.e_entry;
    Print(L"Config:%X %X\n",ttt.VideoBufferBase,ttt.VideoBufferSzie);

    //关闭Protocol
    gBS->CloseProtocol(file,&gEfiSimpleFileSystemProtocolGuid,ImageHandle,NULL);
    gBS->CloseProtocol(video,&gEfiGraphicsOutputProtocolGuid,ImageHandle,NULL);

    //引导启动ELF
    status = ByeBootServices(ImageHandle);
    if (RETRURN_IF_ERROR(status,L"Exit BootService"))
    {
        return status;
    }
    int (*func)(BOOT_CONFIG *dda);
    func=(int (*)(BOOT_CONFIG *dda))entry;
    int res = func(&ttt);
    return res;
}