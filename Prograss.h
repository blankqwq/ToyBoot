#include <Video.h>

typedef struct Prograss
{
    UINT8 current;
    UINT8 max;
    UINT8 flag;
    EFI_GRAPHICS_OUTPUT_PROTOCOL *graph;
    UINTN width;
    UINTN height;
    UINTN x;
    UINTN y;
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL color;
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL bgColor;
} prograss_t;


EFI_PHYSICAL_ADDRESS PrograssInit(
    prograss_t *p,
    EFI_GRAPHICS_OUTPUT_PROTOCOL *graph,
    UINTN X,
    UINTN Y,
    UINTN width,
    UINTN height,
    UINTN max,
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL *bgColor);


EFI_STATUS PrograssIncrement(prograss_t *p);