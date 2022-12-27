#define _CRT_SECURE_NO_WARNINGS

#include "CanvasTableSYS.h"
#include <stdio.h>

extern int CT_ViewX;
extern int CT_ViewY;

//--------------------------------------------------------------------
// Sprite容器格式
//--------------------------------------------------------------------
// [[占位符,id,层,x,y,图像索引,子图像索引,子图像数量,子图像宽度,子图像高度,上次位置x,上次位置y,碰撞类型,边缘碰撞,子图像偏移,碰撞盒x,碰撞盒y,碰撞盒宽度,碰撞盒高度,碰撞盒中心x,碰撞盒中心y,碰撞盒半径],...]
//--------------------------------------------------------------------
int CT_ContainerSprite[10000][25] = {NULL}; //Sprite容器
int CT_ContainerSpriteLength = 0; //Sprite容器长度
int CT_SpriteId = 0; //Sprite对象ID
//--------------------------------------------------------------------
// Sprite对象ID
//--------------------------------------------------------------------
int CT_SpriteGetId()
{
    int id = CT_SpriteId; //当前Sprite Id
    CT_SpriteId ++; //Sprite Id +1
    return id; //返回 Sprite Id
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 渲染Sprite
//--------------------------------------------------------------------
void CT_SpriteRender(int l)
{
    int i;
    for(i=0;i<CT_ContainerSpriteLength;i++) //遍历Sprite容器
    {
        if(CT_ContainerSprite[i][0] != NULL) //如果当前Sprite不为空
        {
            int sl = CT_ContainerSprite[i][2]; //当前Sprite层
            if(sl == l) //如果当前Sprite层等于渲染层
            {
                int x = CT_ContainerSprite[i][3]; //sprite位置x
                int y = CT_ContainerSprite[i][4]; //sprite位置y
                int image = CT_ContainerSprite[i][5]; //sprite图像索引
                int subimg = CT_ContainerSprite[i][6]; //sprite子图像索引
                int subimg_w = CT_ContainerSprite[i][8]; //子图像宽度
                int subimg_h = CT_ContainerSprite[i][9]; //子图像高度
                int subimg_o = CT_ContainerSprite[i][14]; //子图像偏移
                if(image != -1) //如果sprite图像不为空
                {
                    int vx = CT_ViewX; //视野位置x
                    int vy = CT_ViewY; //视野位置y
                    int si_x = subimg_o + (subimg_w * subimg) + (subimg * subimg_o * 2); //图块位置x
                    int si_y = subimg_o; //图块位置y
                    CTCanvas_ImagePart(x-vx,y-vy,image,si_x,si_y,subimg_w,subimg_h); //绘制sprite
                }
            }
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// Sprite位置记录
//--------------------------------------------------------------------
void CT_SpritePosRecord()
{
    int i;
    for(i=0;i<CT_ContainerSpriteLength;i++) //遍历Sprite容器
    {
        if(CT_ContainerSprite[i][0] != NULL) //如果当前Sprite不为空
        {
            int spr_x = CT_ContainerSprite[i][3]; //当前sprite位置x
            int spr_y = CT_ContainerSprite[i][4]; //当前sprite位置y
            CT_ContainerSprite[i][10] = spr_x; //设置sprite上次位置x
            CT_ContainerSprite[i][11] = spr_y; //设置sprite上次位置y
        }
    }
}
//--------------------------------------------------------------------