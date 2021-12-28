#include "Setup.h"
#include <Prograss.h>
#include <Video.h>
//画进度条
EFI_PHYSICAL_ADDRESS PrograssInit(
    prograss_t *p,
    EFI_GRAPHICS_OUTPUT_PROTOCOL *graph,
    UINTN X,
    UINTN Y,
    UINTN width,
    UINTN height,
    UINTN max,
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL *bgColor
)
{
    p->graph = graph;
    p->height = height;
    p->width = width;
    //剧中计算
    p->x = X;
    p->y = Y;
    p->flag = 0;
    p->current = 0;
    p->max = max;
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL white = {255, 255, 255, 0};
    p->color = white;
    p->bgColor = *bgColor;
    return EFI_SUCCESS;
}

EFI_STATUS PrograssIncrement(prograss_t *p)
{
    // 画进度条
    if(p->current==p->max){
        p->current=0;
        p->flag =!p->flag;
    }
    p->current++;
    EFI_STATUS status = EFI_SUCCESS;
    UINTN X = p->x + p->width * p->current;
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL color;
    if ((p->current+p->flag) % 2 == 0)
    {
        color=p->bgColor;
    }
    else
    {
        color=p->color;
    }
    //还没写完就被下一个给写完了
    status = FillColorToVideo(p->graph, &color, X, p->y, p->width, p->height);
    return CHECK_ERROR_BEFORELOG(status, L"Prograss load ...");
}
