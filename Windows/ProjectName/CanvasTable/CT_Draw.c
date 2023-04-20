#include "CanvasTableSYS.h"

extern int CT_ViewX;
extern int CT_ViewY;

int *CT_ContainerDraw = NULL; //Draw容器
int CT_DrawLayerStatusList[32] = {0}; //Draw层状态列表
char *CT_ContainerDraw_String = NULL; //Draw容器(字符串)
char **CT_ContainerDraw_FontCharList_P = NULL; //Draw容器(*字体字符列表)
int **CT_ContainerDraw_FontGlyphList_P = NULL; //Draw容器(*字体字形列表)
int CT_ContainerDrawLength = 0; //Draw容器长度
int CT_ContainerMaxLength_Draw = 0; //Draw容器最大长度
int CT_DrawLayer = 0; //Draw层级
int CT_DrawAlpha = 255; //Draw透明度

//--------------------------------------------------------------------
// Draw容器格式
//--------------------------------------------------------------------
// [Draw数据,...]
// 矩形 : 占位符,1,draw层,矩形位置x,矩形位置y,矩形宽度,矩形高度,矩形颜色,矩形填充
// 线 : 占位符,2,draw层,线起点位置x,线起点位置y,线终点位置x,线终点位置y,线宽度,线颜色
// 圆 : 占位符,3,draw层,圆位置x,圆位置y,圆半径,圆颜色,圆填充
// 字符串 : 占位符,4,draw层,字符串位置x,字符串位置y,字体大小,字体颜色
// 图像 : 占位符,5,draw层,图像位置x,图像位置y,图像资源索引,透明度
// 图像部分 : 占位符,6,draw层,图像位置x,图像位置y,图像,图像内位置x,图像内位置y,图像内宽度,图像内高度,透明度
// 三角形 : 占位符,8,draw层,第一点位置x,第一点位置y,第二点位置x,第二点位置y,第三点位置x,第三点位置y,颜色,填充
// 图像字符串 : 占位符,9,draw层,字符串位置x,字符串位置y,字符串颜色,字体图像索引,字体字符数量,字体字符间隙,字体字符宽度
//   字体字符列表:[列表指针,...]
//   字体字形列表:[列表指针,...]
//--------------------------------------------------------------------

