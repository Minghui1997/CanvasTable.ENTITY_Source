#define _CRT_SECURE_NO_WARNINGS

#include "CanvasTableSYS.h"
#include <stdio.h>
#include <string.h>

extern int CT_RunnerStatus;
extern int CT_sceneWidth;
extern int CT_sceneHeight;
extern int CT_GridWidth;
extern int CT_GridHeight;
extern int CT_TileOffset;
extern int CT_ContainerTile[100000][7];
extern int CT_ContainerTileLength;
extern int **CT_ResourceArgList;
extern int CT_ContainerSprite[10000][25];
extern int CT_ContainerSpriteLength;
extern int CT_Repaint;

//--------------------------------------------------------------------
// 新建场景对象
//--------------------------------------------------------------------
void CTscene_new(CTscene *scene)
{
    scene->width = 960; //场景宽度
    scene->height = 540; //场景高度
    scene->grid_width = 30; //场景网格宽度
    scene->grid_height = 30; //场景网格高度
    scene->tile_image[0] = NULL; //瓦片图像组
    scene->type = -1; //类型
    scene->tile_map[0] = NULL; //瓦片地图层0
    scene->tile_offset = 0; //瓦片图像偏移
    scene->sprite_map = sprite_map_null; //sprite地图
}

void sprite_map_null()
{
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 新建场景对象
//--------------------------------------------------------------------
void scene_enter(CTscene *scene)
{
    CT_RunnerStatus = 0; //停止运行器
    CT_RunnerReset(); //运行器重置
    if(scene->type == -1) //如果对象是场景对象
    {
        int *ti = scene->tile_image; //传递瓦片图像列表;
        int tile_num = 0; //瓦片网格数
        int i;
        CT_sceneWidth = scene->width; //场景宽度
        CT_sceneHeight = scene->height; //场景高度
        CT_GridWidth = scene->grid_width; //场景网格宽度
        CT_GridHeight = scene->grid_height; //场景网格高度
        CT_TileOffset = scene->tile_offset; //瓦片图像偏移
        tile_num = (CT_sceneWidth / CT_GridWidth) * (CT_sceneHeight / CT_GridHeight); //计算瓦片网格数
        if(CT_sceneWidth > 0 && CT_sceneHeight > 0 && CT_GridWidth > 0 && CT_GridHeight) //如果场景大小和网格大小大于0
        {
            if(CT_sceneWidth >= CT_GridWidth && CT_sceneHeight >= CT_GridHeight) //如果场景大小大于等于网格大小
            {
                for(i=0;i<32;i++) //遍历瓦片地图
                {
                    int *tm = scene->tile_map[i]; //传递瓦片地图数据
                    if(tm != NULL) //如果瓦片地图数据不为空
                    {
                        int tile_x = 0; //瓦片位置x
                        int tile_y = 0; //瓦片位置y
                        int i2;
                        for(i2=0;i2<tile_num;i2++) //遍历瓦片地图当前层
                        {
                            if(tile_y < CT_sceneHeight) //如果当前瓦片位置y小于场景高度
                            {
                                if(tile_x < CT_sceneWidth) //如果当前瓦片位置x小于场景宽度
                                {
                                    if(tm[i2] != 0) //如果当前瓦片地图块不为空
                                    {
                                        int ti_i = 0; //瓦片图像索引
                                        int ti_gx = -1; //瓦片图块位置x
                                        int ti_gy = 0; //瓦片图块位置y
                                        int ti_width = CT_ResourceArgList[ti[ti_i]][2]; //当前瓦片图像宽度
                                        int ti_height = CT_ResourceArgList[ti[ti_i]][3]; //当前瓦片图像高度
                                        int ti_gxn = ti_width / CT_GridWidth; //当前瓦片图像横向网格数
                                        int ti_gyn = ti_height / CT_GridHeight; //当前瓦片图像纵向网格数
                                        int ti_index = tm[i2]; //瓦片地图块索引
                                        int i3;
                                        for(i3=0;i3<ti_index;i3++) //遍历瓦片图块
                                        {
                                            if(ti_gy < ti_gyn) //如果当前瓦片图块位置y小于瓦片图像高度
                                            {
                                                if(ti_gx < ti_gxn) //如果瓦片图块位置x小于瓦片图像宽度
                                                {
                                                    ti_gx += 1; //下一个瓦片图块
                                                }
                                                if(ti_gx >= ti_gxn) //如果下一个瓦片图块位置x大于等于瓦片图像宽度
                                                {
                                                    ti_gx = 0; //瓦片图块位置x
                                                    ti_gy += 1; //下一行瓦片图块
                                                }
                                            }
                                            if(ti_gy >= ti_gyn) //如果下一个瓦片图块位置y大于瓦片图像高度
                                            {
                                                ti_gx = 0; //瓦片图块位置x
                                                ti_gy = 0; //瓦片图块位置y
                                                ti_i ++; //切换下一个瓦片图像
                                                ti_width = CT_ResourceArgList[ti[ti_i]][2]; //当前瓦片图片宽度
                                                ti_height = CT_ResourceArgList[ti[ti_i]][3]; //当前瓦片图片高度
                                                ti_gxn = ti_width / CT_GridWidth; //当前瓦片图片横向网格数
                                                ti_gyn = ti_height / CT_GridHeight; //当前瓦片图片纵向网格数
                                                if(ti_i > 255) //如果瓦片图块索引没有对应的瓦片图块
                                                {
                                                    cprint("ERROR : Beyond range of tile image group.");
                                                }
                                            }
                                        }
                                        CT_TileBuild(i,tile_x,tile_y,ti[ti_i],ti_gx,ti_gy); //创建瓦片
                                    }
                                    tile_x += CT_GridWidth; //下一个瓦片位置x
                                }
                                if(tile_x >= CT_sceneWidth) //如果下一个瓦片位置x大于等于场景宽度
                                {
                                    tile_x = 0;
                                    tile_y += CT_GridHeight; //下一行瓦片位置y
                                }
                            }
                            if(tile_y >= CT_sceneHeight) //如果下一行瓦片位置y大于场景高度
                            {
                                tile_x = 0;
                                tile_y = 0;
                            }
                        }
                    }
                }
                scene->sprite_map(); //创建sprite地图
            }
            else
            {
                cprint("ERROR : The size of the scene must be greater than the grid size.");
            }
        }
        else
        {
            cprint("ERROR : Unable to enter the scene.");
        }
    }
    else
    {
        cprint("ERROR : The specified scene is not a scene object.");     
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// sprite地图
//--------------------------------------------------------------------
void m(int x,int y,CTsprite *ctspr)
{
    if(ctspr->type == -2) //如果对象类型是sprite
    {
        int id = ctspr->id; //sprite id
        int image = ctspr->image; //sprite图像
        int image_w = 0; //图像宽度
        int image_h = 0; //图像高度
        int subimg_w = 0; //sprite子图像宽度
        int subimg_h = 0; //sprite子图像高度
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
            int conspr_i = CT_ContainerSpriteLength; //sprite容器sprite层的当前索引
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
        }
        else
        {
            cprint("ERROR : The sprite property have error.");
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 创建瓦片
//--------------------------------------------------------------------
void tile_create(int l,int x,int y,int res,int gx,int gy)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        int tix = CT_TileOffset + (CT_GridWidth * gx) + (gx * CT_TileOffset * 2); //瓦片图像位置x
        int tiy = CT_TileOffset + (CT_GridHeight * gy) + (gy * CT_TileOffset * 2); //瓦片图像位置y
        int tile_i = CT_ContainerTileLength; //tile容器索引
        if(res != -1) //如果有tile图像
        {
            int res_type = CT_ResourceArgList[res][1]; //获取资源类型
            if(res_type == 0) //如果资源类型是图像
            {
                int i;
                for(i=0;i<CT_ContainerTileLength;i++) //遍历Tile容器
                {
                    if(CT_ContainerTile[i][0] == NULL) //如果当前Tile为空
                    {
                        //将Tile数据写入Tile容器
                        CT_ContainerTile[i][0] = 1;
                        CT_ContainerTile[i][1] = l;
                        CT_ContainerTile[i][2] = x;
                        CT_ContainerTile[i][3] = y;
                        CT_ContainerTile[i][4] = res;
                        CT_ContainerTile[i][5] = tix;
                        CT_ContainerTile[i][6] = tiy;
                        CT_Repaint = 1; //请求重绘
                        return; //中断
                    }
                }
                //将Tile数据写入Tile容器
                CT_ContainerTile[tile_i][0] = 1;
                CT_ContainerTile[tile_i][1] = l;
                CT_ContainerTile[tile_i][2] = x;
                CT_ContainerTile[tile_i][3] = y;
                CT_ContainerTile[tile_i][4] = res;
                CT_ContainerTile[tile_i][5] = tix;
                CT_ContainerTile[tile_i][6] = tiy;
                CT_ContainerTileLength ++; //Tile容器索引 +1
                CT_Repaint = 1; //请求重绘
            }
            else
            {
                cprint("ERROR 4 : The resource index corresponding to the resource file is not a image.");
            }
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 销毁瓦片
//--------------------------------------------------------------------
void tile_destroy(int l,int x,int y)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        int i;
        for(i=0;i<CT_ContainerTileLength;i++) //遍历瓦片容器
        {
            if(CT_ContainerTile[i][0] != NULL) //如果当前瓦片不为空
            {
                int tl = CT_ContainerTile[i][1]; //当前瓦片层
                if(tl == l) //如果当前瓦片层等于层
                {
                    int tx = CT_ContainerTile[i][2]; //当前瓦片位置x
                    int ty = CT_ContainerTile[i][3]; //当前瓦片位置y
                    if(x == tx && y == ty) //如果当前瓦片位置与参数位置相同
                    {
                        //清除当前瓦片数据
                        CT_ContainerTile[i][0] = NULL; 
                        CT_ContainerTile[i][1] = NULL;
                        CT_ContainerTile[i][2] = NULL;
                        CT_ContainerTile[i][3] = NULL;
                        CT_ContainerTile[i][4] = NULL;
                        CT_ContainerTile[i][5] = NULL;
                        CT_ContainerTile[i][6] = NULL;
                        CT_Repaint = 1; //请求重绘
                        break; //中断
                    }
                }
            }
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 检查瓦片
//--------------------------------------------------------------------
int tile_check(int l,int x,int y)
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        int i;
        for(i=0;i<CT_ContainerTileLength;i++) //遍历瓦片容器
        {
            if(CT_ContainerTile[i][0] != NULL) //如果当前瓦片不为空
            {
                int tl = CT_ContainerTile[i][1]; //当前瓦片层
                if(tl == l) //如果当前瓦片层等于层
                {
                    int tx = CT_ContainerTile[i][2]; //当前瓦片位置x
                    int ty = CT_ContainerTile[i][3]; //当前瓦片位置y
                    int gw = CT_GridWidth; //场景网格宽度
                    int gh = CT_GridHeight; //场景网格高度
                    if(x >= tx && y >= ty && x < tx + gw && y < ty + gh) //如果当前位置有瓦片
                    {
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 获取场景宽度
//--------------------------------------------------------------------
int scene_width()
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        return CT_sceneWidth; //返回场景宽度
    }
    return 0;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 获取场景高度
//--------------------------------------------------------------------
int scene_height()
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        return CT_sceneHeight; //返回场景高度
    }
    return 0;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 获取场景网格宽度
//--------------------------------------------------------------------
int grid_width()
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        return CT_GridWidth; //返回场景网格宽度
    }
    return 0;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 获取场景网格高度
//--------------------------------------------------------------------
int grid_height()
{
    if(CT_RunnerStatus == 1) //如果运行器状态为1
    {
        return CT_GridHeight; //返回场景网格高度
    }
    return 0;
}
//--------------------------------------------------------------------