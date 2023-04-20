#include "CanvasTableSYS.h"
#include <windows.h>

//--------------------------------------------------------------------
// 多字节字符串转UTF-8字符串
//--------------------------------------------------------------------
void CT_MBCStoUTF8(char *dest,char *source)
{
    size_t str_l = strlen(source);
    char *wchar = NULL;
    int wstr_l = MultiByteToWideChar(CP_ACP,0,source,str_l,NULL,0);
    int ustr_l = 0;
    wchar = malloc(wstr_l*2);
    MultiByteToWideChar(CP_ACP,0,source,str_l,wchar,wstr_l);
    ustr_l = WideCharToMultiByte(CP_UTF8,0,wchar,wstr_l,NULL,0,0,0);
    WideCharToMultiByte(CP_UTF8,0,wchar,wstr_l,dest,ustr_l,0,0);
    free(wchar);
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 多字节字符串转UTF-8字符串的长度
//--------------------------------------------------------------------
int CT_MBCStoUTF8_Length(char *source)
{
    size_t str_l = strlen(source);
    char *wchar = NULL;
    int wstr_l = MultiByteToWideChar(CP_ACP,0,source,str_l,NULL,0);
    int ustr_l = 0;
    wchar = malloc(wstr_l*2);
    MultiByteToWideChar(CP_ACP,0,source,str_l,wchar,wstr_l);
    ustr_l = WideCharToMultiByte(CP_UTF8,0,wchar,wstr_l,NULL,0,0,0);
    free(wchar);
    return ustr_l;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// UTF-8字符串转多字节字符串
//--------------------------------------------------------------------
void CT_UTF8toMBCS(char *dest,char *source)
{
    size_t str_l = strlen(source);
    char *wchar = NULL;
    int wstr_l = MultiByteToWideChar(CP_UTF8,0,source,str_l,NULL,0);
    int mstr_l = 0;
    wchar = malloc(wstr_l*2);
    MultiByteToWideChar(CP_UTF8,0,source,str_l,wchar,wstr_l);
    mstr_l = WideCharToMultiByte(CP_ACP,0,wchar,wstr_l,NULL,0,0,0);
    WideCharToMultiByte(CP_ACP,0,wchar,wstr_l,dest,mstr_l,0,0);
    free(wchar);
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// UTF-8字符串转多字节字符串的长度
//--------------------------------------------------------------------
int CT_UTF8toMBCS_Length(char *source)
{
    size_t str_l = strlen(source);
    char *wchar = NULL;
    int wstr_l = MultiByteToWideChar(CP_UTF8,0,source,str_l,NULL,0);
    int mstr_l = 0;
    wchar = malloc(wstr_l*2);
    MultiByteToWideChar(CP_UTF8,0,source,str_l,wchar,wstr_l);
    mstr_l = WideCharToMultiByte(CP_ACP,0,wchar,wstr_l,NULL,0,0,0);
    free(wchar);
    return mstr_l;
}
//--------------------------------------------------------------------