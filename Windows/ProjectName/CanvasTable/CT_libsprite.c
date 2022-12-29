#define _CRT_SECURE_NO_WARNINGS

#include "CanvasTableSYS.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

extern int CT_ContainerSprite[10000][25];
extern int CT_ContainerSpriteLength;
extern int **CT_ResourceArgList;
extern int CT_SpriteId;
extern int CT_RunnerStatus;
extern int CT_TaskRunIndex;
extern int CT_Repaint;

void CTspr_main_null()
{
}
void CTspr_loop_null()
{
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 新建Sprite对象
//--------------------------------------------------------------------
void CTsprite_new(CTsprite *ctspr)
{
    ctspr->image = -1; //sprite图像索引
    ctspr->layer = 0; //sprite层
    ctspr->subnum = 1; //sprite子图像数量
    ctspr->subimg = 0; //sprite子图像索引
    ctspr->subimg_o = 0; //sprite子图像偏移
    ctspr->collision_type = 0; //sprite碰撞方式
    ctspr->collision_edge = 0; //sprite边缘碰撞
    ctspr->cbox_x = 0; //碰撞盒位置x
    ctspr->cbox_y = 0; //碰撞盒位置y
    ctspr->cbox_w = 0; //碰撞盒宽度
    ctspr->cbox_h = 0; //碰撞盒高度
    ctspr->cbox_cx = 0; //碰撞盒中心位置x
    ctspr->cbox_cy = 0; //碰撞盒中心位置y
    ctspr->cbox_r = 0; //碰撞盒半径
    ctspr->main = CTspr_main_null; //sprite程序main函数
    ctspr->loop = CTspr_loop_null; //sprite程序loop函数
//--------------------------------------------------------------------
    ctspr->id = CT_SpriteGetId(); //sprite id
    ctspr->type = -2; //类型
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 创建Sprite
//--------------------------------------------------------------------
int sprite_create(int x,int y,CTsprite *ctspr)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        int id = ctspr->id; //sprite id
        if(ctspr->type == -2) //如果对象类型是sprite
        {
            int image = ctspr->image; //sprite图像
            int image_w = 0; //图像宽度
            int image_h = 0; //图像高度
            int subimg_w = 0; //sprite子图像宽度
            int subimg_h = 0; //sprite子图像高度
            int conspr_i = CT_ContainerSpriteLength; //sprite容器sprite层的当前索引
            int layer = ctspr->layer; //sprite层
            int subimg = ctspr->subimg; //sprite默认子图像索引
            int subnum = ctspr->subnum; //sprite子图像数量
            int subimg_o = ctspr->subimg_o; //sprite子图像偏移
            int coll_t = ctspr->collision_type; //sprite碰撞类型
            int coll_e = ctspr->collision_edge; //sprite边缘碰撞
            int box_x = ctspr->cbox_x; //sprite碰撞盒位置x
            int box_y = ctspr->cbox_y; //sprite碰撞盒位置y
            int box_w = ctspr->cbox_w; //sprite碰撞盒宽度
            int box_h = ctspr->cbox_h; //sprite碰撞盒高度
            int box_cx = ctspr->cbox_cx; //sprite碰撞盒中心位置x
            int box_cy = ctspr->cbox_cy; //sprite碰撞盒中心位置y
            int box_r = ctspr->cbox_r; //sprite半径
            if(image != -1) //如果sprite有图像
            {
                int res_type = CT_ResourceArgList[image][1]; //获取资源类型
                if(res_type == 0) //如果资源类型是图像
                {
                    image_w = CT_ResourceArgList[image][2]; //sprite图像宽度
                    image_h = CT_ResourceArgList[image][3]; //sprite图像高度
                    if(subimg_o == 0) //如果图像偏移为0
                    {
                        subimg_w = image_w / subnum; //sprite子图像宽度
                        subimg_h = image_h; //子图像高度
                    }
                    if(subimg_o > 0) //如果图像偏移大于0
                    {
                        subimg_w = image_w / subnum - subimg_o * 2; //sprite子图像宽度
                        subimg_h = image_h - subimg_o * 2; //子图像高度
                    }
                    if(box_x == 0 && box_y == 0 && box_w == 0 && box_h == 0) //如果没有设置矩形碰撞盒
                    {
                        box_x = 0; //sprite碰撞盒位置x
                        box_y = 0; //sprite碰撞盒位置y
                        box_w = subimg_w; //sprite碰撞盒宽度/半径
                        box_h = subimg_h; //sprite碰撞盒高度
                    }
                    if(box_cx == 0 && box_cy == 0 && box_r == 0) //如果没有设置圆形碰撞盒
                    {
                        box_cx = subimg_w / 2; //sprite碰撞盒位置x
                        box_cy = subimg_h / 2; //sprite碰撞盒位置y
                        box_r = subimg_w / 2; //sprite碰撞盒宽度/半径
                    }
                }
                else
                {
                    cprint("ERROR : The resource index corresponding to the resource file is not a image.");
                }
            }
            if(subnum >= 1 && subimg <= subnum - 1) //如果sprite子图像数量大于等于1
            {
                int i;
                for(i=0;i<CT_ContainerSpriteLength;i++) //遍历sprite容器
                {
                    if(CT_ContainerSprite[i][0] == NULL) //如果当前sprite为空
                    {
                        //将sprite数据写入sprite容器
                        CT_ContainerSprite[i][0] = 1;
                        CT_ContainerSprite[i][1] = id;
                        CT_ContainerSprite[i][2] = layer;
                        CT_ContainerSprite[i][3] = x;
                        CT_ContainerSprite[i][4] = y;
                        CT_ContainerSprite[i][5] = image;
                        CT_ContainerSprite[i][6] = subimg;
                        CT_ContainerSprite[i][7] = subnum;
                        CT_ContainerSprite[i][8] = subimg_w;
                        CT_ContainerSprite[i][9] = subimg_h;
                        CT_ContainerSprite[i][10] = x;
                        CT_ContainerSprite[i][11] = y;
                        CT_ContainerSprite[i][12] = coll_t;
                        CT_ContainerSprite[i][13] = coll_e;
                        CT_ContainerSprite[i][14] = subimg_o;
                        CT_ContainerSprite[i][15] = box_x;
                        CT_ContainerSprite[i][16] = box_y;
                        CT_ContainerSprite[i][17] = box_w;
                        CT_ContainerSprite[i][18] = box_h;
                        CT_ContainerSprite[i][19] = box_cx;
                        CT_ContainerSprite[i][20] = box_cy;
                        CT_ContainerSprite[i][21] = box_r;
                        CT_TaskCreated(i,ctspr->main,ctspr->loop); //创建sprite程序
                        CT_Repaint = 1; //请求重绘
                        return i; //中断
                    }
                }
                //将sprite数据写入sprite容器
                CT_ContainerSprite[conspr_i][0] = 1;
                CT_ContainerSprite[conspr_i][1] = id;
                CT_ContainerSprite[conspr_i][2] = layer;
                CT_ContainerSprite[conspr_i][3] = x;
                CT_ContainerSprite[conspr_i][4] = y;
                CT_ContainerSprite[conspr_i][5] = image;
                CT_ContainerSprite[conspr_i][6] = subimg;
                CT_ContainerSprite[conspr_i][7] = subnum;
                CT_ContainerSprite[conspr_i][8] = subimg_w;
                CT_ContainerSprite[conspr_i][9] = subimg_h;
                CT_ContainerSprite[conspr_i][10] = x;
                CT_ContainerSprite[conspr_i][11] = y;
                CT_ContainerSprite[conspr_i][12] = coll_t;
                CT_ContainerSprite[conspr_i][13] = coll_e;
                CT_ContainerSprite[conspr_i][14] = subimg_o;
                CT_ContainerSprite[conspr_i][15] = box_x;
                CT_ContainerSprite[conspr_i][16] = box_y;
                CT_ContainerSprite[conspr_i][17] = box_w;
                CT_ContainerSprite[conspr_i][18] = box_h;
                CT_ContainerSprite[conspr_i][19] = box_cx;
                CT_ContainerSprite[conspr_i][20] = box_cy;
                CT_ContainerSprite[conspr_i][21] = box_r;
                CT_TaskCreated(conspr_i,ctspr->main,ctspr->loop); //创建sprite程序
                CT_ContainerSpriteLength ++; //sprite容器索引 +1
                CT_Repaint = 1; //请求重绘
                return conspr_i; //中断
            }
            else
            {
                cprint("ERROR : The sprite property have error.");
            }
        }
        else
        {
            cprint("ERROR : Not is sprite object.");
        }
    }
    return -1;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 销毁Sprite
//--------------------------------------------------------------------
void sprite_destroy()
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        int i;
        for(i=0;i<25;i++) //遍历sprite数据
        {
            CT_ContainerSprite[CT_TaskRunIndex][i] = NULL; //清除sprite数据
        }
        CT_TaskDestroy(CT_TaskRunIndex); //销毁sprite程序
        CT_Repaint = 1; //请求重绘
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// Sprite位置x
//--------------------------------------------------------------------
int sprite_x()
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(CT_ContainerSprite[CT_TaskRunIndex][0] != NULL) //如果sprite不为空
        {
            int x = CT_ContainerSprite[CT_TaskRunIndex][3]; //sprite位置x
            return x; //返回sprite位置x
        }
    }
    return -1;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// Sprite位置y
