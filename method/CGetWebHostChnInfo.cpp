#include "CGetWebHostChnInfo.h"
#include <boost/lexical_cast.hpp>

CGetWebHostChnInfo::CGetWebHostChnInfo(string ip, int port, int timeOut)
    :_result(cJSON_CreateObject())
    ,_channal(cJSON_CreateObject())
    ,_addressList(cJSON_CreateArray())
    ,_ip(ip)
    ,_port(port)
    ,_timeOut(timeOut)
    ,_client(new MPSClient(ip,port,timeOut))
    ,_AnalyParas()
{

}

bool CGetWebHostChnInfo::getWebChnConfig(int ChnNum,CData &oResult, char *cResult)
{
    mLogDebug("run getChnConfig(...)");
    //获取状态

    //add fwb 20201104
    //int  i = 0;
    //int  itmplen   = 0;
    //char cChnTmpbuf[512] = {0};
    char cChnCmd[4096]   = {0};
	int VidCapNum;
	char *ReqBody = NULL;
	//ChnNum大于12，返回所有（12组）通道信息
//	if(ChnNum > 12)
//	{
//		if(AudioBoardType == "new_audio_board")
//		{
//			sprintf(cChnCmd,"eth1.conf\nnet_send%d.app_info\ncomp1.osd1.content\n", ChnNum);
//			for(int i = 1; i <= 3; i++)
//			{
//				ReqBody = cChnCmd + strlen(cChnCmd);
//				sprintf(ReqBody,"yuan%d.info\n", i);
//			}
//		}
//		else
//		{	
//			sprintf(cChnCmd,"eth1.conf\nnet_send%d.app_info\ncomp1.osd1.content\nnet_recv100.app_info\nh323h100.call\nsip100.call\n", ChnNum);
//		}
//		
//		for(int i = 1; i <= 8; i++)
//		{
//			ReqBody = cChnCmd + strlen(cChnCmd);
//			sprintf(ReqBody,"vid_cap%d.ch1.osd1.content\n", i);
//		}
//		for(int i = 1; i <= 12; i++)
//		{
//			ReqBody = cChnCmd + strlen(cChnCmd);
//			sprintf(ReqBody,"net_send%d.rec_current_status\nnet_send%d.rec_task_id\n", i, i);
//		}
//
//	}
	if(ChnNum > 12)
	{
		
		if(AudioBoardType == "new_audio_board")
		{
			sprintf(cChnCmd,"eth1.conf\nnet_send100.app_info\ncomp1.osd1.content\nyuan100.info\nvid_cap100.ch1.osd1.content\n");
//			for(int i = 1; i <= 3; i++)
//			{
//				ReqBody = cChnCmd + strlen(cChnCmd);
//				sprintf(ReqBody,"yuan%d.info\n", i);
//			}
		}
		else
		{	
//			sprintf(cChnCmd,"eth1.conf\nnet_send%d.app_info\ncomp1.osd1.content\nnet_recv100.app_info\nh323h100.call\nsip100.call\n", ChnNum);
			sprintf(cChnCmd,"eth1.conf\nnet_send100.app_info\ncomp1.osd1.content\nnet_recv100.app_info\nh323h100.call\nsip100.call\nvid_cap100.ch1.osd1.content\n");
		}
		
//		for(int i = 1; i <= 8; i++)
//		{
//			ReqBody = cChnCmd + strlen(cChnCmd);
//			sprintf(ReqBody,"vid_cap%d.ch1.osd1.content\n", i);
//		}
//		for(int i = 1; i <= 12; i++)
//		{
//			ReqBody = cChnCmd + strlen(cChnCmd);
//			sprintf(ReqBody,"net_send%d.rec_current_status\nnet_send%d.rec_task_id\n", i, i);
//		}

	}
#if 0	
	else if(ChnNum <= 8)//ChnNum小于等于8，返回对应的通道信息
	{
		VidCapNum = ChnNum;
		sprintf(cChnCmd,INTERFACE_GETWEBHOSTCHNINFO_SENDCMD, ChnNum, VidCapNum);
		ReqBody = cChnCmd + strlen(cChnCmd);
		sprintf(ReqBody,"eth1.conf\nnet_send%d.rec_current_status\nnet_send%d.rec_task_id\n", ChnNum, ChnNum);
	}//ChnNum介于9-13，返回对应的通道信息，无vid_cap网管协议，vid_cap只有1-8通道
	else if((ChnNum > 8)&&(ChnNum <= 12))
	{
		sprintf(cChnCmd,INTERFACE_GETWEBHOSTCHNINFO_WITHOUT_OSD_SENDCMD, ChnNum, 1);
		ReqBody = cChnCmd + strlen(cChnCmd);
		sprintf(ReqBody,"net_send%d.rec_current_status\nnet_send%d.rec_task_id\n", ChnNum, ChnNum);
	}
#else	
	else if(ChnNum == 1)//合成
	{
		sprintf(cChnCmd,"eth1.conf\nnet_send%d.app_info\nnet_send%d.rec_current_status\nnet_send%d.rec_task_id\ncomp%d.osd1.content\n", ChnNum, ChnNum, ChnNum,ChnNum);
	}
	else if((ChnNum >= 2)&&(ChnNum <= 9))//ChnNum小于等于8，返回对应的通道信息
	{
		sprintf(cChnCmd,"eth1.conf\nnet_send%d.app_info\nvid_cap%d.ch1.osd1.content\nnet_send%d.rec_current_status\nnet_send%d.rec_task_id\n", ChnNum, ChnNum-1, ChnNum, ChnNum);
	}
	else if((ChnNum >= 10)&&(ChnNum <= 12))//ChnNum介于11-13，返回对应的通道信息，无字幕
	{
		sprintf(cChnCmd,"eth1.conf\nnet_send%d.app_info\nnet_send%d.rec_current_status\nnet_send%d.rec_task_id\n", ChnNum, ChnNum, ChnNum);
	}
#endif 

    //add fwb 20201027
    char cCmdSend[1024] = {0};
    char cHeadCmd[4] = {0};
    //char *cTmpCmd = INTERFACE_GETHOSTCONFIG_SENDCMD;
    int  cCmdLength = 0;
    memset(cCmdSend, 0, sizeof(1024)); 

    cCmdLength = strlen(cChnCmd) + 4;
    cHeadCmd[0] = 0; //当Buf[0]为0x00时，表示网管协议不加密,当Buf[0]为0x01时，表示网管协议加密
    cHeadCmd[1] = 2; //获取参数指令（GET）
    cHeadCmd[2] = strlen(cChnCmd) >> 8;  
    cHeadCmd[3] = strlen(cChnCmd) & 0xff;

    memcpy(cCmdSend, cHeadCmd, 4);
    memcpy(cCmdSend+4, cChnCmd, strlen(cChnCmd));

    MPSOperationRes opResCode = eMPSResultOK;
    ResponseCode resCode = _client->GetConfigNew(cCmdSend,cCmdLength,opResCode,oResult,cResult);
    if(resCode != eResponseCodeSuccess){
        mLogError("GetConfig(...) error:"<<resCode);
        return false;
    }

    return true;
}

