#include "CanvasTableSYS.h"
#include <windows.h>
#include <gl/gl.h>
#pragma comment(lib,"opengl32.lib")

extern int **CT_ResourceArgList;
extern char CT_DefaultFont_Image[62992];
extern char CT_DefaultFont_CharList[109][4];
extern int CT_DefaultFont_GlythList[109][4];

int CT_DF_Img = -1; //字体图像
int CT_DF_Number = 0; //字体字符数量
int CT_DF_Gap = 0; //字体字符间隙
int CT_DF_Height = 0; //字体字符高度
char *CT_DF_CharList = NULL; //字体字符列表
int *CT_DF_GlyphList = NULL; //字体字形列表
int CT_DF_TID = 0; //默认字体纹理id

//--------------------------------------------------------------------
// 绘制图像字符串(缓冲)
//--------------------------------------------------------------------
void CT_BufferImgString(int x,int y,int c,char *str,int font_img,int font_number,int font_gap,char *font_charlist,int *font_glyphlist)
{
    char _char[5] = ""; //当前字符
    char list_char[5] = ""; //列表当前字符
    int str_l = strlen(str); //字符串长度
    int offset = 0; //字符串偏移量
    int str_i = 0; //字符串索引
    int cx = x; //字符绘制位置x
    while(str_i < str_l) //遍历字符串
    {
        int find_char = 0; //找到字符
        int i2;
        if(str[str_i] < 0) //如果当前值大于0x7f
        {
            _char[0] = str[str_i]; //当前字符字节1
            _char[1] = str[str_i+1]; //当前字符字节2
            offset = 2; //字符串偏移量
        }
        else
        {
            _char[0] = str[str_i]; //当前字符字节1
            offset = 1; //字符串偏移量
        }
        for(i2=0;i2<font_number;i2++) //遍历字符列表
        {
            list_char[0] = font_charlist[i2*4+0]; //复制当前字符的字节1
            list_char[1] = font_charlist[i2*4+1]; //复制当前字符的字节2
            list_char[2] = font_charlist[i2*4+2]; //复制当前字符的字节3
            list_char[3] = font_charlist[i2*4+3]; //复制当前字符的字节4
            if(strcmp(list_char,_char) == 0) //如果字符串当前字符等于列表中的当前字符
            {
                int gx = font_glyphlist[i2*4]; //字形位置x
                int gy = font_glyphlist[i2*4+1]; //字形位置y
                int gw = font_glyphlist[i2*4+2]; //字形位置w
                int gh = font_glyphlist[i2*4+3]; //字形位置h
                CT_BufferFontGlyph(cx,y,font_img,gx,gy,gw,gh,c); //绘制字体字形
                cx += gw + font_gap; //绘制下一个字符的位置
                find_char = 1; //已找到字符
                break; //中断
            }
        }
        str_i += offset; //字符串偏移到下一个字符
        _char[0] = 0x00;
        _char[1] = 0x00;
        if(find_char == 0) //如果没有找到字符
        {
            cprint("ERROR 26 : Not find char in charlist.");
        }
        
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 绘制字体字形(缓冲)
//--------------------------------------------------------------------
void CT_BufferFontGlyph(int x,int y,int i,int ix,int iy,int iw,int ih,int c)
{
    int img_w = 0; //图像宽度
    int img_h = 0; //图像高度
    int tex_id = 0; //纹理id
    double part1_x = 0; //图像部分左下角位置x
    double part1_y = 0; //图像部分左下角位置y
    double part2_x = 0; //图像部分右下角位置x
    double part2_y = 0; //图像部分右下角位置y
    double part3_x = 0; //图像部分右上角位置x
    double part3_y = 0; //图像部分右上角位置y
    double part4_x = 0; //图像部分左上角位置x
    double part4_y = 0; //图像部分左上角位置y
    int r = c >> 24 & 0x000000ff; //获取颜色值 red
    int g = c >> 16 & 0x000000ff; //获取颜色值 green
    int b = c >> 8 & 0x000000ff; //获取颜色值 blue
    int a = c & 0x000000ff; //获取颜色值 alpha
    if(i != -1) //如果图像字体不是默认字体
    {
        img_w = CT_ResourceArgList[i][2]; //获取图像宽度
        img_h = CT_ResourceArgList[i][3]; //获取图像高度
        tex_id = CT_ResourceArgList[i][4]; //获取纹理id
    }
    else
    {
        img_w = 254; //默认字体图像宽度
        img_h = 62; //默认字体图像高度
        tex_id = CT_DF_TID; //默认字体纹理id
    }
    glBindTexture(GL_TEXTURE_2D,tex_id); //绑定纹理
    part1_x = (double)ix / (double)img_w; //计算图像部分左下角位置x
    part1_y = ((double)img_h - (double)iy - (double)ih) / (double)img_h; //计算图像部分左下角位置y
    part2_x = ((double)ix + (double)iw) / (double)img_w; //计算图像部分右下角位置x
    part2_y = ((double)img_h - (double)iy - (double)ih) / (double)img_h; //计算图像部分右下角位置y
    part3_x = ((double)ix + (double)iw) / (double)img_w; //计算图像部分右上角位置x
    part3_y = ((double)img_h - (double)iy) / (double)img_h; //计算图像部分右上角位置y
    part4_x = (double)ix / (double)img_w; //计算图像部分左上角位置x
    part4_y = ((double)img_h - (double)iy) / (double)img_h; //计算图像部分左上角位置y
    glColor4ub(r,g,b,a); //设置颜色
    glBegin(GL_QUADS); //开始绘制 四边形
    glTexCoord2d(part1_x,part1_y); //设置图像位置左下角坐标
    glVertex2i(x,y+ih); //设置顶点
    glTexCoord2d(part2_x,part2_y); //设置图像位置右下角坐标
    glVertex2i(x+iw,y+ih); //设置顶点
    glTexCoord2d(part3_x,part3_y); //设置图像位置右上角坐标
    glVertex2i(x+iw,y); //设置顶点
    glTexCoord2d(part4_x,part4_y); //设置图像位置左上角坐标
    glVertex2i(x,y); //设置顶点
    glEnd(); //结束绘制 四边形
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 设置默认字体
//--------------------------------------------------------------------
void CT_DefaultFontSet()
{
    CT_DF_Number = 95; //字体字符数量
    CT_DF_Gap = 1; //字符间隙
    CT_DF_Height = 14; //字符高度
    CT_DF_CharList = CT_DefaultFont_CharList; //字体字符列表
    CT_DF_GlyphList = CT_DefaultFont_GlythList; //字体字形列表
    glGenTextures(1,&CT_DF_TID); //生成纹理
    glBindTexture(GL_TEXTURE_2D,CT_DF_TID); //绑定纹理
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //设置纹理过滤
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); //设置纹理过滤
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP); //设置纹理环绕
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP); //设置纹理环绕
    glTexImage2D(GL_TEXTURE_2D,0,4,254,62,0,GL_RGBA,GL_UNSIGNED_BYTE,CT_DefaultFont_Image); //加载纹理
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 测量图像字符串宽度
//--------------------------------------------------------------------
int CT_ImgStringMeasure(int font_number,int font_gap,char *font_charlist,int *font_glyphlist,char *str)
{
    char _char[5] = ""; //当前字符
    char list_char[5] = ""; //列表当前字符
    int str_l = strlen(str); //字符串长度
    int offset = 0; //字符串偏移量
    int str_i = 0; //字符串索引
    int gw = 0; //字形宽度
    int imgstr_w = 0;
    if(str_l == 0) //如果字符串长度为0
    {
        return 0;
    }
    while(str_i < str_l) //遍历字符串
    {
        int i2;
        if(str[str_i] < 0) //如果当前值大于0x7f
        {
            _char[0] = str[str_i]; //当前字符字节1
            _char[1] = str[str_i+1]; //当前字符字节2
            offset = 2; //字符串偏移量
        }
        else
        {
            _char[0] = str[str_i]; //当前字符字节1
            offset = 1; //字符串偏移量
        }
        for(i2=0;i2<font_number;i2++) //遍历字符列表
        {
            list_char[0] = font_charlist[i2*4+0]; //复制当前字符的字节1
            list_char[1] = font_charlist[i2*4+1]; //复制当前字符的字节2
            list_char[2] = font_charlist[i2*4+2]; //复制当前字符的字节3
            list_char[3] = font_charlist[i2*4+3]; //复制当前字符的字节4
            if(strcmp(list_char,_char) == 0) //如果字符串当前字符等于列表中的当前字符
            {
                gw = font_glyphlist[i2*4+2]; //字形位置w
                imgstr_w += gw + font_gap; //字符串宽度增加一个字符的宽度
                break; //中断
            }
        }
        str_i += offset; //字符串偏移到下一个字符
        _char[0] = 0x00;
        _char[1] = 0x00;
    }
    if(imgstr_w > 1)
    {
        imgstr_w -= font_gap; //图像字符串-字体间隙
    }
    return imgstr_w;
}
//--------------------------------------------------------------------