//--------------------------------------------------------------------
// Draw渲染
//--------------------------------------------------------------------
void CT_DrawRender(int l)
{
    int vx = CT_ViewX; //视野位置x
    int vy = CT_ViewY; //视野位置y
    int i;
    size_t data_i;
    for(i=0;i<CT_ContainerDrawLength;i++) //遍历draw容器
    {
        data_i = i * 15; //draw数据索引
        if(CT_ContainerDraw[data_i] != 0) //如果当前draw不为空
        {
            int draw_l = CT_ContainerDraw[data_i+2]; //当前draw层
            if(draw_l == l) //如果当前draw层等于渲染层
            {
                int draw_type = CT_ContainerDraw[data_i+1]; //当前draw类型
                if(draw_type == 1) //如果draw类型是矩形
                {
                    int x = CT_ContainerDraw[data_i+3]; //矩形位置x
                    int y = CT_ContainerDraw[data_i+4]; //矩形位置y
                    int w = CT_ContainerDraw[data_i+5]; //矩形宽度
                    int h = CT_ContainerDraw[data_i+6]; //矩形高度
                    int c = CT_ContainerDraw[data_i+7]; //矩形颜色
                    int f = CT_ContainerDraw[data_i+8]; //矩形填充
                    CT_OffCanvas_Rectangle(x-vx,y-vy,w,h,c,f); //绘制矩形
                }
                if(draw_type == 2) //如果draw类型是线
                {
                    int x1 = CT_ContainerDraw[data_i+3]; //线起点位置x
                    int y1 = CT_ContainerDraw[data_i+4]; //线起点位置y
                    int x2 = CT_ContainerDraw[data_i+5]; //线终点位置x
                    int y2 = CT_ContainerDraw[data_i+6]; //线终点位置y
                    int w = CT_ContainerDraw[data_i+7]; //线宽度
                    int c = CT_ContainerDraw[data_i+8]; //线颜色
                    CT_OffCanvas_Line(x1-vx,y1-vy,x2-vx,y2-vy,w,c); //绘制线
                }
                if(draw_type == 3) //如果draw类型是圆
                {
                    int x = CT_ContainerDraw[data_i+3]; //圆位置x
                    int y = CT_ContainerDraw[data_i+4]; //圆位置y
                    int r = CT_ContainerDraw[data_i+5]; //圆半径
                    int c = CT_ContainerDraw[data_i+6]; //圆颜色
                    int f = CT_ContainerDraw[data_i+7]; //圆填充
                    CT_OffCanvasOpaque_Circle(x-vx,y-vy,r,c,f); //绘制圆
                }
                if(draw_type == 4) //如果draw类型是字符串
                {
                    int x = CT_ContainerDraw[data_i+3];
                    int y = CT_ContainerDraw[data_i+4];
                    int s = CT_ContainerDraw[data_i+5];
                    int c = CT_ContainerDraw[data_i+6];
                    char *str_p = CT_ContainerDraw_String + i * 513; //字符串指针
                    CT_OffCanvas_String(x-vx,y-vy,s,c,str_p);
                }
                if(draw_type == 5) //如果draw类型是图像
                {
                    int x = CT_ContainerDraw[data_i+3]; //图像位置x
                    int y = CT_ContainerDraw[data_i+4]; //图像位置y
                    int img = CT_ContainerDraw[data_i+5]; //图像资源索引
                    int a = CT_ContainerDraw[data_i+6]; //图像透明度
                    CT_OffCanvas_Image(x-vx,y-vy,img,a); //绘制图像
                }
                if(draw_type == 6) //如果draw类型是图像部分
                {
                    int x = CT_ContainerDraw[data_i+3];  //图像位置x
                    int y = CT_ContainerDraw[data_i+4]; //图像位置y
                    int img = CT_ContainerDraw[data_i+5]; //图像资源索引
                    int a = CT_ContainerDraw[data_i+10]; //图像透明度
                    unsigned int ix = CT_ContainerDraw[data_i+6]; //图像内位置x
                    unsigned int iy = CT_ContainerDraw[data_i+7]; //图像内位置y
                    unsigned int iw = CT_ContainerDraw[data_i+8]; //图像内宽度
                    unsigned int ih = CT_ContainerDraw[data_i+9]; //图像内高度
                    CT_OffCanvas_ImagePart(x-vx,y-vy,img,a,ix,iy,iw,ih); //绘制图像部分
                }
                if(draw_type == 7) //如果draw类型是曲线
                {
                    int x1 = CT_ContainerDraw[data_i+3]; //曲线起点位置x
                    int y1 = CT_ContainerDraw[data_i+4]; //曲线起点位置y
                    int x2 = CT_ContainerDraw[data_i+5]; //曲线控制点位置x
                    int y2 = CT_ContainerDraw[data_i+6]; //曲线控制点位置y
                    int x3 = CT_ContainerDraw[data_i+7]; //曲线终点位置x
                    int y3 = CT_ContainerDraw[data_i+8]; //曲线终点位置y
                    int w = CT_ContainerDraw[data_i+9]; //曲线宽度
                    int c = CT_ContainerDraw[data_i+10]; //曲线颜色
                    CT_OffCanvas_Curve(x1,y1,x2,y2,x3,y3,w,c); //绘制曲线
                }
                if(draw_type == 8) //如果draw类型是三角形
                {
                    int x1 = CT_ContainerDraw[data_i+3]; //三角形第一点位置x
                    int y1 = CT_ContainerDraw[data_i+4]; //三角形第一点位置y
                    int x2 = CT_ContainerDraw[data_i+5]; //三角形第二点位置x
                    int y2 = CT_ContainerDraw[data_i+6]; //三角形第二点位置y
                    int x3 = CT_ContainerDraw[data_i+7]; //三角形第三点位置x
                    int y3 = CT_ContainerDraw[data_i+8]; //三角形第三点位置y
                    int c = CT_ContainerDraw[data_i+9]; //三角形颜色
                    int f = CT_ContainerDraw[data_i+10]; //三角形填充
                    CT_OffCanvasOpaque_Triangle(x1-vx,y1-vy,x2,y2,x3,y3,c,f); //绘制三角形
                }
                if(draw_type == 9) //如果draw类型是图像字符串
                {
                    int x = CT_ContainerDraw[data_i+3]; //字符串位置x
                    int y = CT_ContainerDraw[data_i+4]; //字符串位置y
                    int c = CT_ContainerDraw[data_i+5]; //字符串颜色
                    int font_i = CT_ContainerDraw[data_i+6]; //字体图像索引
                    int font_n = CT_ContainerDraw[data_i+7]; //字体字符数量
                    int font_g = CT_ContainerDraw[data_i+8]; //字体字符间隙
                    int font_h = CT_ContainerDraw[data_i+9]; //字体字符宽度
                    char *str_p = CT_ContainerDraw_String + i * 513; //字符串指针
                    char *font_cl = CT_ContainerDraw_FontCharList_P[i]; //字体字符列表
                    int *font_gl = CT_ContainerDraw_FontGlyphList_P[i]; //字体字形列表
                    CT_OffCanvas_ImgString(x-vx,y-vy,c,str_p,font_i,font_n,font_g,font_cl,font_gl); //绘制字符串
                }
            }
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// Draw容器预先分配
//--------------------------------------------------------------------
void CT_ContainerPreallocated_Draw(int max_l)
{
    size_t cd_cap = sizeof(int) * 15 * max_l; //draw容器容量
    size_t cds_cap = sizeof(char) * 513 * max_l; //draw字符串容器容量
    size_t cdfcl_cap = sizeof(char*) * max_l; //draw字体字符列表容器容量
    size_t cdfgl_cap = sizeof(int*) * max_l; //draw字体字形列表容器容量
    if(CT_ContainerMaxLength_Draw == 0) //如果draw容器最大长度为0
    {
        CT_ContainerDraw = malloc(cd_cap); //draw容器分配内存
        memset(CT_ContainerDraw,0x00,cd_cap); //draw容器填0
        CT_ContainerDraw_String = malloc(cds_cap); //draw字符串容器分配指针的内存
        memset(CT_ContainerDraw_String,0x00,cds_cap); //draw字符串容器填0
        CT_ContainerDraw_FontCharList_P = malloc(cdfcl_cap); //draw字体字符列表容器分配内存
        memset(CT_ContainerDraw_FontCharList_P,0x00,cdfcl_cap); //draw字体字符列表容器填0
        CT_ContainerDraw_FontGlyphList_P = malloc(cdfgl_cap); //draw字体字形列表容器分配内存
        memset(CT_ContainerDraw_FontGlyphList_P,0x00,cdfgl_cap); //draw字体字形列表容器填0
        CT_ContainerMaxLength_Draw = max_l; //设置draw容器最大长度
    }
    if(CT_ContainerMaxLength_Draw > 0 && max_l > CT_ContainerMaxLength_Draw) //如果draw容器长度大于draw容器最大长度
    {
        size_t cd_nbs = cd_cap - sizeof(int) * 15 * CT_ContainerMaxLength_Draw; //draw容器新内存块大小
        size_t cds_nbs = cds_cap - sizeof(char) * 513 * CT_ContainerMaxLength_Draw; //draw字符串容器新内存块大小
        size_t cdfcl_nbs = cdfcl_cap - sizeof(char*) * CT_ContainerMaxLength_Draw; //draw字体字符列表容器新内存块大小
        size_t cdfgl_nbs = cdfgl_cap - sizeof(int*) * CT_ContainerMaxLength_Draw; //draw字体字形列表容器新内存块大小
        CT_ContainerDraw = realloc(CT_ContainerDraw,cd_cap); //重新分配draw容器的内存
        memset(CT_ContainerDraw + CT_ContainerMaxLength_Draw * 15,0x00,cd_nbs); //对draw容器的新空间填0
        CT_ContainerDraw_String = realloc(CT_ContainerDraw_String,cds_cap); //重新分配draw字符串容器的内存
        memset(CT_ContainerDraw_String + CT_ContainerMaxLength_Draw * sizeof(char) * 513,0x00,cds_nbs); //对draw字符串容器的新空间填0
        CT_ContainerDraw_FontCharList_P = realloc(CT_ContainerDraw_FontCharList_P,cdfcl_cap); //重新分配draw字体字符列表(指针)容器的内存
        memset(CT_ContainerDraw_FontCharList_P + CT_ContainerMaxLength_Draw,0x00,cdfcl_nbs); //对draw字体字符列表(指针)容器的新空间填0
        CT_ContainerDraw_FontGlyphList_P = realloc(CT_ContainerDraw_FontGlyphList_P,cdfgl_cap); //重新分配draw字体字形列表(指针)容器的内存
        memset(CT_ContainerDraw_FontGlyphList_P + CT_ContainerMaxLength_Draw,0x00,cdfgl_nbs); //对draw字体字形列表(指针)容器的新空间填0
        CT_ContainerMaxLength_Draw = max_l; //设置draw容器最大长度
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// Draw容器清除
//--------------------------------------------------------------------
void CT_ContainerDrawClear()
{
    int i;
    int i2;
    size_t data_i;
    for(i=0;i<CT_ContainerDrawLength;i++) //遍历draw容器
    {
        data_i = i * 15; //draw数据索引
        if(CT_ContainerDraw[data_i] != 0) //如果当前draw不为空
        {
            int draw_type = CT_ContainerDraw[data_i+1]; //draw类型
            for(i2=0;i2<15;i2++) //遍历draw数据
            {
                CT_ContainerDraw[data_i+i2] = 0; //清除draw数据
            }
            if(draw_type == 4 || draw_type == 9) //如果draw类型是字符串
            {
                char *draw_p = CT_ContainerDraw_String + i * 513; //draw容器指针
                strclear(draw_p); //清除draw数据
                CT_ContainerDraw_FontCharList_P[i] = NULL; //清除字体字符列表
                CT_ContainerDraw_FontGlyphList_P[i] = NULL; //清除字体字形列表
            }
        }
    }
    for(i=0;i<32;i++) //遍历层状态列表
    {
        CT_DrawLayerStatusList[i] = 0; //将当前draw层状态标记为0
    }
    CT_ContainerDrawLength = 0; //Draw容器长度为0
    CT_DrawLayer = 0; //Draw容器层
}
//--------------------------------------------------------------------