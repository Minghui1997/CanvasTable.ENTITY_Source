#include "CanvasTableSYS.h"
#include "../DirectSound8/dsound.h"
#pragma comment(lib,"DirectSound8/dsound.lib")

extern LPDIRECTSOUNDBUFFER8 *CT_DSBuffer_List;
extern unsigned int *CT_ResourceArgList;

//--------------------------------------------------------------------
// 音频播放
//--------------------------------------------------------------------
void audio_play(int a,int l)
{
    if(a != -1) //如果资源不为空
    {
        size_t ldata_i = a * 5; //资源参数数据索引
        int res_type = CT_ResourceArgList[ldata_i+1]; //资源类型
        if(res_type == 1) //如果资源类型是音频
        {
            int play_mode = DSBPLAY_LOCSOFTWARE; //播放模式
            int ds_index = CT_ResourceArgList[ldata_i+2]; //获取DirectSound列表索引
            if(l == 1) //如果循环播放
            {
                play_mode += DSBPLAY_LOOPING; //播放模式为循环播放
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
// 停止音频播放
//--------------------------------------------------------------------
void audio_stop(int a)
{
    if(a != -1) //如果资源不为空
    {
        size_t ldata_i = a * 5; //资源参数数据索引
        int res_type = CT_ResourceArgList[ldata_i+1]; //资源类型
        if(res_type == 1) //如果资源类型是音频
        {
            int ds_index = CT_ResourceArgList[ldata_i+2]; //获取DirectSound列表索引
            CT_DSBuffer_List[ds_index]->lpVtbl->Stop(CT_DSBuffer_List[ds_index]); //停止播放音频
        }
        else
        {
            cprint("ERROR : The resource index corresponding to the resource file is not a audio.");
        }
    }
}
//--------------------------------------------------------------------