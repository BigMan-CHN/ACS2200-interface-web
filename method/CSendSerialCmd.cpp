#include "CSendSerialCmd.h"
#include <boost/lexical_cast.hpp>

CSendSerialCmd::CSendSerialCmd(string ip, int port, int timeOut)
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

bool CSendSerialCmd::SetSerialConfig( double Val, int ChnNum, int CtrlType, CData &oResult, char *cResult)
{
    mLogDebug("run SetSerialConfig(...)");
    //获取状态
	int SerialID = 2;//串口透传，串口1
    //add fwb 20201119
    int  cCmdLength   = 0;
    char cCmdSend[1024]   = {0};

   // cCmdLength = strlen(Cmd.c_str());
   // memcpy(cCmdSend, Cmd.c_str(), strlen(Cmd.c_str()));
	//<指令类型,操作类型,输入/输出,通道,值>
	if(ChnNum > 31)
	{
		mLogError("SetSerialConfig ChnNum ="<<ChnNum);
        return false;
	}
	float Valtmp = Val;
	float GainVal = Valtmp/10;
	char szReqBody[REQ_BODY_MAXLEN] = { 0 };
	//sprintf(cCmdSend,INTERFACE_SET_AUDIO_GAIN_CONF_SENDCMD, CtrlType, ChnNum-1, Val);
	sprintf(cCmdSend,"<9,1,%d,%d,%.1f>", CtrlType, ChnNum, Val);
	mLogDebug("cCmdSend:"<<cCmdSend);
    cCmdLength = strlen(cCmdSend);
	
    MPSOperationRes opResCode = eMPSResultOK;
	//发送cCmdSend信息给服务器
    ResponseCode resCode = _client->GetSerialNew(SerialID, cCmdSend, cCmdLength, opResCode, oResult,cResult);
	mLogDebug("run SetSerialConfig  cResult="<<cResult);
    if(resCode != eResponseCodeSuccess){
        mLogError("GetConsoleNew(...) error:"<<resCode);
        return false;
    }

    return true;
}

bool CSendSerialCmd::ComposeResult(cJSON *result)
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
 
string CSendSerialCmd::ComposeResult()
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

InterfaceResCode CSendSerialCmd::SendSerialCmd(string &sResult, double Val, int ChnNum, int CtrlType)
{
    mLogInfo("设置音量SendSerialCmd...");
    CData oResult = SVSMAP();
    char cResult[8192] = {0};
	char Result[8192] = {0};

		if((ChnNum == 3||ChnNum == 5||ChnNum == 7||ChnNum == 9 )&& CtrlType == 1)
		{
		    if(SetSerialConfig(Val, ChnNum +1, CtrlType, oResult, Result) == false){
        	mLogError("Failed to run  GetConsoleConfig(...)");
        	return eInterfaceResCodeError;
    		}
			
			if (!_AnalyParas.ConvertSTRING2CJSONParseSerialCmdParam(oResult, Result, _result, _addressList,NULL))
		   {
			mLogError("Failed to run  ConvertSTRING2CJSONParseSerialCmdParam(...)");
			return eInterfaceResCodeError;
		   }
		}
		
		if(SetSerialConfig(Val, ChnNum, CtrlType, oResult, cResult) == false){
			mLogError("Failed to run  GetConsoleConfig(...)");
			return eInterfaceResCodeError;
		}


	    if (!_AnalyParas.ConvertSTRING2CJSONParseSerialCmdParam(oResult, cResult, _result, _addressList,NULL))
	   {
		mLogError("Failed to run  ConvertSTRING2CJSONParseSerialCmdParam(...)");
		return eInterfaceResCodeError;
	   }
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
    return eInterfaceResCodeSuccess;

}
bool CSendSerialCmd::SendAudioSerialCmdInner(int SerialID, string Cmd, CData &oResult, char *cResult)
{
    mLogDebug("run GetSerialConfig(...)");
    //获取状态

    //add fwb 20201119
    int  cCmdLength   = 0;
    char cCmdSend[1024]   = {0};

    cCmdLength = strlen(Cmd.c_str());
    memcpy(cCmdSend, Cmd.c_str(), strlen(Cmd.c_str()));
	mLogDebug("cCmdSend = "<<cCmdSend<<"cCmdLength = "<<cCmdLength);
    MPSOperationRes opResCode = eMPSResultOK;
    ResponseCode resCode = _client->GetSerialNew(SerialID, cCmdSend, cCmdLength, opResCode, oResult,cResult);
	mLogDebug("cResult = "<<cResult);
    if(resCode != eResponseCodeSuccess){
        mLogError("GetConsoleNew(...) error:"<<resCode);
        return false;
    }

    return true;
}

