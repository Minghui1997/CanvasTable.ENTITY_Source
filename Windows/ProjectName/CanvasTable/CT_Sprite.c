#define _CRT_SECURE_NO_WARNINGS

#include "CanvasTableSYS.h"

extern int CT_ViewX;
extern int CT_ViewY;
extern int CT_CanvasW;
extern int CT_CanvasH;
extern unsigned int *CT_ResourceArgList;

//--------------------------------------------------------------------
// sprite容器格式
//--------------------------------------------------------------------
// [占位符,id,层,x,y,图像索引,子图像索引,子图像数量,子图像宽度,子图像高度,上次位置x,上次位置y,碰撞类型,边缘碰撞,空,碰撞盒x,碰撞盒y,碰撞盒宽度,碰撞盒高度,碰撞盒中心x,碰撞盒中心y,碰撞盒半径,颜色,...]
//--------------------------------------------------------------------
int *CT_ContainerSprite = NULL; //Sprite容器
int CT_SpriteLayerStatusList[32] = {0}; //Sprite层状态列表
int CT_ContainerSpriteLength = 0; //Sprite容器长度
int CT_ContainerMaxLength_Sprite = 0; //Sprite容器最大长度
int CT_SpriteId = 0; //Sprite对象ID
//--------------------------------------------------------------------
// sprite对象ID
//--------------------------------------------------------------------
int CT_SpriteGetId()
{
    int id = CT_SpriteId; //当前Sprite Id
    CT_SpriteId ++; //Sprite Id +1
    if(CT_SpriteId > 2000000000) //如果Sprite Id等于2000000000
    {
        CT_SpriteId = 1000000000; //Sprite Id等于1000000000
    }
    return id; //返回 Sprite Id
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// sprite渲染
//--------------------------------------------------------------------
void CT_SpriteRender(int l)
{
    int vx = CT_ViewX; //视野位置x
    int vy = CT_ViewY; //视野位置y
    int i;
    size_t data_i;
    for(i=0;i<CT_ContainerSpriteLength;i++) //遍历sprite容器
    {
        data_i = i * 25; //sprite数据索引
        if(CT_ContainerSprite[data_i] != 0) //如果当前sprite不为空
        {
            int sl = CT_ContainerSprite[data_i+2]; //当前sprite层
            if(sl == l) //如果当前sprite层等于渲染层
            {
                int x = CT_ContainerSprite[data_i+3]; //sprite位置x
                int y = CT_ContainerSprite[data_i+4]; //sprite位置y
                int image = CT_ContainerSprite[data_i+5]; //sprite图像索引
                int subimg = CT_ContainerSprite[data_i+6]; //sprite子图像索引
                int subimg_w = CT_ContainerSprite[data_i+8]; //子图像宽度
                int subimg_h = CT_ContainerSprite[data_i+9]; //子图像高度
                int color = CT_ContainerSprite[data_i+22]; //子图像颜色
                if(image != -1) //如果sprite图像不为空
                {
                    if(x + subimg_w > vx && x < vx + CT_CanvasW && y + subimg_h > vy && y < vy + CT_CanvasH) //如果当前sprite在视野内
                    {
                        unsigned int si_x = subimg_w * subimg; //图块位置x
                        if(color == 0xffffffff)
                        {
                            CT_OffCanvas_ImagePart(x-vx,y-vy,image,255,si_x,0,subimg_w,subimg_h); //绘制sprite
                        }
                        else
                        {
                            CT_OffCanvas_ImagePartFlat(x-vx,y-vy,image,si_x,0,subimg_w,subimg_h,color,0,0,0); //绘制平面sprite
                        }
                    }
                }
            }
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// sprite结束
//--------------------------------------------------------------------
void CT_SpriteEnd()
{
    int i;
    size_t data_i;
    for(i=0;i<CT_ContainerSpriteLength;i++) //遍历sprite容器
    {
        data_i = i * 25; //sprite数据索引
        if(CT_ContainerSprite[data_i] != 0) //如果当前sprite不为空
        {
            int spr_x = CT_ContainerSprite[data_i+3]; //当前sprite位置x
            int spr_y = CT_ContainerSprite[data_i+4]; //当前sprite位置y
            CT_ContainerSprite[data_i+10] = spr_x; //设置sprite上次位置x
            CT_ContainerSprite[data_i+11] = spr_y; //设置sprite上次位置y
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// sprite容器预先分配
//--------------------------------------------------------------------
void CT_ContainerPreallocated_Sprite(int max_l)
{
    size_t cs_cap = sizeof(int) * 25 * max_l; //sprite容器容量
    if(CT_ContainerMaxLength_Sprite == 0) //如果sprite容器最大长度为0
    {
        CT_ContainerSprite = malloc(cs_cap); //sprite容器分配内存
        CT_ContainerMaxLength_Sprite = max_l; //设置sprite容器最大长度
    }
    if(CT_ContainerMaxLength_Sprite > 0 && max_l > CT_ContainerMaxLength_Sprite) //如果sprite容器长度大于sprite容器最大长度
    {
        size_t cs_nbs = cs_cap - sizeof(int) * 25 * CT_ContainerMaxLength_Sprite; //sprite容器新内存块大小
        CT_ContainerSprite = realloc(CT_ContainerSprite,cs_cap); //重新分配sprite容器的内存
        memset(CT_ContainerSprite+CT_ContainerMaxLength_Sprite*25,0x00,cs_nbs); //为sprite容器的新空间填0
        CT_ContainerMaxLength_Sprite = max_l; //设置sprite容器最大长度
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// sprite容器清除
//--------------------------------------------------------------------
void CT_ContainerSpriteClear()
{
    int i;
    size_t data_i;
    int i2;
    for(i=0;i<CT_ContainerSpriteLength;i++) //遍历sprite容器
    {
        data_i = i * 25; //sprite数据索引
        if(CT_ContainerSprite[data_i] != 0) //如果当前sprite不为空
        {
            for(i2=0;i2<25;i2++) //遍历sprite数据
            {
                CT_ContainerSprite[data_i+i2] = 0; //清除sprite数据
            }
        }
    }
    for(i2=0;i2<32;i2++) //遍历层状态列表
    {
        CT_SpriteLayerStatusList[i2] = 0; //将当前sprite层状态标记为0
    }
    CT_ContainerSpriteLength = 0; //Sprite容器长度为0
}
//--------------------------------------------------------------------