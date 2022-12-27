#include "CanvasTableSYS.h"
#include <stdio.h>
#include <string.h>

extern int **CT_ResourceArgList;
extern char **CT_ContainerResource;

//--------------------------------------------------------------------
// 读取文本数据(指针)
//--------------------------------------------------------------------
char *textdata_read(int t)
{
    if(t != -1) //如果资源不为空
    {
        int res_type = CT_ResourceArgList[t][1]; //资源类型
        if(res_type == 2) //如果资源是文本
        {
            return CT_ContainerResource[t]; //返回文本数据(指针)
        }
        else //如果资源不是文本
        {
            cprint("ERROR : The resource index corresponding to the resource file is not a text.");
        }
    }
    return NULL;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 获取文本数据行数
//--------------------------------------------------------------------
int textdata_line(int t)
{
    if(t != -1) //如果资源不为空
    {
        int res_type = CT_ResourceArgList[t][1]; //资源类型
        if(res_type == 2) //如果资源是文本
        {
            int line = 1; //文本行
            int text_l = strlen(CT_ContainerResource[t]); //获取文本长度
            char char_c = 0x00; //当前字符
            char char_c2 = 0x00; //当前字符的下一个字符
            int char_i = 0; //字符索引
            if(text_l == 0) //如果文本长度为0
            {
                return 0;
            }
            while(char_i < text_l) //当字符索引小于文本长度
            {
                char_c = CT_ContainerResource[t][char_i]; //获取当前字符
                char_c2 = CT_ContainerResource[t][char_i+1]; //获取当前字符的下一个字符
                if(char_c == 0x0d || char_c == 0x0a) //如果当前字符是换行符
                {
                    line ++; //文本行 +1
                    if(char_c2 == 0x0d || char_c2 == 0x0a) //如果当前字符的后一个字符是换行符
                    {
                        char_i ++; //字符索引 +1
                    }
                }
                char_i ++; //字符索引 +1
            }
            return line;
        }
        else //如果资源不是文本
        {
            cprint("ERROR : The resource index corresponding to the resource file is not a text.");
        }
    }
    return 0;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 读取文本数据指定行
//--------------------------------------------------------------------
int textdata_read_line(char *dest,int t,int l)
{
    if(t != -1) //如果资源不为空
    {
        int res_type = CT_ResourceArgList[t][1]; //资源类型
        if(res_type == 2) //如果资源是文本
        {
            int text_len = strlen(CT_ContainerResource[t]); //文本数据长度
            int line = 1; //文本行
            int char_i = 0; //字符索引
            int char_ls = 0; //文本行开始的索引
            int char_le = 0; //文本行结束的索引
            int char_n = 0; //文本行字符数
            char char_c = 0x00; //当前字符
            char char_c2 = 0x00; //当前字符的下一个字符
            if(text_len == 0) //如果文本数据长度为0
            {
                strcpy(dest,""); //获取空文本
                return 0; //中断
            }
            while(char_i < text_len) //当字符索引小于文本长度
            {
                char_c = CT_ContainerResource[t][char_i]; //获取当前字符
                char_c2 = CT_ContainerResource[t][char_i+1]; //获取当前字符的下一个字符
                if(char_c == 0x0d || char_c == 0x0a) //如果当前字符是换行符
                {
                    char_le = char_i; //文本行结束的索引等于当前字符索引
                    if(line == l) //如果当前文本行等于文本行(参数)
                    {
                        if(dest != NULL) //如果目标字符串不为空
                        {
                            strslice(dest,CT_ContainerResource[t],char_ls,char_le); //获取文本数据当前行的文本
                        }
                        char_n = char_le - char_ls; //文本行字符数
                        return char_n;
                    }
                    line ++; //文本行 +1
                    if(char_c2 == 0x0d || char_c2 == 0x0a) //如果当前字符的后一个字符是换行符
                    {
                        char_i ++; //字符索引 +1
                    }
                    char_ls = char_i + 1; //文本行开始的索引等于当前字符索引+1
                }
                if(char_i == text_len - 1) //如果当前字符索引是最后一个字符
                {
                    if(line == l) //如果当前文本行等于文本行(参数)
                    {
                        char_le = text_len; //文本行结束的索引为最后一个字符
                        if(dest != NULL) //如果目标字符串不为空
                        {
                            strslice(dest,CT_ContainerResource[t],char_ls,char_le); //获取文本数据当前行的文本
                        }
                        char_n = char_le - char_ls; //文本行字符数
                        return char_n;
                    }
                }
                char_i ++; //字符索引 +1
            }
            if(line == 1 && l == 1) //如果文本数据只有1行文本
            {
                if(dest != NULL) //如果目标字符串不为空
                {
                    strslice(dest,CT_ContainerResource[t],char_ls,char_le); //获取文本数据当前行的文本
                }
                char_n = char_le - char_ls; //文本行字符数
                return char_n;
            }
        }
        else //如果资源不是文本
        {
            cprint("ERROR : The resource index corresponding to the resource file is not a text.");
        }
    }
    if(dest != NULL) //如果目标字符串不为空
    {
        strcpy(dest,""); //获取空文本
    }
    return 0;
}
//--------------------------------------------------------------------