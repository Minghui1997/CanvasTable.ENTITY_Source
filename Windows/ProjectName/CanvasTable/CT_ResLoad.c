#define _CRT_SECURE_NO_WARNINGS

#include "CanvasTableSYS.h"
#include <windows.h>
#include <gl/gl.h>
#include "../DirectSound8/dsound.h"
#pragma comment(lib,"DirectSound8/dsound.lib")
#pragma comment(lib,"opengl32.lib")

extern int CT_CanvasW;
extern int CT_CanvasH;
extern int CT_SPLASH_SHOW;
extern int CT_LOADBAR_SHOW;
extern int CT_DF_Img;
extern int CT_DF_Number;
extern int CT_DF_Gap;
extern int CT_DF_Height;
extern char *CT_DF_CharList;
extern int *CT_DF_GlyphList;
extern int CT_DF_TID;
extern LPDIRECTSOUND8 CT_DirectSound;

void (*CT_Main)(); //Main指针函数
char **CT_ContainerResource = NULL; //资源容器
char **CT_ResourceList = NULL; //资源列表
unsigned int *CT_ResourceArgList = NULL; //资源参数列表
LPDIRECTSOUNDBUFFER8 *CT_DSBuffer_List; //DirectSound Buffer列表
int CT_ResLength = 0; //资源容器长度
int CT_DS_Length = 0; //DirectSound Buffer列表长度
int CT_LoadBarWidgetLoad = 0; //加载条块加载数
int CT_LoadBarWidgetIndex = 0; //加载条块索引
char *CT_SplashImage = NULL; //Splash图像
unsigned int CT_SIwidth = 0; //Splash图像宽度
unsigned int CT_SIheight = 0; //Splash图像高度
unsigned int CT_SplashImageTexID = 0; //Splash图像纹理id
//--------------------------------------------------------------------
// 资源加载开始
//--------------------------------------------------------------------
void CT_ResourceLoadStart(char **rl,size_t rl_s)
{
    int i;
    int rl_l = rl_s / sizeof(char*); //资源列表长度
    cprint("-----Load resource-----");
    CT_ResourceList = malloc(rl_s); //为资源列表分配内存
    CT_ResourceArgList = malloc(rl_l*sizeof(int)*5); //为为资源列表分配内存
    CT_ContainerResource = malloc(rl_s); //为资源容器分配内存
    for(i=0;i<rl_l;i++) //遍历资源列表(参数)
    {
        size_t path_l = strlen(rl[i]); //获取当前资源路径的长度
        CT_ResourceList[i] = malloc(path_l+1); //在资源列表中为当前资源路径分配内存
        CT_ResourceList[i][path_l] = 0x00;
        strcpy(CT_ResourceList[i],rl[i]); //将当前资源路径(参数)复制到资源列表中
    }
    if(CT_SPLASH_SHOW == 1) //如果SPLASH显示
    {
        FILE *bmp_file = fopen("splash.bmp","rb"); //bmp文件指针
        if(bmp_file != NULL) //如果splash.bmp文件存在
        {
            BMPRGBA_BmpInf splash_inf; //Splash图像信息
            BMPRGBA_BmpGetInf(&splash_inf,bmp_file); //获取Splash图像信息
            CT_SIwidth = splash_inf.BmpWidth; //获取Splash图像宽度
            CT_SIheight = splash_inf.BmpHeight; //获取Splash图像高度
            CT_SplashImage = malloc(CT_SIwidth * CT_SIheight * 4); //为Splash图像分配内存
            BMPRGBA_BmpToRGBA(CT_SplashImage,&splash_inf,bmp_file); //加载Splash图像
            glGenTextures(1,&CT_SplashImageTexID); //生成纹理
            glBindTexture(GL_TEXTURE_2D,CT_SplashImageTexID); //绑定纹理
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //设置纹理过滤
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); //设置纹理过滤
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP); //设置纹理环绕
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP); //设置纹理环绕
            glTexImage2D(GL_TEXTURE_2D,0,4,CT_SIwidth,CT_SIheight,0,GL_RGBA,GL_UNSIGNED_BYTE,CT_SplashImage); //加载纹理
            free(CT_SplashImage); //释放Splash图像
            CT_LoadBarShow(0); //显示加载条
            CT_ResourceLoad(rl_l); //加载资源
            return; //中断
        }
        CT_LoadBarShowError(0,"splash.xxx"); //显示错误信息
    }
    else
    {
        CT_LoadBarShow(0); //显示加载条
        CT_ResourceLoad(rl_l); //加载资源
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 资源加载
//--------------------------------------------------------------------
void CT_ResourceLoad(size_t rl_n)
{
    int res_faild = 0; //加载失败
    int i;
    for(i=0;i<rl_n;i++) //遍历资源列表
    {
        if(CT_ResourceList[i][0] != NULL) //当前文件路径不为空
        {
            char res_extname[17] = ""; //资源文件扩展名
            int res_type = 0;
            strslice(res_extname,CT_ResourceList[i],charindex_last(CT_ResourceList[i],'.')+1,strlen(CT_ResourceList[i])); //获取当前资源文件名
            if(strcmp(res_extname,"png") == 0 || strcmp(res_extname,"PNG") == 0) //如果当前资源是png
            {
                res_type = 1;
            }
            if(strcmp(res_extname,"jpg") == 0 || strcmp(res_extname,"JPG") == 0 || strcmp(res_extname,"jpeg") == 0 || strcmp(res_extname,"JPEG") == 0) //如果当前资源是jpg
            {
                res_type = 2;
            }
            if(strcmp(res_extname,"bmp") == 0 || strcmp(res_extname,"BMP") == 0) //如果当前资源是bmp
            {
                int res_status = CT_ResourceLoad_BMP(CT_ResourceList[i]); //加载BMP图像
                res_type = 3;
                if(res_status == 0) //如果资源加载失败
                {
                    res_faild = 1;
                    CT_LoadBarShowError(0,CT_ResourceList[i]); //显示错误信息
                    break; //中断
                }
                            
            }
            if(strcmp(res_extname,"mp3") == 0 || strcmp(res_extname,"MP3") == 0) //如果当前资源是mp3
            {
                res_type = 4;
            }
            if(strcmp(res_extname,"ogg") == 0 || strcmp(res_extname,"OGG") == 0) //如果当前资源是ogg
            {
                res_type = 5;
            }
            if(strcmp(res_extname,"wav") == 0 || strcmp(res_extname,"WAV") == 0) //如果当前资源是wav
            {
                int res_status = CT_ResourceLoad_WAV(CT_ResourceList[i]);
                res_type = 6;
                if(res_status == 0) //如果资源加载失败
                {
                    res_faild = 1;
                    CT_LoadBarShowError(0,CT_ResourceList[i]); //显示错误信息
                    break; //中断
                }
            }
            if(res_type == 0) //如果当前资源是文本
            {
                int res_status = CT_ResourceLoad_Text(CT_ResourceList[i]);
                if(res_status == 0) //如果资源加载失败
                {
                    res_faild = 1;
                    CT_LoadBarShowError(0,CT_ResourceList[i]); //显示错误信息
                    break; //中断
                }
            }
        }
    }
    if(res_faild == 0) //如果没有资源加载失败
    {
        CT_ResourceLoadEnd(); //结束加载资源
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// BMP图像加载
//--------------------------------------------------------------------
int CT_ResourceLoad_BMP(char *path)
{
    unsigned int tex_id = 0; //纹理id
    char infor[257] = "";
    size_t ldata_i = CT_ResLength * 5; //资源参数数据索引
    FILE *bmp_file = fopen(path,"rb"); //BMP文件指针
    if(bmp_file != NULL) //如果BMP文件指针不为空
    {
        BMPRGBA_BmpInf res_bmp; //BMP资源信息
        BMPRGBA_BmpGetInf(&res_bmp,bmp_file); //获取BMP图像信息
        CT_ContainerResource[CT_ResLength] = malloc(res_bmp.BmpWidth * res_bmp.BmpHeight * 4); //为资源容器中的资源分配内存
        BMPRGBA_BmpToRGBA(CT_ContainerResource[CT_ResLength],&res_bmp,bmp_file); //加载bmp图像到资源容器
        fclose(bmp_file); //关闭bmp文件
        glGenTextures(1,&tex_id); //生成纹理
        glBindTexture(GL_TEXTURE_2D,tex_id); //绑定纹理
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //设置纹理过滤
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); //设置纹理过滤
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP); //设置纹理环绕
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP); //设置纹理环绕
        glTexImage2D(GL_TEXTURE_2D,0,4,res_bmp.BmpWidth,res_bmp.BmpHeight,0,GL_RGBA,GL_UNSIGNED_BYTE,CT_ContainerResource[CT_ResLength]); //加载纹理
        free(CT_ContainerResource[CT_ResLength]); //释放资源
        CT_ResourceArgList[ldata_i] = 1; //存入资源占位符
        CT_ResourceArgList[ldata_i+1] = 0; //存入资源参数
        CT_ResourceArgList[ldata_i+2] = res_bmp.BmpWidth; //存入图像宽度
        CT_ResourceArgList[ldata_i+3] = res_bmp.BmpHeight; //存入图像高度
        CT_ResourceArgList[ldata_i+4] = tex_id; //存入纹理id
        CT_loadBarWidget(); //加载条循环
        CT_ResLength ++; //资源长度 +1
        strcat(infor,"Resource load : ");
        strcat(infor,path);
        cprint(infor);
        return 1;
    }
    return 0;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// MP3音频加载
//--------------------------------------------------------------------
int CT_ResourceLoad_MP3(char *path)
{
    return 0;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// WAV音频加载
//--------------------------------------------------------------------
int CT_ResourceLoad_WAV(char *path)
{
    char infor[257] = "";
    char wav_head_data[128] = ""; //WAV头数据
    char str5[5] = ""; //字节数组5
    int pcm_data_i = 0; //WAV头数据data标志的位置
    int i;
    size_t ldata_i = CT_ResLength * 5; //列表数据索引
    DSBUFFERDESC DS_buffer_esc; //DS BUFFER ESC
    WAVEFORMATEX wav_format; //WAV音频格式
    char *lpvWrite; //LPV指针
    int dwLength = 0; //DW长度
    FILE *wav_file = fopen(path,"rb"); //WAV文件指针
    if(wav_file != NULL) //如果当前资源文件存在
    {
        fseek(wav_file,0,SEEK_SET); //WAV文件定位0
        fread(wav_head_data,128,1,wav_file); //读取文件头数据 - 128字节
        for(i=32;i<124;i++) //遍历PCM头数据
        {
            strslice(str5,wav_head_data,i,i+4); //截取4字节数据
            if(strcmp(str5,"data") == 0) //如果找到data标志
            {
                pcm_data_i = i + 8; //PCM数据位置
                break; //中断
            }
        }
        memset(&wav_format, 0, sizeof(WAVEFORMATEX));
        wav_format.wFormatTag = WAVE_FORMAT_PCM;  //WAV音频格式PCM
        wav_format.nChannels = wav_head_data[22] & 0x000000ff; //获取PCM声道
        wav_format.nSamplesPerSec = ((wav_head_data[27] & 0x000000ff) * 0x1000000) + ((wav_head_data[26] & 0x000000ff) * 0x10000) + ((wav_head_data[25] & 0x000000ff) * 0x100) + (wav_head_data[24] & 0x000000ff); //获取PCM采样率
        wav_format.nBlockAlign = 4; //PCM块对齐
        wav_format.nAvgBytesPerSec = ((wav_head_data[31] & 0x000000ff) * 0x1000000) + ((wav_head_data[30] & 0x000000ff) * 0x10000) + ((wav_head_data[29] & 0x000000ff) * 0x100) + (wav_head_data[28] & 0x000000ff); //获取PCM传输速率
        wav_format.wBitsPerSample = 16; //PCM采样字节

        memset(&DS_buffer_esc, 0, sizeof(DSBUFFERDESC));
        DS_buffer_esc.dwSize = sizeof(DSBUFFERDESC);
        DS_buffer_esc.dwFlags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY | DSBCAPS_GLOBALFOCUS | DSBCAPS_LOCDEFER; //DirectSound旗
        DS_buffer_esc.dwBufferBytes = ((wav_head_data[pcm_data_i-1] & 0x000000ff) * 0x1000000) + ((wav_head_data[pcm_data_i-2] & 0x000000ff) * 0x10000) + ((wav_head_data[pcm_data_i-3] & 0x000000ff) * 0x100) + (wav_head_data[pcm_data_i-4] & 0x000000ff); //获取PCM大小
        DS_buffer_esc.lpwfxFormat = &wav_format; //WAV音频格式

        CT_ContainerResource[CT_ResLength] = malloc(DS_buffer_esc.dwBufferBytes); //为资源容器分配内存
        fseek(wav_file,pcm_data_i,SEEK_SET); //WAV文件定位到PCM数据位置
        fread(CT_ContainerResource[CT_ResLength],DS_buffer_esc.dwBufferBytes,1,wav_file); //加载PCM数据

        CT_DSBuffer_List = realloc(CT_DSBuffer_List,sizeof(LPDIRECTSOUNDBUFFER8)*(CT_DS_Length+1)); //DirectSound列表重新分配内存
        CT_DirectSound->lpVtbl->CreateSoundBuffer(CT_DirectSound,&DS_buffer_esc,&CT_DSBuffer_List[CT_DS_Length],NULL); //创建DirectSound Buffer
        CT_DSBuffer_List[CT_DS_Length]->lpVtbl->Lock(CT_DSBuffer_List[CT_DS_Length],0,0,&lpvWrite,&dwLength,NULL,NULL,DSBLOCK_ENTIREBUFFER); //锁定DirectSound Buffer
        memcpy(lpvWrite,CT_ContainerResource[CT_ResLength],dwLength); //将PCM数据复制到lpvWrite
        CT_DSBuffer_List[CT_DS_Length]->lpVtbl->Unlock(CT_DSBuffer_List[CT_DS_Length],lpvWrite,dwLength,NULL,NULL); //解锁DirectSound Buffer

        fclose(wav_file); //关闭wav文件
        free(CT_ContainerResource[CT_ResLength]); //释放资源
        CT_ResourceArgList[ldata_i] = 1; //存入资源占位符
        CT_ResourceArgList[ldata_i+1] = 1; //存入资源参数
        CT_ResourceArgList[ldata_i+2] = CT_DS_Length; //存入DS列表索引
        CT_loadBarWidget(); //加载条循环
        CT_ResLength ++; //资源长度 +1
        CT_DS_Length ++; //DS列表长度 +1
        strcat(infor,"Resource load : ");
        strcat(infor,path);
        cprint(infor);
        return 1;
    }
    return 0;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 文本加载
//--------------------------------------------------------------------
int CT_ResourceLoad_Text(char *path)
{
    int utf8_size = 0; //UTF-8文本大小
    int mbcs_size = 0; //UTF-8文本转换为MBCS的大小
    char utf8_bom[3] = ""; //UTF-8 BOM
    FILE *text_file = fopen(path,"rb"); //bmp文件指针
    char *text_cache = NULL; //文本缓存
    size_t ldata_i = CT_ResLength * 5;
    char infor[257] = "";
    if(text_file != NULL) //如果当前资源文件存在
    {
        fseek(text_file,0,SEEK_SET); //文件定位0
        fread(utf8_bom,3,1,text_file); //读取3字节数据
        if(utf8_bom[0] == 0xef && utf8_bom[1] == 0xbb && utf8_bom[2] == 0xbf) //如果是BOM头
        {
            fseek(text_file,0,SEEK_END); //文件定位 结尾
            utf8_size = ftell(text_file) - 3; //获取UTF-8文本大小
            text_cache = malloc(utf8_size+1); //为文本缓存分配内存
            text_cache[utf8_size] = 0x00;
            fseek(text_file,3,SEEK_SET); //文件定位3
            fread(text_cache,utf8_size,1,text_file); //读取文本数据
        }
        else
        {
            fseek(text_file,0,SEEK_END); //文件定位 结尾
            utf8_size = ftell(text_file); //获取UTF-8文本大小
            text_cache = malloc(utf8_size+1); //为文本缓存分配内存
            text_cache[utf8_size] = 0x00;
            fseek(text_file,0,SEEK_SET); //文件定位0
            fread(text_cache,utf8_size,1,text_file); //读取文本数据
        }
        mbcs_size = CT_UTF8toMBCS_Length(text_cache); //计算文本数据从UTF-8转换为MBCS的长度
        CT_ContainerResource[CT_ResLength] = malloc(mbcs_size+1); //为资源容器中的资源分配内存
        CT_ContainerResource[CT_ResLength][mbcs_size] = 0x00;
        CT_UTF8toMBCS(CT_ContainerResource[CT_ResLength],text_cache); //将UTF-8文本转换为MBCS文本并存入资源容器
        free(text_cache); //释放文本缓存
        fclose(text_file); //关闭文本文件
        CT_ResourceArgList[ldata_i] = 1; //存入资源占位符
        CT_ResourceArgList[ldata_i+1] = 2; //存入资源参数
        CT_ResourceArgList[ldata_i+2] = mbcs_size;
        CT_loadBarWidget(); //加载条循环
        CT_ResLength ++; //资源长度 +1
        strcat(infor,"Resource load : ");
        strcat(infor,path);
        cprint(infor);
        return 1;
    }
    return 0;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 加载资源结束
//--------------------------------------------------------------------
void CT_ResourceLoadEnd()
{
    cprint("------------------------");
    if(CT_SPLASH_SHOW == 1) //如果SPLASH显示
    {
        glDeleteTextures(1,&CT_SplashImageTexID); //删除splash图像纹理
    }
    glBindTexture(GL_TEXTURE_2D,0); //取消绑定纹理
    CT_RunnerInit(); //初始化运行器
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 加载条循环
//--------------------------------------------------------------------
void CT_loadBarWidget()
{
    CT_LoadBarWidgetLoad ++; //加载条的加载数 +1
    if(CT_LoadBarWidgetLoad == 10) //如果加载条的加载数等于10
    {
        CT_LoadBarWidgetIndex ++; //加载条块索引+1
        if(CT_LoadBarWidgetIndex > 14) //如果加载条块索引大于14
        {
            CT_LoadBarWidgetIndex = 0; //加载条索引为0
        }
        CT_LoadBarShow(CT_LoadBarWidgetIndex); //显示加载条
        CT_LoadBarWidgetLoad = 0; //加载条的加载数为0
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 显示splash图片
//--------------------------------------------------------------------
void CT_SplashShow()
{
    glColor4ub(255,255,255,255); //设置颜色
    glBindTexture(GL_TEXTURE_2D,CT_SplashImageTexID); //绑定纹理
    glBegin(GL_QUADS); //开始绘制 四边形
    glTexCoord2f(0.0,0.0); //设置图像位置左下角坐标
    glVertex2f(0,CT_SIheight); //设置顶点
    glTexCoord2f(1.0,0.0); //设置图像位置右下角坐标
    glVertex2f(CT_SIwidth,CT_SIheight); //设置顶点
    glTexCoord2f(1.0,1.0); //设置图像位置右上角坐标
    glVertex2f(CT_SIwidth,0); //设置顶点
    glTexCoord2f(0.0,1.0); //设置图像位置左上角坐标
    glVertex2f(0,0); //设置顶点
    glEnd(); //结束绘制 四边形
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 加载条显示
//--------------------------------------------------------------------
void CT_LoadBarShow(int n)
{
    int bar_w = 400; //加载条宽度
    int bar_h = 30; //加载条高度
    char title[64] = "Loading..."; //加载条标题
    int widget_w = 28; //加载条块宽度
    int widget_h = 22; //加载条高度
    int bar_x = (CT_CanvasW - bar_w) / 2; //加载条位置x
    int bar_y = (CT_CanvasH - bar_h) / 2; //加载条位置y
    int title_w = istring_pxwidth(NULL,title); //加载条标题宽度
    int title_h = 20; //加载条标题高度
    int title_x = (CT_CanvasW - title_w) / 2; //加载条标题位置x
    int title_y = bar_y - 40; //加载条标题位置y
    int bar_c = color(210,193,153,255); //加载条颜色
    int widget_x = 0;
    int widget_y = 0;
    CT_OffCanvas_Clear(); //清除离屏画布
    if(CT_SPLASH_SHOW == 1) //如果SPLASH显示
    {
        CT_SplashShow(); //绘制splash图片
    }
    if(CT_LOADBAR_SHOW == 1) //如果加载条显示
    {
        CT_OffCanvas_ImgString(title_x,title_y,bar_c,title,CT_DF_Img,CT_DF_Number,CT_DF_Gap,CT_DF_CharList,CT_DF_GlyphList); //绘制加载条标题
        CT_OffCanvas_Rectangle(bar_x,bar_y,400,30,bar_c,0); //绘制加载条
        CT_OffCanvas_Rectangle(bar_x+1,bar_y+1,398,28,bar_c,0); //绘制加载条
        if(n >= 1 && n <= 14) //如果加载条块索引在指定范围内
        {
            widget_x = bar_x + 4 + (n - 1) * widget_w; //加载条块位置x
            widget_y = bar_y + 4; //加载条块位置y
            CT_OffCanvas_Rectangle(widget_x,widget_y,widget_w,widget_h,bar_c,1); //绘制加载条块
        }
    }
    CT_OffCanvas_Swap(); //交换离屏画布
}
//--------------------------------------------------------------------
void CT_LoadBarShowError(int c,char *s)
{
    int err_x = CT_CanvasW * 0.03; //错误信息位置x
    int err_y = ((CT_CanvasH - 30) / 2) + 40; //错误信息位置y
    int err_c = color(210,193,153,255); //错误信息颜色
    char err[512] = "";
    if(CT_LOADBAR_SHOW == 1) //如果加载条显示
    {
        if(c == 0) //如果错误代码是0
        {
            CT_OffCanvas_Clear(); //清除离屏画布
            strcat(err,"ERROR : The resource file is not found : ");
            strcat(err,s);
            CT_OffCanvas_ImgString(err_x,err_y,err_c,err,CT_DF_Img,CT_DF_Number,CT_DF_Gap,CT_DF_CharList,CT_DF_GlyphList); //绘制错误信息
            CT_OffCanvas_Swap(); //交换离屏画布
        }
    }
}
//--------------------------------------------------------------------