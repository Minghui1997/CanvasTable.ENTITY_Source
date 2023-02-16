#include "CanvasTableSYS.h"

int **CT_ContainerTimer = NULL; //定时器容器  格式:[[定时器占位符,定时器时间,定时器时长,定时器状态],......]
char **CT_ContainerTimerName = NULL; //定时器名称列表
int CT_ContainerTimerLength = 0; //定时器容器长度
int CT_ContainerTimerCapacity = 0; //定时器容器容量
int **CT_ContainerGlobalTimer = NULL; //全局定时器容器  格式:[[定时器占位符,定时器时间,定时器时长,定时器状态],......]
char **CT_ContainerGlobalTimerName = NULL; //全局定时器名称列表
int CT_ContainerGlobalTimerLength = 0; //全局定时器容器长度
int CT_ContainerGlobalTimerCapacity = 0; //全局定时器容器容量

//--------------------------------------------------------------------
// 定时器运行
//--------------------------------------------------------------------
void CT_TimerRun()
{
    int i;
    for(i=0;i<CT_ContainerTimerLength;i++) //查找整个定时器列表
    {
        if(CT_ContainerTimer[i][0] != NULL) //如果当前定时器不为空
         {
            if(CT_ContainerTimer[i][3] == 1) //如果当前定时器状态为1
            {
                CT_ContainerTimer[i][1] -= 1; //当前定时器的时间帧 -1
                if(CT_ContainerTimer[i][1] == 0) //如果当前定时器定时结束
                {
                    int timer_duration = CT_ContainerTimer[i][2]; //定时器时长
                    CT_ContainerTimer[i][3] = 2; //将当前定时器的状态标记为结束
                    CT_ContainerTimer[i][1] = timer_duration; //重置定时器
                }
            }
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 全局定时器运行
//--------------------------------------------------------------------
void CT_GlobalTimerRun()
{
    int i;
    for(i=0;i<CT_ContainerGlobalTimerLength;i++) //查找整个定时器列表
    {
        if(CT_ContainerGlobalTimer[i][0] != NULL) //如果当前定时器不为空
        {
            if(CT_ContainerGlobalTimer[i][3] == 1) //如果当前定时器状态为1
            {
                CT_ContainerGlobalTimer[i][1] -= 1; //当前定时器的时间帧 -1
                if(CT_ContainerGlobalTimer[i][1] == 0) //如果当前定时器定时结束
                {
                    int timer_duration = CT_ContainerGlobalTimer[i][2]; //定时器时长
                    CT_ContainerGlobalTimer[i][3] = 2; //将当前定时器的状态标记为结束
                    CT_ContainerGlobalTimer[i][1] = timer_duration; //重置定时器
                }
            }
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 定时器容器预先分配
//--------------------------------------------------------------------
void CT_ContainerPreallocated_Timer(int cap)
{
    int i;
    if(CT_ContainerTimerCapacity == 0) //如果定时器容器容量为0
    {
        CT_ContainerTimer = malloc(sizeof(int*) * cap); //定时器容器分配指针的内存
        CT_ContainerTimerName = malloc(sizeof(char*) * cap); //定时器名称容器分配指针的内存
        for(i=0;i<cap;i++) //遍历容器
        {
            CT_ContainerTimer[i] = malloc(sizeof(int) * 4); //定时器容器分配内存
            memset(CT_ContainerTimer[i],0x00,sizeof(int) * 4); //定时器容器填0
            CT_ContainerTimerName[i] = malloc(sizeof(char) * 129); //定时器名称容器分配内存
            memset(CT_ContainerTimerName[i],0x00,sizeof(char) * 129); //定时器名称容器填0
        }
        CT_ContainerTimerCapacity = cap; //设置定时器容器容量
    }
    if(CT_ContainerTimerCapacity > 0 && cap > CT_ContainerTimerCapacity) //如果容量大于定时器容器容量
    {
        CT_ContainerTimer = realloc(CT_ContainerTimer,sizeof(int*) * cap); //定时器容器重新分配指针的内存
        CT_ContainerTimerName = realloc(CT_ContainerTimerName,sizeof(char*) * cap); //定时器名称容器重新分配指针的内存
        for(i=CT_ContainerTimerCapacity;i<cap;i++) //遍历容器
        {
            CT_ContainerTimer[i] = malloc(sizeof(int) * 4); //定时器容器分配内存
            memset(CT_ContainerTimer[i],0x00,sizeof(int) * 4); //定时器容器填0
            CT_ContainerTimerName[i] = malloc(sizeof(char) * 129); //定时器名称容器分配内存
            memset(CT_ContainerTimerName[i],0x00,sizeof(char) * 129); //定时器名称容器填0
        }
        CT_ContainerTimerCapacity = cap; //设置定时器容器容量
    }

}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 全局定时器容器预先分配
//--------------------------------------------------------------------
void CT_ContainerPreallocated_GlobalTimer(int cap)
{
    int i;
    if(CT_ContainerGlobalTimerCapacity == 0) //如果全局定时器容器容量为0
    {
        CT_ContainerGlobalTimer = malloc(sizeof(int*) * cap); //全局定时器容器分配指针的内存
        CT_ContainerGlobalTimerName = malloc(sizeof(char*) * cap); //全局定时器名称容器分配指针的内存
        for(i=0;i<cap;i++) //遍历容器
        {
            CT_ContainerGlobalTimer[i] = malloc(sizeof(int) * 4); //全局定时器容器分配内存
            memset(CT_ContainerGlobalTimer[i],0x00,sizeof(int) * 4); //全局定时器容器填0
            CT_ContainerGlobalTimerName[i] = malloc(sizeof(char) * 129); //全局定时器名称容器分配内存
            memset(CT_ContainerGlobalTimerName[i],0x00,sizeof(char) * 129); //全局定时器名称容器填0
        }
        CT_ContainerGlobalTimerCapacity = cap; //设置全局定时器容器容量
    }
    if(CT_ContainerGlobalTimerCapacity > 0 && cap > CT_ContainerGlobalTimerCapacity) //如果容量大于全局定时器容器容量
    {
        CT_ContainerGlobalTimer = realloc(CT_ContainerGlobalTimer,sizeof(int*) * cap); //全局定时器容器重新分配指针的内存
        CT_ContainerGlobalTimerName = realloc(CT_ContainerGlobalTimerName,sizeof(char*) * cap); //全局定时器名称容器重新分配指针的内存
        for(i=CT_ContainerGlobalTimerCapacity;i<cap;i++) //遍历容器
        {
            CT_ContainerGlobalTimer[i] = malloc(sizeof(int) * 4); //全局定时器容器分配内存
            memset(CT_ContainerGlobalTimer[i],0x00,sizeof(int) * 4); //全局定时器容器填0
            CT_ContainerGlobalTimerName[i] = malloc(sizeof(char) * 129); //全局定时器名称容器分配内存
            memset(CT_ContainerGlobalTimerName[i],0x00,sizeof(char) * 129); //全局定时器名称容器填0
        }
        CT_ContainerGlobalTimerCapacity = cap; //设置全局定时器容器容量
    }

}
//--------------------------------------------------------------------