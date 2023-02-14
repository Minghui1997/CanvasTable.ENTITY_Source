#define _CRT_SECURE_NO_WARNINGS

#include "CanvasTableSYS.h"

char *CT_RectabData = NULL; //当前记录表数据
char CT_RectabName[129] = ""; //当前记录表名称
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
            char rectab_path[140] = "RECTAB/"; //记录表路径
            strcat(rectab_path,name); //在记录表路径中添加记录表名称
            strcat(rectab_path,".rec"); //在记录表路径中添加记录表扩展名
            if((CT_RectabFile=fopen(rectab_path,"r+")) != NULL) //如果记录表文件夹中有记录表文件
            {
                int rectab_size = 0; //记录表大小
                fseek(CT_RectabFile,0,SEEK_END); //文件指针移至文件末尾
                rectab_size = ftell(CT_RectabFile) - 3; //获取记录表大小
                if(rectab_size > 0) //如果记录表大小大于0
                {
                    char *utf8_str = NULL; //记录表UTF-8字符串
                    int utf8rec_l = 0; //记录表UTF-8字符串长度
                    utf8_str = malloc(rectab_size+1); //为记录表UTF-8字符串分配内存
                    utf8_str[rectab_size] = 0x00;
                    fseek(CT_RectabFile,3,SEEK_SET); //文件指针移至3
                    fread(utf8_str,rectab_size,1,CT_RectabFile); //读取记录表UTF-8文本
                    utf8rec_l = CT_MBCStoUTF8_Length(utf8_str); //获取读取记录表UTF-8文本长度
                    CT_RectabData = malloc(utf8rec_l+1); //为记录表数据分配内存
                    CT_RectabData[utf8rec_l] = 0x00;
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
                CT_RectabFile = fopen(rectab_path,"w+"); //新建记录表文件
                CT_RectabData = malloc(1); //为当前记录表数据分配内存
                CT_RectabData[0] = 0x00;
            }
            strcat(CT_RectabName,name); //当前打开的记录表名称
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
    fclose(CT_RectabFile); //关闭记录表文件
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
            char rectab_path[140] = "RECTAB/"; //记录表路径
            strcat(rectab_path,name); //在记录表路径中添加记录表名称
            strcat(rectab_path,".rec"); //在记录表路径中添加记录表扩展名
            remove(rectab_path); //删除记录表文件
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
        if(charindex(title,'&') == -1 && charindex(title,'=') == -1) //如果参数title没有 "&" 和 "="
        {
            if(CT_RectabData[0] != 0x00) //如果记录表不为空
            {
                char _title[129] = ""; //标题
                int title_pos = 0; //标题位置
                int data_size = 0; //数据大小
                strcpy(_title,title); //将标题(参数)复制到标题
                strcat(_title,"="); //标题添加=
                title_pos = strindex(CT_RectabData,_title); //记录表中标题的位置
                if(title_pos != -1) //如果记录表中有标题
                {
                    int data_pos = title_pos + strlen(_title); //记录表中数据的位置
                    int rectab_list_l = strlen(CT_RectabData); //记录表长度
                    int data_end_pos = 0; //数据结束的位置
                    int i = data_pos; //数据的位置
                    while(i <= rectab_list_l) //从数据的位置开始向后查找整个记录表
                    {
                        if(CT_RectabData[i] == '&') //如果找到数据结束符
                        {
                            data_end_pos = i; //数据结束符的位置
                            break; //中断
                        }
                        i += 1;
                    }
                    data_size = data_end_pos - data_pos; //数据大小
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
    strcpy(dest,""); //传递空字符串
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
        if(charindex(title,'&') == -1 && charindex(title,'=') == -1) //如果标题没有 "&" 和 "="
        {
            if(charindex(text,'&') == -1 && charindex(text,'=') == -1) //如果参数文本没有 "&" 和 "="
            {
                char _title[129] = ""; //标题
                char replace_part[513] = ""; //替换的部分
                int rectab_l = strlen(CT_RectabData); //获取记录表数据长度
                int replace_part_l = 0; //替换的部分的长度
                int rectab_cl = 0; //记录表新长度
                strcpy(_title,title); //将标题(参数)复制到标题
                strcat(_title,"="); //标题添加=
                if(CT_RectabData[0] == 0x00) //如果记录表为空
                {
                    strcat(replace_part,title); //在记录表中添加标题
                    strcat(replace_part,"="); //在记录表中添加=
                    strcat(replace_part,text); //在记录表中添加文本
                    strcat(replace_part,"&"); //在记录表中添加&
                    replace_part_l = strlen(replace_part); //获取替换的部分的长度
                    rectab_cl = replace_part_l + 1; //记录表新长度
                    CT_RectabData = realloc(CT_RectabData,rectab_cl); //为记录表重新分配内存
                    CT_RectabData[rectab_cl-1] = 0x00;
                    strcat(CT_RectabData,replace_part); //将替换的部分添加到记录表数据
                }
                else
                {
                    int title_pos = strindex(CT_RectabData,_title); //记录表中标题的位置
                    if(title_pos == -1) //如果记录表中没有指定标题
                    {
                        char replace_part[513] = ""; //替换的部分
                        strcat(replace_part,title); //在记录表中添加标题
                        strcat(replace_part,"="); //在记录表中添加=
                        strcat(replace_part,text); //在记录表中添加文本
                        strcat(replace_part,"&"); //在记录表中添加&
                        replace_part_l = strlen(replace_part); //获取替换的部分的长度
                        rectab_l = strlen(CT_RectabData); //获取记录表数据长度
                        rectab_cl = rectab_l + replace_part_l + 1; //记录表新长度
                        CT_RectabData = realloc(CT_RectabData,rectab_cl); //为记录表重新分配内存
                        CT_RectabData[rectab_cl-1] = 0x00;
                        strcat(CT_RectabData,replace_part); //将替换的部分添加到记录表数据
                    }
                    else
                    {
                        char replace_data[513] = ""; //替换的数据
                        char replace_part[513] = ""; //替换的部分
                        char new_part[641] = ""; //新部分
                        int data_pos = title_pos + strlen(_title); //记录表中数据的位置
                        int i = data_pos; //数据位置
                        int data_end_pos = 0; //数据结束的位置
                        int replace_part_l = 0; //替换的部分的长度
                        int new_part_l = 0; //新部分的长度
                        while(i <= rectab_l) //查找整个记录表
                        {
                            if(CT_RectabData[i] == '&') //如果找到数据结束符
                            { 
                                data_end_pos = i; //数据结束符的位置
                                break;
                            }
                            i += 1;
                        }
                        strslice(replace_data,CT_RectabData,data_pos,data_end_pos); //获取要替换的数据
                        strcat(replace_part,title); //替换的部分添加标题
                        strcat(replace_part,"="); //替换的部分添加=
                        strcat(replace_part,replace_data); //替换的部分添加替换的数据
                        strcat(replace_part,"&"); //替换的部分添加&
                        strcat(new_part,title); //在新部分中添加标题
                        strcat(new_part,"="); //在新部分中添加=
                        strcat(new_part,text); //在新部分中添加文本
                        strcat(new_part,"&"); //在新部分中添加&
                        replace_part_l = strlen(replace_part); //替换的部分的长度
                        new_part_l = strlen(new_part); //新部分的长度
                        if(new_part_l > replace_part_l) //如果新部分的长度大于替换的部分的长度
                        {
                            rectab_cl = rectab_l + (new_part_l - replace_part_l) + 1; //记录表新长度
                            CT_RectabData = realloc(CT_RectabData,rectab_cl); //为记录表重新分配内存
                            CT_RectabData[rectab_cl-1] = 0x00;
                        }
                        strreplace(CT_RectabData,replace_part,new_part); //在记录表中替换
                    }
                }
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
        if(charindex(title,'&') == -1 && charindex(title,'=') == -1) //如果标题不为空
        {
            if(CT_RectabData[0] != 0x00) //如果记录表不为空
            {
                char _title[129] = ""; //标题
                int title_pos = 0; //标题位置
                strcpy(_title,title); //将标题(参数)复制到标题
                strcat(_title,"="); //标题添加=
                title_pos = strindex(CT_RectabData,_title); //记录表中标题的位置
                if(title_pos != -1) //如果记录表中有指定标题
                {
                    int rectab_l = strlen(CT_RectabData); //记录表长度
                    int data_end_pos = 0; //数据结束的位置
                    int data_pos = 0; //数据位置
                    char delete_data[513] = ""; //删除的数据
                    int i;
                    data_pos = title_pos + strlen(_title); //记录表中数据的位置
                    i = data_pos; //数据位置
                    while(i <= rectab_l) //从数据的位置开始向后查找整个记录表
                    {
                        if(CT_RectabData[i] == '&') //如果找到数据结束符
                        {
                            data_end_pos = i; //数据结束符的位置
                            break;
                        }
                        i += 1;
                    }
                    strslice(delete_data,CT_RectabData,title_pos,data_end_pos+1); //获取要替换的数据
                    strreplace(CT_RectabData,delete_data,""); //在记录表中清除
                }
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
    char rectab_path[140] = "RECTAB/"; //记录表路径
    strcat(rectab_path,CT_RectabName); //在记录表路径中添加记录表名称0
    strcat(rectab_path,".rec"); //在记录表路径中添加记录表扩展名
    if(CT_RectabName[0] != 0x00) //如果打开了记录表文件
    {
        char utf8_bom[3] = {0xef,0xbb,0xbf}; //UTF-8 BOM
        int utf8_str_l = CT_MBCStoUTF8_Length(CT_RectabData); //计算记录表数据从MBCS转换为UTF-8的长度
        char *utf8_str = malloc(utf8_str_l); //为UTF-8字符串分配内存
        CT_MBCStoUTF8(utf8_str,CT_RectabData); //将记录表数据从MBCS转换为UTF-8
        remove(rectab_path); //删除记录表文件
        CT_RectabFile = fopen(rectab_path,"w+"); //新建记录表文件
        fseek(CT_RectabFile,0,SEEK_SET); //设置记录表文件位置
        fwrite(utf8_bom,3,1,CT_RectabFile); //写入UTF-8 BOM
        fseek(CT_RectabFile,3,SEEK_SET); //设置记录表文件位置
        fwrite(utf8_str,utf8_str_l,1,CT_RectabFile); //写入记录表数据
        free(utf8_str); //释放UTF-8字符串
    }
}
//--------------------------------------------------------------------