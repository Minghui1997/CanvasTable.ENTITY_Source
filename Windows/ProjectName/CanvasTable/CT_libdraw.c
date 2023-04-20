#define _CRT_SECURE_NO_WARNINGS

#include "CanvasTableSYS.h"

extern int *CT_ContainerDraw;
extern int CT_DrawLayerStatusList[32];
extern char *CT_ContainerDraw_String;
extern char **CT_ContainerDraw_FontCharList_P;
extern int **CT_ContainerDraw_FontGlyphList_P;
extern int CT_ContainerDrawLength;
extern int CT_ContainerMaxLength_Draw;
extern unsigned int *CT_ResourceArgList;
extern int CT_DrawLayer;
extern int CT_DF_Img;
extern int CT_DF_Number;
extern int CT_DF_Gap;
extern int CT_DF_Height;
extern char *CT_DF_CharList;
extern int *CT_DF_GlyphList;
extern int CT_DF_TID;
extern int CT_Repaint;
extern int CT_DrawAlpha;

//--------------------------------------------------------------------
// Draw层设置
//--------------------------------------------------------------------
void draw_set_layer(int l)
{
    if(l >= 0 && l <= 31) //如果设置的层在正确范围内
    {
        CT_DrawLayer = l; //设置draw层
    }
    else
    {
        cprint("ERROR : The draw layer is error.");
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// draw清除
//--------------------------------------------------------------------
void draw_clear()
{
    int i;
    size_t data_i; //数据索引
    for(i=0;i<CT_ContainerDrawLength;i++) //遍历draw容器
    {
        data_i = i * 15; //获取当前draw数据索引
        if(CT_ContainerDraw[data_i] != 0) //如果当前draw不为空
        {
            int draw_l = CT_ContainerDraw[data_i+2]; //当前draw层
            if(draw_l == CT_DrawLayer) //如果当前draw层等于draw层
            {
                int draw_type = CT_ContainerDraw[data_i+1]; //draw类型;
                int i2;
                for(i2=0;i2<15;i2++) //遍历当前draw
                {
                    CT_ContainerDraw[data_i+i2] = 0; //清除当前draw数据
                }
                if(draw_type == 4 || draw_type == 9) //如果draw类型是字符串
                {
                    char *draw_p = CT_ContainerDraw_String + i * 513; //draw字符串容器指针
                    strclear(draw_p); //清除字符串
                    CT_ContainerDraw_FontCharList_P[i] = NULL; //清除字符列表指针
                    CT_ContainerDraw_FontGlyphList_P[i] = NULL; //清除字形列表指针
                }
            }
        }
    }
    for(i=CT_ContainerDrawLength-1;i>=-1;i--) //反向遍历draw容器
    {
        if(i >= 0) //如果draw索引大于等于0
        {
            data_i = i * 15; //获取当前draw数据索引
            if(CT_ContainerDraw[data_i] != 0) //如果当前draw不为空
            {
                CT_ContainerDrawLength = i + 1; //确定draw容器长度
                break; //中断
            }
        }
        else
        {
            CT_ContainerDrawLength = 0; //draw容器长度为0
        }
    }
    CT_DrawLayerStatusList[CT_DrawLayer] = 0; //将当前draw层的状态标记为0
    CT_Repaint = 1; //请求重绘
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// draw透明度设置
//--------------------------------------------------------------------
void draw_set_alpha(int a)
{
    if(a >= 0 && a <= 255) //如果alpha值在正确范围内
    {
        CT_DrawAlpha = a; //设置draw alpha
    }
    else
    {
        cprint("ERROR : The color value is error.");
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 绘制矩形(容器)
//--------------------------------------------------------------------
void draw_rect(int x,int y,int w,int h,int c,int f)
{
    int type = 1; //draw类型
    int col = (c << 8) + 255; //颜色值
    size_t data_i = CT_ContainerDrawLength * 15; //draw数据索引
    //将draw数据写入draw容器
    CT_ContainerDraw[data_i] = 1;
    CT_ContainerDraw[data_i+1] = type;
    CT_ContainerDraw[data_i+2] = CT_DrawLayer;
    CT_ContainerDraw[data_i+3] = x;
    CT_ContainerDraw[data_i+4] = y;
    CT_ContainerDraw[data_i+5] = w;
    CT_ContainerDraw[data_i+6] = h;
    CT_ContainerDraw[data_i+7] = col;
    CT_ContainerDraw[data_i+8] = f;
    CT_ContainerDrawLength ++; //draw容器索引 +1
    if(CT_ContainerDrawLength == CT_ContainerMaxLength_Draw) //如果draw容器长度等于draw容器最大长度
    {
        CT_ContainerPreallocated_Draw(CT_ContainerDrawLength+300); //为draw容器重新分配内存
    }
    CT_DrawLayerStatusList[CT_DrawLayer] = 1; //将当前draw层的状态标记为1
    CT_Repaint = 1; //请求重绘
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 绘制线(容器)
//--------------------------------------------------------------------
void draw_line(int x1,int y1,int x2,int y2,int w,int c)
{
    int type = 2; //draw类型
    int col = (c << 8) + 255; //颜色值
    size_t data_i = CT_ContainerDrawLength * 15; //draw数据索引
    //将draw数据写入draw容器
    CT_ContainerDraw[data_i] = 1;
    CT_ContainerDraw[data_i+1] = type;
    CT_ContainerDraw[data_i+2] = CT_DrawLayer;
    CT_ContainerDraw[data_i+3] = x1;
    CT_ContainerDraw[data_i+4] = y1;
    CT_ContainerDraw[data_i+5] = x2;
    CT_ContainerDraw[data_i+6] = y2;
    CT_ContainerDraw[data_i+7] = w;
    CT_ContainerDraw[data_i+8] = col;
    CT_ContainerDrawLength ++; //draw容器索引 +1
    if(CT_ContainerDrawLength == CT_ContainerMaxLength_Draw) //如果draw容器长度等于draw容器最大长度
    {
        CT_ContainerPreallocated_Draw(CT_ContainerDrawLength+300); //为draw容器重新分配内存
    }
    CT_DrawLayerStatusList[CT_DrawLayer] = 1; //将当前draw层的状态标记为1
    CT_Repaint = 1; //请求重绘
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 绘制圆(容器)
//--------------------------------------------------------------------
void draw_circle(int x,int y,int r,int c,int f)
{
    int type = 3; //draw类型
    int col = (c << 8) + 255; //颜色值
    size_t data_i = CT_ContainerDrawLength * 15; //draw数据索引
    //将draw数据写入draw容器
    CT_ContainerDraw[data_i] = 1;
    CT_ContainerDraw[data_i+1] = type;
    CT_ContainerDraw[data_i+2] = CT_DrawLayer;
    CT_ContainerDraw[data_i+3] = x;
    CT_ContainerDraw[data_i+4] = y;
    CT_ContainerDraw[data_i+5] = r;
    CT_ContainerDraw[data_i+6] = col;
    CT_ContainerDraw[data_i+7] = f;
    CT_ContainerDrawLength ++; //draw容器索引 +1
    if(CT_ContainerDrawLength == CT_ContainerMaxLength_Draw) //如果draw容器长度等于draw容器最大长度
    {
        CT_ContainerPreallocated_Draw(CT_ContainerDrawLength+300); //为draw容器重新分配内存
    }
    CT_DrawLayerStatusList[CT_DrawLayer] = 1; //将当前draw层的状态标记为1
    CT_Repaint = 1; //请求重绘
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 绘制字符串(容器)
//--------------------------------------------------------------------
void draw_string(int x,int y,int s,int c,char *t)
{
    int type = 4; //draw类型
    int col = (c << 8) + CT_DrawAlpha; //颜色值
    size_t data_i = CT_ContainerDrawLength * 15; //draw数据索引
    char *str_p = CT_ContainerDraw_String + CT_ContainerDrawLength * 513; //draw字符串容器指针
    //将draw数据写入draw容器
    CT_ContainerDraw[data_i] = 1;
    CT_ContainerDraw[data_i+1] = type;
    CT_ContainerDraw[data_i+2] = CT_DrawLayer;
    CT_ContainerDraw[data_i+3] = x;
    CT_ContainerDraw[data_i+4] = y;
    CT_ContainerDraw[data_i+5] = s;
    CT_ContainerDraw[data_i+6] = col;
    strcpy(str_p,t); //将字符串写入draw字符串容器
    CT_ContainerDrawLength ++; //draw容器索引 +1
    if(CT_ContainerDrawLength == CT_ContainerMaxLength_Draw) //如果draw容器长度等于draw容器最大长度
    {
        CT_ContainerPreallocated_Draw(CT_ContainerDrawLength+300); //为draw容器重新分配内存
    }
    CT_DrawLayerStatusList[CT_DrawLayer] = 1; //将当前draw层的状态标记为1
    CT_Repaint = 1; //请求重绘
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 绘制图像(容器)
//--------------------------------------------------------------------
void draw_image(int x,int y,int img)
{
    int type = 5; //draw类型
    size_t data_i = CT_ContainerDrawLength * 15; //draw数据索引
    if(img != -1) //如果资源不为空
    {
        int res_type = CT_ResourceArgList[img*5+1]; //获取资源类型
        if(res_type == 0) //如果资源类型是图像
        {
            //将draw数据写入draw容器
            CT_ContainerDraw[data_i] = 1;
            CT_ContainerDraw[data_i+1] = type;
            CT_ContainerDraw[data_i+2] = CT_DrawLayer;
            CT_ContainerDraw[data_i+3] = x;
            CT_ContainerDraw[data_i+4] = y;
            CT_ContainerDraw[data_i+5] = img;
            CT_ContainerDraw[data_i+6] = CT_DrawAlpha;
            CT_ContainerDrawLength ++; //draw容器索引 +1
            if(CT_ContainerDrawLength == CT_ContainerMaxLength_Draw) //如果draw容器长度等于draw容器最大长度
            {
                CT_ContainerPreallocated_Draw(CT_ContainerDrawLength+300); //为draw容器重新分配内存
            }
            CT_DrawLayerStatusList[CT_DrawLayer] = 1; //将当前draw层的状态标记为1
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
void draw_image_part(int x,int y,int img,unsigned int ix,unsigned int iy,unsigned int iw,unsigned int ih)
{
    int type = 6; //draw类型
    size_t data_i = CT_ContainerDrawLength * 15; //draw数据索引
    if(img != -1) //如果资源不为空
    {
        int res_type = CT_ResourceArgList[img*5+1]; //获取资源类型
        if(res_type == 0) //如果资源类型是图像
        {
            //将draw数据写入draw容器
            CT_ContainerDraw[data_i] = 1;
            CT_ContainerDraw[data_i+1] = type;
            CT_ContainerDraw[data_i+2] = CT_DrawLayer;
            CT_ContainerDraw[data_i+3] = x;
            CT_ContainerDraw[data_i+4] = y;
            CT_ContainerDraw[data_i+5] = img;
            CT_ContainerDraw[data_i+6] = ix;
            CT_ContainerDraw[data_i+7] = iy;
            CT_ContainerDraw[data_i+8] = iw;
            CT_ContainerDraw[data_i+9] = ih;
            CT_ContainerDraw[data_i+10] = CT_DrawAlpha;
            CT_ContainerDrawLength ++; //draw容器索引 +1
            if(CT_ContainerDrawLength == CT_ContainerMaxLength_Draw) //如果draw容器长度等于draw容器最大长度
            {
                CT_ContainerPreallocated_Draw(CT_ContainerDrawLength+300); //为draw容器重新分配内存
            }
            CT_DrawLayerStatusList[CT_DrawLayer] = 1; //将当前draw层的状态标记为1
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
// 绘制曲线(容器)
//--------------------------------------------------------------------
void draw_curve(int x1,int y1,int x2,int y2,int x3,int y3,int w,int c)
{
    int type = 7; //draw类型
    int col = (c << 8) + 255; //颜色值
    size_t data_i = CT_ContainerDrawLength * 15; //draw数据索引
    //将draw数据写入draw容器
    CT_ContainerDraw[data_i] = 1;
    CT_ContainerDraw[data_i+1] = type;
    CT_ContainerDraw[data_i+2] = CT_DrawLayer;
    CT_ContainerDraw[data_i+3] = x1;
    CT_ContainerDraw[data_i+4] = y1;
    CT_ContainerDraw[data_i+5] = x2;
    CT_ContainerDraw[data_i+6] = y2;
    CT_ContainerDraw[data_i+7] = x3;
    CT_ContainerDraw[data_i+8] = y3;
    CT_ContainerDraw[data_i+9] = w;
    CT_ContainerDraw[data_i+10] = col;
    CT_ContainerDrawLength ++; //draw容器索引 +1
    if(CT_ContainerDrawLength == CT_ContainerMaxLength_Draw) //如果draw容器长度等于draw容器最大长度
    {
        CT_ContainerPreallocated_Draw(CT_ContainerDrawLength+300); //为draw容器重新分配内存
    }
    CT_DrawLayerStatusList[CT_DrawLayer] = 1; //将当前draw层的状态标记为1
    CT_Repaint = 1; //请求重绘
}
//--------------------------------------------------------------------
// 绘制三角形(容器)
//--------------------------------------------------------------------
void draw_tri(int x1,int y1,int x2,int y2,int x3,int y3,int c,int f)
{
    int type = 8; //draw类型
    int col = (c << 8) + 255; //颜色值
    size_t data_i = CT_ContainerDrawLength * 15; //draw数据索引
    //将draw数据写入draw容器
    CT_ContainerDraw[data_i] = 1;
    CT_ContainerDraw[data_i+1] = type;
    CT_ContainerDraw[data_i+2] = CT_DrawLayer;
    CT_ContainerDraw[data_i+3] = x1;
    CT_ContainerDraw[data_i+4] = y1;
    CT_ContainerDraw[data_i+5] = x2;
    CT_ContainerDraw[data_i+6] = y2;
    CT_ContainerDraw[data_i+7] = x3;
    CT_ContainerDraw[data_i+8] = y3;
    CT_ContainerDraw[data_i+9] = col;
    CT_ContainerDraw[data_i+10] = f;
    CT_ContainerDrawLength ++; //draw容器索引 +1
    if(CT_ContainerDrawLength == CT_ContainerMaxLength_Draw) //如果draw容器长度等于draw容器最大长度
    {
        CT_ContainerPreallocated_Draw(CT_ContainerDrawLength+300); //为draw容器重新分配内存
    }
    CT_DrawLayerStatusList[CT_DrawLayer] = 1; //将当前draw层的状态标记为1
    CT_Repaint = 1; //请求重绘
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 绘制图像字符串(容器)
//--------------------------------------------------------------------
void draw_istring(int x,int y,int c,CTimgfont *font,char *t)
{
    int type = 9; //draw类型
    int col = (c << 8) + CT_DrawAlpha; //颜色值
    size_t data_i = CT_ContainerDrawLength * 15; //draw数据索引
    int font_img = CT_DF_Img; //字体图像
    int font_charnum = CT_DF_Number; //字体字符数量
    int font_chargap = CT_DF_Gap; //字体字符间隙
    int font_charheight = CT_DF_Height; //字体字符高度
    char *font_charlist = CT_DF_CharList; //字体字符列表
    int *font_glyphlist = CT_DF_GlyphList; //字体字形列表
    char *str_p = CT_ContainerDraw_String + CT_ContainerDrawLength * 513; //draw字符串容器指针
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
            cprint("ERROR : Invalid imgfont.");
        }
    }
    //将draw数据写入draw容器
    CT_ContainerDraw[data_i] = 1;
    CT_ContainerDraw[data_i+1] = type;
    CT_ContainerDraw[data_i+2] = CT_DrawLayer;
    CT_ContainerDraw[data_i+3] = x;
    CT_ContainerDraw[data_i+4] = y;
    CT_ContainerDraw[data_i+5] = col;
    CT_ContainerDraw[data_i+6] = font_img;
    CT_ContainerDraw[data_i+7] = font_charnum;
    CT_ContainerDraw[data_i+8] = font_chargap;
    CT_ContainerDraw[data_i+9] = font_charheight;
    CT_ContainerDraw_FontCharList_P[CT_ContainerDrawLength] = font_charlist;
    CT_ContainerDraw_FontGlyphList_P[CT_ContainerDrawLength] = font_glyphlist;
    strcpy(str_p,t); //将字符串写入draw字符串容器
    CT_ContainerDrawLength ++; //draw容器索引 +1
    if(CT_ContainerDrawLength == CT_ContainerMaxLength_Draw) //如果draw容器长度等于draw容器最大长度
    {
        CT_ContainerPreallocated_Draw(CT_ContainerDrawLength+300); //为draw容器重新分配内存
    }
    CT_DrawLayerStatusList[CT_DrawLayer] = 1; //将当前draw层的状态标记为1
    CT_Repaint = 1; //请求重绘
}
//--------------------------------------------------------------------