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
int **CT_ResourceArgList = NULL; //资源参数列表
LPDIRECTSOUNDBUFFER8 *CT_DSBuffer_List; //DirectSound Buffer列表
int CT_ResIndex = 0; //资源容器索引
int CT_DS_Index = 0; //DirectSound Buffer列表索引
int CT_LoadBarWidgetLoad = 0; //加载条块加载数
int CT_LoadBarWidgetIndex = 0; //加载条块索引
char *CT_SplashImage = NULL; //Splash图像
int CT_SIwidth = 0; //Splash图像宽度
int CT_SIheight = 0; //Splash图像高度
int CT_SplashImageTexID = 0; //Splash图像纹理id
//--------------------------------------------------------------------
// 开始加载资源
//--------------------------------------------------------------------
void CT_ResourceLoadStart(int rl_n)
{
    cprint("-----Load resource-----");
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
            CT_RunnerWait(16667); //等待一帧
            CT_ResourceLoad(rl_n); //加载资源
            return; //中断
        }
        CT_LoadBarShowError(0,"splash.xxx"); //显示错误信息
    }
    else
    {
        CT_LoadBarShow(0); //显示加载条
        CT_ResourceLoad(rl_n); //加载资源
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 加载资源
//--------------------------------------------------------------------
void CT_ResourceLoad(int rl_n)
{
    int res_faild = 0; //加载失败
    int i;
    CT_ContainerResource = malloc(sizeof(char*)); //为资源容器分配内存
    CT_ResourceArgList = malloc(sizeof(int*)); //为为资源列表分配内存
    for(i=0;i<rl_n;i++) //遍历资源列表
    {
        if(CT_ResourceList[i][0] != NULL) //当前文件路径不为空
        {
            char res_name[128] = ""; //资源文件名
            strslice(res_name,CT_ResourceList[i],charindex_last(CT_ResourceList[i],'/')+1,strlen(CT_ResourceList[i])); //获取当前资源文件名
            if(charindex(res_name,'.') != -1) //如果当前资源文件名中有"."
            {
                if(charindex(res_name,'.') != 0) //如果当前资源文件名中"."的位置不为0
                {
                    char res_extname[16] = ""; //资源文件扩展名
                    int extname_pos = charindex(res_name,'.') + 1; //获取当前资源文件扩展名位置
                    strslice(res_extname,res_name,extname_pos,strlen(res_name)); //获取当前资源文件扩展名
                    if(strcmp(res_extname,"png") == 0 || strcmp(res_extname,"jpg") == 0 || strcmp(res_extname,"bmp") == 0 || strcmp(res_extname,"mp3") == 0 || strcmp(res_extname,"ogg") == 0 || strcmp(res_extname,"wav") == 0) //如果当前资源文件类型是支持的
                    {
                        if(strcmp(res_extname,"png") == 0) //如果当前资源是png
                        {

                        }
                        if(strcmp(res_extname,"jpg") == 0) //如果当前资源是jpg
                        {

                        }
                        if(strcmp(res_extname,"bmp") == 0) //如果当前资源是bmp
                        {
                            int res_status = CT_ResourceLoad_BMP(CT_ResourceList[i]); //加载BMP图像
                            if(res_status == 0) //如果资源加载失败
                            {
                                res_faild = 1;
                                CT_LoadBarShowError(0,CT_ResourceList[i]); //显示错误信息
                                break; //中断
                            }
                            
                        }
                        if(strcmp(res_extname,"mp3") == 0) //如果当前资源是mp3
                        {
                            int res_status = CT_ResourceLoad_MP3(CT_ResourceList[i]);
                            if(res_status == 0) //如果资源加载失败
                            {
                                res_faild = 1;
                                CT_LoadBarShowError(0,CT_ResourceList[i]); //显示错误信息
                                break; //中断
                            }
                        }
                        if(strcmp(res_extname,"ogg") == 0) //如果当前资源是ogg
                        {

                        }
                        if(strcmp(res_extname,"wav") == 0) //如果当前资源是wav
                        {
                            int res_status = CT_ResourceLoad_WAV(CT_ResourceList[i]);
                            if(res_status == 0) //如果资源加载失败
                            {
                                res_faild = 1;
                                CT_LoadBarShowError(0,CT_ResourceList[i]); //显示错误信息
                                break; //中断
                            }
                        }
                    }
                    else
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
                else
                {
                    char infor[1024] = "";
                    strcat(infor,"ERROR 2 : The resource file name is error : ");
                    strcat(infor,CT_ResourceList[i]);
                    cprint(infor);
                    res_faild = 1;
                    break;
                }
            }
            else
            {
                char infor[1024] = "";
                strcat(infor,"ERROR 1 : The resource file name is not support : ");
                strcat(infor,CT_ResourceList[i]);
                cprint(infor);
                res_faild = 1;
                break;
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
// 加载BMP图像
//--------------------------------------------------------------------
int CT_ResourceLoad_BMP(char *path)
{
    int img_w = 0; //图像宽度
    int img_h = 0; //图像高度
    int tex_w = 0; //纹理宽度
    int tex_h = 0; //纹理高度
    int tex_id = 0; //纹理id
    char infor[1024] = "";
    FILE *bmp_file = fopen(path,"rb"); //BMP文件指针
    if(bmp_file != NULL) //如果BMP文件指针不为空
    {
        BMPRGBA_BmpInf res_bmp; //BMP资源信息
        BMPRGBA_BmpGetInf(&res_bmp,bmp_file); //获取BMP图像信息
        img_w = res_bmp.BmpWidth; //获取BMP图像宽度
        img_h = res_bmp.BmpHeight; //获取BMP图像高度
        CT_ContainerResource = realloc(CT_ContainerResource,sizeof(char*)*(CT_ResIndex+1)); //资源容器重新分配内存
        CT_ContainerResource[CT_ResIndex] = malloc(img_w * img_h * 4); //为资源容器中的资源分配内存
        BMPRGBA_BmpToRGBA(CT_ContainerResource[CT_ResIndex],&res_bmp,bmp_file); //加载bmp图像到资源容器
        fclose(bmp_file); //关闭bmp文件
        glGenTextures(1,&tex_id); //生成纹理
        glBindTexture(GL_TEXTURE_2D,tex_id); //绑定纹理
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //设置纹理过滤
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); //设置纹理过滤
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP); //设置纹理环绕
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP); //设置纹理环绕
        glTexImage2D(GL_TEXTURE_2D,0,4,img_w,img_h,0,GL_RGBA,GL_UNSIGNED_BYTE,CT_ContainerResource[CT_ResIndex]); //加载纹理
        free(CT_ContainerResource[CT_ResIndex]); //释放资源
        CT_ResourceArgList = realloc(CT_ResourceArgList,sizeof(int*)*(CT_ResIndex+1)); //资源列表重新分配内存
        CT_ResourceArgList[CT_ResIndex] = malloc(20); //为资源列表中的项分配内存
        CT_ResourceArgList[CT_ResIndex][0] = 1; //存入资源占位符
        CT_ResourceArgList[CT_ResIndex][1] = 0; //存入资源参数
        CT_ResourceArgList[CT_ResIndex][2] = img_w; //存入图像宽度
        CT_ResourceArgList[CT_ResIndex][3] = img_h; //存入图像高度
        CT_ResourceArgList[CT_ResIndex][4] = tex_id; //存入纹理id
        CT_loadBarWidget(); //加载条循环
        CT_ResIndex ++; //资源索引 +1
        strcat(infor,"Resource load : ");
        strcat(infor,path);
        cprint(infor);
        return 1;
    }
    return 0;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 加载MP3音频
//--------------------------------------------------------------------
int CT_ResourceLoad_MP3(char *path)
{
    return 0;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 加载WAV音频
//--------------------------------------------------------------------
int CT_ResourceLoad_WAV(char *path)
{
    char infor[1024] = "";
    char wav_head_data[128] = ""; //WAV头数据
    char str5[5] = ""; //字节数组5
    int pcm_channel = 0; //PCM声道
    int pcm_sample = 0; //PCM采样率
    int pcm_rate = 0; //PCM传输速率
    int pcm_data_i = 0; //WAV头数据data标志的位置
    int pcm_size = 0; //PCM大小
    int pcm_kbps = 0; //PCM比特率
    int i;
    DSBUFFERDESC DS_buffer_esc; //DS BUFFER ESC
    WAVEFORMATEX wav_format; //WAV音频格式
    char *lpvWrite; //LPV指针
    int dwLength = 0; //DW长度

    FILE *wav_file = fopen(path,"rb"); //WAV文件指针
    if(wav_file != NULL) //如果当前资源文件存在
    {
        fseek(wav_file,0,SEEK_SET); //WAV文件定位0
        fread(wav_head_data,128,1,wav_file); //读取文件头数据 - 128字节
        pcm_channel = wav_head_data[22] & 0x000000ff; //获取PCM声道
        pcm_sample = ((wav_head_data[27] & 0x000000ff) * 0x1000000) + ((wav_head_data[26] & 0x000000ff) * 0x10000) + ((wav_head_data[25] & 0x000000ff) * 0x100) + (wav_head_data[24] & 0x000000ff); //获取PCM采样率
        pcm_rate = ((wav_head_data[31] & 0x000000ff) * 0x1000000) + ((wav_head_data[30] & 0x000000ff) * 0x10000) + ((wav_head_data[29] & 0x000000ff) * 0x100) + (wav_head_data[28] & 0x000000ff); //获取PCM传输速率
        pcm_kbps = pcm_rate * 8 / 1000; //PCM比特率
        for(i=32;i<124;i++) //遍历PCM头数据
        {
            strslice(str5,wav_head_data,i,i+4); //截取4字节数据
            if(strcmp(str5,"data") == 0) //如果找到data标志
            {
                pcm_data_i = i + 8; //PCM数据位置
                break; //中断
            }
        }
        pcm_size = ((wav_head_data[pcm_data_i-1] & 0x000000ff) * 0x1000000) + ((wav_head_data[pcm_data_i-2] & 0x000000ff) * 0x10000) + ((wav_head_data[pcm_data_i-3] & 0x000000ff) * 0x100) + (wav_head_data[pcm_data_i-4] & 0x000000ff); //获取PCM大小
        CT_ContainerResource = realloc(CT_ContainerResource,sizeof(char*)*(CT_ResIndex+1)); //资源列表重新分配内存
        CT_ContainerResource[CT_ResIndex] = malloc(pcm_size); //为资源容器分配内存
        fseek(wav_file,pcm_data_i,SEEK_SET); //WAV文件定位到PCM数据位置
        fread(CT_ContainerResource[CT_ResIndex],pcm_size,1,wav_file); //加载PCM数据

        memset(&wav_format, 0, sizeof(WAVEFORMATEX));
        wav_format.wFormatTag = WAVE_FORMAT_PCM;  //WAV音频格式PCM
        wav_format.nChannels = pcm_channel; //声道
        wav_format.nSamplesPerSec = pcm_sample; //采样率
        wav_format.nBlockAlign = 4; 
        wav_format.nAvgBytesPerSec = pcm_rate; //传输速率
        wav_format.wBitsPerSample = 16;

        memset(&DS_buffer_esc, 0, sizeof(DSBUFFERDESC));
        DS_buffer_esc.dwSize = sizeof(DSBUFFERDESC);
        DS_buffer_esc.dwFlags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY | DSBCAPS_GLOBALFOCUS; //DirectSound旗
        DS_buffer_esc.dwBufferBytes = pcm_size; //PCM大小
        DS_buffer_esc.lpwfxFormat = &wav_format; //WAV音频格式

        CT_DSBuffer_List = realloc(CT_DSBuffer_List,sizeof(LPDIRECTSOUNDBUFFER8)*(CT_DS_Index+1)); //DirectSound列表重新分配内存
        CT_DirectSound->lpVtbl->CreateSoundBuffer(CT_DirectSound,&DS_buffer_esc,&CT_DSBuffer_List[CT_DS_Index],NULL); //创建DirectSound Buffer
        CT_DSBuffer_List[CT_DS_Index]->lpVtbl->Lock(CT_DSBuffer_List[CT_DS_Index],0,0,&lpvWrite,&dwLength,NULL,NULL,DSBLOCK_ENTIREBUFFER); //锁定DirectSound Buffer
        memcpy(lpvWrite,CT_ContainerResource[CT_ResIndex],dwLength); //将PCM数据复制到lpvWrite
        CT_DSBuffer_List[CT_DS_Index]->lpVtbl->Unlock(CT_DSBuffer_List[CT_DS_Index],lpvWrite,dwLength,NULL,NULL); //解锁DirectSound Buffer

        fclose(wav_file); //关闭bmp文件
        free(CT_ContainerResource[CT_ResIndex]); //释放资源
        CT_ResourceArgList = realloc(CT_ResourceArgList,sizeof(int*)*(CT_ResIndex+1)); //资源列表重新分配内存
        CT_ResourceArgList[CT_ResIndex] = malloc(20); //为资源列表中的项分配内存
        CT_ResourceArgList[CT_ResIndex][0] = 1; //存入资源占位符
        CT_ResourceArgList[CT_ResIndex][1] = 1; //存入资源参数
        CT_ResourceArgList[CT_ResIndex][2] = CT_DS_Index; //存入DS列表索引
        CT_loadBarWidget(); //加载条循环
        CT_ResIndex ++; //资源索引 +1
        CT_DS_Index ++; //DS列表索引 +1
        strcat(infor,"Resource load : ");
        strcat(infor,path);
        cprint(infor);
        return 1;
    }
    return 0;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 加载文本
//--------------------------------------------------------------------
int CT_ResourceLoad_Text(char *path)
{
    int utf8_size = 0; //UTF-8文本大小
    int mbcs_size = 0; //UTF-8文本转换为MBCS的大小
    char utf8_bom[3] = ""; //UTF-8 BOM
    FILE *text_file = fopen(path,"rb"); //bmp文件指针
    char *text_cache = NULL; //文本缓存
    char infor[1024] = "";
    if(text_file != NULL) //如果当前资源文件存在
    {
        fseek(text_file,0,SEEK_SET); //文件定位0
        fread(utf8_bom,3,1,text_file); //读取3字节数据
        if(utf8_bom[0] == -17 && utf8_bom[1] == -69 && utf8_bom[2] == -65) //如果是BOM头
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
        CT_ContainerResource = realloc(CT_ContainerResource,sizeof(char*)*(CT_ResIndex+1)); //资源容器重新分配内存
        CT_ContainerResource[CT_ResIndex] = malloc(mbcs_size+1); //为资源容器中的资源分配内存
        CT_ContainerResource[CT_ResIndex][mbcs_size] = 0x00;
        CT_UTF8toMBCS(CT_ContainerResource[CT_ResIndex],text_cache); //将UTF-8文本转换为MBCS文本并存入资源容器
        free(text_cache); //释放文本缓存
        fclose(text_file); //关闭文本文件
        CT_ResourceArgList = realloc(CT_ResourceArgList,sizeof(int*)*(CT_ResIndex+1));
        CT_ResourceArgList[CT_ResIndex] = malloc(20);
        CT_ResourceArgList[CT_ResIndex][0] = 1; //存入资源占位符
        CT_ResourceArgList[CT_ResIndex][1] = 2; //存入资源参数
        CT_loadBarWidget(); //加载条循环
        CT_ResIndex ++; //资源索引 +1
        strcat(infor,"Resource load : ");
        strcat(infor,path);
        cprint(infor);
        return 1;
    }
    return 0;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 结束加载资源
//--------------------------------------------------------------------
void CT_ResourceLoadEnd()
{
    cprint("------------------------");
    if(CT_SPLASH_SHOW == 1) //如果SPLASH显示
    {
        glDeleteTextures(1,&CT_SplashImageTexID);
    }
    CT_RunnerWait(16667); //等待一帧
    CTCanvas_Clear(); //清除画布
    CTCanvas_Out(); //输出缓冲画布
    glBindTexture(GL_TEXTURE_2D,0); //取消绑定纹理
    wglMakeCurrent(NULL,NULL); //取消选择缓冲画布环境
    CreateThread(NULL,NULL,CT_RunnerStart,NULL,NULL,NULL); //创建运行器线程
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 加载条循环
//--------------------------------------------------------------------
void CT_loadBarWidget()
{
    CT_LoadBarWidgetLoad ++; //加载条块加载数 +1
    if(CT_LoadBarWidgetLoad == 10) //如果加载条块加载数等于10
    {
        CT_LoadBarWidgetIndex ++; //加载条块索引+1
        if(CT_LoadBarWidgetIndex > 14) //如果加载条块索引大于14
        {
            CT_LoadBarWidgetIndex = 0; //加载条块索引
        }
        CT_LoadBarShow(CT_LoadBarWidgetIndex); //显示加载条
        CT_LoadBarWidgetLoad = 0; //加载条块加载数
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
    glTexCoord2d(0.0,0.0); //设置图像位置左下角坐标
    glVertex2f(0,CT_SIheight); //设置顶点
    glTexCoord2d(1.0,0.0); //设置图像位置右下角坐标
    glVertex2f(CT_SIwidth,CT_SIheight); //设置顶点
    glTexCoord2d(1.0,1.0); //设置图像位置右上角坐标
    glVertex2f(CT_SIwidth,0); //设置顶点
    glTexCoord2d(0.0,1.0); //设置图像位置左上角坐标
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
    int lump_w = 28; //加载条块宽度
    int lump_h = 22; //加载条高度
    int bar_x = (CT_CanvasW - bar_w) / 2; //加载条位置x
    int bar_y = (CT_CanvasH - bar_h) / 2; //加载条位置y
    int title_w = istring_pxwidth(NULL,title); //加载条标题宽度
    int title_h = 20; //加载条标题高度
    int title_x = (CT_CanvasW - title_w) / 2; //加载条标题位置x
    int title_y = bar_y - 40; //加载条标题位置y
    int bar_c = color(210,193,153,255); //加载条颜色
    int lump_x = 0;
    int lump_y = 0;
    CTCanvas_Clear(); //清除画布
    if(CT_SPLASH_SHOW == 1) //如果SPLASH显示
    {
        CT_SplashShow(); //绘制splash图片
    }
    if(CT_LOADBAR_SHOW == 1) //如果加载条显示
    {
        CT_BufferImgString(title_x,title_y,bar_c,title,CT_DF_Img,CT_DF_Number,CT_DF_Gap,CT_DF_CharList,CT_DF_GlyphList); //绘制加载条标题
        CTCanvas_Rect(bar_x,bar_y,400,30,bar_c,0); //绘制加载条
        CTCanvas_Rect(bar_x+1,bar_y+1,398,28,bar_c,0); //绘制加载条
        if(n >= 1 && n <= 14) //如果加载条块索引在指定范围内
        {
            lump_x = bar_x + 4 + (n - 1) * lump_w; //加载条块位置x
            lump_y = bar_y + 4; //加载条块位置y
            CTCanvas_Rect(lump_x,lump_y,lump_w,lump_h,bar_c,1); //绘制加载条块
        }
    }
    CTCanvas_Out(); //输出缓冲画布
}
//--------------------------------------------------------------------
void CT_LoadBarShowError(int c,char *s)
{
    int err_x = CT_CanvasW * 0.2; //错误信息位置x
    int err_y = ((CT_CanvasH - 30) / 2) + 40; //错误信息位置y
    int err_c = color(210,193,153,255); //错误信息颜色
    char err[512] = "";
    if(CT_LOADBAR_SHOW == 1) //如果加载条显示
    {
        if(c == 0) //如果错误代码是0
        {
            CTCanvas_Clear(); //清除画布
            strcat(err,"ERROR 0 : The resource file is not found : ");
            strcat(err,s);
            CT_BufferImgString(err_x,err_y,err_c,err,CT_DF_Img,CT_DF_Number,CT_DF_Gap,CT_DF_CharList,CT_DF_GlyphList); //绘制错误信息
            CTCanvas_Out(); //输出缓冲画布
        }
    }
}
//--------------------------------------------------------------------