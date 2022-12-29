#define _CRT_SECURE_NO_WARNINGS

#include "CanvasTableSYS.h"
#include <windows.h>

extern double CT_Frequency;
extern int CT_ContainerKeyState[256];
extern int CT_ContainerKeyDown[256];
extern int CT_ContainerKeyUp[256];
extern int CT_ContainerMKeyState[3];
extern int CT_ContainerMKeyDown[3];
extern int CT_ContainerMKeyUp[3];
extern int CT_ContainerMWheel[2];
extern char CT_KeyChar;
extern int CT_ContainerDraw[10000][15];
extern char CT_ContainerDraw_String[10000][512];
extern int CT_ContainerDrawLength;
extern int CT_DrawLayer;
extern int CT_ContainerWindow[100][15];
extern int CT_ContainerWindowLength;
extern int CT_ContainerUI[1000][25];
extern char CT_ContainerUI_Value[1000][2048];
extern char *CT_ContainerUI_FontCharList_P[1000];
extern int *CT_ContainerUI_FontGlythList_P[1000];
extern int CT_ContainerUILength;
extern int CT_ContainerSprite[10000][25];
extern int CT_ContainerSpriteLength;
extern int CT_ContainerTile[100000][7];
extern int CT_ContainerTileLength;
extern int CT_ContainerTimer[50000][4];
extern char CT_ContainerTimerName[50000][64];
extern int CT_ContainerTimerLength;
extern int CT_Button_Index;
extern int CT_Textbox_Index;
extern int CT_DownX;
extern int CT_DownY;
extern int CT_DialogShow;
extern int CT_Dialog_Index;
extern HDC CT_DCMW; //主窗口dc
extern HDC CT_DCBC;

