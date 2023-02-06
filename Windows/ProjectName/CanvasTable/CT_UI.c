#define _CRT_SECURE_NO_WARNINGS

#include "CanvasTableSYS.h"

extern int **CT_ResourceArgList;
extern int CT_DEVICE;
extern char CT_KeyChar;
extern int CT_Repaint;

//--------------------------------------------------------------------
// CTUI容器格式
//--------------------------------------------------------------------
// [窗口数据,...]
// 窗口:[占位符,id,层,x,y,宽度,高度,背景颜色,上边框颜色,下边框颜色,左边框颜色,右边框颜色,背景图像,背景图像偏移];
// 
// [UI数据,...]
// 按钮:[占位符,id,对应窗口的索引,1,x,y,宽度,高度,背景颜色,上边框颜色,下边框颜色,左边框颜色,右边框颜色,文本颜色,背景图像,文本宽度,文本高度,点击状态,始终按下状态,按下状态,松开状态,字体,字体字符数,字体字符间距,背景图像偏移]
// 文本:[占位符,id,对应窗口的索引,2,x,y,文本颜色,字体,字体字符数,字体字符间距]
// 复选框:[占位符,id,对应窗口的索引,3,x,y,宽度,高度,背景颜色,边框颜色,部件颜色,状态]
// 单选框:[占位符,id,对应窗口的索引,4,x,y,宽度,高度,背景颜色,边框颜色,部件颜色,组,状态]
// 进度条:[占位符,id,对应窗口的索引,5,x,y,宽度,高度,背景颜色,上边框颜色,下边框颜色,左边框颜色,右边框颜色,部件颜色,背景图像,部件宽度,值,最大值,背景图像偏移]
// 文本框:[占位符,id,对应窗口的索引,6,x,y,宽度,高度,背景颜色,上边框颜色,下边框颜色,左边框颜色,右边框颜色,字体颜色,背景图像,密码,数字,最大字符数,只读,子文本框,字体,字体字符数,字体字符间距,文本高度,背景图像偏移]
// 图像元件:[占位符,id,对应窗口的索引,7,x,y,图像索引,子图像索引,子图像数量,子图像宽度,子图像高度,子图像偏移]
//--------------------------------------------------------------------
int CT_UIId = 0; //UI id
int **CT_ContainerWindow = NULL; //窗口容器
int CT_ContainerWindowLength = 0; //窗口容器索引
int CT_ContainerCapacity_Window = 0; //窗口容器容量
int **CT_ContainerUI = NULL; //UI容器
char **CT_ContainerUI_Value = NULL; //UI值容器
char **CT_ContainerUI_Name = NULL; //UI名称索引
char **CT_ContainerUI_FontCharList_P = NULL; //UI字体字符列表容器
int **CT_ContainerUI_FontGlythList_P = NULL; //UI字体字形列表容器
int CT_ContainerUILength = 0; //UI容器索引
int CT_ContainerCapacity_UI = 0; //UI容器容量
int CT_Button_Index = -1; //按下按钮的索引
int CT_Textbox_Index = -1; //点击文本框的索引
int CT_DownX = -1; //鼠标按下的位置x
int CT_DownY = -1; //鼠标按下的位置y
int CT_DialogShow = 0; //对话框显示
int CT_Dialog_Index = -1; //对话框对应文本框的索引
//--------------------------------------------------------------------
// UI对象ID
//--------------------------------------------------------------------
int CT_UIGetId()
{
    int id = CT_UIId; //当前UI Id
    CT_UIId ++; //UI Id +1
    if(CT_UIId > 2100000000) //如果UI id大于2100000000
    {
        CT_UIId = 630000000; //UI id等于630000000
    }
    return id; //返回 UI Id
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// UI运行
//--------------------------------------------------------------------
void CT_UIRun()
{
    int i;
    for(i=0;i<CT_ContainerWindowLength;i++) //遍历窗口容器
    {
        if(CT_ContainerWindow[i][0] != NULL) //如果当前窗口不为空
        {
            int win_x = CT_ContainerWindow[i][3]; //窗口位置x
            int win_y = CT_ContainerWindow[i][4]; //窗口位置y
            int win_w = CT_ContainerWindow[i][5]; //窗口宽度
            int win_h = CT_ContainerWindow[i][6]; //窗口高度
            int i2;
            for(i2=0;i2<CT_ContainerUILength;i2++) //遍历UI容器
            {
                if(CT_ContainerUI[i2][0] != NULL) //如果当前UI对象不为空
                {
                    int ui_win_index = CT_ContainerUI[i2][2]; //当前UI对象对应窗口索引
                    if(ui_win_index == i) //如果前UI对象对应窗口索引等于当前窗口索引
                    {
                        int ui_x = win_x + CT_ContainerUI[i2][4]; //UI位置x
                        int ui_y = win_y + CT_ContainerUI[i2][5]; //UI位置y
                        int ui_w = CT_ContainerUI[i2][6]; //UI宽度
                        int ui_h = CT_ContainerUI[i2][7]; //UI高度
                        int ui_type = CT_ContainerUI[i2][3]; //所点击UI的类型
                        if(ui_type == 1) //如果当前UI对象是按钮
                        {
                            if(mouse_down(mkey_left)) //如果鼠标按下
                            {
                                CT_DownX = mouse_x(); //鼠标按下的位置x
                                CT_DownY = mouse_y(); //鼠标按下的位置y
                                if(CT_DownX >= ui_x && CT_DownY >= ui_y && CT_DownX < ui_x + ui_w && CT_DownY < ui_y + ui_h) //如果鼠标按下的位置在UI范围内
                                {
                                    if(CT_DialogShow == 0) //如果对话框没有显示
                                    {
                                        CT_ContainerUI[i2][18] = 1; //设置当前按钮的始终按下状态
                                        CT_ContainerUI[i2][19] = 1; //设置当前按钮的按下状态
                                    }
                                }
                            }
                            if(mouse_up(mkey_left)) //如果鼠标松开
                            {
                                int up_x = mouse_x(); //鼠标松开的位置x
                                int up_y = mouse_y(); //鼠标松开的位置y
                                CT_ContainerUI[i2][18] = 0; //设置当前按钮的始终按下状态
                                if(CT_DownX >= ui_x && CT_DownY >= ui_y && CT_DownX <= ui_x + ui_w && CT_DownY <= ui_y + ui_h && up_x >= ui_x && up_y >= ui_y && up_x <= ui_x + ui_w && up_y <= ui_y + ui_h) //如果鼠标按下的位置和松开的位置都在UI范围内
                                {
                                    if(CT_DialogShow == 0) //如果对话框没有显示
                                    {
                                        CT_ContainerUI[i2][17] = 1; //设置当前按钮的点击状态
                                        CT_ContainerUI[i2][20] = 1; //设置当前按钮的松开状态
                                    }
                                }
                            }
                        }
                        if(ui_type == 3) //如果当前UI对象是复选框
                        {
                            if(mouse_down(mkey_left)) //如果鼠标按下
                            {
                                CT_DownX = mouse_x(); //鼠标按下的位置x
                                CT_DownY = mouse_y(); //鼠标按下的位置y
                            }
                            if(mouse_up(mkey_left)) //如果鼠标松开
                            {
                                int up_x = mouse_x(); //鼠标松开的位置x
                                int up_y = mouse_y(); //鼠标松开的位置y
                                if(CT_DownX >= ui_x && CT_DownY >= ui_y && CT_DownX <= ui_x + ui_w && CT_DownY <= ui_y + ui_h && up_x >= ui_x && up_y >= ui_y && up_x <= ui_x + ui_w && up_y <= ui_y + ui_h) //如果鼠标按下的位置和松开的位置都在UI范围内
                                {
                                    int c_s = CT_ContainerUI[i2][11]; //复选框状态
                                    if(c_s == 0) //如果复选框没有勾选
                                    {
                                        CT_ContainerUI[i2][11] = 1; //设置复选框状态
                                        CT_Repaint = 1; //请求重绘
                                        return; //中断
                                    }
                                    if(c_s == 1) //如果复选框勾选
                                    {
                                        CT_ContainerUI[i2][11] = 0; //设置复选框状态
                                        CT_Repaint = 1; //请求重绘
                                        return; //中断
                                    }
                                }
                            }
                        }
                        if(ui_type == 4) //如果当前UI对象是单选框
                        {
                            if(mouse_down(mkey_left)) //如果鼠标按下
                            {
                                CT_DownX = mouse_x(); //鼠标按下的位置x
                                CT_DownY = mouse_y(); //鼠标按下的位置y
                            }
                            if(mouse_up(mkey_left)) //如果鼠标松开
                            {
                                int up_x = mouse_x(); //鼠标松开的位置x
                                int up_y = mouse_y(); //鼠标松开的位置y
                                if(CT_DownX >= ui_x && CT_DownY >= ui_y && CT_DownX <= ui_x + ui_w && CT_DownY <= ui_y + ui_h && up_x >= ui_x && up_y >= ui_y && up_x <= ui_x + ui_w && up_y <= ui_y + ui_h) //如果鼠标按下的位置和松开的位置都在UI范围内
                                {
                                    int radio_g = CT_ContainerUI[i2][11]; //当前单选框组
                                    int i3;
                                    for(i3=0;i3<CT_ContainerUILength;i3++) //遍历UI容器
                                    {
                                        if(CT_ContainerUI[i3][0] != NULL) //如果当前UI对象不为空
                                        {
                                            int c_ui_type = CT_ContainerUI[i3][3]; //UI容器当前UI对象类型
                                            if(c_ui_type == 4) //如果UI容器当前UI对象类型是单选框
                                            {
                                                int c_radio_g = CT_ContainerUI[i3][11]; //UI容器当前单选框组
                                                if(c_radio_g == radio_g) //如果UI容器当前单选框组等于当前单选框组
                                                {
                                                    CT_ContainerUI[i3][12] = 0; //将UI容器当前单选框状态设置为0
                                                }
                                            }
                                        }
                                    }
                                    CT_ContainerUI[i2][12] = 1; //将当前单选框状态设置为1
                                    CT_Repaint = 1; //请求重绘
                                }
                            }
                        }
                        if(ui_type == 6) //如果当前UI对象是文本框
                        {
                            if(mouse_down(mkey_left)) //如果鼠标按下
                            {
                                CT_DownX = mouse_x(); //鼠标按下的位置x
                                CT_DownY = mouse_y(); //鼠标按下的位置y
                            }
                            if(mouse_up(mkey_left)) //如果鼠标松开
                            {
                                int up_x = mouse_x(); //鼠标松开的位置x
                                int up_y = mouse_y(); //鼠标松开的位置y
                                if(CT_DownX >= ui_x && CT_DownY >= ui_y && CT_DownX <= ui_x + ui_w && CT_DownY <= ui_y + ui_h && up_x >= ui_x && up_y >= ui_y && up_x <= ui_x + ui_w && up_y <= ui_y + ui_h) //如果鼠标按下的位置和松开的位置都在UI范围内
                                {
                                    int subbox = CT_ContainerUI[i2][19]; //子文本框
                                    if(subbox == 0) //如果不使用子文本框
                                    {
                                        CT_Textbox_Index = i2; //点击的文本框在窗口中的索引
                                    }
                                    if(subbox == 1 || CT_DEVICE == 1) //如果使用子文本框或设备是移动平台
                                    {
                                        int readonly = CT_ContainerUI[i2][18]; //文本框只读参数
                                        if(readonly == 0) //如果文本框不是只读
                                        {
                                            char *t_val = CT_ContainerUI_Value[i2]; //文本框值
                                            char *t_name = CT_ContainerUI_Name[i2]; //文本框名称
                                            int t_max = CT_ContainerUI[i2][17]; //文本框最大字符数
                                            int t_pass = CT_ContainerUI[i2][15]; //文本框密码
                                            int t_numb = CT_ContainerUI[i2][16]; //文本框数字
                                            CT_Dialog_Index = i2; //对话框对应文本框的索引
                                            CT_Textbox_Index = -1; //取消点击的文本框
                                            CT_DialogCreate(t_val,t_name,t_max,t_pass,t_numb); //创建子文本框
                                        }
                                    }
                                    CT_Repaint = 1; //请求重绘
                                    return; //中断
                                }
                            }
                            if(CT_Textbox_Index == i2) //如果点击了文本框
                            {
                                if(CT_ContainerUI[CT_Textbox_Index][0] != NULL) //如果点击的文本框不为空
                                {
                                    int readonly = CT_ContainerUI[CT_Textbox_Index][18]; //文本框只读参数
                                    if(readonly == 0) //如果文本框不是只读
                                    {
                                        if(CT_KeyChar != 0x00 && CT_KeyChar != '\b') //如果输入了字符
                                        {
                                            int tbox_length = strlen(CT_ContainerUI_Value[CT_Textbox_Index]); //文本框字符串长度
                                            int tbox_max = CT_ContainerUI[CT_Textbox_Index][17]; //文本框最大字符数
                                            int tbox_number = CT_ContainerUI[CT_Textbox_Index][16]; //数字文本框参数
                                            if(tbox_length < tbox_max) //如果文本框字符串长度小于文本框最大字符数
                                            {
                                                if(tbox_number != 1) //如果文本框不是数字文本框
                                                {
                                                    CT_ContainerUI_Value[CT_Textbox_Index][tbox_length] = CT_KeyChar; //文本框增加字符
                                                    CT_Repaint = 1; //请求重绘
                                                }
                                                else //如果文本框是数字文本框
                                                {
                                                    if(CT_KeyChar == '0' || CT_KeyChar == '1' || CT_KeyChar == '2' || CT_KeyChar == '3' || CT_KeyChar == '4' || CT_KeyChar == '5' || CT_KeyChar == '6' || CT_KeyChar == '7' || CT_KeyChar == '8' || CT_KeyChar == '9') //如果当前输入的字符是数字
                                                    {
                                                        CT_ContainerUI_Value[CT_Textbox_Index][tbox_length] = CT_KeyChar; //文本框增加字符
                                                        CT_Repaint = 1; //请求重绘
                                                    }
                                                }
                                            }
                                        }
                                        if(CT_KeyChar == '\b') //如果输入了退格
                                        {
                                            int tbox_length = strlen(CT_ContainerUI_Value[CT_Textbox_Index]); //文本框字符串长度
                                            if(tbox_length > 0) //如果文本框字符串长度大于0
                                            {
                                                CT_ContainerUI_Value[CT_Textbox_Index][tbox_length-1] = 0x00; //文本框字符串减少一个字符
                                                CT_Repaint = 1; //请求重绘
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// UI渲染
//--------------------------------------------------------------------
void CT_UIRender(int l)
{
    int i;
    int win_index = 0;
    for(i=0;i<CT_ContainerWindowLength;i++) //遍历窗口容器
    {
        win_index = i; //当前窗口索引
        if(CT_ContainerWindow[i][0] != NULL) //如果当前窗口不为空
        {
            int window_layer = CT_ContainerWindow[i][2]; //当前窗口层
            if(window_layer == l) //如果当前窗口层等于渲染层
            {
                int win_x = CT_ContainerWindow[i][3]; //窗口位置x
                int win_y = CT_ContainerWindow[i][4]; //窗口位置y
                int win_w = CT_ContainerWindow[i][5]; //窗口宽度
                int win_h = CT_ContainerWindow[i][6]; //窗口高度
                int win_cbg = CT_ContainerWindow[i][7]; //窗口背景颜色
                int win_cbu = CT_ContainerWindow[i][8]; //窗口上边框颜色
                int win_cbd = CT_ContainerWindow[i][9]; //窗口下边框颜色
                int win_cbl = CT_ContainerWindow[i][10]; //窗口左边框颜色
                int win_cbr = CT_ContainerWindow[i][11]; //窗口右边框颜色
                int win_img = CT_ContainerWindow[i][12]; //窗口背景图像
                int win_imgo = CT_ContainerWindow[i][13]; //窗口背景图像偏移
                int i2;
                if(win_img != -1) //如果窗口背景图像不为空
                {
                    int img_w = CT_ResourceArgList[win_img][2] - win_imgo * 2; //背景图像宽度
                    int img_h = CT_ResourceArgList[win_img][3] - win_imgo * 2; //背景图像高度
                    CTCanvas_ImagePart(win_x,win_y,win_img,win_imgo,win_imgo,img_w,img_h); //绘制背景图像
                }
                else //如果窗口背景图像为空
                {
                    CTCanvas_Rectangle(win_x,win_y,win_w,win_h,win_cbg,1); //绘制窗口背景
                    CTCanvas_Rectangle(win_x,win_y-2,win_w,2,win_cbu,1); //绘制窗口上边框
                    CTCanvas_Rectangle(win_x,win_y+win_h,win_w,2,win_cbd,1); //绘制窗口下边框
                    CTCanvas_Rectangle(win_x-2,win_y-2,2,win_h+4,win_cbl,1); //绘制窗口左边框
                    CTCanvas_Rectangle(win_x+win_w,win_y-2,2,win_h+4,win_cbr,1); //绘制窗口右边框
                }
                for(i2=0;i2<CT_ContainerUILength;i2++) //遍历UI容器
                {
                    if(CT_ContainerUI[i2][0] != NULL) //如果当前UI对象不为空
                    {
                        int ui_win_index = CT_ContainerUI[i2][2]; //当前UI对象对应窗口索引
                        if(ui_win_index == win_index) //如果当前UI对象对应窗口索引等于当前窗口索引
                        {
                            int ui_type = CT_ContainerUI[i2][3]; //当前UI对象类型
                            if(ui_type == 1) //如果当前UI对象是按钮
                            {
                                int b_x = win_x + CT_ContainerUI[i2][4]; //按钮位置x
                                int b_y = win_y + CT_ContainerUI[i2][5]; //按钮位置y
                                int b_w = CT_ContainerUI[i2][6]; //按钮宽度
                                int b_h = CT_ContainerUI[i2][7]; //按钮高度
                                int b_cbg = CT_ContainerUI[i2][8]; //按钮背景颜色
                                int b_cbu = CT_ContainerUI[i2][9]; //按钮上边框颜色
                                int b_cbd = CT_ContainerUI[i2][10]; //按钮下边框颜色
                                int b_cbl = CT_ContainerUI[i2][11]; //按钮左边框颜色
                                int b_cbr = CT_ContainerUI[i2][12]; //按钮右边框颜色
                                int b_ct = CT_ContainerUI[i2][13]; //按钮文本颜色
                                int b_img = CT_ContainerUI[i2][14]; //按钮背景图像
                                int b_imgo = CT_ContainerUI[i2][24]; //背景图像偏移
                                char *b_text = CT_ContainerUI_Value[i2]; //按钮显示文本
                                int b_tw = CT_ContainerUI[i2][15]; //按钮文本宽度
                                int b_th = CT_ContainerUI[i2][16]; //按钮文本高度
                                int b_font_img = CT_ContainerUI[i2][21]; //字体图像
                                int b_font_charnum = CT_ContainerUI[i2][22]; //字体字符数
                                int b_font_chargap = CT_ContainerUI[i2][23]; //字体间隙
                                char *b_font_charlist = CT_ContainerUI_FontCharList_P[i2]; //字体字符列表
                                int *b_font_glyphlist = CT_ContainerUI_FontGlythList_P[i2]; //字体字形列表
                                int b_tx = b_x + ((b_w - b_tw) / 2); //显示文本位置x
                                int b_ty = b_y + ((b_h - b_th) / 2); //显示文本位置y
                                if(b_img != -1) //如果按钮背景图像不为空
                                {
                                    int img_w = CT_ResourceArgList[b_img][2] - b_imgo * 2; //背景图像宽度
                                    int img_h = CT_ResourceArgList[b_img][3] - b_imgo * 2; //背景图像高度
                                    CTCanvas_ImagePart(b_x,b_y,b_img,b_imgo,b_imgo,img_w,img_h); //绘制背景图像
                                }
                                else //如果按钮背景图像为空
                                {
                                    CTCanvas_Rectangle(b_x,b_y,b_w,b_h,b_cbg,1); //绘制按钮背景
                                    CTCanvas_Rectangle(b_x,b_y,b_w,2,b_cbu,1); //绘制按钮上边框
                                    CTCanvas_Rectangle(b_x,b_y+b_h-2,b_w,2,b_cbd,1); //绘制按钮下边框
                                    CTCanvas_Rectangle(b_x,b_y,2,b_h-2,b_cbl,1); //绘制按钮左边框
                                    CTCanvas_Rectangle(b_x+b_w-2,b_y,2,b_h-2,b_cbr,1); //绘制按钮右边框
                                }
                                CTCanvas_ImgString(b_tx,b_ty,b_ct,b_text,b_font_img,b_font_charnum,b_font_chargap,b_font_charlist,b_font_glyphlist); //绘制按钮文本
                            }
                            if(ui_type == 2) //如果当前UI对象是文本
                            {
                                int t_x = win_x + CT_ContainerUI[i2][4]; //文本位置x
                                int t_y = win_y + CT_ContainerUI[i2][5]; //文本位置y
                                int t_ct = CT_ContainerUI[i2][6]; //文本颜色
                                char *t_text = CT_ContainerUI_Value[i2]; //文本
                                int t_font_img = CT_ContainerUI[i2][7]; //字体图像
                                int t_font_charnum = CT_ContainerUI[i2][8]; //字体字符数
                                int t_font_chargap = CT_ContainerUI[i2][9]; //字体字符间隙
                                char *t_font_charlist = CT_ContainerUI_FontCharList_P[i2]; //字体字符列表
                                int *t_font_glyphlist = CT_ContainerUI_FontGlythList_P[i2]; //字体字形列表
                                CTCanvas_ImgString(t_x,t_y,t_ct,t_text,t_font_img,t_font_charnum,t_font_chargap,t_font_charlist,t_font_glyphlist); //绘制文本
                            }
                            if(ui_type == 3) //如果当前UI对象是复选框
                            {
                                int c_x = win_x + CT_ContainerUI[i2][4]; //复选框位置x
                                int c_y = win_y + CT_ContainerUI[i2][5]; //复选框位置y
                                int c_w = CT_ContainerUI[i2][6]; //复选框宽度
                                int c_h = CT_ContainerUI[i2][7]; //复选框高度
                                int c_cbg = CT_ContainerUI[i2][8]; //复选框背景颜色
                                int c_cb = CT_ContainerUI[i2][9]; //复选框边框颜色
                                int c_cw = CT_ContainerUI[i2][10]; //复选框部件颜色
                                int c_s = CT_ContainerUI[i2][11]; //复选框状态
                                CTCanvas_Rectangle(c_x,c_y,c_w,c_h,c_cb,1); //绘制复选框外边框
                                CTCanvas_Rectangle(c_x+2,c_y+2,c_w-4,c_h-4,c_cbg,1); //绘制复选框内边框
                                if(c_s == 1) //如果复选框状态为１
                                {
                                    CTCanvas_Line(c_x+4,c_y+9,c_x+8,c_y+13,2,c_cw); //绘制复选框部件1
                                    CTCanvas_Line(c_x+8,c_y+13,c_x+13,c_y+4,2,c_cw); //绘制复选框部件2
                                }
                            }
                            if(ui_type == 4) //如果当前UI对象是单选框
                            {
                                int r_x = win_x + CT_ContainerUI[i2][4]; //单选框位置x
                                int r_y = win_y + CT_ContainerUI[i2][5]; //单选框位置y
                                int r_w = CT_ContainerUI[i2][6]; //单选框宽度
                                int r_cbg = CT_ContainerUI[i2][8]; //单选框背景颜色
                                int r_cb = CT_ContainerUI[i2][9]; //单选框边框颜色
                                int r_cw = CT_ContainerUI[i2][10]; //单选框部件颜色
                                int r_s = CT_ContainerUI[i2][12]; //单选框状态
                                CTCanvas_Circle(r_x+8,r_y+8,r_w/2,r_cb,1); //绘制单选框边框
                                CTCanvas_Circle(r_x+8,r_y+8,r_w/2-2,r_cbg,1); //绘制单选框边框
                                if(r_s == 1) //如果单选框状态为１
                                {
                                    CTCanvas_Circle(r_x+8,r_y+8,r_w/2-4,r_cw,1); //绘制单选框部件
                                }
                            }
                            if(ui_type == 5) //如果当前UI对象是进度条
                            {
                                int p_x = win_x + CT_ContainerUI[i2][4]; //进度条位置x
                                int p_y = win_y + CT_ContainerUI[i2][5]; //进度条位置y
                                int p_w = CT_ContainerUI[i2][6]; //进度条宽度
                                int p_h = CT_ContainerUI[i2][7]; //进度条高度
                                int p_cbg = CT_ContainerUI[i2][8]; //进度条背景颜色
                                int p_cbu = CT_ContainerUI[i2][9]; //进度条上边框颜色
                                int p_cbd = CT_ContainerUI[i2][10]; //进度条下边框颜色
                                int p_cbl = CT_ContainerUI[i2][11]; //进度条左边框颜色
                                int p_cbr = CT_ContainerUI[i2][12]; //进度条右边框颜色
                                int p_cw = CT_ContainerUI[i2][13]; //进度条部件颜色
                                int p_img = CT_ContainerUI[i2][14]; //进度条背景图像
                                int p_imgo = CT_ContainerUI[i2][18];
                                int p_ww = CT_ContainerUI[i2][15]; //进度条部件宽度
                                if(p_img != -1) //如果进度条背景图像不为空
                                {
                                    int img_w = CT_ResourceArgList[p_img][2] - p_imgo * 2; //背景图像宽度
                                    int img_h = CT_ResourceArgList[p_img][3] - p_imgo * 2; //背景图像高度
                                    CTCanvas_ImagePart(p_x,p_y,p_img,p_imgo,p_imgo,img_w,img_h); //绘制背景图像
                                }
                                else //如果进度条背景图像为空
                                {
                                    CTCanvas_Rectangle(p_x,p_y,p_w,p_h,p_cbg,1); //绘制进度条背景
                                    CTCanvas_Rectangle(p_x,p_y,p_w,2,p_cbu,1); //绘制进度条上边框
                                    CTCanvas_Rectangle(p_x,p_y+p_h-2,p_w,2,p_cbd,1); //绘制进度条下边框
                                    CTCanvas_Rectangle(p_x,p_y,2,p_h-2,p_cbl,1); //绘制进度条左边框
                                    CTCanvas_Rectangle(p_x+p_w-2,p_y,2,p_h-2,p_cbr,1); //绘制进度条右边框
                                }
                                CTCanvas_Rectangle(p_x+4,p_y+4,p_ww,p_h-8,p_cw,1); //绘制进度条部件
                            }
                            if(ui_type == 6) //如果当前UI对象是文本框
                            {
                                int t_x = win_x + CT_ContainerUI[i2][4]; //文本框位置x
                                int t_y = win_y + CT_ContainerUI[i2][5]; //文本框位置y
                                int t_w = CT_ContainerUI[i2][6]; //文本框宽度
                                int t_h = CT_ContainerUI[i2][7]; //文本框高度
                                int t_cbg = CT_ContainerUI[i2][8]; //文本框背景颜色
                                int t_cbu = CT_ContainerUI[i2][9]; //文本框上边框颜色
                                int t_cbd = CT_ContainerUI[i2][10]; //文本框下边框颜色
                                int t_cbl = CT_ContainerUI[i2][11]; //文本框左边框颜色
                                int t_cbr = CT_ContainerUI[i2][12]; //文本框右边框颜色
                                int t_ct = CT_ContainerUI[i2][13]; //文本框字体颜色
                                int t_img = CT_ContainerUI[i2][14]; //文本框背景图像
                                int t_imgo = CT_ContainerUI[i2][24];
                                char *t_text = CT_ContainerUI_Value[i2]; //文本框字符串
                                int t_pass = CT_ContainerUI[i2][15]; //密码文本框参数
                                int t_readonly = CT_ContainerUI[i2][18]; //文本框只读参数
                                int t_font_img = CT_ContainerUI[i2][20]; //字体图像
                                int t_font_charnum = CT_ContainerUI[i2][21]; //字体最大字符数
                                int t_font_chargap = CT_ContainerUI[i2][22]; //字体字符间隙
                                int t_th = CT_ContainerUI[i2][23]; //文本高度
                                char *t_font_charlist = CT_ContainerUI_FontCharList_P[i2]; //字体字符列表
                                int *t_font_glyphlist = CT_ContainerUI_FontGlythList_P[i2]; //字体字形列表
                                int t_ty = t_y + (t_h - t_th) / 2; //文本框字符串位置y
                                if(t_img != -1) //如果文本框背景图像不为空
                                {
                                    int img_w = CT_ResourceArgList[t_img][2] - t_imgo * 2; //图像宽度
                                    int img_h = CT_ResourceArgList[t_img][3] - t_imgo * 2; //图像高度
                                    CTCanvas_ImagePart(t_x,t_y,t_img,t_imgo,t_imgo,img_w,img_h); //绘制背景图像
                                }
                                else
                                {
                                    CTCanvas_Rectangle(t_x,t_y,t_w,t_h,t_cbg,1); //绘制文本框背景
                                    CTCanvas_Rectangle(t_x,t_y,t_w,2,t_cbu,1); //绘制文本框上边框
                                    CTCanvas_Rectangle(t_x,t_y+t_h-2,t_w,2,t_cbd,1); //绘制文本框下边框
                                    CTCanvas_Rectangle(t_x,t_y,2,t_h-2,t_cbl,1); //绘制文本框左边框
                                    CTCanvas_Rectangle(t_x+t_w-2,t_y,2,t_h-2,t_cbr,1); //绘制文本框右边框
                                }
                                if(t_pass != 1) //如果文本框不是密码文本框
                                {
                                    CTCanvas_ImgString(t_x+4,t_ty,t_ct,t_text,t_font_img,t_font_charnum,t_font_chargap,t_font_charlist,t_font_glyphlist); //绘制文本框文本
                                    if(i2 == CT_Textbox_Index) //如果选择的文本框是当前文本框
                                    {
                                        int val_width = CT_ImgStringMeasure(t_font_charnum,t_font_chargap,t_font_charlist,t_font_glyphlist,t_text); //文本框字符串宽度
                                        if(t_readonly == 0)
                                        {
                                            CTCanvas_Line(t_x+4+val_width+1,t_y+3,t_x+4+val_width+1,t_y+t_h-3,2,color(0,0,0,255)); //绘制文本框输入线
                                        }
                                    }
                                }
                                else //如果文本框是密码文本框
                                {
                                    int t_val_l = strlen(t_text); //文本框字符串长度
                                    int pass_x = t_x + 4; //密码字符串位置x
                                    int passc_w = CT_ImgStringMeasure(t_font_charnum,t_font_chargap,t_font_charlist,t_font_glyphlist,"+"); //密码符宽度
                                    int passi;
                                    for(passi=0;passi<t_val_l;passi++) //遍历文本框字符串
                                    {
                                        CTCanvas_ImgString(pass_x,t_ty,t_ct,"+",t_font_img,t_font_charnum,t_font_chargap,t_font_charlist,t_font_glyphlist); //绘制文本框文本      //绘制文本框密码串
                                        pass_x += passc_w + 1; //密码字符串+
                                    }
                                    if(i2 == CT_Textbox_Index) //如果选择的文本框是当前文本框
                                    {
                                        int t_val_l = strlen(t_text); //文本框字符串长度
                                        int val_width = CT_ImgStringMeasure(t_font_charnum,t_font_chargap,t_font_charlist,t_font_glyphlist,"+") * t_val_l + t_val_l; //文本框密码串宽度
                                        if(t_readonly == 0)
                                        {
                                            CTCanvas_Line(t_x+4+val_width+1,t_y+3,t_x+4+val_width+1,t_y+t_h-3,2,color(0,0,0,255)); //绘制文本框输入线
                                        }
                                    }
                                }
                            }
                            if(ui_type == 7) //如果当前UI对象是图像元件
                            {
                                int ie_x = win_x + CT_ContainerUI[i2][4]; //图像元件位置x
                                int ie_y = win_y + CT_ContainerUI[i2][5]; //图像元件位置y
                                int ie_i = CT_ContainerUI[i2][6]; //图像元件图像索引
                                int ie_si = CT_ContainerUI[i2][7]; //图像元件子图像索引
                                int ie_si_o = CT_ContainerUI[i2][11]; //子图像偏移
                                int ie_si_w = CT_ContainerUI[i2][9]; //子图像宽度
                                int ie_si_h = CT_ContainerUI[i2][10]; //子图像高度
                                if(ie_i != -1) //如果图像元件图像不为空
                                {
                                    int si_x = ie_si * ie_si_w; //图块位置x
                                    int si_y = 0; //图块位置y
                                    if(ie_si_o > 0) //子如果图像偏移大于0
                                    {
                                        si_x = ie_si_o + (ie_si_w * ie_si) + (ie_si * ie_si_o * 2); //图块位置x
                                        si_y = ie_si_o; //图块位置y
                                    }
                                    CTCanvas_ImagePart(ie_x,ie_y,ie_i,si_x,si_y,ie_si_w,ie_si_h); //绘制图像元件
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 按钮重置
//--------------------------------------------------------------------
void CT_ButtonReset()
{
    int i;
    int ui_type = 0;
    for(i=0;i<CT_ContainerUILength;i++) //遍历UI容器
    {
        if(CT_ContainerUI[i][0] != NULL) //如果当前UI不为空
        {
            ui_type = CT_ContainerUI[i][3]; //获取当前UI类型
            if(ui_type == 1) //如果当前UI是按钮
            {
                CT_ContainerUI[i][17] = 0; //设置当前按钮的点击状态
                CT_ContainerUI[i][19] = 0; //设置当前按钮的按下状态
                CT_ContainerUI[i][20] = 0; //设置当前按钮的松开状态
            }
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 窗口容器预先分配
//--------------------------------------------------------------------
void CT_ContainerPreallocated_Window(int cap)
{
    int i;
    if(CT_ContainerCapacity_Window == 0) //如果窗口容器容量为0
    {
        CT_ContainerWindow = malloc(sizeof(int*) * cap); //窗口容器分配指针的内存
        for(i=0;i<cap;i++) //遍历容器
        {
            CT_ContainerWindow[i] = malloc(sizeof(int) * 15); //窗口容器分配内存
            memset(CT_ContainerWindow[i],0x00,sizeof(int) * 15); //窗口容器填0
        }
        CT_ContainerCapacity_Window = cap; //设置窗口容器容量
    }
    if(CT_ContainerCapacity_Window > 0 && cap > CT_ContainerCapacity_Window) //如果容量大于窗口容器容量
    {
        CT_ContainerWindow = realloc(CT_ContainerWindow,sizeof(int*) * cap); //窗口容器重新分配指针的内存
        for(i=CT_ContainerCapacity_Window;i<cap;i++) //遍历容器
        {
            CT_ContainerWindow[i] = malloc(sizeof(int) * 15); //窗口容器分配内存
            memset(CT_ContainerWindow[i],0x00,sizeof(int) * 15); //窗口容器填0
        }
        CT_ContainerCapacity_Window = cap; //设置窗口容器容量
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// UI容器预先分配
//--------------------------------------------------------------------
void CT_ContainerPreallocated_UI(int cap)
{
    int i;
    if(CT_ContainerCapacity_UI == 0) //如果UI容器容量为0
    {
        CT_ContainerUI = malloc(sizeof(int*) * cap); //UI容器分配指针的内存
        CT_ContainerUI_Value = malloc(sizeof(char*) * cap); //UI值容器分配指针的内存
        CT_ContainerUI_Name = malloc(sizeof(char*) * cap); //UI名称容器分配指针的内存
        CT_ContainerUI_FontCharList_P = malloc(sizeof(char*) * cap); //UI字体字符列表容器分配指针的内存
        CT_ContainerUI_FontGlythList_P = malloc(sizeof(int*) * cap); //UI字体字形列表容器分配指针的内存
        for(i=0;i<cap;i++) //遍历容器
        {
            CT_ContainerUI[i] = malloc(sizeof(int) * 25); //UI容器分配内存
            memset(CT_ContainerUI[i],0x00,sizeof(int) * 25); //UI容器填0
            CT_ContainerUI_Value[i] = malloc(sizeof(char) * 2049); //UI值容器分配内存
            memset(CT_ContainerUI_Value[i],0x00,2049); //UI值容器填0
            CT_ContainerUI_Name[i] = malloc(sizeof(char) * 129); //UI名称容器分配内存
            memset(CT_ContainerUI_Name[i],0x00,129); //UI值名称容器填0
            CT_ContainerUI_FontCharList_P[i] = 0; //UI字体字符列表指针
            CT_ContainerUI_FontGlythList_P[i] = 0; //UI字体字形列表指针
        }
        CT_ContainerCapacity_UI = cap; //设置UI容器容量
    }
    if(CT_ContainerCapacity_UI > 0 && cap > CT_ContainerCapacity_UI) //如果容量大于UI容器容量
    {
        CT_ContainerUI = realloc(CT_ContainerUI,sizeof(int*) * cap); //UI容器重新分配内存
        CT_ContainerUI_Value = realloc(CT_ContainerUI_Value,sizeof(char*) * cap); //UI值容器重新分配内存
        CT_ContainerUI_Name = realloc(CT_ContainerUI_Name,sizeof(char*) * cap); //UI名称容器重新分配内存
        CT_ContainerUI_FontCharList_P = realloc(CT_ContainerUI_FontCharList_P,sizeof(char*) * cap); //UI字体字符列表容器重新分配指针的指针的内存
        CT_ContainerUI_FontGlythList_P = realloc(CT_ContainerUI_FontGlythList_P,sizeof(int*) * cap); //UI字体字形列表容器重新分配指针的指针的内存
        for(i=CT_ContainerCapacity_UI;i<cap;i++) //遍历容器
        {
            CT_ContainerUI[i] = malloc(sizeof(int) * 25); //UI容器分配内存
            memset(CT_ContainerUI[i],0x00,sizeof(int) * 25); //UI容器填0
            CT_ContainerUI_Value[i] = malloc(sizeof(char) * 2049); //UI值容器分配内存
            memset(CT_ContainerUI_Value[i],0x00,2049); //UI值容器填0
            CT_ContainerUI_Name[i] = malloc(sizeof(char) * 129); //UI名称容器分配内存
            memset(CT_ContainerUI_Name[i],0x00,129); //UI值名称容器填0
            CT_ContainerUI_FontCharList_P[i] = 0; //UI字体字符列表指针
            CT_ContainerUI_FontGlythList_P[i] = 0; //UI字体字形列表指针
        }
        CT_ContainerCapacity_UI = cap; //设置窗口容器容量
    }
}
//--------------------------------------------------------------------