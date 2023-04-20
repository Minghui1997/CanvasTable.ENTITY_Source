#include "../EditString/EditString.h"
#include "../BMPRGBA/BMPRGBA.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef long long varn;
typedef double vard;

//--------------------------------------------------------------------
// 按键键值
//--------------------------------------------------------------------
extern int key_a;
extern int key_b;
extern int key_c;
extern int key_d;
extern int key_e;
extern int key_f;
extern int key_g;
extern int key_h;
extern int key_i;
extern int key_j;
extern int key_k;
extern int key_l;
extern int key_m;
extern int key_n;
extern int key_o;
extern int key_p;
extern int key_q;
extern int key_r;
extern int key_s;
extern int key_t;
extern int key_u;
extern int key_v;
extern int key_w;
extern int key_x;
extern int key_y;
extern int key_z;
extern int key_1;
extern int key_2;
extern int key_3;
extern int key_4;
extern int key_5;
extern int key_6;
extern int key_7;
extern int key_8;
extern int key_9;
extern int key_0;
extern int key_f1;
extern int key_f2;
extern int key_f3;
extern int key_f4;
extern int key_f5;
extern int key_f6;
extern int key_f7;
extern int key_f8;
extern int key_f9;
extern int key_f10;
extern int key_f11;
extern int key_f12;
extern int key_up;
extern int key_down;
extern int key_left;
extern int key_right;
extern int key_esc;
extern int key_tab;
extern int key_cap;
extern int key_shift;
extern int key_ctrl;
extern int key_alt;
extern int key_space;
extern int key_enter;
extern int key_back;
extern int key_num1;
extern int key_num2;
extern int key_num3;
extern int key_num4;
extern int key_num5;
extern int key_num6;
extern int key_num7;
extern int key_num8;
extern int key_num9;
extern int key_num0;
extern int mkey_left;
extern int mkey_middle;
extern int mkey_right;
extern int mkey_up;
extern int mkey_down;
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 图像字体对象
//--------------------------------------------------------------------
typedef struct _CTimgfont
{
    int FontImg;
    int CharNumber;
    int CharGap;
    int CharHeight;
    char *CharList;
    int *GlyphList;
}CTimgfont;
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 窗口对象
//--------------------------------------------------------------------
typedef struct _CTwindow
{
    int x; //窗口位置x
    int y; //窗口位置y
    int width; //窗口宽度
    int height; //窗口高度
    int color_background; //窗口背景颜色
    int color_borderUp; //窗口上边框颜色
    int color_borderDown; //窗口下边框颜色
    int color_borderLeft; //窗口左边框颜色
    int color_borderRight; //窗口右边框颜色
    int image; //窗口背景图像
    int layer; //窗口层
//--------------------------------------------------------------------
    int type; //ui对象类型
    int index; //窗口索引
    int id; //ID
}CTwindow;
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// UI对象
//--------------------------------------------------------------------
typedef struct _CTui
{
    int index; //ui对象索引
    int type; //ui对象类型
    int id; //ID
//--------------------------------------------------------------------
    int x; //UI位置x
    int y; //UI位置y
    int width; //UI宽度
    int height; //UI高度
    int color_background; //UI背景颜色
    int color_borderUp; //UI上边框颜色
    int color_borderDown; //UI下边框颜色
    int color_borderLeft; //UI左边框颜色
    int color_borderRight; //UI右边框颜色
    int color_text; //UI文本颜色
    int color_border; //UI边框颜色
    int color_widget; //UI部件颜色
    int background; //UI背景图像
    char *text; //UI显示文本
    int status; //UI状态
    int group; //UI选框组
    int value; //UI默认值
    int maxvalue; //UI最大值
    int password; //密码文本框
    int number; //数字文本框
    int max; //文本框最大字符数
    int readonly; //文本框只读
    int subbox; //子文本框
    char *name; //文本框名称
    CTimgfont *imgfont; //图像字体
    int image; //图像索引
    int subnum; //子图像数量
    int subimg; //子图像索引
}CTui;
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 场景对象
//--------------------------------------------------------------------
typedef struct _CTscene
{
    int width; //场景宽度
    int height; //场景高度
    int grid_width; //场景网格宽度
    int grid_height; //场景网格高度
    int tile_image[32]; //瓦片图像列表
    int *tile_map[32]; //瓦片地图层0
    void (*sprite_map)(); //sprite地图
//--------------------------------------------------------------------
    int type; //类型
}CTscene;
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// Sprite对象
//--------------------------------------------------------------------
typedef struct _CTsprite
{
    int image; //sprite图像索引
    int layer; //sprite层
    int subnum; //sprite子图像数量
    int subimg; //sprite子图像索引
    int collision_type; //sprite碰撞方式
    int collision_edge; //sprite边缘碰撞
    int cbox_x; //碰撞盒位置x
    int cbox_y; //碰撞盒位置y
    int cbox_w; //碰撞盒宽度
    int cbox_h; //碰撞盒高度
    int cbox_cx; //碰撞盒中心位置x
    int cbox_cy; //碰撞盒中心位置y
    int cbox_r; //碰撞盒半径
    int color; //sprite颜色
    int colora; //sprite透明度
    void (*main)(); //sprite程序main函数
    void (*loop)(); //sprite程序loop函数
//--------------------------------------------------------------------
    int id; //sprite id
    int type; //类型
}CTsprite;
//--------------------------------------------------------------------
void cprint(char *str);
int color(int r,int g,int b,int a);
int mouse_press(int mkey);
int mouse_down(int mkey);
int mouse_up(int mkey);
int mouse_x();
int mouse_y();
int istring_pxwidth(CTimgfont *font,char *str);

