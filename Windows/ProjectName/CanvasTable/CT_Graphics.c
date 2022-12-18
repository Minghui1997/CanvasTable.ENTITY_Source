#include "CanvasTableSYS.h"
#include <windows.h>
#include <gl/gl.h>
#pragma comment(lib,"opengl32.lib")

extern HDC CT_DCMW;
extern char **CT_ContainerResource;
extern int **CT_ResourceArgList;

//--------------------------------------------------------------------
// 清除缓冲画布
//--------------------------------------------------------------------
void CTCanvas_Clear()
{
    glClear(GL_COLOR_BUFFER_BIT); //清除缓冲区
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 缓冲画布输出
//--------------------------------------------------------------------
void CTCanvas_Out()
{
    SwapBuffers(CT_DCMW); //输出缓冲区
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 绘制矩形(缓冲)
//--------------------------------------------------------------------
void CTCanvas_Rect(int x,int y,int w,int h,int c,int f)
{
    int r = c >> 24 & 0x000000ff; //获取颜色值 red
    int g = c >> 16 & 0x000000ff; //获取颜色值 green
    int b = c >> 8 & 0x000000ff; //获取颜色值 blue
    int a = c & 0x000000ff; //获取颜色值 alpha
    glBindTexture(GL_TEXTURE_2D,0); //取消绑定纹理
    glColor4ub(r,g,b,a); //设置颜色
    if(f == 0) //如果是空心
    {
        glBegin(GL_LINE_LOOP); //开始绘制 闭合线
        glVertex2f(x+0.5,y+0.5); //设置顶点
        glVertex2f(x+w-0.5,y+0.5); //设置顶点
        glVertex2f(x+w-0.5,y+h-0.5); //设置顶点
        glVertex2f(x+0.5,y+h-0.5); //设置顶点
        glEnd(); //结束绘制 闭合线
    }
    if(f == 1) //如果是实心
    {
        glBegin(GL_POLYGON); //开始绘制 多边形
        glVertex2f(x,y); //设置顶点
        glVertex2f(x+w,y); //设置顶点
        glVertex2f(x+w,y+h); //设置顶点
        glVertex2f(x,y+h); //设置顶点
        glEnd(); //结束绘制 多边形
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 绘制线(缓冲)
//--------------------------------------------------------------------
void CTCanvas_Line(int x1,int y1,int x2,int y2,int w,int c)
{
    int r = c >> 24 & 0x000000ff; //获取颜色值 red
    int g = c >> 16 & 0x000000ff; //获取颜色值 green
    int b = c >> 8 & 0x000000ff; //获取颜色值 blue
    int a = c & 0x000000ff;  //获取颜色值 alpha
    float line_move = w * 0.5; //线偏移
    glBindTexture(GL_TEXTURE_2D,0); //取消绑定纹理
    glColor4ub(r,g,b,a); //设置颜色
    glLineWidth(w); //设置线宽度
    glBegin(GL_LINES); //开始绘制 线
    if(x1 == x2) //如果是竖线
    {
        glVertex2f(x1+line_move,y1); //设置顶点
        glVertex2f(x2+line_move,y2); //设置顶点
    }
    if(y1 == y2) //如果是横线
    {
        glVertex2f(x1,y1+line_move); //设置顶点
        glVertex2f(x2,y2+line_move); //设置顶点
    }
    if(x1 != x2 && y1 != y2) //如果是斜线
    {
        glVertex2f(x1,y1); //设置顶点
        glVertex2f(x2,y2); //设置顶点
    }
    glEnd(); //结束绘制 线
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 绘制圆(缓冲)
//--------------------------------------------------------------------
void CTCanvas_Circle(int x,int y,int r,int c,int f)
{
    int cr = c >> 24 & 0x000000ff; //获取颜色值 red
    int cg = c >> 16 & 0x000000ff; //获取颜色值 green
    int cb = c >> 8 & 0x000000ff;  //获取颜色值 blue
    int ca = c & 0x000000ff; //获取颜色值 alpha
    float px = 0; //点位置
    float py = 0; //点位置
    float pi = 3.1415926; //圆周率
    float theta = 0;
    int i;
    glBindTexture(GL_TEXTURE_2D,0); //取消绑定纹理
    glColor4ub(cr,cg,cb,ca); //设置颜色
    if(f == 0) //如果是空心
    {
        glBegin(GL_LINE_LOOP); //开始绘制 闭合线
        for (i=0;i<360;i++)
        {
            theta = i * pi / 180;
            glVertex2f(x+r*cos(theta),y+r*sin(theta));
        }
        glEnd(); //结束绘制 闭合线
    }
    if(f == 1) //如果是实心
    {
        glBegin(GL_POLYGON); //开始绘制 多边形
        for (i=0;i<360;i++)
        {
            theta = i * pi / 180;
            glVertex2f(x+r*cos(theta),y+r*sin(theta));
        }
        glEnd(); //结束绘制 多边形
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 绘制文本(缓冲)
//--------------------------------------------------------------------
void CTCanvas_String(int x,int y,int s,int c,char *t)
{

}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 绘制图像(缓冲)
//--------------------------------------------------------------------
void CTCanvas_Image(int x,int y,int i)
{
    int img_w = CT_ResourceArgList[i][2]; //获取图像宽度
    int img_h = CT_ResourceArgList[i][3]; //获取图像高度
    int tex_id = CT_ResourceArgList[i][4]; //获取纹理id
    double part1_x = (double)0; //计算图像部分左下角位置x
    double part1_y = (double)0; //计算图像部分左下角位置y
    double part2_x = (double)img_w / (double)img_w; //计算图像部分右下角位置x
    double part2_y = (double)0; //计算图像部分右下角位置y
    double part3_x = (double)img_w / (double)img_w; //计算图像部分右上角位置x
    double part3_y = (double)img_h / (double)img_h; //计算图像部分右上角位置y
    double part4_x = (double)0; //计算图像部分左上角位置x
    double part4_y = (double)img_h / (double)img_h; //计算图像部分左上角位置y
    glBindTexture(GL_TEXTURE_2D,tex_id); //绑定纹理
    glColor4ub(255,255,255,255); //设置颜色
    glBegin(GL_QUADS); //开始绘制 四边形
    glTexCoord2d(part1_x,part1_y); //设置图像位置左下角坐标
    glVertex2i(x,y+img_h); //设置顶点
    glTexCoord2d(part2_x,part2_y); //设置图像位置右下角坐标
    glVertex2i(x+img_w,y+img_h); //设置顶点
    glTexCoord2d(part3_x,part3_y); //设置图像位置右上角坐标
    glVertex2i(x+img_w,y); //设置顶点
    glTexCoord2d(part4_x,part4_y); //设置图像位置左上角坐标
    glVertex2i(x,y); //设置顶点
    glEnd(); //结束绘制 四边形
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 绘制图像部分(缓冲)
//--------------------------------------------------------------------
void CTCanvas_ImagePart(int x,int y,int i,int ix,int iy,int iw,int ih)
{
    int img_w = CT_ResourceArgList[i][2]; //获取图像宽度
    int img_h = CT_ResourceArgList[i][3]; //获取图像高度
    int tex_id = CT_ResourceArgList[i][4]; //获取纹理id
    double part1_x = (double)ix / (double)img_w; //计算图像部分左下角位置x
    double part1_y = ((double)img_h - (double)iy - (double)ih) / (double)img_h; //计算图像部分左下角位置y
    double part2_x = ((double)ix + (double)iw) / (double)img_w; //计算图像部分右下角位置x
    double part2_y = ((double)img_h - (double)iy - (double)ih) / (double)img_h; //计算图像部分右下角位置y
    double part3_x = ((double)ix + (double)iw) / (double)img_w; //计算图像部分右上角位置x
    double part3_y = ((double)img_h - (double)iy) / (double)img_h; //计算图像部分右上角位置y
    double part4_x = (double)ix / (double)img_w; //计算图像部分左上角位置x
    double part4_y = ((double)img_h - (double)iy) / (double)img_h; //计算图像部分左上角位置y
    glBindTexture(GL_TEXTURE_2D,tex_id); //绑定纹理
    glColor4ub(255,255,255,255); //设置颜色
    glBegin(GL_QUADS); //开始绘制 四边形
    glTexCoord2d(part1_x,part1_y); //设置图像位置左下角坐标
    glVertex2i(x,y+ih); //设置顶点
    glTexCoord2d(part2_x,part2_y); //设置图像位置右下角坐标
    glVertex2i(x+iw,y+ih); //设置顶点
    glTexCoord2d(part3_x,part3_y); //设置图像位置右上角坐标
    glVertex2i(x+iw,y); //设置顶点
    glTexCoord2d(part4_x,part4_y); //设置图像位置左上角坐标
    glVertex2i(x,y); //设置顶点
    glEnd(); //结束绘制 四边形
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 绘制三角形(缓冲)
//--------------------------------------------------------------------
void CTCanvas_Tri(int x1,int y1,int x2,int y2,int x3,int y3,int c,int f)
{
    int r = c >> 24 & 0x000000ff; //获取颜色值 red
    int g = c >> 16 & 0x000000ff; //获取颜色值 green
    int b = c >> 8 & 0x000000ff; //获取颜色值 blue
    int a = c & 0x000000ff; //获取颜色值 alpha
    glBindTexture(GL_TEXTURE_2D,0); //取消绑定纹理
    glColor4ub(r,g,b,a); //设置颜色
    if(f == 0) //如果是空心
    {
        glBegin(GL_LINE_LOOP); //开始绘制 闭合线
        glVertex2f(x1+0.5,y1+0.5); //设置顶点
        glVertex2f(x2+0.5,y2+0.5); //设置顶点
        glVertex2f(x3+0.5,y3+0.5); //设置顶点
        glEnd(); //结束绘制 闭合线
    }
    if(f == 1)
    {
        glBegin(GL_POLYGON); //开始绘制 多边形
        glVertex2f(x1,y1); //设置顶点
        glVertex2f(x2,y2); //设置顶点
        glVertex2f(x3,y3); //设置顶点
        glEnd(); //结束绘制 多边形
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 绘制曲线(缓冲)
//--------------------------------------------------------------------
void CTCanvas_Curve(int x1,int y1,int x2,int y2,int x3,int y3,int w,int c)
{
    int r = c >> 24 & 0x000000ff; //获取颜色值 red
    int g = c >> 16 & 0x000000ff; //获取颜色值 green
    int b = c >> 8 & 0x000000ff; //获取颜色值 blue
    int a = c & 0x000000ff; //获取颜色值 alpha
    int i;
    float ctrlpoints[4][3] = {{x1,y1,0},{x2,y2,0},{x2,y2,0},{x3,y3,0}}; //控制点数组
    glColor4ub(r,g,b,a); //设置颜色
    glLineWidth(w); //设置线宽度
    glShadeModel(GL_FLAT); //设置着色模式
    glMap1f(GL_MAP1_VERTEX_3,0.0,1.0,3,4,&ctrlpoints[0][0]); //定义一维计算器
    glBegin(GL_LINE_STRIP); //开始绘制 线带
    for (i=0;i<=30;i++)
    {
        glEvalCoord1f((GLfloat)i/30.0); //定义映射
    }
    glEnd(); //结束绘制 线带
}
//--------------------------------------------------------------------