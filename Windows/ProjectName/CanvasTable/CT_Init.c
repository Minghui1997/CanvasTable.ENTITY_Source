#define _CRT_SECURE_NO_WARNINGS

#include "CanvasTableSYS.h"
#include <windows.h>
#include <shlwapi.h>
#include <mmsystem.h>
#include <gl/gl.h>
#include "../DirectSound8/dsound.h"

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"imm32.lib")
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"DirectSound8/dsound.lib")
#pragma comment(lib,"Shlwapi.lib")

extern char **CT_ResourceList;

LRESULT CALLBACK WindowProc(HWND CT_MainWindow,UINT uMsg,WPARAM wParam,LPARAM lParam);
HWND CT_MainWindow; //主窗口句柄
HDC CT_DCMW; //主窗口dc
HDC CT_DCBC; //缓冲画布dc
LPDIRECTSOUND8 CT_DirectSound; //DirectSound对象

PROC wglSwapIntervalEXT; //OpenGL扩展函数

void (*CT_Main)(); //Main指针函数
void (*CT_Loop)(); //Loop指针函数
int CT_CanvasW = 0; //画布宽度
int CT_CanvasH = 0; //画布高度
int CT_WindowX = 0; //窗口位置x
int CT_WindowY = 0; //窗口位置y
int CT_WindowW = 0; //窗口宽度
int CT_WindowH = 0; //窗口高度
float CT_CanvasCRX = 1; //画布相对窗口宽度倍数
float CT_CanvasCRY = 1; //画布相对窗口高度倍数
int CT_DEBUG_MODE = 1; //调试模式
int CT_SPLASH_SHOW = 0; //splash图片显示
int CT_LOADBAR_SHOW = 0; //加载条显示
int CT_SYSTEM = 1; //系统
int CT_DEVICE = 0; //设备
double CT_Frequency = 0; //性能频率
//--------------------------------------------------------------------
// 初始化函数
//--------------------------------------------------------------------
void CT_init(int iw,int ih,char **rl,int rl_s,void *CT_main,void *CT_loop,int dm,int sph,int lbh,int ww,int wh,int arg6,int arg7,int arg8,int arg9,int arg10,int arg11,int arg12,int arg13,int arg14,int arg15,int arg16)
{
    int i;
    int rl_l = rl_s / sizeof(char*); //资源列表长度
    CT_CanvasW = iw; //画布宽度
    CT_CanvasH = ih; //画布高度
    CT_DEBUG_MODE = dm; //设置调试模式
    CT_SPLASH_SHOW = sph; //SPLASH显示
    CT_LOADBAR_SHOW = lbh; //加载条显示
    CT_WindowW = ww; //窗口宽度
    CT_WindowH = wh; //窗口高度
    CT_Main = CT_main; //Main函数
    CT_Loop = CT_loop; //Loop函数
    CT_Frequency = CT_QueryFrequency(); //获取性能频率
    cprint("CanvasTable.ENTITY Version : 0.11.3");
    cprint("CanvasTable.ENTITY Build : 2023 / ?? / ??");
    cprint("----------Init----------");
    timeBeginPeriod(1); //设置Sleep最低分辨率
    ImmDisableIME(0); //禁用输入法
    CT_PreferSet(); //设置偏好
    CT_WindowCreate(); //创建主画布
    CT_GLContextCreate(); //创建缓冲画布
    CT_AudioInit(); //初始化音频
    CT_DefaultFontSet(); //设置默认字体
    if(PathFileExists("RECTAB") == 0) //如果应用程序所在目录中没有RECTAB文件夹
    {
        CreateDirectory("RECTAB",NULL); //创建RECTAB文件夹
    }
    cprint("------------------------");
    ShowWindow(CT_MainWindow,SW_SHOW); //显示窗口
    UpdateWindow(CT_MainWindow); //刷新窗口
    CT_ResourceList = malloc(sizeof(char*)); //为资源列表分配内存
    for(i=0;i<rl_l;i++) //遍历资源列表(参数)
    {
        int path_l = strlen(rl[i]); //获取当前资源路径的长度
        CT_ResourceList = realloc(CT_ResourceList,sizeof(char*) * ( i + 1)); //为资源列表重新分配内存
        CT_ResourceList[i] = malloc(path_l+1); //在资源列表中为当前资源路径分配内存
        CT_ResourceList[i][path_l] = 0x00;
        strcpy(CT_ResourceList[i],rl[i]); //将当前资源路径(参数)复制到资源列表中
    }
    CT_ResourceLoadStart(rl_l); //开始加载资源
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
        char infor[1024] = "";
        char arg0[8] = "";
        char arg1[8] = "";
        char arg2[8] = "";
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
        RegisterClass(&WC_MainWindow);
        CT_MainWindow = CreateWindow("MainWindow","",WS_SYSMENU|WS_MINIMIZEBOX,0,0,200,200,NULL,NULL,NULL,NULL); //创建窗口
        GetClientRect(CT_MainWindow,&Wrect); //获取窗口客户区信息
        winborder_w = 200 - (Wrect.right-Wrect.left); //窗口边框宽度
        winborder_h = 200 - (Wrect.bottom-Wrect.top); //窗口边框高度
        screen_w = GetSystemMetrics(SM_CXSCREEN); //获取屏幕宽度
        screen_h = GetSystemMetrics(SM_CYSCREEN); //获取屏幕高度
        CT_WindowX = (screen_w - (CT_WindowW + winborder_w)) / 2; //窗口位置x
        CT_WindowY = (screen_h - (CT_WindowH + winborder_h)) / 2; //窗口位置y
        CT_CanvasCRX = (float)CT_CanvasW / (float)CT_WindowW; //画布相对窗口宽度的倍数
        CT_CanvasCRY = (float)CT_CanvasH / (float)CT_WindowH; //画布相对窗口高度的倍数
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
// 创建OpenGL环境
//--------------------------------------------------------------------
void CT_GLContextCreate()
{
    int pixelformat;
    PIXELFORMATDESCRIPTOR ppfd; //像素格式
    ppfd.dwFlags = PFD_DOUBLEBUFFER | PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL; //像素格式旗
    pixelformat = ChoosePixelFormat(CT_DCMW,&ppfd); //获取主窗口的像素格式
    SetPixelFormat(CT_DCMW,pixelformat,&ppfd); //设置像素格式
    CT_DCBC = wglCreateContext(CT_DCMW); //创建缓冲画布环境
    wglMakeCurrent(CT_DCMW,CT_DCBC); //选择缓冲画布环境
    glOrtho(0,CT_CanvasW,CT_CanvasH,0,-1,1); //设置opengl正交
    glEnable(GL_TEXTURE_2D); //启用2d纹理
    glEnable(GL_BLEND); //启用混合
    glEnable(GL_MAP1_VERTEX_3); //启用一维顶点
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); //设置alpha混合模式
    wglSwapIntervalEXT = wglGetProcAddress("wglSwapIntervalEXT"); //加载OpenGL扩展函数
    wglSwapIntervalEXT(1); //启用垂直同步
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
    null_buffer.dwFlags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY | DSBCAPS_GLOBALFOCUS;
    null_buffer.dwBufferBytes = 16;
    null_buffer.lpwfxFormat = &nullwav_format;

    CT_DirectSound->lpVtbl->CreateSoundBuffer(CT_DirectSound,&null_buffer,&nullwav_lpd,NULL); //创建空音频Buffer
    nullwav_lpd->lpVtbl->Lock(nullwav_lpd,0,0,&lpvWrite,&dwLength,NULL,NULL,DSBLOCK_ENTIREBUFFER); //空音频Buffer
    memcpy(lpvWrite,nullwav_data,dwLength); //将空音频PCM数据复制到lpvWrite
    nullwav_lpd->lpVtbl->Unlock(nullwav_lpd,lpvWrite,dwLength,NULL,NULL); //解锁空音频Buffer
    nullwav_lpd->lpVtbl->Play(nullwav_lpd,0,0,0); //播放空音频
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 查询性能频率
//--------------------------------------------------------------------
double CT_QueryFrequency()
{
    LARGE_INTEGER Frequency; //性能频率
    double fre = 0; //性能频率
    QueryPerformanceFrequency(&Frequency); //查询性能频率
    fre = Frequency.QuadPart; //获取性能频率
    return fre;
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
        timeEndPeriod(1); //取消Sleep最低分辨率
        PostQuitMessage(0); //退出
    }
    if(uMsg == WM_QUIT) //如果消息是退出
    {
    }
    return DefWindowProc(CT_MainWindow,uMsg,wParam,lParam);
}
//--------------------------------------------------------------------