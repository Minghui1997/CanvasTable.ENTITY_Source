#include "CanvasTableSYS.h"

extern int CT_DF_Number;
extern int CT_DF_Gap;
extern char *CT_DF_CharList;
extern int *CT_DF_GlyphList;
extern int CT_DF_TID;

//--------------------------------------------------------------------
// 新建图像字体对象
//--------------------------------------------------------------------
void CTimgfont_new(CTimgfont *imgfont)
{
    imgfont->FontImg = 0; //字体图像
    imgfont->CharNumber = 0; //字体字符数量
    imgfont->CharGap = 1; //字体字符间隙
    imgfont->CharHeight = 0; //字体字符高度
    imgfont->CharList = NULL; //字体字符列表
    imgfont->GlyphList = NULL; //字体字形列表
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 获取图像字符串宽度
//--------------------------------------------------------------------
int istring_pxwidth(CTimgfont *font,char *str)
{
    int font_charnum = CT_DF_Number; //字体字符数量
    int font_chargap = CT_DF_Gap; //字体字符间隙
    char *font_charlist = CT_DF_CharList; //字体字符列表
    int *font_glyphlist = CT_DF_GlyphList; //字体字形列表
    int is_w = 0; //图像字符串宽度
    if(font != NULL) //如果不是默认字体
    {
        font_charnum = font->CharNumber; //字体字符数量
        font_chargap = font->CharGap; //字体字符间隙
        font_charlist = font->CharList; //字体字符列表
        font_glyphlist = font->GlyphList; //字体字形列表
    }
    is_w = CT_ImgStringMeasure(font_charnum,font_chargap,font_charlist,font_glyphlist,str); //测量图像字符串宽度
    return is_w;
}
//--------------------------------------------------------------------