#define _CRT_SECURE_NO_WARNINGS

#include "CanvasTableSYS.h"
#include <windows.h>
#include <math.h>

extern double CT_Frequency;
extern int CT_ContainerKeyState[256];
extern int CT_ContainerKeyDown[256];
extern int CT_ContainerKeyUp[256];
extern int CT_ContainerMKeyState[3];
extern int CT_ContainerMKeyDown[3];
extern int CT_ContainerMKeyUp[3];
extern int CT_ContainerMWheel[2];
extern char CT_KeyChar;
extern int CT_ContainerDrawLength;
extern int CT_DrawLayer;
extern int **CT_ContainerDraw;
extern char **CT_ContainerDraw_String;
extern char **CT_ContainerDraw_FontCharList_P;
extern int **CT_ContainerDraw_FontGlyphList_P;
extern int **CT_ContainerWindow;
extern int CT_ContainerWindowLength;
extern int **CT_ContainerUI;
extern char **CT_ContainerUI_Value;
extern char **CT_ContainerUI_Name;
extern char **CT_ContainerUI_FontCharList_P;
extern int **CT_ContainerUI_FontGlythList_P;
extern int CT_ContainerUILength;
extern int **CT_ContainerSprite;
extern int CT_ContainerSpriteLength;
extern int **CT_ContainerTile;
extern int CT_ContainerTileLength;
extern int **CT_ContainerTimer;
extern char **CT_ContainerTimerName;
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
void (***CT_ContainerTask)(); //子程序容器
double *CT_ContainerTaskNum = 0; //子程序数字变量容器
char **CT_ContainerTaskStr = NULL; //子程序字符串变量容器
int *CT_TaskStateList = NULL; //子程序状态列表
int CT_ContainerTaskLength = 0; //子程序容器长度
int CT_ContainerCapacity_Task = 0; //子程序容器容量
int CT_TaskRunIndex = 0; //子程序运行索引
int CT_Repaint = 0; //请求重绘
int CT_BufferSwap = 0; //opengl缓冲区交换
int CT_ViewX = 0; //视野位置x
int CT_ViewY = 0; //视野位置y
int CT_ViewLastX = 0; //上次视野位置x
int CT_ViewLastY = 0; //上次视野位置y
int CT_RunnerStatus = 1; //运行器状态
int CT_SleepMaxDeviationUS = 0; //Sleep最大误差(微秒)
int CT_SleepMaxDeviationMS = 0; //Sleep最大误差(毫秒)
int CT_SleepDeviationReset = 0; //重置Sleep最大误差的计数
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
// 运行器等待帧
//--------------------------------------------------------------------
void CT_RunnerWaitFrame(double start_time,double now_time)
{
    double next_frame = start_time + 16667; //下一帧时间
    int interval_ms = (next_frame - now_time) / 1000; //时间间隔(毫秒)
    int deviation = 0; //Sleep误差
    if(interval_ms > CT_SleepMaxDeviationMS) //时间间隔(毫秒)大于Sleep最大误差(毫秒)
    {
        Sleep(interval_ms-CT_SleepMaxDeviationMS); //等待
        now_time = CT_RunnerQueryCounter(); //获取现在时间
        deviation = now_time - start_time - interval_ms * 1000; //Sleep误差
        if(deviation > 3000) //如果Sleep误差大于3000
        {
            deviation = 3000; //如果Sleep误差为3000
        }
        if(deviation > CT_SleepMaxDeviationUS) //如果Sleep误差大于Sleep最大误差
        {
            CT_SleepMaxDeviationUS = deviation; //Sleep最大误差
            CT_SleepMaxDeviationMS = CT_SleepMaxDeviationUS / 1000; //Sleep最大误差(微秒)
        }
    }
    if(now_time < next_frame - CT_SleepMaxDeviationUS) //如果现在时间与下一帧时间的间隔大于Sleep最大误差
    {
        Sleep(1); //等待1ms
    }
    CT_SleepDeviationReset += 1; //重置Sleep最大误差的计数 +1
    if(CT_SleepDeviationReset == 100) //如果重置Sleep最大误差的计数等于100
    {
        CT_SleepMaxDeviationUS = 0; //重置Sleep最大误差(微秒)
        CT_SleepMaxDeviationMS = 0; //重置Sleep最大误差(毫秒)
        CT_SleepDeviationReset = 0; //重置Sleep最大误差的计数为0
    }
    while(now_time < next_frame) //等待时间结束
    {
        now_time = CT_RunnerQueryCounter(); //获取现在时间
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 运行器初始化
//--------------------------------------------------------------------
void CT_RunnerInit()
{
    CT_ContainerPreallocated(); //预先分配容器
    CTCanvas_Clear(); //清除画布
    CTCanvas_Output(); //输出缓冲画布
    wglMakeCurrent(NULL,NULL); //选择缓冲画布环境
    CreateThread(NULL,NULL,CT_RunnerStart,NULL,NULL,NULL); //创建运行器线程
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 运行器开始
//--------------------------------------------------------------------
void CT_RunnerStart()
{
    double start_time = 0; //开始时间
    double now_time = 0; //现在时间
    wglMakeCurrent(CT_DCMW,CT_DCBC); //选择缓冲画布环境
    cprint("CanvasTable Running...");
    start_time = CT_RunnerQueryCounter(); //获取现在时间
    (*CT_Main)(); //执行Main函数
    now_time = CT_RunnerQueryCounter(); //获取现在时间
    CT_RunnerWaitFrame(start_time,now_time); //等待到下一帧
    CT_RunnerLoop(); //运行开始循环
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 运行器循环
//--------------------------------------------------------------------
void CT_RunnerLoop()
{
    double start_time = 0; //开始时间
    double now_time = 0; //现在时间
    while(1)
    {
        start_time = CT_RunnerQueryCounter(); //获取现在时间
        if(CT_BufferSwap == 1) //如果翻转opengl缓冲区
        {
            CTCanvas_Output(); //输出缓冲画布
            CT_BufferSwap = 0;
        }
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
        CT_SpriteReset(); //sprite重置
        CT_ButtonReset(); //按钮重置
        CT_KeyboardReset(); //按键重置
        if(CT_Repaint == 1) //如果请求重绘
        {
            if(CT_RunnerStatus == 1) //如果运行器没有停止
            {
                CT_RunnerBufferRender(); //双重缓冲渲染
                CT_Repaint = 0;
                CT_BufferSwap = 1; //翻转opengl缓冲区
            }
        }
        now_time = CT_RunnerQueryCounter(); //获取现在时间
        CT_RunnerWaitFrame(start_time,now_time); //等待到下一帧
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
    CT_TouchKeyRender(); //渲染触摸键
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 创建子程序
//--------------------------------------------------------------------
void CT_TaskCreated(int task_i,void *task_m,void *task_l)
{
    if(task_i <= CT_ContainerTaskLength) //如果子程序索引小于子程序容器
    {
        CT_ContainerTask[task_i][0] = task_m; //将子程序存入容器
        CT_ContainerTask[task_i][1] = task_l; //将子程序存入容器
        CT_TaskStateList[task_i] = 0; //将子程序状态设置为等待
        CT_ContainerTaskLength ++; //子程序长度 +1
        if(CT_ContainerTaskLength % 1000 == 0) //如果子程序容器长度为1000的倍数
        {
            CT_ContainerPreallocated_Task(CT_ContainerTaskLength+1000); //预先分配子程序容器
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 销毁子程序
//--------------------------------------------------------------------
void CT_TaskDestroy(int ti)
{
    if(ti < CT_ContainerTaskLength) //如果子程序索引小于子程序容器长度
    {
        int var_s = ti * 100;
        int var_e = var_s + 100;
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
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 子程序变量赋值 数字
//--------------------------------------------------------------------
void CT_TaskSetNum(int ti,int vi,double n)
{
    if(ti < CT_ContainerTaskLength) //如果子程序索引小于子程序容器长度
    {
        int var_index = ti * 100 + vi; //数字变量索引
        CT_ContainerTaskNum[var_index] = n; //数字变量赋值
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 子程序变量 数字
//--------------------------------------------------------------------
double CT_TaskNum(int ti,int vi)
{
    if(ti < CT_ContainerTaskLength) //如果子程序索引小于子程序容器长度
    {
        int var_index = ti * 100 + vi; //数字变量索引
        double num = CT_ContainerTaskNum[var_index]; //获取数字变量
        return num;
    }
    return 0;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 子程序变量赋值 字符串
//--------------------------------------------------------------------
void CT_TaskSetStr(int ti,int vi,char *s)
{
    if(ti < CT_ContainerTaskLength) //如果子程序索引小于子程序容器长度
    {
        int var_index = ti * 100 + vi; //字符串变量索引
        strcpy(CT_ContainerTaskStr[var_index],s); //复制字符串
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 程序变量 字符串
//--------------------------------------------------------------------
char *CT_TaskStr(int ti,int vi)
{
    if(ti < CT_ContainerTaskLength) //如果子程序索引小于子程序容器长度
    {
        int var_index = ti * 100 + vi; //字符串变量索引
        char *str = CT_ContainerTaskStr[var_index]; //获取字符串变量指针
        return str;
    }
    return 0;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 子程序准备
//--------------------------------------------------------------------
void CT_TaskReady()
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
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 运行子程序main函数
//--------------------------------------------------------------------
void CT_TaskRunMain()
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
    CT_TaskRunIndex = 0; //子程序运行索引
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 运行子程序loop函数
//--------------------------------------------------------------------
void CT_TaskRunLoop()
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
        int draw_type = CT_ContainerDraw[i][1]; //draw类型
        for(i2=0;i2<15;i2++) //遍历draw数据
        {
            CT_ContainerDraw[i][i2] = NULL; //清除draw数据
        }
        if(draw_type == 4 || draw_type == 9) //如果draw类型是字符串
        {
            strclear(CT_ContainerDraw_String[i]); //清除draw数据
            CT_ContainerDraw_FontCharList_P[i] = NULL; //清除字体字符列表
            CT_ContainerDraw_FontGlyphList_P[i] = NULL; //清除字体字形列表
        }
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
        int ui_type = CT_ContainerUI[i][3]; //UI对象id
        for(i2=0;i2<25;i2++) //遍历UI数据
        {
            CT_ContainerUI[i][i2] = NULL; //清除UI数据
        }
        if(ui_type == 1 || ui_type == 2 || ui_type == 6) //如果ui是按钮 / 文本 / 文本框
        {
            strclear(CT_ContainerUI_Value[i]); //清除UI数据
            strclear(CT_ContainerUI_Name[i]); //清除UI名称
            CT_ContainerUI_FontCharList_P[i] = NULL; //清除UI字体字符列表
            CT_ContainerUI_FontGlythList_P[i] = NULL; //清除UI字体字形列表
        }
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
            int var_s = i * 100;
            int var_e = var_s + 100;
            int i2;
            CT_ContainerTask[i][0] = NULL; //清除子程序
            CT_ContainerTask[i][1] = NULL; //清除子程序
            CT_TaskStateList[i] = 0; //清除子程序列表
            for(i2=var_s;i2<var_e;i2++) //遍历当前子程序变量
            {
                CT_ContainerTaskNum[i2] = 0; //清除子程序数字变量
                strclear(CT_ContainerTaskStr[i2]); //清除子程序字符串变量
            }
        }
    }
    for(i=0;i<CT_ContainerTimerLength;i++) //遍历定时器容器
    {
        for(i2=0;i2<4;i2++) //遍历定时器数据
        {
            CT_ContainerTimer[i][i2] = NULL; //清除定时器数据
        }
        strclear(CT_ContainerTimerName[i]);
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
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 子程序容器预先分配
//--------------------------------------------------------------------
void CT_ContainerPreallocated_Task(int cap)
{
    int i;
    if(CT_ContainerCapacity_Task == 0) //如果子程序容器容量为0
    {
        CT_ContainerTask = malloc(sizeof(void*) * cap); //子程序容器分配指针的内存
        CT_TaskStateList = malloc(sizeof(int) * cap); //子程序状态列表分配内存
        CT_ContainerTaskNum = malloc(sizeof(double) * 100 * cap); //子程序数字变量容器分配内存
        CT_ContainerTaskStr = malloc(sizeof(char*) * 100 * cap); //子程序字符串变量容器分配指针的内存
        for(i=0;i<100*cap;i++) //遍历子程序变量
        {
            CT_ContainerTaskNum[i] = 0; //子程序数字变量赋值
            CT_ContainerTaskStr[i] = malloc(sizeof(char) * 129); //子程序字符串变量容器分配内存
            memset(CT_ContainerTaskStr[i],0x00,129); //子程序字符串变量容器填0
        }
        for(i=0;i<cap;i++) //遍历子程序容器
        {
            CT_ContainerTask[i] = malloc(sizeof(void*) * 2); //子程序容器分配指针的指针的内存
            CT_ContainerTask[i][0] = 0; //子程序main函数指针
            CT_ContainerTask[i][1] = 0; //子程序loop函数指针
            CT_TaskStateList[i] = 0; //子程序状态
        }
        CT_ContainerCapacity_Task = cap; //设置子程序容器容量
    }
    if(CT_ContainerCapacity_Task > 0 && cap > CT_ContainerCapacity_Task) //如果容量大于子程序容器容量
    {
        CT_ContainerTask = realloc(CT_ContainerTask,sizeof(void*) * cap); //子程序容器重新分配指针的内存
        CT_TaskStateList = realloc(CT_TaskStateList,sizeof(int) * cap); //子程序状态列表重新分配内存
        CT_ContainerTaskNum = realloc(CT_ContainerTaskNum,sizeof(double) * 100 * cap); //子程序数字变量容器重新分配内存
        CT_ContainerTaskStr = realloc(CT_ContainerTaskStr,sizeof(char*) * 100 * cap); //子程序字符串变量容器重新分配内存
        for(i=CT_ContainerCapacity_Task*100;i<100*cap;i++) //遍历子程序变量
        {
            CT_ContainerTaskNum[i] = 0; //子程序数字变量赋值
            CT_ContainerTaskStr[i] = malloc(sizeof(char) * 129); //子程序字符串变量容器分配内存
            memset(CT_ContainerTaskStr[i],0x00,129); //子程序字符串变量容器填0
        }
        for(i=CT_ContainerCapacity_Task;i<cap;i++) //遍历容器
        {
            CT_ContainerTask[i] = malloc(sizeof(void*) * 2); //子程序容器重新分配指针的指针的内存
            CT_ContainerTask[i][0] = 0; //子程序main函数指针
            CT_ContainerTask[i][1] = 0; //子程序loop函数指针
            CT_TaskStateList[i] = 0; //子程序状态
        }
        CT_ContainerCapacity_Task = cap; //设置子程序容器容量
    }
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