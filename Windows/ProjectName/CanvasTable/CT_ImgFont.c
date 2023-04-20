#include "CanvasTableSYS.h"
#include <windows.h>
#include <gl/gl.h>
#pragma comment(lib,"opengl32.lib")

extern unsigned int *CT_ResourceArgList;
extern char CT_DefaultFont_Image[65536];
extern char CT_DefaultFont_CharList[109][4];
extern int CT_DefaultFont_GlyphList[109][4];
extern int CT_DF_Img;
extern int CT_DF_Number;
extern int CT_DF_Gap;
extern int CT_DF_Height;
extern char *CT_DF_CharList;
extern int *CT_DF_GlyphList;
extern int CT_DF_TID;
extern int CT_DF_TW;
extern int CT_DF_TH;
//--------------------------------------------------------------------
// 绘制图像字符串
//--------------------------------------------------------------------
void CT_OffCanvas_ImgString(int x,int y,int c,char *str,int font_img,int font_number,int font_gap,char *font_charlist,int *font_glyphlist)
{
    char _char[5] = ""; //当前字符
    char treel_char[5] = ""; //树的左节点的当前字符
    char treer_char[5] = ""; //树的右节点的当前字符
    size_t str_l = strlen(str); //字符串长度
    int offset = 0; //字符串偏移量
    size_t str_i = 0; //字符串索引
    int cx = x; //字符绘制位置x
    unsigned int tex_id = 0; //纹理id
    unsigned int img_w = 0; //字体图像宽度
    unsigned int img_h = 0; //字体图像高度
    if(font_img != -1) //如果有字体图像
    {
        size_t ldata_i = font_img * 5; //资源参数数据索引
        img_w = CT_ResourceArgList[ldata_i+2]; //获取字体图像宽度
        img_h = CT_ResourceArgList[ldata_i+3]; //获取字体图像高度
        tex_id = CT_ResourceArgList[ldata_i+4]; //获取字体纹理id
    }
    else
    {
        img_w = CT_DF_TW; //获取默认字体图像宽度
        img_h = CT_DF_TH; //获取默认字体图像高度
        tex_id = CT_DF_TID; //获取字体纹理id
    }
    while(str_i < str_l) //遍历字符串
    {
        int find_char = 0; //找到字符
        int tree_l = font_number; //树长度
        int tree_li = 0; //树的左节点索引
        int tree_le = tree_l / 2 - 1; //树的左节点长度
        int tree_ri = tree_le + 1; //树的右节点索引
        int tree_re = tree_l - 1; //树的右节点索引
        int tree_end = 0; //树结束
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
        while(tree_end < 2) //搜索树
        {
            treel_char[0] = font_charlist[tree_li*4];
            treel_char[1] = font_charlist[tree_li*4+1];
            if(strcmp(treel_char,_char) == 0) //如果在树的左节点中找到当前字符
            {
                unsigned int gx = font_glyphlist[tree_li*4]; //字形位置x
                unsigned int gy = font_glyphlist[tree_li*4+1]; //字形位置y
                unsigned int gw = font_glyphlist[tree_li*4+2]; //字形位置w
                unsigned int gh = font_glyphlist[tree_li*4+3]; //字形位置h
                CT_OffCanvas_Glyph(cx,y,font_img,gx,gy,gw,gh,c,tex_id,img_w,img_h); //绘制字体字形
                cx += gw + font_gap; //绘制下一个字符的位置
                find_char = 1; //已找到字符
                break; //中断
            }
            treer_char[0] = font_charlist[tree_ri*4];
            treer_char[1] = font_charlist[tree_ri*4+1];
            if(strcmp(treer_char,_char) == 0) //如果在树的右节点中找到当前字符
            {
                unsigned int gx = font_glyphlist[tree_ri*4]; //字形位置x
                unsigned int gy = font_glyphlist[tree_ri*4+1]; //字形位置y
                unsigned int gw = font_glyphlist[tree_ri*4+2]; //字形位置w
                unsigned int gh = font_glyphlist[tree_ri*4+3]; //字形位置h
                CT_OffCanvas_Glyph(cx,y,font_img,gx,gy,gw,gh,c,tex_id,img_w,img_h); //绘制字体字形
                cx += gw + font_gap; //绘制下一个字符的位置
                find_char = 1; //已找到字符
                break; //中断
            }
            if(tree_li < tree_le) //如果树的左节点索引小于树的左节点长度
            {
                tree_li ++; //树的左节点索引+1
            }
            else
            {
                tree_end ++;
            }
            if(tree_ri < tree_re) //如果树的右节点索引小于树的右节点长度
            {
                tree_ri ++; //树的右节点索引+1
            }
            else
            {
                tree_end ++;
            }
        }
        str_i += offset; //字符串偏移到下一个字符
        _char[0] = 0x00;
        _char[1] = 0x00;
        if(find_char == 0) //如果没有找到字符
        {
            cprint("ERROR : Not find char in charlist.");
        }
        
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 绘制字体字形(离屏)
//--------------------------------------------------------------------
void CT_OffCanvas_Glyph(int x,int y,int i,unsigned int ix,unsigned int iy,unsigned int iw,unsigned int ih,int c,unsigned int tex_id,unsigned int img_w,unsigned int img_h)
{
    int r = c >> 24 & 0x000000ff; //获取颜色值 red
    int g = c >> 16 & 0x000000ff; //获取颜色值 green
    int b = c >> 8 & 0x000000ff; //获取颜色值 blue
    int a = c & 0x000000ff; //获取颜色值 alpha
    float part1_x = (float)ix / (float)img_w; //图像部分左下角位置x
    float part1_y = ((float)img_h - (float)iy - (float)ih) / (float)img_h; //图像部分左下角位置y
    float part2_x = ((float)ix + (float)iw) / (float)img_w; //图像部分右下角位置x
    float part2_y = ((float)img_h - (float)iy - (float)ih) / (float)img_h; //图像部分右下角位置y
    float part3_x = ((float)ix + (float)iw) / (float)img_w; //图像部分右上角位置x
    float part3_y = ((float)img_h - (float)iy) / (float)img_h; //图像部分右上角位置y
    float part4_x = (float)ix / (float)img_w; //图像部分左上角位置x
    float part4_y = ((float)img_h - (float)iy) / (float)img_h; //图像部分左上角位置y
    glColor4ub(r,g,b,a); //设置颜色
    glBindTexture(GL_TEXTURE_2D,tex_id); //绑定纹理
    glBegin(GL_POLYGON); //开始绘制 多边形
    glTexCoord2f(part1_x,part1_y); //设置纹理的左下角坐标
    glVertex2f(x,y+ih); //设置顶点
    glTexCoord2f(part2_x,part2_y); //设置纹理的右下角坐标
    glVertex2f(x+iw,y+ih); //设置顶点
    glTexCoord2f(part3_x,part3_y); //设置纹理的右上角坐标
    glVertex2f(x+iw,y); //设置顶点
    glTexCoord2f(part4_x,part4_y); //设置纹理的左上角坐标
    glVertex2f(x,y); //设置顶点
    glEnd(); //结束绘制 多边形
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
    CT_DF_GlyphList = CT_DefaultFont_GlyphList; //字体字形列表
    CT_DF_TW = 256; //默认字体图像宽度
    CT_DF_TH = 64; //默认字体图像高度
    glGenTextures(1,&CT_DF_TID); //生成纹理
    glBindTexture(GL_TEXTURE_2D,CT_DF_TID); //绑定纹理
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //设置纹理过滤
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); //设置纹理过滤
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP); //设置纹理环绕
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP); //设置纹理环绕
    glTexImage2D(GL_TEXTURE_2D,0,4,CT_DF_TW,CT_DF_TH,0,GL_RGBA,GL_UNSIGNED_BYTE,CT_DefaultFont_Image); //加载默认纹理
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 测量图像字符串宽度
//--------------------------------------------------------------------
int CT_ImgStringMeasure(int font_number,int font_gap,char *font_charlist,int *font_glyphlist,char *str)
{
    char _char[5] = ""; //当前字符
    char treel_char[5] = ""; //树的左节点的当前字符
    char treer_char[5] = ""; //树的右节点的当前字符
    size_t str_l = strlen(str); //字符串长度
    int offset = 0; //字符串偏移量
    size_t str_i = 0; //字符串索引
    int gw = 0; //字形宽度
    int imgstr_w = 0; //图像字符串宽度
    if(str_l == 0) //如果字符串长度为0
    {
        return 0;
    }
    while(str_i < str_l) //遍历字符串
    {
        int find_char = 0; //找到字符
        int tree_l = font_number; //树长度
        int tree_li = 0; //树的左节点索引
        int tree_le = tree_l / 2 - 1; //树的左节点长度
        int tree_ri = tree_le + 1; //树的右节点索引
        int tree_re = tree_l - 1; //树的右节点索引
        int tree_end = 0; //树结束
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
        while(tree_end < 2) //搜索平衡二叉树
        {
            treel_char[0] = font_charlist[tree_li*4];
            treel_char[1] = font_charlist[tree_li*4+1];
            if(strcmp(treel_char,_char) == 0) //如果在树的左节点中找到当前字符
            {
                gw = font_glyphlist[tree_li*4+2]; //字形位置w
                imgstr_w += gw + font_gap; //字符串宽度增加一个字符的宽度
                break; //中断
            }
            treer_char[0] = font_charlist[tree_ri*4];
            treer_char[1] = font_charlist[tree_ri*4+1];
            if(strcmp(treer_char,_char) == 0) //如果在树的右节点中找到当前字符
            {
                gw = font_glyphlist[tree_ri*4+2]; //字形位置w
                imgstr_w += gw + font_gap; //字符串宽度增加一个字符的宽度
                break; //中断
            }
            if(tree_li < tree_le) //如果树的左节点索引小于树的左节点长度
            {
                tree_li ++; //树的左节点索引+1
            }
            else
            {
                tree_end ++;
            }
            if(tree_ri < tree_re) //如果树的右节点索引小于树的右节点长度
            {
                tree_ri ++; //树的右节点索引+1
            }
            else
            {
                tree_end ++;
            }
        }
        str_i += offset; //字符串偏移到下一个字符
        _char[0] = 0x00;
        _char[1] = 0x00;
    }
    if(imgstr_w > 1) //如果图像字符串宽度
    {
        imgstr_w -= font_gap; //图像字符串-字体间隙
    }
    return imgstr_w;
}
//--------------------------------------------------------------------