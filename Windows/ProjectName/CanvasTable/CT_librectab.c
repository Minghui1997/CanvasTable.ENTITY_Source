#define _CRT_SECURE_NO_WARNINGS

#include "CanvasTableSYS.h"
#include <shlwapi.h>
#pragma comment(lib,"Shlwapi.lib")

char *CT_RectabData = NULL; //当前记录表数据
int CT_RectabDataLength = 0; //记录表数据长度
int CT_RectabDataMaxLength = 0; //记录表数据最大长度
char CT_RectabName[257] = ""; //当前记录表名称
char CT_RectabPath[264] = "RECTAB/"; //当前记录表路径
FILE *CT_RectabFile = NULL; //当前记录表文件指针

//--------------------------------------------------------------------
// 记录表打开
//--------------------------------------------------------------------
void rectab_open(char *name)
{
    if(name[0] != 0x00 && charindex(name,'/') == -1 && charindex(name,'*') == -1 && charindex(name,':') == -1 && charindex(name,'?') == -1 && charindex(name,'<') == -1 && charindex(name,'>') == -1 && charindex(name,'|') == -1) //如果记录表名称格式正确
    {
        if(CT_RectabName[0] == 0x00) //如果没有打开记录表
        {
            strcpy(CT_RectabName,name); //复制记录表名称
            strcat(CT_RectabPath,name); //在记录表路径中添加记录表名称
            strcat(CT_RectabPath,".rec"); //在记录表路径中添加记录表扩展名
            if(PathFileExists("RECTAB") == 0) //如果应用程序所在目录中没有RECTAB文件夹
            {
                CreateDirectory("RECTAB",NULL); //创建RECTAB文件夹
            }
            CT_RectabFile = fopen(CT_RectabPath,"r+"); //打开记录表文件
            if(CT_RectabFile != NULL) //如果记录表文件夹中有记录表文件
            {
                int rectab_size = 0; //记录表大小
                fseek(CT_RectabFile,0,SEEK_END); //文件指针移至文件末尾
                rectab_size = ftell(CT_RectabFile) - 3; //获取记录表大小
                if(rectab_size > 0) //如果记录表大小大于0
                {
                    char *utf8_str = NULL; //记录表UTF-8字符串
                    utf8_str = malloc(rectab_size+1); //为记录表UTF-8字符串分配内存
                    utf8_str[rectab_size] = 0x00;
                    fseek(CT_RectabFile,3,SEEK_SET); //文件指针移至3
                    fread(utf8_str,rectab_size,1,CT_RectabFile); //读取记录表UTF-8文本
                    CT_RectabDataLength = CT_UTF8toMBCS_Length(utf8_str); //获取读取记录表UTF-8文本长度
                    CT_RectabDataMaxLength = CT_RectabDataLength; //记录表数据最大长度
                    CT_RectabData = malloc(CT_RectabDataLength+1); //为记录表数据分配内存
                    CT_RectabData[CT_RectabDataLength] = 0x00;
                    CT_UTF8toMBCS(CT_RectabData,utf8_str); //将记录表UTF-8文本转换成MBCS文本
                    free(utf8_str); //释放记录表UTF-8字符串
                }
                else
                {
                    CT_RectabData = malloc(1); //为当前记录表数据分配内存
                    CT_RectabData[0] = 0x00;
                }
            }
            else
            {
                CT_RectabFile = fopen(CT_RectabPath,"w+"); //新建记录表文件
                CT_RectabData = malloc(1); //为当前记录表数据分配内存
                CT_RectabData[0] = 0x00;
            }
        }
        else
        {
            cprint("ERROR : The record table is opened.");
        }
    }
    else
    {
        cprint("ERROR : The record table name is error.");
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 记录表关闭
//--------------------------------------------------------------------
void rectab_close()
{
    free(CT_RectabData); //清除记录表数据
    strclear(CT_RectabName); //清除记录表名称
    strclear(CT_RectabPath+7); //清除记录表路径
    fclose(CT_RectabFile); //关闭记录表文件
    CT_RectabDataLength = 0; //记录表数据长度
    CT_RectabDataMaxLength = 0; //记录表数据最大长度
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 记录表删除
//--------------------------------------------------------------------
void rectab_delete(char *name)
{
    if(name[0] != 0x00 && charindex(name,'/') == -1 && charindex(name,'*') == -1 && charindex(name,':') == -1 && charindex(name,'?') == -1 && charindex(name,'<') == -1 && charindex(name,'>') == -1 && charindex(name,'|') == -1) //如果记录表名称格式正确
    {
        if(strcmp(CT_RectabName,name) != 0) //如果当前打开的记录表不是要删除的记录表
        {
            strcat(CT_RectabPath,name); //在记录表路径中添加记录表名称
            strcat(CT_RectabPath,".rec"); //在记录表路径中添加记录表扩展名
            remove(CT_RectabPath); //删除记录表文件
            strclear(CT_RectabPath+7); //清除记录表路径
        }
        else
        {
            cprint("ERROR : The record table is opened.");
        }
    }
    else
    {
        cprint("ERROR : The record table name is error.");
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 记录表读取
//--------------------------------------------------------------------
int rectab_read(char *dest,char *title)
{
    if(title[0] != 0x00) //如果记录表标题不为空
    {
        if(charindex(title,'#') == -1 && charindex(title,'=') == -1) //如果参数title没有 "#" 和 "="
        {
            if(CT_RectabData[0] != 0x00) //如果记录表不为空
            {
                size_t title_l = strlen(title); //标题长度
                char *_title = malloc(title_l+2); //标题
                size_t title_pos = 0; //标题位置
                int data_size = 0; //数据大小
                _title[title_l+1] = 0x00;
                strcpy(_title,title); //将标题(参数)复制到标题
                strcat(_title,"="); //标题添加=
                title_pos = strindex(CT_RectabData,_title); //记录表中标题的位置
                free(_title); //释放标题
                if(title_pos != -1) //如果记录表中有标题
                {
                    int data_pos = (int)title_pos + ((int)title_l + 1); //记录表中数据的位置
                    int data_end_pos = 0; //数据结束的位置
                    int i = data_pos; //数据的位置
                    while(i <= CT_RectabDataLength) //从数据的位置开始向后查找整个记录表
                    {
                        if(CT_RectabData[i] == '#') //如果找到数据结束符
                        {
                            data_end_pos = i; //数据结束符的位置
                            break; //中断
                        }
                        i += 1; //字符串索引 +1
                    }
                    data_size = (int)data_end_pos - (int)data_pos; //数据大小
                    if(dest != NULL) //如果目标字符串不为空
                    {
                        strslice(dest,CT_RectabData,data_pos,data_end_pos); //从记录表中传递指定数据
                    }
                    return data_size; //中断
                }
            }
        }
        else
        {
            cprint("ERROR : The record table title is error.");
        }
    }
    return 0;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 记录表写入
//--------------------------------------------------------------------
void rectab_write(char *title,char *text)
{
    if(title[0] != 0x00 && text[0] != 0x00) //如果标题和文本不为空
    {
        if(charindex(title,'#') == -1 && charindex(title,'=') == -1) //如果标题没有 "#" 和 "="
        {
            if(charindex(text,'#') == -1 && charindex(text,'=') == -1) //如果参数文本没有 "#" 和 "="
            {
                size_t title_l = strlen(title); //标题长度
                size_t text_l = strlen(text); //文本长度
                int replace_part_l = (int)title_l + (int)text_l + 2; //替换部分的长度
                char *_title = malloc(title_l+2); //标题;
                char *replace_part = malloc(replace_part_l+1); //替换的部分
                _title[title_l+1] = 0x00;
                replace_part[replace_part_l] = 0x00;
                strcpy(_title,title); //将标题(参数)复制到标题
                strcat(_title,"="); //标题添加=
                strcpy(replace_part,_title); //替换部分添加标题
                strcat(replace_part,text); //替换部分添加文本
                strcat(replace_part,"#"); //替换部分添加&
                if(CT_RectabData[0] == 0x00) //如果记录表为空
                {
                    CT_RectabDataLength = replace_part_l; //记录表数据长度
                    if(CT_RectabDataLength > CT_RectabDataMaxLength) //如果记录表数据长度大于记录表数据最大长度
                    {
                        CT_RectabData = realloc(CT_RectabData,replace_part_l+1); //为记录表重新分配内存
                        CT_RectabData[replace_part_l] = 0x00;
                        CT_RectabDataMaxLength = CT_RectabDataLength; //记录表数据最大长度
                    }
                    strcat(CT_RectabData,replace_part); //将替换的部分添加到记录表数据
                }
                else
                {
                    size_t title_pos = strindex(CT_RectabData,_title); //记录表中标题的位置
                    if(title_pos == -1) //如果记录表中没有指定标题
                    {
                        CT_RectabDataLength = CT_RectabDataLength + replace_part_l; //记录表新长度
                        if(CT_RectabDataLength > CT_RectabDataMaxLength) //如果记录表数据长度大于记录表数据最大长度
                        {
                            CT_RectabData = realloc(CT_RectabData,CT_RectabDataLength+1); //为记录表重新分配内存
                            CT_RectabData[CT_RectabDataLength] = 0x00;
                            CT_RectabDataMaxLength = CT_RectabDataLength; //记录表数据最大长度
                        }
                        strcat(CT_RectabData,replace_part); //将替换的部分添加到记录表数据
                    }
                    else
                    {
                        char *sour_part = NULL; //源部分
                        int sour_part_l = 0; //源部分长度
                        int sour_part_pos = (int)title_pos; //记录表中数据的位置
                        int i = sour_part_pos; //数据位置
                        int sour_partend_pos = 0; //数据结束的位置
                        while(i < CT_RectabDataLength) //查找整个记录表
                        {
                            if(CT_RectabData[i] == '#') //如果找到数据结束符
                            { 
                                sour_partend_pos = i; //数据结束符的位置
                                break;
                            }
                            i += 1; //字符串索引 +1
                        }
                        sour_part_l = sour_partend_pos - sour_part_pos + 1; //源部分长度
                        sour_part = malloc(sour_part_l+1); //为源部分分配内存
                        sour_part[sour_part_l] = 0x00;
                        strslice(sour_part,CT_RectabData,sour_part_pos,sour_partend_pos+1); //获取要替换的数据
                        if(replace_part_l > sour_part_l) //如果新部分的长度大于替换的部分的长度
                        {
                            CT_RectabDataLength += replace_part_l - sour_part_l; //记录表新长度
                            if(CT_RectabDataLength > CT_RectabDataMaxLength) //如果记录表数据长度大于记录表数据最大长度
                            {
                                CT_RectabData = realloc(CT_RectabData,CT_RectabDataLength+1); //为记录表重新分配内存
                                CT_RectabData[CT_RectabDataLength] = 0x00;
                                CT_RectabDataMaxLength = CT_RectabDataLength; //记录表数据最大长度
                            }
                        }
                        strreplace(CT_RectabData,sour_part,replace_part); //在记录表中替换
                        free(sour_part); //释放源部分
                    }
                }
                free(_title); //释放标题
                free(replace_part); //释放替换的部分
            }
            else
            {
                cprint("ERROR : The record table text is error.");
            }
        }
        else
        {
            cprint("ERROR : The record table title is error.");
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 清除指定标题的数据
//--------------------------------------------------------------------
void rectab_null(char *title)
{
    if(title[0] != 0x00) //如果标题不为空
    {
        if(charindex(title,'#') == -1 && charindex(title,'=') == -1) //如果标题不为空
        {
            size_t title_l = strlen(title); //标题长度
            size_t title_pos = strindex(CT_RectabData,title); //记录表中标题的位置
            char *clear_part = NULL; //清除的部分
            int clear_part_l = 0; //清除的部分的长度
            if(title_pos != -1) //如果记录表中有指定标题
            {
                int data_pos = (int)title_pos + (int)title_l + 1; //数据位置
                int data_pos_end = 0; //数据结束的位置
                int i = data_pos; //数据索引
                while(i < CT_RectabDataLength) //查找整个记录表
                {
                    if(CT_RectabData[i] == '#') //如果找到数据结束符
                    { 
                        data_pos_end = i; //数据结束符的位置
                        break;
                    }
                    i += 1; //字符串索引 +1
                }
                clear_part_l = ((int)title_l + 1) + (data_pos_end - data_pos + 1); //清除的部分的长度
                clear_part = malloc(clear_part_l+1); //为清除的部分分配内存
                clear_part[clear_part_l] = 0x00;
                strslice(clear_part,CT_RectabData,title_pos,data_pos_end+1); //获取清除的部分
                strreplace(CT_RectabData,clear_part,""); //清除记录表指定标题的数据
                free(clear_part); //释放清除的部分
            }
        }
        else
        {
            cprint("ERROR : The record table title is error.");
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 记录表保存
//--------------------------------------------------------------------
void rectab_save()
{
    if(CT_RectabName[0] != 0x00) //如果打开了记录表文件
    {
        char utf8_bom[3] = {0xef,0xbb,0xbf}; //UTF-8 BOM
        int utf8_str_l = CT_MBCStoUTF8_Length(CT_RectabData); //计算记录表数据从MBCS转换为UTF-8的长度
        char *utf8_str = malloc(utf8_str_l); //为UTF-8字符串分配内存
        CT_MBCStoUTF8(utf8_str,CT_RectabData); //将记录表数据从MBCS转换为UTF-8
        remove(CT_RectabPath); //删除记录表文件
        CT_RectabFile = fopen(CT_RectabPath,"w+"); //新建记录表文件
        fseek(CT_RectabFile,0,SEEK_SET); //设置记录表文件位置
        fwrite(utf8_bom,3,1,CT_RectabFile); //写入UTF-8 BOM
        fseek(CT_RectabFile,3,SEEK_SET); //设置记录表文件位置
        fwrite(utf8_str,utf8_str_l,1,CT_RectabFile); //写入记录表数据
        free(utf8_str); //释放UTF-8字符串
    }
}
//--------------------------------------------------------------------