#include "CanvasTableSYS.h"
#include <windows.h>
#include <gl/gl.h>
#include <math.h>
#include "GLEXT.h"
#pragma comment(lib,"opengl32.lib")

extern int CT_CanvasW;
extern int CT_CanvasH;
extern int CT_WindowW;
extern int CT_WindowH;
extern char **CT_ContainerResource;
extern unsigned int *CT_ResourceArgList;

unsigned int CT_FrameBufferID = 0; //帧缓冲区ID
unsigned int CT_FrameBufferSurfaceID = 0; //帧缓冲平面ID
unsigned int CT_RenderBufferID = 0; //渲染缓冲区ID

//--------------------------------------------------------------------
// 离屏画布清除
//--------------------------------------------------------------------
void CT_OffCanvas_Clear()
{
    glBindFramebuffer(GL_FRAMEBUFFER,CT_FrameBufferID); //绑定帧缓冲区
    glViewport(0,0,CT_CanvasW,CT_CanvasH); //设置画布视口
    glLoadIdentity(); //将当前矩阵设置为帧缓冲区
    glOrtho(0,CT_CanvasW,CT_CanvasH,0,-1,1); //设置画布正交
    glEnable(GL_BLEND); //启用混合
    glClear(GL_COLOR_BUFFER_BIT); //清除画布
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 离屏画布交换
//--------------------------------------------------------------------
void CT_OffCanvas_Swap()
{
    glDisable(GL_BLEND); //禁用混合
    glBindFramebuffer(GL_FRAMEBUFFER,0); //取消绑定帧缓冲区
    glLoadIdentity(); //将当前矩阵设置为窗口缓冲区
    glViewport(0,0,CT_WindowW,CT_WindowH); //设置窗口视口
    glOrtho(0,CT_WindowW,CT_WindowH,0,-1,1); //设置窗口正交
    glClear(GL_COLOR_BUFFER_BIT); //清除窗口
    glBindTexture(GL_TEXTURE_2D,CT_FrameBufferSurfaceID); //绑定帧缓冲平面
    glColor4ub(255,255,255,255); //设置颜色
    glBegin(GL_POLYGON); //开始绘制 多边形
    glTexCoord2f(0,0); //设置帧缓冲平面的左下角位置
    glVertex2f(0,CT_WindowH); //设置帧缓冲平面顶点
    glTexCoord2f(1,0); //设置帧缓冲平面的右下角坐标
    glVertex2f(CT_WindowW,CT_WindowH); //设置帧缓冲平面顶点
    glTexCoord2f(1,1); //设置帧缓冲平面的右上角坐标
    glVertex2f(CT_WindowW,0); //设置帧缓冲平面顶点
    glTexCoord2f(0,1); //设置帧缓冲平面的左上角坐标
    glVertex2f(0,0); //设置帧缓冲平面顶点
    glEnd(); //结束绘制 多边形
    glFinish(); //结束渲染
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 绘制矩形(离屏)
//--------------------------------------------------------------------
void CT_OffCanvas_Rectangle(int x,int y,int w,int h,int c,int f)
{
    int r = c >> 24 & 0x000000ff; //获取颜色值 red
    int g = c >> 16 & 0x000000ff; //获取颜色值 green
    int b = c >> 8 & 0x000000ff; //获取颜色值 blue
    int a = c & 0x000000ff; //获取颜色值 alpha
    glBindTexture(GL_TEXTURE_2D,0); //取消绑定纹理
    glColor4ub(r,g,b,a); //设置颜色
    if(f == 0) //如果矩形不填充
    {
        glDisable(GL_LINE_SMOOTH); //禁用线条抗锯齿
        glLineWidth(1); //设置线宽度
        glBegin(GL_LINE_LOOP); //开始绘制 闭合线
        glVertex2f(x+0.5,y); //设置顶点
        glVertex2f(x+w-0.5,y); //设置顶点
        glVertex2f(x+w-0.5,y+h-1); //设置顶点
        glVertex2f(x+0.5,y+h-1); //设置顶点
        glEnd(); //结束绘制 闭合线
    }
    if(f == 1) //如果矩形填充
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
// 绘制线(离屏)
//--------------------------------------------------------------------
void CT_OffCanvas_Line(int x1,int y1,int x2,int y2,int w,int c)
{
    int r = c >> 24 & 0x000000ff; //获取颜色值 red
    int g = c >> 16 & 0x000000ff; //获取颜色值 green
    int b = c >> 8 & 0x000000ff; //获取颜色值 blue
    int a = c & 0x000000ff;  //获取颜色值 alpha
    float x1_o = 0; //线起点位置x偏移
    float y1_o = 0; //线起点位置y偏移
    float x2_o = 0; //线终点位置x偏移
    float y2_o = 0; //线终点位置y偏移
    int smooth = 0; //线抗锯齿
    if(x1 == x2) //如果是竖线
    {
        x1_o = 0.5; //线起点位置x偏移
        x2_o = 0.5; //线终点位置x偏移
        if(y1 < y2) //如果线起点位置y小于线终点位置y
        {
            y2_o = 1; //线终点位置y偏移
        }
        if(y1 > y2) //如果线起点位置y大于线终点位置y
        {
            y2_o = -1; //线终点位置y偏移
        }
    }
    if(y1 == y2) //如果是横线
    {
        if(x1 < x2) //如果线起点位置x小于线终点位置x
        {
            x2_o = 1; //线终点位置x偏移
        }
        if(x1 > x2) //如果线起点位置x大于线终点位置x
        {
            x1_o = 1; //线起点位置x偏移
        }
    }
    if(x1 != x2 && y1 != y2) //如果是斜线
    {
        smooth = 1; //线抗锯齿
        if(x1 < x2) //如果线起点位置x小于线终点位置x
        {
            x2_o = 0.5; //线终点位置x偏移
        }
        if(y1 < y2) //如果线起点位置y小于线终点位置y
        {
            y2_o = 0.5; //线终点位置y偏移
        }
        if(x1 > x2) //如果线起点位置x大于线终点位置x
        {
            x1_o = 0.5; //线起点位置x偏移
        }
        if(y1 > y2) //如果线起点位置y大于线终点位置y
        {
            y1_o = 0.5; //线起点位置y偏移
        }
    }
    glBindTexture(GL_TEXTURE_2D,0); //取消绑定纹理
    glColor4ub(r,g,b,a); //设置颜色
    if(smooth == 1) //如果线抗锯齿
    {
        glEnable(GL_LINE_SMOOTH); //启用线条抗锯齿
    }
    else
    {
        glDisable(GL_LINE_SMOOTH); //禁用线条抗锯齿
    }
    glLineWidth(w*0.8); //设置线宽度
    glBegin(GL_LINES); //开始绘制 线
    glVertex2f(x1+x1_o,y1+y1_o); //设置顶点
    glVertex2f(x2+x2_o,y2+y2_o); //设置顶点
    glEnd(); //结束绘制 线
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 绘制不透明圆(离屏)
//--------------------------------------------------------------------
void CT_OffCanvasOpaque_Circle(int x,int y,int r,int c,int f)
{
    int cr = c >> 24 & 0x000000ff; //获取颜色值 red
    int cg = c >> 16 & 0x000000ff; //获取颜色值 green
    int cb = c >> 8 & 0x000000ff;  //获取颜色值 blue
    float pi = 3.1415926; //圆周率
    float lx = 0; //顶点位置x
    float ly = 0; //顶点位置y
    int n = r * 3; //边数
    int i;
    glBindTexture(GL_TEXTURE_2D,0); //取消绑定纹理
    glColor4ub(cr,cg,cb,255); //设置颜色
    if(f == 0) //如果圆不填充
    {
        glEnable(GL_LINE_SMOOTH); //启用线条抗锯齿
        glLineWidth(1); //设置线宽度
        glBegin(GL_LINE_LOOP); //开始绘制 闭合线
        for(i=0;i<n;i++)
        {
            lx = x + r * cos( 2 * pi * i / n); //顶点位置x
            ly = y + r * sin( 2 * pi * i / n); //顶点位置y
            glVertex2f(lx+0.5,ly+0.25); //设置顶点
        }
        glEnd(); //结束绘制 闭合线  
    }
    if(f == 1) //如果圆填充
    {
        glEnable(GL_LINE_SMOOTH); //启用线条抗锯齿
        glLineWidth(1.5); //设置线宽度
        glBegin(GL_LINE_LOOP); //开始绘制 闭合线
        for(i=0;i<n;i++)
        {
            lx = x + (r - 0.5) * cos( 2 * pi * i / n); //顶点位置x
            ly = y + (r - 0.5) * sin( 2 * pi * i / n); //顶点位置y
            glVertex2f(lx+0.5,ly+0.25); //设置顶点
        }
        glEnd(); //结束绘制 闭合线
        glBegin(GL_POLYGON); //开始绘制 多边形
        for(i=0;i<n;i++)
        {
            lx = x + (r - 1.0 + 0.5) * cos( 2 * pi * i / n); //顶点位置x
            ly = y + (r - 1.0 + 0.5) * sin( 2 * pi * i / n); //顶点位置y
            glVertex2f(lx+0.5,ly+0.75); //设置顶点
        }
        glEnd(); //结束绘制 闭合线
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 绘制圆点(离屏)
//--------------------------------------------------------------------
void CT_OffCanvas_CirclePoint(int x,int y,int r,int c)
{
    int cr = c >> 24 & 0x000000ff; //获取颜色值 red
    int cg = c >> 16 & 0x000000ff; //获取颜色值 green
    int cb = c >> 8 & 0x000000ff; //获取颜色值 blue
    int ca = c & 0x000000ff;  //获取颜色值 alpha
    glColor4ub(cr,cg,cb,ca); //设置颜色
    glPointSize(r*2+1); //设置点大小
    glBegin(GL_POINTS); //开始绘制 点
    glVertex2f(x+0.5,y+0.5); //设置顶点
    glEnd(); //结束绘制 点
}
//--------------------------------------------------------------------
// 绘制文本(离屏)
//--------------------------------------------------------------------
void CT_OffCanvas_String(int x,int y,int s,int c,char *t)
{
    cprint("ERROR : This function is invalid in CanvasTable.ENTITY,please use function draw_istring().");
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 绘制图像(离屏)
//--------------------------------------------------------------------
void CT_OffCanvas_Image(int x,int y,int i,int a)
{
    unsigned int img_w = CT_ResourceArgList[i*5+2]; //获取图像宽度
    unsigned int img_h = CT_ResourceArgList[i*5+3]; //获取图像高度
    unsigned int tex_id = CT_ResourceArgList[i*5+4]; //获取纹理id
    glBindTexture(GL_TEXTURE_2D,tex_id); //绑定纹理
    glColor4ub(255,255,255,a); //设置颜色
    glBegin(GL_POLYGON); //开始绘制 多边形
    glTexCoord2f(0,0); //设置纹理的左下角坐标
    glVertex2f(x,y+img_h); //设置顶点
    glTexCoord2f(1,0); //设置纹理的右下角坐标
    glVertex2f(x+img_w,y+img_h); //设置顶点
    glTexCoord2f(1,1); //设置纹理的右上角坐标
    glVertex2f(x+img_w,y); //设置顶点
    glTexCoord2f(0,1); //设置纹理的左上角坐标
    glVertex2f(x,y); //设置顶点
    glEnd(); //结束绘制 多边形
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 绘制图像部分(离屏)
//--------------------------------------------------------------------
void CT_OffCanvas_ImagePart(int x,int y,int i,int a,unsigned int ix,unsigned int iy,unsigned int iw,unsigned int ih)
{
    unsigned int img_w = CT_ResourceArgList[i*5+2]; //获取图像宽度
    unsigned int img_h = CT_ResourceArgList[i*5+3]; //获取图像高度
    unsigned int tex_id = CT_ResourceArgList[i*5+4]; //获取纹理id
    float part1_x = (float)ix / (float)img_w; //图像部分左下角位置x
    float part1_y = ((float)img_h - (float)iy - (float)ih) / (float)img_h; //图像部分左下角位置y
    float part2_x = ((float)ix + (float)iw) / (float)img_w; //图像部分右下角位置x
    float part2_y = ((float)img_h - (float)iy - (float)ih) / (float)img_h; //图像部分右下角位置y
    float part3_x = ((float)ix + (float)iw) / (float)img_w; //图像部分右上角位置x
    float part3_y = ((float)img_h - (float)iy) / (float)img_h; //图像部分右上角位置y
    float part4_x = (float)ix / (float)img_w; //图像部分左上角位置x
    float part4_y = ((float)img_h - (float)iy) / (float)img_h; //图像部分左上角位置y
    glBindTexture(GL_TEXTURE_2D,tex_id); //绑定纹理
    glColor4ub(255,255,255,a); //设置颜色
    glBegin(GL_POLYGON); //开始绘制 多边形
    glTexCoord2f(part1_x,part1_y); //设置纹理的左下角坐标
    glVertex2f(x,y+ih); //设置顶点
    glTexCoord2f(part2_x,part2_y); //设置纹理的右下角坐标
    glVertex2f(x+iw,y+ih); //设置顶点
    glTexCoord2f(part3_x,part3_y); //设置纹理的右上角坐标
    glVertex2f(x+iw,y); //设置顶点
    glTexCoord2f(part4_x,part4_y); //设置纹理的左上角坐标
    glVertex2f(x,y); //设置顶点
    glEnd(); //结束绘制 多边形
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 绘制平面图像部分(离屏)
//--------------------------------------------------------------------
void CT_OffCanvas_ImagePartFlat(int x,int y,int img,unsigned int six,unsigned int siy,unsigned int siw,unsigned int sih,int col,float sx,float sy,int rot)
{
    unsigned int img_w = CT_ResourceArgList[img*5+2]; //获取图像宽度
    unsigned int img_h = CT_ResourceArgList[img*5+3]; //获取图像高度
    unsigned int tex_id = CT_ResourceArgList[img*5+4]; //获取纹理id
    float part1_x = (float)six / (float)img_w; //计算图像部分左下角位置x
    float part1_y = ((float)img_h - (float)siy - (float)sih) / (float)img_h; //计算图像部分左下角位置y
    float part2_x = ((float)six + (float)siw) / (float)img_w; //计算图像部分右下角位置x
    float part2_y = ((float)img_h - (float)siy - (float)sih) / (float)img_h; //计算图像部分右下角位置y
    float part3_x = ((float)six + (float)siw) / (float)img_w; //计算图像部分右上角位置x
    float part3_y = ((float)img_h - (float)siy) / (float)img_h; //计算图像部分右上角位置y
    float part4_x = (float)six / (float)img_w; //计算图像部分左上角位置x
    float part4_y = ((float)img_h - (float)siy) / (float)img_h; //计算图像部分左上角位置y
    if(col != 0xffffffff) //如果设置了sprite颜色
    {
        int r = col >> 24 & 0x000000ff; //获取颜色值 red
        int g = col >> 16 & 0x000000ff; //获取颜色值 green
        int b = col >> 8 & 0x000000ff;  //获取颜色值 blue
        int a = col & 0x000000ff; //获取颜色值 alpha
        glColor4ub(r,g,b,a); //设置颜色
    }
    glBindTexture(GL_TEXTURE_2D,tex_id); //绑定纹理
    glBegin(GL_POLYGON); //开始绘制 多边形
    glTexCoord2f(part1_x,part1_y); //设置图像位置左下角坐标
    glVertex2f(x,y+sih); //设置顶点
    glTexCoord2f(part2_x,part2_y); //设置图像位置右下角坐标
    glVertex2f(x+siw,y+sih); //设置顶点
    glTexCoord2f(part3_x,part3_y); //设置图像位置右上角坐标
    glVertex2f(x+siw,y); //设置顶点
    glTexCoord2f(part4_x,part4_y); //设置图像位置左上角坐标
    glVertex2f(x,y); //设置顶点
    glEnd(); //结束绘制 多边形
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 绘制三角形(离屏)
//--------------------------------------------------------------------
void CT_OffCanvasOpaque_Triangle(int x1,int y1,int x2,int y2,int x3,int y3,int c,int f)
{
    int r = c >> 24 & 0x000000ff; //获取颜色值 red
    int g = c >> 16 & 0x000000ff; //获取颜色值 green
    int b = c >> 8 & 0x000000ff; //获取颜色值 blue
    glBindTexture(GL_TEXTURE_2D,0); //取消绑定纹理
    glColor4ub(r,g,b,255); //设置颜色
    if(f == 0) //如果三角形不填充
    {
        glEnable(GL_LINE_SMOOTH); //启用线条抗锯齿
        glLineWidth(1); //设置线宽度
        glBegin(GL_LINE_LOOP); //开始绘制 闭合线
        glVertex2f(x1+0.25,y1+0.25); //设置顶点
        glVertex2f(x2+0.25,y2+0.25); //设置顶点
        glVertex2f(x3+0.25,y3+0.25); //设置顶点
        glEnd(); //结束绘制 闭合线
    }
    if(f == 1) //如果三角形填充
    {
        glEnable(GL_LINE_SMOOTH); //启用线条抗锯齿
        glLineWidth(1); //设置线宽度
        glBegin(GL_LINE_LOOP); //开始绘制 闭合线
        glVertex2f(x1+0.25,y1+0.25); //设置顶点
        glVertex2f(x2+0.25,y2+0.25); //设置顶点
        glVertex2f(x3+0.25,y3+0.25); //设置顶点
        glEnd(); //结束绘制 闭合线
        glBegin(GL_TRIANGLES); //开始绘制 多边形
        glVertex2f(x1+0.25,y1+0.25); //设置顶点
        glVertex2f(x2+0.25,y2+0.25); //设置顶点
        glVertex2f(x3+0.25,y3+0.25); //设置顶点
        glEnd(); //结束绘制 多边形
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 绘制曲线(离屏)
//--------------------------------------------------------------------
void CT_OffCanvas_Curve(int x1,int y1,int x2,int y2,int x3,int y3,int w,int c)
{
    int r = c >> 24 & 0x000000ff; //获取颜色值 red
    int g = c >> 16 & 0x000000ff; //获取颜色值 green
    int b = c >> 8 & 0x000000ff; //获取颜色值 blue
    int a = c & 0x000000ff; //获取颜色值 alpha
    int i;
    float ctrlpoints[4][3] = {{x1+0.5,y1+0.5,0},{x2+0.5,y2+0.5,0},{x2+0.5,y2+0.5,0},{x3+0.5,y3+0.5,0}}; //控制点数组
    glBindTexture(GL_TEXTURE_2D,0); //取消绑定纹理
    glColor4ub(r,g,b,a); //设置颜色
    glEnable(GL_LINE_SMOOTH); //启用线条抗锯齿
    glLineWidth(w*0.85); //设置线宽度
    glShadeModel(GL_FLAT); //设置着色模式
    glMap1f(GL_MAP1_VERTEX_3,0.0,1.0,3,4,&ctrlpoints[0][0]); //定义一维计算器
    glBegin(GL_LINE_STRIP); //开始绘制 线带
    for(i=0;i<=30;i++)
    {
        glEvalCoord1f((float)i/30.0); //定义映射
    }
    glEnd(); //结束绘制 线带
}
//--------------------------------------------------------------------