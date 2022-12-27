#define _CRT_SECURE_NO_WARNINGS

#include "CanvasTableSYS.h"
#include <string.h>
#include <stdio.h>

extern int CT_ContainerKeyState[256];
extern int CT_ContainerKeyDown[256];
extern int CT_ContainerKeyUp[256];
extern int CT_ContainerMKeyState[3];
extern int CT_ContainerMKeyDown[3];
extern int CT_ContainerMKeyUp[3];
extern int CT_ContainerMWheel[2];
extern int CT_MousePosX;
extern int CT_MousePosY;
extern char CT_KeyChar[5];
extern int CT_ContainerTouchKey[16][7];
extern int CT_Repaint;

//--------------------------------------------------------------------
// 按键键值
//--------------------------------------------------------------------
int key_a = 65;
int key_b = 66;
int key_c = 67;
int key_d = 68;
int key_e = 69;
int key_f = 70;
int key_g = 71;
int key_h = 72;
int key_i = 73;
int key_j = 74;
int key_k = 75;
int key_l = 76;
int key_m = 77;
int key_n = 78;
int key_o = 79;
int key_p = 80;
int key_q = 81;
int key_r = 82;
int key_s = 83;
int key_t = 84;
int key_u = 85;
int key_v = 86;
int key_w = 87;
int key_x = 88;
int key_y = 89;
int key_z = 90;
int key_1 = 49;
int key_2 = 50;
int key_3 = 51;
int key_4 = 52;
int key_5 = 53;
int key_6 = 54;
int key_7 = 55;
int key_8 = 56;
int key_9 = 57;
int key_0 = 48;
int key_f1 = 112;
int key_f2 = 113;
int key_f3 = 114;
int key_f4 = 115;
int key_f5 = 116;
int key_f6 = 117;
int key_f7 = 118;
int key_f8 = 119;
int key_f9 = 120;
int key_f10 = 121;
int key_f11 = 122;
int key_f12 = 123;
int key_up = 38;
int key_down = 40;
int key_left = 37;
int key_right = 39;
int key_esc = 27;
int key_tab = 9;
int key_cap = 20;
int key_shift = 16;
int key_ctrl = 17;
int key_alt = 18;
int key_space = 32;
int key_enter = 13;
int key_back = 8;
int key_num1 = 97;
int key_num2 = 98;
int key_num3 = 99;
int key_num4 = 100;
int key_num5 = 101;
int key_num6 = 102;
int key_num7 = 103;
int key_num8 = 104;
int key_num9 = 105;
int key_num0 = 96;
int mkey_left = 0;
int mkey_middle = 1;
int mkey_right = 2;
int mkey_up = 0;
int mkey_down = 1;
//--------------------------------------------------------------------

