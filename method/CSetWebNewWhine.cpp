#include "CSetWebNewWhine.h"
#include <boost/lexical_cast.hpp>

CSetWebNewWhine::CSetWebNewWhine(string ip, int port, int timeOut)
    :_result(cJSON_CreateObject())
    ,_total(cJSON_CreateObject())
    ,_addressList(cJSON_CreateArray())
    ,_ip(ip)
    ,_port(port)
    ,_timeOut(timeOut)
    ,_client(new MPSClient(ip,port,timeOut))
    ,_AnalyParas()
{

}

bool CSetWebNewWhine::ComposeResult(cJSON *result)
{
    if(_result == NULL || result == NULL){
        mLogError("cJSON result == NULL.");
        result = NULL;
        return false;
    }

    if(_addressList == NULL){
        mLogWarn("cJSON _addressList == NULL.");
        return false;
    }
    if(_total == NULL){
        mLogWarn("cJSON _total == NULL.");
        return false;
    }

    //result中添加节点
	//cJSON_AddItemToObject(_result,"Total",_total);
    //cJSON_AddItemToObject(_result,"AddressList",_addressList);	
    return true;
}

string CSetWebNewWhine::ComposeResult()
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
bool CSetWebNewWhine::SetWebNewWhineINner(int WhineChn,int WhineType, CData &oResult, char *cResult)
{
	mLogDebug("run SetWebNewWhineINner(...)");
	
	//1. 封装业务请求
	char szReqBody[REQ_BODY_MAXLEN] = { 0 };
	int num;
	switch(WhineChn)
	{
		case 1:
			 num = 1;
			 break;
		case 2:
			 num = 2;
			 break;
		case 3:
			 num = 5;
			 break;
		case 4:
			 num = 6;
			 break;
		default:
			 mLogError("SetWebNewWhineINner WhineChn error !!!\n"<<WhineChn);
			 return false;
	 }
						
	sprintf(szReqBody, INTERFACE_SET_AUDIO_WHINE_CONF_SENDCMD, num, WhineType);
	int realBodySize = strlen(szReqBody);
					
	char szReqCmd[REQ_CMD_MAXLEN] = { 0 };
	szReqCmd[0] = 0;  //不加密
	szReqCmd[1] = 3;  //2Get请求, 3是SET, 4是SYS
	szReqCmd[2] = realBodySize >> 8;
	szReqCmd[3] = realBodySize;
	memcpy(&szReqCmd[4], szReqBody, realBodySize);
					
	//2. 发送socket请求
	MPSOperationRes opResCode = eMPSResultOK; //??
	ResponseCode resCode = _client->GetConfigNew(szReqCmd, 4 + realBodySize, opResCode, oResult, cResult);
	if (resCode != eResponseCodeSuccess) {
		mLogError("GetConfig(...) error:" << resCode);
		return false;
	}

	return true;

}
bool CSetWebNewWhine::GetWebNewWhineINner( CData &oResult, char *cResult)
{
	mLogDebug("run SetWebNewWhineINner(...)");
	
	//1. 封装业务请求
	char *ReqBody;
	int num;
	char szReqBody[REQ_BODY_MAXLEN] = { 0 };
	memset(szReqBody,0,REQ_BODY_MAXLEN);
	for(int WhineChn = 1; WhineChn <= 4; WhineChn++)
	{
		switch(WhineChn)
	    {
			case 1:
				 num = 1;
				 break;
			case 2:
				 num = 2;
				 break;
			case 3:
				 num = 5;
				 break;
			case 4:
				 num = 6;
				 break;
			default:
				 mLogError("GetWebNewWhineINner WhineChn error !!!\n"<<WhineChn);
				 return false;
		 }

		ReqBody = szReqBody + strlen(szReqBody);
		sprintf(ReqBody, INTERFACE_GET_AUDIO_WHINE_CONF_SENDCMD, num,num);
	}	
	
	//sprintf(szReqBody, "aud_out1.ch1.audio_crack_mode\n");
	mLogDebug("szReqBody = "<<szReqBody);
	int realBodySize = strlen(szReqBody);
					
	char szReqCmd[REQ_CMD_MAXLEN] = { 0 };
	szReqCmd[0] = 0;  //不加密
	szReqCmd[1] = 2;  //2Get请求, 3是SET, 4是SYS
	szReqCmd[2] = realBodySize >> 8;
	szReqCmd[3] = realBodySize;
	memcpy(&szReqCmd[4], szReqBody, realBodySize);
					
	//2. 发送socket请求
	MPSOperationRes opResCode = eMPSResultOK; //??
	ResponseCode resCode = _client->GetConfigNew(szReqCmd, 4 + realBodySize, opResCode, oResult, cResult);
	if (resCode != eResponseCodeSuccess) {
		mLogError("GetConfig(...) error:" << resCode);
		return false;
	}

	return true;

}
bool CSetWebNewWhine::SetWebNewWhineButtonINner(int WhineChn,int WhineButton, CData &oResult, char *cResult)
{
	mLogDebug("run SetWebNewWhineButtonINner(...)");
	
	//1. 封装业务请求
	char szReqBody[REQ_BODY_MAXLEN] = { 0 };
	int num;
	switch(WhineChn)
    {
		case 1:
			 num = 1;
			 break;
		case 2:
			 num = 2;
			 break;
		case 3:
			 num = 5;
			 break;
		case 4:
			 num= 6;
			 break;
		default:
			 mLogError("SetWebNewWhineButtonINner WhineChn error !!!\n"<<WhineChn);
			 return false;
	 }					
	sprintf(szReqBody, INTERFACE_SET_AUDIO_WHINE_BUTTON_SENDCMD, num, WhineButton);
	int realBodySize = strlen(szReqBody);
					
	char szReqCmd[REQ_CMD_MAXLEN] = { 0 };
	szReqCmd[0] = 0;  //不加密
	szReqCmd[1] = 3;  //2Get请求, 3是SET, 4是SYS
	szReqCmd[2] = realBodySize >> 8;
	szReqCmd[3] = realBodySize;
	memcpy(&szReqCmd[4], szReqBody, realBodySize);
					
	//2. 发送socket请求
	MPSOperationRes opResCode = eMPSResultOK; //??
	ResponseCode resCode = _client->GetConfigNew(szReqCmd, 4 + realBodySize, opResCode, oResult, cResult);
	if (resCode != eResponseCodeSuccess) {
		mLogError("GetConfig(...) error:" << resCode);
		return false;
	}

	return true;

}
bool CSetWebNewWhine::GetWebNewWhineButtonINner( CData &oResult, char *cResult)
{
	mLogDebug("run GetWebNewWhineButtonINner(...)");
	
	//1. 封装业务请求
	char *ReqBody;
	int num;
	char szReqBody[REQ_BODY_MAXLEN] = { 0 };
	memset(szReqBody,0,REQ_BODY_MAXLEN);
	for(int WhineChn = 1; WhineChn <= 4; WhineChn++)
	{
		switch(WhineChn)
	    {
			case 1:
				 num = 1;
				 break;
			case 2:
				 num = 2;
				 break;
			case 3:
				 num = 5;
				 break;
			case 4:
				 num= 6;
				 break;
			default:
				 mLogError("GetWebNewWhineButtonINner WhineChn error !!!\n"<<WhineChn);
				 return false;
		 }
		ReqBody = szReqBody + strlen(szReqBody);
		//sprintf(ReqBody, INTERFACE_SET_AUDIO_WHINE_BUTTON_SENDCMD, num);
		sprintf(ReqBody, INTERFACE_GET_AUDIO_WHINE_CONF_SENDCMD, num,num);
	}					
	//mLogDebug("szReqBody = "<<szReqBody);
	int realBodySize = strlen(szReqBody);
					
	char szReqCmd[REQ_CMD_MAXLEN] = { 0 };
	szReqCmd[0] = 0;  //不加密
	szReqCmd[1] = 2;  //2Get请求, 3是SET, 4是SYS
	szReqCmd[2] = realBodySize >> 8;
	szReqCmd[3] = realBodySize;
	memcpy(&szReqCmd[4], szReqBody, realBodySize);
					
	//2. 发送socket请求
	MPSOperationRes opResCode = eMPSResultOK; //??
	ResponseCode resCode = _client->GetConfigNew(szReqCmd, 4 + realBodySize, opResCode, oResult, cResult);
	if (resCode != eResponseCodeSuccess) {
		mLogError("GetConfig(...) error:" << resCode);
		return false;
	}

	return true;

}