void CT_PreferSet();
void CT_WindowCreate();
void CT_WindowShow();
void CT_GraphicsInit();
void CT_OffscreenCanvasCreate();
void CT_AudioInit();
void CT_InputListener();
void CT_TouchKeyRender();
void CT_KeyboardEnd();
long long CT_RunnerClockFrequency();
long long CT_RunnerClockTime();
void CT_RunnerInit();
void CT_RunnerStart();
void CT_RequestFrameStart();
void CT_RequestFrameLoop();
void CT_RunnerLoop();
void CT_RunnerOffRender();
void CT_RunnerReset();
void CT_ContainerPreallocated();
void CT_loadBarWidget();
void CT_SplashShow();
void CT_LoadBarShow(int n);
void CT_LoadBarShowError(int c,char *s);
void CT_ResourceLoadStart(char **rl,size_t rl_s);
void CT_ResourceLoad(size_t rl_n);
void CT_ResourceLoadEnd();
int CT_ResourceLoad_BMP(char *path);
int CT_ResourceLoad_MP3(char *path);
int CT_ResourceLoad_WAV(char *path);
int CT_ResourceLoad_Text(char *path);
void CT_CanvasClear();
void CT_OffCanvas_Clear();
void CT_OffCanvas_Swap();
void CT_OffCanvas_Rectangle(int x,int y,int w,int h,int c,int f);
void CT_OffCanvas_Line(int x1,int y1,int x2,int y2,int w,int c);
void CT_OffCanvasOpaque_Circle(int x,int y,int r,int c,int f);
void CT_OffCanvas_CirclePoint(int x,int y,int r,int c);
void CT_OffCanvasOpaque_Triangle(int x1,int y1,int x2,int y2,int x3,int y3,int c,int f);
void CT_OffCanvas_String(int x,int y,int s,int c,char *t);
void CT_OffCanvas_Image(int x,int y,int i,int a);
void CT_OffCanvas_ImagePart(int x,int y,int i,int a,unsigned int ix,unsigned int iy,unsigned int iw,unsigned int ih);
void CT_OffCanvas_ImagePartFlat(int x,int y,int img,unsigned int six,unsigned int siy,unsigned int siw,unsigned int sih,int col,float sx,float sy,int rot);
void CT_OffCanvas_Curve(int x1,int y1,int x2,int y2,int x3,int y3,int w,int c);
void CT_DrawRender(int l);
void CT_ContainerPreallocated_Draw(int max_l);
void CT_ContainerDrawClear();
void CT_TimerRun();
void CT_GlobalTimerRun();
void CT_ContainerTimerClear();
void CT_ContainerPreallocated_Timer(int max_l);
void CT_ContainerPreallocated_GlobalTimer(int max_l);
void CT_TaskCreate(int task_i,void *task_m,void *task_l);
void CT_TaskDestroy(int ti);
void CT_TaskSetNum(int ti,int vi,varn n);
varn CT_TaskNum(int ti,int vi);
void CT_TaskSetDec(int ti,int vi,vard d);
vard CT_TaskDec(int ti,int vi);
void CT_TaskSetStr(int ti,int vi,char *s);
char *CT_TaskStr(int ti,int vi);
void CT_TaskReady();
void CT_TaskRunMain();
void CT_TaskRunLoop();
void CT_ContainerPreallocated_Task(int max_l);
void CT_ContainerTaskClear();
int CT_UIGetId();
void CT_UIRun();
void CT_UIRender(int l);
void CT_ButtonEnd();
void CT_ContainerPreallocated_Window(int max_l);
void CT_ContainerPreallocated_UI(int max_l);
void CT_ContainerWindowClear();
void CT_ContainerUIClear();
void CT_TileRender(int l);
void CT_TileBuild(int l,int x,int y,int res,int gx,int gy);
void CT_ContainerPreallocated_Tile(int max_l);
void CT_ContainerTileClear();
int CT_SpriteGetId();
void CT_SpriteRender(int l);
void CT_SpriteEnd();
void CT_ContainerPreallocated_Sprite(int max_l);
void CT_ContainerSpriteClear();
void CT_DefaultFontSet();
void CT_OffCanvas_ImgString(int x,int y,int c,char *str,int font_img,int font_number,int font_gap,char *font_charlist,int *font_glyphlist);
void CT_OffCanvas_Glyph(int x,int y,int i,unsigned int ix,unsigned int iy,unsigned int iw,unsigned int ih,int c,unsigned int tex_id,unsigned int img_w,unsigned int img_h);
int CT_ImgStringMeasure(int font_number,int font_gap,char *font_charlist,int *font_glyphlist,char *str);
void CT_DialogCreate(char *val,char *name,int max,int pass,int numb);
void CT_DialogDestroy();
void CT_DialogReturn();
void CT_MBCStoUTF8(char *dest,char *source);
int CT_MBCStoUTF8_Length(char *source);
void CT_UTF8toMBCS(char *dest,char *source);
int CT_UTF8toMBCS_Length(char *source);