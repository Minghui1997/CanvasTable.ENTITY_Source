#include "CanvasTableSYS.h"

int CT_ContainerDraw[10000][15] = {NULL}; //Draw容器
char CT_ContainerDraw_String[10000][512] = {NULL}; //Draw容器(str)
char *CT_ContainerDraw_FontCharList_P[10000] = {NULL}; //Draw容器(*字体字符列表)
int *CT_ContainerDraw_FontGlythList_P[10000] = {NULL}; //Draw容器(*字体字形列表)
int CT_ContainerDrawIndex = 0; //Draw容器索引
int CT_DrawLayer = 0; //Draw层级

//--------------------------------------------------------------------
// Draw容器格式
//--------------------------------------------------------------------
// [Draw数据,...]
// 矩形:[占位符,1,draw层,矩形位置x,矩形位置y,矩形宽度,矩形高度,矩形颜色,矩形填充]
// 线:[占位符,2,draw层,线起点位置x,线起点位置y,线终点位置x,线终点位置y,线宽度,线颜色]
// 圆:[占位符,3,draw层,圆位置x,圆位置y,圆半径,圆颜色,圆填充]
// 图像:[占位符,5,draw层,图像位置x,图像位置y,图像资源索引]
// 图像部分:[占位符,6,draw层,图像位置x,图像位置y,图像,图像内位置x,图像内位置y,图像内宽度,图像内高度]
// 三角形:[占位符,8,draw层,第一点位置x,第一点位置y,第二点位置x,第二点位置y,第三点位置x,第三点位置y,颜色,实心]
// 字符串:[占位符,9,draw层,字符串位置x,字符串位置y,字符串颜色,字体图像索引,字体字符数量,字体字符间隙,字体字符宽度]
//   字体字符列表:[列表指针,...]
//   字体字形列表:[列表指针,...]
//--------------------------------------------------------------------