InterfaceResCode CSetWebNewWhine::SetWebNewWhine(string &sResult,int WhineChn,int WhineType)
{
		mLogInfo("SetWebNewWhine...");
		CData oResult = SVSMAP();
		char cResult[8192] = {0};
#if 1
		if(SetWebNewWhineINner(WhineChn, WhineType, oResult, cResult) == false){
			mLogError("Failed to run  GetConsoleConfig(...)");
			return eInterfaceResCodeError;
		}
#endif 
	
#if 0	
		if (!_AnalyParas.ConvertSTRING2CJSONParseSerialCmdParam(oResult, cResult, _result, _addressList))
		{
			mLogError("Failed to run  ConvertSTRING2CJSONParseSerialCmdParam(...)");
			return eInterfaceResCodeError;
		}
#endif		
#if 0
		int iRecvLength = (cResult[2] << 8) + cResult[3] + 4;
		printf("SendConsoleCmd\n");
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
	
		if( _AnalyParas.ConvertSTRING2CJSONSendConsoleCmd(oResult, cResult) == false){
			mLogError("Failed to run  ConvertSTRING2CJSONSendConsoleCmd(...)");
			return eInterfaceResCodeError;
		}
#endif 
	
		sResult = ComposeResult();
		cJSON_Delete(_result);

		return eInterfaceResCodeSuccess;

}


