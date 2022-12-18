#define _CRT_SECURE_NO_WARNINGS

#include "CanvasTableSYS.h"
#include <stdio.h>
#include <string.h>

extern int **CT_ResourceArgList;
extern int CT_RunnerStatus; 
extern int CT_ContainerWindow[100][15];
extern int CT_ContainerWindowIndex;
extern int CT_ContainerUI[1000][25];
extern char CT_ContainerUI_Value[1000][2048];
extern char CT_ContainerUI_Name[1000][64];
extern char *CT_ContainerUI_FontCharList_P[1000];
extern int *CT_ContainerUI_FontGlythList_P[1000];
extern int CT_ContainerUIIndex;
extern int CT_DF_Img;
extern int CT_DF_Number;
extern int CT_DF_Gap;
extern int CT_DF_Height;
extern char *CT_DF_CharList;
extern int *CT_DF_GlyphList;
extern char CT_NullStr[2];

//--------------------------------------------------------------------
// 新建窗口对象
//--------------------------------------------------------------------
void CTwindow_new(CTwindow *win)
{
    win->x = 0; //窗口位置x
    win->y = 0; //窗口位置y
    win->width = 300; //窗口宽度
    win->height = 200; //窗口高度
    win->color_background = color(228,226,211,255); //窗口背景颜色
    win->color_borderUp = color(170,168,153,255); //窗口上边框颜色
    win->color_borderDown = color(170,168,153,255); //窗口下边框颜色
    win->color_borderLeft = color(170,168,153,255); //窗口左边框颜色
    win->color_borderRight = color(170,168,153,255); //窗口右边框颜色
    win->image = -1; //窗口背景图像
    win->image_o = 0; //窗口背景图像偏移
    win->layer = 0; //窗口层
    win->type = 0; //ui对象类型
    win->index = 0; //窗口索引
    win->id = CT_UIGetId(); //ID
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 新建按钮对象
//--------------------------------------------------------------------
void CTbutton_new(CTui *ctui)
{
    ctui->index = 0; //ui对象索引
    ctui->type = 1; //ui对象类型
    ctui->id = CT_UIGetId(); //ID
//--------------------------------------------------------------------
    ctui->x = 0; //按钮位置x
    ctui->y = 0; //按钮位置y
    ctui->width = 60; //按钮宽度
    ctui->height = 30; //按钮高度
    ctui->color_background = color(203,201,183,255); //按钮背景颜色
    ctui->color_borderUp = color(181,179,162,255); //按钮上边框颜色
    ctui->color_borderDown = color(181,179,162,255); //按钮下边框颜色
    ctui->color_borderLeft = color(181,179,162,255); //按钮左边框颜色
    ctui->color_borderRight = color(181,179,162,255); //按钮右边框颜色
    ctui->background = -1; //按钮背景图像
    ctui->background_o = 0; //按钮背景图像偏移
    ctui->color_text = color(0,0,0,255); //按钮文本颜色
    ctui->text = "Button"; //按钮显示文本
    ctui->imgfont = NULL; //按钮字体
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 新建文本对象
//--------------------------------------------------------------------
void CTtext_new(CTui *ctui)
{
    ctui->index = 0; //ui对象索引
    ctui->type = 2; //ui对象类型
    ctui->id = CT_UIGetId(); //ID
//--------------------------------------------------------------------
    ctui->x = 0; //文本位置x
    ctui->y = 0; //文本位置y
    ctui->color_text = color(0,0,0,255); //文本颜色
    ctui->text = "Text"; //显示文本
    ctui->imgfont = NULL; //文本字体
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 新建复选框对象
//--------------------------------------------------------------------
void CTcheckbox_new(CTui *ctui)
{
    ctui->index = 0; //ui对象索引
    ctui->type = 3; //ui对象类型
    ctui->id = CT_UIGetId(); //ID
//--------------------------------------------------------------------
    ctui->x = 0; //复选框位置x
    ctui->y = 0; //复选框位置y
    ctui->color_border = color(181,179,162,255); //复选框边框颜色
    ctui->color_background = color(255,255,255,255); //复选框背景颜色
    ctui->color_widget = color(34,172,56,255); //复选框部件颜色
    ctui->status = 0; //复选框状态
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 新建单选框对象
//--------------------------------------------------------------------
void CTradio_new(CTui *ctui)
{
    ctui->index = 0; //ui对象索引
    ctui->type = 4; //ui对象类型
    ctui->id = CT_UIGetId(); //ID
//--------------------------------------------------------------------
    ctui->x = 0; //单选框位置x
    ctui->y = 0; //单选框位置y
    ctui->color_background = color(255,255,255,255); //单选框背景颜色
    ctui->color_border = color(181,179,162,255); //单选框边框颜色
    ctui->color_widget = color(34,172,56,255); //单选框部件颜色
    ctui->group = 0; //单选框组
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 新建进度条对象
//--------------------------------------------------------------------
void CTprogressbar_new(CTui *ctui)
{
    ctui->index = 0; //ui对象索引
    ctui->type = 5; //ui对象类型
    ctui->id = CT_UIGetId(); //ID
//--------------------------------------------------------------------
    ctui->x = 0; //进度条位置x
    ctui->y = 0; //进度条位置y
    ctui->width = 80; //进度条宽度
    ctui->height = 30; //进度条高度
    ctui->color_background = color(255,255,255,255); //进度条背景颜色
    ctui->color_borderUp = color(181,179,162,255); //进度条上边框颜色
    ctui->color_borderDown = color(181,179,162,255); //进度条下边框颜色
    ctui->color_borderLeft = color(181,179,162,255); //进度条左边框颜色
    ctui->color_borderRight = color(181,179,162,255); //进度条右边框颜色
    ctui->color_widget = color(34,172,56,255); //进度条部件颜色
    ctui->background = -1; //进度条背景图像
    ctui->background_o = 0; //进度条背景图像偏移
    ctui->value = 0; //进度条默认值
    ctui->maxvalue = 10; //进度条最大值
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 新建文本框对象
//--------------------------------------------------------------------
void CTtextbox_new(CTui *ctui)
{
    ctui->index = 0; //ui对象索引
    ctui->type = 6; //ui对象类型
    ctui->id = CT_UIGetId(); //ID
//--------------------------------------------------------------------
    ctui->x = 0; //文本框位置x
    ctui->y = 0; //文本框位置y
    ctui->width = 250; //文本框宽度
    ctui->height = 30; //文本框高度
    ctui->color_background = color(255,255,255,255); //文本框背景颜色
    ctui->color_borderUp = color(181,179,162,255); //文本框上边框颜色
    ctui->color_borderDown = color(181,179,162,255); //文本框下边框颜色
    ctui->color_borderLeft = color(181,179,162,255); //文本框左边框颜色
    ctui->color_borderRight = color(181,179,162,255); //文本框右边框颜色
    ctui->color_text = color(0,0,0,255); //文本框字体颜色
    ctui->background = -1; //文本框背景图像
    ctui->background_o = 0; //文本框背景图像偏移
    ctui->password = 0; //密码文本框
    ctui->number = 0; //数字文本框
    ctui->max = 30; //文本框最大字符数
    ctui->readonly = 0; //只读
    ctui->subbox = 0; //子文本框
    ctui->name = "Title"; //文本框名称
    ctui->imgfont = NULL; //文本框字体
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 创建窗口
//--------------------------------------------------------------------
void CTwindow_create(CTwindow *win)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(win->type == 0) //如果对象类型是窗口
        {
            int id = win->id; //窗口对象ID
            int x = win->x; //窗口对象位置x
            int y = win->y; //窗口对象位置y
            int w = win->width; //窗口对象宽度
            int h = win->height; //窗口对象高度
            int l = win->layer; //窗口对象层
            int cbg = win->color_background; //窗口对象背景颜色
            int cbu = win->color_borderUp; //窗口对象上边框颜色
            int cbd = win->color_borderDown; //窗口对象下边框颜色
            int cbl = win->color_borderLeft; //窗口对象左边框颜色
            int cbr = win->color_borderRight; //窗口对象右边框颜色
            int bi = win->image; //窗口对象背景图像
            int bio = win->image_o; //窗口对象背景图像偏移
            int i;
            for(i=0;i<CT_ContainerWindowIndex;i++) //遍历窗口容器
            {
                if(CT_ContainerWindow[i][0] != NULL) //如果当前窗口不为空
                {
                    int win_id = CT_ContainerWindow[i][1]; //当前窗口ID
                    if(win_id == id) //如果当前窗口ID等于窗口ID
                    {
                        cprint("ERROR : Window have been created.");
                        return; //中断
                    }
                }
            }
            if(bi != -1) //如果有背景图像
            {
                int res_type = CT_ResourceArgList[bi][1]; //获取资源类型
                if(res_type != 0) //如果资源类型不是图像
                {
                    bi = -1; //窗口无背景图像
                    cprint("ERROR : The resource index corresponding to the resource file is not a image.");
                }
            }
            if(w > 0 && h > 0) //如果窗口对象大小正确
            {
                int i2;
                for(i2=0;i2<CT_ContainerWindowIndex;i2++) //遍历窗口容器
                {
                    if(CT_ContainerWindow[i2][0] == NULL) //如果当前窗口为空
                    {
                        //将窗口数据写入窗口容器
                        CT_ContainerWindow[i2][0] = 1;
                        CT_ContainerWindow[i2][1] = id;
                        CT_ContainerWindow[i2][2] = l;
                        CT_ContainerWindow[i2][3] = x;
                        CT_ContainerWindow[i2][4] = y;
                        CT_ContainerWindow[i2][5] = w;
                        CT_ContainerWindow[i2][6] = h;
                        CT_ContainerWindow[i2][7] = cbg;
                        CT_ContainerWindow[i2][8] = cbu;
                        CT_ContainerWindow[i2][9] = cbd;
                        CT_ContainerWindow[i2][10] = cbl;
                        CT_ContainerWindow[i2][11] = cbr;
                        CT_ContainerWindow[i2][12] = bi;
                        CT_ContainerWindow[i2][13] = bio;
                        win->index = i2;
                        CT_InterfaceRepaint(); //请求重绘
                        return; //中断
                    }                  
                }
                //将窗口数据写入窗口容器
                CT_ContainerWindow[CT_ContainerWindowIndex][0] = 1;
                CT_ContainerWindow[CT_ContainerWindowIndex][1] = id;
                CT_ContainerWindow[CT_ContainerWindowIndex][2] = l;
                CT_ContainerWindow[CT_ContainerWindowIndex][3] = x;
                CT_ContainerWindow[CT_ContainerWindowIndex][4] = y;
                CT_ContainerWindow[CT_ContainerWindowIndex][5] = w;
                CT_ContainerWindow[CT_ContainerWindowIndex][6] = h;
                CT_ContainerWindow[CT_ContainerWindowIndex][7] = cbg;
                CT_ContainerWindow[CT_ContainerWindowIndex][8] = cbu;
                CT_ContainerWindow[CT_ContainerWindowIndex][9] = cbd;
                CT_ContainerWindow[CT_ContainerWindowIndex][10] = cbl;
                CT_ContainerWindow[CT_ContainerWindowIndex][11] = cbr;
                CT_ContainerWindow[CT_ContainerWindowIndex][12] = bi;
                CT_ContainerWindow[CT_ContainerWindowIndex][13] = bio;
                win->index = CT_ContainerWindowIndex; //窗口对象索引
                CT_ContainerWindowIndex ++; //窗口容器索引 +1
                CT_InterfaceRepaint(); //请求重绘
            }
            else
            {
                cprint("ERROR : Unable to create window.");
            }
        }
        else
        {
            cprint("ERROR : Not is window object.");
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 销毁窗口
//--------------------------------------------------------------------
void CTwindow_destroy(CTwindow *win)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(win->type == 0) //如果对象类型是窗口
        {
            int win_index = win->index; //窗口对象索引
            if(CT_ContainerWindow[win_index][0] != NULL) //如果窗口容器中指定窗口对象数据不为空
            {
                int Cwin_id = CT_ContainerWindow[win_index][1]; //窗口容器中指定窗口对象的ID
                if(win->id == Cwin_id) //如果窗口对象id等于容器中指定窗口对象的ID
                {
                    int i;
                    int i2;
                    for(i=0;i<15;i++) //遍历窗口对象数据
                    {
                        CT_ContainerWindow[win_index][i] = 0; //清空窗口对象容器中窗口对象的数据
                    }
                    for(i2=0;i2<CT_ContainerUIIndex;i2++) //遍历UI对象容器
                    {
                        if(CT_ContainerUI[i2][0] != NULL) //如果当前UI对象不为空
                        {
                            int ui_win_index = CT_ContainerUI[i2][2]; //当前UI对象对应窗口的索引
                            if(ui_win_index == win_index) //如果当前UI对象对应窗口的索引等于窗口对象索引
                            {
                                int ui_type = CT_ContainerUI[i2][1]; //获取当前ui类型
                                int i3;
                                for(i3=0;i3<25;i3++) //遍历UI对象数据
                                {
                                    CT_ContainerUI[i2][i3] = NULL; //清空UI对象容器中UI对象的数据
                                }
                                if(ui_type == 1 || ui_type == 2 || ui_type == 6) //如果ui类型是按钮 文本 文本框
                                {
                                    strclear(CT_ContainerUI_Value[i2]); //清空UI对象容器中UI对象的字符串
                                    CT_ContainerUI_FontCharList_P[i2] = NULL;
                                    CT_ContainerUI_FontGlythList_P[i2] = NULL;
                                }
                            }
                        }
                    }
                    CT_InterfaceRepaint(); //请求重绘
                }
            }
        }
        else
        {
            cprint("ERROR : Not is window object.");
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 设置窗口位置x
//--------------------------------------------------------------------
void CTwindow_set_x(CTwindow *win,int x)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(win->type == 0) //如果UI对象是窗口
        {
            if(CT_ContainerWindow[win->index][0] != NULL) //如果UI容器中指定窗口对象数据不为空
            {
                int Cwin_id = CT_ContainerWindow[win->index][1]; //窗口容器中指定窗口对象的ID
                if(win->id == Cwin_id) //如果窗口对象id等于容器中指定窗口对象的ID
                {
                    CT_ContainerWindow[win->index][3] = x; //设置窗口位置x
                    CT_InterfaceRepaint(); //请求重绘
                }
            }
        }
        else
        {
            cprint("ERROR : Not is window object.");
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 设置窗口位置y
//--------------------------------------------------------------------
void CTwindow_set_y(CTwindow *win,int y)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(win->type == 0) //如果UI对象是窗口
        {
            if(CT_ContainerWindow[win->index][0] != NULL) //如果UI容器中指定窗口对象数据不为空
            {
                int Cwin_id = CT_ContainerWindow[win->index][1]; //窗口容器中指定窗口对象的ID
                if(win->id == Cwin_id) //如果窗口对象id等于容器中指定窗口对象的ID
                {
                    CT_ContainerWindow[win->index][4] = y; //设置窗口位置x
                    CT_InterfaceRepaint(); //请求重绘
                }
            }
        }
        else
        {
            cprint("ERROR : Not is window object.");
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 设置窗口宽度
//--------------------------------------------------------------------
void CTwindow_set_width(CTwindow *win,int w)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(win->type == 0) //如果UI对象是窗口
        {
            if(CT_ContainerWindow[win->index][0] != NULL) //如果UI容器中指定窗口对象数据不为空
            {
                int Cwin_id = CT_ContainerWindow[win->index][1]; //窗口容器中指定窗口对象的ID
                if(win->id == Cwin_id) //如果窗口对象id等于容器中指定窗口对象的ID
                {
                    CT_ContainerWindow[win->index][5] = w; //设置窗口位置x
                    CT_InterfaceRepaint(); //请求重绘
                }
            }
        }
        else
        {
            cprint("ERROR : Not is window object.");
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 设置窗口高度
//--------------------------------------------------------------------
void CTwindow_set_height(CTwindow *win,int h)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(win->type == 0) //如果UI对象是窗口
        {
            if(CT_ContainerWindow[win->index][0] != NULL) //如果UI容器中指定窗口对象数据不为空
            {
                int Cwin_id = CT_ContainerWindow[win->index][1]; //窗口容器中指定窗口对象的ID
                if(win->id == Cwin_id) //如果窗口对象id等于容器中指定窗口对象的ID
                {
                    CT_ContainerWindow[win->index][6] = h; //设置窗口位置x
                    CT_InterfaceRepaint(); //请求重绘
                }
            }
        }
        else
        {
            cprint("ERROR : Not is window object.");
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 设置窗口背景图像
//--------------------------------------------------------------------
void CTwindow_set_image(CTwindow *win,int i)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(win->type == 0) //如果UI对象是窗口
        {
            if(CT_ContainerWindow[win->index][0] != NULL) //如果UI容器中指定窗口对象数据不为空
            {
                int Cwin_id = CT_ContainerWindow[win->index][1]; //窗口容器中指定窗口对象的ID
                if(win->id == Cwin_id) //如果窗口对象id等于容器中指定窗口对象的ID
                {
                    int res_type = CT_ResourceArgList[i][1]; //获取资源类型
                    if(res_type == 0) //如果资源类型是图像
                    {
                        CT_ContainerWindow[win->index][12] = i; //设置窗口位置x
                        CT_InterfaceRepaint(); //请求重绘
                    }
                    else
                    {
                        cprint("ERROR : The resource index corresponding to the resource file is not a image.");
                    }
                }
            }
        }
        else
        {
            cprint("ERROR : Not is window object.");
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 设置窗口颜色
//--------------------------------------------------------------------
void CTwindow_set_color(CTwindow *win,char *prop,int col)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(win->type == 0) //如果UI对象是窗口
        {
            if(CT_ContainerWindow[win->index][0] != NULL) //如果UI容器中指定窗口对象数据不为空
            {
                int Cwin_id = CT_ContainerWindow[win->index][1]; //窗口容器中指定窗口对象的ID
                if(win->id == Cwin_id) //如果窗口对象id等于容器中指定窗口对象的ID
                {
                    if(strcmp(prop,"back") == 0) //如果属性是背景
                    {
                        CT_ContainerWindow[win->index][7] = col; //设置窗口背景颜色
                        CT_InterfaceRepaint(); //请求重绘
                    }
                    if(strcmp(prop,"up") == 0) //如果属性是上边框
                    {
                        CT_ContainerWindow[win->index][8] = col; //设置窗口上边框颜色
                        CT_InterfaceRepaint(); //请求重绘
                    }
                    if(strcmp(prop,"down") == 0) //如果属性是下边框
                    {
                        CT_ContainerWindow[win->index][9] = col; //设置窗口下边框颜色
                        CT_InterfaceRepaint(); //请求重绘
                    }
                    if(strcmp(prop,"left") == 0) //如果属性是左边框
                    {
                        CT_ContainerWindow[win->index][10] = col; //设置窗口左边框颜色
                        CT_InterfaceRepaint(); //请求重绘
                    }
                    if(strcmp(prop,"right") == 0) //如果属性是右边框
                    {
                        CT_ContainerWindow[win->index][11] = col; //设置窗口右边框颜色
                        CT_InterfaceRepaint(); //请求重绘
                    }
                }
            }
        }
        else
        {
            cprint("ERROR : Not is window object.");
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 创建UI对象
//--------------------------------------------------------------------
void CTui_create(CTwindow *win,CTui *ctui)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(win->type == 0) //如果对象类型是窗口
        {
            int id = ctui->id; //UI对象ID
            int win_index = win->index; //窗口对象索引
            int type = ctui->type; //UI对象类型
            int i;
            for(i=0;i<CT_ContainerUIIndex;i++) //遍历UI容器
            {
                if(CT_ContainerUI[i][0] != NULL) //如果当前UI对象不为空
                {
                    int ui_id = CT_ContainerUI[i][1]; //当前UI对象的ID
                    if(ui_id == id) //如果当前UI对象的ID等于UI对象ID
                    {
                        cprint("ERROR : The UI object has been created.");
                        return; //中断
                    }
                }
            }
            if(type == 1) //如果UI是按钮
            {
                int x = ctui->x; //按钮位置x
                int y = ctui->y; //按钮位置y
                int w = ctui->width; //按钮宽度
                int h = ctui->height; //按钮高度
                int cbg = ctui->color_background; //按钮背景颜色
                int cbu = ctui->color_borderUp; //按钮上边框颜色
                int cbd = ctui->color_borderDown; //按钮下边框颜色
                int cbl = ctui->color_borderLeft; //按钮左边框颜色
                int cbr = ctui->color_borderRight; //按钮右边框颜色
                int bi = ctui->background; //按钮背景图像
                int bio = ctui->background_o; //按钮背景图像偏移
                int ct = ctui->color_text; //按钮文本颜色
                char *text = ctui->text; //按钮显示文本
                int font_i = CT_DF_Img; //字体图像
                int font_n = CT_DF_Number; //字体字符数
                int font_g = CT_DF_Gap; //字体字符间隙
                char *font_cl = CT_DF_CharList; //字体字符列表
                int *font_gl = CT_DF_GlyphList; //字体字形列表
                int text_w = 0; //按钮显示文本宽度
                int text_h = CT_DF_Height; //按钮显示文本高度
                if(bi != -1) //如果有背景图像
                {
                    int res_type = CT_ResourceArgList[bi][1]; //获取资源类型
                    if(res_type != 0) //如果资源类型不是图像
                    {
                        bi = -1; //无背景图像
                        cprint("ERROR : The resource index corresponding to the resource file is not a image.");
                    }
                }
                if(ctui->imgfont != NULL) //如果UI图像字体不为空
                {
                    if(ctui->imgfont->FontImg != -1 && ctui->imgfont->CharNumber > 0 && ctui->imgfont->CharList != NULL && ctui->imgfont->GlyphList != NULL) //如果资源不为空
                    {
                        font_i = ctui->imgfont->FontImg; //获取字体图像
                        font_n = ctui->imgfont->CharNumber; //获取字体字符数
                        font_g = ctui->imgfont->CharGap; //获取字体字符间隙
                        font_cl = ctui->imgfont->CharList; //获取字体字符列表
                        font_gl = ctui->imgfont->GlyphList; //获取字体字形列表
                        text_h = ctui->imgfont->CharHeight; //按钮显示文本高度
                    }
                    else
                    {
                        cprint("ERROR : Image of the imgfont is invalid.");
                    }
                }
                text_w = CT_ImgStringMeasure(font_n,font_g,font_cl,font_gl,text); //获取按钮显示文本宽度
                if(w > 0 && h > 0) //如果按钮大小正确
                {
                    int i2;
                    for(i2=0;i2<CT_ContainerUIIndex;i2++) //遍历UI容器
                    {
                        if(CT_ContainerUI[i2][0] == NULL) //如果当前UI为空
                        {
                            //将UI数据写入UI容器
                            CT_ContainerUI[i2][0] = 1;
                            CT_ContainerUI[i2][1] = id;
                            CT_ContainerUI[i2][2] = win_index;
                            CT_ContainerUI[i2][3] = type;
                            CT_ContainerUI[i2][4] = x;
                            CT_ContainerUI[i2][5] = y;
                            CT_ContainerUI[i2][6] = w;
                            CT_ContainerUI[i2][7] = h;
                            CT_ContainerUI[i2][8] = cbg;
                            CT_ContainerUI[i2][9] = cbu;
                            CT_ContainerUI[i2][10] = cbd;
                            CT_ContainerUI[i2][11] = cbl;
                            CT_ContainerUI[i2][12] = cbr;
                            CT_ContainerUI[i2][13] = ct;
                            CT_ContainerUI[i2][14] = bi;
                            strcpy(CT_ContainerUI_Value[i2],text); 
                            CT_ContainerUI[i2][15] = text_w;
                            CT_ContainerUI[i2][16] = text_h;
                            CT_ContainerUI[i2][17] = 0;
                            CT_ContainerUI[i2][18] = 0;
                            CT_ContainerUI[i2][19] = 0;
                            CT_ContainerUI[i2][20] = 0;
                            CT_ContainerUI[i2][21] = font_i;
                            CT_ContainerUI[i2][22] = font_n;
                            CT_ContainerUI[i2][23] = font_g;
                            CT_ContainerUI[i2][24] = bio;
                            CT_ContainerUI_FontCharList_P[i2] = font_cl;
                            CT_ContainerUI_FontGlythList_P[i2] = font_gl;
                            ctui->index = i2; //UI对象索引
                            CT_InterfaceRepaint(); //请求重绘
                            return; //中断
                        }
                    }
                    //将UI数据写入UI容器
                    CT_ContainerUI[CT_ContainerUIIndex][0] = 1;
                    CT_ContainerUI[CT_ContainerUIIndex][1] = id;
                    CT_ContainerUI[CT_ContainerUIIndex][2] = win_index;
                    CT_ContainerUI[CT_ContainerUIIndex][3] = type;
                    CT_ContainerUI[CT_ContainerUIIndex][4] = x;
                    CT_ContainerUI[CT_ContainerUIIndex][5] = y;
                    CT_ContainerUI[CT_ContainerUIIndex][6] = w;
                    CT_ContainerUI[CT_ContainerUIIndex][7] = h;
                    CT_ContainerUI[CT_ContainerUIIndex][8] = cbg;
                    CT_ContainerUI[CT_ContainerUIIndex][9] = cbu;
                    CT_ContainerUI[CT_ContainerUIIndex][10] = cbd;
                    CT_ContainerUI[CT_ContainerUIIndex][11] = cbl;
                    CT_ContainerUI[CT_ContainerUIIndex][12] = cbr;
                    CT_ContainerUI[CT_ContainerUIIndex][13] = ct;
                    CT_ContainerUI[CT_ContainerUIIndex][14] = bi;
                    strcpy(CT_ContainerUI_Value[CT_ContainerUIIndex],text);
                    CT_ContainerUI[CT_ContainerUIIndex][15] = text_w;
                    CT_ContainerUI[CT_ContainerUIIndex][16] = text_h;
                    CT_ContainerUI[CT_ContainerUIIndex][17] = 0;
                    CT_ContainerUI[CT_ContainerUIIndex][18] = 0;
                    CT_ContainerUI[CT_ContainerUIIndex][19] = 0;
                    CT_ContainerUI[CT_ContainerUIIndex][20] = 0;
                    CT_ContainerUI[CT_ContainerUIIndex][21] = font_i;
                    CT_ContainerUI[CT_ContainerUIIndex][22] = font_n;
                    CT_ContainerUI[CT_ContainerUIIndex][23] = font_g;
                    CT_ContainerUI[CT_ContainerUIIndex][24] = bio;
                    CT_ContainerUI_FontCharList_P[CT_ContainerUIIndex] = font_cl;
                    CT_ContainerUI_FontGlythList_P[CT_ContainerUIIndex] = font_gl;
                    ctui->index = CT_ContainerUIIndex; //UI对象索引
                    CT_ContainerUIIndex ++; //UI容器索引 +1
                    CT_InterfaceRepaint(); //请求重绘
                }
                else
                {
                    cprint("ERROR : Unable to create button.");
                }
            }
            if(type == 2) //如果UI是文本
            {
                int x = ctui->x; //文本位置x
                int y = ctui->y; //文本位置y
                int ct = ctui->color_text; //文本颜色
                char *text = ctui->text; //文本
                int font_i = CT_DF_Img; //字体图像
                int font_n = CT_DF_Number; //字体字符数
                int font_g = CT_DF_Gap; //字体字符间隙
                char *font_cl = CT_DF_CharList; //字体字符列表
                int *font_gl = CT_DF_GlyphList; //字体字形列表
                int i2;
                if(ctui->imgfont != NULL) //如果UI图像字体不为空
                {
                    if(ctui->imgfont->FontImg != -1 && ctui->imgfont->CharNumber > 0 && ctui->imgfont->CharList != NULL && ctui->imgfont->GlyphList != NULL) //如果资源不为空
                    {
                        font_i = ctui->imgfont->FontImg; //获取字体图像
                        font_n = ctui->imgfont->CharNumber; //获取字体字符数
                        font_g = ctui->imgfont->CharGap; //获取字体字符间隙
                        font_cl = ctui->imgfont->CharList; //获取字体字符列表
                        font_gl = ctui->imgfont->GlyphList; //获取字体字形列表
                    }
                    else
                    {
                        cprint("ERROR : Image of the imgfont is invalid.");
                    }
                }
                for(i2=0;i2<CT_ContainerUIIndex;i2++) //遍历UI容器
                {
                    if(CT_ContainerUI[i2][0] == NULL) //如果当前UI为空
                    {
                        //将UI数据写入UI容器
                        CT_ContainerUI[i2][0] = 1;
                        CT_ContainerUI[i2][1] = id;
                        CT_ContainerUI[i2][2] = win_index;
                        CT_ContainerUI[i2][3] = type;
                        CT_ContainerUI[i2][4] = x;
                        CT_ContainerUI[i2][5] = y;
                        CT_ContainerUI[i2][6] = ct;
                        strcpy(CT_ContainerUI_Value[i2],text);
                        CT_ContainerUI[i2][7] = font_i;
                        CT_ContainerUI[i2][8] = font_n;
                        CT_ContainerUI[i2][9] = font_g;
                        CT_ContainerUI_FontCharList_P[i2] = font_cl;
                        CT_ContainerUI_FontGlythList_P[i2] = font_gl;
                        ctui->index = i2; //UI对象索引
                        CT_InterfaceRepaint(); //请求重绘
                        return; //中断
                    }
                }
                //将UI数据写入UI容器
                CT_ContainerUI[CT_ContainerUIIndex][0] = 1;
                CT_ContainerUI[CT_ContainerUIIndex][1] = id;
                CT_ContainerUI[CT_ContainerUIIndex][2] = win_index;
                CT_ContainerUI[CT_ContainerUIIndex][3] = type;
                CT_ContainerUI[CT_ContainerUIIndex][4] = x;
                CT_ContainerUI[CT_ContainerUIIndex][5] = y;
                CT_ContainerUI[CT_ContainerUIIndex][6] = ct;
                strcpy(CT_ContainerUI_Value[CT_ContainerUIIndex],text);
                CT_ContainerUI[CT_ContainerUIIndex][7] = font_i;
                CT_ContainerUI[CT_ContainerUIIndex][8] = font_n;
                CT_ContainerUI[CT_ContainerUIIndex][9] = font_g;
                CT_ContainerUI_FontCharList_P[CT_ContainerUIIndex] = font_cl;
                CT_ContainerUI_FontGlythList_P[CT_ContainerUIIndex] = font_gl;
                ctui->index = CT_ContainerUIIndex; //UI对象索引
                CT_ContainerUIIndex ++; //UI容器索引 +1
                CT_InterfaceRepaint(); //请求重绘
                return; //中断
            }
            if(type == 3) //如果UI是复选框
            {
                int x = ctui->x; //复选框位置x
                int y = ctui->y; //复选框位置y
                int w = 16; //复选框宽度
                int h = 16; //复选框高度
                int cb = ctui->color_border; //复选框边框颜色
                int cbg = ctui->color_background; //复选框背景颜色
                int cw = ctui->color_widget; //复选框部件颜色
                int status = ctui->status; //复选框状态
                if(status == 0 || status == 1) //如果复选框属性正确
                {
                    int i2;
                    for(i2=0;i2<CT_ContainerUIIndex;i2++) //遍历UI容器
                    {
                        if(CT_ContainerUI[i2][0] == NULL) //如果当前UI为空
                        {
                            //将UI数据写入UI容器
                            CT_ContainerUI[i2][0] = 1;
                            CT_ContainerUI[i2][1] = id;
                            CT_ContainerUI[i2][2] = win_index;
                            CT_ContainerUI[i2][3] = type;
                            CT_ContainerUI[i2][4] = x;
                            CT_ContainerUI[i2][5] = y;
                            CT_ContainerUI[i2][6] = w;
                            CT_ContainerUI[i2][7] = h;
                            CT_ContainerUI[i2][8] = cbg;
                            CT_ContainerUI[i2][9] = cb;
                            CT_ContainerUI[i2][10] = cw;
                            CT_ContainerUI[i2][11] = status;
                            ctui->index = i2;
                            CT_InterfaceRepaint(); //请求重绘
                            return; //中断
                        }
                    }
                    //将UI数据写入UI容器
                    CT_ContainerUI[CT_ContainerUIIndex][0] = 1;
                    CT_ContainerUI[CT_ContainerUIIndex][1] = id;
                    CT_ContainerUI[CT_ContainerUIIndex][2] = win_index;
                    CT_ContainerUI[CT_ContainerUIIndex][3] = type;
                    CT_ContainerUI[CT_ContainerUIIndex][4] = x;
                    CT_ContainerUI[CT_ContainerUIIndex][5] = y;
                    CT_ContainerUI[CT_ContainerUIIndex][6] = w;
                    CT_ContainerUI[CT_ContainerUIIndex][7] = h;
                    CT_ContainerUI[CT_ContainerUIIndex][8] = cbg;
                    CT_ContainerUI[CT_ContainerUIIndex][9] = cb;
                    CT_ContainerUI[CT_ContainerUIIndex][10] = cw;
                    CT_ContainerUI[CT_ContainerUIIndex][11] = status;
                    ctui->index = CT_ContainerUIIndex; //UI对象索引
                    CT_ContainerUIIndex ++; //UI容器索引 +1
                    CT_InterfaceRepaint(); //请求重绘
                }
                else
                {
                    cprint("ERROR : Checkbox state error.");
                }
            }
            if(type == 4) //如果UI是单选框
            {
                int x = ctui->x; //单选框位置x
                int y = ctui->y; //单选框位置y
                int w = 16; //单选框宽度
                int h = 16; //单选框高度
                int cbg = ctui->color_background; //单选框背景颜色
                int cb = ctui->color_border; //单选框边框颜色
                int cw = ctui->color_widget; //单选框部件颜色
                int g = ctui->group; //单选框组
                if(g >= 0) //如果单选框组有效
                {
                    int radio_sel = 0;
                    int radio_first = -1;
                    int i3;
                    int i2;
                    for(i2=0;i2<CT_ContainerUIIndex;i2++) //遍历UI容器
                    {
                        if(CT_ContainerUI[i2][0] == NULL) //如果当前UI为空
                        {
                            //将UI数据写入UI容器
                            CT_ContainerUI[i2][0] = 1;
                            CT_ContainerUI[i2][1] = id;
                            CT_ContainerUI[i2][2] = win_index;
                            CT_ContainerUI[i2][3] = type;
                            CT_ContainerUI[i2][4] = x;
                            CT_ContainerUI[i2][5] = y;
                            CT_ContainerUI[i2][6] = w;
                            CT_ContainerUI[i2][7] = h;
                            CT_ContainerUI[i2][8] = cbg;
                            CT_ContainerUI[i2][9] = cb;
                            CT_ContainerUI[i2][10] = cw;
                            CT_ContainerUI[i2][11] = g;
                            CT_ContainerUI[i2][12] = 0;
                            ctui->index = i2;
                            for(i3=0;i3<CT_ContainerUIIndex;i3++) //遍历UI容器
                            {
                                if(CT_ContainerUI[i3][0] != NULL) //如果当前UI对象不为空
                                {
                                    int ui_type = CT_ContainerUI[i3][3]; //当前UI类型
                                    if(ui_type == 4) //如果当前UI是单选框
                                    {
                                        int radio_g = CT_ContainerUI[i3][11]; //当前单选框组
                                        if(radio_g == g) //如果当前单选框组等于单选框组
                                        {
                                            int radio_s = CT_ContainerUI[i3][12]; //当前单选框状态
                                            if(radio_first == -1) //如果没有确定单选框组中的第一个单选框索引
                                            {
                                                radio_first = i3; //确定单选框组中的第一个单选框索引
                                            }
                                            if(radio_s == 1) //如果当前单选框已选择
                                            {
                                                radio_sel = 1; //单选框组中的选择的单选框索引
                                            }
                                        }
                                    }
                                }
                            }
                            if(radio_sel == 0) //如果没有单选框被选择
                            {
                                CT_ContainerUI[radio_first][12] = 1; //将单选框组中的第一个单选框设置为选择
                            }
                            CT_InterfaceRepaint(); //请求重绘
                            return; //中断
                        }
                    }
                    //将UI数据写入UI容器
                    CT_ContainerUI[CT_ContainerUIIndex][0] = 1;
                    CT_ContainerUI[CT_ContainerUIIndex][1] = id;
                    CT_ContainerUI[CT_ContainerUIIndex][2] = win_index;
                    CT_ContainerUI[CT_ContainerUIIndex][3] = type;
                    CT_ContainerUI[CT_ContainerUIIndex][4] = x;
                    CT_ContainerUI[CT_ContainerUIIndex][5] = y;
                    CT_ContainerUI[CT_ContainerUIIndex][6] = w;
                    CT_ContainerUI[CT_ContainerUIIndex][7] = h;
                    CT_ContainerUI[CT_ContainerUIIndex][8] = cbg;
                    CT_ContainerUI[CT_ContainerUIIndex][9] = cb;
                    CT_ContainerUI[CT_ContainerUIIndex][10] = cw;
                    CT_ContainerUI[CT_ContainerUIIndex][11] = g;
                    CT_ContainerUI[CT_ContainerUIIndex][12] = 0;
                    ctui->index = CT_ContainerUIIndex; //UI对象索引
                    CT_ContainerUIIndex ++; //UI容器索引 +1
                    CT_InterfaceRepaint(); //请求重绘
                    for(i3=0;i3<CT_ContainerUIIndex;i3++) //遍历UI容器
                    {
                        if(CT_ContainerUI[i3][0] != NULL) //如果当前UI对象不为空
                        {
                            int ui_type = CT_ContainerUI[i3][3]; //当前UI类型
                            if(ui_type == 4) //如果当前UI是单选框
                            {
                                int radio_g = CT_ContainerUI[i3][11]; //当前单选框组
                                if(radio_g == g) //如果当前单选框组等于单选框组
                                {
                                    int radio_s = CT_ContainerUI[i3][12]; //当前单选框状态
                                    if(radio_first == -1) //如果没有确定单选框组中的第一个单选框索引
                                    {
                                        radio_first = i3; //确定单选框组中的第一个单选框索引
                                    }
                                    if(radio_s == 1) //如果当前单选框已选择
                                    {
                                        radio_sel = 1; //单选框组中的选择的单选框索引
                                    }
                                }
                            }
                        }
                    }
                    if(radio_sel == 0) //如果没有单选框被选择
                    {
                        CT_ContainerUI[radio_first][12] = 1; //将单选框组中的第一个单选框设置为选择
                    }
                    CT_InterfaceRepaint(); //请求重绘
                }
                else
                {
                    cprint("ERROR : Invalid radio group.");
                }
            }
            if(type == 5) //如果UI是进度条
            {
                int x = ctui->x; //进度条位置x
                int y = ctui->y; //进度条位置y
                int w = ctui->width; //进度条宽度
                int h = ctui->height; //进度条高度
                int cbg = ctui->color_background; //进度条背景颜色
                int cbu = ctui->color_borderUp; //进度条上边框颜色
                int cbd = ctui->color_borderDown; //进度条下边框颜色
                int cbl = ctui->color_borderLeft; //进度条左边框颜色
                int cbr = ctui->color_borderRight; //进度条右边框颜色
                int cw = ctui->color_widget; //进度条部件颜色
                int bi = ctui->background; //进度条背景图像
                int bio = ctui->background_o; //进度条背景图像偏移
                int val = ctui->value; //进度条默认值
                int maxval = ctui->maxvalue; //进度条最大值
                if(bi != -1) //如果有背景图像
                {
                    int res_type = CT_ResourceArgList[bi][1]; //获取资源类型
                    if(res_type != 0) //如果资源类型不是图像
                    {
                        bi = -1; //无背景图像
                        cprint("ERROR : The resource index corresponding to the resource file is not a image.");
                    }
                }
                if(w > 0 && h > 0 && val >= 0 && maxval > 0) //如果进度条大小正确
                {
                    int w_w = (w - 8) / maxval * val; //进度条点宽度
                    int i2;
                    for(i2=0;i2<CT_ContainerUIIndex;i2++) //遍历UI容器
                    {
                        if(CT_ContainerUI[i2][0] == NULL) //如果当前UI为空
                        {
                            //将UI数据写入UI容器
                            CT_ContainerUI[i2][0] = 1;
                            CT_ContainerUI[i2][1] = id;
                            CT_ContainerUI[i2][2] = win_index;
                            CT_ContainerUI[i2][3] = type;
                            CT_ContainerUI[i2][4] = x;
                            CT_ContainerUI[i2][5] = y;
                            CT_ContainerUI[i2][6] = w;
                            CT_ContainerUI[i2][7] = h;
                            CT_ContainerUI[i2][8] = cbg;
                            CT_ContainerUI[i2][9] = cbu;
                            CT_ContainerUI[i2][10] = cbd;
                            CT_ContainerUI[i2][11] = cbl;
                            CT_ContainerUI[i2][12] = cbr;
                            CT_ContainerUI[i2][13] = cw;
                            CT_ContainerUI[i2][14] = bi;
                            CT_ContainerUI[i2][15] = w_w;
                            CT_ContainerUI[i2][16] = val;
                            CT_ContainerUI[i2][17] = maxval;
                            CT_ContainerUI[i2][18] = bio; 
                            ctui->index = i2; //UI对象索引
                            CT_InterfaceRepaint(); //请求重绘
                            return; //中断
                        }
                    }
                    //将UI数据写入UI容器
                    CT_ContainerUI[CT_ContainerUIIndex][0] = 1;
                    CT_ContainerUI[CT_ContainerUIIndex][1] = id;
                    CT_ContainerUI[CT_ContainerUIIndex][2] = win_index;
                    CT_ContainerUI[CT_ContainerUIIndex][3] = type;
                    CT_ContainerUI[CT_ContainerUIIndex][4] = x;
                    CT_ContainerUI[CT_ContainerUIIndex][5] = y;
                    CT_ContainerUI[CT_ContainerUIIndex][6] = w;
                    CT_ContainerUI[CT_ContainerUIIndex][7] = h;
                    CT_ContainerUI[CT_ContainerUIIndex][8] = cbg;
                    CT_ContainerUI[CT_ContainerUIIndex][9] = cbu;
                    CT_ContainerUI[CT_ContainerUIIndex][10] = cbd;
                    CT_ContainerUI[CT_ContainerUIIndex][11] = cbl;
                    CT_ContainerUI[CT_ContainerUIIndex][12] = cbr;
                    CT_ContainerUI[CT_ContainerUIIndex][13] = cw;
                    CT_ContainerUI[CT_ContainerUIIndex][14] = bi;
                    CT_ContainerUI[CT_ContainerUIIndex][15] = w_w;
                    CT_ContainerUI[CT_ContainerUIIndex][16] = val;
                    CT_ContainerUI[CT_ContainerUIIndex][17] = maxval;
                    CT_ContainerUI[CT_ContainerUIIndex][18] = bio;
                    ctui->index = CT_ContainerUIIndex; //UI对象索引
                    CT_ContainerUIIndex ++; //UI容器索引 +1
                    CT_InterfaceRepaint(); //请求重绘
                }             
                else
                {
                    cprint("ERROR : Unable to create progressbar.");
                }
            }
            if(type == 6) //如果UI是文本框
            {
                int x = ctui->x; //文本框位置x
                int y = ctui->y; //文本框位置y
                int w = ctui->width; //文本框宽度
                int h = ctui->height; //文本框高度
                int cbg = ctui->color_background; //文本框背景颜色
                int cbu = ctui->color_borderUp; //文本框上边框颜色
                int cbd = ctui->color_borderDown; //文本框下边框颜色
                int cbl = ctui->color_borderLeft; //文本框左边框颜色
                int cbr = ctui->color_borderRight; //文本框右边框颜色
                int ct = ctui->color_text; //文本框字体颜色
                int bi = ctui->background; //文本框背景图像
                int bio = ctui->background_o; //文本框背景图像偏移
                char *name = ctui->name; //文本框名称
                int tpass = ctui->password; //密码文本框参数
                int tnumb = ctui->number; //数字文本框参数
                int tmax = ctui->max; //文本框最大字符数
                int readonly = ctui->readonly; //只读参数
                int subbox = ctui->subbox; //子文本框参数
                int font_i = CT_DF_Img; //字体图像
                int font_n = CT_DF_Number; //字体字符数
                int font_g = CT_DF_Gap; //字体字符间隙
                char *font_cl = CT_DF_CharList; //字体字符列表
                int *font_gl = CT_DF_GlyphList; //字体字形列表
                int text_h = CT_DF_Height; //按钮显示文本高度
                if(bi != -1) //如果有背景图像
                {
                    int res_type = CT_ResourceArgList[bi][1]; //获取资源类型
                    if(res_type != 0) //如果资源类型不是图像
                    {
                        bi = -1; //无背景图像
                        cprint("ERROR : The resource index corresponding to the resource file is not a image.");
                    }
                }
                if(ctui->imgfont != NULL) //如果UI图像字体不为空
                {
                    if(ctui->imgfont->FontImg != -1 && ctui->imgfont->CharNumber > 0 && ctui->imgfont->CharList != NULL && ctui->imgfont->GlyphList != NULL) //如果资源不为空
                    {
                        font_i = ctui->imgfont->FontImg; //获取字体图像
                        font_n = ctui->imgfont->CharNumber; //获取字体字符数
                        font_g = ctui->imgfont->CharGap; //获取字体字符间隙
                        font_cl = ctui->imgfont->CharList; //获取字体字符列表
                        font_gl = ctui->imgfont->GlyphList; //获取字体字形列表
                        text_h = ctui->imgfont->CharHeight; //按钮显示文本高度
                    }
                    else
                    {
                        cprint("ERROR : Image of the imgfont is invalid.");
                    }
                }
                if(w > 0 && h > 0 && tpass >= 0 && tnumb >= 0 && tmax > 0) //如果文本框大小正确
                {
                    int i2;
                    for(i2=0;i2<CT_ContainerUIIndex;i2++) //遍历UI容器
                    {
                        if(CT_ContainerUI[i2][0] == NULL) //如果当前UI为空
                        {
                            //将UI数据写入UI容器
                            CT_ContainerUI[i2][0] = 1;
                            CT_ContainerUI[i2][1] = id;
                            CT_ContainerUI[i2][2] = win_index;
                            CT_ContainerUI[i2][3] = type;
                            CT_ContainerUI[i2][4] = x;
                            CT_ContainerUI[i2][5] = y;
                            CT_ContainerUI[i2][6] = w;
                            CT_ContainerUI[i2][7] = h;
                            CT_ContainerUI[i2][8] = cbg;
                            CT_ContainerUI[i2][9] = cbu;
                            CT_ContainerUI[i2][10] = cbd;
                            CT_ContainerUI[i2][11] = cbl;
                            CT_ContainerUI[i2][12] = cbr;
                            CT_ContainerUI[i2][13] = ct;
                            CT_ContainerUI[i2][14] = bi;
                            CT_ContainerUI[i2][15] = tpass;
                            CT_ContainerUI[i2][16] = tnumb;
                            CT_ContainerUI[i2][17] = tmax;
                            CT_ContainerUI[i2][18] = readonly;
                            CT_ContainerUI[i2][19] = subbox;
                            CT_ContainerUI[i2][20] = font_i;
                            CT_ContainerUI[i2][21] = font_n;
                            CT_ContainerUI[i2][22] = font_g;
                            CT_ContainerUI[i2][23] = text_h;
                            CT_ContainerUI[i2][24] = bio;
                            CT_ContainerUI_FontCharList_P[i2] = font_cl;
                            CT_ContainerUI_FontGlythList_P[i2] = font_gl;
                            strcpy(CT_ContainerUI_Name[i2],name);
                            strcpy(CT_ContainerUI_Value[i2],"");
                            ctui->index = i2; //UI对象索引
                            CT_InterfaceRepaint(); //请求重绘
                            return; //中断
                        }
                    }
                    //将UI数据写入UI容器
                    CT_ContainerUI[CT_ContainerUIIndex][0] = 1;
                    CT_ContainerUI[CT_ContainerUIIndex][1] = id;
                    CT_ContainerUI[CT_ContainerUIIndex][2] = win_index;
                    CT_ContainerUI[CT_ContainerUIIndex][3] = type;
                    CT_ContainerUI[CT_ContainerUIIndex][4] = x;
                    CT_ContainerUI[CT_ContainerUIIndex][5] = y;
                    CT_ContainerUI[CT_ContainerUIIndex][6] = w;
                    CT_ContainerUI[CT_ContainerUIIndex][7] = h;
                    CT_ContainerUI[CT_ContainerUIIndex][8] = cbg;
                    CT_ContainerUI[CT_ContainerUIIndex][9] = cbu;
                    CT_ContainerUI[CT_ContainerUIIndex][10] = cbd;
                    CT_ContainerUI[CT_ContainerUIIndex][11] = cbl;
                    CT_ContainerUI[CT_ContainerUIIndex][12] = cbr;
                    CT_ContainerUI[CT_ContainerUIIndex][13] = ct;
                    CT_ContainerUI[CT_ContainerUIIndex][14] = bi;
                    CT_ContainerUI[CT_ContainerUIIndex][15] = tpass;
                    CT_ContainerUI[CT_ContainerUIIndex][16] = tnumb;
                    CT_ContainerUI[CT_ContainerUIIndex][17] = tmax;
                    CT_ContainerUI[CT_ContainerUIIndex][18] = readonly;
                    CT_ContainerUI[CT_ContainerUIIndex][19] = subbox;
                    CT_ContainerUI[CT_ContainerUIIndex][20] = font_i;
                    CT_ContainerUI[CT_ContainerUIIndex][21] = font_n;
                    CT_ContainerUI[CT_ContainerUIIndex][22] = font_g;
                    CT_ContainerUI[CT_ContainerUIIndex][23] = text_h;
                    CT_ContainerUI[CT_ContainerUIIndex][24] = bio;
                    CT_ContainerUI_FontCharList_P[i2] = font_cl;
                    CT_ContainerUI_FontGlythList_P[i2] = font_gl;
                    strcpy(CT_ContainerUI_Value[CT_ContainerUIIndex],"");
                    strcpy(CT_ContainerUI_Name[CT_ContainerUIIndex],name);
                    ctui->index = CT_ContainerUIIndex; //UI对象索引
                    CT_ContainerUIIndex ++; //UI容器索引 +1
                    CT_InterfaceRepaint(); //请求重绘
                }
                else
                {
                    cprint("ERROR : Unable to create textbox.");
                }
            }
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 销毁UI
//--------------------------------------------------------------------
void CTui_destroy(CTui *ctui)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        int ui_type = ctui->type; //ui对象类型
        if(ui_type > 0) //如果是UI对象
        {
            int ui_index = ctui->index; //ui对象索引
            if(CT_ContainerUI[ui_index][0] != NULL) //如果UI容器中指定UI对象数据不为空
            {
                int Cui_id = CT_ContainerUI[ui_index][1]; //UI容器中指定UI对象的ID
                if(Cui_id == ctui->id) //如果UI对象id等于容器中指定UI对象的ID
                {
                    int i;
                    for(i=0;i<25;i++) //遍历UI对象数据
                    {
                        CT_ContainerUI[ui_index][i] = NULL; //清除UI容器中UI对象的数据
                    }
                    if(ui_type == 1 || ui_type == 2 || ui_type == 6) //如果ui类型是按钮 文本 文本框
                    {
                        strclear(CT_ContainerUI_Value[ui_index]); //清除UI容器中UI对象的字符串数据
                        CT_ContainerUI_FontCharList_P[ui_index] = NULL;
                        CT_ContainerUI_FontGlythList_P[ui_index] = NULL;
                    }
                    CT_InterfaceRepaint(); //请求重绘
                }
            }
        }
        else
        {
            cprint("ERROR : Not is UI object.");
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 设置UI位置x
//--------------------------------------------------------------------
void CTui_set_x(CTui *ctui,int x)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        int ui_type = ctui->type; //ui对象类型
        if(ui_type > 0) //如果对象是UI对象
        {
            int ui_index = ctui->index; //ui对象索引
            if(CT_ContainerUI[ui_index] != NULL) //如果UI容器中指定UI对象的数据不为空
            {
                int Cui_id = CT_ContainerUI[ui_index][1]; //UI容器中指定UI对象的ID
                if(Cui_id == ctui->id) //如果UI对象id等于容器中指定UI对象的ID
                {
                    CT_ContainerUI[ui_index][4] = x; //设置UI对象位置x
                    CT_InterfaceRepaint(); //请求重绘
                }
            }
        }
        else
        {
            cprint("ERROR : Not is UI object.");
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 设置UI位置y
//--------------------------------------------------------------------
void CTui_set_y(CTui *ctui,int y)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        int ui_type = ctui->type; //ui对象类型
        if(ui_type > 0) //如果对象是UI对象
        {
            int ui_index = ctui->index; //ui对象索引
            if(CT_ContainerUI[ui_index] != NULL) //如果UI容器中指定UI对象的数据不为空
            {
                int Cui_id = CT_ContainerUI[ui_index][1]; //UI容器中指定UI对象的ID
                if(Cui_id == ctui->id) //如果UI对象id等于容器中指定UI对象的ID
                {
                    CT_ContainerUI[ui_index][5] = y; //设置UI对象位置y
                    CT_InterfaceRepaint(); //请求重绘
                }
            }
        }
        else
        {
            cprint("ERROR : Not is UI object.");
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 设置UI宽度
//--------------------------------------------------------------------
void CTui_set_width(CTui *ctui,int w)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        int ui_type = ctui->type; //ui对象类型
        if(ui_type > 0) //如果对象是UI对象
        {
            int ui_index = ctui->index; //ui对象索引
            if(CT_ContainerUI[ui_index][0] != NULL) //如果UI容器中指定窗口对象数据不为空
            {
                int Cui_id = CT_ContainerUI[ui_index][1]; //UI容器中指定UI对象的ID
                if(Cui_id == ctui->id) //如果UI对象id等于容器中指定UI对象的ID
                {
                    if(ui_type == 1 || ui_type == 6) //如果UI对象是按钮 文本框
                    {
                        CT_ContainerUI[ui_index][6] = w; //设置宽度
                        CT_InterfaceRepaint(); //请求重绘
                    }
                    if(ui_type == 5) //如果UI对象是进度条
                    {
                        int p_w = CT_ContainerUI[ui_index][6]; //进度条宽度
                        int val = CT_ContainerUI[ui_index][16]; //进度条值
                        int p_maxval = CT_ContainerUI[ui_index][17]; //进度条最大值
                        int p_ww  = 0;
                        CT_ContainerUI[ui_index][6] = w; //设置宽度
                        if(val < p_maxval) //如果设置的值小于最大值
                        {
                            p_ww = (p_w - 8) / p_maxval * val; //内条宽度
                        }
                        if(val == p_maxval) //如果设置的值等于最大值
                        {
                            p_ww = p_w - 8; //点宽度
                        }
                        CT_ContainerUI[ui_index][15] = p_ww; //设置部件宽度
                        CT_InterfaceRepaint(); //请求重绘
                    }
                    if(ui_type == 2 || ui_type == 3 || ui_type == 4) //如果UI对象是文本 复选框 单选框
                    {
                        cprint("ERROR : The UI object not have this property.");
                    }
                }
            }
        }
        else
        {
            cprint("ERROR : Not is UI object.");
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 设置UI高度
//--------------------------------------------------------------------
void CTui_set_height(CTui *ctui,int h)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        int ui_type = ctui->type; //ui对象类型
        if(ui_type > 0) //如果对象是UI对象
        {
            int ui_index = ctui->index; //ui对象索引
            if(CT_ContainerUI[ui_index][0] != NULL) //如果UI容器中指定窗口对象数据不为空
            {
                int Cui_id = CT_ContainerUI[ui_index][1]; //UI容器中指定UI对象的ID
                if(Cui_id == ctui->id) //如果UI对象id等于容器中指定UI对象的ID
                {
                    if(ui_type == 1 || ui_type == 5 || ui_type == 6) //如果UI对象是按钮 文本框
                    {
                        CT_ContainerUI[ui_index][7] = h; //设置高度
                        CT_InterfaceRepaint(); //请求重绘
                    }
                    if(ui_type == 2 || ui_type == 3 || ui_type == 4) //如果UI对象是文本 复选框 单选框
                    {
                        cprint("ERROR : The UI object not have this property.");
                    }
                }
            }
        }
        else //如果对象不是UI对象
        {
            cprint("ERROR : Not is UI object.");
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 设置UI图像
//--------------------------------------------------------------------
void CTui_set_image(CTui *ctui,int img)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        int ui_type = ctui->type; //ui对象类型
        if(ui_type > 0) //如果对象是UI对象
        {
            int ui_index = ctui->index; //ui对象索引
            if(CT_ContainerUI[ui_index][0] != NULL) //如果UI容器中指定UI对象的数据不为空
            {
                int Cui_id = CT_ContainerUI[ui_index][1]; //UI容器中指定UI对象的ID
                if(Cui_id == ctui->id) //如果UI对象id等于容器中指定UI对象的ID
                {
                    if(ui_type == 1 || ui_type == 5 || ui_type == 6) //如果UI对象是按钮 进度条 文本框
                    {
                        if(img != -1) //如果有背景图像
                        {
                            int res_type = CT_ResourceArgList[img][1]; //获取资源类型
                            if(res_type == 0)
                            {
                                CT_ContainerUI[ui_index][14] = img; //设置背景图像
                                CT_InterfaceRepaint(); //请求重绘
                            }
                            else
                            {
                                img = -1;
                                cprint("ERROR : The resource index corresponding to the resource file is not a image.");
                            }
                        }
                        else
                        {
                            CT_ContainerUI[ui_index][14] = img; //设置背景图像
                            CT_InterfaceRepaint(); //请求重绘
                        }
                    }
                    if(ui_type == 2 || ui_type == 3 || ui_type == 4) //如果UI对象是文本 复选框 单选框
                    {
                        cprint("ERROR : The UI object not support background image.");
                    }
                }
            }
        }
        else //如果对象不是UI对象
        {
            cprint("ERROR : Not is UI object.");
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 设置UI颜色
//--------------------------------------------------------------------
void CTui_set_color(CTui *ctui,char *prop,int col)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        int ui_type = ctui->type; //ui对象类型
        if(ui_type > 0) //如果对象是UI对象
        {
            int ui_index = ctui->index; //ui对象索引
            if(CT_ContainerUI[ui_index][0] != NULL) //如果UI容器中指定UI对象的数据不为空
            {
                int Cui_id = CT_ContainerUI[ui_index][1]; //UI容器中指定UI对象的ID
                if(Cui_id == ctui->id) //如果UI对象id等于容器中指定UI对象的ID
                {
                    if(strcmp(prop,"back") == 0) //如果属性是背景
                    {
                        if(ui_type == 1 || ui_type == 3 || ui_type == 4 || ui_type == 5 || ui_type == 6) //如果UI对象是按钮 复选框 单选框 进度条 文本框
                        {
                            CT_ContainerUI[ui_index][8] = col; //设置背景颜色
                            CT_InterfaceRepaint(); //请求重绘
                        }
                        if(ui_type == 2) //如果UI对象是文本
                        {
                            cprint("ERROR : The UI object not have this property.");
                        }
                    }
                    if(strcmp(prop,"up") == 0) //如果属性是上边框
                    {
                        if(ui_type == 1 || ui_type == 3 || ui_type == 4 || ui_type == 5 || ui_type == 6) //如果UI对象是按钮 复选框 单选框 进度条 文本框
                        {
                            CT_ContainerUI[ui_index][9] = col; //设置上边框颜色
                            CT_InterfaceRepaint(); //请求重绘
                        }
                        if(ui_type == 2) //如果UI对象是文本
                        {
                            cprint("ERROR : The UI object not have this property.");
                        }
                    }
                    if(strcmp(prop,"down") == 0) //如果属性是下边框
                    {
                        if(ui_type == 1 || ui_type == 5 || ui_type == 6) //如果UI对象是按钮 进度条 文本框
                        {
                            CT_ContainerUI[ui_index][10] = col; //设置下边框颜色
                            CT_InterfaceRepaint(); //请求重绘
                        }
                        if(ui_type == 3 || ui_type == 4) //如果UI对象是复选框 单选框
                        {
                            CT_ContainerUI[ui_index][9] = col; //设置边框颜色
                            CT_InterfaceRepaint(); //请求重绘
                        }
                        if(ui_type == 2) //如果UI对象是文本
                        {
                            cprint("ERROR : The UI object not have this property.");
                        }
                    }
                    if(strcmp(prop,"left") == 0) //如果属性是左边框
                    {
                        if(ui_type == 1 || ui_type == 5 || ui_type == 6) //如果UI对象是按钮 进度条 文本框
                        {
                            CT_ContainerUI[ui_index][11] = col; //设置下边框颜色
                            CT_InterfaceRepaint(); //请求重绘
                        }
                        if(ui_type == 3 || ui_type == 4) //如果UI对象是复选框 单选框
                        {
                            CT_ContainerUI[ui_index][9] = col; //设置复选框边框颜色
                            CT_InterfaceRepaint(); //请求重绘
                        }
                        if(ui_type == 2) //如果UI对象是文本
                        {
                            cprint("ERROR : The UI object not have this property.");
                        }
                    }
                    if(strcmp(prop,"right") == 0) //如果属性是右边框
                    {
                        if(ui_type == 1 || ui_type == 5 || ui_type == 6) //如果UI对象是按钮 进度条 文本框
                        {
                            CT_ContainerUI[ui_index][12] = col; //设置下边框颜色
                            CT_InterfaceRepaint(); //请求重绘
                        }
                        if(ui_type == 3 || ui_type == 4) //如果UI对象是复选框 单选框
                        {
                            CT_ContainerUI[ui_index][9] = col; //设置边框颜色
                            CT_InterfaceRepaint(); //请求重绘
                        }
                        if(ui_type == 2) //如果UI对象是文本
                        {
                            cprint("ERROR : The UI object not have this property.");
                        }
                    }
                    if(strcmp(prop,"text") == 0) //如果属性是文本
                    {
                        if(ui_type == 1 || ui_type == 6) //如果UI对象是按钮 文本框
                        {
                            CT_ContainerUI[ui_index][13] = col; //设置文本颜色
                        }
                        if(ui_type == 2) //如果UI对象是文本
                        {
                            CT_ContainerUI[ui_index][6] = col; //设置文本颜色
                        }
                        if(ui_type == 3 || ui_type == 4 || ui_type == 5)
                        {
                            cprint("ERROR : The UI object not have this property.");
                        }
                    }
                    if(strcmp(prop,"widget") == 0) //如果属性是部件
                    {
                        if(ui_type == 3 || ui_type == 4) //如果UI对象是复选框 单选框
                        {
                            CT_ContainerUI[ui_index][10] = col; //设置部件颜色
                        }
                        if(ui_type == 5) //如果UI对象是进度条
                        {
                            CT_ContainerUI[ui_index][13] = col; //设置部件颜色
                        }
                        if(ui_type == 1 || ui_type == 2 || ui_type == 6)
                        {
                            cprint("ERROR : The UI object not have this property.");
                        }
                    }
                }
            }
        }
    }
    else //如果对象不是UI对象
    {
        cprint("ERROR : Not is UI object.");
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 设置UI图像字体
//--------------------------------------------------------------------
void CTui_set_imgfont(CTui *ctui,CTimgfont *font)
{
    int char_n = CT_DF_Number; //图像字体字符数
    int char_g = CT_DF_Gap; //图像字体间隙
    int ifont_img = CT_DF_Img; //图像字体图像
    int ifont_h = CT_DF_Height; //图像字体字符高度
    char *ifont_clist = CT_DF_CharList; //图像字体字符列表
    char *ifont_glist = CT_DF_GlyphList; //图像字体字形列表
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        int ui_type = ctui->type; //ui对象类型
        if(ui_type == 1 || ui_type == 2 || ui_type == 6) //如果对象是UI对象
        {
            int ui_index = ctui->index; //ui对象索引
            if(CT_ContainerUI[ui_index] != NULL) //如果UI容器中指定UI对象的数据不为空
            {
                int Cui_id = CT_ContainerUI[ui_index][1]; //UI容器中指定UI对象的ID
                if(Cui_id == ctui->id) //如果UI对象id等于容器中指定UI对象的ID
                {
                    if(ui_type == 1) //如果UI对象是按钮
                    {
                        char *ui_text = CT_ContainerUI_Value[ui_index]; //获取按钮显示文本
                        int text_w = 0; //文本宽度
                        if(font != NULL) //如果字体不为空
                        {
                            char_n = font->CharNumber; //图像字体字符数
                            char_g = font->CharGap; //图像字体间隙
                            ifont_img = font->FontImg; //图像字体图像
                            ifont_h = font->CharHeight; //图像字体字符高度
                            ifont_clist = font->CharList; //图像字体字符列表
                            ifont_glist = font->GlyphList; //图像字体字形列表
                        }
                        text_w = CT_ImgStringMeasure(char_n,char_g,ifont_clist,ifont_glist,ui_text); //测量图像字符串像素宽度
                        //将字体数据存入UI容器
                        CT_ContainerUI[ui_index][15] = text_w;
                        CT_ContainerUI[ui_index][16] = ifont_h;
                        CT_ContainerUI[ui_index][21] = ifont_img;
                        CT_ContainerUI[ui_index][22] = char_n;
                        CT_ContainerUI[ui_index][23] = char_g;
                        //存入字体列表
                        CT_ContainerUI_FontCharList_P[ui_index] = ifont_clist;
                        CT_ContainerUI_FontGlythList_P[ui_index] = ifont_glist;
                        CT_InterfaceRepaint(); //请求重绘
                    }
                    if(ui_type == 2) //如果UI对象是文本
                    {
                        if(font != NULL) //如果字体不为空
                        {
                            char_n = font->CharNumber; //图像字体字符数
                            char_g = font->CharGap; //图像字体间隙
                            ifont_img = font->FontImg; //图像字体图像
                            ifont_h = font->CharHeight; //图像字体字符高度
                            ifont_clist = font->CharList; //图像字体字符列表
                            ifont_glist = font->GlyphList; //图像字体字形列表
                        }
                        //将字体数据存入UI容器
                        CT_ContainerUI[ui_index][7] = ifont_img;
                        CT_ContainerUI[ui_index][8] = char_n;
                        CT_ContainerUI[ui_index][9] = char_g;
                        CT_ContainerUI_FontCharList_P[ui_index] = ifont_clist;
                        CT_ContainerUI_FontGlythList_P[ui_index] = ifont_glist;
                        CT_InterfaceRepaint(); //请求重绘
                    }
                    if(ui_type == 6) //如果UI对象是文本框
                    {
                        if(font != NULL) //如果字体不为空
                        {
                            char_n = font->CharNumber; //图像字体字符数
                            char_g = font->CharGap; //图像字体间隙
                            ifont_img = font->FontImg; //图像字体图像
                            ifont_h = font->CharHeight; //图像字体字符高度
                            ifont_clist = font->CharList; //图像字体字符列表
                            ifont_glist = font->GlyphList; //图像字体字形列表
                        }
                        //将字体数据存入UI容器
                        CT_ContainerUI[ui_index][20] = ifont_img;
                        CT_ContainerUI[ui_index][21] = char_n;
                        CT_ContainerUI[ui_index][22] = char_g;
                        CT_ContainerUI[ui_index][23] = ifont_h;
                        CT_ContainerUI_FontCharList_P[ui_index] = ifont_clist;
                        CT_ContainerUI_FontGlythList_P[ui_index] = ifont_glist;
                        CT_InterfaceRepaint(); //请求重绘
                    }
                    
                }
            }
        }
        else
        {
            cprint("ERROR : Not is UI object.");
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 按钮点击判断
//--------------------------------------------------------------------
int button_click(CTui *ctui)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(ctui->type == 1) //如果UI对象是按钮
        {
            if(CT_ContainerUI[ctui->index] != NULL) //如果UI容器中指定UI对象的数据不为空
            {
                int Cui_id = CT_ContainerUI[ctui->index][1]; //UI容器中指定UI对象的ID
                if(ctui->id == Cui_id) //如果UI对象id等于容器中指定UI对象的ID
                {
                    int b_s = CT_ContainerUI[ctui->index][17]; //指定按钮对象的状态
                    if(b_s == 1) //如果按钮被点击
                    {
                        return 1;
                    }
                }
            }
        }
        else
        {
            cprint("ERROR : Not is button object.");
        }
    }
    return 0;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 按钮始终按下判断
//--------------------------------------------------------------------
int button_press(CTui *ctui)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(ctui->type == 1) //如果UI对象是按钮
        {
            if(CT_ContainerUI[ctui->index] != NULL) //如果UI容器中指定UI对象的数据不为空
            {
                int Cui_id = CT_ContainerUI[ctui->index][1]; //UI容器中指定UI对象的ID
                if(ctui->id == Cui_id) //如果UI对象id等于容器中指定UI对象的ID
                {
                    int b_s = CT_ContainerUI[ctui->index][18]; //指定按钮对象的状态
                    if(b_s == 1) //如果按钮被始终按下
                    {
                        return 1;
                    }
                }
            }
        }
        else
        {
            cprint("ERROR : Not is button object.");
        }
    }
    return 0;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 按钮按下判断
//--------------------------------------------------------------------
int button_down(CTui *ctui)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(ctui->type == 1) //如果UI对象是按钮
        {
            if(CT_ContainerUI[ctui->index] != NULL) //如果UI容器中指定UI对象的数据不为空
            {
               int  Cui_id = CT_ContainerUI[ctui->index][1]; //UI容器中指定UI对象的ID
                if(ctui->id == Cui_id) //如果UI对象id等于容器中指定UI对象的ID
                {
                    int b_s = CT_ContainerUI[ctui->index][19]; //指定按钮对象的状态
                    if(b_s == 1) //如果按钮被按下
                    {
                        return 1;
                    }
                }
            }
        }
        else
        {
            cprint("ERROR : Not is button object.");
        }
    }
    return 0;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 按钮松开判断
//--------------------------------------------------------------------
int button_up(CTui *ctui)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(ctui->type == 1) //如果UI对象是按钮
        {
            if(CT_ContainerUI[ctui->index] != NULL) //如果UI容器中指定UI对象的数据不为空
            {
                int Cui_id = CT_ContainerUI[ctui->index][1]; //UI容器中指定UI对象的ID
                if(ctui->id == Cui_id) //如果UI对象id等于容器中指定UI对象的ID
                {
                    int b_s = CT_ContainerUI[ctui->index][20]; //指定按钮对象的状态
                    if(b_s == 1) //如果按钮被松开
                    {
                        return 1;
                    }
                }
            }
        }
        else
        {
            cprint("ERROR : Not is button object.");
        }
    }
    return 0;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 复选框状态
//--------------------------------------------------------------------
int checkbox_status(CTui *ctui)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(ctui->type == 3) //如果UI对象是复选框
        {
            if(CT_ContainerUI[ctui->index][0] != NULL) //如果UI容器中指定UI对象的数据不为空
            {
                int Cui_id = CT_ContainerUI[ctui->index][1]; //UI容器中指定UI对象的ID
                if(ctui->id == Cui_id) //如果UI对象id等于容器中指定UI对象的ID
                {
                    int c_s = CT_ContainerUI[ctui->index][11]; //复选框状态
                    return c_s; //返回复选框状态
                }
            }
        }
        else //如果UI对象不是复选框
        {
            cprint("ERROR : Not is checkbox object.");
        }
    }
    return 0;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 单选框状态
//--------------------------------------------------------------------
int radio_status(CTui *ctui)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(ctui->type == 4) //如果UI对象是复选框
        {
            if(CT_ContainerUI[ctui->index][0] != NULL) //如果UI容器中指定UI对象的数据不为空
            {
                int Cui_id = CT_ContainerUI[ctui->index][1]; //UI容器中指定UI对象的ID
                if(ctui->id == Cui_id) //如果UI对象id等于容器中指定UI对象的ID
                {
                    int r_s = CT_ContainerUI[ctui->index][12]; //复选框状态
                    return r_s; //返回复选框状态
                }
            }
        }
        else //如果UI对象不是复选框
        {
            cprint("ERROR : Not is radio object.");
        }
    }
    return 0;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 进度条设置
//--------------------------------------------------------------------
void progressbar_set(CTui *ctui,int val)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(ctui->type == 5)
        {
            if(CT_ContainerUI[ctui->index][0] != NULL) //如果UI容器中指定进度条对象的数据不为空
            {
                int Cui_id = CT_ContainerUI[ctui->index][1]; //UI容器中指定UI对象的ID
                if(ctui->id == Cui_id) //如果UI对象id等于容器中指定UI对象的ID
                {
                    int p_w = CT_ContainerUI[ctui->index][6]; //进度条宽度
                    int p_maxval = CT_ContainerUI[ctui->index][17]; //进度条最大值
                    int bp_w = 0; //进度条点宽度
                    if(val >= 0) //如果进度条值大于等于0
                    {
                        if(val <= p_maxval) //如果进度条值小于等于最大值
                        {
                            if(val < p_maxval) //如果设置的值小于最大值
                            {
                                bp_w = (p_w - 8) / p_maxval * val; //进度条点宽度
                            }
                            if(val == p_maxval) //如果设置的值等于最大值
                            {
                                bp_w = p_w - 8; //进度条点宽度
                            }
                            CT_ContainerUI[ctui->index][16] = val; //设置进度条值
                            CT_ContainerUI[ctui->index][15] = bp_w; //设置点宽度
                            CT_InterfaceRepaint(); //请求重绘
                        }
                        else
                        {
                            cprint("ERROR : The progressbar value greater than the maximum.");
                        }
                    }
                    else
                    {
                        cprint("ERROR : Progressbar value error.");
                    }
                }
            }
        }
        else
        {
            cprint("ERROR : Not is progressbar object.");
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 文本框的内容
//--------------------------------------------------------------------
char *textbox_string(CTui *ctui)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(ctui->type == 6) //如果UI对象是文本框
        {
            if(CT_ContainerUI[ctui->index][0] != NULL) //如果UI容器中指定文本框对象的数据不为空
            {
                int Cui_id = CT_ContainerUI[ctui->index][1]; //UI容器中指定UI对象的ID
                if(ctui->id == Cui_id) //如果UI对象id等于容器中指定UI对象的ID
                {
                    char *t_s = CT_ContainerUI_Value[ctui->index]; //复制文本框内容
                    return t_s;
                }
            }
        }
        else //如果UI对象不是复选框
        {
            cprint("ERROR : Not is textbox object.");
        }
    }
    return CT_NullStr;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 设置文本框的内容
//--------------------------------------------------------------------
void textbox_set_string(CTui *ctui,char *str)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(ctui->type == 6) //如果UI对象是文本框
        {
            if(CT_ContainerUI[ctui->index][0] != NULL) //如果UI容器中指定文本框对象的数据不为空
            {
                int Cui_id = CT_ContainerUI[ctui->index][1]; //UI容器中指定UI对象的ID
                if(ctui->id == Cui_id) //如果UI对象id等于容器中指定UI对象的ID
                {
                    strcpy(CT_ContainerUI_Value[ctui->index],str); //设置文本框内容
                    CT_InterfaceRepaint(); //请求重绘
                }
            }
        }
        else //如果UI对象不是复选框
        {
            cprint("ERROR : Not is textbox object.");
        }
    }
}
//--------------------------------------------------------------------