bool CGetWebHostChnInfo::ComposeResult(cJSON *result)
{
    if(_result == NULL || result == NULL){
        mLogError("cJSON result == NULL.");
        result = NULL;
        return false;
    }

    //result中添加节点
    cJSON_AddItemToObject(_result,GET_HOST_CONFIG_RES_WEB_CHN,_addressList);
    return true;
}

string CGetWebHostChnInfo::ComposeResult()
{
    cJSON * result = cJSON_CreateObject();
    if(ComposeResult(result) == false){
        cJSON_Delete(result);
        return "";
    }else{
        char *cRes = cJSON_Print(_result);
        string res = cRes;
        cJSON_Delete(result);
        return res;
    }
}

bool CGetWebHostChnInfo::GetWebAudioBoardTypeInner(CData& oResult, char* cResult)
{
	mLogDebug("run GetWebAudioBoardTypeInner(...)");
	string	resolution,encodeMode,coderateType;

	char szReqBody[REQ_BODY_MAXLEN] = { 0 };
	sprintf(szReqBody,"sys.audio_type\n");

	int realBodySize = strlen(szReqBody);

	char szReqCmd[REQ_CMD_MAXLEN] = { 0 };
	szReqCmd[0] = 0;
	szReqCmd[1] = 2;
	szReqCmd[2] = realBodySize >> 8;
	szReqCmd[3] = realBodySize & 0xff;

	memcpy(&szReqCmd[4], szReqBody, realBodySize);
#if 1
	MPSOperationRes opResCode = eMPSResultOK;
	ResponseCode resCode = _client->GetConfigNew(szReqCmd, 4 + realBodySize, opResCode, oResult, cResult);
	if (resCode != eResponseCodeSuccess)
	{
		mLogError("GetWebAudioBoardTypeInner Run Failed !!!");
		return false;
	}
#endif

	return true;


}

