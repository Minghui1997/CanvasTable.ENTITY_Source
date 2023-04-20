#define _CRT_SECURE_NO_WARNINGS

#include "CanvasTableSYS.h"
#include <windows.h>
#include <math.h>

extern HDC CT_DCMW;
extern HGLRC CT_GLDCMW;
extern int CT_DrawLayerStatusList[32];
extern int CT_WindowLayerStatusList[32];
extern int CT_SpriteLayerStatusList[32];
extern int CT_TileLayerStatusList[32];

void (*CT_Main)(); //Main指针函数
void (*CT_Loop)(); //Loop指针函数
int CT_Repaint = 0; //请求重绘
int CT_ViewX = 0; //视野位置x
int CT_ViewY = 0; //视野位置y
int CT_ViewLastX = 0; //上次视野位置x
int CT_ViewLastY = 0; //上次视野位置y
int CT_RunnerStatus = 1; //运行器状态
long long CT_ClockFrequency = 0; //高精度时钟频率
long long CT_NowTime = 0; //现在时间
long long CT_NextFrameTime = 0; //下一帧时间
long long CT_FrameInterval = 16667; //帧间隔
int CT_TimeCalibration = 1; //时间校准
//--------------------------------------------------------------------
// 查询性能频率
//--------------------------------------------------------------------
long long CT_RunnerClockFrequency()
{
    LARGE_INTEGER Frequency; //性能频率
    QueryPerformanceFrequency(&Frequency); //查询性能频率
    return Frequency.QuadPart; //获取性能频率
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 运行器查询计数器
//--------------------------------------------------------------------
long long CT_RunnerClockTime()
{
    LARGE_INTEGER CT_Counter; //运行器时间
    QueryPerformanceCounter(&CT_Counter); //查询性能计数器
    return CT_Counter.QuadPart; //获取计数器时间
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 请求帧循环 - 开始
//--------------------------------------------------------------------
void CT_RequestFrameStart()
{
    CT_NowTime = CT_RunnerClockTime(); //获取运行器时间
    CT_FrameInterval = CT_FrameInterval * CT_ClockFrequency / 1000000; //获取帧间隔
    CT_NextFrameTime = CT_NowTime + CT_FrameInterval; //下一帧时间
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 请求帧循环
//--------------------------------------------------------------------
void CT_RequestFrameLoop()
{
    int sleep_ms = 0; //Sleep时间 毫秒
    long long sleep_start = 0; //Sleep开始时间
    int sleep_deviation = 1000; //Sleep误差
    long long sleep1_free = 0; //Sleep空闲时间
    CT_NowTime = CT_RunnerClockTime(); //获取运行器时间
    sleep_start = CT_NowTime; //Sleep开始时间
    sleep_ms = (CT_NextFrameTime - CT_NowTime) * 1000000 / CT_ClockFrequency / 1000 / 2; //Sleep时间(毫秒)
    if(sleep_ms >= 3) //如果Sleep时间(毫秒)大于0
    {
        Sleep(sleep_ms); //sleep
        CT_NowTime = CT_RunnerClockTime(); //获取运行器时间
        sleep_deviation = (CT_NowTime - sleep_start) * 1000000 / CT_ClockFrequency - sleep_ms * 1000; //Sleep误差
        sleep_deviation = (sleep_deviation / 500) * 500 + 500; //Sleep误差调整为500的倍数
        if(sleep_deviation < 0) //如果Sleep误差小于0
        {
            sleep_deviation = 0; //Sleep误差
        }
    }
    CT_NowTime = CT_RunnerClockTime(); //获取运行器时间
    sleep1_free = (sleep_deviation + 1000) * CT_ClockFrequency / 1000000; //Sleep空闲时间
    while(CT_NextFrameTime - CT_NowTime > sleep1_free) //当运行器时间小于下一帧时间和1毫秒+最大误差
    {
        sleep_start = CT_NowTime; //Sleep开始时间
        Sleep(1); //等待1毫秒
        CT_NowTime = CT_RunnerClockTime(); //获取运行器时间
        sleep_deviation = (CT_NowTime - sleep_start) * 1000000 / CT_ClockFrequency - 1 * 1000;  //Sleep误差
        sleep_deviation = (sleep_deviation / 500) * 500 + 500; //Sleep误差调整为500的倍数
        if(sleep_deviation < 0) //如果Sleep误差小于0
        {
            sleep_deviation = 0; //Sleep误差为0
        }
        sleep1_free = (sleep_deviation + 1000) * CT_ClockFrequency / 1000000; //Sleep空闲时间
        CT_NowTime = CT_RunnerClockTime(); //获取运行器时间
    }
    CT_NowTime = CT_RunnerClockTime(); //获取运行器时间
    while(CT_NextFrameTime - CT_NowTime > 0) //自旋锁 持续到当前帧结束
    {    
        CT_NowTime = CT_RunnerClockTime(); //获取运行器时间
    }
    CT_NowTime = CT_RunnerClockTime(); //获取运行器时间
    CT_NextFrameTime = CT_NowTime + CT_FrameInterval; //下一帧时间
    CT_TimeCalibration ++;
    if(CT_TimeCalibration == 6) //如果校准时间
    {
        CT_NextFrameTime -= 2 * CT_ClockFrequency / 1000000; //下一帧时间减2微秒
        CT_TimeCalibration = 1;
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 运行器初始化
//--------------------------------------------------------------------
void CT_RunnerInit()
{
    CT_ContainerPreallocated(); //预先分配容器
    CT_OffCanvas_Clear(); //清除离屏画布
    CT_OffCanvas_Swap(); //交换离屏画布
    wglMakeCurrent(NULL,NULL); //取消当前opengl环境
    CT_ClockFrequency = CT_RunnerClockFrequency(); //获取性能频率
    CreateThread(NULL,NULL,CT_RunnerStart,NULL,NULL,NULL); //创建运行器线程
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 运行器开始
//--------------------------------------------------------------------
void CT_RunnerStart()
{
    CT_RequestFrameStart(); //开始帧循环
    wglMakeCurrent(CT_DCMW,CT_GLDCMW); //选择缓冲画布环境
    cprint("CanvasTable Running...");
    (*CT_Main)(); //执行Main函数
    CT_RequestFrameLoop(); //请求帧循环
    CT_RunnerLoop(); //运行器开始循环
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 运行器循环
//--------------------------------------------------------------------
void CT_RunnerLoop()
{
    while(1)
    {
        if(CT_RunnerStatus == 0) //如果运行器停止
        {
            CT_RunnerStatus = 1; //恢复运行器
            CT_Repaint = 1; //进行重绘
        }
        CT_GlobalTimerRun(); //运行全局定时器
        CT_TimerRun(); //运行定时器
        CT_UIRun(); //运行UI
        CT_TaskReady(); //准备子程序
        CT_TaskRunMain(); //运行子程序main函数
        CT_TaskRunLoop(); //运行子程序循环函数
        (*CT_Loop)(); //执行循环函数
        CT_SpriteEnd(); //sprite结束
        CT_ButtonEnd(); //按钮结束
        CT_KeyboardEnd(); //按键结束
        if(CT_Repaint == 1) //如果请求重绘
        {
            if(CT_RunnerStatus == 1) //如果运行器没有停止
            {
                CT_RunnerOffRender(); //离屏渲染
                CT_Repaint = 0;
            }
        }
        CT_RequestFrameLoop(); //请求帧循环
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 重绘
//--------------------------------------------------------------------
void CT_InterfaceRepaint()
{
    CT_Repaint = 1; //请求重绘
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 离屏渲染
//--------------------------------------------------------------------
void CT_RunnerOffRender()
{
    int tile_ls = 0; //瓦片层状态
    int sprite_ls = 0; //sprite层状态
    int window_ls = 0; //窗口层状态
    int draw_ls = 0; //draw层状态
    int i;
    CT_OffCanvas_Clear(); //清除缓冲画布
    for(i=0;i<32;i++) //遍历层
    {
        tile_ls = CT_TileLayerStatusList[i]; //获取瓦片层状态
        sprite_ls = CT_SpriteLayerStatusList[i]; //获取sprite层状态
        window_ls = CT_WindowLayerStatusList[i]; //获取窗口层状态
        draw_ls = CT_DrawLayerStatusList[i]; //获取draw层状态
        if(tile_ls == 1) //如果当前瓦片层有瓦片
        {
            CT_TileRender(i); //渲染瓦片
        }
        if(sprite_ls == 1) //如果当前sprite层有sprite
        {
            CT_SpriteRender(i); //渲染sprite
        }
        if(window_ls == 1)  //如果当前窗口层有窗口
        {
            CT_UIRender(i); //渲染UI
        }
        if(draw_ls == 1) //如果当前draw层被使用
        {
            CT_DrawRender(i); //渲染draw
        }
    }
    CT_TouchKeyRender(); //渲染触摸键
    CT_OffCanvas_Swap(); //交换离屏画布
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 运行器重置
//--------------------------------------------------------------------
void CT_RunnerReset()
{
    CT_ContainerDrawClear(); //清除draw容器
    CT_ContainerTaskClear(); //清除子程序容器
    CT_ContainerTimerClear(); //清除定时器容器
    CT_ContainerTileClear(); //清除瓦片容器
    CT_ContainerWindowClear(); //清除窗口容器
    CT_ContainerUIClear(); //清除UI容器
    CT_ContainerSpriteClear(); //清除sprite容器
    CT_ViewX = 0; //视野位置x
    CT_ViewY = 0; //视野位置y
    CT_ViewLastX = 0; //上次视野位置x
    CT_ViewLastY = 0; //上次视野位置y
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 容器预先分配
//--------------------------------------------------------------------
void CT_ContainerPreallocated()
{
    CT_ContainerPreallocated_Draw(1000); //预先分配draw容器
    CT_ContainerPreallocated_Window(100); //预先分配窗口容器
    CT_ContainerPreallocated_UI(500); //预先分配UI容器
    CT_ContainerPreallocated_Task(1000); //预先分配子程序容器
    CT_ContainerPreallocated_Tile(10000); //预先分配瓦片容器
    CT_ContainerPreallocated_Sprite(1000); //预先分配sprite容器
    CT_ContainerPreallocated_Timer(1000); //预先分配定时器容器
    CT_ContainerPreallocated_GlobalTimer(1000); //预先分配全局定时器容器
}
//--------------------------------------------------------------------