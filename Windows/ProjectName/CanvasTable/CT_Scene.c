#define _CRT_SECURE_NO_WARNINGS

#include "CanvasTableSYS.h"
#include <stdio.h>

extern int **CT_ResourceArgList;
extern int CT_ViewX;
extern int CT_ViewY;

//瓦片容器格式
// [[占位符,层,瓷砖位置x,瓷砖位置y,瓷砖图像索引,图像区x,图像区y],...]

int CT_sceneWidth = 0; //场景宽度
int CT_sceneHeight = 0; //场景高度
int CT_GridWidth = 0; //场景网格宽度
int CT_GridHeight = 0; //场景网格高度
int CT_TileOffset = 0; //瓦片图像偏移
int CT_ContainerTile[100000][7] = {NULL}; //瓦片容器
int CT_ContainerTileLength = 0; //Tile容器长度

//--------------------------------------------------------------------
// 渲染瓦片
//--------------------------------------------------------------------
void CT_TileRender(int l)
{
    int vx = CT_ViewX; //视野位置x
    int vy = CT_ViewY; //视野位置y
    int i;
    for(i=0;i<CT_ContainerTileLength;i++) //遍历瓦片容器当前
    {
        if(CT_ContainerTile[i][0] != NULL) //如果当前瓦片不为空
        {
            int tl = CT_ContainerTile[i][1]; //当前瓦片层
            if(tl == l) //如果当前瓦片层等于渲染层
            {
                int tx = CT_ContainerTile[i][2]; //当前瓦片位置x
                int ty = CT_ContainerTile[i][3]; //当前瓦片位置y
                int ti = CT_ContainerTile[i][4]; //当前瓦片图像索引
                int tix = CT_ContainerTile[i][5]; //当前瓦片图像位置x
                int tiy = CT_ContainerTile[i][6]; //当前瓦片图像位置y
                CTCanvas_ImagePart(tx-vx,ty-vy,ti,tix,tiy,CT_GridWidth,CT_GridHeight); //绘制瓦片
            }
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 瓦片生成
//--------------------------------------------------------------------
void CT_TileBuild(int l,int x,int y,int res,int gx,int gy)
{
    int tix = CT_TileOffset + (CT_GridWidth * gx) + (gx * CT_TileOffset * 2); //瓦片图像位置x
    int tiy = CT_TileOffset + (CT_GridHeight * gy) + (gy * CT_TileOffset * 2); //瓦片图像位置y
    int tile_i = CT_ContainerTileLength; //tile容器索引
    if(res != -1) //如果有tile图像
    {
        int res_type = CT_ResourceArgList[res][1]; //获取资源类型
        if(res_type == 0) //如果资源类型是图像
        {
            //将Tile数据写入Tile容器
            CT_ContainerTile[tile_i][0] = 1;
            CT_ContainerTile[tile_i][1] = l;
            CT_ContainerTile[tile_i][2] = x;
            CT_ContainerTile[tile_i][3] = y;
            CT_ContainerTile[tile_i][4] = res;
            CT_ContainerTile[tile_i][5] = tix;
            CT_ContainerTile[tile_i][6] = tiy;
            CT_ContainerTileLength ++; //Tile容器索引 +1
        }
        else
        {
            cprint("ERROR : The resource index corresponding to the resource file is not a image.");
        }
    }
}
//--------------------------------------------------------------------