InterfaceResCode CGetWebHostChnInfo::GetWebHostChnInfo(string &sResult,int ChnNum)
{
    //TODO
    mLogInfo("GetWebHostChnInfo...");
    CData oResult = SVSMAP();
    char cResult[8192] = {0};
	cJSON* temList[15];
	
	string IP;
	string Method = "GetWebHostChnInfo";
    //printf("GetWebHostChnInfo ChnNum %d\n", ChnNum);
#if 1
	if (!GetWebAudioBoardTypeInner( oResult, cResult))
	{
		mLogError("GetWebHostChnInfo GetWebAudioBoardTypeInner failed !!!");
		return eInterfaceResCodeError;
	}
	if (!_AnalyParas.ConvertSTRING2CJSONGetWebParam(oResult, cResult, _result, _addressList, NULL, Method, IP, 0))
	{
		mLogError("Failed to run  ConvertSTRING2CJSONGetWebParam(...)");
		return eInterfaceResCodeError;
	}
	cJSON *BoardType = cJSON_GetObjectItem(_result,"AudioBoardType");
//	AudioBoardType = BoardType->valueint;
	AudioBoardType = BoardType->valuestring;
	mLogInfo("AudioBoardType: "<<AudioBoardType);

    if(getWebChnConfig(ChnNum, oResult, cResult) == false){
        mLogError("Failed to run  getMPSConfig(...)");
        return eInterfaceResCodeError;
    }
#endif 
    if(ChnNum > 12)
    {
   		for(int i = 1; i <= 13; i++)
		{//用于生成节点，存储解析后的参数
			temList[i] = cJSON_CreateObject();
		}
    }else{
		temList[ChnNum] = cJSON_CreateObject();
	}
#if 0
    int iRecvLength = (cResult[2] << 8) + cResult[3] + 4;
    printf("getMPSConfig\n");
    for (int i = 0; i < iRecvLength; i++)
    {
	    if (i < 4)
	    {
		    printf("%d", cResult[i]);
	    }
	    else 
	    {
		    printf("%c", cResult[i]);
	    }
    }
#endif 

    if( _AnalyParas.ConvertSTRING2CJSONGetWebHostChnInfo(oResult, cResult, _result, temList, _addressList, Method, ChnNum, AudioBoardType) == false){
        mLogError("Failed to run  ConvertSTRING2CJSONGetWebHostChnInfo(...)");
        return eInterfaceResCodeError;
    }


    sResult = ComposeResult();
	cJSON_Delete(_result);
    return eInterfaceResCodeSuccess;
}


