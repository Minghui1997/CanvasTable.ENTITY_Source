#define _CRT_SECURE_NO_WARNINGS

#include "CanvasTableSYS.h"
#include <stdio.h>
#include <string.h>

extern int CT_ContainerDraw[10000][15];
extern char CT_ContainerDraw_String[10000][512];
extern char *CT_ContainerDraw_FontCharList_P[10000];
extern int *CT_ContainerDraw_FontGlythList_P[10000];
extern int CT_ContainerDrawLength;
extern int **CT_ResourceArgList;
extern int CT_DrawLayer;
extern int CT_DF_Img;
extern int CT_DF_Number;
extern int CT_DF_Gap;
extern int CT_DF_Height;
extern char *CT_DF_CharList;
extern int *CT_DF_GlyphList;
extern int CT_DF_TID;
extern int CT_Repaint;

//--------------------------------------------------------------------
// 设置Draw层
//--------------------------------------------------------------------
void draw_set_layer(int l)
{
    CT_DrawLayer = l; //draw层
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 清除draw
//--------------------------------------------------------------------
void draw_clear()
{
    int i;
    for(i=0;i<CT_ContainerDrawLength;i++) //遍历draw容器
    {
        if(CT_ContainerDraw[i][0] != NULL) //如果当前draw不为空
        {
            int draw_l = CT_ContainerDraw[i][2]; //当前draw层
            if(draw_l == CT_DrawLayer) //如果当前draw层等于draw层
            {
                int draw_type = 0;
                int i2;
                for(i2=0;i2<15;i2++) //遍历当前draw
                {
                    CT_ContainerDraw[i][i2] = NULL; //当前draw的当前参数为空
                }
                draw_type = CT_ContainerDraw[i][1]; //draw类型
                if(draw_type == 4) //如果draw类型是字符串
                {
                    strclear(CT_ContainerDraw_String[i]); //清除字符串
                    CT_ContainerDraw_FontCharList_P[i] = NULL;
                    CT_ContainerDraw_FontGlythList_P[i] = NULL;
                }
            }
        }
    }
    CT_Repaint = 1; //请求重绘
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 绘制矩形(容器)
//--------------------------------------------------------------------
void draw_rect(int x,int y,int w,int h,int c,int f)
{
    int type = 1; //draw类型
    //将draw数据写入draw容器
    CT_ContainerDraw[CT_ContainerDrawLength][0] = 1;
    CT_ContainerDraw[CT_ContainerDrawLength][1] = type;
    CT_ContainerDraw[CT_ContainerDrawLength][2] = CT_DrawLayer;
    CT_ContainerDraw[CT_ContainerDrawLength][3] = x;
    CT_ContainerDraw[CT_ContainerDrawLength][4] = y;
    CT_ContainerDraw[CT_ContainerDrawLength][5] = w;
    CT_ContainerDraw[CT_ContainerDrawLength][6] = h;
    CT_ContainerDraw[CT_ContainerDrawLength][7] = c;
    CT_ContainerDraw[CT_ContainerDrawLength][8] = f;
    CT_ContainerDrawLength ++; //draw容器索引 +1
    CT_Repaint = 1; //请求重绘
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 绘制线(容器)
//--------------------------------------------------------------------
void draw_line(int x1,int y1,int x2,int y2,int w,int c)
{
    int type = 2; //draw类型
    //将draw数据写入draw容器
    CT_ContainerDraw[CT_ContainerDrawLength][0] = 1;
    CT_ContainerDraw[CT_ContainerDrawLength][1] = type;
    CT_ContainerDraw[CT_ContainerDrawLength][2] = CT_DrawLayer;
    CT_ContainerDraw[CT_ContainerDrawLength][3] = x1;
    CT_ContainerDraw[CT_ContainerDrawLength][4] = y1;
    CT_ContainerDraw[CT_ContainerDrawLength][5] = x2;
    CT_ContainerDraw[CT_ContainerDrawLength][6] = y2;
    CT_ContainerDraw[CT_ContainerDrawLength][7] = w;
    CT_ContainerDraw[CT_ContainerDrawLength][8] = c;
    CT_ContainerDrawLength ++; //draw容器索引 +1
    CT_Repaint = 1; //请求重绘
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 绘制圆(容器)
//--------------------------------------------------------------------
void draw_circle(int x,int y,int r,int c,int f)
{
    int type = 3; //draw类型
    //将draw数据写入draw容器
    CT_ContainerDraw[CT_ContainerDrawLength][0] = 1;
    CT_ContainerDraw[CT_ContainerDrawLength][1] = type;
    CT_ContainerDraw[CT_ContainerDrawLength][2] = CT_DrawLayer;
    CT_ContainerDraw[CT_ContainerDrawLength][3] = x;
    CT_ContainerDraw[CT_ContainerDrawLength][4] = y;
    CT_ContainerDraw[CT_ContainerDrawLength][5] = r;
    CT_ContainerDraw[CT_ContainerDrawLength][6] = c;
    CT_ContainerDraw[CT_ContainerDrawLength][7] = f;
    CT_ContainerDrawLength ++; //draw容器索引 +1
    CT_Repaint = 1; //请求重绘
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 绘制字符串(容器)
//--------------------------------------------------------------------
void draw_string(int x,int y,int s,int c,char *t)
{
    cprint("ERROR : This function is invalid in CanvasTable.ENTITY,please use function draw_istring().");
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 绘制图像(容器)
//--------------------------------------------------------------------
void draw_image(int x,int y,int img)
{
    int type = 5; //draw类型
    if(img != -1) //如果资源不为空
    {
        int res_type = CT_ResourceArgList[img][1]; //获取资源类型
        if(res_type == 0) //如果资源类型是图像
        {
            //将draw数据写入draw容器
            CT_ContainerDraw[CT_ContainerDrawLength][0] = 1;
            CT_ContainerDraw[CT_ContainerDrawLength][1] = type;
            CT_ContainerDraw[CT_ContainerDrawLength][2] = CT_DrawLayer;
            CT_ContainerDraw[CT_ContainerDrawLength][3] = x;
            CT_ContainerDraw[CT_ContainerDrawLength][4] = y;
            CT_ContainerDraw[CT_ContainerDrawLength][5] = img;
            CT_ContainerDrawLength ++; //draw容器索引 +1
            CT_Repaint = 1; //请求重绘
        }
        else
        {
            cprint("ERROR : The resource index corresponding to the resource file is not a image.");
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 绘制图像部分(容器)
//--------------------------------------------------------------------
void draw_image_part(int x,int y,int img,int ix,int iy,int iw,int ih)
{
    int type = 6; //draw类型
    if(img != -1) //如果资源不为空
    {
        int res_type = CT_ResourceArgList[img][1]; //获取资源类型
        if(res_type == 0) //如果资源类型是图像
        {
            //将draw数据写入draw容器
            CT_ContainerDraw[CT_ContainerDrawLength][0] = 1;
            CT_ContainerDraw[CT_ContainerDrawLength][1] = type;
            CT_ContainerDraw[CT_ContainerDrawLength][2] = CT_DrawLayer;
            CT_ContainerDraw[CT_ContainerDrawLength][3] = x;
            CT_ContainerDraw[CT_ContainerDrawLength][4] = y;
            CT_ContainerDraw[CT_ContainerDrawLength][5] = img;
            CT_ContainerDraw[CT_ContainerDrawLength][6] = ix;
            CT_ContainerDraw[CT_ContainerDrawLength][7] = iy;
            CT_ContainerDraw[CT_ContainerDrawLength][8] = iw;
            CT_ContainerDraw[CT_ContainerDrawLength][9] = ih;
            CT_ContainerDrawLength ++; //draw容器索引 +1
            CT_Repaint = 1; //请求重绘
        }
        else
        {
            cprint("ERROR 4 : The resource index corresponding to the resource file is not a image.");
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 绘制曲线(容器)
//--------------------------------------------------------------------
void draw_curve(int x1,int y1,int x2,int y2,int x3,int y3,int w,int c)
{
    int type = 7; //draw类型
    CT_ContainerDraw[CT_ContainerDrawLength][0] = 1;
    CT_ContainerDraw[CT_ContainerDrawLength][1] = type;
    CT_ContainerDraw[CT_ContainerDrawLength][2] = CT_DrawLayer;
    CT_ContainerDraw[CT_ContainerDrawLength][3] = x1;
    CT_ContainerDraw[CT_ContainerDrawLength][4] = y1;
    CT_ContainerDraw[CT_ContainerDrawLength][5] = x2;
    CT_ContainerDraw[CT_ContainerDrawLength][6] = y2;
    CT_ContainerDraw[CT_ContainerDrawLength][7] = x3;
    CT_ContainerDraw[CT_ContainerDrawLength][8] = y3;
    CT_ContainerDraw[CT_ContainerDrawLength][9] = w;
    CT_ContainerDraw[CT_ContainerDrawLength][10] = c;
    CT_ContainerDrawLength ++; //draw容器索引 +1
    CT_Repaint = 1; //请求重绘
}
//--------------------------------------------------------------------
// 绘制三角形(容器)
//--------------------------------------------------------------------
void draw_tri(int x1,int y1,int x2,int y2,int x3,int y3,int c,int f)
{
    int type = 8; //draw类型
    //将draw数据写入draw容器
    CT_ContainerDraw[CT_ContainerDrawLength][0] = 1;
    CT_ContainerDraw[CT_ContainerDrawLength][1] = type;
    CT_ContainerDraw[CT_ContainerDrawLength][2] = CT_DrawLayer;
    CT_ContainerDraw[CT_ContainerDrawLength][3] = x1;
    CT_ContainerDraw[CT_ContainerDrawLength][4] = y1;
    CT_ContainerDraw[CT_ContainerDrawLength][5] = x2;
    CT_ContainerDraw[CT_ContainerDrawLength][6] = y2;
    CT_ContainerDraw[CT_ContainerDrawLength][7] = x3;
    CT_ContainerDraw[CT_ContainerDrawLength][8] = y3;
    CT_ContainerDraw[CT_ContainerDrawLength][9] = c;
    CT_ContainerDraw[CT_ContainerDrawLength][10] = f;
    CT_ContainerDrawLength ++; //draw容器索引 +1
    CT_Repaint = 1; //请求重绘
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 绘制图像字符串(容器)
//--------------------------------------------------------------------
void draw_istring(int x,int y,int c,CTimgfont *font,char *t)
{
    int type = 9; //draw类型
    int font_img = CT_DF_Img; //字体图像
    int font_charnum = CT_DF_Number; //字体字符数量
    int font_chargap = CT_DF_Gap; //字体字符间隙
    int font_charheight = CT_DF_Height; //字体字符高度
    char *font_charlist = CT_DF_CharList; //字体字符列表
    int *font_glyphlist = CT_DF_GlyphList; //字体字形列表
    if(font != NULL) //如果不是默认字体
    {
        font_img = font->FontImg; //获取字体图像
        if(font_img != -1 && font->CharNumber > 0 && font->CharList != NULL && font->GlyphList != NULL) //如果字体属性正确
        {
            font_img = font->FontImg; //字体图像
            font_charnum = font->CharNumber; //字体字符数量
            font_chargap = font->CharGap; //字体字符间隙
            font_charheight = font->CharHeight; //字体字符高度
            font_charlist = font->CharList; //字体字符列表
            font_glyphlist = font->GlyphList; //字体字形列表
        }
        else
        {
            cprint("ERROR : The imgfont property have error.");
        }
    }
    CT_ContainerDraw[CT_ContainerDrawLength][0] = 1;
    CT_ContainerDraw[CT_ContainerDrawLength][1] = type;
    CT_ContainerDraw[CT_ContainerDrawLength][2] = CT_DrawLayer;
    CT_ContainerDraw[CT_ContainerDrawLength][3] = x;
    CT_ContainerDraw[CT_ContainerDrawLength][4] = y;
    CT_ContainerDraw[CT_ContainerDrawLength][5] = c;
    CT_ContainerDraw[CT_ContainerDrawLength][6] = font_img;
    CT_ContainerDraw[CT_ContainerDrawLength][7] = font_charnum;
    CT_ContainerDraw[CT_ContainerDrawLength][8] = font_chargap;
    CT_ContainerDraw[CT_ContainerDrawLength][9] = font_charheight;
    CT_ContainerDraw_FontCharList_P[CT_ContainerDrawLength] = font_charlist;
    CT_ContainerDraw_FontGlythList_P[CT_ContainerDrawLength] = font_glyphlist;
    strcpy(CT_ContainerDraw_String[CT_ContainerDrawLength],t); //将字符串写入draw_str容器
    CT_ContainerDrawLength ++; //draw容器索引 +1
    CT_Repaint = 1; //请求重绘
}
//--------------------------------------------------------------------