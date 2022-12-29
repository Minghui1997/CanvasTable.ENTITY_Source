#define _CRT_SECURE_NO_WARNINGS

#include "CanvasTableSYS.h"
#include <stdio.h>
#include <string.h>

extern int CT_ContainerTimer[50000][4];
extern char CT_ContainerTimerName[50000][64];
extern int CT_ContainerTimerLength;
extern int CT_ContainerGlobalTimer[1000][4];
extern char CT_ContainerGlobalTimerName[1000][64];
extern int CT_ContainerGlobalTimerLength;

//--------------------------------------------------------------------
// 新建定时器
//--------------------------------------------------------------------
void timer_set(char *timer,float time)
{
    int i;
    for(i=0;i<CT_ContainerTimerLength;i++) //遍历定时器容器
    {
        if(CT_ContainerTimer[i][0] != NULL) //如果当前定时器不为空
        {
            char *timer_name = CT_ContainerTimerName[i]; //获取当前定时器名称
            if(strcmp(timer_name,timer) == 0) //如果当前定时器名称与参数定时器名称一致
            {
                cprint("ERROR : The timer have been created.");
                return; //中断
            }
        }
    }
    if(time >= 0.017) //如果定时器时间大于等于0.017
    {
        int time_frame = time * 60; //计算时间帧
        int i2;
        for(i2=0;i2<CT_ContainerTimerLength;i2++) //遍历定时器容器
        {
            if(CT_ContainerTimer[i2][0] == NULL) //如果当前定时器为空
            {
                //将定时器数据写入定时器索引
                strcpy(CT_ContainerTimerName[i2],timer);
                CT_ContainerTimer[i2][0] = 1;
                CT_ContainerTimer[i2][1] = time_frame;
                CT_ContainerTimer[i2][2] = time_frame;
                CT_ContainerTimer[i2][3] = 0;
                return; //中断
            }
        }
        //将定时器数据写入定时器索引
        strcpy(CT_ContainerTimerName[CT_ContainerTimerLength],timer);
        CT_ContainerTimer[CT_ContainerTimerLength][0] = 1;
        CT_ContainerTimer[CT_ContainerTimerLength][1] = time_frame;
        CT_ContainerTimer[CT_ContainerTimerLength][2] = time_frame;
        CT_ContainerTimer[CT_ContainerTimerLength][3] = 0;
        CT_ContainerTimerLength ++; //定时器容器索引 +1
    }
    else
    {
        cprint("ERROR : The timer duration than the minimum.");
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 新建帧定时器
//--------------------------------------------------------------------
void timer_set_f(char *timer,int fn)
{
    int i;
    for(i=0;i<CT_ContainerTimerLength;i++) //遍历定时器容器
    {
        if(CT_ContainerTimer[i][0] != NULL) //如果当前定时器不为空
        {
            char *timer_name = CT_ContainerTimerName[i]; //获取当前定时器名称
            if(strcmp(timer_name,timer) == 0) //如果当前定时器名称与参数定时器名称一致
            {
                cprint("ERROR : The timer have been created.");
                return; //中断
            }
        }
    }
    if(fn >= 1) //如果定时器时间大于等于0.017
    {
        int i2;
        for(i2=0;i2<CT_ContainerTimerLength;i2++) //遍历定时器容器
        {
            if(CT_ContainerTimer[i2][0] == NULL) //如果当前定时器为空
            {
                //将定时器数据写入定时器索引
                strcpy(CT_ContainerTimerName[i2],timer);
                CT_ContainerTimer[i2][0] = 1;
                CT_ContainerTimer[i2][1] = fn;
                CT_ContainerTimer[i2][2] = fn;
                CT_ContainerTimer[i2][3] = 0;
                return; //中断
            }
        }
        //将定时器数据写入定时器索引
        strcpy(CT_ContainerTimerName[CT_ContainerTimerLength],timer);
        CT_ContainerTimer[CT_ContainerTimerLength][0] = 1;
        CT_ContainerTimer[CT_ContainerTimerLength][1] = fn;
        CT_ContainerTimer[CT_ContainerTimerLength][2] = fn;
        CT_ContainerTimer[CT_ContainerTimerLength][3] = 0;
        CT_ContainerTimerLength ++; //定时器容器索引 +1
    }
    else
    {
        cprint("ERROR : The timer duration than the minimum.");
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 清除定时器
//--------------------------------------------------------------------
void timer_clear(char *timer)
{
    int i;
    for(i=0;i<CT_ContainerTimerLength;i++) //遍历定时器容器
    {
        if(CT_ContainerTimer[i][0] != NULL) //如果当前定时器不为空
        {
            char *timer_name = CT_ContainerTimerName[i]; //获取当前定时器名称
            if(strcmp(timer_name,timer) == 0) //如果当前定时器名称与参数定时器名称一致
            {
                //清除定时器容器数据
                CT_ContainerTimer[i][0] = 0;
                CT_ContainerTimer[i][1] = 0;
                CT_ContainerTimer[i][2] = 0;
                CT_ContainerTimer[i][3] = 0;
                strclear(CT_ContainerTimerName[i]);
            }
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 运行定时器
//--------------------------------------------------------------------
void timer_run(char *timer)
{
    int i;
    for(i=0;i<CT_ContainerTimerLength;i++) //遍历定时器容器
    {
        if(CT_ContainerTimer[i][0] != NULL) //如果当前定时器不为空
        {
            char *timer_name = CT_ContainerTimerName[i]; //获取当前定时器名称
            if(strcmp(timer_name,timer) == 0) //如果当前定时器名称与参数定时器名称一致
            {
                CT_ContainerTimer[i][3] = 1; //运行指定定时器
                return; //中断
            }
        }
    }
    cprint("ERROR : Timer invalid.");
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 停止定时器
//--------------------------------------------------------------------
void timer_stop(char *timer)
{
    int i;
    for(i=0;i<CT_ContainerTimerLength;i++) //遍历定时器容器
    {
        if(CT_ContainerTimer[i][0] != NULL) //如果当前定时器不为空
        {
            char *timer_name = CT_ContainerTimerName[i]; //获取当前定时器名称
            if(strcmp(timer_name,timer) == 0) //如果当前定时器名称与参数定时器名称一致
            {
                int timer_duration = CT_ContainerTimer[i][2]; //定时器时长
                CT_ContainerTimer[i][1] = timer_duration; //重置定时器
                CT_ContainerTimer[i][3] = 0; //停止指定定时器
                return; //中断
            }
        }
    }
    cprint("ERROR : Timer invalid.");
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 暂停定时器
//--------------------------------------------------------------------
void timer_pause(char *timer)
{
    int i;
    for(i=0;i<CT_ContainerTimerLength;i++) //遍历定时器容器
    {
        if(CT_ContainerTimer[i][0] != NULL) //如果当前定时器不为空
        {
            char *timer_name = CT_ContainerTimerName[i]; //获取当前定时器名称
            if(strcmp(timer_name,timer) == 0) //如果当前定时器名称与参数定时器名称一致
            {
                CT_ContainerTimer[i][3] = 0; //停止指定定时器
                return; //中断
            }
        }
    }
    cprint("ERROR : Timer invalid.");
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 检查定时器是否结束
//--------------------------------------------------------------------
int timer_end(char *timer)
{
    int i;
    for(i=0;i<CT_ContainerTimerLength;i++) //遍历定时器容器
    {
        if(CT_ContainerTimer[i][0] != NULL) //如果当前定时器不为空
        {
            char *timer_name = CT_ContainerTimerName[i]; //获取当前定时器名称
            if(strcmp(timer_name,timer) == 0) //如果当前定时器名称与参数定时器名称一致
            {
                if(CT_ContainerTimer[i][3] == 2) //如果指定定时器结束
                {
                    return 1;
                }
                return 0;
            }
        }
    }
    cprint("ERROR : Timer invalid.");
    return 0;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 新建全局定时器
//--------------------------------------------------------------------
void globaltimer_set(char *timer,float time)
{
    int i;
    for(i=0;i<CT_ContainerGlobalTimerLength;i++) //遍历全局定时器容器
    {
        if(CT_ContainerGlobalTimer[i][0] != NULL) //如果当前全局定时器不为空
        {
            char *globaltimer_name = CT_ContainerGlobalTimerName[i]; //获取当前全局定时器名称
            if(strcmp(globaltimer_name,timer) == 0) //如果当前全局定时器名称与参数全局定时器名称一致
            {
                cprint("ERROR : The timer have been created.");
                return; //中断
            }
        }
    }
    if(time >= 0.017) //如果全局定时器时间大于等于0.017
    {
        int time_frame = time * 60; //计算时间帧
        int i2;
        for(i2=0;i2<CT_ContainerGlobalTimerLength;i2++) //遍历全局定时器容器
        {
            if(CT_ContainerGlobalTimer[i2][0] == NULL) //如果当前全局定时器为空
            {
                //将全局定时器数据写入全局定时器索引
                strcpy(CT_ContainerGlobalTimerName[i2],timer);
                CT_ContainerGlobalTimer[i2][0] = 1;
                CT_ContainerGlobalTimer[i2][1] = time_frame;
                CT_ContainerGlobalTimer[i2][2] = time_frame;
                CT_ContainerGlobalTimer[i2][3] = 0;
                return; //中断
            }
        }
        //将全局定时器数据写入全局定时器索引
        strcpy(CT_ContainerGlobalTimerName[CT_ContainerGlobalTimerLength],timer);
        CT_ContainerGlobalTimer[CT_ContainerGlobalTimerLength][0] = 1;
        CT_ContainerGlobalTimer[CT_ContainerGlobalTimerLength][1] = time_frame;
        CT_ContainerGlobalTimer[CT_ContainerGlobalTimerLength][2] = time_frame;
        CT_ContainerGlobalTimer[CT_ContainerGlobalTimerLength][3] = 0;
        CT_ContainerGlobalTimerLength ++; //全局定时器容器索引 +1
    }
    else
    {
        cprint("ERROR : The timer duration than the minimum.");
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 新建全局帧定时器
//--------------------------------------------------------------------
void globaltimer_set_f(char *timer,int fn)
{
    int i;
    for(i=0;i<CT_ContainerGlobalTimerLength;i++) //遍历全局定时器容器
    {
        if(CT_ContainerGlobalTimer[i][0] != NULL) //如果当前全局定时器不为空
        {
            char *globaltimer_name = CT_ContainerGlobalTimerName[i]; //获取当前全局定时器名称
            if(strcmp(globaltimer_name,timer) == 0) //如果当前全局定时器名称与参数全局定时器名称一致
            {
                cprint("ERROR : The timer have been created.");
                return; //中断
            }
        }
    }
    if(fn >= 1) //如果全局定时器时间帧大于等于1
    {
        int i2;
        for(i2=0;i2<CT_ContainerGlobalTimerLength;i2++) //遍历全局定时器容器
        {
            if(CT_ContainerGlobalTimer[i2][0] == NULL) //如果当前全局定时器为空
            {
                //将全局定时器数据写入全局定时器索引
                strcpy(CT_ContainerGlobalTimerName[i2],timer);
                CT_ContainerGlobalTimer[i2][0] = 1;
                CT_ContainerGlobalTimer[i2][1] = fn;
                CT_ContainerGlobalTimer[i2][2] = fn;
                CT_ContainerGlobalTimer[i2][3] = 0;
                return; //中断
            }
        }
        //将全局定时器数据写入全局定时器索引
        strcpy(CT_ContainerGlobalTimerName[CT_ContainerGlobalTimerLength],timer);
        CT_ContainerGlobalTimer[CT_ContainerGlobalTimerLength][0] = 1;
        CT_ContainerGlobalTimer[CT_ContainerGlobalTimerLength][1] = fn;
        CT_ContainerGlobalTimer[CT_ContainerGlobalTimerLength][2] = fn;
        CT_ContainerGlobalTimer[CT_ContainerGlobalTimerLength][3] = 0;
        CT_ContainerGlobalTimerLength ++; //全局定时器容器索引 +1
    }
    else
    {
        cprint("ERROR : The timer duration than the minimum.");
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 清除全局定时器
//--------------------------------------------------------------------
void globaltimer_clear(char *timer)
{
    int i;
    for(i=0;i<CT_ContainerGlobalTimerLength;i++) //遍历全局定时器容器
    {
        if(CT_ContainerGlobalTimer[i][0] != NULL) //如果当前全局定时器不为空
        {
            char *globaltimer_name = CT_ContainerGlobalTimerName[i]; //获取当前全局定时器名称
            if(strcmp(globaltimer_name,timer) == 0) //如果当前全局定时器名称与参数全局定时器名称一致
            {
                //清除全局定时器容器数据
                CT_ContainerGlobalTimer[i][0] = 0;
                CT_ContainerGlobalTimer[i][1] = 0;
                CT_ContainerGlobalTimer[i][2] = 0;
                CT_ContainerGlobalTimer[i][3] = 0;
                strclear(CT_ContainerGlobalTimerName[i]);
            }
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 运行全局定时器
//--------------------------------------------------------------------
void globaltimer_run(char *timer)
{
    int i;
    for(i=0;i<CT_ContainerGlobalTimerLength;i++) //遍历全局定时器容器
    {
        if(CT_ContainerGlobalTimer[i][0] != NULL) //如果当前全局定时器不为空
        {
            char *globaltimer_name = CT_ContainerGlobalTimerName[i]; //获取当前全局定时器名称
            if(strcmp(globaltimer_name,timer) == 0) //如果当前全局定时器名称与参数全局定时器名称一致
            {
                CT_ContainerGlobalTimer[i][3] = 1; //运行指定全局定时器
                return; //中断
            }
        }
    }
    cprint("ERROR : Timer invalid.");
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 停止全局定时器
//--------------------------------------------------------------------
void globaltimer_stop(char *timer)
{
    int i;
    for(i=0;i<CT_ContainerGlobalTimerLength;i++) //遍历全局定时器容器
    {
        if(CT_ContainerGlobalTimer[i][0] != NULL) //如果当前全局定时器不为空
        {
            char *globaltimer_name = CT_ContainerGlobalTimerName[i]; //获取当前全局定时器名称
            if(strcmp(globaltimer_name,timer) == 0) //如果当前全局定时器名称与参数全局定时器名称一致
            {
                int globaltimer_duration = CT_ContainerGlobalTimer[i][2]; //全局定时器时长
                CT_ContainerGlobalTimer[i][1] = globaltimer_duration; //重置全局定时器
                CT_ContainerGlobalTimer[i][3] = 0; //停止指定全局定时器
                return; //中断
            }
        }
    }
    cprint("ERROR : Timer invalid.");
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 暂停全局定时器
//--------------------------------------------------------------------
void globaltimer_pause(char *timer)
{
    int i;
    for(i=0;i<CT_ContainerGlobalTimerLength;i++) //遍历全局定时器容器
    {
        if(CT_ContainerGlobalTimer[i][0] != NULL) //如果当前全局定时器不为空
        {
            char *globaltimer_name = CT_ContainerGlobalTimerName[i]; //获取当前全局定时器名称
            if(strcmp(globaltimer_name,timer) == 0) //如果当前全局定时器名称与参数全局定时器名称一致
            {
                CT_ContainerGlobalTimer[i][3] = 0; //停止指定全局定时器
                return; //中断
            }
        }
    }
    cprint("ERROR : Timer invalid.");
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 检查全局定时器是否结束
//--------------------------------------------------------------------
int globaltimer_end(char *timer)
{
    int i;
    for(i=0;i<CT_ContainerGlobalTimerLength;i++) //遍历全局定时器容器
    {
        if(CT_ContainerGlobalTimer[i][0] != NULL) //如果当前全局定时器不为空
        {
            char *globaltimer_name = CT_ContainerGlobalTimerName[i]; //获取当前全局定时器名称
            if(strcmp(globaltimer_name,timer) == 0) //如果当前全局定时器名称与参数全局定时器名称一致
            {
                if(CT_ContainerGlobalTimer[i][3] == 2) //如果指定全局定时器结束
                {
                    return 1;
                }
                return 0;
            }
        }
    }
    cprint("ERROR : Timer invalid.");
    return 0;
}
//--------------------------------------------------------------------