#define _CRT_SECURE_NO_WARNINGS

#include "CanvasTableSYS.h"

extern int *CT_ContainerTimer;
extern char *CT_ContainerTimerName;
extern int CT_ContainerTimerLength;
extern int *CT_ContainerGlobalTimer;
extern char *CT_ContainerGlobalTimerName;
extern int CT_ContainerGlobalTimerLength;
extern int CT_ContainerMaxLength_Timer;
extern int CT_ContainerMaxLength_GlobalTimer;
//--------------------------------------------------------------------
// 定时器新建
//--------------------------------------------------------------------
void timer_set(char *timer,float time)
{
    int i;
    for(i=0;i<CT_ContainerTimerLength;i++) //遍历定时器容器
    {
        if(CT_ContainerTimer[i*4] != 0) //如果当前定时器不为空
        {
            char *timer_name = CT_ContainerTimerName + i * 129; //获取当前定时器名称
            if(strcmp(timer_name,timer) == 0) //如果当前定时器名称与参数定时器名称一致
            {
                cprint("ERROR : Timer have been create.");
                return; //中断
            }
        }
    }
    if(time >= 0.1) //如果定时器时间大于等于0.1
    {
        int time_frame = (int)(time * 60); //计算时间帧
        char *timer_name = NULL; //定时器名称指针
        int i2;
        size_t data_i;
        for(i2=0;i2<CT_ContainerTimerLength;i2++) //遍历定时器容器
        {
            data_i = i2 * 4; //定时器数据索引
            if(CT_ContainerTimer[data_i] == 0) //如果当前定时器为空
            {
                timer_name = CT_ContainerTimerName + i2 * 129; //获取定时器名称指针
                //将定时器数据写入定时器容器
                strcpy(timer_name,timer);
                CT_ContainerTimer[data_i] = 1;
                CT_ContainerTimer[data_i+1] = time_frame;
                CT_ContainerTimer[data_i+2] = time_frame;
                CT_ContainerTimer[data_i+3] = 0;
                return; //中断
            }
        }
        data_i = CT_ContainerTimerLength * 4; //定时器数据索引
        timer_name = CT_ContainerTimerName + CT_ContainerTimerLength * 129; //获取定时器名称指针
        //将定时器数据写入定时器容器
        strcpy(timer_name,timer);
        CT_ContainerTimer[data_i] = 1;
        CT_ContainerTimer[data_i+1] = time_frame;
        CT_ContainerTimer[data_i+2] = time_frame;
        CT_ContainerTimer[data_i+3] = 0;
        CT_ContainerTimerLength ++; //定时器容器长度 +1
        if(CT_ContainerTimerLength == CT_ContainerMaxLength_Timer) //如果定时器容器长度等于定时器最大长度
        {
            CT_ContainerPreallocated_Timer(CT_ContainerTimerLength+300); //为定时器容器重新分配内存
        }
    }
    else
    {
        cprint("ERROR : The timer duration than minimum.");
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 帧定时器新建
//--------------------------------------------------------------------
void timer_set_f(char *timer,int fn)
{
    int i;
    for(i=0;i<CT_ContainerTimerLength;i++) //遍历定时器容器
    {
        if(CT_ContainerTimer[i*4] != 0) //如果当前定时器不为空
        {
            char *timer_name = CT_ContainerTimerName + CT_ContainerTimerLength * 129; //获取当前定时器名称
            if(strcmp(timer_name,timer) == 0) //如果当前定时器名称与参数定时器名称一致
            {
                cprint("ERROR : The timer have been create.");
                return; //中断
            }
        }
    }
    if(fn >= 1) //如果定时器帧大于等于1
    {
        int i2;
        size_t data_i;
        char *timer_name = NULL; //定时器名称指针
        for(i2=0;i2<CT_ContainerTimerLength;i2++) //遍历定时器容器
        {
            data_i = i2 * 4; //定时器数据索引
            if(CT_ContainerTimer[data_i] == 0) //如果当前定时器为空
            {
                timer_name = CT_ContainerTimerName + i2 * 129; //获取定时器名称指针
                //将定时器数据写入定时器容器
                strcpy(timer_name,timer);
                CT_ContainerTimer[data_i] = 1;
                CT_ContainerTimer[data_i+1] = fn;
                CT_ContainerTimer[data_i+2] = fn;
                CT_ContainerTimer[data_i+3] = 0;
                return; //中断
            }
        }
        data_i = CT_ContainerTimerLength * 4; //定时器数据索引
        timer_name = CT_ContainerTimerName + CT_ContainerTimerLength * 129; //获取定时器名称指针
        //将定时器数据写入定时器容器
        strcpy(timer_name,timer);
        CT_ContainerTimer[data_i] = 1;
        CT_ContainerTimer[data_i+1] = fn;
        CT_ContainerTimer[data_i+2] = fn;
        CT_ContainerTimer[data_i+3] = 0;
        CT_ContainerTimerLength ++; //定时器容器长度 +1
        if(CT_ContainerTimerLength == CT_ContainerMaxLength_Timer) //如果定时器容器长度等于定时器最大长度
        {
            CT_ContainerPreallocated_Timer(CT_ContainerTimerLength+300); //为定时器容器重新分配内存
        }
    }
    else
    {
        cprint("ERROR : The timer duration than minimum.");
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 定时器清除
//--------------------------------------------------------------------
void timer_clear(char *timer)
{
    int i;
    size_t data_i;
    for(i=0;i<CT_ContainerTimerLength;i++) //遍历定时器容器
    {
        data_i = i * 4; //定时器数据索引
        if(CT_ContainerTimer[data_i] != 0) //如果当前定时器不为空
        {
            char *timer_name = CT_ContainerTimerName + i * 129; //获取当前定时器名称
            if(strcmp(timer_name,timer) == 0) //如果当前定时器名称与参数定时器名称一致
            {
                //清除定时器容器数据
                CT_ContainerTimer[data_i] = 0;
                CT_ContainerTimer[data_i+1] = 0;
                CT_ContainerTimer[data_i+2] = 0;
                CT_ContainerTimer[data_i+3] = 0;
                strclear(timer_name);
                for(i=CT_ContainerTimerLength-1;i>=-1;i--) //反向遍历定时器容器
                {
                    if(i >= 0) //如果定时器索引大于等于0
                    {
                        if(CT_ContainerTimer[i*4] != 0) //如果当前定时器不为空
                        {
                            CT_ContainerTimerLength = i + 1; //确定定时器容器长度
                            break; //中断
                        }
                    }
                    else
                    {
                        CT_ContainerTimerLength = 0; //定时器容器长度为0
                    }
                }
                break;
            }
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 定时器运行
//--------------------------------------------------------------------
void timer_run(char *timer)
{
    int i;
    size_t data_i;
    for(i=0;i<CT_ContainerTimerLength;i++) //遍历定时器容器
    {
        data_i = i * 4; //定时器数据索引
        if(CT_ContainerTimer[data_i] != 0) //如果当前定时器不为空
        {
            char *timer_name = CT_ContainerTimerName + i * 129; //获取当前定时器名称
            if(strcmp(timer_name,timer) == 0) //如果当前定时器名称与参数定时器名称一致
            {
                CT_ContainerTimer[data_i+3] = 1; //运行指定定时器
                return; //中断
            }
        }
    }
    cprint("ERROR : Timer invalid.");
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 定时器停止
//--------------------------------------------------------------------
void timer_stop(char *timer)
{
    int i;
    size_t data_i;
    for(i=0;i<CT_ContainerTimerLength;i++) //遍历定时器容器
    {
        data_i = i * 4; //定时器数据索引
        if(CT_ContainerTimer[data_i] != 0) //如果当前定时器不为空
        {
            char *timer_name = CT_ContainerTimerName + i * 129; //获取当前定时器名称
            if(strcmp(timer_name,timer) == 0) //如果当前定时器名称与参数定时器名称一致
            {
                int timer_duration = CT_ContainerTimer[data_i+2]; //定时器时长
                CT_ContainerTimer[data_i+1] = timer_duration; //重置定时器
                CT_ContainerTimer[data_i+3] = 0; //停止指定定时器
                return; //中断
            }
        }
    }
    cprint("ERROR : Timer invalid.");
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 定时器暂停
//--------------------------------------------------------------------
void timer_pause(char *timer)
{
    int i;
    size_t data_i;
    for(i=0;i<CT_ContainerTimerLength;i++) //遍历定时器容器
    {
        data_i = i * 4;
        if(CT_ContainerTimer[data_i] != 0) //如果当前定时器不为空
        {
            char *timer_name = CT_ContainerTimerName + i * 129; //获取当前定时器名称
            if(strcmp(timer_name,timer) == 0) //如果当前定时器名称与参数定时器名称一致
            {
                CT_ContainerTimer[data_i+3] = 0; //停止指定定时器
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
    size_t data_i;
    for(i=0;i<CT_ContainerTimerLength;i++) //遍历定时器容器
    {
        data_i = i * 4; //定时器数据索引
        if(CT_ContainerTimer[data_i] != 0) //如果当前定时器不为空
        {
            char *timer_name = CT_ContainerTimerName + i * 129; //获取当前定时器名称
            if(strcmp(timer_name,timer) == 0) //如果当前定时器名称与参数定时器名称一致
            {
                if(CT_ContainerTimer[data_i+3] == 2) //如果指定定时器结束
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
// 全局定时器新建
//--------------------------------------------------------------------
void globaltimer_set(char *timer,float time)
{
    int i;
    for(i=0;i<CT_ContainerGlobalTimerLength;i++) //遍历全局定时器容器
    {
        if(CT_ContainerGlobalTimer[i*4] != 0) //如果当前全局定时器不为空
        {
            char *globaltimer_name = CT_ContainerGlobalTimerName + i * 129; //获取当前全局定时器名称
            if(strcmp(globaltimer_name,timer) == 0) //如果当前全局定时器名称与参数全局定时器名称一致
            {
                cprint("ERROR : Timer have been create.");
                return; //中断
            }
        }
    }
    if(time >= 0.1) //如果全局定时器时间大于等于0.1
    {
        int time_frame = (int)(time * 60); //计算时间帧
        char *globaltimer_name = NULL; //全局定时器名称指针
        int i2;
        size_t data_i;
        for(i2=0;i2<CT_ContainerGlobalTimerLength;i2++) //遍历全局定时器容器
        {
            data_i = i2 * 4; //全局定时器数据索引
            if(CT_ContainerGlobalTimer[data_i] == 0) //如果当前全局定时器为空
            {
                globaltimer_name = CT_ContainerGlobalTimerName + i2 * 129; //全局定时器名称指针
                //将全局定时器数据写入全局定时器容器
                strcpy(globaltimer_name,timer);
                CT_ContainerGlobalTimer[data_i] = 1;
                CT_ContainerGlobalTimer[data_i+1] = time_frame;
                CT_ContainerGlobalTimer[data_i+2] = time_frame;
                CT_ContainerGlobalTimer[data_i+3] = 0;
                return; //中断
            }
        }
        data_i = CT_ContainerGlobalTimerLength * 4; //全局定时器数据索引
        globaltimer_name = CT_ContainerGlobalTimerName + CT_ContainerGlobalTimerLength * 129; //获取当前全局定时器名称
        //将全局定时器数据写入全局定时器容器
        strcpy(globaltimer_name,timer);
        CT_ContainerGlobalTimer[data_i] = 1;
        CT_ContainerGlobalTimer[data_i+1] = time_frame;
        CT_ContainerGlobalTimer[data_i+2] = time_frame;
        CT_ContainerGlobalTimer[data_i+3] = 0;
        CT_ContainerGlobalTimerLength ++; //全局定时器容器长度 +1
        if(CT_ContainerGlobalTimerLength == CT_ContainerMaxLength_GlobalTimer) //如果全局定时器容器长度等于全局定时器容器最大长度
        {
            CT_ContainerPreallocated_Timer(CT_ContainerGlobalTimerLength+300); //为全局定时器容器重新分配内存
        }
    }
    else
    {
        cprint("ERROR : The globaltimer duration than the minimum.");
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 帧全局定时器新建
//--------------------------------------------------------------------
void globaltimer_set_f(char *timer,int fn)
{
    int i;
    for(i=0;i<CT_ContainerGlobalTimerLength;i++) //遍历全局定时器容器
    {
        if(CT_ContainerGlobalTimer[i*4] != 0) //如果当前全局定时器不为空
        {
            char *globaltimer_name = CT_ContainerGlobalTimerName + CT_ContainerGlobalTimerLength * 129; //获取当前全局定时器名称
            if(strcmp(globaltimer_name,timer) == 0) //如果当前全局定时器名称与参数全局定时器名称一致
            {
                cprint("ERROR : The globaltimer have been create.");
                return; //中断
            }
        }
    }
    if(fn >= 1) //如果全局定时器时间大于等于0.017
    {
        int i2;
        size_t data_i;
        char *globaltimer_name = NULL; //全局定时器名称指针
        for(i2=0;i2<CT_ContainerGlobalTimerLength;i2++) //遍历全局定时器容器
        {
            data_i = i2 * 4;
            if(CT_ContainerGlobalTimer[data_i] == 0) //如果当前全局定时器为空
            {
                globaltimer_name = CT_ContainerGlobalTimerName + i2 * 129; //全局定时器名称指针
                //将全局定时器数据写入全局定时器容器
                strcpy(globaltimer_name,timer);
                CT_ContainerGlobalTimer[data_i] = 1;
                CT_ContainerGlobalTimer[data_i+1] = fn;
                CT_ContainerGlobalTimer[data_i+2] = fn;
                CT_ContainerGlobalTimer[data_i+3] = 0;
                return; //中断
            }
        }
        data_i = CT_ContainerGlobalTimerLength * 4; //全局定时器数据索引
        globaltimer_name = CT_ContainerGlobalTimerName + CT_ContainerGlobalTimerLength * 129; //全局定时器名称指针
        //将全局定时器数据写入全局定时器容器
        strcpy(globaltimer_name,timer);
        CT_ContainerGlobalTimer[data_i] = 1;
        CT_ContainerGlobalTimer[data_i+1] = fn;
        CT_ContainerGlobalTimer[data_i+2] = fn;
        CT_ContainerGlobalTimer[data_i+3] = 0;
        CT_ContainerGlobalTimerLength ++; //全局定时器容器长度 +1
        if(CT_ContainerGlobalTimerLength == CT_ContainerMaxLength_GlobalTimer) //如果全局定时器容器长度等于全局定时器容器最大长度
        {
            CT_ContainerPreallocated_Timer(CT_ContainerGlobalTimerLength+300); //为全局定时器容器重新分配内存
        }
    }
    else
    {
        cprint("ERROR : The timer duration than minimum.");
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 全局定时器清除
//--------------------------------------------------------------------
void globaltimer_clear(char *timer)
{
    int i;
    size_t data_i;
    for(i=0;i<CT_ContainerGlobalTimerLength;i++) //遍历全局定时器容器
    {
        data_i = i * 4; //全局定时器数据索引
        if(CT_ContainerGlobalTimer[data_i] != 0) //如果当前全局定时器不为空
        {
            char *globaltimer_name = CT_ContainerGlobalTimerName + i * 129; //获取当前全局定时器名称
            if(strcmp(globaltimer_name,timer) == 0) //如果当前全局定时器名称与参数全局定时器名称一致
            {
                //清除全局定时器容器数据
                CT_ContainerGlobalTimer[data_i] = 0;
                CT_ContainerGlobalTimer[data_i+1] = 0;
                CT_ContainerGlobalTimer[data_i+2] = 0;
                CT_ContainerGlobalTimer[data_i+3] = 0;
                strclear(globaltimer_name);
                for(i=CT_ContainerGlobalTimerLength-1;i>=-1;i--) //反向遍历全局定时器容器
                {
                    if(i >= 0) //如果全局定时器索引大于等于0
                    {
                        if(CT_ContainerGlobalTimer[i*4] != 0) //如果当前全局定时器不为空
                        {
                            CT_ContainerGlobalTimerLength = i + 1; //确定全局定时器容器长度
                            break; //中断
                        }
                    }
                    else
                    {
                        CT_ContainerGlobalTimerLength = 0; //全局定时器容器长度为0
                    }
                }
                break;
            }
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 全局定时器运行
//--------------------------------------------------------------------
void globaltimer_run(char *timer)
{
    int i;
    size_t data_i;
    for(i=0;i<CT_ContainerGlobalTimerLength;i++) //遍历全局定时器容器
    {
        data_i = i * 4; //全局定时器数据索引
        if(CT_ContainerGlobalTimer[data_i] != 0) //如果当前全局定时器不为空
        {
            char *globaltimer_name = CT_ContainerGlobalTimerName + i * 129; //获取当前全局定时器名称
            if(strcmp(globaltimer_name,timer) == 0) //如果当前全局定时器名称与参数全局定时器名称一致
            {
                CT_ContainerGlobalTimer[data_i+3] = 1; //运行指定全局定时器
                return; //中断
            }
        }
    }
    cprint("ERROR : Timer invalid.");
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 全局定时器停止
//--------------------------------------------------------------------
void globaltimer_stop(char *timer)
{
    int i;
    size_t data_i;
    for(i=0;i<CT_ContainerGlobalTimerLength;i++) //遍历全局定时器容器
    {
        data_i = i * 4; //全局定时器数据索引
        if(CT_ContainerGlobalTimer[data_i] != 0) //如果当前全局定时器不为空
        {
            char *globaltimer_name = CT_ContainerGlobalTimerName + i * 129; //获取当前全局定时器名称
            if(strcmp(globaltimer_name,timer) == 0) //如果当前全局定时器名称与参数全局定时器名称一致
            {
                int globaltimer_duration = CT_ContainerGlobalTimer[data_i+2]; //全局定时器时长
                CT_ContainerGlobalTimer[data_i+1] = globaltimer_duration; //重置全局定时器
                CT_ContainerGlobalTimer[data_i+3] = 0; //停止指定全局定时器
                return; //中断
            }
        }
    }
    cprint("ERROR : Timer invalid.");
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 全局定时器暂停
//--------------------------------------------------------------------
void globaltimer_pause(char *timer)
{
    int i;
    size_t data_i;
    for(i=0;i<CT_ContainerGlobalTimerLength;i++) //遍历全局定时器容器
    {
        data_i = i * 4; //全局定时器数据索引
        if(CT_ContainerGlobalTimer[data_i] != 0) //如果当前全局定时器不为空
        {
            char *globaltimer_name = CT_ContainerGlobalTimerName + i * 129; //获取当前全局定时器名称
            if(strcmp(globaltimer_name,timer) == 0) //如果当前全局定时器名称与参数全局定时器名称一致
            {
                CT_ContainerGlobalTimer[data_i+3] = 0; //停止指定全局定时器
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
    size_t data_i;
    for(i=0;i<CT_ContainerGlobalTimerLength;i++) //遍历全局定时器容器
    {
        data_i = i * 4; //全局定时器数据索引
        if(CT_ContainerGlobalTimer[data_i] != 0) //如果当前全局定时器不为空
        {
            char *globaltimer_name = CT_ContainerGlobalTimerName + i * 129; //获取当前全局定时器名称
            if(strcmp(globaltimer_name,timer) == 0) //如果当前全局定时器名称与参数全局定时器名称一致
            {
                if(CT_ContainerGlobalTimer[data_i+3] == 2) //如果指定全局定时器结束
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