#include "CSetWebEffectCtrl.h"
#include <boost/lexical_cast.hpp>

CSetWebEffectCtrl::CSetWebEffectCtrl(string ip, int port, int timeOut)
    :_result(cJSON_CreateObject())
    ,_VideoFiles(cJSON_CreateArray())
    ,_ip(ip)
    ,_port(port)
    ,_timeOut(timeOut)
    ,_client(new MPSClient(ip,port,timeOut))
    ,_AnalyParas()
{

}

bool CSetWebEffectCtrl::SetWebEffectNew( int OverlayDisplay, int Chn, int Post, int Num, int Type, string Content, int Color, int FontSize, int SelectType, int TimeORText, CData &oResult, char *cResult)
{
    mLogDebug("run SetRecStart(...)");
    //获取状态

    //add fwb 20201110
    char cChnCmd[4096] = {0};
	char *ReqBody;
	string Enctype, osd_x, osd_y, osd_w, osd_h;
	switch(Type)
	{
		case 1:
			Enctype = "UTF-8";
			break;
		case 2:
			Enctype = "GB2312";
			break;
		default:
		    mLogError("SetWebEffectNew  Enctype error:"<<Type);
            return false;
	}
	
//	if(FontSize > 32) FontSize = 32;
//	if(FontSize < 16) FontSize = 16;
	
	if(SelectType == 0)
	{
//		sprintf(cChnCmd,"vid_cap%d.ch1.osd_on=%d\nvid_cap%d.ch1.osd%d.type=%d\nvid_cap%d.ch1.osd%d.char_set=%s\nvid_cap%d.ch1.osd%d.s_display=%d\nvid_cap%d.ch1.osd%d.content=%s\nvid_cap%d.ch1.osd%d.color=%d\nvid_cap%d.ch1.osd%d.size=%d\nvid_cap%d.ch1.osd%d.position=%d\n", \
//			Chn, 1, Chn, Num, TimeORText, Chn, Num, Enctype.c_str(),Chn, Num, OverlayDisplay, Chn, Num, Content.c_str(), Chn, Num, Color, Chn, Num, FontSize, Chn, Num, Post);
		sprintf(cChnCmd,"vid_cap%d.ch1.osd_on=%d\nvid_cap%d.ch1.osd%d.s_display=%d\nvid_cap%d.ch1.osd%d.type=%d\nvid_cap%d.ch1.osd%d.char_set=%s\nvid_cap%d.ch1.osd%d.content=%s\nvid_cap%d.ch1.osd%d.color=%d\nvid_cap%d.ch1.osd%d.size=%d\nvid_cap%d.ch1.osd%d.position=%d\n", \
			Chn, 1, Chn, Num, OverlayDisplay, Chn, Num, TimeORText, Chn, Num, Enctype.c_str(), Chn, Num, Content.c_str(), Chn, Num, Color, Chn, Num, FontSize, Chn, Num, Post);

	}
	else if(SelectType == 1)
	{
//		sprintf(cChnCmd,"comp%d.osd_on=%d\ncomp%d.osd%d.type=%d\ncomp%d.osd%d.s_display=%d\ncomp%d.osd%d.char_set=%s\ncomp%d.osd%d.content=%s\ncomp%d.osd%d.color=%d\ncomp%d.osd%d.size=%d\ncomp%d.osd%d.position=%d\n", \
//		Chn, 1, Chn, Num, TimeORText, Chn, Num, OverlayDisplay, Chn, Num, Enctype.c_str(), Chn, Num, Content.c_str(), Chn, Num, Color, Chn, Num, FontSize,Chn, Num, Post);
		sprintf(cChnCmd,"comp%d.osd_on=%d\ncomp%d.osd%d.s_display=%d\ncomp%d.osd%d.type=%d\ncomp%d.osd%d.char_set=%s\ncomp%d.osd%d.content=%s\ncomp%d.osd%d.color=%d\ncomp%d.osd%d.size=%d\ncomp%d.osd%d.position=%d\n", \
		Chn, 1, Chn, Num, OverlayDisplay, Chn, Num, TimeORText, Chn, Num, Enctype.c_str(), Chn, Num, Content.c_str(), Chn, Num, Color, Chn, Num, FontSize,Chn, Num, Post);
	}
	else  
	{
		mLogError("SetWebEffectNew SelectType = " << SelectType);
		return false;
	}
#if 0	
	else if(SelectType == 2)
	{
		sprintf(cChnCmd, "vid_enc%d.osd_on=%d\nvid_enc%d.osd%d.s_display=%d\nvid_enc%d.osd%d.char_set=%s\nvid_enc%d.osd%d.type=%d\nvid_enc%d.osd%d.color=%d\nvid_enc%d.osd%d.size=%d\nvid_enc%d.osd%d.content=%s\nvid_enc%d.osd%d.position=%d\n", \
			Chn, 1, Chn, Num, OverlayDisplay, Chn, Num, Enctype.c_str(), Chn, Num, 0, Chn, Num, Color, Chn, Num, FontSize, Chn, Num, Content.c_str(),Chn, Num, Post);

		ReqBody = cChnCmd + strlen(cChnCmd);
		
		sprintf(ReqBody, "vid_enc%d.substream.osd_on=%d\nvid_enc%d.superstream.osd_on=%d\n",Chn, 1, Chn, 1);
	}


    //sprintf(cChnCmd,INTERFACE_SETWEBEXTEFFECT_SENDCMD, Chn, Num, Enctype.c_str(),Chn, Num, OverlayDisplay, Chn, Num, Post, Chn, Num, Content.c_str(), Chn, Num, Color, Chn, Num, FontSize);
	//sprintf(cChnCmd,"vid_cap%d.ch1.osd%d.char_set=%s\nvid_cap%d.ch1.osd%d.s_display=%d\nvid_cap%d.ch1.osd%d.position=%d\nvid_cap%d.ch1.osd%d.content=%s\nvid_cap%d.ch1.osd%d.color=%d\nvid_cap%d.ch1.osd%d.size=%d\n",\
	//Chn, Num, Enctype.c_str(),Chn, Num, OverlayDisplay, Chn, Num, Post, Chn, Num, Content.c_str(), Chn, Num, Color, Chn, Num, FontSize);

#endif 

    //add fwb 20201110
    char cCmdSend[1024] = {0};
    char cHeadCmd[4] = {0};
    //char *cTmpCmd = INTERFACE_GETHOSTCONFIG_SENDCMD;
    int  cCmdLength = 0;
    memset(cCmdSend, 0, sizeof(1024)); 

    cCmdLength = strlen(cChnCmd) + 4;
    cHeadCmd[0] = 0; //当Buf[0]为0x00时，表示网管协议不加密,当Buf[0]为0x01时，表示网管协议加密
    cHeadCmd[1] = 3; //获取参数指令（2GET）
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

bool CSetWebEffectCtrl::SetWebEffectCaption(int Chn, int Post, int Num, int Type, string Content, CData &oResult, char *cResult)
{
    mLogDebug("run getChnConfig(...)");
    //获取状态

    //add fwb 20201110
    char cChnCmd[4096] = {0};
#if 1
    sprintf(cChnCmd,INTERFACE_SETWEBEXTEFFECT_SENDCMD, Chn, Num, Chn, Num, Chn, Num, Post, Chn, Num, Content.c_str());
#endif 

    //add fwb 20201027
    char cCmdSend[1024] = {0};
    char cHeadCmd[4] = {0};
    //char *cTmpCmd = INTERFACE_GETHOSTCONFIG_SENDCMD;
    int  cCmdLength = 0;
    memset(cCmdSend, 0, sizeof(1024)); 

    cCmdLength = strlen(cChnCmd) + 4;
    cHeadCmd[0] = 0; //当Buf[0]为0x00时，表示网管协议不加密,当Buf[0]为0x01时，表示网管协议加密
    cHeadCmd[1] = 3; //获取参数指令（GET）
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

bool CSetWebEffectCtrl::ComposeResult(cJSON *result)
{
    if(_result == NULL || result == NULL){
        mLogError("cJSON result == NULL.");
        result = NULL;
        return false;
    }

    return true;
}

string CSetWebEffectCtrl::ComposeResult()
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

bool CSetWebEffectCtrl::GetComposeResult(cJSON *result)
{
    if(_result == NULL || result == NULL){
        mLogError("cJSON result == NULL.");
        result = NULL;
        return false;
    }

    cJSON_AddItemToObject(_result, GET_INFOLIST_RES_VIDEOFILE, _VideoFiles);
    return true;
}

string CSetWebEffectCtrl::GetComposeResult()
{
    cJSON * result = cJSON_CreateObject();
    if(GetComposeResult(result) == false){
        cJSON_Delete(result);
        return "";
    }else{
        char *cRes = cJSON_Print(_result);
        string res = cRes;
        cJSON_Delete(result);
        return res;
    }
}

InterfaceResCode CSetWebEffectCtrl::SetWebEffectCtrlNew(string &sResult,int Chn, int Post,int Num, int Type, string Content, int OverlayDisplay, int Color, int FontSize, int SelectType, int TimeORText)
{
    //TODO
    mLogInfo("SetWebEffectCtrlNew...");
    CData oResult = SVSMAP();
    char cResult[4096] = {0};
    //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
#if 1
    if(SetWebEffectNew( OverlayDisplay, Chn, Post, Num, Type, Content, Color, FontSize, SelectType, TimeORText, oResult, cResult) == false){
        mLogError("Failed to run  SetRecCtrlStart(...)");
        return eInterfaceResCodeError;
    }
#endif 
    
    
#if 0
    int iRecvLength = (cResult[2] << 8) + cResult[3] + 4;
    printf("SetRecCtrlStart\n");
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

#if 1
    if( _AnalyParas.ConvertSTRING2CJSONSetWebEffectCtrl(oResult, cResult) == false){
        mLogError("Failed to run  ConvertSTRING2CJSONSetWebEffectCtrl(...)");
        return eInterfaceResCodeError;
    }
#endif 

    sResult = ComposeResult();
    return eInterfaceResCodeSuccess;
}

InterfaceResCode CSetWebEffectCtrl::SetWebEffectCtrlCaption(string &sResult,int Chn, int Post,int Num, int Type, string Content)
{
    //TODO
    mLogInfo("获取主机网络配置和通道配置信息...");
    CData oResult = SVSMAP();
    char cResult[4096] = {0};
    //printf("SetRecCtrlStop RecMode %d ChnID %d TaskId %s\n", RecMode, ChnID, TaskId.c_str());
#if 1
    if(SetWebEffectCaption(Chn, Post, Num, Type, Content,oResult, cResult) == false){
        mLogError("Failed to run  SetRecStop(...)");
        return eInterfaceResCodeError;
    }
#endif 
    
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

#if 1
    if( _AnalyParas.ConvertSTRING2CJSONSetWebEffectCtrl(oResult, cResult) == false){
        mLogError("Failed to run  ConvertSTRING2CJSONSetWebEffectCtrl(...)");
        return eInterfaceResCodeError;
    }
#endif 

    sResult = ComposeResult();
    return eInterfaceResCodeSuccess;
}
