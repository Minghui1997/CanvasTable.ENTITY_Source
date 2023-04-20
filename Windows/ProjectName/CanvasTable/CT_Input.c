#include "CanvasTableSYS.h"
#include <windows.h>

extern HWND CT_MainWindow;
extern float CT_CanvasWWM;
extern float CT_CanvasWHM;

//--------------------------------------------------------------------
// 按键容器
//--------------------------------------------------------------------
int CT_ContainerKeyState[256] = {0}; //按键状态容器
int CT_ContainerKeyDown[256] = {0}; //按键按下容器
int CT_ContainerKeyUp[256] = {0}; //按键松开容器
int CT_ContainerMKeyState[3] = {0}; //鼠标按键状态容器
int CT_ContainerMKeyDown[3] = {0}; //鼠标按键按下状态容器
int CT_ContainerMKeyUp[3] = {0}; //鼠标按键松开容器
int CT_ContainerMWheel[2] = {0}; //鼠标滚轮容器
int CT_MousePosX = 0; //鼠标位置x
int CT_MousePosY = 0; //鼠标位置y
char CT_KeyChar = 0x00; //按键字符
int CT_ContainerTouchKey[32][7] = {0}; //触摸键容器
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 输入监听
//--------------------------------------------------------------------
void CT_InputListener()
{
    MSG msg; //消息
    while(GetMessage(&msg,NULL,0,0)) //处理消息列队
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        if(msg.message == WM_KEYDOWN) //如果按键按下
        {
            CT_ContainerKeyState[msg.wParam] = 1; //将按键状态容器中按下的键标记为1
            if(CT_ContainerKeyDown[msg.wParam] == 0) //如果按键中断容器中按下的键不为1
            {
                CT_ContainerKeyDown[msg.wParam] = 1; //将按键按下容器中按下的键标记为1
            }
            if(msg.wParam == 8) //如果当前按下的键是退格
            {
                CT_KeyChar = '\b'; //按键字符为退格
            }
        }
        if(msg.message == WM_KEYUP) //如果按键松开
        {
            CT_ContainerKeyState[msg.wParam] = 0; //将始终按下的键的状态标记为0
            CT_ContainerKeyDown[msg.wParam] = 0; //将按键中断容器中松开的键标记为0
            CT_ContainerKeyUp[msg.wParam] = 1; //将按键松开容器中松开的键标记为1
        }
        if(msg.message == WM_CHAR) //输入的字符
        {
            CT_KeyChar = msg.wParam; //按键字符
        }
        if(msg.message == WM_LBUTTONDOWN) //如果鼠标左键按下
        {
            CT_ContainerMKeyState[0] = 1; //将鼠标按键状态容器中按下的键标记为1
            CT_ContainerMKeyDown[0] = 1; //将鼠标按键按下容器中按下的键标记为1
        }
        if(msg.message == WM_MBUTTONDOWN) //如果鼠标中键按下
        {
            CT_ContainerMKeyState[1] = 1; //将鼠标按键状态容器中按下的键标记为1
            CT_ContainerMKeyDown[1] = 1; //将鼠标按键按下容器中按下的键标记为1
        }
        if(msg.message == WM_RBUTTONDOWN) //如果鼠标右键按下
        {
            CT_ContainerMKeyState[2] = 1; //将鼠标按键状态容器中按下的键标记为1
            CT_ContainerMKeyDown[2] = 1; //将鼠标按键按下容器中按下的键标记为1
        }
        if(msg.message == WM_LBUTTONUP) //如果鼠标左键松开
        {
            CT_ContainerMKeyState[0] = 0; //将鼠标按键状态容器中按下的键标记为0
            CT_ContainerMKeyUp[0] = 1; //将鼠标按键松开容器中松开的键标记为1
        }
        if(msg.message == WM_MBUTTONUP) //如果鼠标中键松开
        {
            CT_ContainerMKeyState[1] = 0; //将鼠标按键状态容器中按下的键标记为0
            CT_ContainerMKeyUp[1] = 1; //将鼠标按键松开容器中松开的键标记为1
        }
        if(msg.message == WM_RBUTTONUP) //如果鼠标右键松开
        {
            CT_ContainerMKeyState[2] = 0; //将鼠标按键状态容器中按下的键标记为0
            CT_ContainerMKeyUp[2] = 1; //将鼠标按键松开容器中松开的键标记为1
        }
        if(msg.message == WM_MOUSEMOVE) //如果鼠标移动
        {
            POINT CT_MousePosPoint; //鼠标位置对象
            GetCursorPos(&CT_MousePosPoint); //获取鼠标位置
            ScreenToClient(CT_MainWindow,&CT_MousePosPoint); //鼠标位置坐标转换
            CT_MousePosX = CT_MousePosPoint.x * CT_CanvasWWM; //鼠标位置x
            CT_MousePosY = CT_MousePosPoint.y * CT_CanvasWHM; //鼠标位置y
        }
        if(msg.message == WM_MOUSEWHEEL) //如果鼠标滚轮滚动
        {
            if((int)msg.wParam > 0) //鼠标滚轮向上滚动
            {
                CT_ContainerMWheel[0] = 1; //将鼠标滚轮容器中上滚动标记为1
            }
            if((int)msg.wParam < 0)
            {
                CT_ContainerMWheel[1] = 1; //将鼠标滚轮容器中下滚动标记为1
            }
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 触摸键图片渲染
//--------------------------------------------------------------------
void CT_TouchKeyRender()
{
    int i;
    for(i=0;i<32;i++) //遍历触摸键容器
    {
        if(CT_ContainerTouchKey[i][0] != 0) //如果当前触摸键不为空
        {
            int tx = CT_ContainerTouchKey[i][1]; //当前触摸键位置x
            int ty = CT_ContainerTouchKey[i][2]; //当前触摸键位置y
            int ti = CT_ContainerTouchKey[i][6]; //当前触摸键图像
            if(ti != -1) //如果触摸键有图像
            {
                CT_OffCanvas_Image(tx,ty,ti,255); //绘制触摸键图像
            }
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 按键结束
//--------------------------------------------------------------------
void CT_KeyboardEnd()
{
    int i;
    int i2;
    int i3;
    int i4;
    for(i=0;i<256;i++) //遍历按键按下容器
    {
        if(CT_ContainerKeyDown[i] == 1) //如果当前按键被按下
        {
            CT_ContainerKeyDown[i] = 2; //将当前按下的键标记为2
        }
    }
    for(i2=0;i2<256;i2++) //遍历按键松开容器
    {
        CT_ContainerKeyUp[i2] = 0; //重置按键松开容器
    }
    for(i3=0;i3<3;i3++) //遍历鼠标按键按下容器
    {
        if(CT_ContainerMKeyDown[i3] == 1) //如果当前按键被按下
        {
            CT_ContainerMKeyDown[i3] = 2; //将当前按下的键标记为2
        }
    }
    for(i4=0;i4<3;i4++) //遍历鼠标按键松开容器
    {
        CT_ContainerMKeyUp[i4] = 0; //重置鼠标按键松开容器
    }
    CT_ContainerMWheel[0] = 0; //重置鼠标滚轮容器
    CT_ContainerMWheel[1] = 0; //重置鼠标滚轮容器
    CT_KeyChar = 0x00; //清除按键字符
}
//--------------------------------------------------------------------