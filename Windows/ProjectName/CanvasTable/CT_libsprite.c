#define _CRT_SECURE_NO_WARNINGS

#include "CanvasTableSYS.h"
#include <math.h>

extern int *CT_ContainerSprite;
extern int CT_SpriteLayerStatusList[32];
extern int CT_ContainerSpriteLength;
extern int CT_ContainerMaxLength_Sprite;
extern unsigned int *CT_ResourceArgList;
extern int CT_SpriteId;
extern int CT_RunnerStatus;
extern int CT_TaskRunIndex;
extern int CT_Repaint;

//--------------------------------------------------------------------
// 新建Sprite对象
//--------------------------------------------------------------------
void CTsprite_new(CTsprite *ctspr)
{
    ctspr->image = -1; //sprite图像索引
    ctspr->layer = 0; //sprite层
    ctspr->subnum = 1; //sprite子图像数量
    ctspr->subimg = 0; //sprite子图像索引
    ctspr->collision_type = 0; //sprite碰撞方式
    ctspr->collision_edge = 0; //sprite边缘碰撞
    ctspr->cbox_x = 0; //碰撞盒位置x
    ctspr->cbox_y = 0; //碰撞盒位置y
    ctspr->cbox_w = 0; //碰撞盒宽度
    ctspr->cbox_h = 0; //碰撞盒高度
    ctspr->cbox_cx = 0; //碰撞盒中心位置x
    ctspr->cbox_cy = 0; //碰撞盒中心位置y
    ctspr->cbox_r = 0; //碰撞盒半径
    ctspr->color = 0xffffff; //sprite颜色
    ctspr->colora = 255; //sprite透明度
    ctspr->main = NULL; //sprite程序main函数
    ctspr->loop = NULL; //sprite程序loop函数
//--------------------------------------------------------------------
    ctspr->id = CT_SpriteGetId(); //sprite id
    ctspr->type = -2; //类型
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// sprite创建
//--------------------------------------------------------------------
int sprite_create(int x,int y,CTsprite *ctspr)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        int id = ctspr->id; //sprite id
        if(ctspr->type == -2) //如果对象类型是sprite
        {
            int image = ctspr->image; //sprite图像
            unsigned int image_w = 0; //图像宽度
            unsigned int image_h = 0; //图像高度
            int subimg_w = 0; //sprite子图像宽度
            int subimg_h = 0; //sprite子图像高度
            int conspr_i = CT_ContainerSpriteLength; //sprite数据索引
            int layer = ctspr->layer; //sprite层
            int subimg = ctspr->subimg; //sprite默认子图像索引
            int subnum = ctspr->subnum; //sprite子图像数量
            int coll_t = ctspr->collision_type; //sprite碰撞类型
            int coll_e = ctspr->collision_edge; //sprite边缘碰撞
            int box_x = ctspr->cbox_x; //sprite碰撞盒位置x
            int box_y = ctspr->cbox_y; //sprite碰撞盒位置y
            int box_w = ctspr->cbox_w; //sprite碰撞盒宽度
            int box_h = ctspr->cbox_h; //sprite碰撞盒高度
            int box_cx = ctspr->cbox_cx; //sprite碰撞盒中心位置x
            int box_cy = ctspr->cbox_cy; //sprite碰撞盒中心位置y
            int box_r = ctspr->cbox_r; //sprite半径
            int color = ctspr->color; //sprite颜色
            int colora = ctspr->colora; //sprite透明度
            if(image != -1) //如果sprite有图像
            {
                size_t ldata_i = image * 5; //资源参数数据索引
                int res_type = CT_ResourceArgList[ldata_i+1]; //获取资源类型
                if(res_type == 0) //如果资源类型是图像
                {
                    image_w = CT_ResourceArgList[ldata_i+2]; //sprite图像宽度
                    image_h = CT_ResourceArgList[ldata_i+3]; //sprite图像高度
                    subimg_w = image_w / subnum; //sprite子图像宽度
                    subimg_h = image_h; //子图像高度
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
                size_t data_i;
                for(i=0;i<CT_ContainerSpriteLength;i++) //遍历sprite容器
                {
                    data_i = i * 25; //sprite数据索引
                    if(CT_ContainerSprite[data_i] == 0) //如果当前sprite为空
                    {
                        //将sprite数据写入sprite容器
                        CT_ContainerSprite[data_i] = 1;
                        CT_ContainerSprite[data_i+1] = id;
                        CT_ContainerSprite[data_i+2] = layer;
                        CT_ContainerSprite[data_i+3] = x;
                        CT_ContainerSprite[data_i+4] = y;
                        CT_ContainerSprite[data_i+5] = image;
                        CT_ContainerSprite[data_i+6] = subimg;
                        CT_ContainerSprite[data_i+7] = subnum;
                        CT_ContainerSprite[data_i+8] = subimg_w;
                        CT_ContainerSprite[data_i+9] = subimg_h;
                        CT_ContainerSprite[data_i+10] = x;
                        CT_ContainerSprite[data_i+11] = y;
                        CT_ContainerSprite[data_i+12] = coll_t;
                        CT_ContainerSprite[data_i+13] = coll_e;
                        CT_ContainerSprite[data_i+15] = box_x;
                        CT_ContainerSprite[data_i+16] = box_y;
                        CT_ContainerSprite[data_i+17] = box_w;
                        CT_ContainerSprite[data_i+18] = box_h;
                        CT_ContainerSprite[data_i+19] = box_cx;
                        CT_ContainerSprite[data_i+20] = box_cy;
                        CT_ContainerSprite[data_i+21] = box_r;
                        CT_ContainerSprite[data_i+22] = color * 0x100 + colora;
                        CT_TaskCreate(i,ctspr->main,ctspr->loop); //创建sprite子程序
                        CT_SpriteLayerStatusList[layer] = 1; //将当前sprite层的状态标记为1
                        CT_Repaint = 1; //请求重绘
                        return i; //中断
                    }
                }
                data_i = conspr_i * 25; //sprite数据索引
                 //将sprite数据写入sprite容器
                CT_ContainerSprite[data_i] = 1;
                CT_ContainerSprite[data_i+1] = id;
                CT_ContainerSprite[data_i+2] = layer;
                CT_ContainerSprite[data_i+3] = x;
                CT_ContainerSprite[data_i+4] = y;
                CT_ContainerSprite[data_i+5] = image;
                CT_ContainerSprite[data_i+6] = subimg;
                CT_ContainerSprite[data_i+7] = subnum;
                CT_ContainerSprite[data_i+8] = subimg_w;
                CT_ContainerSprite[data_i+9] = subimg_h;
                CT_ContainerSprite[data_i+10] = x;
                CT_ContainerSprite[data_i+11] = y;
                CT_ContainerSprite[data_i+12] = coll_t;
                CT_ContainerSprite[data_i+13] = coll_e;
                CT_ContainerSprite[data_i+15] = box_x;
                CT_ContainerSprite[data_i+16] = box_y;
                CT_ContainerSprite[data_i+17] = box_w;
                CT_ContainerSprite[data_i+18] = box_h;
                CT_ContainerSprite[data_i+19] = box_cx;
                CT_ContainerSprite[data_i+20] = box_cy;
                CT_ContainerSprite[data_i+21] = box_r;
                CT_ContainerSprite[data_i+22] = color * 0x100 + colora;
                CT_TaskCreate(conspr_i,ctspr->main,ctspr->loop); //创建sprite子程序
                CT_ContainerSpriteLength ++; //sprite容器长度 +1
                if(CT_ContainerSpriteLength == CT_ContainerMaxLength_Sprite) //如果sprite容器长度等于sprite容器最大长度
                {
                    CT_ContainerPreallocated_Sprite(CT_ContainerSpriteLength+100); //为sprite容器重新分配内存
                }
                CT_SpriteLayerStatusList[layer] = 1; //将当前sprite层的状态标记为1
                CT_Repaint = 1; //请求重绘
                return conspr_i; //中断
            }
            else
            {
                cprint("ERROR : The sprite subimage is error.");
            }
        }
    }
    return -1;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// sprite销毁
//--------------------------------------------------------------------
void sprite_destroy()
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        size_t data_i = CT_TaskRunIndex * 25; //sprite数据索引
        int i;
        int i2;
        for(i=0;i<25;i++) //遍历sprite数据
        {
            CT_ContainerSprite[data_i+i] = 0; //清除sprite数据
        }
        CT_TaskDestroy(CT_TaskRunIndex); //销毁sprite子程序
        for(i2=CT_ContainerSpriteLength-1;i2>=-1;i2--) //反向遍历sprite容器
        {
            if(i2 >= 0) //如果sprite索引大于等于0
            {
                if(CT_ContainerSprite[i2*25] != 0) //如果当前sprite不为空
                {
                    CT_ContainerSpriteLength = i2 + 1; //确定sprite容器长度
                    break; //中断
                }
            }
            else
            {
                CT_ContainerSpriteLength = 0; //sprite容器长度为0
            }
        }
        CT_Repaint = 1; //请求重绘
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// sprite位置x
//--------------------------------------------------------------------
int sprite_x()
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        size_t data_i = CT_TaskRunIndex * 25; //sprite数据索引
        if(CT_ContainerSprite[data_i] != 0) //如果sprite不为空
        {
            return CT_ContainerSprite[data_i+3]; //sprite位置x
        }
    }
    return -1;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// sprite位置y
