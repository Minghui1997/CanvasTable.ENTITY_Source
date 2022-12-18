#define _CRT_SECURE_NO_WARNINGS

#include "CanvasTableSYS.h"
#include <windows.h>
#include <windowsx.h>

extern int CT_WindowX;
extern int CT_WindowY;
extern HWND CT_MainWindow;
extern int CT_DialogShow;
extern int CT_WindowW;
extern int CT_WindowH;
extern int CT_Dialog_Index;
extern int CT_ContainerUI[1000][25];
extern char CT_ContainerUI_Value[1000][2048];

HWND CT_DialogWindow; //对话框窗口句柄
HWND CT_DialogTextbox; //对话框文本框句柄
HWND CT_DialogButton; //对话框按钮句柄
HFONT CT_DialogFont; //对话框字体句柄
LRESULT CALLBACK CT_DialogProc(HWND CT_Dialog,UINT uMsg,WPARAM wParam,LPARAM lParam);
int CT_DialogW = 0; //对话框宽度
int CT_DialogH = 0; //对话框高度
int CT_DialogX = 0; //对话框位置x
int CT_DialogY = 0; //对话框位置y
int CT_BoxMax = 0; //文本框最大字符数
int CT_BoxNumber = 0; //数字文本框
char *CT_DialogName = NULL; //对话框名称
//--------------------------------------------------------------------
// 创建对话框
//--------------------------------------------------------------------
void CT_DialogCreate(char *val,char *name,int max,int pass,int numb)
{
    CT_DialogW = CT_WindowW * 0.6; //对话框宽度
    CT_DialogH = CT_WindowH * 0.4 + 30; //对话框高度
    CT_DialogX = CT_WindowX + (CT_WindowW - CT_DialogW) / 2; //对话框位置x
    CT_DialogY = CT_WindowY + (CT_WindowH - CT_DialogH) / 2; //对话框位置y
    CT_BoxMax = max; //文本框最大字符数
    CT_BoxNumber = numb; //数字文本框
    CT_DialogName = name; //对话框名称
    if(CT_DialogShow == 0) //如果对话框没有显示
    {
        int textbox_x = CT_DialogW * 0.1; //文本框位置x
        int textbox_y = CT_DialogH * 0.25; //文本框位置y
        int textbox_w = CT_DialogW * 0.8; //文本框宽度
        int textbox_h = CT_DialogH * 0.15; //文本框高度
        int button_x = CT_DialogW * 0.3; //按钮位置x
        int button_y = CT_DialogH * 0.45; //按钮位置y
        int button_w = CT_DialogW * 0.4; //按钮宽度
        int button_h = CT_DialogH * 0.2; //按钮高度
        int textbox_style = WS_VISIBLE|WS_CHILD|ES_LEFT|ES_AUTOHSCROLL; //文本框样式
        int dialog_font_size = CT_DialogH * 0.15; //对话框字体大小
        WNDCLASS WC_Dialog; //对话框窗口类
        MSG msg; //消息
        WC_Dialog.cbClsExtra = 0;
        WC_Dialog.cbWndExtra = 0;
        WC_Dialog.hbrBackground = CreateSolidBrush(0xd3e2e4); //窗口背景颜色
        WC_Dialog.hCursor = LoadCursor(NULL,IDC_ARROW);
        WC_Dialog.hIcon = LoadIcon(NULL,IDI_APPLICATION);
        WC_Dialog.lpfnWndProc = CT_DialogProc;
        WC_Dialog.lpszClassName = "DialogWindow";
        WC_Dialog.lpszMenuName = NULL;
        WC_Dialog.style = CS_HREDRAW | CS_VREDRAW;
        RegisterClass(&WC_Dialog);
        CT_DialogWindow = CreateWindow("DialogWindow","",NULL,CT_DialogX,CT_DialogY,CT_DialogW,CT_DialogH,CT_MainWindow,NULL,NULL,NULL); //创建窗口 
        ShowWindow(CT_DialogWindow,SW_SHOW); //显示窗口
        UpdateWindow(CT_DialogWindow); //刷新窗口
        CT_DialogFont = CreateFont(dialog_font_size,0,0,0,400,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_CHARACTER_PRECIS,CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY,DEFAULT_PITCH,""); //创建字体
        if(pass == 1) //如果是密码文本框
        {
            textbox_style += ES_PASSWORD; //文本框增加样式
        }
        if(numb == 1) //如果是数字文本框
        {
            textbox_style += ES_NUMBER; //文本框增加样式
        }
        CT_DialogTextbox = CreateWindow("EDIT","",textbox_style,textbox_x,textbox_y,textbox_w,textbox_h,CT_DialogWindow,NULL,NULL,NULL); //创建文本框
        SendMessage(CT_DialogTextbox,EM_SETLIMITTEXT,max,TRUE); //设置
        SendMessage(CT_DialogTextbox,WM_SETFONT,CT_DialogFont,TRUE); //设置文本框字体
        Edit_SetText(CT_DialogTextbox,val); //设置文本框文本
        CT_DialogButton = CreateWindow("BUTTON","√",WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_DEFPUSHBUTTON,button_x,button_y,button_w,button_h,CT_DialogWindow,NULL,NULL,NULL); //创建按钮
        SendMessage(CT_DialogButton,WM_SETFONT,CT_DialogFont,TRUE); //设置按钮字体
        CT_DialogShow = 1; //对话框显示
        while(GetMessage(&msg,NULL,0,0)) //处理消息列队
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            if(CT_DialogShow == 0) //如果对话框不显示
            {
                break;
            }
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 销毁对话框
//--------------------------------------------------------------------
void CT_DialogDestroy()
{
    if(CT_DialogShow == 1) //如果对话框显示
    {
        DeleteObject(CT_DialogFont); //删除字体
        DestroyWindow(CT_DialogWindow); //销毁对话框窗口
        CT_DialogShow = 0; //对话框不显示
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 对话框返回
//--------------------------------------------------------------------
void CT_DialogReturn()
{
    if(CT_ContainerUI[CT_Dialog_Index][0] == 1) //如果对话框对应文本框的窗口不为空
    {
        Edit_GetText(CT_DialogTextbox,CT_ContainerUI_Value[CT_Dialog_Index],CT_BoxMax); //将对话框输入的内容返回给文本框
        CT_DialogDestroy(); //销毁对话框
        CT_InterfaceRepaint(); //请求重绘
    }
}
//--------------------------------------------------------------------

LRESULT CALLBACK CT_DialogProc(HWND CT_DialogWindow,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
    if(uMsg == WM_CREATE)
    {
    }
    if(uMsg == WM_CLOSE)
    {
        
    }
    if(uMsg == WM_COMMAND)
    {
        if(wParam == BN_CLICKED)
        {
            if(lParam == CT_DialogButton)
            {
                CT_DialogReturn(); //对话框返回
            }
        }
    }
    if(uMsg == WM_PAINT)
    {
        int title_x = CT_DialogW * 0.1; //标题位置x
        int title_y = CT_DialogH * 0.08; //标题位置y
        HDC title = GetDC(CT_DialogWindow); //获取对话框dc
        SetBkColor(title,0xd3e2e4); //设置标题背景颜色
        SelectObject(title,CT_DialogFont); //设置标题字体
        TextOut(title,title_x,title_y,CT_DialogName,strlen(CT_DialogName)); //显示标题
        ReleaseDC(CT_DialogWindow,title); //释放对话框dc
    }
    return DefWindowProc(CT_DialogWindow,uMsg,wParam,lParam);
}