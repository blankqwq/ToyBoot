#include "Setup.h"
#include <Load.h>
#include <Protocol.h>
#include <Video.h>
// 获取protocol
EFI_STATUS getVideoProtocol(
    IN EFI_HANDLE imageHandle,
    IN UINT8 index,
    OUT EFI_GRAPHICS_OUTPUT_PROTOCOL **graph)
{
    //指针的地址一点得外部传入，否者好像也会变成形参
    EFI_HANDLE *graphHandle = NULL;
    EFI_STATUS status = getHandleByProtocol(
        imageHandle,
        &gEfiGraphicsOutputProtocolGuid,
        &graphHandle);

    if (RETRURN_IF_ERROR(status, L"get video handle"))
    {
        return status;
    }
    status = openHandleByBuffer(imageHandle, graphHandle[index], &gEfiGraphicsOutputProtocolGuid, (void **)graph);
    return CHECK_ERROR_BEFORELOG(status, L"get video");
}

EFI_STATUS ResizeVideo(EFI_GRAPHICS_OUTPUT_PROTOCOL *graph)
{
    //设置一个VideoConfig结构体=>值
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info;
    UINTN sizeof_info = 0;
    EFI_STATUS status = EFI_SUCCESS;
    for (UINTN i = 0; i < graph->Mode->MaxMode; i++)
    {
        status = graph->QueryMode(
            graph,
            i,
            &sizeof_info,
            &info);
        if (RETRURN_IF_ERROR(status, L"Query mode"))
        {
            return status;
        }
        Print(L"Mode %d, Height = %d,Width = %d\n", i, info->HorizontalResolution, info->VerticalResolution);
    }
    return status;
}

/**
 * @brief 
 * 填充颜色
 * @param video 
 * @param color 
 * @param X 
 * @param Y 
 * @param width 
 * @param height 
 * @return EFI_STATUS 
 */
EFI_STATUS FillColorToVideo(
    EFI_GRAPHICS_OUTPUT_PROTOCOL *graph, 
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL *color, 
    UINTN X, UINTN Y, 
    UINTN width, UINTN height)
{
     EFI_STATUS status = graph->Blt(
        graph,
        color,
        EfiBltVideoFill,
        0,0,
        X,Y,
        width,height,
        0
    );
    return CHECK_ERROR_BEFORELOG(status,L"Fill color");
}



UINTN getVideoCurrentHeight(EFI_GRAPHICS_OUTPUT_PROTOCOL *graph){
    return graph->Mode->Info->HorizontalResolution;
}

UINTN getVideoCurrentWidth(EFI_GRAPHICS_OUTPUT_PROTOCOL *graph){
    return graph->Mode->Info->VerticalResolution;
}

/**
 * @brief 
 * 加载logo
 * @param graph 
 * @param file 
 * @param filename 
 * @return EFI_STATUS 
 */
EFI_STATUS LoadBmpToVideo(
    EFI_GRAPHICS_OUTPUT_PROTOCOL *graph,
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *file,
    CHAR16 *filename,
    UINTN offsetX,
    UINTN offsetY)
{
    BmpStruct bmp = LoadBmpFileToMemory(file, filename);
    if (bmp.start == 0)
    {
        return 2;
    }
    // 载入位置
    UINTN x = getVideoCurrentHeight(graph);
    UINTN y = getVideoCurrentWidth(graph);
    x=0.5*(x-bmp.width)-offsetX;
    y=0.5*(y-bmp.height)-offsetY;
    // 计算剧中显示
    EFI_STATUS status = graph->Blt(
        graph,
        (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)bmp.start,
        EfiBltBufferToVideo,
        0, 0,
        x, y,
        bmp.width, bmp.height,
    0);
    return CHECK_ERROR_BEFORELOG(status,L"Write logo to graph");
}