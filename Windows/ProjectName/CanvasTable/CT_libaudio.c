#include "CanvasTableSYS.h"
#include "../DirectSound8/dsound.h"
#pragma comment(lib,"DirectSound8/dsound.lib")

extern LPDIRECTSOUNDBUFFER8 *CT_DSBuffer_List;
extern int **CT_ResourceArgList;

//--------------------------------------------------------------------
// 播放音频
//--------------------------------------------------------------------
void audio_play(int a,int l)
{
    if(a != -1) //如果资源不为空
    {
        int res_type = CT_ResourceArgList[a][1]; //资源类型
        if(res_type == 1) //如果资源类型是音频
        {
            int play_mode = 0; //播放模式
            int ds_index = CT_ResourceArgList[a][2]; //获取DirectSound列表索引
            if(l == 1) //如果循环播放
            {
                play_mode = DSBPLAY_LOOPING; //播放模式为循环播放
            }
            CT_DSBuffer_List[ds_index]->lpVtbl->SetCurrentPosition(CT_DSBuffer_List[ds_index],0); //设置音频位置
            CT_DSBuffer_List[ds_index]->lpVtbl->Play(CT_DSBuffer_List[ds_index],0,0,play_mode); //播放音频
        }
        else
        {
            cprint("ERROR : The resource index corresponding to the resource file is not a audio.");
        }
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// 停止播放音频
//--------------------------------------------------------------------
void audio_stop(int a)
{
    if(a != -1) //如果资源不为空
    {
        int res_type = CT_ResourceArgList[a][1]; //资源类型
        if(res_type == 1) //如果资源类型是音频
        {
            int ds_index = CT_ResourceArgList[a][2]; //获取DirectSound列表索引
            CT_DSBuffer_List[ds_index]->lpVtbl->Stop(CT_DSBuffer_List[ds_index]); //停止播放音频
        }
        else
        {
            cprint("ERROR : The resource index corresponding to the resource file is not a audio.");
        }
    }
}
//--------------------------------------------------------------------