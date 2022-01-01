#include <Setup.h>
#include <Load.h>
#include <Protocol.h>


EFI_STATUS getVideoProtocol(
    IN EFI_HANDLE imageHandle,
    IN UINT8 index,
    OUT EFI_GRAPHICS_OUTPUT_PROTOCOL **graph);



EFI_STATUS LoadLogo(
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *file,
    EFI_GRAPHICS_OUTPUT_PROTOCOL *graph,
    CHAR16 *filename,
    UINTN X,UINTN Y
);

EFI_STATUS ResizeVideo(
    EFI_GRAPHICS_OUTPUT_PROTOCOL *graph
);

EFI_STATUS LoadBmpToVideo(   
    EFI_GRAPHICS_OUTPUT_PROTOCOL *graph,
     EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *file,
    CHAR16 *filename,
    UINTN offsetX,
    UINTN offsetY
);

EFI_STATUS FillColorToVideo(
    EFI_GRAPHICS_OUTPUT_PROTOCOL *graph, 
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL *color, 
    UINTN X, UINTN Y, 
    UINTN width, UINTN height);



#ifndef _VIDEO_CONFIG_STRUCT_
#define _VIDEO_CONFIG_STRUCT_
typedef struct VideoConfigStruct{

}VideoConfig;
#endif