//--------------------------------------------------------------------
// 按键按住检查
//--------------------------------------------------------------------
int keyboard_press(int key)
{
    if(key >= 0 && key <= 255) //如果按键键值在范围内
    {
        if(CT_ContainerKeyState[key] == 1) //如果按键状态容器中指定的按键标记为1
        {
            return 1;
        }
    }
    else //如果按键键值不在范围内
    {
        cprint("ERROR : Key value beyond range.");
    }
    return 0;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 按键按下检查
//--------------------------------------------------------------------
int keyboard_down(int key)
{
    if(key >= 0 && key <= 255) //如果按键键值在范围内
    {
        if(CT_ContainerKeyDown[key] == 1)  //如果按键按下容器中指定的按键标记为1
        {
            return 1;
        }
    }
    else //如果按键键值不在范围内
    {
        cprint("ERROR : Key value beyond range.");
    }
    return 0;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 按键松开检查
//--------------------------------------------------------------------
int keyboard_up(int key)
{
    if(key >= 0 && key <= 255) //如果按键键值在范围内
    {
        if(CT_ContainerKeyUp[key] == 1) //如果按键松开状态容器中指定的按键标记为1
        {
            return 1;
        }
    }
    else //如果按键键值不在范围内
    {
        cprint("ERROR : Key value beyond range.");
    }
    return 0;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 鼠标按住检查
//--------------------------------------------------------------------
int mouse_press(int mkey)
{
    if(mkey >= 0 && mkey <= 2) //如果鼠标键值在范围内
    {
        if(CT_ContainerMKeyState[mkey] == 1) //如果鼠标按键状态容器中指定的按键标记为1
        {
            return 1;
        }
    }
    else //如果鼠标键值不在范围内
    {
        cprint("ERROR : Mouse key value beyond range.");
    }
    return 0;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 鼠标按下检查
//--------------------------------------------------------------------
int mouse_down(int mkey)
{
    if(mkey >= 0 && mkey <= 2) //如果鼠标键值在范围内
    {
        if(CT_ContainerMKeyDown[mkey] == 1) //如果鼠标按键状态容器中指定的按键标记为1
        {
            return 1;
        }
    }
    else //如果鼠标键值不在范围内
    {
        cprint("ERROR : Mouse key value beyond range.");
    }
    return 0;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 鼠标按下检查
//--------------------------------------------------------------------
int mouse_up(int mkey)
{
    if(mkey >= 0 && mkey <= 2) //如果鼠标键值在范围内
    {
        if(CT_ContainerMKeyUp[mkey] == 1) //如果鼠标按键状态容器中指定的按键标记为1
        {
            return 1;
        }
    }
    else //如果鼠标键值不在范围内
    {
        cprint("ERROR : Mouse key value beyond range.");
    }
    return 0;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 鼠标位置x
//--------------------------------------------------------------------
int mouse_x()
{
    return CT_MousePosX;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 鼠标位置y
//--------------------------------------------------------------------
int mouse_y()
{
    return CT_MousePosY;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 鼠标滚轮检查
//--------------------------------------------------------------------
int mouse_wheel(int mkey)
{
    if(mkey == 0 || mkey == 1) //如果鼠标滚轮参数正确
    {
        if(CT_ContainerMWheel[mkey] == 1) //如果鼠标滚轮容器中指定的滚动标记为1
        {
            return 1;
        }
    }
    else //如果鼠标滚轮参数不正确
    {
        cprint("ERROR : Mouse key value beyond range.");
    }
    return 0;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 创建触摸按键
//--------------------------------------------------------------------
void touchkey_create(int id,int x,int y,int w,int h,int key)
{
    if(CT_ContainerTouchKey[id][0] == NULL) //如果触摸键容器指定索引为空
    {
        if(w > 0 && h > 0 && key >= 0 && key <= 255) //如果触摸键属性正确
        {
            //将触摸键数据存入触摸键容器指定索引中
            CT_ContainerTouchKey[id][0] = 1;
            CT_ContainerTouchKey[id][1] = x;
            CT_ContainerTouchKey[id][2] = y;
            CT_ContainerTouchKey[id][3] = w;
            CT_ContainerTouchKey[id][4] = h;
            CT_ContainerTouchKey[id][5] = key;
            CT_ContainerTouchKey[id][6] = -1;
            CT_Repaint = 1; //请求重绘
        }
        else
        {
            cprint("ERROR : Unable to create TouchKey.");
        }
    }
    else //如果触摸键容器指定索引不为空
    {
        cprint("ERROR : TouchKey have been created.");
    }
    cprint("NOTE : TouchKey only support the mobile platform.");
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 删除触摸按键
//--------------------------------------------------------------------
void touchkey_delete(int id)
{
    if(CT_ContainerTouchKey[id][0] != 0) //如果触摸键容器指定索引不为空
    {
        //清除触摸键数据
        CT_ContainerTouchKey[id][0] = 0;
        CT_ContainerTouchKey[id][1] = 0;
        CT_ContainerTouchKey[id][2] = 0;
        CT_ContainerTouchKey[id][3] = 0;
        CT_ContainerTouchKey[id][4] = 0;
        CT_ContainerTouchKey[id][5] = 0;
        CT_ContainerTouchKey[id][6] = 0;
        CT_ContainerTouchKey[id][7] = 0;
        CT_Repaint = 1; //请求重绘
    }
    else //如果触摸键容器指定索引为空
    {
        cprint("ERROR 18 : The TouchKey is not created.");
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 触摸按键图片
//--------------------------------------------------------------------
void touchkey_image(int id,int img)
{
    if(CT_ContainerTouchKey[id][0] != 0) //如果触摸键容器指定索引不为空
    {
        CT_ContainerTouchKey[id][6] = img; //设置触摸键图像
        CT_Repaint = 1; //请求重绘
    }
}
//--------------------------------------------------------------------