//--------------------------------------------------------------------
int sprite_y()
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(CT_ContainerSprite[CT_TaskRunIndex][0] != NULL) //如果sprite不为空
        {
            int y = CT_ContainerSprite[CT_TaskRunIndex][4]; //sprite位置x
            return y; //返回sprite位置x
        }
    }
    return -1;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 设置Sprite位置x
//--------------------------------------------------------------------
void sprite_set_x(int x)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(CT_ContainerSprite[CT_TaskRunIndex] != NULL) //如果sprite不为空
        {
            CT_ContainerSprite[CT_TaskRunIndex][3] = x; //设置sprite位置x
            CT_Repaint = 1; //请求重绘
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 设置Sprite位置y
//--------------------------------------------------------------------
void sprite_set_y(int y)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(CT_ContainerSprite[CT_TaskRunIndex] != NULL) //如果sprite不为空
        {
            CT_ContainerSprite[CT_TaskRunIndex][4] = y; //设置sprite位置x
            CT_Repaint = 1; //请求重绘
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 设置Sprite子图像
//--------------------------------------------------------------------
void sprite_set_subimg(int index)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(CT_ContainerSprite[CT_TaskRunIndex][0] != NULL) //如果sprite不为空
        {
            int spr_subnum = CT_ContainerSprite[CT_TaskRunIndex][7]; //sprite子图像数量
            if(index <= spr_subnum - 1) //如果sprite子图像索引小于sprite子图像数量-1
            {
                CT_ContainerSprite[CT_TaskRunIndex][6] = index; //设置sprite子图像索引
                CT_Repaint = 1; //请求重绘
            }
            else
            {
                cprint("ERROR : The sprite subimage index is greater than the number of subimage.");
            }
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// Sprite变量赋值 数字
//--------------------------------------------------------------------
void spritenum_set(int i,double n)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(CT_ContainerSprite[CT_TaskRunIndex][0] != NULL) //如果sprite不为空
        {
            CT_TaskSetNum(CT_TaskRunIndex,i,n); //sprite程序数字变量赋值
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// Sprite变量赋值 字符串
//--------------------------------------------------------------------
void spritestr_set(int i,char *s)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(CT_ContainerSprite[CT_TaskRunIndex][0] != NULL) //如果sprite不为空
        {
            CT_TaskSetStr(CT_TaskRunIndex,i,s); //sprite程序字符串变量赋值
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// Sprite变量 数字
//--------------------------------------------------------------------
double spritenum(int i)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(CT_ContainerSprite[CT_TaskRunIndex][0] != NULL) //如果sprite不为空
        {
            double var_n = CT_TaskNum(CT_TaskRunIndex,i); //sprite程序数字变量值
            return var_n; //返回sprite程序数字变量值
        }
    }
    return 0;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// Sprite变量 字符串
//--------------------------------------------------------------------
char *spritestr(int i)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(CT_ContainerSprite[CT_TaskRunIndex][0] != NULL) //如果列表中sprite不为空
        {
            char *str = CT_TaskStr(CT_TaskRunIndex,i); //获取sprite程序字符串变量值
            return str;
        }
    }
    return 0;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 碰撞检测
//--------------------------------------------------------------------
int sprite_collision(CTsprite *collspr)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(collspr->type == -2) //如果对象是sprite
        {
            if(CT_ContainerSprite[CT_TaskRunIndex][0] != NULL) //如果sprite不为空
            {
                int spr_l = CT_ContainerSprite[CT_TaskRunIndex][2]; //sprite层
                int spr_x = CT_ContainerSprite[CT_TaskRunIndex][3]; //sprite位置x
                int spr_y = CT_ContainerSprite[CT_TaskRunIndex][4]; //sprite位置y
                int box_x = CT_ContainerSprite[CT_TaskRunIndex][15]; //sprite碰撞盒位置x
                int box_y = CT_ContainerSprite[CT_TaskRunIndex][16]; //sprite碰撞盒位置y
                int box_w = CT_ContainerSprite[CT_TaskRunIndex][17]; //sprite碰撞盒宽度
                int box_h = CT_ContainerSprite[CT_TaskRunIndex][18]; //sprite碰撞盒高度
                int box_cx = CT_ContainerSprite[CT_TaskRunIndex][19]; //sprite碰撞盒中心位置x
                int box_cy = CT_ContainerSprite[CT_TaskRunIndex][20]; //sprite碰撞盒中心位置y
                int box_r = CT_ContainerSprite[CT_TaskRunIndex][21]; //sprite碰撞盒半径
                int i;
                for(i=0;i<CT_ContainerSpriteLength;i++) //遍历sprite容器
                {
                    if(CT_ContainerSprite[i][0] != NULL) //如果当前sprite不为空
                    {
                        int cspr_id = CT_ContainerSprite[i][1]; //当前sprite id
                        int cspr_l = CT_ContainerSprite[i][2]; //当前sprite层
                        int cspr_x = CT_ContainerSprite[i][3]; //当前sprite位置x
                        int cspr_y = CT_ContainerSprite[i][4]; //当前sprite位置y
                        int cspr_coll_t = CT_ContainerSprite[i][12]; //当前sprite碰撞类型
                        int cspr_coll_e = CT_ContainerSprite[i][13]; //当前sprite边缘碰撞
                        int cbox_x = CT_ContainerSprite[i][15]; //sprite碰撞盒位置x
                        int cbox_y = CT_ContainerSprite[i][16]; //sprite碰撞盒位置y
                        int cbox_w = CT_ContainerSprite[i][17]; //sprite碰撞盒宽度
                        int cbox_h = CT_ContainerSprite[i][18]; //sprite碰撞盒高度
                        int cbox_cx = CT_ContainerSprite[i][19]; //sprite碰撞盒中心位置x
                        int cbox_cy = CT_ContainerSprite[i][20]; //sprite碰撞盒中心位置y
                        int cbox_r = CT_ContainerSprite[i][21]; //sprite碰撞盒半径
                        if(cspr_id == collspr->id) //如果当前sprite id等于碰撞的sprite id
                        {
                            if(spr_l == cspr_l) //如果sprite层等于碰撞的sprite层
                            {
                                if(cspr_coll_t == 0) //如果碰撞的sprite碰撞类型是矩形碰撞
                                {
                                    if(spr_x + box_x + box_w > cspr_x + cbox_x && spr_x + box_x < cspr_x + cbox_x + cbox_w && spr_y + box_y + box_h > cspr_y + cbox_y && spr_y + box_y < cspr_y + cbox_y + cbox_h) //如果sprite图像与碰撞的sprite图像相交
                                    {
                                        if(cspr_coll_e == 0) //如果碰撞的sprite不是边缘碰撞
                                        {
                                            return 1;
                                        }
                                        if(cspr_coll_e == 1) //如果碰撞的sprite是边缘碰撞
                                        {
                                            int spr_lx = CT_ContainerSprite[CT_TaskRunIndex][10]; //sprite上次位置x
                                            int spr_ly = CT_ContainerSprite[CT_TaskRunIndex][11]; //sprite上次位置y
                                            int dx = abs(spr_lx-spr_x);
                                            int dy = -abs(spr_ly-spr_y);
                                            int sx = spr_x < spr_lx ? 1 : -1;
                                            int sy = spr_y < spr_ly ? 1 : -1;
                                            int dd = dx + dy;
                                            int dd2;
                                            while(1)
                                            {
                                                if(!(spr_x + box_x + box_w > cspr_x + cbox_x && spr_x + box_x < cspr_x + cbox_x + cbox_w && spr_y + box_y + box_h > cspr_y + cbox_y && spr_y + box_y < cspr_y + cbox_y + cbox_h)) //如果sprite图像与碰撞的sprite图像不相交
                                                {
                                                    CT_ContainerSprite[CT_TaskRunIndex][3] = spr_x; //设置sprite位置
                                                    CT_ContainerSprite[CT_TaskRunIndex][4] = spr_y; //设置sprite位置
                                                    break;
                                                }
                                                if(spr_x == spr_lx && spr_y == spr_ly)
                                                {
                                                    break;
                                                }
                                                dd2 = 2 * dd;
                                                if(dd2 >= dy)
                                                {
                                                    dd += dy;
                                                    spr_x += sx;
                                                }
                                                if(dd2 <= dx)
                                                {
                                                    dd += dx;
                                                    spr_y += sy;
                                                }
                                            }
                                            CT_Repaint = 1; //请求重绘
                                            return 1;
                                        }
                                    }
                                }
                                if(cspr_coll_t == 1) //如果碰撞的sprite碰撞类型是圆形碰撞
                                {
                                    int spr_distance = sqrt(pow((spr_x+box_cx)-(cspr_x+cbox_cx),2)+pow((spr_y+box_cy)-(cspr_y+cbox_cy),2));
                                    if(spr_distance < box_r + cbox_r) //如果sprite图像在碰撞的sprite图像半径内
                                    {
                                        if(cspr_coll_e == 0) //如果碰撞的sprite不是边缘碰撞
                                        {
                                            return 1;
                                        }
                                        if(cspr_coll_e == 1) //如果碰撞的sprite是边缘碰撞
                                        {
                                            int spr_lx = CT_ContainerSprite[CT_TaskRunIndex][10]; //sprite上次位置x
                                            int spr_ly = CT_ContainerSprite[CT_TaskRunIndex][11]; //sprite上次位置y
                                            int dx = abs(spr_lx-spr_x);
                                            int dy = -abs(spr_ly-spr_y);
                                            int sx = spr_x < spr_lx ? 1 : -1;
                                            int sy = spr_y < spr_ly ? 1 : -1;
                                            int dd = dx + dy;
                                            int dd2;
                                            while(1)
                                            {
                                                int spr_distance = sqrt(pow((spr_x+box_cx)-(cspr_x+cbox_cx),2)+pow((spr_y+box_cy)-(cspr_y+cbox_cy),2));
                                                if(spr_distance >= box_r + cbox_r) //如果sprite图像在碰撞的sprite图像半径内
                                                {
                                                    CT_ContainerSprite[CT_TaskRunIndex][3] = spr_x; //设置sprite位置
                                                    CT_ContainerSprite[CT_TaskRunIndex][4] = spr_y; //设置sprite位置
                                                    break;
                                                }
                                                if(spr_x == spr_lx && spr_y == spr_ly)
                                                {
                                                    break;
                                                }
                                                dd2 = 2 * dd;
                                                if(dd2 >= dy)
                                                {
                                                    dd += dy;
                                                    spr_x += sx;
                                                }
                                                if(dd2 <= dx)
                                                {
                                                    dd += dx;
                                                    spr_y += sy;
                                                }
                                            }
                                            CT_Repaint = 1; //请求重绘
                                            return 1;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }              
        }
        else
        {
            cprint("ERROR : Not is sprite object.");
        }
    }
    return 0;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// Sprite设置矩形碰撞盒
//--------------------------------------------------------------------
void sprite_set_rectbox(int x,int y,int w,int h)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(CT_ContainerSprite[CT_TaskRunIndex][0] != NULL) //如果列表中sprite不为空
        {
            CT_ContainerSprite[CT_TaskRunIndex][15] = x; //sprite碰撞盒位置x
            CT_ContainerSprite[CT_TaskRunIndex][16] = y; //sprite碰撞盒位置y
            CT_ContainerSprite[CT_TaskRunIndex][17] = w; //sprite碰撞盒宽度
            CT_ContainerSprite[CT_TaskRunIndex][18] = h; //sprite碰撞盒高度
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// Sprite设置圆形碰撞盒
//--------------------------------------------------------------------
void sprite_set_cirbox(int cx,int cy,int r)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(CT_ContainerSprite[CT_TaskRunIndex][0] != NULL) //如果列表中sprite不为空
        {
            CT_ContainerSprite[CT_TaskRunIndex][19] = cx; //sprite碰撞盒中心位置x
            CT_ContainerSprite[CT_TaskRunIndex][20] = cy; //sprite碰撞盒中心位置y
            CT_ContainerSprite[CT_TaskRunIndex][21] = r; //sprite碰撞盒半径
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// sprite检查
//--------------------------------------------------------------------
int sprite_check(int x,int y,CTsprite *ctspr)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        int id = ctspr->id; //sprite id
        if(ctspr->type == -2) //如果对象是sprite
        {
            int i;
            for(i=0;i<CT_ContainerSpriteLength;i++) //遍历sprite容器
            {
                if(CT_ContainerSprite[i][0] != NULL) //如果当前sprite不为空
                {
                    int spr_id = CT_ContainerSprite[i][1]; //当前sprite id
                    if(spr_id == id) //如果当前sprite id等于sprite id
                    {
                        int spr_x = CT_ContainerSprite[i][3]; //当前sprite位置x
                        int spr_y = CT_ContainerSprite[i][4]; //当前sprite位置y
                        int spr_collw = CT_ContainerSprite[i][8]; //当前sprite碰撞区宽度
                        int spr_collh = CT_ContainerSprite[i][9]; //当前sprite碰撞区高度
                        if(x >= spr_x && y >= spr_y && x < spr_x + spr_collw && y < spr_y + spr_collh) //如果当前sprite在指定位置中
                        {
                            return 1;
                        }
                    }
                }
            }
        }
        else
        {
            cprint("ERROR : Not is sprite object.");
        }
    }
    return 0;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// sprite索引
//--------------------------------------------------------------------
int sprite_index()
{
    return CT_TaskRunIndex; //返回sprite索引
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 销毁索引Sprite
//--------------------------------------------------------------------
void isprite_destroy(int si)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(CT_ContainerSprite[si][0] != NULL) //如果指定索引的sprite不为空
        {
            int i;
            for(i=0;i<25;i++) //遍历sprite数据
            {
                CT_ContainerSprite[si][i] = NULL; //清空sprite
            }
            CT_TaskDestroy(si); //销毁sprite程序
            CT_Repaint = 1; //请求重绘
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 索引Sprite位置x
//--------------------------------------------------------------------
int isprite_x(int si)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(CT_ContainerSprite[si][0] != NULL) //如果指定索引的sprite不为空
        {
            int x = CT_ContainerSprite[si][3]; //sprite位置x
            return x; //返回sprite位置x
        }
        else
        {
            cprint("ERROR : The sprite is null.");
        }
    }
    return -1;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 索引Sprite位置y
//--------------------------------------------------------------------
int isprite_y(int si)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(CT_ContainerSprite[si][0] != NULL) //如果指定索引的sprite不为空
        {
            int y = CT_ContainerSprite[si][4]; //sprite位置y
            return y; //返回sprite位置y
        }
        else
        {
            cprint("ERROR : The sprite is null.");
        }
    }
    return -1;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 设置索引Sprite位置x
//--------------------------------------------------------------------
void isprite_set_x(int si,int x)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(CT_ContainerSprite[si][0] != NULL) //如果指定索引的sprite不为空
        {
            CT_ContainerSprite[si][3] = x; //设置sprite位置x
            CT_Repaint = 1; //请求重绘
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 设置索引Sprite位置y
//--------------------------------------------------------------------
void isprite_set_y(int si,int y)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(CT_ContainerSprite[si][0] != NULL) //如果指定索引的sprite不为空
        {
            CT_ContainerSprite[si][4] = y; //设置sprite位置y
            CT_Repaint = 1; //请求重绘
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 设置索引Sprite子图像
//--------------------------------------------------------------------
void isprite_set_subimg(int si,int index)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(CT_ContainerSprite[si][0] != NULL) //如果指定索引的sprite不为空
        {
            int spr_subnum = CT_ContainerSprite[si][7]; //sprite子图像数量
            if(index <= spr_subnum - 1) //如果sprite子图像索引小于sprite子图像数量-1
            {
                CT_ContainerSprite[si][6] = index; //设置sprite子图像索引
                CT_Repaint = 1; //请求重绘
            }
            else
            {
                cprint("ERROR : The sprite subimage index is greater than the number of subimage.");
            }
        }
        else
        {
            cprint("ERROR : The sprite is null.");
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 索引Sprite变量赋值 数字
//--------------------------------------------------------------------
void ispritenum_set(int si,int i,double n)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(CT_ContainerSprite[si][0] != NULL) //如果指定索引的sprite不为空
        {
            CT_TaskSetNum(si,i,n); //sprite程序数字变量赋值
        }
        else
        {
            cprint("ERROR : The sprite is null.");
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 索引Sprite变量赋值 字符串
//--------------------------------------------------------------------
void ispritestr_set(int si,int i,char *s)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(CT_ContainerSprite[si][0] != NULL) //如果sprite不为空
        {
            CT_TaskSetStr(si,i,s); //sprite程序字符串变量赋值
        }
        else
        {
            cprint("ERROR : The sprite is null.");
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 索引Sprite变量 数字
//--------------------------------------------------------------------
double ispritenum(int si,int i)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(CT_ContainerSprite[si][0] != NULL) //如果sprite不为空
        {
            double var_n = CT_TaskNum(si,i); //sprite程序数字变量值
            return var_n; //返回sprite程序数字变量值
        }
        else
        {
            cprint("ERROR : The sprite is null.");
        }
    }
    return 0;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 索引Sprite变量 字符串
//--------------------------------------------------------------------
char *ispritestr(int si,int i)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(CT_ContainerSprite[si][0] != NULL) //如果列表中sprite不为空
        {
            char *str = CT_TaskStr(si,i); //获取sprite程序字符串变量值
            return str;
        }
        else
        {
            cprint("ERROR : The sprite is null.");
        }
    }
    return 0;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 销毁所有Sprite
//--------------------------------------------------------------------
void sprite_destroy_all(CTsprite *ctspr)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        int id = ctspr->id; //sprite id
        if(ctspr->type == -2) //如果对象是sprite
        {
            int i;
            for(i=0;i<CT_ContainerSpriteLength;i++) //遍历sprite容器
            {
                if(CT_ContainerSprite[i][0] != NULL) //如果当前sprite不为空
                {
                    int spr_id = CT_ContainerSprite[i][1]; //当前sprite id
                    if(spr_id == id) //如果当前sprite id等于sprite id
                    {
                        int i2;
                        for(i2=0;i2<20;i2++) //遍历sprite数据
                        {
                            CT_ContainerSprite[i][i2] = NULL; //清空当前sprite
                        }
                        CT_TaskDestroy(i); //销毁sprite程序
                        CT_Repaint = 1; //请求重绘
                    }
                }
            }
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 设置所有Sprite位置x
//--------------------------------------------------------------------
void sprite_all_set_x(CTsprite *ctspr,int x)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        int id = ctspr->id; //sprite id
        if(ctspr->type == -2) //如果对象是sprite
        {
            int i;
            for(i=0;i<CT_ContainerSpriteLength;i++) //遍历sprite容器
            {
                if(CT_ContainerSprite[i][0] != NULL) //如果当前sprite不为空
                {
                    int spr_id = CT_ContainerSprite[i][1]; //当前sprite id
                    if(spr_id == id) //如果当前sprite id等于sprite id
                    {
                        CT_ContainerSprite[i][3] = x; //设置当前sprite位置x
                        CT_Repaint = 1; //请求重绘
                    }
                }
            }
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 设置所有Sprite位置y
//--------------------------------------------------------------------
void sprite_all_set_y(CTsprite *ctspr,int y)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        int id = ctspr->id; //sprite id
        if(ctspr->type == -2) //如果对象是sprite
        {
            int i;
            for(i=0;i<CT_ContainerSpriteLength;i++) //遍历sprite容器
            {
                if(CT_ContainerSprite[i][0] != NULL) //如果当前sprite不为空
                {
                    int spr_id = CT_ContainerSprite[i][1]; //当前sprite id
                    if(spr_id == id) //如果当前sprite id等于sprite id
                    {
                        CT_ContainerSprite[i][4] = y; //设置当前sprite位置y
                        CT_Repaint = 1; //请求重绘
                    }
                }
            }
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 设置所有Sprite子图像
//--------------------------------------------------------------------
void sprite_all_set_subimg(CTsprite *ctspr,int index)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        int id = ctspr->id; //sprite id
        if(ctspr->type == -2) //如果对象是sprite
        {
            int i;
            for(i=0;i<CT_ContainerSpriteLength;i++) //遍历sprite容器
            {
                if(CT_ContainerSprite[i][0] != NULL) //如果当前sprite不为空
                {
                    int spr_id = CT_ContainerSprite[i][1]; //当前sprite id
                    if(spr_id == id) //如果当前sprite id等于sprite id
                    {
                        int spr_subnum = CT_ContainerSprite[i][7]; //当前sprite子图像数量
                        if(index <= spr_subnum - 1) //如果sprite子图像索引小于sprite子图像数量-1
                        {
                            CT_ContainerSprite[i][6] = index; //设置sprite子图像索引
                            CT_Repaint = 1; //请求重绘
                        }
                        else
                        {
                            cprint("ERROR : The sprite subimage index is greater than the number of subimage.");
                        }
                    }
                }
            }
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 所有Sprite变量赋值 数字
//--------------------------------------------------------------------
void spritenum_all_set(CTsprite *ctspr,int vi,double n)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        int id = ctspr->id; //sprite id
        if(ctspr->type == -2) //如果对象是sprite
        {
            int i;
            for(i=0;i<CT_ContainerSpriteLength;i++) //遍历sprite容器
            {
               if(CT_ContainerSprite[i][0] != NULL) //如果当前sprite不为空
                {
                    int spr_id = CT_ContainerSprite[i][1]; //当前sprite id
                    if(spr_id == id) //如果当前sprite id等于sprite id
                    {
                        CT_TaskSetNum(i,vi,n); //sprite程序数字变量赋值
                    }
                }
            }
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 所有Sprite变量赋值 字符串
//--------------------------------------------------------------------
void spritestr_all_set(CTsprite *ctspr,int vi,char *s)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        int id = ctspr->id; //sprite id
        if(ctspr->type == -2) //如果对象是sprite
        {
            int i;
            for(i=0;i<CT_ContainerSpriteLength;i++) //遍历sprite容器
            {
                if(CT_ContainerSprite[i][0] != NULL) //如果当前sprite不为空
                {
                    int spr_id = CT_ContainerSprite[i][1]; //当前sprite id
                    if(spr_id == id) //如果当前sprite id等于sprite id
                    {
                        CT_TaskSetStr(i,vi,s); //sprite程序字符串变量赋值
                    }
                }
            }
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 设置sprite层
//--------------------------------------------------------------------
void sprite_set_layer(int l)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(CT_ContainerSprite[CT_TaskRunIndex][0] != NULL) //如果sprite不为空
        {
            CT_ContainerSprite[CT_TaskRunIndex][2] = l; //设置sprite层
            CT_Repaint = 1; //请求重绘
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 设置索引Sprite层
//--------------------------------------------------------------------
void isprite_set_layer(int si,int l)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(CT_ContainerSprite[si][0] != NULL) //如果指定索引的sprite不为空
        {
            CT_ContainerSprite[si][2] = l; //设置指定索引sprite层
            CT_Repaint = 1; //请求重绘
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 设置所有Sprite层
//--------------------------------------------------------------------
void sprite_all_set_layer(CTsprite *ctspr,int l)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        int id = ctspr->id; //sprite id
        if(ctspr->type == -2) //如果对象是sprite
        {
            int i;
            for(i=0;i<CT_ContainerSpriteLength;i++) //遍历sprite容器
            {
                if(CT_ContainerSprite[i][0] != NULL) //如果当前sprite不为空
                {
                    int spr_id = CT_ContainerSprite[i][1]; //当前sprite id
                    if(spr_id == id) //如果当前sprite id等于sprite id
                    {
                        CT_ContainerSprite[i][2] = l; //设置当前sprite层
                        CT_Repaint = 1; //请求重绘
                    }
                }
            }
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 索引Sprite碰撞检测
//--------------------------------------------------------------------
int isprite_collision(int si)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(CT_ContainerSprite[CT_TaskRunIndex][0] != NULL) //如果sprite不为空
        {
            int spr_l = CT_ContainerSprite[CT_TaskRunIndex][2]; //sprite层
            int spr_x = CT_ContainerSprite[CT_TaskRunIndex][3]; //sprite位置x
            int spr_y = CT_ContainerSprite[CT_TaskRunIndex][4]; //sprite位置y
            int box_x = CT_ContainerSprite[CT_TaskRunIndex][15]; //sprite碰撞盒位置x
            int box_y = CT_ContainerSprite[CT_TaskRunIndex][16]; //sprite碰撞盒位置y
            int box_w = CT_ContainerSprite[CT_TaskRunIndex][17]; //sprite碰撞盒宽度
            int box_h = CT_ContainerSprite[CT_TaskRunIndex][18]; //sprite碰撞盒高度
            int box_cx = CT_ContainerSprite[CT_TaskRunIndex][19]; //sprite碰撞盒中心位置x
            int box_cy = CT_ContainerSprite[CT_TaskRunIndex][20]; //sprite碰撞盒中心位置y
            int box_r = CT_ContainerSprite[CT_TaskRunIndex][21]; //sprite碰撞盒半径
            int i;
            if(CT_ContainerSprite[si][0] != NULL) //如果当前sprite不为空
            {
                int cspr_id = CT_ContainerSprite[si][1]; //当前sprite id
                int cspr_l = CT_ContainerSprite[si][2]; //当前sprite层
                int cspr_x = CT_ContainerSprite[si][3]; //当前sprite位置x
                int cspr_y = CT_ContainerSprite[si][4]; //当前sprite位置y
                int cspr_coll_t = CT_ContainerSprite[si][12]; //当前sprite碰撞类型
                int cspr_coll_e = CT_ContainerSprite[si][13]; //当前sprite边缘碰撞
                int cbox_x = CT_ContainerSprite[si][15]; //sprite碰撞盒位置x
                int cbox_y = CT_ContainerSprite[si][16]; //sprite碰撞盒位置y
                int cbox_w = CT_ContainerSprite[si][17]; //sprite碰撞盒宽度
                int cbox_h = CT_ContainerSprite[si][18]; //sprite碰撞盒高度
                int cbox_cx = CT_ContainerSprite[si][19]; //sprite碰撞盒中心位置x
                int cbox_cy = CT_ContainerSprite[si][20]; //sprite碰撞盒中心位置y
                int cbox_r = CT_ContainerSprite[si][21]; //sprite碰撞盒半径
                if(spr_l == cspr_l) //如果sprite层等于碰撞的sprite层
                {
                    if(cspr_coll_t == 0) //如果碰撞的sprite碰撞类型是矩形碰撞
                    {
                        if(spr_x + box_x + box_w > cspr_x + cbox_x && spr_x + box_x < cspr_x + cbox_x + cbox_w && spr_y + box_y + box_h > cspr_y + cbox_y && spr_y + box_y < cspr_y + cbox_y + cbox_h) //如果sprite图像与碰撞的sprite图像相交
                        {
                            if(cspr_coll_e == 0) //如果碰撞的sprite不是边缘碰撞
                            {
                                return 1;
                            }
                            if(cspr_coll_e == 1) //如果碰撞的sprite是边缘碰撞
                            {
                                int spr_lx = CT_ContainerSprite[CT_TaskRunIndex][10]; //sprite上次位置x
                                int spr_ly = CT_ContainerSprite[CT_TaskRunIndex][11]; //sprite上次位置y
                                int dx = abs(spr_lx-spr_x);
                                int dy = -abs(spr_ly-spr_y);
                                int sx = spr_x < spr_lx ? 1 : -1;
                                int sy = spr_y < spr_ly ? 1 : -1;
                                int dd = dx + dy;
                                int dd2;
                                while(1)
                                {
                                    if(!(spr_x + box_x + box_w > cspr_x + cbox_x && spr_x + box_x < cspr_x + cbox_x + cbox_w && spr_y + box_y + box_h > cspr_y + cbox_y && spr_y + box_y < cspr_y + cbox_y + cbox_h)) //如果sprite图像与碰撞的sprite图像不相交
                                    {
                                        CT_ContainerSprite[CT_TaskRunIndex][3] = spr_x; //设置sprite位置
                                        CT_ContainerSprite[CT_TaskRunIndex][4] = spr_y; //设置sprite位置
                                        break;
                                    }
                                    if(spr_x == spr_lx && spr_y == spr_ly)
                                    {
                                        break;
                                    }
                                    dd2 = 2 * dd;
                                    if(dd2 >= dy)
                                    {
                                        dd += dy;
                                        spr_x += sx;
                                    }
                                    if(dd2 <= dx)
                                    {
                                        dd += dx;
                                        spr_y += sy;
                                    }
                                }
                                CT_Repaint = 1; //请求重绘
                                return 1;
                            }
                        }
                    }
                    if(cspr_coll_t == 1) //如果碰撞的sprite碰撞类型是圆形碰撞
                    {
                        int spr_distance = sqrt(pow((spr_x+box_cx)-(cspr_x+cbox_cx),2)+pow((spr_y+box_cy)-(cspr_y+cbox_cy),2));
                        if(spr_distance < box_r + cbox_r) //如果sprite图像在碰撞的sprite图像半径内
                        {
                            if(cspr_coll_e == 0) //如果碰撞的sprite不是边缘碰撞
                            {
                                return 1;
                            }
                            if(cspr_coll_e == 1) //如果碰撞的sprite是边缘碰撞
                            {
                                int spr_lx = CT_ContainerSprite[CT_TaskRunIndex][10]; //sprite上次位置x
                                int spr_ly = CT_ContainerSprite[CT_TaskRunIndex][11]; //sprite上次位置y
                                int dx = abs(spr_lx-spr_x);
                                int dy = -abs(spr_ly-spr_y);
                                int sx = spr_x < spr_lx ? 1 : -1;
                                int sy = spr_y < spr_ly ? 1 : -1;
                                int dd = dx + dy;
                                int dd2;
                                while(1)
                                {
                                    int spr_distance = sqrt(pow((spr_x+box_cx)-(cspr_x+cbox_cx),2)+pow((spr_y+box_cy)-(cspr_y+cbox_cy),2));
                                    if(spr_distance >= box_r + cbox_r) //如果sprite图像在碰撞的sprite图像半径内
                                    {
                                        CT_ContainerSprite[CT_TaskRunIndex][3] = spr_x; //设置sprite位置
                                        CT_ContainerSprite[CT_TaskRunIndex][4] = spr_y; //设置sprite位置
                                        break;
                                    }
                                    if(spr_x == spr_lx && spr_y == spr_ly)
                                    {
                                        break;
                                    }
                                    dd2 = 2 * dd;
                                    if(dd2 >= dy)
                                    {
                                        dd += dy;
                                        spr_x += sx;
                                    }
                                    if(dd2 <= dx)
                                    {
                                        dd += dx;
                                        spr_y += sy;
                                    }
                                }
                                CT_Repaint = 1; //请求重绘
                                return 1;
                            }
                        }
                    }
                }
            }
        }
    }              
    return 0;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 索引Sprite设置矩形碰撞盒
//--------------------------------------------------------------------
void isprite_set_rectbox(int si,int x,int y,int w,int h)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(CT_ContainerSprite[si][0] != NULL) //如果列表中sprite不为空
        {
            CT_ContainerSprite[si][15] = x; //sprite碰撞盒位置x
            CT_ContainerSprite[si][16] = y; //sprite碰撞盒位置y
            CT_ContainerSprite[si][17] = w; //sprite碰撞盒宽度
            CT_ContainerSprite[si][18] = h; //sprite碰撞盒高度
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 索引Sprite设置圆形碰撞盒
//--------------------------------------------------------------------
void isprite_set_cirbox(int si,int cx,int cy,int r)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(CT_ContainerSprite[si][0] != NULL) //如果列表中sprite不为空
        {
            CT_ContainerSprite[si][19] = cx; //sprite碰撞盒中心位置x
            CT_ContainerSprite[si][20] = cy; //sprite碰撞盒中心位置y
            CT_ContainerSprite[si][21] = r; //sprite碰撞盒半径
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// sprite检查
//--------------------------------------------------------------------
int isprite_check(int x,int y,int si)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(CT_ContainerSprite[si][0] != NULL) //如果当前sprite不为空
        {
            int spr_x = CT_ContainerSprite[si][3]; //sprite位置x
            int spr_y = CT_ContainerSprite[si][4]; //sprite位置y
            int spr_collw = CT_ContainerSprite[si][8]; //sprite碰撞区宽度
            int spr_collh = CT_ContainerSprite[si][9]; //sprite碰撞区高度
            if(x >= spr_x && y >= spr_y && x < spr_x + spr_collw && y < spr_y + spr_collh) //如果sprite在指定位置中
            {
                return 1;
            }
        }
    }
    return 0;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 所有Sprite设置矩形碰撞盒
//--------------------------------------------------------------------
void sprite_all_set_rectbox(CTsprite *ctspr,int x,int y,int w,int h)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        int id = ctspr->id; //sprite id
        if(ctspr->type == -2) //如果对象是sprite
        {
            int i;
            for(i=0;i<CT_ContainerSpriteLength;i++) //遍历sprite容器
            {
                if(CT_ContainerSprite[i][0] != NULL) //如果当前sprite不为空
                {
                    int spr_id = CT_ContainerSprite[i][1]; //当前sprite id
                    if(spr_id == id) //如果当前sprite id等于sprite id
                    {
                        CT_ContainerSprite[i][15] = x; //当前sprite碰撞盒位置x
                        CT_ContainerSprite[i][16] = y; //当前sprite碰撞盒位置y
                        CT_ContainerSprite[i][17] = w; //当前sprite碰撞盒宽度
                        CT_ContainerSprite[i][18] = h; //当前sprite碰撞盒高度
                    }
                }
            }
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 所有Sprite设置圆形碰撞盒
//--------------------------------------------------------------------
void sprite_all_set_cirbox(CTsprite *ctspr,int cx,int cy,int r)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        int id = ctspr->id; //sprite id
        if(ctspr->type == -2) //如果对象是sprite
        {
            int i;
            for(i=0;i<CT_ContainerSpriteLength;i++) //遍历sprite容器
            {
                if(CT_ContainerSprite[i][0] != NULL) //如果当前sprite不为空
                {
                    int spr_id = CT_ContainerSprite[i][1]; //当前sprite id
                    if(spr_id == id) //如果当前sprite id等于sprite id
                    {
                        CT_ContainerSprite[i][19] = cx; //当前sprite碰撞盒中心位置x
                        CT_ContainerSprite[i][20] = cy; //当前sprite碰撞盒中心位置y
                        CT_ContainerSprite[i][21] = r; //当前sprite碰撞盒半径
                    }
                }
            }
        }
    }
}
//--------------------------------------------------------------------