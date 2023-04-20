#define _CRT_SECURE_NO_WARNINGS

#include "CanvasTableSYS.h"
#include <windows.h>
#include <mmsystem.h>
#include <gl/gl.h>
#include "../DirectSound8/dsound.h"
#include "GLEXT.h"

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"imm32.lib")
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"DirectSound8/dsound.lib")

extern char **CT_ResourceList;
extern unsigned int CT_FrameBufferID;
extern unsigned int CT_FrameBufferSurfaceID;
extern unsigned int CT_RenderBufferID;

LRESULT CALLBACK WindowProc(HWND CT_MainWindow,UINT uMsg,WPARAM wParam,LPARAM lParam);
HWND CT_MainWindow; //主窗口句柄
HDC CT_DCMW; //主窗口dc
HGLRC CT_GLDCMW; //缓冲画布dc
LPDIRECTSOUND8 CT_DirectSound; //DirectSound对象

void (*CT_Main)() = NULL; //Main指针函数
void (*CT_Loop)() = NULL; //Loop指针函数
int CT_CanvasW = 0; //画布宽度
int CT_CanvasH = 0; //画布高度
int CT_WindowX = 0; //窗口位置x
int CT_WindowY = 0; //窗口位置y
int CT_WindowW = 0; //窗口宽度
int CT_WindowH = 0; //窗口高度
float CT_CanvasWWM = 1; //画布相对窗口宽度的倍数
float CT_CanvasWHM = 1; //画布相对窗口高度的倍数
int CT_DEBUG_MODE = 1; //调试模式
int CT_SPLASH_SHOW = 0; //splash图片显示
int CT_LOADBAR_SHOW = 0; //加载条显示
int CT_SYSTEM = 1; //操作系统
int CT_DEVICE = 0; //设备类型
//--------------------------------------------------------------------
// 初始化函数
//--------------------------------------------------------------------
void CT_init(int iw,int ih,char **rl,size_t rl_s,void *CT_main,void *CT_loop,int dm,int sph,int lbh,int ww,int wh,int arg6,int arg7,int arg8,int arg9,int arg10,int arg11,int arg12,int arg13,int arg14,int arg15,int arg16)
{
    CT_CanvasW = iw; //画布宽度
    CT_CanvasH = ih; //画布高度
    CT_DEBUG_MODE = dm; //调试模式
    CT_SPLASH_SHOW = sph; //Splash显示
    CT_LOADBAR_SHOW = lbh; //加载条显示
    CT_WindowW = ww; //窗口宽度
    CT_WindowH = wh; //窗口高度
    CT_Main = CT_main; //Main函数
    CT_Loop = CT_loop; //Loop函数
    cprint("CanvasTable.ENTITY Version : 0.20.0");
    cprint("CanvasTable.ENTITY Build : 2023 / ?? / ??");
    cprint("----------Init----------");
    timeBeginPeriod(1); //设置Sleep精度
    ImmDisableIME(0); //禁用输入法
    CT_PreferSet(); //设置偏好
    CT_WindowCreate(); //创建主窗口
    CT_GraphicsInit(); //创建OpenGL环境
    CT_OffscreenCanvasCreate(); //创建离屏画布
    CT_AudioInit(); //初始化音频
    CT_DefaultFontSet(); //设置默认字体
    cprint("------------------------");
    CT_WindowShow(); //显示主窗口
    CT_ResourceLoadStart(rl,rl_s); //开始加载资源
    CT_InputListener(); //监听输入
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 设置偏好
//--------------------------------------------------------------------
void CT_PreferSet()
{
    if(CT_SYSTEM == 1) //如果系统是windows
    {
        cprint("OS : Windows");
    }
    if(CT_SYSTEM == 2) //如果系统是mac os
    {
        cprint("OS : Mac os");
    }
    if(CT_SYSTEM == 3) //如果系统是unix
    {
        cprint("OS : UNIX");
    }
    if(CT_SYSTEM == 4) //如果系统是linux
    {
        cprint("OS : Linux");
    }
    if(CT_SYSTEM == 5) //如果系统是android
    {
        cprint("OS : Android");
    }
    if(CT_SYSTEM == 6) //如果系统是ios
    {
        cprint("OS : iOS");
    }
    if(CT_DEVICE == 0) //如果设备是桌面
    {
        cprint("Device : Table");
    }
    if(CT_DEVICE == 1) //如果设备是移动平台
    {
        cprint("Device : Phone");
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 创建窗口
//--------------------------------------------------------------------
void CT_WindowCreate()
{
    if(CT_CanvasW > 0 && CT_CanvasH > 0) //如果画布宽度和高度都大于0
    {
        int win_x = 0; //窗口位置x
        int win_y = 0; //窗口位置y
        int winborder_w = 0; //窗口边框宽度
        int winborder_h = 0; //窗口边框高度
        int screen_w = 0; //屏幕宽度
        int screen_h = 0; //屏幕高度
        char infor[128] = "";
        char arg0[11] = "";
        char arg1[11] = "";
        char arg2[11] = "";
        RECT Wrect; //窗口矩形
        WNDCLASS WC_MainWindow; //窗口类
        WC_MainWindow.cbClsExtra = 0;
        WC_MainWindow.cbWndExtra = 0;
        WC_MainWindow.hbrBackground = CreateSolidBrush(0x000000); //窗口背景颜色
        WC_MainWindow.hCursor = LoadCursor(NULL,IDC_ARROW);
        WC_MainWindow.hIcon = LoadIcon(NULL,IDI_APPLICATION);
        WC_MainWindow.lpfnWndProc = WindowProc;
        WC_MainWindow.lpszClassName ="MainWindow";
        WC_MainWindow.lpszMenuName = NULL;
        WC_MainWindow.style = CS_HREDRAW | CS_VREDRAW;
        RegisterClass(&WC_MainWindow); //注册窗口类
        CT_MainWindow = CreateWindow("MainWindow","",WS_SYSMENU|WS_MINIMIZEBOX,0,0,200,200,NULL,NULL,NULL,NULL); //创建窗口
        GetClientRect(CT_MainWindow,&Wrect); //获取窗口客户区信息
        winborder_w = 200 - (Wrect.right-Wrect.left); //窗口边框宽度
        winborder_h = 200 - (Wrect.bottom-Wrect.top); //窗口边框高度
        screen_w = GetSystemMetrics(SM_CXSCREEN); //获取屏幕宽度
        screen_h = GetSystemMetrics(SM_CYSCREEN); //获取屏幕高度
        CT_WindowX = (screen_w - (CT_WindowW + winborder_w)) / 2; //窗口位置x
        CT_WindowY = (screen_h - (CT_WindowH + winborder_h)) / 2; //窗口位置y
        CT_CanvasWWM = (float)CT_CanvasW / (float)CT_WindowW; //画布相对窗口宽度的倍数
        CT_CanvasWHM = (float)CT_CanvasH / (float)CT_WindowH; //画布相对窗口高度的倍数
        SetWindowPos(CT_MainWindow,HWND_BOTTOM,CT_WindowX,CT_WindowY,CT_WindowW+winborder_w,CT_WindowH+winborder_h,SWP_ASYNCWINDOWPOS); //设置窗口位置和大小
        CT_DCMW = GetDC(CT_MainWindow); //获取主窗口句柄
        //输出画布大小信息
        strcat(infor,"Created canvas : width=");
        sprintf(arg0,"%d",CT_CanvasW);
        sprintf(arg1,"%d",CT_CanvasH);
        strcat(infor,arg0);
        strcat(infor," height=");
        strcat(infor,arg1);
        cprint(infor);
    }
    else
    {
        cprint("Unable to create canvas.");
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 显示窗口
//--------------------------------------------------------------------
void CT_WindowShow()
{
    ShowWindow(CT_MainWindow,SW_SHOW); //显示窗口
    UpdateWindow(CT_MainWindow); //刷新窗口
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 初始化图形
//--------------------------------------------------------------------
void CT_GraphicsInit()
{
    int pixelformat;
    PIXELFORMATDESCRIPTOR ppfd; //像素格式
    ppfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL; //像素格式旗
    pixelformat = ChoosePixelFormat(CT_DCMW,&ppfd); //获取主窗口的像素格式
    SetPixelFormat(CT_DCMW,pixelformat,&ppfd); //设置像素格式
    CT_GLDCMW = wglCreateContext(CT_DCMW); //创建OpenGL环境
    wglMakeCurrent(CT_DCMW,CT_GLDCMW); //设置主窗口的OpenGL环境
    GL_FunctionGet(); //获取OpenGL函数
    glEnable(GL_TEXTURE_2D); //启用2d纹理
    glEnable(GL_BLEND); //启用混合
    glEnable(GL_MAP1_VERTEX_3); //启用MAP1_VERTEX
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); //设置alpha混合模式
    glEnable(GL_POINT_SMOOTH); //启用点抗锯齿
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 离屏画布创建
//--------------------------------------------------------------------
void CT_OffscreenCanvasCreate()
{
    glGenRenderbuffers(1,&CT_RenderBufferID); //生成渲染缓冲区
    glBindRenderbuffer(GL_RENDERBUFFER,CT_RenderBufferID); //绑定渲染缓冲区
    glRenderbufferStorage(GL_RENDERBUFFER,GL_RGBA8,CT_CanvasW,CT_CanvasH); //设置渲染缓冲区存储
    glGenTextures(1,&CT_FrameBufferSurfaceID); //生成帧缓冲平面
    glBindTexture(GL_TEXTURE_2D,CT_FrameBufferSurfaceID); //绑定帧缓冲平面
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //设置帧缓冲平面的属性
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); //设置帧缓冲平面的属性
    glTexImage2D(GL_TEXTURE_2D,0,4,CT_CanvasW,CT_CanvasH,0,GL_RGBA,GL_UNSIGNED_BYTE,NULL); //创建帧缓冲平面
    glGenFramebuffers(1,&CT_FrameBufferID); //生成帧缓冲区
    glBindFramebuffer(GL_FRAMEBUFFER,CT_FrameBufferID); //绑定帧缓冲区
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_RENDERBUFFER,CT_RenderBufferID); //设置帧缓冲区的渲染缓冲区
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,CT_FrameBufferID,0); //设置帧缓冲区的帧缓冲平面
    glBindFramebuffer(GL_FRAMEBUFFER,0); //取消绑定帧缓冲区
    glBindTexture(GL_TEXTURE_2D,0); //取消绑定帧缓冲平面
    glBindRenderbuffer(GL_RENDERBUFFER,0); //取消绑定渲染缓冲区
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 初始化音频
//--------------------------------------------------------------------
void CT_AudioInit()
{
    DSBUFFERDESC null_buffer; //空音频buffer esc
    WAVEFORMATEX nullwav_format; //空音频格式
    LPDIRECTSOUNDBUFFER8 nullwav_lpd; //空音频buffer
    char nullwav_data[16] = ""; //空音频数据
    char *lpvWrite; //LPV指针
    int dwLength = 0; //DW长度
    DirectSoundCreate8(NULL,&CT_DirectSound,NULL); //创建DirectSound对象
    CT_DirectSound->lpVtbl->SetCooperativeLevel(CT_DirectSound,CT_MainWindow,DSSCL_PRIORITY); //设置DirectSound合作级别

    //空音频格式
    memset(&nullwav_format, 0, sizeof(WAVEFORMATEX));
    nullwav_format.wFormatTag = WAVE_FORMAT_PCM;
    nullwav_format.nChannels = 2;
    nullwav_format.nSamplesPerSec = 32000;
    nullwav_format.nBlockAlign = 4; 
    nullwav_format.nAvgBytesPerSec = 128000;
    nullwav_format.wBitsPerSample = 16;

    //空音频buffer esc
    memset(&null_buffer, 0, sizeof(DSBUFFERDESC));
    null_buffer.dwSize = sizeof(DSBUFFERDESC);
    null_buffer.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_GLOBALFOCUS | DSBCAPS_LOCDEFER;
    null_buffer.dwBufferBytes = 16;
    null_buffer.lpwfxFormat = &nullwav_format;

    CT_DirectSound->lpVtbl->CreateSoundBuffer(CT_DirectSound,&null_buffer,&nullwav_lpd,NULL); //创建空音频Buffer
    nullwav_lpd->lpVtbl->Lock(nullwav_lpd,0,0,&lpvWrite,&dwLength,NULL,NULL,DSBLOCK_ENTIREBUFFER); //空音频Buffer
    memcpy(lpvWrite,nullwav_data,dwLength); //将空音频PCM数据复制到lpvWrite
    nullwav_lpd->lpVtbl->Unlock(nullwav_lpd,lpvWrite,dwLength,NULL,NULL); //解锁空音频Buffer
    nullwav_lpd->lpVtbl->Play(nullwav_lpd,0,0,DSBPLAY_LOCSOFTWARE); //播放空音频

}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// WindowProc
//--------------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND CT_MainWindow,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
    if(uMsg == WM_CREATE) //如果窗口打开
    {
    }
    if(uMsg == WM_CLOSE) //如果窗口关闭
    {
        DestroyWindow(CT_MainWindow); //销毁窗口
    }
    if(uMsg == WM_DESTROY) //如果窗口销毁
    {
        timeEndPeriod(1); //取消Sleep精度
        PostQuitMessage(0); //退出
    }
    if(uMsg == WM_QUIT) //如果消息是退出
    {
    }
    return DefWindowProc(CT_MainWindow,uMsg,wParam,lParam);
}
//--------------------------------------------------------------------