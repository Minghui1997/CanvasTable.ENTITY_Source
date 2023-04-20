#include "CanvasTableSYS.h"

int *CT_ContainerTimer = NULL; //定时器容器  格式:[占位符,定时器时间,定时器时长,定时器状态,......]
char *CT_ContainerTimerName = NULL; //定时器名称列表
int CT_ContainerTimerLength = 0; //定时器容器长度
int CT_ContainerMaxLength_Timer = 0; //定时器容器最大长度
int *CT_ContainerGlobalTimer = NULL; //全局定时器容器  格式:[占位符,定时器时间,定时器时长,定时器状态,......]
char *CT_ContainerGlobalTimerName = NULL; //全局定时器名称列表
int CT_ContainerGlobalTimerLength = 0; //全局定时器容器长度
int CT_ContainerMaxLength_GlobalTimer = 0; //全局定时器容器最大长度

//--------------------------------------------------------------------
// 定时器运行
//--------------------------------------------------------------------
void CT_TimerRun()
{
    int i;
    size_t data_i;
    for(i=0;i<CT_ContainerTimerLength;i++) //查找整个定时器列表
    {
        data_i = i * 4; //定时器数据索引
        if(CT_ContainerTimer[data_i] != 0) //如果当前定时器不为空
         {
            if(CT_ContainerTimer[data_i+3] == 1) //如果当前定时器状态为1
            {
                CT_ContainerTimer[data_i+1] -= 1; //当前定时器的时间帧 -1
                if(CT_ContainerTimer[data_i+1] == 0) //如果当前定时器定时结束
                {
                    int timer_duration = CT_ContainerTimer[data_i+2]; //定时器时长
                    CT_ContainerTimer[data_i+3] = 2; //将当前定时器的状态标记为结束
                    CT_ContainerTimer[data_i+1] = timer_duration; //重置定时器
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
    size_t data_i;
    for(i=0;i<CT_ContainerGlobalTimerLength;i++) //查找整个定时器列表
    {
        data_i = i * 4; //定时器数据索引
        if(CT_ContainerGlobalTimer[data_i] != 0) //如果当前定时器不为空
        {
            if(CT_ContainerGlobalTimer[data_i+3] == 1) //如果当前定时器状态为1
            {
                CT_ContainerGlobalTimer[data_i+1] -= 1; //当前定时器的时间帧 -1
                if(CT_ContainerGlobalTimer[data_i+1] == 0) //如果当前定时器定时结束
                {
                    int timer_duration = CT_ContainerGlobalTimer[data_i+2]; //定时器时长
                    CT_ContainerGlobalTimer[data_i+3] = 2; //将当前定时器的状态标记为结束
                    CT_ContainerGlobalTimer[data_i+1] = timer_duration; //重置定时器
                }
            }
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 定时器容器预先分配
//--------------------------------------------------------------------
void CT_ContainerPreallocated_Timer(int max_l)
{
    size_t ct_cap = sizeof(int) * 4 * max_l; //定时器容器容量
    size_t ctn_cap = sizeof(char) * 129 * max_l; //定时器名称容器容量
    if(CT_ContainerMaxLength_Timer == 0) //如果定时器容器最大长度为0
    {
        CT_ContainerTimer = malloc(ct_cap); //定时器容器分配内存
        CT_ContainerTimerName = malloc(ctn_cap); //定时器名称容器分配内存
        CT_ContainerMaxLength_Timer = max_l; //设置定时器容器最大长度
    }
    if(CT_ContainerMaxLength_Timer > 0 && max_l > CT_ContainerMaxLength_Timer) //如果定时器容器长度大于定时器容器最大长度
    {
        size_t ct_nbs = ct_cap - sizeof(int) * 4 * CT_ContainerMaxLength_Timer; //定时器容器的新内存块大小
        size_t ctn_nbs = ctn_cap - sizeof(char) * 129 * CT_ContainerMaxLength_Timer; //定时器名称容器的新内存块大小
        CT_ContainerTimer = realloc(CT_ContainerTimer,ct_cap); //定时器容器重新分配内存
        memset(CT_ContainerTimer+CT_ContainerMaxLength_Timer*4,0x00,ct_nbs); //对定时器容器的新空间填0
        CT_ContainerTimerName = realloc(CT_ContainerTimerName,ctn_cap); //定时器名称容器重新分配内存
        memset(CT_ContainerTimerName+CT_ContainerMaxLength_Timer*129,0x00,ctn_nbs); //对定时器名称容器的新空间填0
        CT_ContainerMaxLength_Timer = max_l; //设置定时器容器最大长度
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 全局定时器容器预先分配
//--------------------------------------------------------------------
void CT_ContainerPreallocated_GlobalTimer(int max_l)
{
    size_t cgt_cap = sizeof(int) * 4 * max_l; //全局定时器容器容量
    size_t cgtn_cap = sizeof(char) * 129 * max_l; //全局定时器名称容器容量
    if(CT_ContainerMaxLength_GlobalTimer == 0) //如果全局定时器容器最大长度为0
    {
        CT_ContainerGlobalTimer = malloc(cgt_cap); //全局定时器容器分配内存
        CT_ContainerGlobalTimerName = malloc(cgtn_cap); //全局定时器名称容器分配内存
        CT_ContainerMaxLength_GlobalTimer = max_l; //设置全局定时器容器最大长度
    }
    if(CT_ContainerMaxLength_GlobalTimer > 0 && max_l > CT_ContainerMaxLength_GlobalTimer) //如果全局定时器容器长度大于定时器容器最大长度
    {
        size_t cgt_nbs = cgt_cap - sizeof(int) * 4 * CT_ContainerMaxLength_GlobalTimer; //全局定时器容器的新内存块大小
        size_t cgtn_nbs = cgtn_cap - sizeof(char) * 129 * CT_ContainerMaxLength_GlobalTimer; //全局定时器名称容器的新内存块大小
        CT_ContainerGlobalTimer = realloc(CT_ContainerGlobalTimer,cgt_cap); //全局定时器容器重新分配内存
        memset(CT_ContainerGlobalTimer+CT_ContainerMaxLength_GlobalTimer*4,0x00,cgt_nbs); //对全局定时器容器的新空间填0
        CT_ContainerGlobalTimerName = realloc(CT_ContainerGlobalTimerName,cgtn_cap); //全局定时器名称容器重新分配内存
        memset(CT_ContainerGlobalTimerName+CT_ContainerMaxLength_GlobalTimer*129,0x00,cgtn_nbs); //对全局定时器名称容器的新空间填0
        CT_ContainerMaxLength_GlobalTimer = max_l; //设置全局定时器容器最大长度
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 定时器容器清除
//--------------------------------------------------------------------
void CT_ContainerTimerClear()
{
    int i;
    int i2;
    size_t data_i;
    for(i=0;i<CT_ContainerTimerLength;i++) //遍历定时器容器
    {
        data_i = i * 4; //定时器数据索引
        if(CT_ContainerTimer[data_i] != 0) //如果当前定时器不为空
        {
            for(i2=0;i2<4;i2++) //遍历定时器数据
            {
                CT_ContainerTimer[data_i+i2] = 0; //清除定时器数据
            }
            strclear(CT_ContainerTimerName+i*129); //清除定时器名称
        }
    }
    CT_ContainerTimerLength = 0; //定时器容器长度为0
}
//--------------------------------------------------------------------