//--------------------------------------------------------------------
// Draw渲染
//--------------------------------------------------------------------
void CT_DrawRender(int l)
{
    int _x = view_x(); //视野位置x
    int _y = view_y(); //视野位置y
    int i;
    for(i=0;i<CT_ContainerDrawIndex;i++) //遍历draw容器
    {
        if(CT_ContainerDraw[i][0] != NULL) //如果当前draw不为空
        {
            int draw_l = CT_ContainerDraw[i][2]; //当前draw层
            if(draw_l == l) //如果当前draw层等于渲染层
            {
                int draw_type = CT_ContainerDraw[i][1]; //当前draw类型
                if(draw_type == 1) //如果draw类型是矩形
                {
                    int x = CT_ContainerDraw[i][3]; //矩形位置x
                    int y = CT_ContainerDraw[i][4]; //矩形位置y
                    int w = CT_ContainerDraw[i][5]; //矩形宽度
                    int h = CT_ContainerDraw[i][6]; //矩形高度
                    int c = CT_ContainerDraw[i][7]; //矩形颜色
                    int f = CT_ContainerDraw[i][8]; //矩形填充
                    CTCanvas_Rect(x-_x,y-_y,w,h,c,f); //绘制矩形
                }
                if(draw_type == 2) //如果draw类型是线
                {
                    int x1 = CT_ContainerDraw[i][3]; //线起点位置x
                    int y1 = CT_ContainerDraw[i][4]; //线起点位置y
                    int x2 = CT_ContainerDraw[i][5]; //线终点位置x
                    int y2 = CT_ContainerDraw[i][6]; //线终点位置y
                    int w = CT_ContainerDraw[i][7]; //线宽度
                    int c = CT_ContainerDraw[i][8]; //线颜色
                    CTCanvas_Line(x1-_x,y1-_y,x2-_x,y2-_y,w,c); //绘制线
                }
                if(draw_type == 3) //如果draw类型是圆
                {
                    int x = CT_ContainerDraw[i][3]; //圆位置x
                    int y = CT_ContainerDraw[i][4]; //圆位置y
                    int r = CT_ContainerDraw[i][5]; //圆半径
                    int c = CT_ContainerDraw[i][6]; //圆颜色
                    int f = CT_ContainerDraw[i][7]; //圆填充
                    CTCanvas_Circle(x-_x,y-_y,r,c,f); //绘制圆
                }
                if(draw_type == 5) //如果draw类型是图像
                {
                    int x = CT_ContainerDraw[i][3]; //图像位置x
                    int y = CT_ContainerDraw[i][4]; //图像位置y
                    int img = CT_ContainerDraw[i][5]; //图像资源索引
                    CTCanvas_Image(x-_x,y-_y,img); //绘制图像
                }
                if(draw_type == 6) //如果draw类型是图像部分
                {
                    int x = CT_ContainerDraw[i][3];  //图像位置x
                    int y = CT_ContainerDraw[i][4]; //图像位置y
                    int img = CT_ContainerDraw[i][5]; //图像
                    int ix = CT_ContainerDraw[i][6]; //图像内位置x
                    int iy = CT_ContainerDraw[i][7]; //图像内位置y
                    int iw = CT_ContainerDraw[i][8]; //图像内宽度
                    int ih = CT_ContainerDraw[i][9]; //图像内高度
                    CTCanvas_ImagePart(x-_x,y-_y,img,ix,iy,iw,ih); //绘制图像部分
                }
                if(draw_type == 7) //如果draw类型是曲线
                {
                    int x1 = CT_ContainerDraw[i][3]; //曲线起点位置x
                    int y1 = CT_ContainerDraw[i][4]; //曲线起点位置y
                    int x2 = CT_ContainerDraw[i][5]; //曲线控制点位置x
                    int y2 = CT_ContainerDraw[i][6]; //曲线控制点位置y
                    int x3 = CT_ContainerDraw[i][7]; //曲线终点位置x
                    int y3 = CT_ContainerDraw[i][8]; //曲线终点位置y
                    int w = CT_ContainerDraw[i][9]; //曲线宽度
                    int c = CT_ContainerDraw[i][10]; //曲线颜色
                    CTCanvas_Curve(x1,y1,x2,y2,x3,y3,w,c); //绘制曲线
                }
                if(draw_type == 8) //如果draw类型是三角形
                {
                    int x1 = CT_ContainerDraw[i][3]; //三角形第一点位置x
                    int y1 = CT_ContainerDraw[i][4]; //三角形第一点位置y
                    int x2 = CT_ContainerDraw[i][5]; //三角形第二点位置x
                    int y2 = CT_ContainerDraw[i][6]; //三角形第二点位置y
                    int x3 = CT_ContainerDraw[i][7]; //三角形第三点位置x
                    int y3 = CT_ContainerDraw[i][8]; //三角形第三点位置y
                    int c = CT_ContainerDraw[i][9]; //三角形颜色
                    int f = CT_ContainerDraw[i][10]; //三角形填充
                    CTCanvas_Tri(x1-_x,y1-_y,x2,y2,x3,y3,c,f); //绘制三角形
                }
                if(draw_type == 9) //如果draw类型是图像字符串
                {
                    int x = CT_ContainerDraw[i][3]; //字符串位置x
                    int y = CT_ContainerDraw[i][4]; //字符串位置y
                    int c = CT_ContainerDraw[i][5]; //字符串颜色
                    int font_i = CT_ContainerDraw[i][6]; //字体图像索引
                    int font_n = CT_ContainerDraw[i][7]; //字体字符数量
                    int font_g = CT_ContainerDraw[i][8]; //字体字符间隙
                    int font_h = CT_ContainerDraw[i][9]; //字体字符宽度
                    char *font_cl = CT_ContainerDraw_FontCharList_P[i]; //字体字符列表
                    int *font_gl = CT_ContainerDraw_FontGlythList_P[i]; //字体字形列表
                    CTCanvas_ImgString(x-_x,y-_y,c,CT_ContainerDraw_String[i],font_i,font_n,font_g,font_cl,font_gl); //绘制字符串
                }
            }
        }
    }
}
//--------------------------------------------------------------------