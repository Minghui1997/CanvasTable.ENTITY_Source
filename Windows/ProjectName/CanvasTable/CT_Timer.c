#include "CanvasTableSYS.h"
#include <stdio.h>

int CT_ContainerTimer[50000][4] = {NULL}; //定时器容器  格式:[[定时器占位符,定时器时间,定时器时长,定时器状态],......]
char CT_ContainerTimerName[50000][64] = {NULL}; //定时器名称列表
int CT_ContainerTimerLength = 0; //定时器长度
int CT_ContainerGlobalTimer[1000][4] = {NULL}; //全局定时器容器  格式:[[定时器占位符,定时器时间,定时器时长,定时器状态],......]
char CT_ContainerGlobalTimerName[1000][64] = {NULL}; //全局定时器名称列表
int CT_ContainerGlobalTimerLength = 0; //全局定时器长度

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