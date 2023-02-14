#include <string.h>
#include "EditString.h"

//--------------------------------------------------------------------
// Str slice
//--------------------------------------------------------------------
void strslice(char *dest,char *sour,int s,int e)
{
    int str_i = 0;
    int i;
    strclear(dest);
    for(i=s;i<e;i++)
    {
        dest[str_i] = sour[i];
        str_i ++;
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// Str clear
//--------------------------------------------------------------------
void strclear(char *dest)
{
    int str_i = 0;
    while(dest[str_i] != 0x00)
    {
        dest[str_i] = 0x00;
        str_i ++;
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// Char index
//--------------------------------------------------------------------
int charindex(char *dest,char fc)
{
    int str_i = 0;
    while(dest[str_i] != 0x00)
    {
        if(dest[str_i] == fc)
        {
            return str_i;
        }
        str_i ++;
    }
    return -1;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// Char index - Last
//--------------------------------------------------------------------
int charindex_last(char *dest,char fc)
{
    int str_i = strlen(dest) - 1;
    while(str_i >= 0)
    {
        if(dest[str_i] == fc)
        {
            return str_i;
        }
        str_i --;
    }
    return -1;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// String index
//--------------------------------------------------------------------
int strindex(char *dest,char *str)
{
    int str_l = strlen(str);
    int tra_e = strlen(dest) - str_l + 1;
    int equ_n = 0;
    int i;
    int i2;
    if(str[0] == 0x00)
    {
        return -1;
    }
    for(i=0;i<tra_e;i++)
    {
        for(i2=0;i2<str_l;i2++)
        {
            if(dest[i+i2] == str[i2])
            {
                equ_n ++;
                if(equ_n == str_l)
                {
                    return i;
                }
            }
        }
        equ_n = 0;
    }
    return -1;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// String index - Last
//--------------------------------------------------------------------
int strindex_last(char *dest,char *str)
{
    int str_l = strlen(str);
    int tra_s = strlen(dest) - str_l + 1;
    int equ_n = 0;
    int i;
    int i2;
    if(str[0] == 0x00)
    {
        return -1;
    }
    for(i=tra_s;i>=0;i--)
    {
        for(i2=0;i2<str_l;i2++)
        {
            if(dest[i+i2] == str[i2])
            {
                equ_n ++;
                if(equ_n == str_l)
                {
                    return i;
                }
            }
        }
        equ_n = 0;
    }
    return -1;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// String replace
//--------------------------------------------------------------------
void strreplace(char *dest,char *repstr,char *sour)
{
    int dest_l = strlen(dest);
    int reps_l = strlen(repstr);
    int str_l = strlen(sour);
    int reps_i = strindex(dest,repstr);
    int str_offset = 0;
    int offset_s = 0;
    int offset_e = 0;
    int str_i = 0;
    int i;
    if(reps_i != -1)
    {
        if(sour[0] != 0x00)
        {
            if(str_l > reps_l)
            {
                str_offset = str_l - reps_l;
                offset_s = dest_l - 1 + str_offset;
                offset_e = reps_i + reps_l;
                for(i=offset_s;i>offset_e;i--)
                {
                    char temp_c = dest[i - str_offset];
                    dest[i] = temp_c;
                }
                for(i=reps_i;i<reps_i+str_l;i++)
                {
                    dest[i] = sour[str_i];
                    str_i ++;
                }
            }
            if(str_l < reps_l)
            {
                str_offset = reps_l - str_l;
                offset_s = reps_i + reps_l - str_offset;
                offset_e = dest_l - str_offset + 1;
                for(i=offset_s;i<offset_e;i++)
                {
                    char temp_c = dest[i + str_offset];
                    dest[i] = temp_c;
                }
                for(i=reps_i;i<reps_i+str_l;i++)
                {
                    dest[i] = sour[str_i];
                    str_i ++;
                }
            }
            if(str_l == reps_l)
            {
                for(i=reps_i;i<reps_i+str_l;i++)
                {
                    dest[i] = sour[str_i];
                    str_i ++;
                }
            }
        }
        else
        {
            str_offset = reps_l;
            offset_s = reps_i;
            offset_e = dest_l - str_offset + 1;
            for(i=offset_s;i<offset_e;i++)
            {
                char temp_c = dest[i + str_offset];
                dest[i] = temp_c;
            }
        }
    }
}
//--------------------------------------------------------------------