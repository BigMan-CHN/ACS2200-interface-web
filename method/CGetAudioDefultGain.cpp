#include "CGetAudioDefultGain.h"
#include <boost/lexical_cast.hpp>

CGetAudioDefultGain::CGetAudioDefultGain(string ip, int port, int timeOut)
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

bool CGetAudioDefultGain::ComposeResult(cJSON *result)
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

string CGetAudioDefultGain::ComposeResult()
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
bool CGetAudioDefultGain::SetWebAudioSQLCreatAndUpdateInner(CData& oResult, char* cResult)
{


}

InterfaceResCode CGetAudioDefultGain::SetWebAudioSQLCreatAndUpdate(string &sResult)
{
	mLogDebug("run SetWebAudioSQLCreatAndUpdate(...)");

	char cResult[RES_BUF_MAXLEN] = { 0 };
	char Result[RES_BUF_MAXLEN] = { 0 };
	CData oResult = SVSMAP();

	sqlite3 *reg_db = NULL;
	char cmdbuf[32];
	char *register_db = "Audio_manage.db";
	int ChnNum[5] = {1,16,28,1,4};
	
	STU stu;
	time_t cur_time = time(NULL);
	struct tm *tm_ptr;
	tm_ptr = localtime(&cur_time);
	reg_db = (sqlite3 *)create_db(register_db); //创建数据库
	create_db_table(reg_db);  //创建注册表

	//delete_all_stu_db(reg_db); //从数据库删除所有数据
	//获取所有音频增益
	for(int CtrlType = 1; CtrlType <= 2; CtrlType++)
	{
		memset(stu.val, 0 , 256);
		if (!GetAudioAllGainInner(oResult, stu.val, ChnNum[0], CtrlType))
		{
			mLogError("Failed to run  GetAudioDefultGainInner(...)");
			return eInterfaceResCodeError112;
		}
		//sleep(1);
		
		//初始化数据结构
		stu.id = CtrlType;
		//stu.name = "GetAudioDefultGain";
		sprintf(stu.name,"Audio_AllDefultGain%d",CtrlType);
		sprintf(stu.time, "%04d/%02d/%02d-%02d:%02d:%02d", tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
		delete_stu_db(reg_db, &stu);
		insert_stu_db(reg_db, &stu); //add 到数据库 
		
		cJSON_AddItemToObject(_result, stu.name, cJSON_CreateString(stu.val));
		memset(stu.val, 0 , 256);
		if (!GetWebAudioAllMuteInner(oResult, stu.val, CtrlType, ChnNum[0]))
		{
			mLogError("Failed to run  GetWebAudioMuteInner(...)");
			return eInterfaceResCodeError113;
		}
		//sleep(1);
		stu.id = CtrlType + 2;
		sprintf(stu.name,"Audio_ALLDefultMute%d",CtrlType);
		//stu.name = "GetWebAudioDefultMute";	
		sprintf(stu.time, "%04d/%02d/%02d-%02d:%02d:%02d", tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
		
		delete_stu_db(reg_db, &stu);
		insert_stu_db(reg_db, &stu); //add 到数据库 
		cJSON_AddItemToObject(_result, stu.name, cJSON_CreateString(stu.val));

	}
	//获取音频参数（激励、回声消除等），cmd从3开始是因为灵敏度和幻象电源采用单通道获取方式，此处去掉获取灵敏度和幻象电源
	for(int cmd = 3; cmd <= 8; cmd++)//暂时不支持语音激励
	{
		memset(stu.val, 0 , 256);
		if (!GetWebAudioConfInner(oResult, stu.val, cmd))
		{
			mLogError("Failed to run  GetWebAudioConfInner(...)");
			return eInterfaceResCodeError120;
		}

		switch(cmd)
		{
			case 1:
				stu.id = 200;
				sprintf(stu.name,"Audio_SPL");
				sprintf(stu.time, "%04d/%02d/%02d-%02d:%02d:%02d", tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
				delete_stu_db(reg_db, &stu);
				insert_stu_db(reg_db, &stu); //add 到数据库 
				cJSON_AddItemToObject(_result, stu.name, cJSON_CreateString(stu.val));
				break;
			case 2:
				stu.id = 201;
				sprintf(stu.name,"Audio_IsPower");
				sprintf(stu.time, "%04d/%02d/%02d-%02d:%02d:%02d", tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
				delete_stu_db(reg_db, &stu);

				insert_stu_db(reg_db, &stu); //add 到数据库 
				cJSON_AddItemToObject(_result, stu.name, cJSON_CreateString(stu.val));
				break;
			case 3:
				stu.id = 5;
				sprintf(stu.name,"Audio_IsAGC");
				sprintf(stu.time, "%04d/%02d/%02d-%02d:%02d:%02d", tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
				delete_stu_db(reg_db, &stu);
				insert_stu_db(reg_db, &stu); //add 到数据库 
				cJSON_AddItemToObject(_result, stu.name, cJSON_CreateString(stu.val));
				break;
			case 4:
				stu.id = 6;
				sprintf(stu.name,"Audio_IsFeedback");
				sprintf(stu.time, "%04d/%02d/%02d-%02d:%02d:%02d", tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
				delete_stu_db(reg_db, &stu);
				insert_stu_db(reg_db, &stu); //add 到数据库 
				cJSON_AddItemToObject(_result, stu.name, cJSON_CreateString(stu.val));
				break;
			case 5:
				stu.id = 7;
				sprintf(stu.name,"Audio_IsAEC");
				sprintf(stu.time, "%04d/%02d/%02d-%02d:%02d:%02d", tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
				delete_stu_db(reg_db, &stu);
				insert_stu_db(reg_db, &stu); //add 到数据库 
				cJSON_AddItemToObject(_result, stu.name, cJSON_CreateString(stu.val));
				break;
			case 6:
				stu.id = 8;
				sprintf(stu.name,"Audio_IsANS");
				sprintf(stu.time, "%04d/%02d/%02d-%02d:%02d:%02d", tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
				delete_stu_db(reg_db, &stu);
				insert_stu_db(reg_db, &stu); //add 到数据库 
				cJSON_AddItemToObject(_result, stu.name, cJSON_CreateString(stu.val));
				break;
			case 7:
				stu.id = 9;
				sprintf(stu.name,"Audio_IsIncentives");
				sprintf(stu.time, "%04d/%02d/%02d-%02d:%02d:%02d", tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
				delete_stu_db(reg_db, &stu);
				insert_stu_db(reg_db, &stu); //add 到数据库 
				cJSON_AddItemToObject(_result, stu.name, cJSON_CreateString(stu.val));
				break;
 
		}

	}
#if 1	
	//获取MIC变声（14路，第12路控制13，14路）变声开关状态与变声大小，通道号为0会返回所有通道数据，否则返回对应通道数据
	for(int ChnNum = 0 ;ChnNum <=0; ChnNum++)
	{	
		memset(stu.val, 0 , 256);
		if (!GetWebMICWhineButtonInner(oResult, stu.val, ChnNum))
		{
			mLogError("Failed to run  GetWebMICWhineButtonInner(...)");
			return eInterfaceResCodeError119;
		}
		//sleep(1);
//		stu.id = ChnNum + 11;
		stu.id = 10;
		sprintf(stu.name,"Audio_MICWhineButton%d",ChnNum);
		sprintf(stu.time, "%04d/%02d/%02d-%02d:%02d:%02d", tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
		delete_stu_db(reg_db, &stu);
		insert_stu_db(reg_db, &stu); //add 到数据库 
		cJSON_AddItemToObject(_result, stu.name, cJSON_CreateString(stu.val));
		memset(stu.val, 0 , 256);
		if(GetWebMICWhineGainInner( oResult, stu.val, ChnNum) == false)
		{
			mLogError("Failed to run  GetWebMICGainInner(...)");
			return eInterfaceResCodeError118;
		}
		//sleep(1);
		stu.id = 11;
		sprintf(stu.name,"Audio_MICWhineGain%d",ChnNum);
		sprintf(stu.time, "%04d/%02d/%02d-%02d:%02d:%02d", tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
		delete_stu_db(reg_db, &stu);
		insert_stu_db(reg_db, &stu); //add 到数据库 
		cJSON_AddItemToObject(_result, stu.name, cJSON_CreateString(stu.val));
	}
#endif
	//获取MIC输入1-14（14路，第12路控制13，14路）通道灵敏度，通道号为0会返回所有通道数据，否则返回对应通道数据
	for(int ChnNum = 0 ;ChnNum <= 0; ChnNum++)
	{	
		memset(stu.val, 0 , 256);
		if (!GetWebAudioSPLInner(oResult, stu.val, ChnNum))
		{
			mLogError("Failed to run  GetWebAudioSPLInner(...)");
			return eInterfaceResCodeError120;
		}
		//sleep(1);
		stu.id = 12;
		sprintf(stu.name,"Audio_MICInPutSPL%d",ChnNum);
		sprintf(stu.time, "%04d/%02d/%02d-%02d:%02d:%02d", tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
		delete_stu_db(reg_db, &stu);
		insert_stu_db(reg_db, &stu); //add 到数据库 
		cJSON_AddItemToObject(_result, stu.name, cJSON_CreateString(stu.val));
	}
	//获取MIC输入1-14（14路，第12路控制13，14路）通道幻象电源状态
	for(int ChnNum = 0 ;ChnNum <= 0; ChnNum++)
	{	
		memset(stu.val, 0 , 256);
		if (!GetWebAudioPowerInner(oResult, stu.val, ChnNum))
		{
			mLogError("Failed to run  GetWebAudioPowerInner(...)");
			return eInterfaceResCodeError120;
		}
		//sleep(1);
		stu.id = 13;
		sprintf(stu.name,"Audio_MICInPutPower%d",ChnNum);
		sprintf(stu.time, "%04d/%02d/%02d-%02d:%02d:%02d", tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
		delete_stu_db(reg_db, &stu);
		insert_stu_db(reg_db, &stu); //add 到数据库 
		cJSON_AddItemToObject(_result, stu.name, cJSON_CreateString(stu.val));
	}
#if 1	
	//获取语音激励阈值
	for(int ChnNum = 0 ;ChnNum <= 0; ChnNum++)
	{	
		memset(stu.val, 0 , 256);
		if (!GetWebAudioMotivationValInner(oResult, stu.val, ChnNum))
		{
			mLogError("Failed to run  GetWebAudioMotivationValInner(...)");
			return eInterfaceResCodeError120;
		}
		//sleep(1);
		stu.id = 14;
		sprintf(stu.name,"Audio_MotivationVal%d",ChnNum);
		sprintf(stu.time, "%04d/%02d/%02d-%02d:%02d:%02d", tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
		delete_stu_db(reg_db, &stu);
		insert_stu_db(reg_db, &stu); //add 到数据库 
		cJSON_AddItemToObject(_result, stu.name, cJSON_CreateString(stu.val));
	}
	//获取语音激励反应时间
	for(int ChnNum = 0 ;ChnNum <= 0; ChnNum++)
	{	
		memset(stu.val, 0 , 256);
		if (!GetWebAudioMotivationTimeInner(oResult, stu.val, ChnNum))
		{
			mLogError("Failed to run  GetWebAudioMotivationTimeInner(...)");
			return eInterfaceResCodeError120;
		}
		//sleep(1);
		stu.id = 15;
		sprintf(stu.name,"Audio_MotivationTime%d",ChnNum);
		sprintf(stu.time, "%04d/%02d/%02d-%02d:%02d:%02d", tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
		delete_stu_db(reg_db, &stu);
		insert_stu_db(reg_db, &stu); //add 到数据库 
		cJSON_AddItemToObject(_result, stu.name, cJSON_CreateString(stu.val));
	}
#endif
	sqlite3_close(reg_db);
	sResult = ComposeResult();
	cJSON_Delete(_result);
	
	return eInterfaceResCodeSuccess;
}
bool CGetAudioDefultGain::GetWebAudioSPLInner( CData& oResult, char* cResult, int ChnNum)
{
	mLogDebug("run GetWebAudioSPLInner(...)");

	//获取状态
	int SerialID = 2;//串口透传，串口1
	int  cCmdLength   = 0;
	char cCmdSend[1024]   = {0};
	int Flag = 0;
//	sprintf( cCmdSend, "<1,2,2,%d,0>", ChnNum-1); 
	sprintf( cCmdSend, "<1,2,2,0,0>");
	cCmdLength = strlen(cCmdSend);
	
	MPSOperationRes opResCode = eMPSResultOK;
ReSend:
	//发送cCmdSend信息给服务器
	ResponseCode resCode = _client->GetSerialNew(SerialID, cCmdSend, cCmdLength, opResCode, oResult, cResult);
	mLogDebug("GetWebAudioSPLInner cResult"<<cResult);
	if(resCode != eResponseCodeSuccess){
		mLogError("GetSerialNew(...) error:"<<resCode);
		return false;
	}
	if(!strcmp(cResult, "<error>"))
	{
		if (Flag < 2)
		{
			mLogDebug("GetWebAudioSPLInner Send GetSerialNew:"<<Flag);
			Flag++;
			goto ReSend;
		}else{
			mLogError("GetWebAudioSPLInner GetSerialNew error:"<<Flag);
			return false;

		}
		
	}
//	char *arg[MAX_ARG_NUM] = {};
//	for (int i = 0; i < MAX_ARG_NUM; i ++)
//	{
//		arg[i] = (char *)calloc(1, strlen(cResult)+1);
//		if(NULL == arg[i]) 
//		{
//		  mLogError("calloc is error\n");
//		  return false;
//		 }
//	 }
//	//mLogDebug("cResult:"<<cResult);
//	//命令格式cResult: "<9（指令类型）,2（操作类型，1:SET，2:GET）,2（输入2/输出1）,1（通道0~31）,80（值）,ok（状态）>"
//	sscanf(cResult, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s", arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7], arg[8], arg[9]);
	char *carg[40] = {};
	for (int i = 0; i < 40; i ++)
	{
		 carg[i] = (char *)calloc(1, strlen(cResult)+1);
		 if(NULL == carg[i]) 
		 {
		  mLogError("calloc is error\n");
		  return false;
		}
	 }
	sscanf(cResult, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s",\
	carg[0], carg[1], carg[2], carg[3], carg[4], carg[5], carg[6], carg[7], carg[8], carg[9],\
	carg[10], carg[11], carg[12], carg[13], carg[14], carg[15], carg[16], carg[17], carg[18], carg[19],\
	carg[20], carg[21], carg[22], carg[23], carg[24], carg[25], carg[26], carg[27], carg[28], carg[29], \
	carg[30], carg[31], carg[32], carg[33], carg[34], carg[35], carg[36], carg[37], carg[38], carg[39]);

	if(strcmp(carg[16], "ok>"))
    {
	  mLogError("GetWebAudioSPLInner Get  failed !!!\n");;
	  return false;
    }
	 return true;

}
bool CGetAudioDefultGain::GetWebAudioPowerInner( CData& oResult, char* cResult, int ChnNum)
{
	mLogDebug("run GetWebAudioPowerInner(...)");

	//获取状态
	int SerialID = 2;//串口透传，串口1
	int  cCmdLength   = 0;
	char cCmdSend[1024]   = {0};
	int Flag = 0;
	//sprintf( cCmdSend, "<2,2,2,%d,0>", ChnNum-1); 
	sprintf( cCmdSend, "<2,2,2,0,0>"); 
	cCmdLength = strlen(cCmdSend);
	
	MPSOperationRes opResCode = eMPSResultOK;
ReSend:
	//发送cCmdSend信息给服务器
	ResponseCode resCode = _client->GetSerialNew(SerialID, cCmdSend, cCmdLength, opResCode, oResult, cResult);
	if(resCode != eResponseCodeSuccess){
		mLogError("GetSerialNew(...) error:"<<resCode);
		return false;
	}
	if(!strcmp(cResult, "<error>"))
	{
		if (Flag < 2)
		{
			mLogDebug("GetWebAudioPowerInner Send GetSerialNew:"<<Flag);
			Flag++;
			goto ReSend;
		}else{
			mLogError("GetWebAudioPowerInner GetSerialNew error:"<<Flag);
			return false;

		}
		
	}
//	char *arg[MAX_ARG_NUM] = {};
//	for (int i = 0; i < MAX_ARG_NUM; i ++)
//	{
//		arg[i] = (char *)calloc(1, strlen(cResult)+1);
//		if(NULL == arg[i]) 
//		{
//		  mLogError("calloc is error\n");
//		  return false;
//		 }
//	 }
//	//mLogDebug("cResult:"<<cResult);
//	//命令格式cResult: "<9（指令类型）,2（操作类型，1:SET，2:GET）,2（输入2/输出1）,1（通道0~31）,80（值）,ok（状态）>"
//	sscanf(cResult, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s", arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7], arg[8], arg[9]);
	char *carg[40] = {};
	for (int i = 0; i < 40; i ++)
	{
		 carg[i] = (char *)calloc(1, strlen(cResult)+1);
		 if(NULL == carg[i]) 
		 {
		  mLogError("calloc is error\n");
		  return false;
		}
	 }
	sscanf(cResult, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s",\
	carg[0], carg[1], carg[2], carg[3], carg[4], carg[5], carg[6], carg[7], carg[8], carg[9],\
	carg[10], carg[11], carg[12], carg[13], carg[14], carg[15], carg[16], carg[17], carg[18], carg[19],\
	carg[20], carg[21], carg[22], carg[23], carg[24], carg[25], carg[26], carg[27], carg[28], carg[29], \
	carg[30], carg[31], carg[32], carg[33], carg[34], carg[35], carg[36], carg[37], carg[38], carg[39]);

	if(strcmp(carg[16], "ok>"))
    {
	  mLogError("GetWebAudioPowerInner Get  failed !!!\n");;
	  return false;
    }
	 return true;

}
bool CGetAudioDefultGain::GetWebAudioMotivationValInner( CData& oResult, char* cResult, int ChnNum)
{
	mLogDebug("run GetWebAudioMotivationValInner(...)");

	//获取状态
	int SerialID = 2;//串口透传，串口1
	int  cCmdLength   = 0;
	char cCmdSend[1024]   = {0};
	int Flag = 0;
	//sprintf( cCmdSend, "<2,2,2,%d,0>", ChnNum-1); 
	sprintf( cCmdSend, "<14,2,1,0,0>"); 
	cCmdLength = strlen(cCmdSend);
	
	MPSOperationRes opResCode = eMPSResultOK;
ReSend:
	//发送cCmdSend信息给服务器
	ResponseCode resCode = _client->GetSerialNew(SerialID, cCmdSend, cCmdLength, opResCode, oResult, cResult);
	if(resCode != eResponseCodeSuccess){
		mLogError("GetSerialNew(...) error:"<<resCode);
		return false;
	}
	if(!strcmp(cResult, "<error>"))
	{
		if (Flag < 2)
		{
			mLogDebug("GetWebAudioMotivationValInner Send GetSerialNew:"<<Flag);
			Flag++;
			goto ReSend;
		}else{
			mLogError("GetWebAudioMotivationValInner GetSerialNew error:"<<Flag);
			return false;

		}
		
	}
//	char *arg[MAX_ARG_NUM] = {};
//	for (int i = 0; i < MAX_ARG_NUM; i ++)
//	{
//		arg[i] = (char *)calloc(1, strlen(cResult)+1);
//		if(NULL == arg[i]) 
//		{
//		  mLogError("calloc is error\n");
//		  return false;
//		 }
//	 }
//	//mLogDebug("cResult:"<<cResult);
//	//命令格式cResult: "<9（指令类型）,2（操作类型，1:SET，2:GET）,2（输入2/输出1）,1（通道0~31）,80（值）,ok（状态）>"
//	sscanf(cResult, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s", arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7], arg[8], arg[9]);
	char *carg[40] = {};
	for (int i = 0; i < 40; i ++)
	{
		 carg[i] = (char *)calloc(1, strlen(cResult)+1);
		 if(NULL == carg[i]) 
		 {
		  mLogError("calloc is error\n");
		  return false;
		}
	 }
	sscanf(cResult, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s",\
	carg[0], carg[1], carg[2], carg[3], carg[4], carg[5], carg[6], carg[7], carg[8], carg[9],\
	carg[10], carg[11], carg[12], carg[13], carg[14], carg[15], carg[16], carg[17], carg[18], carg[19],\
	carg[20], carg[21], carg[22], carg[23], carg[24], carg[25], carg[26], carg[27], carg[28], carg[29], \
	carg[30], carg[31], carg[32], carg[33], carg[34], carg[35], carg[36], carg[37], carg[38], carg[39]);

	if(strcmp(carg[16], "ok>"))
    {
	  mLogError("GetWebAudioMotivationValInner Get  failed !!!\n");;
	  return false;
    }
	 return true;

}
bool CGetAudioDefultGain::GetWebAudioMotivationTimeInner( CData& oResult, char* cResult, int ChnNum)
{
	mLogDebug("run GetWebAudioMotivationTimeInner(...)");

	//获取状态
	int SerialID = 2;//串口透传，串口1
	int  cCmdLength   = 0;
	char cCmdSend[1024]   = {0};
	int Flag = 0;
	//sprintf( cCmdSend, "<2,2,2,%d,0>", ChnNum-1); 
	sprintf( cCmdSend, "<15,2,1,1,0>"); 
	cCmdLength = strlen(cCmdSend);
	
	MPSOperationRes opResCode = eMPSResultOK;
ReSend:
	//发送cCmdSend信息给服务器
	ResponseCode resCode = _client->GetSerialNew(SerialID, cCmdSend, cCmdLength, opResCode, oResult, cResult);
	if(resCode != eResponseCodeSuccess){
		mLogError("GetSerialNew(...) error:"<<resCode);
		return false;
	}
	if(!strcmp(cResult, "<error>"))
	{
		if (Flag < 2)
		{
			mLogDebug("GetWebAudioMotivationTimeInner Send GetSerialNew:"<<Flag);
			Flag++;
			goto ReSend;
		}else{
			mLogError("GetWebAudioMotivationTimeInner GetSerialNew error:"<<Flag);
			return false;

		}
		
	}
//	char *arg[MAX_ARG_NUM] = {};
//	for (int i = 0; i < MAX_ARG_NUM; i ++)
//	{
//		arg[i] = (char *)calloc(1, strlen(cResult)+1);
//		if(NULL == arg[i]) 
//		{
//		  mLogError("calloc is error\n");
//		  return false;
//		 }
//	 }
//	//mLogDebug("cResult:"<<cResult);
//	//命令格式cResult: "<9（指令类型）,2（操作类型，1:SET，2:GET）,2（输入2/输出1）,1（通道0~31）,80（值）,ok（状态）>"
//	sscanf(cResult, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s", arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7], arg[8], arg[9]);
	char *carg[40] = {};
	for (int i = 0; i < 40; i ++)
	{
		 carg[i] = (char *)calloc(1, strlen(cResult)+1);
		 if(NULL == carg[i]) 
		 {
		  mLogError("calloc is error\n");
		  return false;
		}
	 }
	sscanf(cResult, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s",\
	carg[0], carg[1], carg[2], carg[3], carg[4], carg[5], carg[6], carg[7], carg[8], carg[9],\
	carg[10], carg[11], carg[12], carg[13], carg[14], carg[15], carg[16], carg[17], carg[18], carg[19],\
	carg[20], carg[21], carg[22], carg[23], carg[24], carg[25], carg[26], carg[27], carg[28], carg[29], \
	carg[30], carg[31], carg[32], carg[33], carg[34], carg[35], carg[36], carg[37], carg[38], carg[39]);

	if(strcmp(carg[5], "ok>"))
    {
	  mLogError("GetWebAudioMotivationTimeInner Get  failed !!!\n");;
	  return false;
    }
	 return true;

}

bool CGetAudioDefultGain::GetWebAudioConfInner(CData& oResult, char* cResult, int cmd)
{
	mLogDebug("run GetWebAudioConfInner(...)");

	 //获取状态
	 int SerialID = 2;//串口透传，串口1
	 int  cCmdLength   = 0;
	 char cCmdSend[1024]   = {0};
	 int Flag = 0;
	 //string Cmd[8] = {"<1,2,2,0,0>","<2,2,2,0,0>","<3,2,2,0,0>","<4,2,2,0,0>","<5,2,2,0,0>","<6,2,2,0,0>","<7,2,2,0,0>"};
	 //<指令类型,操作类型,输入/输出,通道,值>
	//语音激励开关
	 if(cmd == 7)
	 	cmd = 11;
	 sprintf(cCmdSend,"<%d,2,2,0,0>",cmd); 
	 cCmdLength = strlen(cCmdSend);
	 
	 MPSOperationRes opResCode = eMPSResultOK;
	 //发送cCmdSend信息给服务器
ReSend:
	 ResponseCode resCode = _client->GetSerialNew(SerialID, cCmdSend, cCmdLength, opResCode, oResult,cResult);
	 mLogDebug("GetWebAudioConfInner cResult:"<<cResult);

	 if(resCode != eResponseCodeSuccess){
		 mLogError("GetConsoleNew(...) error:"<<resCode);
		 return false;
	 }

	if(!strcmp(cResult, "<error>"))
	{
		if (Flag < 2)
		{
			mLogDebug("GetWebAudioConfInner Send GetSerialNew:"<<Flag);
			Flag++;
			goto ReSend;
		}else{
			mLogError("GetWebAudioConfInner GetSerialNew error:"<<Flag);
			return false;

		}
		
	}
	char *arg[MAX_ARG_NUM] = {};
	for (int i = 0; i < MAX_ARG_NUM; i ++)
	{
		arg[i] = (char *)calloc(1, strlen(cResult)+1);
		if(NULL == arg[i]) 
		{
		  mLogError("calloc is error\n");
		  return false;
		 }
	 }
	//mLogDebug("cResult:"<<cResult);
	//命令格式cResult: "<9（指令类型）,2（操作类型，1:SET，2:GET）,2（输入2/输出1）,1（通道0~31）,80（值）,ok（状态）>"
	sscanf(cResult, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s", arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7], arg[8], arg[9]);
	if(!strcmp(arg[5], "error>"))
    {
	  mLogError("GetWebAudioConfInner Get  failed !!!\n");;
	  return false;
    }
	 return true;
}

bool CGetAudioDefultGain::GetWebMICWhineButtonInner(CData& oResult, char* cResult, int ChnNam)
{
	mLogDebug("run GetWebMICWhineButtonInner(...)");

	 //获取状态
	 int SerialID = 2;//串口透传，串口1
	 int  cCmdLength   = 0;
	 char cCmdSend[1024]   = {0};
	 int Flag = 0;
	 //string Cmd[8] = {"<1,2,2,0,0>","<2,2,2,0,0>","<3,2,2,0,0>","<4,2,2,0,0>","<5,2,2,0,0>","<6,2,2,0,0>","<7,2,2,0,0>"};
	 //cCmdLength = strlen(Cmd.c_str());
	 //memcpy(cCmdSend, Cmd.c_str(), strlen(Cmd.c_str()));

	 //<指令类型,操作类型,输入/输出,通道,值>
	 //开启变声开关
//	 sprintf(cCmdSend,INTERFACE_GETWEB_WHINE_OPEN_SENDCMD,ChnNam-1);
	 sprintf(cCmdSend,"<7,2,2,0,0>"); 
	 cCmdLength = strlen(cCmdSend);
	 
	 MPSOperationRes opResCode = eMPSResultOK;
	 //发送cCmdSend信息给服务器
ReSend:
	 ResponseCode resCode = _client->GetSerialNew(SerialID, cCmdSend, cCmdLength, opResCode, oResult, cResult);
	 mLogDebug("GetWebMICWhineButtonInner cResult:"<<cResult);
	 if(resCode != eResponseCodeSuccess){
		 mLogError("GetSerialNew1(...) error:"<<resCode);
		 return false;
	 } 
	 if(!strcmp(cResult, "<error>"))
	{
		if (Flag < 2)
		{
			mLogDebug("GetWebMICWhineButtonInner Send GetSerialNew:"<<Flag);
			Flag++;
			goto ReSend;
		}else{
			mLogError("GetWebMICWhineButtonInner GetSerialNew error:"<<Flag);
			return false;

		}
		
	}
	 //判断开关状态
//	 char *arg[MAX_ARG_NUM] = {};
//	 for (int i = 0; i < MAX_ARG_NUM; i ++)
//	 {
//		 arg[i] = (char *)calloc(1, strlen(cResult)+1);
//		 if(NULL == arg[i]) 
//		 {
//		   mLogError("calloc is error\n");
//		   return false;
//		  }
//	  }
//	 sscanf(cResult, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s", arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7], arg[8], arg[9]);
	 char *carg[40] = {};
	 for (int i = 0; i < 40; i ++)
	 {
		  carg[i] = (char *)calloc(1, strlen(cResult)+1);
		  if(NULL == carg[i]) 
		  {
		   mLogError("calloc is error\n");
		   return false;
		 }
	  }
	 sscanf(cResult, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s",\
	 carg[0], carg[1], carg[2], carg[3], carg[4], carg[5], carg[6], carg[7], carg[8], carg[9],\
	 carg[10], carg[11], carg[12], carg[13], carg[14], carg[15], carg[16], carg[17], carg[18], carg[19],\
	 carg[20], carg[21], carg[22], carg[23], carg[24], carg[25], carg[26], carg[27], carg[28], carg[29], \
	 carg[30], carg[31], carg[32], carg[33], carg[34], carg[35], carg[36], carg[37], carg[38], carg[39]);

	 if (strcmp("ok>", carg[27]))
	 {
		 mLogError("GET MIC button failed");
		 return false;
	 }

	 return true;

}

bool CGetAudioDefultGain::GetWebMICWhineGainInner(CData& oResult, char* cResult, int ChnNam)
{
	mLogDebug("run GetWebMICGainInner(...)");

	 int SerialID = 2;//串口透传，串口1
	 int  cCmdLength   = 0;
	 char cCmdSend[1024]   = {0};
	 int Flag = 0;
	 //string Cmd[8] = {"<1,2,2,0,0>","<2,2,2,0,0>","<3,2,2,0,0>","<4,2,2,0,0>","<5,2,2,0,0>","<6,2,2,0,0>","<7,2,2,0,0>"};
	 //cCmdLength = strlen(Cmd.c_str());
	 //memcpy(cCmdSend, Cmd.c_str(), strlen(Cmd.c_str()));

	 //<指令类型,操作类型,输入/输出,通道,值>
//	 sprintf(cCmdSend,INTERFACE_GETWEB_MIC_GAIN_SENDCMD, ChnNam-1);
	 sprintf(cCmdSend,"<8,2,2,0,0>"); 
	 cCmdLength = strlen(cCmdSend);
	 
	 MPSOperationRes opResCode = eMPSResultOK;
ReSend:
	 ResponseCode resCode = _client->GetSerialNew(SerialID, cCmdSend, cCmdLength, opResCode, oResult,cResult);
	 mLogDebug("GetWebMICWhineGainInner cResult:"<<cResult);
	 if(resCode != eResponseCodeSuccess){
		 mLogError("GetSerialNew(...) error:"<<resCode);
		 return false;
	 }
	 
	if(!strcmp(cResult, "<error>"))
	{
		if (Flag < 2)
		{
			mLogDebug("GetWebMICWhineGainInner Send GetSerialNew:"<<Flag);
			Flag++;
			goto ReSend;
		}else{
			mLogError("GetWebMICWhineGainInner GetSerialNew error:"<<Flag);
			return false;
		}
		
	}
	//判断是否获取变声调节成功
//	 char *arg[MAX_ARG_NUM] = {};
//	 for (int i = 0; i < MAX_ARG_NUM; i ++)
//	 {
//		 arg[i] = (char *)calloc(1, strlen(cResult)+1);
//		 if(NULL == arg[i]) 
//		 {
//		   mLogError("calloc is error\n");
//		   return false;
//		  }
//	  }
//	 sscanf(cResult, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s", arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7], arg[8], arg[9]);
	 char *carg[40] = {};
	 for (int i = 0; i < 40; i ++)
	 {
		  carg[i] = (char *)calloc(1, strlen(cResult)+1);
		  if(NULL == carg[i]) 
		  {
		   mLogError("calloc is error\n");
		   return false;
		 }
	  }
	 sscanf(cResult, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s",\
	 carg[0], carg[1], carg[2], carg[3], carg[4], carg[5], carg[6], carg[7], carg[8], carg[9],\
	 carg[10], carg[11], carg[12], carg[13], carg[14], carg[15], carg[16], carg[17], carg[18], carg[19],\
	 carg[20], carg[21], carg[22], carg[23], carg[24], carg[25], carg[26], carg[27], carg[28], carg[29], \
	 carg[30], carg[31], carg[32], carg[33], carg[34], carg[35], carg[36], carg[37], carg[38], carg[39]);

	 if (strcmp("ok>", carg[27]))
	 {
		 mLogError("GetWebMICGainInner failed");
		 return false;
	 }

	 return true;


}

bool CGetAudioDefultGain::GetWebAudioMuteInner(CData& oResult, char* cResult, int CtrlType,int ChnNam)
{
	mLogDebug("run GetWebAudioMuteInner(...)");

	 int SerialID = 2;//串口透传，串口1
	 int  cCmdLength   = 0;
	 char cCmdSend[1024]   = {0};
	 //string Cmd[8] = {"<1,2,2,0,0>","<2,2,2,0,0>","<3,2,2,0,0>","<4,2,2,0,0>","<5,2,2,0,0>","<6,2,2,0,0>","<7,2,2,0,0>"};
	 //cCmdLength = strlen(Cmd.c_str());
	 //memcpy(cCmdSend, Cmd.c_str(), strlen(Cmd.c_str()));

	 //<指令类型,操作类型,输入/输出,通道,值>
	 sprintf(cCmdSend,INTERFACE_GET_AUDIO_MUTE_SENDCMD, CtrlType, ChnNam-1); 
	 cCmdLength = strlen(cCmdSend);
	 
	 MPSOperationRes opResCode = eMPSResultOK;
	 ResponseCode resCode = _client->GetSerialNew(SerialID, cCmdSend, cCmdLength, opResCode, oResult,cResult);
	 if(resCode != eResponseCodeSuccess){
		 mLogError("GetSerialNew(...) error:"<<resCode);
		 return false;
	 }
	//判断是否设置变声调节成功
	 char *arg[MAX_ARG_NUM] = {};
	 for (int i = 0; i < MAX_ARG_NUM; i ++)
	 {
		 arg[i] = (char *)calloc(1, strlen(cResult)+1);
		 if(NULL == arg[i]) 
		 {
		   mLogError("calloc is error\n");
		   return false;
		  }
	  }
	 sscanf(cResult, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s", arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7], arg[8], arg[9]);
	 if (strcmp("ok>", arg[5]))
	 {
		 mLogError("GetWebAudioMuteInner failed");
		 return false;
	 }
	 return true;


}

bool CGetAudioDefultGain::GetWebAudioGainInner(CData& oResult, char* cResult, int ChnNum, int CtrlType)
{
		mLogDebug("run GetAudioGainInner(...)");
			
		//获取状态
		int SerialID = 2;//串口透传，串口1
		//add fwb 20201119
		int val = 0;
		int  cCmdLength   = 0;
		char cCmdSend[1024]   = {0};
		
		//<指令类型,操作类型,输入/输出,通道,值>
		if(ChnNum > 32)
		{
			mLogError("GetAudioGainInner ChnNum ="<<ChnNum-1);
       		return false;
		}
		sprintf(cCmdSend,INTERFACE_GET_AUDIO_GAIN_CONF_SENDCMD, CtrlType, ChnNum-1, val);
		
		cCmdLength = strlen(cCmdSend);
		
		MPSOperationRes opResCode = eMPSResultOK;
		//发送cCmdSend信息给服务器
		ResponseCode resCode = _client->GetSerialNew(SerialID, cCmdSend, cCmdLength, opResCode, oResult,cResult);
		if(resCode != eResponseCodeSuccess){
			mLogError("GetConsoleNew(...) error:"<<resCode);
			return false;
		}
		return true;

}


bool CGetAudioDefultGain::GetWebAudioAllMuteInner(CData& oResult, char* cResult, int CtrlType,int ChnNum)
{
	mLogDebug("run GetWebAudioMuteInner(...)");

	 int SerialID = 2;//串口透传，串口1
	 int  cCmdLength   = 0;
	 char cCmdSend[1024]   = {0};
	 int Flag = 0;
	 //string Cmd[8] = {"<1,2,2,0,0>","<2,2,2,0,0>","<3,2,2,0,0>","<4,2,2,0,0>","<5,2,2,0,0>","<6,2,2,0,0>","<7,2,2,0,0>"};
	 //cCmdLength = strlen(Cmd.c_str());
	 //memcpy(cCmdSend, Cmd.c_str(), strlen(Cmd.c_str()));

	 //<指令类型,操作类型,输入/输出,通道,值>
	  
#if 1		
		sprintf(cCmdSend,"<10,2,%d,0,0>", CtrlType);
#else
		sprintf(cCmdSend,INTERFACE_GET_AUDIO_MUTE_SENDCMD, CtrlType, ChnNum);
#endif		
	 cCmdLength = strlen(cCmdSend);
	 
	 MPSOperationRes opResCode = eMPSResultOK;
ReSend:
	 ResponseCode resCode = _client->GetSerialNew(SerialID, cCmdSend, cCmdLength, opResCode, oResult,cResult);
	 mLogDebug("GetWebAudioAllMuteInner cResult:"<<cResult);
	 if(resCode != eResponseCodeSuccess){
		 mLogError("GetSerialNew(...) error:"<<resCode);
		 return false;
	 }
	 
	 if(!strcmp(cResult, "<error>"))
	{
		if (Flag < 2)
		{
			mLogDebug("GetWebAudioAllMuteInner Send GetSerialNew:"<<Flag);
			Flag++;
			goto ReSend;
		}else{
			mLogError("GetWebAudioAllMuteInner GetSerialNew error:"<<Flag);
			return false;
		}
		
	}
#if 1
	//判断是否获取静音成功
	char *carg[40] = {};
	for (int i = 0; i < 40; i ++)
	{
		 carg[i] = (char *)calloc(1, strlen(cResult)+1);
		 if(NULL == carg[i]) 
		 {
		  mLogError("calloc is error\n");
		  return false;
		}
	 }
	sscanf(cResult, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s",\
	carg[0], carg[1], carg[2], carg[3], carg[4], carg[5], carg[6], carg[7], carg[8], carg[9],\
	carg[10], carg[11], carg[12], carg[13], carg[14], carg[15], carg[16], carg[17], carg[18], carg[19],\
	carg[20], carg[21], carg[22], carg[23], carg[24], carg[25], carg[26], carg[27], carg[28], carg[29], \
	carg[30], carg[31], carg[32], carg[33], carg[34], carg[35], carg[36], carg[37], carg[38], carg[39]);
	
	//sscanf(Result, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s", carg[0], carg[1], carg[2], carg[3], carg[4], carg[5], carg[6], carg[7], carg[8], carg[9]);
	//2:输入
	if(!strcmp(carg[2], "2"))
	{	//判断是否出错
		if(strcmp(carg[27], "ok>"))
	   {
		  mLogError("GetWebAudioAllMuteInner Get MUTE failed !!!");
		  return false;
	   }
	}
	else
	{	//1:输出
		if(strcmp(carg[23], "ok>"))
		{
		   mLogError("GetWebAudioAllMuteInner audio output get MUTE failed !!!");
		  return false;
		}
	}

#endif
	 return true;


}

bool CGetAudioDefultGain::GetAudioAllGainInner(CData& oResult, char* cResult, int ChnNum, int CtrlType)
{
	mLogDebug("run GetAudioAllGainInner(...)");
		
	//获取状态
	int SerialID = 2;//串口透传，串口1
	
	int val = 0;
	int Flag = 0;
	int  cCmdLength   = 0;
	char cCmdSend[1024]   = {0};
	
	//<指令类型,操作类型,输入/输出,通道,值>
	
	sprintf(cCmdSend,"<9,2,%d,0,0>", CtrlType);

	//sprintf(cCmdSend,INTERFACE_GET_AUDIO_GAIN_CONF_SENDCMD, CtrlType, ChnNum, val);

	
	cCmdLength = strlen(cCmdSend);
	
	MPSOperationRes opResCode = eMPSResultOK;
ReSend:	
	//发送cCmdSend信息给服务器
	ResponseCode resCode = _client->GetSerialNew(SerialID, cCmdSend, cCmdLength, opResCode, oResult,cResult);
	//printf("cResult:%s",cResult);
	mLogDebug("GetAudioAllGainInner cResult:"<<cResult);
	if(resCode != eResponseCodeSuccess){
		mLogError("GetConsoleNew(...) error:"<<resCode);
		return false;
	}
		
	 if(!strcmp(cResult, "<error>"))
	{
		if (Flag < 2)
		{
			mLogDebug("GetAudioAllGainInner Send GetSerialNew:"<<Flag);
			Flag++;
			goto ReSend;
		}else{
			mLogError("GetAudioAllGainInner GetSerialNew error:"<<Flag);
			return false;
		}
	}

	//解析
	char *arg[40] = {};
	for (int i = 0; i < 40; i ++)
	{
		arg[i] = (char *)calloc(1, strlen(cResult)+1);
		if(NULL == arg[i]) 
		{
		   mLogError("calloc is error\n");
		   return false;
		}
	}
	
	//mLogDebug("cResult:"<<cResult);
	
	//命令格式cResult: "<9（指令类型）,2（操作类型，1:SET，2:GET）,2（输入2/输出1）,1（通道0~31）,80（值）,ok（状态）>"
	sscanf(cResult, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s",\
	arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7], arg[8], arg[9],\
	arg[10], arg[11], arg[12], arg[13], arg[14], arg[15], arg[16], arg[17], arg[18], arg[19],\
	arg[20], arg[21], arg[22], arg[23], arg[24], arg[25], arg[26], arg[27], arg[28], arg[29], \
	arg[30], arg[31], arg[32], arg[33], arg[34], arg[35], arg[36], arg[37], arg[38], arg[39]);
	//2:输入
	if(!strcmp(arg[2], "2"))
	{	//判断是否出错
		if(strcmp(arg[27], "ok>"))
	   {
		  mLogError("ConvertSTRING2CJSONParseSerialCmdParam Get gain failed !!!\n");
		  return false;
	   }
	}
	else
	{  //1:输出
		if(strcmp(arg[23], "ok>"))
		  {
			 mLogError("audio output get gain failed\n");
			 return false;
		  }
	}
	return true;

}

bool CGetAudioDefultGain::GetAudioDefultGainInner(CData& oResult, char* cResult, int ChnNum, int CtrlType)
{
		mLogDebug("run GetAudioDefultGainInner(...)");
#if 0			
		//获取状态
		int SerialID = 1;//串口透传，串口1
		
		int val = 0;
		int  cCmdLength   = 0;
		char cCmdSend[1024]   = {0};
		
		//<指令类型,操作类型,输入/输出,通道,值>
		
		sprintf(cCmdSend,"<13,2,%d,0,0>", CtrlType);

		//sprintf(cCmdSend,INTERFACE_GET_AUDIO_GAIN_CONF_SENDCMD, CtrlType, ChnNum, val);
	
		
		cCmdLength = strlen(cCmdSend);
		
		MPSOperationRes opResCode = eMPSResultOK;
		//发送cCmdSend信息给服务器
		ResponseCode resCode = _client->GetSerialNew(SerialID, cCmdSend, cCmdLength, opResCode, oResult,cResult);
		//printf("cResult:%s",cResult);
		mLogDebug("GetAudioDefultGainInner cResult:"<<cResult);
		if(resCode != eResponseCodeSuccess){
			mLogError("GetConsoleNew(...) error:"<<resCode);
			return false;
		}
		return true;
#else 
	sqlite3 *reg_db = NULL;

	char cmdbuf[1024];
	char *register_db = "Audio_manage.db";
	STU stu;
	
	reg_db = (sqlite3 *)create_db(register_db); //创建数据库
	create_db_table(reg_db);  //创建注册表
	sprintf(stu.name,"Audio_AllDefultGain%d",CtrlType);		
	find_stu_db(reg_db, &stu, cResult);

	return true;
#endif

}
bool CGetAudioDefultGain::GetWebAudioDefultMuteInner(CData& oResult, char* cResult, int CtrlType,int ChnNum)
{
	mLogDebug("run GetWebAudioMuteInner(...)");
#if 0

	 int SerialID = 1;//串口透传，串口1
	 int  cCmdLength   = 0;
	 char cCmdSend[1024]   = {0};
	 //string Cmd[8] = {"<1,2,2,0,0>","<2,2,2,0,0>","<3,2,2,0,0>","<4,2,2,0,0>","<5,2,2,0,0>","<6,2,2,0,0>","<7,2,2,0,0>"};
	 //cCmdLength = strlen(Cmd.c_str());
	 //memcpy(cCmdSend, Cmd.c_str(), strlen(Cmd.c_str()));

	 //<指令类型,操作类型,输入/输出,通道,值>
	  
		
	sprintf(cCmdSend,"<14,2,%d,0,0>", CtrlType);

	//sprintf(cCmdSend,INTERFACE_GET_AUDIO_MUTE_SENDCMD, CtrlType, ChnNum);
		
	 cCmdLength = strlen(cCmdSend);
	 
	 MPSOperationRes opResCode = eMPSResultOK;
	 ResponseCode resCode = _client->GetSerialNew(SerialID, cCmdSend, cCmdLength, opResCode, oResult,cResult);
	 mLogDebug("GetWebAudioDefultMuteInner cResult:"<<cResult);
	 if(resCode != eResponseCodeSuccess){
		 mLogError("GetSerialNew(...) error:"<<resCode);
		 return false;
	 }
#else
	sqlite3 *reg_db = NULL;

	char cmdbuf[1024];
	char *register_db = "Audio_manage.db";
	STU stu;
	
	reg_db = (sqlite3 *)create_db(register_db); //创建数据库
	create_db_table(reg_db);  //创建注册表
			
	//stu.name = "GetWebAudioDefultMute";
	sprintf(stu.name,"Audio_ALLDefultMute%d",CtrlType);
	find_stu_db(reg_db, &stu, cResult);

#endif
	 return true;


}

InterfaceResCode CGetAudioDefultGain::GetAudioDefultGain(string &sResult)
{
	mLogInfo("获取 GetAudioDefultGain 参数...");
	//cJSON* node;
	char cResult[RES_BUF_MAXLEN] = { 0 };
	char Result[RES_BUF_MAXLEN] = { 0 };
	CData oResult = SVSMAP();
	char *satrt, *end;
	int chn;
	int ChnNumbers;
	int ChnNum[5] = {1,16,28,1,4};
	cJSON *tmp[40];
#if 1
	for(int CtrlType = 1; CtrlType <= 2; CtrlType++)
	{
		//sleep(1);//数据访问频繁，防止音频板处理不了
		memset(cResult, 0 , RES_BUF_MAXLEN);
		memset(Result, 0 , RES_BUF_MAXLEN);
		if (!GetAudioDefultGainInner(oResult, cResult, ChnNum[0], CtrlType))
		{
			mLogError("Failed to run  GetAudioDefultGainInner(...)");
			return eInterfaceResCodeError112;
		}
		//sleep(1);
		if (!GetWebAudioDefultMuteInner(oResult, Result, CtrlType, ChnNum[0]))
		{
			mLogError("Failed to run  GetWebAudioMuteInner(...)");
			return eInterfaceResCodeError113;
		}

		//取出数据
		//satrt = strstr(cResult,"<");
		//end   = strstr(cResult,">");
		//memcpy(cResult , satrt, end-satrt+1);
		mLogInfo("cResult = "<<cResult);

		//satrt = strstr(Result,"<");
		//end   = strstr(Result,">");
		//memcpy(Result , satrt, end-satrt+1);
		mLogInfo("Result = "<<Result);
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
		
		//命令格式cResult: "<9（指令类型）,2（操作类型，1:SET，2:GET）,2（输入2/输出1）,1（通道0~31）,80（值）,ok（状态）>"
		sscanf(cResult, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s",\
		arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7], arg[8], arg[9],\
		arg[10], arg[11], arg[12], arg[13], arg[14], arg[15], arg[16], arg[17], arg[18], arg[19],\
		arg[20], arg[21], arg[22], arg[23], arg[24], arg[25], arg[26], arg[27], arg[28], arg[29], \
		arg[30], arg[31], arg[32], arg[33], arg[34], arg[35], arg[36], arg[37], arg[38], arg[39]);
		//2:输入
		if(!strcmp(arg[2], "2"))
		{	//判断是否出错
			if(strcmp(arg[27], "ok>"))
		   {
			  mLogError("ConvertSTRING2CJSONParseSerialCmdParam Get gain failed !!!\n");
			  return eInterfaceResCodeError114;
		   }
			//输入通道类型及返回值
			ChnNumbers = atoi(arg[3]);
			for(int i = 1; i <= ChnNumbers; i++)
			{
				//cJSON_AddItemToObject(tmp[i], AUDIO_CHN_CONF_PARAMS_CTRLTYPE, cJSON_CreateNumber(2));
				//cJSON_AddItemToObject(tmp[i], AUDIO_CHN_CONF_PARAMS_CHNNUM, cJSON_CreateNumber(i+1));
				//cJSON_AddItemToObject(tmp[i], AUDIO_CHN_CONF_PARAMS_VAL, cJSON_CreateNumber(atoi(arg[i+4])));
				//获取的是默认的5个通道音量，输入的1、16、28，输出的1,4
				switch(i)
				{
					case 1:
					{
						char ctmp[10];
						float Valtmp;
						sprintf(ctmp,"%s",arg[i+3]);
						
						sscanf(ctmp,"%f",&Valtmp);
					//	 mLogDebug("Valtmp:"<<Valtmp);
						int Val = Valtmp*10;//转换为web音量取值范围-720-120

						cJSON_AddItemToObject(tmp[i], AUDIO_CHN_CONF_PARAMS_CTRLTYPE, cJSON_CreateNumber(2));
						cJSON_AddItemToObject(tmp[i], AUDIO_CHN_CONF_PARAMS_CHNNUM, cJSON_CreateNumber(i));
						cJSON_AddItemToObject(tmp[i], AUDIO_CHN_CONF_PARAMS_VAL, cJSON_CreateNumber(Valtmp));
						
						break;
					}
					case 13:
					{
						 char ctmp[10];
						 float Valtmp;
						 sprintf(ctmp,"%s",arg[i+3]);
						
						 sscanf(ctmp,"%f",&Valtmp);
					//	 mLogDebug("Valtmp:"<<Valtmp);
						 int Val = Valtmp*10;//转换为web音量取值范围-720-120
						cJSON_AddItemToObject(tmp[i], AUDIO_CHN_CONF_PARAMS_CTRLTYPE, cJSON_CreateNumber(2));
						cJSON_AddItemToObject(tmp[i], AUDIO_CHN_CONF_PARAMS_CHNNUM, cJSON_CreateNumber(i));
						cJSON_AddItemToObject(tmp[i], AUDIO_CHN_CONF_PARAMS_VAL, cJSON_CreateNumber(Valtmp));
						break;
					}
					case 20:
					{
						 char ctmp[10];
						 float Valtmp;
						 sprintf(ctmp,"%s",arg[i+3]);
						
						 sscanf(ctmp,"%f",&Valtmp);
					//	 mLogDebug("Valtmp:"<<Valtmp);
						 int Val = Valtmp*10;//转换为web音量取值范围-720-120
						cJSON_AddItemToObject(tmp[i], AUDIO_CHN_CONF_PARAMS_CTRLTYPE, cJSON_CreateNumber(2));
						cJSON_AddItemToObject(tmp[i], AUDIO_CHN_CONF_PARAMS_CHNNUM, cJSON_CreateNumber(i));
						cJSON_AddItemToObject(tmp[i], AUDIO_CHN_CONF_PARAMS_VAL, cJSON_CreateNumber(Valtmp));
						break;
					}
				}
			}
		} 
		else
		{  //1:输出
		   if(strcmp(arg[23], "ok>"))
		   {
			  mLogError("audio output get gain failed\n");
			 return eInterfaceResCodeError115;
		   }
		   //输出通道类型及返回值
			ChnNumbers = atoi(arg[3]);
			for(int i = 1; i <= ChnNumbers; i++)
			{
				//cJSON_AddItemToObject(tmp[i], AUDIO_CHN_CONF_PARAMS_CTRLTYPE, cJSON_CreateNumber(1));
				//cJSON_AddItemToObject(tmp[i], AUDIO_CHN_CONF_PARAMS_CHNNUM, cJSON_CreateNumber(i+1));
				//cJSON_AddItemToObject(tmp[i], AUDIO_CHN_CONF_PARAMS_VAL, cJSON_CreateNumber(atoi(arg[i+4])));
				//获取的是默认的5个通道音量，输入的1、16、28，输出的1,4
				switch(i)
				{
					case 1:
					{
						 char ctmp[10];
						 float Valtmp;
						 sprintf(ctmp,"%s",arg[i+3]);
						
						 sscanf(ctmp,"%f",&Valtmp);
						 
					//	 mLogDebug("Valtmp:"<<Valtmp);
						 int Val = Valtmp*10;//转换为web音量取值范围-720-120
						cJSON_AddItemToObject(tmp[i], AUDIO_CHN_CONF_PARAMS_CTRLTYPE, cJSON_CreateNumber(1));
						cJSON_AddItemToObject(tmp[i], AUDIO_CHN_CONF_PARAMS_CHNNUM, cJSON_CreateNumber(i));
						cJSON_AddItemToObject(tmp[i], AUDIO_CHN_CONF_PARAMS_VAL, cJSON_CreateNumber(Valtmp));
						break;
					}
					case 3:
					{
						 char ctmp[10];
						 float Valtmp;
						 sprintf(ctmp,"%s",arg[i+3]);
						
						 sscanf(ctmp,"%f",&Valtmp);
				
					//	 mLogDebug("Valtmp:"<<Valtmp);
						 int Val = Valtmp*10;//转换为web音量取值范围-720-120
						cJSON_AddItemToObject(tmp[i], AUDIO_CHN_CONF_PARAMS_CTRLTYPE, cJSON_CreateNumber(1));
						cJSON_AddItemToObject(tmp[i], AUDIO_CHN_CONF_PARAMS_CHNNUM, cJSON_CreateNumber(i));
						cJSON_AddItemToObject(tmp[i], AUDIO_CHN_CONF_PARAMS_VAL, cJSON_CreateNumber(Valtmp));
						break;
					}
				}
			}

		}
/************************************************************************************************************/
		//判断是否获取静音成功
	 	char *carg[40] = {};
	 	for (int i = 0; i < 40; i ++)
	 	{
			 carg[i] = (char *)calloc(1, strlen(Result)+1);
			 if(NULL == carg[i]) 
			 {
		 	  mLogError("calloc is error\n");
		  	  return eInterfaceResCodeError;
		  	}
	 	 }
		sscanf(Result, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s",\
		carg[0], carg[1], carg[2], carg[3], carg[4], carg[5], carg[6], carg[7], carg[8], carg[9],\
		carg[10], carg[11], carg[12], carg[13], carg[14], carg[15], carg[16], carg[17], carg[18], carg[19],\
		carg[20], carg[21], carg[22], carg[23], carg[24], carg[25], carg[26], carg[27], carg[28], carg[29], \
		carg[30], carg[31], carg[32], carg[33], carg[34], carg[35], carg[36], carg[37], carg[38], carg[39]);

		//sscanf(Result, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s", carg[0], carg[1], carg[2], carg[3], carg[4], carg[5], carg[6], carg[7], carg[8], carg[9]);
		//2:输入
		if(!strcmp(carg[2], "2"))
		{	//判断是否出错
			if(strcmp(carg[27], "ok>"))
		   {
			  mLogError("ConvertSTRING2CJSONParseSerialCmdParam Get MUTE failed !!!\n");
			  return eInterfaceResCodeError116;
		   }
			//输入通道类型及返回值
			ChnNumbers = atoi(arg[3]);
			for(int i = 1; i <= ChnNumbers; i++)
			{
				//cJSON_AddItemToObject(tmp[i], "MuteVal", cJSON_CreateNumber(atoi(carg[i+4])));
				int Mute;
				switch(i)
				{
					case 1:
					
						if(!strcmp(carg[i+3],"true"))
						{
							Mute = 1; 
						}
						else if(!strcmp(carg[i+3],"false"))
						{
							Mute = 0;
						}

						cJSON_AddItemToObject(tmp[i], "MuteVal", cJSON_CreateNumber(Mute));
						break;
					case 13:
					
						if(!strcmp(carg[i+3],"true"))
						{
							Mute = 1; 
						}
						else if(!strcmp(carg[i+3],"false"))
						{
							Mute = 0;
						}

						cJSON_AddItemToObject(tmp[i], "MuteVal", cJSON_CreateNumber(Mute));
						break;
					case 20:
						if(!strcmp(carg[i+3],"true"))
						{
							Mute = 1; 
						}
						else if(!strcmp(carg[i+3],"false"))
						{
							Mute = 0;
						}

						cJSON_AddItemToObject(tmp[i], "MuteVal", cJSON_CreateNumber(Mute));
						break;
				}			
			}

		} 
		else
		{  //1:输出
		   if(strcmp(carg[23], "ok>"))
		   {
			  mLogError("audio output get MUTE failed\n");
			 return eInterfaceResCodeError117;
		   }
		    //chn = atoi(carg[3]);
		    //输出通道类型及返回值
		   	ChnNumbers = atoi(arg[3]);
			for(int i = 1; i <= ChnNumbers; i++)
			{
				int Mute;
				switch(i)
				{
					case 1:
					
						if(!strcmp(carg[i+3],"true"))
						{
							Mute = 1; 
						}
						else if(!strcmp(carg[i+3],"false"))
						{
							Mute = 0;
						}

//						cJSON_AddItemToObject(tmp[i], "MuteVal", cJSON_CreateNumber(atoi(carg[i+3])));
						cJSON_AddItemToObject(tmp[i], "MuteVal", cJSON_CreateNumber(Mute));
						break;
					case 3:
						
						if(!strcmp(carg[i+3],"true"))
						{
							Mute = 1; 
						}
						else if(!strcmp(carg[i+3],"false"))
						{
							Mute = 0;
						}

						cJSON_AddItemToObject(tmp[i], "MuteVal", cJSON_CreateNumber(Mute));
						break;
				}

			}
		}

		if(CtrlType == 1)
		{
			cJSON_AddItemToArray(_addressList, tmp[1]);
			cJSON_AddItemToArray(_addressList, tmp[3]);
		}else{
			cJSON_AddItemToArray(_addressList, tmp[1]);
			cJSON_AddItemToArray(_addressList, tmp[13]);
			cJSON_AddItemToArray(_addressList, tmp[20]);
		}


	}

#else

#endif
	cJSON_AddItemToObject(_result, "AudioGainList", _addressList);
	//3. 将step2中获取的响应格式化为Json字符串
	sResult = ComposeResult();
	cJSON_Delete(_result);

	return eInterfaceResCodeSuccess;

}


