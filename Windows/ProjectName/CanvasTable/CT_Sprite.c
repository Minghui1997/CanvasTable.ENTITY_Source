#define _CRT_SECURE_NO_WARNINGS

#include "CanvasTableSYS.h"
#include <windows.h>
#include <gl/gl.h>
#pragma comment(lib,"opengl32.lib")

extern int CT_ViewX;
extern int CT_ViewY;
extern int CT_CanvasW;
extern int CT_CanvasH;
extern int **CT_ResourceArgList;

//--------------------------------------------------------------------
// Sprite容器格式
//--------------------------------------------------------------------
// [[占位符,id,层,x,y,图像索引,子图像索引,子图像数量,子图像宽度,子图像高度,上次位置x,上次位置y,碰撞类型,边缘碰撞,子图像偏移,碰撞盒x,碰撞盒y,碰撞盒宽度,碰撞盒高度,碰撞盒中心x,碰撞盒中心y,碰撞盒半径,颜色],...]
//--------------------------------------------------------------------
int **CT_ContainerSprite = NULL; //Sprite容器
int CT_ContainerSpriteLength = 0; //Sprite容器长度
int CT_ContainerCapacity_Sprite = 0; //Sprite容器容量
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
    int vx = CT_ViewX; //视野位置x
    int vy = CT_ViewY; //视野位置y
    int i;
    for(i=0;i<CT_ContainerSpriteLength;i++) //遍历sprite容器
    {
        if(CT_ContainerSprite[i][0] != NULL) //如果当前sprite不为空
        {
            int sl = CT_ContainerSprite[i][2]; //当前sprite层
            if(sl == l) //如果当前sprite层等于渲染层
            {
                int x = CT_ContainerSprite[i][3]; //sprite位置x
                int y = CT_ContainerSprite[i][4]; //sprite位置y
                int image = CT_ContainerSprite[i][5]; //sprite图像索引
                int subimg = CT_ContainerSprite[i][6]; //sprite子图像索引
                int subimg_w = CT_ContainerSprite[i][8]; //子图像宽度
                int subimg_h = CT_ContainerSprite[i][9]; //子图像高度
                int subimg_o = CT_ContainerSprite[i][14]; //子图像偏移
                int color = CT_ContainerSprite[i][22]; //子图像偏移
                if(image != -1) //如果sprite图像不为空
                {
                    if(x + subimg_w > vx && x < vx + CT_CanvasW && y + subimg_h > vy && y < vy + CT_CanvasH) //如果当前sprite在视野内
                    {
                        int si_x = subimg_o + (subimg_w * subimg) + (subimg * subimg_o * 2); //图块位置x
                        int si_y = subimg_o; //图块位置y
                        if(color == 0xffffffff)
                        {
                            CTCanvas_ImagePart(x-vx,y-vy,image,si_x,si_y,subimg_w,subimg_h); //绘制sprite
                        }
                        else
                        {
                            CT_SpriteFlat(x-vx,y-vy,image,si_x,si_y,subimg_w,subimg_h,color,0,0,0); //绘制平面sprite
                        }
                    }
                }
            }
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// Sprite重置
//--------------------------------------------------------------------
void CT_SpriteReset()
{
    int i;
    for(i=0;i<CT_ContainerSpriteLength;i++) //遍历sprite容器
    {
        if(CT_ContainerSprite[i][0] != NULL) //如果当前sprite不为空
        {
            int spr_x = CT_ContainerSprite[i][3]; //当前sprite位置x
            int spr_y = CT_ContainerSprite[i][4]; //当前sprite位置y
            CT_ContainerSprite[i][10] = spr_x; //设置sprite上次位置x
            CT_ContainerSprite[i][11] = spr_y; //设置sprite上次位置y
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// Sprite平面
//--------------------------------------------------------------------
void CT_SpriteFlat(int x,int y,int img,int six,int siy,int siw,int sih,int col,int sx,int sy,int rot)
{
    int img_w = CT_ResourceArgList[img][2]; //获取图像宽度
    int img_h = CT_ResourceArgList[img][3]; //获取图像高度
    int tex_id = CT_ResourceArgList[img][4]; //获取纹理id
    double part1_x = (double)six / (double)img_w; //计算图像部分左下角位置x
    double part1_y = ((double)img_h - (double)siy - (double)sih) / (double)img_h; //计算图像部分左下角位置y
    double part2_x = ((double)six + (double)siw) / (double)img_w; //计算图像部分右下角位置x
    double part2_y = ((double)img_h - (double)siy - (double)sih) / (double)img_h; //计算图像部分右下角位置y
    double part3_x = ((double)six + (double)siw) / (double)img_w; //计算图像部分右上角位置x
    double part3_y = ((double)img_h - (double)siy) / (double)img_h; //计算图像部分右上角位置y
    double part4_x = (double)six / (double)img_w; //计算图像部分左上角位置x
    double part4_y = ((double)img_h - (double)siy) / (double)img_h; //计算图像部分左上角位置y
    if(col != 0xffffffff) //如果设置了sprite颜色
    {
        int r = col >> 24 & 0x000000ff; //获取颜色值 red
        int g = col >> 16 & 0x000000ff; //获取颜色值 green
        int b = col >> 8 & 0x000000ff;  //获取颜色值 blue
        int a = col & 0x000000ff; //获取颜色值 alpha
        glColor4ub(r,g,b,a); //设置颜色
    }
    glBindTexture(GL_TEXTURE_2D,tex_id); //绑定纹理
    glBegin(GL_QUADS); //开始绘制 四边形
    glTexCoord2d(part1_x,part1_y); //设置图像位置左下角坐标
    glVertex2i(x,y+sih); //设置顶点
    glTexCoord2d(part2_x,part2_y); //设置图像位置右下角坐标
    glVertex2i(x+siw,y+sih); //设置顶点
    glTexCoord2d(part3_x,part3_y); //设置图像位置右上角坐标
    glVertex2i(x+siw,y); //设置顶点
    glTexCoord2d(part4_x,part4_y); //设置图像位置左上角坐标
    glVertex2i(x,y); //设置顶点
    glEnd(); //结束绘制 四边形
    
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// Sprite容器预先分配
//--------------------------------------------------------------------
void CT_ContainerPreallocated_Sprite(int cap)
{
    int i;
    if(CT_ContainerCapacity_Sprite == 0) //如果sprite容器容量为0
    {
        CT_ContainerSprite = malloc(sizeof(int*) * cap); //sprite容器分配指针的内存
        for(i=0;i<cap;i++) //遍历容器
        {
            CT_ContainerSprite[i] = malloc(sizeof(int) * 25); //sprite容器分配内存
            memset(CT_ContainerSprite[i],0x00,sizeof(int) * 25); //sprite容器填0
        }
        CT_ContainerCapacity_Sprite = cap; //设置sprite容器容量
    }
    if(CT_ContainerCapacity_Sprite > 0 && cap > CT_ContainerCapacity_Sprite) //如果sprite容器容量大于容量
    {
        CT_ContainerSprite = realloc(CT_ContainerSprite,sizeof(int*) * cap); //重新分配sprite容器分配指针的内存
        for(i=CT_ContainerCapacity_Sprite;i<cap;i++) //遍历容器
        {
            CT_ContainerSprite[i] = malloc(sizeof(int) * 25); //sprite容器分配内存
            memset(CT_ContainerSprite[i],0x00,sizeof(int) * 25); //sprite容器填0
        }
        CT_ContainerCapacity_Sprite = cap; //设置sprite容器容量
    }
}
//--------------------------------------------------------------------