void (*CT_Main)(); //Main指针函数
void (*CT_Loop)(); //Loop指针函数
void (*CT_ContainerTask[10000][2])(); //子程序容器
double CT_ContainerTaskNum[1000000] = {0}; //子程序数字变量容器
char CT_ContainerTaskStr[1000000][64] = {NULL}; //子程序字符串变量容器
int CT_TaskStateList[10000] = {NULL}; //子程序状态列表
int CT_ContainerTaskLength = 0; //子程序容器长度
int CT_TaskRunIndex = 0; //子程序运行索引
int CT_Repaint = 0; //请求重绘
int CT_ViewX = 0; //视野位置x
int CT_ViewY = 0; //视野位置y
int CT_ViewLastX = 0; //上次视野位置x
int CT_ViewLastY = 0; //上次视野位置y
int CT_RunnerStatus = 1; //运行器状态
//--------------------------------------------------------------------
// 运行器查询计数器
//--------------------------------------------------------------------
double CT_RunnerQueryCounter()
{
    LARGE_INTEGER Counter; //计数器
    double counter = 0; //计数器
    QueryPerformanceCounter(&Counter); //查询性能计数器
    counter = Counter.QuadPart * 1000000 / CT_Frequency; //获取计数器时间
    return counter;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 运行器等待
//--------------------------------------------------------------------
void CT_RunnerWait(double time)
{
    double start_time = 0; //开始时间
    double end_time = 0; //结束时间
    double now_time = 0; //现在时间
    start_time = CT_RunnerQueryCounter(); //获取等待开始的时间
    end_time = start_time + time; //等待结束的时间
    now_time = start_time; //现在时间
    while(now_time < end_time) //如果现在时间小于结束的时间，始终循环
    {
        now_time = CT_RunnerQueryCounter(); //获取现在时间
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 运行器开始
//--------------------------------------------------------------------
void CT_RunnerStart()
{
    wglMakeCurrent(CT_DCMW,CT_DCBC); //选择缓冲画布环境
    cprint("CanvasTable Running...");
    (*CT_Main)(); //执行Main函数
    CT_RunnerWait(16667); //等待1帧
    CT_RunnerLoop(); //运行器循环
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 运行器循环
//--------------------------------------------------------------------
void CT_RunnerLoop()
{
    double now_time = 0; //现在时间
    double next_time = 0; //下一帧时间
    int interval_s = 0; //时间间隔(秒)
    while(1)
    {
        now_time = CT_RunnerQueryCounter(); //获取现在时间
        next_time = now_time + 16667; //下一帧时间
//--------------------------------------------------------------------
        if(CT_RunnerStatus == 0) //如果运行器停止
        {
            CT_RunnerStatus = 1; //恢复运行器
            CT_Repaint = 1; //进行重绘
        }
        CT_GlobalTimerRun(); //运行全局定时器
        CT_TimerRun(); //运行定时器
        CT_UIRun(); //运行UI 
        CT_SpritePosRecord(); //记录sprite位置
        CT_TaskReady(); //准备子程序
        CT_TaskRunMain(); //运行子程序main函数
        CT_TaskRunLoop(); //运行子程序循环函数
        (*CT_Loop)(); //执行循环函数
        CT_ButtonReset(); //按钮重置
        CT_KeyboardReset(); //按键重置
        if(CT_Repaint == 1) //如果请求重绘
        {
            if(CT_RunnerStatus == 1) //如果运行器没有停止
            {
                CT_RunnerBufferRender(); //双重缓冲渲染
            }
            CT_Repaint = 0;
        }
//--------------------------------------------------------------------
        now_time = CT_RunnerQueryCounter(); //获取现在时间
        interval_s = ((next_time - now_time) / 1000); //时间间隔(毫秒)
        if(interval_s - 1 > 0) //时间间隔(秒)大于0
        {
            Sleep(interval_s-1); //睡眠
        }
        while(now_time < next_time) //等待时间到下一帧
        {
            now_time = CT_RunnerQueryCounter(); //获取现在时间
        }
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
// 双重缓冲渲染
//--------------------------------------------------------------------
void CT_RunnerBufferRender()
{
    int i;
    CTCanvas_Clear(); //清除缓冲画布
    for(i=0;i<32;i++) //遍历层
    {
        CT_TileRender(i); //渲染瓦片
        CT_SpriteRender(i); //渲染sprite
        CT_UIRender(i); //渲染UI
        CT_DrawRender(i); //渲染draw
    }
    CT_TouchKeyRender();
    CTCanvas_Output(); //输出缓冲画布
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 创建程序
//--------------------------------------------------------------------
void CT_TaskCreated(int task_i,void *task_m,void *task_l)
{
    CT_ContainerTask[task_i][0] = task_m; //将子程序存入容器
    CT_ContainerTask[task_i][1] = task_l; //将子程序存入容器
    CT_TaskStateList[task_i] = 0; //将子程序状态设置为等待
    if(task_i + 1 > CT_ContainerTaskLength)
    {
        CT_ContainerTaskLength = task_i + 1; //改变子程序索引
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 销毁程序
//--------------------------------------------------------------------
void CT_TaskDestroy(int ti)
{
    int var_s = ti * 128;
    int var_e = var_s + 128;
    int i;
    CT_ContainerTask[ti][0] = NULL; //清除子程序函数
    CT_ContainerTask[ti][1] = NULL; //清除子程序函数
    CT_TaskStateList[ti] = 0; //子程序状态为0
    for(i=var_s;i<var_e;i++) //遍历该子程序的变量数组
    {
        CT_ContainerTaskNum[i] = 0; //清除数字变量
        strclear(CT_ContainerTaskStr[i]); //清除字符串变量
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 程序变量赋值 数字
//--------------------------------------------------------------------
void CT_TaskSetNum(int ti,int vi,double n)
{
    int var_index = ti * 128 + vi; //数字变量索引
    CT_ContainerTaskNum[var_index] = n; //数字变量赋值
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 程序变量 数字
//--------------------------------------------------------------------
double CT_TaskNum(int ti,int vi)
{
    double num = 0; //数字变量
    int var_index = ti * 128 + vi; //数字变量索引
    num = CT_ContainerTaskNum[var_index]; //获取数字变量
    return num;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 程序变量赋值 字符串
//--------------------------------------------------------------------
void CT_TaskSetStr(int ti,int vi,char *s)
{
    int var_index = ti * 128 + vi; //字符串变量索引
    strcpy(CT_ContainerTaskStr[var_index],s);
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 程序变量 字符串
//--------------------------------------------------------------------
char *CT_TaskStr(int ti,int vi)
{
    char *str = NULL;
    int var_index = ti * 128 + vi; //字符串变量索引
    str = CT_ContainerTaskStr[var_index]; //获取字符串变量指针
    return str;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 子程序准备
//--------------------------------------------------------------------
void CT_TaskReady()
{
    if(CT_ContainerTaskLength != 0) //如果子程序容器长度不为0
    {
        int i;
        for(i=0;i<CT_ContainerTaskLength;i++) //遍历子程序容器
        {
            if(CT_ContainerTask[i][0] != NULL) //如果当前子程序不为空
            {
                if(CT_TaskStateList[i] == 0) //如果当前子程序状态为准备
                {
                    CT_TaskStateList[i] = 1; //将当前子程序标记为运行
                }
            }
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 运行添加的程序main函数
//--------------------------------------------------------------------
void CT_TaskRunMain()
{
    if(CT_ContainerTaskLength != 0) //如果子程序容器长度不为0
    {
        int i;
        for(i=0;i<CT_ContainerTaskLength;i++) //遍历子程序容器
        {
            if(CT_ContainerTask[i][0] != NULL) //如果当前子程序不为空
            {
                if(CT_TaskStateList[i] == 1) //如果当前子程序状态为等待
                {
                    CT_TaskRunIndex = i; //子程序运行索引
                    CT_ContainerTask[i][0](); //执行子程序main函数
                    CT_TaskStateList[i] = 2; //将当前子程序状态设置为运行
                }
            }
        }
    }
    CT_TaskRunIndex = 0; //子程序运行索引
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 运行程序loop函数
//--------------------------------------------------------------------
void CT_TaskRunLoop()
{
    if(CT_ContainerTaskLength != 0) //如果子程序容器长度不为0
    {
        int i;
        for(i=0;i<CT_ContainerTaskLength;i++) //遍历子程序容器
        {
            if(CT_ContainerTask[i][1] != NULL) //如果当前子程序不为空
            {
                if(CT_TaskStateList[i] == 2) //如果当前子程序状态为运行
                {
                    CT_TaskRunIndex = i; //子程序运行索引
                    CT_ContainerTask[i][1](); //执行子程序循环函数
                }
            }
        }
    }
    CT_TaskRunIndex = 0; //子程序运行索引
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 运行器重置
//--------------------------------------------------------------------
void CT_RunnerReset()
{
    int i;
    int i2;
    for(i=0;i<CT_ContainerDrawLength;i++) //遍历draw容器
    {
        for(i2=0;i2<15;i2++) //遍历draw数据
        {
            CT_ContainerDraw[i][i2] = NULL; //清除draw数据
        }
        strclear(CT_ContainerDraw_String[i]); //清除draw数据
    }
    for(i=0;i<CT_ContainerWindowLength;i++) //遍历窗口容器
    {
        for(i2=0;i2<15;i2++) //遍历窗口数据
        {
            CT_ContainerWindow[i][i2] = NULL; //清除窗口数据
        }
    }
    for(i=0;i<CT_ContainerUILength;i++) //遍历UI容器
    {
        for(i2=0;i2<25;i2++) //遍历UI数据
        {
            CT_ContainerUI[i][i2] = NULL; //清除UI数据
        }
        strclear(CT_ContainerUI_Value[i]); //清除UI数据
        CT_ContainerUI_FontCharList_P[i] = NULL;
        CT_ContainerUI_FontGlythList_P[i] = NULL;
    }
    for(i=0;i<CT_ContainerSpriteLength;i++) //遍历sprite容器
    {
        for(i2=0;i2<25;i2++) //遍历sprite数据
        {
            CT_ContainerSprite[i][i2] = NULL; //清除sprite数据
        }
    }
    for(i=0;i<CT_ContainerTileLength;i++) //遍历瓦片容器
    {
        for(i2=0;i2<7;i2++) //遍历瓦片数据
        {
            CT_ContainerTile[i][i2] = NULL; //清除瓦片数据
        }
    }
    for(i=0;i<CT_ContainerTaskLength;i++) //遍历子程序
    {
        if(CT_ContainerTask[i][0] != NULL) //如果当前子程序不为空
        {
            int var_s = i * 128;
            int var_e = var_s + 128;
            int i2;
            for(i2=var_s;i2<var_e;i2++) //遍历当前子程序变量
            {
                CT_ContainerTaskNum[i2] = 0; //清除子程序数字变量
                strclear(CT_ContainerTaskStr[i2]); //清除子程序字符串变量
            }
        }
    }
    for(i=0;i<CT_ContainerTaskLength;i++) //遍历子程序
    {
        CT_ContainerTask[i][0] = NULL; //清除子程序
        CT_ContainerTask[i][1] = NULL; //清除子程序
        CT_TaskStateList[i] = NULL; //清除子程序列表
    }
    for(i=0;i<CT_ContainerTimerLength;i++) //遍历定时器容器
    {
        for(i2=0;i2<4;i2++) //遍历定时器数据
        {
            CT_ContainerTimer[i][i2] = NULL; //清除定时器数据
        }
    }
    CT_ContainerDrawLength = 0; //Draw容器长度
    CT_DrawLayer = 0; //Draw容器层
    CT_ContainerWindowLength = 0; //窗口容器长度
    CT_ContainerUILength = 0; //UI容器长度
    CT_ContainerSpriteLength = 0; //Sprite容器长度
    CT_ContainerTileLength = 0; //Tile容器长度
    CT_ContainerTaskLength = 0; //子程序长度
    CT_TaskRunIndex = 0; //子程序运行索引
    CT_ContainerTimerLength = 0; //定时器长度
    CT_ViewX = 0; //视野位置x
    CT_ViewY = 0; //视野位置y
    CT_ViewLastX = 0; //上次视野位置x
    CT_ViewLastY = 0; //上次视野位置y
    CT_DialogDestroy(); //销毁对话框
    CT_Button_Index = -1; //按下按钮的索引
    CT_Textbox_Index = -1; //点击UI的索引
    CT_DownX = -1; //鼠标按下的位置x
    CT_DownY = -1; //鼠标按下的位置y
    CT_DialogShow = 0; //对话框显示
    CT_Dialog_Index = -1; //对话框索引
}
//-------------------------------------------------------------