InterfaceResCode CSendSerialCmd::SendAudioSerialCmd(string &sResult, int SerialID, string Cmd)
{
    mLogInfo("发送串口透传指令...");
    CData oResult = SVSMAP();
    char cResult[8192] = {0};
	cJSON *tmp[40];
#if 1
    if(SendAudioSerialCmdInner(SerialID, Cmd, oResult, cResult) == false){
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


    if( _AnalyParas.ConvertSTRING2CJSONSendConsoleCmd(oResult, cResult) == false){
        mLogError("Failed to run  ConvertSTRING2CJSONSendConsoleCmd(...)");
        return eInterfaceResCodeError;
    }
#endif
 		//解析
	   	char *arg[40] = {};
		for (int i = 0; i < 40; i ++)
		{
			tmp[i] = cJSON_CreateObject();
			arg[i] = (char *)calloc(1, strlen(cResult)+1);
			if(NULL == arg[i]) 
			{
		 	   mLogError("calloc is error\n");
		 	   return eInterfaceResCodeError;
		 	}
	 	}
		
		//mLogDebug("cResult:"<<cResult);
		
		//命令格式cResult:
		sscanf(cResult, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s",\
		arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7], arg[8], arg[9],\
		arg[10], arg[11], arg[12], arg[13], arg[14], arg[15], arg[16], arg[17], arg[18], arg[19],\
		arg[20], arg[21], arg[22], arg[23], arg[24], arg[25], arg[26], arg[27], arg[28], arg[29], \
		arg[30], arg[31], arg[32], arg[33], arg[34], arg[35], arg[36], arg[37], arg[38], arg[39]);


		if(!strcmp(arg[0], "<Audio"))
		{
			for(int i = 0; i <= 30; i++)
			{
				cJSON_AddItemToObject(tmp[i], "AudioChn", cJSON_CreateNumber(i));
				cJSON_AddItemToObject(tmp[i], "Val", cJSON_CreateNumber(atoi(arg[i+1])));
				cJSON_AddItemToArray(_addressList, tmp[i]);
			}
		}else if(!strcmp(arg[1], "ok>"))
		{
			sResult = ComposeResult();
			cJSON_Delete(_result);
			return eInterfaceResCodeSuccess;
		}else{
			mLogError("SendAudioSerialCmd recv error\n");
			return eInterfaceResCodeError;
		}
		
	cJSON_AddItemToObject(_result,"AddressList",_addressList);
    sResult = ComposeResult();
	cJSON_Delete(_result);
    return eInterfaceResCodeSuccess;
}
bool CSendSerialCmd::SetAudioGainInner(int SerialID, int AudioChn, int Val, CData &oResult, char *cResult)
{
    mLogDebug("run SetAudioGainInner(...)");
    //获取状态

    //add fwb 20201119
    int  cCmdLength   = 0;
    char cCmdSend[1024]   = {0};
	//string cmd;

	sprintf(cCmdSend, "gain -s%d %d\r\n", AudioChn, Val);
	//memcpy(cCmdSend,);
    cCmdLength = strlen(cCmdSend);
	mLogDebug("cCmdSend = "<<cCmdSend<<"cCmdLength = "<<cCmdLength);
    //memcpy(cCmdSend, Cmd.c_str(), strlen(Cmd.c_str()));

    MPSOperationRes opResCode = eMPSResultOK;
    ResponseCode resCode = _client->GetSerialNew(SerialID, cCmdSend, cCmdLength, opResCode, oResult,cResult);
	mLogDebug("cResult = "<<cResult);
    if(resCode != eResponseCodeSuccess){
        mLogError("GetConsoleNew(...) error:"<<resCode);
        return false;
    }

    return true;
}

InterfaceResCode CSendSerialCmd::SetAudioGain(string &sResult, int SerialID, int AudioChn, int Val)
{
    mLogInfo("发送串口透传指令...");
    CData oResult = SVSMAP();
    char cResult[8192] = {0};
	char Result[8192] = {0};
#if 1
    if(SetAudioGainInner(SerialID, AudioChn, Val, oResult, cResult) == false){
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

    if( _AnalyParas.ConvertSTRING2CJSONSendConsoleCmd(oResult, cResult) == false){
        mLogError("Failed to run  ConvertSTRING2CJSONSendConsoleCmd(...)");
        return eInterfaceResCodeError;
    }
#endif 
	 		//解析
	   	char *arg[40] = {};
		for (int i = 0; i < 40; i ++)
		{
			//tmp[i] = cJSON_CreateObject();
			arg[i] = (char *)calloc(1, strlen(cResult)+1);
			if(NULL == arg[i]) 
			{
		 	   mLogError("calloc is error\n");
		 	   return eInterfaceResCodeError;
		 	}
	 	}
		
		//mLogDebug("cResult:"<<cResult);
		
		//命令格式cResult:
		sscanf(cResult, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s",\
		arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7], arg[8], arg[9],\
		arg[10], arg[11], arg[12], arg[13], arg[14], arg[15], arg[16], arg[17], arg[18], arg[19],\
		arg[20], arg[21], arg[22], arg[23], arg[24], arg[25], arg[26], arg[27], arg[28], arg[29], \
		arg[30], arg[31], arg[32], arg[33], arg[34], arg[35], arg[36], arg[37], arg[38], arg[39]);


		if(strcmp(arg[1], "ok>"))
		{
			 mLogError("SendAudioSerialCmd Get gain failed !!!\n");
			 return eInterfaceResCodeError;
		}
#if 0		
		//保存设置
		string Cmd = "drvw25 -w\r\n";
	if(SendAudioSerialCmdInner(SerialID, Cmd, oResult, Result) == false){
        mLogError("Failed to run  GetConsoleConfig(...)");
        return eInterfaceResCodeError;
    }
	
	char *argc[MAX_ARG_NUM] = {};
		for (int i = 0; i < MAX_ARG_NUM; i ++)
		{
			argc[i] = (char *)calloc(1, strlen(Result)+1);
			if(NULL == argc[i]) 
			{
			  mLogError("calloc is error\n");
			  return eInterfaceResCodeError;
			 }
		 }
		sscanf(Result, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s", argc[0], argc[1], argc[2], argc[3], argc[4], argc[5], argc[6], argc[7], argc[8], argc[9]);

		if(strcmp(argc[1], "ok>"))
		{
			mLogError("SendAudioSerialCmdInner save audio error\n");
			return eInterfaceResCodeError;

		}
#endif
    sResult = ComposeResult();
	cJSON_Delete(_result);
    return eInterfaceResCodeSuccess;
}

