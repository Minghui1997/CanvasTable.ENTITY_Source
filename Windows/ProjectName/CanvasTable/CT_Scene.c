#define _CRT_SECURE_NO_WARNINGS

#include "CanvasTableSYS.h"

extern unsigned int *CT_ResourceArgList;
extern int CT_ViewX;
extern int CT_ViewY;
extern int CT_CanvasW;
extern int CT_CanvasH;

//瓦片容器格式
// [占位符,层,瓷砖位置x,瓷砖位置y,瓷砖图像索引,图像区x,图像区y,...]

int CT_sceneWidth = 0; //场景宽度
int CT_sceneHeight = 0; //场景高度
int CT_GridWidth = 30; //场景网格宽度
int CT_GridHeight = 30; //场景网格高度
int *CT_ContainerTile = NULL; //瓦片容器
int CT_TileLayerStatusList[32] = {0}; //Tile层状态列表
int CT_ContainerTileLength = 0; //Tile容器长度
int CT_ContainerMaxLength_Tile = 0; //Tile容器最大长度

//--------------------------------------------------------------------
// 渲染瓦片
//--------------------------------------------------------------------
void CT_TileRender(int l)
{
    int vx = CT_ViewX; //视野位置x
    int vy = CT_ViewY; //视野位置y
    int i;
    size_t data_i;
    for(i=0;i<CT_ContainerTileLength;i++) //遍历瓦片容器当前
    {
        data_i = i * 7; //瓦片数据索引
        if(CT_ContainerTile[data_i] != 0) //如果当前瓦片不为空
        {
            int tl = CT_ContainerTile[data_i+1]; //当前瓦片层
            if(tl == l) //如果当前瓦片层等于渲染层
            {
                int tx = CT_ContainerTile[data_i+2]; //当前瓦片位置x
                int ty = CT_ContainerTile[data_i+3]; //当前瓦片位置y
                int ti = CT_ContainerTile[data_i+4]; //当前瓦片图像索引
                int tix = CT_ContainerTile[data_i+5]; //当前瓦片图像位置x
                int tiy = CT_ContainerTile[data_i+6]; //当前瓦片图像位置y
                if(tx + CT_GridWidth > vx && tx < vx + CT_CanvasW && ty + CT_GridHeight > vy && ty < vy + CT_CanvasH) //如果当前瓦片在视野内
                {
                    CT_OffCanvas_ImagePart(tx-vx,ty-vy,ti,255,tix,tiy,CT_GridWidth,CT_GridHeight); //绘制瓦片
                }
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
    unsigned int tix = CT_GridWidth * gx; //瓦片图像位置x
    unsigned int tiy = CT_GridHeight * gy; //瓦片图像位置y
    size_t tile_i = CT_ContainerTileLength * 7; //tile数据索引
    if(l >= 0 && l <= 31) //如果tile层在正确范围内
    {
        if(res != -1) //如果有tile图像
        {
            int res_type = CT_ResourceArgList[res*5+1]; //获取资源类型
            if(res_type == 0) //如果资源类型是图像
            {
                //将Tile数据写入Tile容器
                CT_ContainerTile[tile_i] = 1;
                CT_ContainerTile[tile_i+1] = l;
                CT_ContainerTile[tile_i+2] = x;
                CT_ContainerTile[tile_i+3] = y;
                CT_ContainerTile[tile_i+4] = res;
                CT_ContainerTile[tile_i+5] = tix;
                CT_ContainerTile[tile_i+6] = tiy;
                CT_ContainerTileLength ++; //Tile容器长度 +1
                if(CT_ContainerTileLength == CT_ContainerMaxLength_Tile) //如果Tile容器长度等于Tile容器最大长度
                {
                    CT_ContainerPreallocated_Tile(CT_ContainerTileLength+300); //重新分配Tile容器
                }
                CT_TileLayerStatusList[l] = 1; //将当前tile层的状态标记为1
            }
            else
            {
                cprint("ERROR : The resource index corresponding to the resource file is not a image.");
            }
        }
    }
    else
    {
        cprint("ERROR : Tile layer is error.");
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 瓦片容器预先分配
//--------------------------------------------------------------------
void CT_ContainerPreallocated_Tile(int max_l)
{
    size_t ct_cap = sizeof(int) * 7 * max_l; //瓦片容器容量
    if(CT_ContainerMaxLength_Tile == 0) //如果瓦片容器最大长度为0
    {
        CT_ContainerTile = malloc(ct_cap); //为瓦片容器分配内存
        memset(CT_ContainerTile,0x00,ct_cap); //tile容器填0
    }
    if(CT_ContainerMaxLength_Tile > 0 && max_l > CT_ContainerMaxLength_Tile) //如果瓦片容器长度大于瓦片容器最大长度
    {
        size_t ct_nbs = ct_cap - sizeof(int) * 7 * CT_ContainerMaxLength_Tile; //瓦片容器新内存块大小
        CT_ContainerTile = realloc(CT_ContainerTile,ct_cap); //瓦片容器重新分配内存
        memset(CT_ContainerTile+CT_ContainerMaxLength_Tile*7,0x00,ct_nbs); //对瓦片容器的新空间填0
    }
    CT_ContainerMaxLength_Tile = max_l; //设置瓦片容器最大长度
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 瓦片容器清除
//--------------------------------------------------------------------
void CT_ContainerTileClear()
{
    int i;
    size_t data_i;
    int i2;
    for(i=0;i<CT_ContainerTileLength;i++) //遍历瓦片容器
    {
        data_i = i * 7; //瓦片数据索引
        if(CT_ContainerTile[data_i] != 0) //如果当前瓦片不为空
        {
            for(i2=0;i2<7;i2++) //遍历瓦片数据
            {
                CT_ContainerTile[data_i+i2] = 0; //清除瓦片数据
            }
        }
    }
    for(i2=0;i2<32;i2++) //遍历层状态列表
    {
        CT_TileLayerStatusList[i2] = 0; //将当前tile层状态标记为0
    }
    CT_ContainerTileLength = 0; //Tile容器长度为0
}
//--------------------------------------------------------------------