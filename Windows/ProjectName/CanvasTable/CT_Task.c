#define _CRT_SECURE_NO_WARNINGS

#include "CanvasTableSYS.h"

void (**CT_ContainerTask)(); //子程序容器
long long *CT_ContainerTaskNum = NULL; //子程序数字变量容器
char *CT_ContainerTaskStr = NULL; //子程序字符串变量容器
int *CT_TaskStatusList = NULL; //子程序状态列表
int CT_ContainerTaskLength = 0; //子程序容器长度
int CT_ContainerMaxLength_Task = 0; //子程序容器最大长度
int CT_TaskRunIndex = 0; //子程序运行索引

//--------------------------------------------------------------------
// 子程序创建
//--------------------------------------------------------------------
void CT_TaskCreate(int task_i,void *task_m,void *task_l)
{
    size_t data_i = task_i * 2; //子程序数据索引
    if(task_i >= CT_ContainerMaxLength_Task - 1) //如果子程序索引大于等于子程序容器最大长度
    {
        CT_ContainerPreallocated_Task(task_i+1+100); //预先分配子程序容器
    }
    CT_ContainerTask[data_i] = task_m; //将子程序的主函数指针存入容器
    CT_ContainerTask[data_i+1] = task_l; //将子程序的循环函数指针存入容器
    CT_TaskStatusList[task_i] = 1; //将子程序状态设置为等待
    if(task_i >= CT_ContainerTaskLength) //如果子程序索引大于等于子程序容器长度
    {
        CT_ContainerTaskLength = task_i + 1; //确定子程序长度
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 子程序销毁
//--------------------------------------------------------------------
void CT_TaskDestroy(int ti)
{
    if(ti < CT_ContainerTaskLength) //如果子程序索引对应的子程序不为空
    {
        size_t data_i = ti * 256; //子程序数据索引
        int i;
        int i2;
        CT_ContainerTask[ti*2] = NULL; //清除子程序主函数指针
        CT_ContainerTask[ti*2+1] = NULL; //清除子程序循环函数指针
        CT_TaskStatusList[ti] = 0; //子程序状态为0
        for(i=0;i<256;i++) //遍历该子程序的变量数组
        {
            char *vars_p = CT_ContainerTaskStr + data_i + i * 129; //字符串变量指针
            CT_ContainerTaskNum[data_i+i] = 0; //清除数字变量
            strclear(vars_p); //清除字符串变量
        }
        for(i2=CT_ContainerTaskLength-1;i2>=-1;i2--) //反向遍历子程序容器
        {
            if(i2 >= 0) //如果子程序索引大于0
            {
                if(CT_TaskStatusList[i2] != 0) //如果当前子程序不为空
                {
                    CT_ContainerTaskLength = i2 + 1; //确定子程序容器长度
                    break; //中断
                }
            }
            else
            {
                CT_ContainerTaskLength = 0; //子程序容器长度为0
            }
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 子程序变量赋值 数字
//--------------------------------------------------------------------
void CT_TaskSetNum(int ti,int vi,varn n)
{
    if(ti < CT_ContainerTaskLength) //如果子程序索引对应的子程序不为空
    {
        if(vi < 256) //如果变量索引小于256
        {
            CT_ContainerTaskNum[ti * 256 + vi] = n; //数字变量赋值
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 子程序变量 数字
//--------------------------------------------------------------------
varn CT_TaskNum(int ti,int vi)
{
    if(ti < CT_ContainerTaskLength) //如果子程序索引对应的子程序不为空
    {
        if(vi < 256) //如果变量索引小于256
        {
            return CT_ContainerTaskNum[ti * 256 + vi]; //获取数字变量
        }
    }
    return 0;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 子程序变量赋值 小数
//--------------------------------------------------------------------
void CT_TaskSetDec(int ti,int vi,vard d)
{
    if(ti < CT_ContainerTaskLength) //如果子程序索引对应的子程序不为空
    {
        if(vi < 256) //如果变量索引小于256
        {
            CT_ContainerTaskNum[ti * 256 + vi] = *(long long*) & d; //小数变量赋值
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 子程序变量 小数
//--------------------------------------------------------------------
vard CT_TaskDec(int ti,int vi)
{
    if(ti < CT_ContainerTaskLength) //如果子程序索引对应的子程序不为空
    {
        if(vi < 256) //如果变量索引小于256
        {
            return *(double*) & CT_ContainerTaskNum[ti * 256 + vi]; //获取小数变量
        }
    }
    return 0;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 子程序变量赋值 字符串
//--------------------------------------------------------------------
void CT_TaskSetStr(int ti,int vi,char *s)
{
    if(ti < CT_ContainerTaskLength) //如果子程序索引对应的子程序不为空
    {
        if(vi < 256)
        {
            strcpy(CT_ContainerTaskStr + ti * 256 + vi * 129,s); //复制字符串
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 程序变量 字符串
//--------------------------------------------------------------------
char *CT_TaskStr(int ti,int vi)
{
    if(ti < CT_ContainerTaskLength) //如果子程序索引对应的子程序不为空
    {
        if(vi < 256)
        {
            return CT_ContainerTaskStr + ti * 256 + vi * 129; //获取字符串变量指针
        }
    }
    return NULL;
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
        if(CT_TaskStatusList[i] == 1) //如果当前子程序状态为准备
        {
            CT_TaskStatusList[i] = 2; //将当前子程序标记为运行
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 运行子程序主函数
//--------------------------------------------------------------------
void CT_TaskRunMain()
{
    int i;
    size_t data_i;
    for(i=0;i<CT_ContainerTaskLength;i++) //遍历子程序容器
    {
        data_i = i * 2; //子程序函数索引
        if(CT_TaskStatusList[i] == 2) //如果当前子程序状态为运行
        {
            if(CT_ContainerTask[data_i] != NULL) //如果当前子程序主函数指针不为空
            {
                CT_TaskRunIndex = i; //子程序运行索引
                CT_ContainerTask[data_i](); //执行子程序main函数
                CT_TaskStatusList[i] = 3; //将当前子程序状态设置为循环
            }
        }
    }
    CT_TaskRunIndex = 0; //子程序运行索引
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 运行子程序循环函数
//--------------------------------------------------------------------
void CT_TaskRunLoop()
{
    int i;
    size_t data_i;
    for(i=0;i<CT_ContainerTaskLength;i++) //遍历子程序容器
    {
        data_i = i * 2; //子程序函数索引
        if(CT_TaskStatusList[i] == 3) //如果当前子程序状态为循环
        {
            if(CT_ContainerTask[data_i+1] != NULL) //如果当前子程序循环函数指针不为空
            {
                CT_TaskRunIndex = i; //子程序运行索引
                CT_ContainerTask[data_i+1](); //执行子程序循环函数
            }
        }
    }
    CT_TaskRunIndex = 0; //子程序运行索引
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 子程序容器预先分配
//--------------------------------------------------------------------
void CT_ContainerPreallocated_Task(int max_l)
{
    size_t ctf_cap = sizeof(void*) * 2 * max_l; //子程序容器容量
    size_t tsl_cap = sizeof(int) * max_l; //子程序状态列表容量
    size_t ctn_cap = sizeof(long long) * 256 * max_l; //子程序数字变量容器
    size_t cts_cap = sizeof(char) * 128 * 256 * max_l; //子程序字符串容器容量
    if(CT_ContainerMaxLength_Task == 0) //如果子程序容器最大长度为0
    {
        CT_ContainerTask = malloc(ctf_cap); //子程序容器分配内存
        memset(CT_ContainerTask,0x00,ctf_cap); //子程序容器填0
        CT_TaskStatusList = malloc(tsl_cap); //子程序状态列表分配内存
        memset(CT_TaskStatusList,0x00,tsl_cap); //子程序状态列表填0
        CT_ContainerTaskNum = malloc(ctn_cap); //子程序数字变量容器分配内存
        memset(CT_ContainerTaskNum,0x00,ctn_cap); //子程序数字变量填0
        CT_ContainerTaskStr = malloc(cts_cap); //子程序字符串变量容器分配内存
        memset(CT_ContainerTaskStr,0x00,cts_cap); //子程序字符串容器填0
        CT_ContainerMaxLength_Task = max_l; //设置子程序容器最大长度
    }
    if(CT_ContainerMaxLength_Task > 0 && max_l > CT_ContainerMaxLength_Task) //如果子程序容器长度大于子程序容器最大长度
    {
        size_t ctf_nbs = ctf_cap - sizeof(void*) * 2 * CT_ContainerMaxLength_Task; //子程序容器新内存块大小
        size_t tsl_nbs = tsl_cap - sizeof(int) * CT_ContainerMaxLength_Task; //子程序状态列表新内存块大小
        size_t ctn_nbs = ctn_cap - sizeof(long long) * 256 * CT_ContainerMaxLength_Task; //子程序数字变量容器新内存块大小
        size_t cts_nbs = cts_cap - sizeof(char) * 128 * 256 * CT_ContainerMaxLength_Task; //子程序字符串变量容器新内存块大小
        CT_ContainerTask = realloc(CT_ContainerTask,ctf_cap); //子程序容器重新分配内存
        memset(CT_ContainerTask+CT_ContainerMaxLength_Task*2,0x00,ctf_nbs); //对子程序容器的新空间填0
        CT_TaskStatusList = realloc(CT_TaskStatusList,tsl_cap); //子程序状态列表重新分配内存
        memset(CT_TaskStatusList+CT_ContainerMaxLength_Task,0x00,tsl_nbs); //对子程序状态列表的新空间填0
        CT_ContainerTaskNum = realloc(CT_ContainerTaskNum,ctn_cap); //子程序数字变量容器重新分配内存
        memset(CT_ContainerTaskNum+CT_ContainerMaxLength_Task*256,0x00,ctn_nbs); //对子程序数字变量容器的新空间填0
        CT_ContainerTaskStr = realloc(CT_ContainerTaskStr,cts_cap); //子程序字符串变量容器重新分配内存
        memset(CT_ContainerTaskStr+CT_ContainerMaxLength_Task*128*256,0x00,cts_nbs); //对子程序字符串变量容器的新空间填0
        CT_ContainerMaxLength_Task = max_l; //设置子程序容器最大长度
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 子程序容器清除
//--------------------------------------------------------------------
void CT_ContainerTaskClear()
{
    int i;
    size_t data_i;
    int i2;
    for(i=0;i<CT_ContainerTaskLength;i++) //遍历子程序
    {
        if(CT_TaskStatusList[i] != 0) //如果当前子程序不为空
        {
            data_i = i * 256; //子程序数据索引
            CT_ContainerTask[data_i*2] = NULL; //清除子程序函数
            CT_ContainerTask[data_i*2+1] = NULL; //清除子程序函数
            CT_TaskStatusList[i] = 0; //子程序状态为0
            for(i2=0;i2<256;i2++) //遍历该子程序的变量数组
            {
                char *str_p = CT_ContainerTaskStr + data_i + i2 * 129; //字符串变量指针
                CT_ContainerTaskNum[data_i+i2] = 0; //清除数字变量
                strclear(str_p); //清除字符串变量
            }
        }
    }
    CT_ContainerTaskLength = 0; //子程序长度
    CT_TaskRunIndex = 0; //子程序运行索引
}
//--------------------------------------------------------------------