//--------------------------------------------------------------------
int sprite_y()
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        size_t data_i = CT_TaskRunIndex * 25; //sprite数据索引
        if(CT_ContainerSprite[data_i] != 0) //如果sprite不为空
        {
            return CT_ContainerSprite[data_i+4]; //sprite位置x
        }
    }
    return -1;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// sprite位置x设置
//--------------------------------------------------------------------
void sprite_set_x(int x)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        size_t data_i = CT_TaskRunIndex * 25; //sprite数据索引
        if(CT_ContainerSprite[data_i] != 0) //如果sprite不为空
        {
            CT_ContainerSprite[data_i+3] = x; //设置sprite位置x
            CT_Repaint = 1; //请求重绘
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// sprite位置y设置
//--------------------------------------------------------------------
void sprite_set_y(int y)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        size_t data_i = CT_TaskRunIndex * 25; //sprite数据索引
        if(CT_ContainerSprite[data_i] != 0) //如果sprite不为空
        {
            CT_ContainerSprite[data_i+4] = y; //设置sprite位置x
            CT_Repaint = 1; //请求重绘
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// sprite层设置
//--------------------------------------------------------------------
void sprite_set_layer(int l)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        size_t data_i = CT_TaskRunIndex * 25; //sprite数据索引
        if(CT_ContainerSprite[data_i] != 0) //如果sprite不为空
        {
            CT_ContainerSprite[data_i+2] = l; //设置sprite层
            CT_SpriteLayerStatusList[l] = 1; //将sprite层状态标记为1
            CT_Repaint = 1; //请求重绘
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 碰撞检测
//--------------------------------------------------------------------
int sprite_collision(CTsprite *ctspr)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(ctspr->type == -2) //如果对象是sprite
        {
            size_t data_i = CT_TaskRunIndex * 25; //sprite数据索引
            if(CT_ContainerSprite[data_i] != 0) //如果sprite不为空
            {
                int collision = 0; //有碰撞
                int spr_x = CT_ContainerSprite[data_i+3]; //sprite位置x
                int spr_y = CT_ContainerSprite[data_i+4]; //sprite位置y
                int box_x = CT_ContainerSprite[data_i+15]; //sprite碰撞盒位置x
                int box_y = CT_ContainerSprite[data_i+16]; //sprite碰撞盒位置y
                int box_w = CT_ContainerSprite[data_i+17]; //sprite碰撞盒宽度
                int box_h = CT_ContainerSprite[data_i+18]; //sprite碰撞盒高度
                int box_cx = CT_ContainerSprite[data_i+19]; //sprite碰撞盒中心位置x
                int box_cy = CT_ContainerSprite[data_i+20]; //sprite碰撞盒中心位置y
                int box_r = CT_ContainerSprite[data_i+21]; //sprite碰撞盒半径
                int i;
                size_t data_i2;
                for(i=0;i<CT_ContainerSpriteLength;i++) //遍历sprite容器
                {
                    data_i2 = i * 25; //sprite数据索引
                    if(CT_ContainerSprite[data_i] != 0) //如果当前sprite不为空
                    {
                        int cspr_id = CT_ContainerSprite[data_i2+1]; //当前sprite id
                        int cspr_x = CT_ContainerSprite[data_i2+3]; //当前sprite位置x
                        int cspr_y = CT_ContainerSprite[data_i2+4]; //当前sprite位置y
                        int cspr_coll_t = CT_ContainerSprite[data_i2+12]; //当前sprite碰撞类型
                        int cspr_coll_e = CT_ContainerSprite[data_i2+13]; //当前sprite边缘碰撞
                        int cbox_x = CT_ContainerSprite[data_i2+15]; //当前sprite碰撞盒位置x
                        int cbox_y = CT_ContainerSprite[data_i2+16]; //当前sprite碰撞盒位置y
                        int cbox_w = CT_ContainerSprite[data_i2+17]; //当前sprite碰撞盒宽度
                        int cbox_h = CT_ContainerSprite[data_i2+18]; //当前sprite碰撞盒高度
                        int cbox_cx = CT_ContainerSprite[data_i2+19]; //当前sprite碰撞盒中心位置x
                        int cbox_cy = CT_ContainerSprite[data_i2+20]; //当前sprite碰撞盒中心位置y
                        int cbox_r = CT_ContainerSprite[data_i2+21]; //当前sprite碰撞盒半径
                        if(cspr_id == ctspr->id) //如果当前sprite id等于碰撞的sprite id
                        {
                            if(cspr_coll_t == 0) //如果碰撞的sprite碰撞类型是矩形碰撞
                            {
                                if(spr_x + box_x + box_w > cspr_x + cbox_x && spr_x + box_x < cspr_x + cbox_x + cbox_w && spr_y + box_y + box_h > cspr_y + cbox_y && spr_y + box_y < cspr_y + cbox_y + cbox_h) //如果sprite图像与碰撞的sprite图像相交
                                {
                                    collision = 1; //有碰撞
                                    if(cspr_coll_e == 1) //如果碰撞的sprite是边缘碰撞
                                    {
                                        int spr_lx = CT_ContainerSprite[data_i+10]; //sprite上次位置x
                                        int spr_ly = CT_ContainerSprite[data_i+11]; //sprite上次位置y
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
                                                CT_ContainerSprite[data_i+3] = spr_x; //设置sprite位置
                                                CT_ContainerSprite[data_i+4] = spr_y; //设置sprite位置
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
                                    }
                                }
                            }
                            if(cspr_coll_t == 1) //如果碰撞的sprite碰撞类型是圆形碰撞
                            {
                                double spr_distance = sqrt(pow((spr_x+box_cx)-(cspr_x+cbox_cx),2)+pow((spr_y+box_cy)-(cspr_y+cbox_cy),2));
                                if(spr_distance < box_r + cbox_r) //如果sprite图像在碰撞的sprite图像半径内
                                {
                                    collision = 1; //有碰撞
                                    if(cspr_coll_e == 1) //如果碰撞的sprite是边缘碰撞
                                    {
                                        int spr_lx = CT_ContainerSprite[data_i+10]; //sprite上次位置x
                                        int spr_ly = CT_ContainerSprite[data_i+11]; //sprite上次位置y
                                        int dx = abs(spr_lx-spr_x);
                                        int dy = -abs(spr_ly-spr_y);
                                        int sx = spr_x < spr_lx ? 1 : -1;
                                        int sy = spr_y < spr_ly ? 1 : -1;
                                        int dd = dx + dy;
                                        int dd2;
                                        while(1)
                                        {
                                            double spr_distance = sqrt(pow((spr_x+box_cx)-(cspr_x+cbox_cx),2)+pow((spr_y+box_cy)-(cspr_y+cbox_cy),2));
                                            if(spr_distance >= box_r + cbox_r) //如果sprite图像在碰撞的sprite图像半径内
                                            {
                                                CT_ContainerSprite[data_i+3] = spr_x; //设置sprite位置
                                                CT_ContainerSprite[data_i+4] = spr_y; //设置sprite位置
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
                                    }
                                }
                            }
                        }
                    }
                }
                if(collision == 1) //如果有碰撞
                {
                    CT_Repaint = 1; //请求重绘
                    return 1;
                }
            }              
        }
        else
        {
            cprint("ERROR : Not is the sprite object.");
        }
    }
    return 0;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// sprite设置矩形碰撞盒
//--------------------------------------------------------------------
void sprite_set_rectbox(int x,int y,int w,int h)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        size_t data_i = CT_TaskRunIndex * 25; //sprite数据索引
        if(CT_ContainerSprite[data_i] != 0) //如果列表中sprite不为空
        {
            CT_ContainerSprite[data_i+15] = x; //sprite碰撞盒位置x
            CT_ContainerSprite[data_i+16] = y; //sprite碰撞盒位置y
            CT_ContainerSprite[data_i+17] = w; //sprite碰撞盒宽度
            CT_ContainerSprite[data_i+18] = h; //sprite碰撞盒高度
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// sprite设置圆形碰撞盒
//--------------------------------------------------------------------
void sprite_set_cirbox(int cx,int cy,int r)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        size_t data_i = CT_TaskRunIndex * 25; //sprite数据索引
        if(CT_ContainerSprite[data_i] != 0) //如果列表中sprite不为空
        {
            CT_ContainerSprite[data_i+19] = cx; //sprite碰撞盒中心位置x
            CT_ContainerSprite[data_i+20] = cy; //sprite碰撞盒中心位置y
            CT_ContainerSprite[data_i+21] = r; //sprite碰撞盒半径
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// sprite子图像设置
//--------------------------------------------------------------------
void sprite_set_subimg(int i)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        size_t data_i = CT_TaskRunIndex * 25; //sprite数据索引
        if(CT_ContainerSprite[data_i] != 0) //如果sprite不为空
        {
            int spr_subnum = CT_ContainerSprite[data_i+7]; //sprite子图像数量
            if(i <= spr_subnum - 1) //如果sprite子图像索引小于sprite子图像数量-1
            {
                CT_ContainerSprite[data_i+6] = i; //设置sprite子图像索引
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
// sprite颜色设置
//--------------------------------------------------------------------
void sprite_set_color(int col)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        size_t data_i = CT_TaskRunIndex * 25;
        if(CT_ContainerSprite[data_i] != 0) //如果sprite不为空
        {
            int alpha = CT_ContainerSprite[data_i+22] & 0x000000ff; //获取sprite透明度
            CT_ContainerSprite[data_i+22] = col * 0x100 + alpha; //设置sprite颜色
            CT_Repaint = 1; //请求重绘
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// sprite透明度设置
//--------------------------------------------------------------------
void sprite_set_colora(int alp)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        size_t data_i = CT_TaskRunIndex * 25;
        if(CT_ContainerSprite[data_i] != 0) //如果sprite不为空
        {
            int color = CT_ContainerSprite[data_i+22] & 0xffffff00; //获取sprite颜色
            CT_ContainerSprite[data_i+22] = color + alp; //设置sprite透明度
            CT_Repaint = 1; //请求重绘
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// sprite变量赋值 数字
//--------------------------------------------------------------------
void spritenum_set(int i,varn n)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        CT_TaskSetNum(CT_TaskRunIndex,i,n); //sprite程序数字变量赋值
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// sprite变量赋值 小数
//--------------------------------------------------------------------
void spritedec_set(int i,vard d)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        CT_TaskSetDec(CT_TaskRunIndex,i,d); //sprite程序小数变量赋值
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// sprite变量赋值 字符串
//--------------------------------------------------------------------
void spritestr_set(int i,char *s)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        CT_TaskSetStr(CT_TaskRunIndex,i,s); //sprite程序字符串变量赋值
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// sprite变量 数字
//--------------------------------------------------------------------
varn spritenum(int i)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        return CT_TaskNum(CT_TaskRunIndex,i); //sprite程序数字变量值
    }
    return 0;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// sprite变量 小数
//--------------------------------------------------------------------
vard spritedec(int i)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        return CT_TaskDec(CT_TaskRunIndex,i); //sprite程序小数变量值
    }
    return 0;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// sprite变量 字符串
//--------------------------------------------------------------------
char *spritestr(int i)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        return CT_TaskStr(CT_TaskRunIndex,i); //获取sprite程序字符串变量值
    }
    return NULL;
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
            size_t data_i;
            for(i=0;i<CT_ContainerSpriteLength;i++) //遍历sprite容器
            {
                data_i = i * 25; //sprite数据索引
                if(CT_ContainerSprite[data_i] != 0) //如果当前sprite不为空
                {
                    int spr_id = CT_ContainerSprite[data_i+1]; //当前sprite id
                    if(spr_id == id) //如果当前sprite id等于sprite id
                    {
                        int spr_x = CT_ContainerSprite[data_i+3]; //当前sprite位置x
                        int spr_y = CT_ContainerSprite[data_i+4]; //当前sprite位置y
                        int spr_box_x = CT_ContainerSprite[data_i+15]; //当前sprite碰撞盒位置x
                        int spr_box_y = CT_ContainerSprite[data_i+16]; //当前sprite碰撞盒位置y
                        int spr_box_w = CT_ContainerSprite[data_i+17]; //当前sprite碰撞盒宽度
                        int spr_box_h = CT_ContainerSprite[data_i+18]; //当前sprite碰撞盒高度
                        if(x >= spr_x + spr_box_x && x < spr_x + spr_box_x + spr_box_w && y >= spr_y + spr_box_y && y < spr_y + spr_box_y + spr_box_h) //如果当前sprite在指定位置中
                        {
                            return 1;
                        }
                    }
                }
            }
        }
        else
        {
            cprint("ERROR : Not is the sprite object.");
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
// 索引sprite销毁
//--------------------------------------------------------------------
void isprite_destroy(int si)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(si < CT_ContainerSpriteLength) //如果sprite索引小于sprite容器长度
        {
            size_t data_i = si * 25; //sprite数据索引
            if(CT_ContainerSprite[data_i] != 0) //如果指定索引的sprite不为空
            {
                int i;
                int i2;
                for(i=0;i<25;i++) //遍历sprite数据
                {
                    CT_ContainerSprite[data_i+i] = 0; //清空sprite
                }
                CT_TaskDestroy(si); //销毁sprite子程序
                for(i2=CT_ContainerSpriteLength-1;i2>=-1;i2--) //反向遍历sprite容器
                {
                    if(i2 >= 0) //如果sprite索引大于等于0
                    {
                        if(CT_ContainerSprite[i2*25] != 0) //如果当前sprite不为空
                        {
                            CT_ContainerSpriteLength = i2 + 1; //确定sprite容器长度
                            break; //中断
                        }
                    }
                    else
                    {
                        CT_ContainerSpriteLength = 0; //sprite容器长度为0
                    }
                }
                CT_Repaint = 1; //请求重绘
            }
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 索引sprite位置x
//--------------------------------------------------------------------
int isprite_x(int si)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(si < CT_ContainerSpriteLength) //如果sprite索引小于sprite容器长度
        {
            size_t data_i = si * 25; //sprite数据索引
            if(CT_ContainerSprite[data_i] != 0) //如果指定索引的sprite不为空
            {
                return CT_ContainerSprite[data_i+3]; //sprite位置x
            }
            else
            {
                cprint("ERROR : The sprite is null.");
            }
        }
    }
    return -1;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 索引sprite位置y
//--------------------------------------------------------------------
int isprite_y(int si)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(si < CT_ContainerSpriteLength) //如果sprite索引小于sprite容器长度
        {
            size_t data_i = si * 25; //sprite数据索引
            if(CT_ContainerSprite[data_i] != 0) //如果指定索引的sprite不为空
            {
                return CT_ContainerSprite[data_i+4]; //sprite位置y
            }
            else
            {
                cprint("ERROR : The sprite is null.");
            }
        }
    }
    return -1;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 索引sprite位置x设置
//--------------------------------------------------------------------
void isprite_set_x(int si,int x)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(si < CT_ContainerSpriteLength) //如果sprite索引小于sprite容器长度
        {
            size_t data_i = si * 25; //sprite数据索引
            if(CT_ContainerSprite[data_i] != 0) //如果指定索引的sprite不为空
            {
                CT_ContainerSprite[data_i+3] = x; //设置sprite位置x
                CT_Repaint = 1; //请求重绘
            }
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 索引Sprite位置y设置
//--------------------------------------------------------------------
void isprite_set_y(int si,int y)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(si < CT_ContainerSpriteLength) //如果sprite索引小于sprite容器长度
        {
            size_t data_i = si * 25; //sprite数据索引
            if(CT_ContainerSprite[data_i] != 0) //如果指定索引的sprite不为空
            {
                CT_ContainerSprite[data_i+4] = y; //设置sprite位置y
                CT_Repaint = 1; //请求重绘
            }
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 索引sprite层设置
//--------------------------------------------------------------------
void isprite_set_layer(int si,int l)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(si < CT_ContainerSpriteLength) //如果sprite索引小于sprite容器长度
        {
            size_t data_i = si * 25; //sprite数据索引
            if(CT_ContainerSprite[data_i] != 0) //如果指定索引的sprite不为空
            {
                CT_ContainerSprite[data_i+2] = l; //设置指定索引sprite层
                CT_SpriteLayerStatusList[l] = 1; //将sprite层状态标记为1
                CT_Repaint = 1; //请求重绘
            }
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 索引sprite碰撞检测
//--------------------------------------------------------------------
int isprite_collision(int si)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(si < CT_ContainerSpriteLength) //如果sprite索引小于sprite容器长度
        {
            size_t data_i = CT_TaskRunIndex * 25; //sprite数据索引
            if(CT_ContainerSprite[data_i] != 0) //如果sprite不为空
            {
                int spr_x = CT_ContainerSprite[data_i+3]; //sprite位置x
                int spr_y = CT_ContainerSprite[data_i+4]; //sprite位置y
                int box_x = CT_ContainerSprite[data_i+15]; //sprite碰撞盒位置x
                int box_y = CT_ContainerSprite[data_i+16]; //sprite碰撞盒位置y
                int box_w = CT_ContainerSprite[data_i+17]; //sprite碰撞盒宽度
                int box_h = CT_ContainerSprite[data_i+18]; //sprite碰撞盒高度
                int box_cx = CT_ContainerSprite[data_i+19]; //sprite碰撞盒中心位置x
                int box_cy = CT_ContainerSprite[data_i+20]; //sprite碰撞盒中心位置y
                int box_r = CT_ContainerSprite[data_i+21]; //sprite碰撞盒半径
                size_t data_i2 = si * 25; //指定的sprite数据索引
                if(CT_ContainerSprite[data_i2] != 0) //如果指定的sprite不为空
                {
                    int cspr_x = CT_ContainerSprite[data_i2+3]; //指定的sprite位置x
                    int cspr_y = CT_ContainerSprite[data_i2+4]; //指定的sprite位置y
                    int cspr_coll_t = CT_ContainerSprite[data_i2+12]; //指定的sprite碰撞类型
                    int cspr_coll_e = CT_ContainerSprite[data_i2+13]; //指定的sprite边缘碰撞
                    int cbox_x = CT_ContainerSprite[data_i2+15]; //指定的sprite碰撞盒位置x
                    int cbox_y = CT_ContainerSprite[data_i2+16]; //指定的sprite碰撞盒位置y
                    int cbox_w = CT_ContainerSprite[data_i2+17]; //指定的sprite碰撞盒宽度
                    int cbox_h = CT_ContainerSprite[data_i2+18]; //指定的sprite碰撞盒高度
                    int cbox_cx = CT_ContainerSprite[data_i2+19]; //指定的sprite碰撞盒中心位置x
                    int cbox_cy = CT_ContainerSprite[data_i2+20]; //指定的sprite碰撞盒中心位置y
                    int cbox_r = CT_ContainerSprite[data_i2+21]; //指定的sprite碰撞盒半径
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
                                int spr_lx = CT_ContainerSprite[data_i+10]; //sprite上次位置x
                                int spr_ly = CT_ContainerSprite[data_i+11]; //sprite上次位置y
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
                                        CT_ContainerSprite[data_i+3] = spr_x; //设置sprite位置
                                        CT_ContainerSprite[data_i+4] = spr_y; //设置sprite位置
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
                        double spr_distance = sqrt(pow((spr_x+box_cx)-(cspr_x+cbox_cx),2)+pow((spr_y+box_cy)-(cspr_y+cbox_cy),2));
                        if(spr_distance < box_r + cbox_r) //如果sprite图像在碰撞的sprite图像半径内
                        {
                            if(cspr_coll_e == 0) //如果碰撞的sprite不是边缘碰撞
                            {
                                return 1;
                            }
                            if(cspr_coll_e == 1) //如果碰撞的sprite是边缘碰撞
                            {
                                int spr_lx = CT_ContainerSprite[data_i+10]; //sprite上次位置x
                                int spr_ly = CT_ContainerSprite[data_i+11]; //sprite上次位置y
                                int dx = abs(spr_lx-spr_x);
                                int dy = -abs(spr_ly-spr_y);
                                int sx = spr_x < spr_lx ? 1 : -1;
                                int sy = spr_y < spr_ly ? 1 : -1;
                                int dd = dx + dy;
                                int dd2;
                                while(1)
                                {
                                    double spr_distance = sqrt(pow((spr_x+box_cx)-(cspr_x+cbox_cx),2)+pow((spr_y+box_cy)-(cspr_y+cbox_cy),2));
                                    if(spr_distance >= box_r + cbox_r) //如果sprite图像在碰撞的sprite图像半径内
                                    {
                                        CT_ContainerSprite[data_i+3] = spr_x; //设置sprite位置
                                        CT_ContainerSprite[data_i+4] = spr_y; //设置sprite位置
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
// 索引sprite设置矩形碰撞盒
//--------------------------------------------------------------------
void isprite_set_rectbox(int si,int x,int y,int w,int h)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(si < CT_ContainerSpriteLength) //如果sprite索引小于sprite容器长度
        {
            size_t data_i = si * 25; //sprite数据索引
            if(CT_ContainerSprite[data_i] != 0) //如果列表中sprite不为空
            {
                CT_ContainerSprite[data_i+15] = x; //sprite碰撞盒位置x
                CT_ContainerSprite[data_i+16] = y; //sprite碰撞盒位置y
                CT_ContainerSprite[data_i+17] = w; //sprite碰撞盒宽度
                CT_ContainerSprite[data_i+18] = h; //sprite碰撞盒高度
            }
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 索引sprite设置圆形碰撞盒
//--------------------------------------------------------------------
void isprite_set_cirbox(int si,int cx,int cy,int r)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(si < CT_ContainerSpriteLength) //如果sprite索引小于sprite容器长度
        {
            size_t data_i = si * 25; //sprite数据索引
            if(CT_ContainerSprite[data_i] != 0) //如果列表中sprite不为空
            {
                CT_ContainerSprite[data_i+19] = cx; //sprite碰撞盒中心位置x
                CT_ContainerSprite[data_i+20] = cy; //sprite碰撞盒中心位置y
                CT_ContainerSprite[data_i+21] = r; //sprite碰撞盒半径
            }
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 索引sprite检查
//--------------------------------------------------------------------
int isprite_check(int x,int y,int si)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(si < CT_ContainerSpriteLength) //如果sprite索引小于sprite容器长度
        {
            size_t data_i = si * 25; //sprite数据索引
            if(CT_ContainerSprite[data_i] != 0) //如果当前sprite不为空
            {
                int spr_x = CT_ContainerSprite[data_i+3]; //sprite位置x
                int spr_y = CT_ContainerSprite[data_i+4]; //sprite位置y
                int spr_box_x = CT_ContainerSprite[data_i+15]; //sprite碰撞盒位置x
                int spr_box_y = CT_ContainerSprite[data_i+16]; //sprite碰撞盒位置y
                int spr_box_w = CT_ContainerSprite[data_i+17]; //sprite碰撞盒宽度
                int spr_box_h = CT_ContainerSprite[data_i+18]; //sprite碰撞盒高度
                if(x >= spr_x + spr_box_x && x < spr_x + spr_box_x + spr_box_w && y >= spr_y + spr_box_y && y < spr_y + spr_box_y + spr_box_h) //如果当前sprite在指定位置中
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 索引sprite子图像设置
//--------------------------------------------------------------------
void isprite_set_subimg(int si,int i)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(si < CT_ContainerSpriteLength) //如果sprite索引小于sprite容器长度
        {
            size_t data_i = si * 25; //sprite数据索引
            if(CT_ContainerSprite[data_i] != 0) //如果指定索引的sprite不为空
            {
                int spr_subnum = CT_ContainerSprite[data_i+7]; //sprite子图像数量
                if(i <= spr_subnum - 1) //如果sprite子图像索引小于sprite子图像数量-1
                {
                    CT_ContainerSprite[data_i+6] = i; //设置sprite子图像索引
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
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 索引sprite颜色设置
//--------------------------------------------------------------------
void isprite_set_color(int si,int col)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(si < CT_ContainerSpriteLength) //如果sprite索引小于sprite容器长度
        {
            size_t data_i = si * 25; //sprite数据索引
            if(CT_ContainerSprite[data_i] != 0) //如果指定索引的sprite不为空
            {
                int alpha = CT_ContainerSprite[data_i+22] & 0x000000ff; //获取sprite透明度
                CT_ContainerSprite[data_i+22] = col * 0x100 + alpha; //设置指定索引sprite颜色
                CT_Repaint = 1; //请求重绘
            }
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 索引sprite透明度设置
//--------------------------------------------------------------------
void isprite_set_colora(int si,int alp)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(si < CT_ContainerSpriteLength) //如果sprite索引小于sprite容器长度
        {
            size_t data_i = si * 25; //sprite数据索引
            if(CT_ContainerSprite[data_i] != 0) //如果指定索引的sprite不为空
            {
                int color = CT_ContainerSprite[data_i+22] & 0xffffff00; //获取sprite颜色
                CT_ContainerSprite[data_i+22] = color + alp; //设置指定索引sprite透明度
                CT_Repaint = 1; //请求重绘
            }
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 索引sprite变量赋值 数字
//--------------------------------------------------------------------
void ispritenum_set(int si,int i,varn n)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(si < CT_ContainerSpriteLength) //如果sprite索引小于sprite容器长度
        {
            if(CT_ContainerSprite[si*25] != 0) //如果指定索引的sprite不为空
            {
                CT_TaskSetNum(si,i,n); //sprite程序数字变量赋值
            }
            else
            {
                cprint("ERROR : The sprite is null.");
            }
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 索引sprite变量赋值 小数
//--------------------------------------------------------------------
void ispritedec_set(int si,int i,vard d)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(si < CT_ContainerSpriteLength) //如果sprite索引小于sprite容器长度
        {
            if(CT_ContainerSprite[si*25] != 0) //如果指定索引的sprite不为空
            {
                CT_TaskSetDec(si,i,d); //sprite程序小数变量赋值
            }
            else
            {
                cprint("ERROR : The sprite is null.");
            }
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 索引sprite变量赋值 字符串
//--------------------------------------------------------------------
void ispritestr_set(int si,int i,char *s)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(si < CT_ContainerSpriteLength) //如果sprite索引小于sprite容器长度
        {
            if(CT_ContainerSprite[si*25] != 0) //如果sprite不为空
            {
                CT_TaskSetStr(si,i,s); //sprite程序字符串变量赋值
            }
            else
            {
                cprint("ERROR : The sprite is null.");
            }
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 索引sprite变量 数字
//--------------------------------------------------------------------
varn ispritenum(int si,int i)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(si < CT_ContainerSpriteLength) //如果sprite索引小于sprite容器长度
        {
            if(CT_ContainerSprite[si*25] != 0) //如果sprite不为空
            {
                return CT_TaskNum(si,i); //sprite程序数字变量值
            }
            else
            {
                cprint("ERROR : The sprite is null.");
            }
        }
    }
    return 0;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 索引sprite变量 小数
//--------------------------------------------------------------------
vard ispritedec(int si,int i)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(si < CT_ContainerSpriteLength) //如果sprite索引小于sprite容器长度
        {
            if(CT_ContainerSprite[si*25] != 0) //如果sprite不为空
            {
                return CT_TaskDec(si,i); //sprite程序小数变量值
            }
            else
            {
                cprint("ERROR : The sprite is null.");
            }
        }
    }
    return 0;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 索引sprite变量 字符串
//--------------------------------------------------------------------
char *ispritestr(int si,int i)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        if(si < CT_ContainerSpriteLength) //如果sprite索引小于sprite容器长度
        {
            if(CT_ContainerSprite[si*25] != 0) //如果列表中sprite不为空
            {
                return CT_TaskStr(si,i); //获取sprite程序字符串变量值
            }
            else
            {
                cprint("ERROR : The sprite is null.");
            }
        }
    }
    return 0;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 所有sprite销毁
//--------------------------------------------------------------------
void sprite_destroy_all(CTsprite *ctspr)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        int id = ctspr->id; //sprite id
        if(ctspr->type == -2) //如果对象是sprite
        {
            int i;
            size_t data_i;
            int i2;
            for(i=0;i<CT_ContainerSpriteLength;i++) //遍历sprite容器
            {
                data_i = i * 25; //sprite数据索引
                if(CT_ContainerSprite[data_i] != 0) //如果当前sprite不为空
                {
                    int spr_id = CT_ContainerSprite[data_i+1]; //当前sprite id
                    if(spr_id == id) //如果当前sprite id等于sprite id
                    {
                        for(i2=0;i2<25;i2++) //遍历sprite数据
                        {
                            CT_ContainerSprite[data_i+i2] = 0; //清空当前sprite
                        }
                        CT_TaskDestroy(i); //销毁sprite子程序
                        CT_Repaint = 1; //请求重绘
                    }
                }
            }
            for(i2=CT_ContainerSpriteLength-1;i2>=-1;i2--) //反向遍历sprite容器
            {
                if(i2 >= 0) //如果sprite索引大于等于0
                {
                    data_i = i2 * 25; //sprite数据索引
                    if(CT_ContainerSprite[data_i] != 0) //如果当前sprite不为空
                    {
                        CT_ContainerSpriteLength = i2 + 1; //确定sprite容器长度
                        break; //中断
                    }
                }
                else
                {
                    CT_ContainerSpriteLength = 0; //sprite容器长度为0
                }
            }
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 所有sprite位置x设置
//--------------------------------------------------------------------
void sprite_all_set_x(CTsprite *ctspr,int x)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        int id = ctspr->id; //sprite id
        if(ctspr->type == -2) //如果对象是sprite
        {
            int i;
            size_t data_i;
            for(i=0;i<CT_ContainerSpriteLength;i++) //遍历sprite容器
            {
                data_i = i * 25; //sprite数据索引
                if(CT_ContainerSprite[data_i] != 0) //如果当前sprite不为空
                {
                    int spr_id = CT_ContainerSprite[data_i+1]; //当前sprite id
                    if(spr_id == id) //如果当前sprite id等于sprite id
                    {
                        CT_ContainerSprite[data_i+3] = x; //设置当前sprite位置x
                        CT_Repaint = 1; //请求重绘
                    }
                }
            }
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 所有sprite位置y设置
//--------------------------------------------------------------------
void sprite_all_set_y(CTsprite *ctspr,int y)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        int id = ctspr->id; //sprite id
        if(ctspr->type == -2) //如果对象是sprite
        {
            int i;
            size_t data_i;
            for(i=0;i<CT_ContainerSpriteLength;i++) //遍历sprite容器
            {
                data_i = i * 25; //sprite数据索引
                if(CT_ContainerSprite[data_i] != 0) //如果当前sprite不为空
                {
                    int spr_id = CT_ContainerSprite[data_i+1]; //当前sprite id
                    if(spr_id == id) //如果当前sprite id等于sprite id
                    {
                        CT_ContainerSprite[data_i+4] = y; //设置当前sprite位置y
                        CT_Repaint = 1; //请求重绘
                    }
                }
            }
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 所有sprite层设置
//--------------------------------------------------------------------
void sprite_all_set_layer(CTsprite *ctspr,int l)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        int id = ctspr->id; //sprite id
        if(ctspr->type == -2) //如果对象是sprite
        {
            int i;
            size_t data_i;
            for(i=0;i<CT_ContainerSpriteLength;i++) //遍历sprite容器
            {
                data_i = i * 25; //sprite数据索引
                if(CT_ContainerSprite[data_i] != 0) //如果当前sprite不为空
                {
                    int spr_id = CT_ContainerSprite[data_i+1]; //当前sprite id
                    if(spr_id == id) //如果当前sprite id等于sprite id
                    {
                        CT_ContainerSprite[data_i+2] = l; //设置当前sprite层
                        CT_SpriteLayerStatusList[l] = 1; //将sprite层状态标记为1
                        CT_Repaint = 1; //请求重绘
                    }
                }
            }
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 所有sprite设置矩形碰撞盒
//--------------------------------------------------------------------
void sprite_all_set_rectbox(CTsprite *ctspr,int x,int y,int w,int h)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        int id = ctspr->id; //sprite id
        if(ctspr->type == -2) //如果对象是sprite
        {
            int i;
            size_t data_i;
            for(i=0;i<CT_ContainerSpriteLength;i++) //遍历sprite容器
            {
                data_i = i * 25; //sprite数据索引
                if(CT_ContainerSprite[data_i] != 0) //如果当前sprite不为空
                {
                    int spr_id = CT_ContainerSprite[data_i+1]; //当前sprite id
                    if(spr_id == id) //如果当前sprite id等于sprite id
                    {
                        CT_ContainerSprite[data_i+15] = x; //当前sprite碰撞盒位置x
                        CT_ContainerSprite[data_i+16] = y; //当前sprite碰撞盒位置y
                        CT_ContainerSprite[data_i+17] = w; //当前sprite碰撞盒宽度
                        CT_ContainerSprite[data_i+18] = h; //当前sprite碰撞盒高度
                    }
                }
            }
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 所有sprite设置圆形碰撞盒
//--------------------------------------------------------------------
void sprite_all_set_cirbox(CTsprite *ctspr,int cx,int cy,int r)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        int id = ctspr->id; //sprite id
        if(ctspr->type == -2) //如果对象是sprite
        {
            int i;
            size_t data_i;
            for(i=0;i<CT_ContainerSpriteLength;i++) //遍历sprite容器
            {
                data_i = i * 25; //sprite数据索引
                if(CT_ContainerSprite[data_i] != 0) //如果当前sprite不为空
                {
                    int spr_id = CT_ContainerSprite[data_i+1]; //当前sprite id
                    if(spr_id == id) //如果当前sprite id等于sprite id
                    {
                        CT_ContainerSprite[data_i+19] = cx; //当前sprite碰撞盒中心位置x
                        CT_ContainerSprite[data_i+20] = cy; //当前sprite碰撞盒中心位置y
                        CT_ContainerSprite[data_i+21] = r; //当前sprite碰撞盒半径
                    }
                }
            }
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 所有sprite子图像设置
//--------------------------------------------------------------------
void sprite_all_set_subimg(CTsprite *ctspr,int subi)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        int id = ctspr->id; //sprite id
        if(ctspr->type == -2) //如果对象是sprite
        {
            int i;
            size_t data_i;
            for(i=0;i<CT_ContainerSpriteLength;i++) //遍历sprite容器
            {
                data_i = i * 25; //sprite数据索引
                if(CT_ContainerSprite[data_i] != 0) //如果当前sprite不为空
                {
                    int spr_id = CT_ContainerSprite[data_i+1]; //当前sprite id
                    if(spr_id == id) //如果当前sprite id等于sprite id
                    {
                        int spr_subnum = CT_ContainerSprite[data_i+7]; //当前sprite子图像数量
                        if(subi <= spr_subnum - 1) //如果sprite子图像索引小于sprite子图像数量-1
                        {
                            CT_ContainerSprite[data_i+6] = subi; //设置sprite子图像索引
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
// 所有sprite颜色设置
//--------------------------------------------------------------------
void sprite_all_set_color(CTsprite *ctspr,int col)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        int id = ctspr->id; //sprite id
        if(ctspr->type == -2) //如果对象是sprite
        {
            int i;
            size_t data_i;
            for(i=0;i<CT_ContainerSpriteLength;i++) //遍历sprite容器
            {
                data_i = i * 25; //sprite数据索引
                if(CT_ContainerSprite[data_i] != 0) //如果当前sprite不为空
                {
                    int spr_id = CT_ContainerSprite[data_i+1]; //当前sprite id
                    if(spr_id == id) //如果当前sprite id等于sprite id
                    {
                        int alpha = CT_ContainerSprite[data_i+22] & 0x000000ff; //获取sprite透明度
                        CT_ContainerSprite[data_i+22] = col * 0x100 + alpha; //设置当前sprite颜色
                    }
                }
            }
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 所有sprite透明度设置
//--------------------------------------------------------------------
void sprite_all_set_colora(CTsprite *ctspr,int alp)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        int id = ctspr->id; //sprite id
        if(ctspr->type == -2) //如果对象是sprite
        {
            int i;
            size_t data_i;
            for(i=0;i<CT_ContainerSpriteLength;i++) //遍历sprite容器
            {
                data_i = i * 25; //sprite数据索引
                if(CT_ContainerSprite[data_i] != 0) //如果当前sprite不为空
                {
                    int spr_id = CT_ContainerSprite[data_i+1]; //当前sprite id
                    if(spr_id == id) //如果当前sprite id等于sprite id
                    {
                        int color = CT_ContainerSprite[data_i+22] & 0xffffff00; //获取sprite颜色
                        CT_ContainerSprite[data_i+22] = color + alp; //设置当前sprite颜色
                    }
                }
            }
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 所有sprite变量赋值 数字
//--------------------------------------------------------------------
void spritenum_all_set(CTsprite *ctspr,int vi,varn n)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        int id = ctspr->id; //sprite id
        if(ctspr->type == -2) //如果对象是sprite
        {
            int i;
            size_t data_i;
            for(i=0;i<CT_ContainerSpriteLength;i++) //遍历sprite容器
            {
               data_i = i * 25; //sprite数据索引
               if(CT_ContainerSprite[data_i] != 0) //如果当前sprite不为空
                {
                    int spr_id = CT_ContainerSprite[data_i+1]; //当前sprite id
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
// 所有sprite变量赋值 小数
//--------------------------------------------------------------------
void spritedec_all_set(CTsprite *ctspr,int vi,vard d)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        int id = ctspr->id; //sprite id
        if(ctspr->type == -2) //如果对象是sprite
        {
            int i;
            size_t data_i;
            for(i=0;i<CT_ContainerSpriteLength;i++) //遍历sprite容器
            {
               data_i = i * 25; //sprite数据索引
               if(CT_ContainerSprite[data_i] != 0) //如果当前sprite不为空
                {
                    int spr_id = CT_ContainerSprite[data_i+1]; //当前sprite id
                    if(spr_id == id) //如果当前sprite id等于sprite id
                    {
                        CT_TaskSetDec(i,vi,d); //sprite程序小数变量赋值
                    }
                }
            }
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 所有sprite变量赋值 字符串
//--------------------------------------------------------------------
void spritestr_all_set(CTsprite *ctspr,int vi,char *s)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        int id = ctspr->id; //sprite id
        if(ctspr->type == -2) //如果对象是sprite
        {
            int i;
            size_t data_i;
            for(i=0;i<CT_ContainerSpriteLength;i++) //遍历sprite容器
            {
                data_i = i * 25; //sprite数据索引
                if(CT_ContainerSprite[data_i] != 0) //如果当前sprite不为空
                {
                    int spr_id = CT_ContainerSprite[data_i+1]; //当前sprite id
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