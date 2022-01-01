#include <File.h>
#include <Prograss.h>
#include <Load.h>

int (*func)();
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
        i++;
    }
    //加载efi
    ADDRESS bin;
    ADDRESS bufferBase =video->Mode->FrameBufferBase;
    Elf64_Ehdr elf;
    status =LoadElf64ToMemroy(ImageHandle,file,L"kernel.elf",&bin,&elf);
    if (RETRURN_IF_ERROR(status,L"Boot load ELF"))
    {
        return status;
    }
    Print(L"load ff:%X,%X\n",bin,elf.e_entry);
    Print(L"get addr:%X,%X\n",bin,bufferBase);
    // goto *();
    // bin = bin+elf.e_entry;
    ADDRESS entry=bin+elf.e_entry;
    // goto *entry;
    func=(void *)entry;
    // asm("jmp %0"::"m"(entry));
    func();
    return status;
}