InterfaceResCode CSetWebNewWhine::GetWebNewWhine(string &sResult)
{
		mLogInfo("GetWebNewWhine...");
		CData oResult = SVSMAP();
		char cResult[8192] = {0};
		int ParamNumber = 4;
		cJSON *tmplist[20];
		string Method = METHOD_GET_WEB_NEW_WHINE;
#if 1
		if(GetWebNewWhineINner(oResult, cResult) == false){
			mLogError("Failed to run  GetConsoleConfig(...)");
			return eInterfaceResCodeError;
		}
#endif 	
	
#if 0
		int iRecvLength = (cResult[2] << 8) + cResult[3] + 4;
		printf("SendConsoleCmd\n");
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
		//mLogDebug("GetWebNewWhineINner ");
		for(int i = 1; i <= ParamNumber; i++)
		{
			tmplist[i] = cJSON_CreateObject();
		}
		if( _AnalyParas.ConvertSTRING2CJSONGetWebNewWhine(oResult, cResult, Method,_result, _addressList, tmplist,  ParamNumber) == false){
			mLogError("Failed to run  ConvertSTRING2CJSONSendConsoleCmd(...)");
			return eInterfaceResCodeError;
		}
 
		cJSON_AddItemToObject(_result, "WhineList", _addressList);
		sResult = ComposeResult();
		cJSON_Delete(_result);

		return eInterfaceResCodeSuccess;

}
InterfaceResCode CSetWebNewWhine::SetWebNewWhineButton(string &sResult,int WhineChn,int WhineButton)
{
		mLogInfo("SetWebNewWhineButton...");
		CData oResult = SVSMAP();
		char cResult[8192] = {0};
#if 1
		if(SetWebNewWhineButtonINner(WhineChn, WhineButton, oResult, cResult) == false){
			mLogError("Failed to run  GetConsoleConfig(...)");
			return eInterfaceResCodeError;
		}
#endif 
	
#if 0	
		if (!_AnalyParas.ConvertSTRING2CJSONParseSerialCmdParam(oResult, cResult, _result, _addressList))
		{
			mLogError("Failed to run  ConvertSTRING2CJSONParseSerialCmdParam(...)");
			return eInterfaceResCodeError;
		}
#endif		
#if 0
		int iRecvLength = (cResult[2] << 8) + cResult[3] + 4;
		printf("SendConsoleCmd\n");
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
	
		if( _AnalyParas.ConvertSTRING2CJSONSendConsoleCmd(oResult, cResult) == false){
			mLogError("Failed to run  ConvertSTRING2CJSONSendConsoleCmd(...)");
			return eInterfaceResCodeError;
		}
#endif 
	
		sResult = ComposeResult();
		cJSON_Delete(_result);

		return eInterfaceResCodeSuccess;

}

InterfaceResCode CSetWebNewWhine::GetWebNewWhineButton(string &sResult)
{
		mLogInfo("GetWebNewWhineButton...");
		CData oResult = SVSMAP();
		char cResult[8192] = {0};
		int ParamNumber = 4;
		cJSON *tmplist[20];
		string Method = METHOD_GET_WEB_NEW_WHINE_BUTTON;
#if 1
		if(GetWebNewWhineButtonINner(oResult, cResult) == false){
			mLogError("Failed to run  GetConsoleConfig(...)");
			return eInterfaceResCodeError;
		}
#endif 	
	
#if 0
		int iRecvLength = (cResult[2] << 8) + cResult[3] + 4;
		printf("SendConsoleCmd\n");
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
		for(int i = 1; i <= ParamNumber; i++)
		{
			tmplist[i] = cJSON_CreateObject();
			//mLogDebug("2222222222");
		}
		if( _AnalyParas.ConvertSTRING2CJSONGetWebNewWhine(oResult, cResult, Method,_result, _addressList, tmplist,  ParamNumber) == false){
			mLogError("Failed to run  ConvertSTRING2CJSONSendConsoleCmd(...)");
			return eInterfaceResCodeError;
		}
 
		cJSON_AddItemToObject(_result, "WhineList", _addressList);
		sResult = ComposeResult();
		cJSON_Delete(_result);

		return eInterfaceResCodeSuccess;

}



