#include "SetWebAudioConf.h"
#include "MPSClient.h"

CSetWebAudioConf::CSetWebAudioConf(const std::string& ip, unsigned short port, int timeOut)
	:_mpsClient(new MPSClient(ip, port, timeOut))
	,_result(cJSON_CreateObject())
	,_addressList(cJSON_CreateArray())
{

}

CSetWebAudioConf::~CSetWebAudioConf()
{
	
}

bool CSetWebAudioConf::ComposeResult(cJSON* result)
{
// 	if (_result == NULL || result == NULL) {
// 		mLogError("cJSON result == NULL.");
// 		result = NULL;
// 		return false;
// 	}

	return true;
}

std::string CSetWebAudioConf::ComposeResult()
{
// 	cJSON* result = cJSON_CreateObject();
// 	if (ComposeResult(result) == false) {
// 		cJSON_Delete(result);
// 		return "";
// 	}
// 	else {
// 		char* cRes = cJSON_Print(_result);
// 		std::string res = cRes;
// 		cJSON_Delete(result);
// 		return res;
// 	}

	//assume the setmethod's response code is always okay. -_-!
	char* cRes = cJSON_Print(_result);
	std::string res = cRes;
	return res;
}


InterfaceResCode CSetWebAudioConf::SetWebAudioConf(std::string& sResult, int isPower, int IsANS,
	int isIncentives, int isFeedBack, int isAEC, int isAGC, int spl)
{
	mLogInfo("设置 SetWebAudioConf...");
	int cmd,val;
	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };
	//cmd从3开始是因为灵敏度和幻象电源采用单通道获取方式，此处去掉获取灵敏度和幻象电源
	 for(cmd = 4 ;cmd <= 6; cmd++)
	{
		if (!SetWebAudioConfInner(cmd,val,isPower, IsANS, isIncentives, isFeedBack, isAEC, isAGC, spl, oResult, cResult))
		{
			mLogError("SetWebAudioConfInner failed !!!");
			return eInterfaceResCodeError;
		}
		
	 	if (!_analyzePara.ConvertSTRING2CJSONParseSerialCmdParam(oResult, cResult, _result, _addressList,NULL))
		{
	 		 mLogError("Failed to run	ConvertSTRING2CJSONParseSerialCmdParam(...)");
	  		 return eInterfaceResCodeError;
	 	}
		memset(cResult,0,RES_BUF_MAXLEN);
		
	}
#if 0
		//解析响应, 默认不处理
	if (!_analyzePara.ConvertSTRING2CJSONSetWebAudioConf(oResult, cResult)) {
		mLogError("Failed to run  ConvertSTRING2CJSONSetWebAudioConf(...)");
		return eInterfaceResCodeError;
	}
#endif
	sResult = ComposeResult();
	return eInterfaceResCodeSuccess;
}

bool CSetWebAudioConf::SetWebAudioConfInner(int cmd,int val,int isPower, int IsANS,int isIncentives,
	int isFeedBack, int isAEC, int isAGC,
	int spl, CData& oResult, char* cResult)
{
	mLogDebug("run SetWebAudioConfInner(...)");

	//获取状态
	int SerialID = 2;//串口透传，串口1
	int  cCmdLength   = 0;
	char cCmdSend[1024]   = {0};
	//cCmdLength = strlen(Cmd.c_str());
	//memcpy(cCmdSend, Cmd.c_str(), strlen(Cmd.c_str()));
//	float Valtmp = Val;
//	float GainVal = Valtmp/10;
	switch(cmd)
	{
		case 1:
			val = spl;
			break;
		case 2:
			val = isPower;
			break;
		case 3:
			val = isAGC;
			break;
		case 4:
			val = isFeedBack;
			break;
		case 5:
			val = isAEC;
			break;
		case 6:
			val = IsANS;
			break;
		case 7:
			cmd = 11;
			val = isIncentives;
			break;
		default:
			mLogError("failed SetWebAudioConfInner(...)");
			return false;

	}
	 char Mute[10];
	if(val == 1)
	{
		sprintf(Mute,"true"); 

	}else{
		sprintf(Mute,"false"); 

	}

	//<指令类型,操作类型,输入/输出,通道,值>
	sprintf( cCmdSend, "<%d,1,2,0,%s>", cmd, Mute); 
	cCmdLength = strlen(cCmdSend);
	
	MPSOperationRes opResCode = eMPSResultOK;
	//发送cCmdSend信息给服务器
	ResponseCode resCode = _mpsClient->GetSerialNew(SerialID, cCmdSend, cCmdLength, opResCode, oResult,cResult);
	if(resCode != eResponseCodeSuccess){
		mLogError("GetConsoleNew(...) error:"<<resCode);
		return false;
	}
	return true;

}
InterfaceResCode CSetWebAudioConf::GetWebAudioSPL(std::string& sResult)
{
	mLogInfo("GetWebAudioSPL");
	int ChnNum,val;
	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };
	char Result[RES_BUF_MAXLEN] = { 0 };
	cJSON *tmp[20];
	 for(ChnNum = 0 ;ChnNum <= 0; ChnNum++)
	{
		for (int i = 0; i < 20; i ++)
		{

			_param[i] = cJSON_CreateObject();

		}

		if (!GetWebAudioSPLInner( oResult, cResult, ChnNum))
		{
			mLogError("GetWebAudioSPLInner failed !!!");
			return eInterfaceResCodeError;
		}
		if (!GetWebAudioPowerInner( oResult, Result, ChnNum))
		{
			mLogError("GetWebAudioPowerInner failed !!!");
			return eInterfaceResCodeError;
		}
		if (!GetWebAudioAGCInner( oResult, cResult, ChnNum))
		{
			mLogError("GetWebAudioSPLInner failed !!!");
			return eInterfaceResCodeError;
		}
		if (!GetWebAudioIncentivesInner( oResult, Result, ChnNum))
		{
			mLogError("GetWebAudioPowerInner failed !!!");
			return eInterfaceResCodeError;
		}
		if (!GetWebAudioMotivationValInner( oResult, cResult, ChnNum))
		{
			mLogError("GetWebAudioSPLInner failed !!!");
			return eInterfaceResCodeError;
		}
//		if (!GetWebAudioMotivationTimeInner( oResult, Result, ChnNum))
//		{
//			mLogError("GetWebAudioPowerInner failed !!!");
//			return eInterfaceResCodeError;
//		}

//		char *carg[40] = {};
//		for (int i = 0; i < 40; i ++)
//		{
//			 carg[i] = (char *)calloc(1, strlen(cResult)+1);
//			 if(NULL == carg[i]) 
//			 {
//			  mLogError("calloc is error\n");
//			  return false;
//			}
//		 }
//		sscanf(cResult, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s",\
//		carg[0], carg[1], carg[2], carg[3], carg[4], carg[5], carg[6], carg[7], carg[8], carg[9],\
//		carg[10], carg[11], carg[12], carg[13], carg[14], carg[15], carg[16], carg[17], carg[18], carg[19],\
//		carg[20], carg[21], carg[22], carg[23], carg[24], carg[25], carg[26], carg[27], carg[28], carg[29], \
//		carg[30], carg[31], carg[32], carg[33], carg[34], carg[35], carg[36], carg[37], carg[38], carg[39]);

//		tmp[ChnNum] = cJSON_CreateObject();
//		cJSON_AddItemToObject(tmp[ChnNum], "ChnNum", cJSON_CreateNumber(atoi(arg[3])+1));
//		cJSON_AddItemToObject(tmp[ChnNum], "SPL", cJSON_CreateNumber(atoi(arg[4])));
//		cJSON_AddItemToObject(tmp[ChnNum], "IsPower", cJSON_CreateNumber(atoi(carg[4])));
		
	}
	 for (int i = 1; i <= 14; i ++)
	{

		cJSON_AddItemToArray(_addressList, _param[i]);

	}
	cJSON_AddItemToObject(_result, "List", _addressList);
	sResult = ComposeResult();
	cJSON_Delete(_result);
	return eInterfaceResCodeSuccess;
}

bool CSetWebAudioConf::GetWebAudioSPLInner(CData& oResult, char* cResult, int ChnNum)
{
	mLogDebug("run GetWebAudioSPLInner(...)");
	sqlite3 *reg_db = NULL;

	char cmdbuf[1024];
	char *register_db = "Audio_manage.db";
	STU stu;
	
	reg_db = (sqlite3 *)create_db(register_db); //创建数据库
	create_db_table(reg_db);  //创建注册表
			
	//stu.name = "GetWebAudioDefultMute";
	sprintf(stu.name,"Audio_MICInPutSPL%d",ChnNum);
	find_stu_db(reg_db, &stu, cResult);
	
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
	for (int i = 1; i <= 12; i ++)
	{
		if(i == 12)
		{
			cJSON_AddItemToObject(_param[i+1], "ChnNum", cJSON_CreateNumber(i+1));
			cJSON_AddItemToObject(_param[i+2], "ChnNum", cJSON_CreateNumber(i+2));
			cJSON_AddItemToObject(_param[i+1], "SPL", cJSON_CreateNumber(atoi(carg[i+3])));
			cJSON_AddItemToObject(_param[i+2], "SPL", cJSON_CreateNumber(atoi(carg[i+3])));
		}
		cJSON_AddItemToObject(_param[i], "ChnNum", cJSON_CreateNumber(i));
		cJSON_AddItemToObject(_param[i], "SPL", cJSON_CreateNumber(atoi(carg[i+3])));
	}

	return true;

}
InterfaceResCode CSetWebAudioConf::GetWebAudioPower(std::string& sResult)
{
	mLogInfo("GetWebAudioPower");
	int ChnNum,val;
	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };
	cJSON *tmp[15];
	 for(ChnNum = 0 ;ChnNum <= 0; ChnNum++)
	{
		if (!GetWebAudioPowerInner( oResult, cResult, ChnNum))
		{
			mLogError("GetWebAudioPowerInner failed !!!");
			return eInterfaceResCodeError;
		}
		
		char *arg[MAX_ARG_NUM] = {};
		for (int i = 0; i < MAX_ARG_NUM; i ++)
		{
			arg[i] = (char *)calloc(1, strlen(cResult)+1);
			if(NULL == arg[i]) 
			{
			  mLogError("calloc is error\n");
			  return eInterfaceResCodeError;
			 }
		 }
		sscanf(cResult, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s", arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7], arg[8], arg[9]);

		tmp[ChnNum] = cJSON_CreateObject();
		cJSON_AddItemToObject(tmp[ChnNum], "ChnNum", cJSON_CreateNumber(atoi(arg[3])+1));
		cJSON_AddItemToObject(tmp[ChnNum], "IsPower", cJSON_CreateNumber(atoi(arg[4])));
		cJSON_AddItemToArray(_addressList, tmp[ChnNum]);
	}
	cJSON_AddItemToObject(_result, "List", _addressList);

	sResult = ComposeResult();
	cJSON_Delete(_result);
	return eInterfaceResCodeSuccess;
}

bool CSetWebAudioConf::GetWebAudioPowerInner(CData& oResult, char* cResult, int ChnNum)
{
	mLogDebug("run GetWebAudioPowerInner(...)");
	sqlite3 *reg_db = NULL;

	char cmdbuf[1024];
	char *register_db = "Audio_manage.db";
	STU stu;
	
	reg_db = (sqlite3 *)create_db(register_db); //创建数据库
	create_db_table(reg_db);  //创建注册表
			
	//stu.name = "GetWebAudioDefultMute";
	sprintf(stu.name,"Audio_MICInPutPower%d",ChnNum);
	find_stu_db(reg_db, &stu, cResult);
	
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
	for (int i = 1; i <= 12; i ++)
	{
		int Mute;
		if(!strcmp(carg[i+3],"true"))
		{
			Mute = 1; 
		}
		else if(!strcmp(carg[i+3],"false"))
		{
			Mute = 0;
		}
		
		if(i == 12)
		{
			cJSON_AddItemToObject(_param[i+1], "IsPower", cJSON_CreateNumber(Mute));
			cJSON_AddItemToObject(_param[i+2], "IsPower", cJSON_CreateNumber(Mute));
		}
		cJSON_AddItemToObject(_param[i], "IsPower", cJSON_CreateNumber(Mute));
	}

	return true;

}
bool CSetWebAudioConf::GetWebAudioMotivationValInner(CData& oResult, char* cResult, int ChnNum)
{
	mLogDebug("run GetWebAudioMotivationValInner(...)");
	sqlite3 *reg_db = NULL;

	char cmdbuf[1024];
	char *register_db = "Audio_manage.db";
	STU stu;
	
	reg_db = (sqlite3 *)create_db(register_db); //创建数据库
	create_db_table(reg_db);  //创建注册表
			
	//stu.name = "GetWebAudioDefultMute";
	sprintf(stu.name,"Audio_MotivationVal%d",ChnNum);
	find_stu_db(reg_db, &stu, cResult);
	
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
	for (int i = 1; i <= 12; i ++)
	{
		if(i == 12)
		{
			cJSON_AddItemToObject(_param[i+1], "MotVal", cJSON_CreateNumber(atoi(carg[i+3])));
			cJSON_AddItemToObject(_param[i+2], "MotVal", cJSON_CreateNumber(atoi(carg[i+3])));
		}
		cJSON_AddItemToObject(_param[i], "MotVal", cJSON_CreateNumber(atoi(carg[i+3])));
	}

	return true;

}
bool CSetWebAudioConf::GetWebAudioAGCInner(CData& oResult, char* cResult, int ChnNum)
{
	mLogDebug("run GetWebAudioAGCInner(...)");
	sqlite3 *reg_db = NULL;

	char cmdbuf[1024];
	char *register_db = "Audio_manage.db";
	STU stu;
	
	reg_db = (sqlite3 *)create_db(register_db); //创建数据库
	create_db_table(reg_db);  //创建注册表
			
	//stu.name = "GetWebAudioDefultMute";
	sprintf(stu.name,"Audio_IsAGC");
	find_stu_db(reg_db, &stu, cResult);
	
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
	for (int i = 1; i <= 12; i ++)
	{
		int Mute;
		if(!strcmp(carg[i+3],"true"))
		{
			Mute = 0; 
		}
		else if(!strcmp(carg[i+3],"false"))
		{
			Mute = 1;
		}
		if(i == 12)
		{
			cJSON_AddItemToObject(_param[i+1], "IsAGC", cJSON_CreateNumber(Mute));
			cJSON_AddItemToObject(_param[i+2], "IsAGC", cJSON_CreateNumber(Mute));
		}
		cJSON_AddItemToObject(_param[i], "IsAGC", cJSON_CreateNumber(Mute));
	}

	return true;

}
bool CSetWebAudioConf::GetWebAudioIncentivesInner(CData& oResult, char* cResult, int ChnNum)
{
	mLogDebug("run GetWebAudioIncentivesInner(...)");
	sqlite3 *reg_db = NULL;

	char cmdbuf[1024];
	char *register_db = "Audio_manage.db";
	STU stu;
	
	reg_db = (sqlite3 *)create_db(register_db); //创建数据库
	create_db_table(reg_db);  //创建注册表
			
	//stu.name = "GetWebAudioDefultMute";
	sprintf(stu.name,"Audio_IsIncentives");
	find_stu_db(reg_db, &stu, cResult);
	
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
	for (int i = 1; i <= 12; i ++)
	{
		int Mute;
		if(!strcmp(carg[i+3],"true"))
		{
			Mute = 1; 
		}
		else if(!strcmp(carg[i+3],"false"))
		{
			Mute = 0;
		}

		if(i == 12)
		{
			cJSON_AddItemToObject(_param[i+1], "IsIncentives", cJSON_CreateNumber(Mute));
			cJSON_AddItemToObject(_param[i+2], "IsIncentives", cJSON_CreateNumber(Mute));
		}
		cJSON_AddItemToObject(_param[i], "IsIncentives", cJSON_CreateNumber(Mute));
	}

	return true;

}
bool CSetWebAudioConf::GetWebAudioMotivationTimeInner(CData& oResult, char* cResult, int ChnNum)
{
	mLogDebug("run GetWebAudioMotivationTimeInner(...)");
	sqlite3 *reg_db = NULL;

	char cmdbuf[1024];
	char *register_db = "Audio_manage.db";
	STU stu;
	
	reg_db = (sqlite3 *)create_db(register_db); //创建数据库
	create_db_table(reg_db);  //创建注册表
			
	//stu.name = "GetWebAudioDefultMute";
	sprintf(stu.name,"Audio_MotivationTime%d",ChnNum);
	find_stu_db(reg_db, &stu, cResult);
	
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
	for (int i = 1; i <= 12; i ++)
	{
		if(i == 12)
		{
			cJSON_AddItemToObject(_param[i+1], "MotTime", cJSON_CreateNumber(atoi(carg[4])));
			cJSON_AddItemToObject(_param[i+2], "MotTime", cJSON_CreateNumber(atoi(carg[4])));
		}
		cJSON_AddItemToObject(_param[i], "MotTime", cJSON_CreateNumber(atoi(carg[4])));
	}

	return true;

}

InterfaceResCode CSetWebAudioConf::SetWebAudioSPL(std::string& sResult, int ChnNum, int SPL)
{
	mLogInfo("SetWebAudioSPL");
	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };
	if(ChnNum == 13||ChnNum == 14)
	{
		ChnNum = 12;
	}
		
	if (!SetWebAudioSPLInner( ChnNum, SPL, oResult, cResult))
	{
		mLogError("SetWebAudioSPLInner failed !!!");
		return eInterfaceResCodeError;
	}
	
	sqlite3 *reg_db = NULL;
	char DataBuf[1024];
	char *buf;
	char *register_db = "Audio_manage.db";
	 
	STU stu;
	time_t cur_time = time(NULL);
	struct tm *tm_ptr;
	tm_ptr = localtime(&cur_time);
	reg_db = (sqlite3 *)create_db(register_db); //创建数据库
	create_db_table(reg_db);  //创建注册表
	//解析数据
	char *arg[MAX_ARG_NUM] = {};
	for (int i = 0; i < MAX_ARG_NUM; i ++)
	{
		arg[i] = (char *)calloc(1, strlen(cResult)+1);
		if(NULL == arg[i]) 
		{
		  mLogError("calloc is error\n");
		  return eInterfaceResCodeError;
		 }
	 }
	sscanf(cResult, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s", arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7], arg[8], arg[9]);
	//获取数据库数据修改并更新
//	if(!strcmp(arg[2], "2"))//2:输入
//	{
	
		if(!strcmp(arg[5], "ok>"))
		{
	
//			arg[1] = "2";//设置为获取类型，以便获取操作直接读取
//			sprintf(DataBuf,"%s", arg[0]);
//			int chnnum = atoi(arg[3])+1;//音频通道默认从0开始
//			for(int i = 1; i <= 5; i++)
//			{					
//				buf = DataBuf + strlen(DataBuf);
//				sprintf(buf,",%s", arg[i]); 				
//			}
//			
//			stu.id = chnnum + 39;
//			sprintf(stu.val,DataBuf);
//			sprintf(stu.name,"Audio_MICInPutSPL%d",chnnum);
//			sprintf(stu.time, "%04d/%02d/%02d-%02d:%02d:%02d", tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
//			update_stu_db(reg_db, &stu); //add 到数据库 

			
			sprintf(stu.name,"Audio_MICInPutSPL0");
			find_stu_db(reg_db, &stu, DataBuf);
			//解析
			char *carg[40] = {};
			for (int i = 0; i < 40; i ++)
			{
				 carg[i] = (char *)calloc(1, strlen(DataBuf)+1);
				 if(NULL == carg[i]) 
				 {
				  mLogError("calloc is error\n");
				  return eInterfaceResCodeError;
				}
			 }
			sscanf(DataBuf, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s",\
			carg[0], carg[1], carg[2], carg[3], carg[4], carg[5], carg[6], carg[7], carg[8], carg[9],\
			carg[10], carg[11], carg[12], carg[13], carg[14], carg[15], carg[16], carg[17], carg[18], carg[19],\
			carg[20], carg[21], carg[22], carg[23], carg[24], carg[25], carg[26], carg[27], carg[28], carg[29], \
			carg[30], carg[31], carg[32], carg[33], carg[34], carg[35], carg[36], carg[37], carg[38], carg[39]);
			//修改相应通道的值
			int chn = atoi(arg[3]) + 3;
			carg[chn] = arg[4];
			//36个
			sprintf(DataBuf,"%s", carg[0]);
			for(int i = 1; i <= 16; i++)
			{					
				buf = DataBuf + strlen(DataBuf);
			
				sprintf(buf,",%s", carg[i]);
				
			}
			mLogDebug("DataBuf = "<<DataBuf);
			//更新数据
			stu.id = 12;
			sprintf(stu.val,DataBuf);
			sprintf(stu.name,"Audio_MICInPutSPL0");
			sprintf(stu.time, "%04d/%02d/%02d-%02d:%02d:%02d", tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
			update_stu_db(reg_db, &stu); //add 到数据库 
			
			mLogDebug("Set SPL Success !!!\n");
			return eInterfaceResCodeSuccess;
		}
		else
		{
			mLogError("SetWebAudioSPL failed");
			return eInterfaceResCodeError;
		}
//	}

	sResult = ComposeResult();
	cJSON_Delete(_result);
	return eInterfaceResCodeSuccess;
}

bool CSetWebAudioConf::SetWebAudioSPLInner(int ChnNum, int SPL, CData& oResult, char* cResult)
{
	mLogDebug("run SetWebAudioConfInner(...)");

	//获取状态
	int SerialID = 2;//串口透传，串口1
	int  cCmdLength   = 0;
	char cCmdSend[1024]   = {0};

	sprintf( cCmdSend, "<1,1,2,%d,%d>", ChnNum, SPL); 
	cCmdLength = strlen(cCmdSend);
	
	MPSOperationRes opResCode = eMPSResultOK;
	//发送cCmdSend信息给服务器
	ResponseCode resCode = _mpsClient->GetSerialNew(SerialID, cCmdSend, cCmdLength, opResCode, oResult,cResult);
	if(resCode != eResponseCodeSuccess){
		mLogError("GetConsoleNew(...) error:"<<resCode);
		return false;
	}
	
	return true;

}
InterfaceResCode CSetWebAudioConf::SetWebAudioPower(std::string& sResult,int ChnNum,int IsPower)
{
	mLogInfo("SetWebAudioPower...");

	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };
	if(ChnNum == 13||ChnNum == 14)
	{
		ChnNum = 12;
	}

	if (!SetWebAudioPowerInner( ChnNum, IsPower, oResult, cResult))
	{
		mLogError("SetWebAudioPowerInner failed !!!");
		return eInterfaceResCodeError;
	}
	
	sqlite3 *reg_db = NULL;
	char DataBuf[1024];
	char *buf;
	char *register_db = "Audio_manage.db";
	 
	STU stu;
	time_t cur_time = time(NULL);
	struct tm *tm_ptr;
	tm_ptr = localtime(&cur_time);
	reg_db = (sqlite3 *)create_db(register_db); //创建数据库
	create_db_table(reg_db);  //创建注册表
	//解析数据
	char *arg[MAX_ARG_NUM] = {};
	for (int i = 0; i < MAX_ARG_NUM; i ++)
	{
		arg[i] = (char *)calloc(1, strlen(cResult)+1);
		if(NULL == arg[i]) 
		{
		  mLogError("calloc is error\n");
		  return eInterfaceResCodeError;
		 }
	 }
	sscanf(cResult, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s", arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7], arg[8], arg[9]);
	//获取数据库数据修改并更新

	
		if(!strcmp(arg[5], "ok>"))
		{
	
//			arg[1] = "2";//设置为获取类型，以便获取操作直接读取
//			sprintf(DataBuf,"%s", arg[0]);
//			int chnnum = atoi(arg[3]) + 1;//音频通道默认从0开始
//			for(int i = 1; i <= 5; i++)
//			{					
//				buf = DataBuf + strlen(DataBuf);
//				sprintf(buf,",%s", arg[i]); 				
//			}
//			
//			stu.id = chnnum + 53;
//			sprintf(stu.val,DataBuf);
//			sprintf(stu.name,"Audio_MICInPutPower%d",chnnum);
//			sprintf(stu.time, "%04d/%02d/%02d-%02d:%02d:%02d", tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
//			update_stu_db(reg_db, &stu); //add 到数据库 
			
			sprintf(stu.name,"Audio_MICInPutPower0");
			find_stu_db(reg_db, &stu, DataBuf);
			//解析
			char *carg[40] = {};
			for (int i = 0; i < 40; i ++)
			{
				 carg[i] = (char *)calloc(1, strlen(DataBuf)+1);
				 if(NULL == carg[i]) 
				 {
				  mLogError("calloc is error\n");
				  return eInterfaceResCodeError;
				}
			 }
			sscanf(DataBuf, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s",\
			carg[0], carg[1], carg[2], carg[3], carg[4], carg[5], carg[6], carg[7], carg[8], carg[9],\
			carg[10], carg[11], carg[12], carg[13], carg[14], carg[15], carg[16], carg[17], carg[18], carg[19],\
			carg[20], carg[21], carg[22], carg[23], carg[24], carg[25], carg[26], carg[27], carg[28], carg[29], \
			carg[30], carg[31], carg[32], carg[33], carg[34], carg[35], carg[36], carg[37], carg[38], carg[39]);
			//修改相应通道的值
			int chn = atoi(arg[3]) + 3;
			carg[chn] = arg[4];
			//36个
			sprintf(DataBuf,"%s", carg[0]);
			for(int i = 1; i <= 16; i++)
			{					
				buf = DataBuf + strlen(DataBuf);
			
				sprintf(buf,",%s", carg[i]);
				
			}
			mLogDebug("DataBuf = "<<DataBuf);
			//更新数据
			stu.id = 13;
			sprintf(stu.val,DataBuf);
			sprintf(stu.name,"Audio_MICInPutPower0");
			sprintf(stu.time, "%04d/%02d/%02d-%02d:%02d:%02d", tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
			update_stu_db(reg_db, &stu); //add 到数据库 
			
			mLogDebug("Set Power Success !!!\n");
			return eInterfaceResCodeSuccess;
		}
		else
		{
			mLogError("SetWebAudioPower failed");
			return eInterfaceResCodeError;
		}


	sResult = ComposeResult();
	cJSON_Delete(_result);
	return eInterfaceResCodeSuccess;
}

bool CSetWebAudioConf::SetWebAudioPowerInner(int ChnNum, int IsPower, CData& oResult, char* cResult)
{
	mLogDebug("run SetWebAudioConfInner(...)");

	//获取状态
	int SerialID = 2;//串口透传，串口1
	int  cCmdLength   = 0;
	char cCmdSend[1024]   = {0};
	char Mute[10];
	if(IsPower == 1)
	{
		sprintf(Mute,"true"); 

	}else{
		sprintf(Mute,"false"); 

	}
	//<指令类型,操作类型,输入/输出,通道,值>
	sprintf( cCmdSend, "<2,1,2,%d,%s>", ChnNum, Mute); 
	cCmdLength = strlen(cCmdSend);
	
	MPSOperationRes opResCode = eMPSResultOK;
	//发送cCmdSend信息给服务器
	ResponseCode resCode = _mpsClient->GetSerialNew(SerialID, cCmdSend, cCmdLength, opResCode, oResult,cResult);
	if(resCode != eResponseCodeSuccess){
		mLogError("GetConsoleNew(...) error:"<<resCode);
		return false;
	}
	return true;

}

InterfaceResCode CSetWebAudioConf::SetWebAudioMotVal(std::string& sResult, int ChnNum, int MotVal)
{
	mLogInfo("SetWebAudioMotVal");
	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };
	if(ChnNum == 13||ChnNum == 14)
	{
		ChnNum = 12;
	}
		
	if (!SetWebAudioMotValInner( ChnNum, MotVal, oResult, cResult))
	{
		mLogError("SetWebAudioMotValInner failed !!!");
		return eInterfaceResCodeError;
	}
	
	sqlite3 *reg_db = NULL;
	char DataBuf[1024];
	char *buf;
	char *register_db = "Audio_manage.db";
	 
	STU stu;
	time_t cur_time = time(NULL);
	struct tm *tm_ptr;
	tm_ptr = localtime(&cur_time);
	reg_db = (sqlite3 *)create_db(register_db); //创建数据库
	create_db_table(reg_db);  //创建注册表
	//解析数据
	char *arg[MAX_ARG_NUM] = {};
	for (int i = 0; i < MAX_ARG_NUM; i ++)
	{
		arg[i] = (char *)calloc(1, strlen(cResult)+1);
		if(NULL == arg[i]) 
		{
		  mLogError("calloc is error\n");
		  return eInterfaceResCodeError;
		 }
	 }
	sscanf(cResult, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s", arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7], arg[8], arg[9]);
	//获取数据库数据修改并更新
	
		if(!strcmp(arg[5], "ok>"))
		{
	
//			arg[1] = "2";//设置为获取类型，以便获取操作直接读取
//			sprintf(DataBuf,"%s", arg[0]);
//			int chnnum = atoi(arg[3])+1;//音频通道默认从0开始
//			for(int i = 1; i <= 5; i++)
//			{					
//				buf = DataBuf + strlen(DataBuf);
//				sprintf(buf,",%s", arg[i]); 				
//			}
//			
//			stu.id = chnnum + 39;
//			sprintf(stu.val,DataBuf);
//			sprintf(stu.name,"Audio_MICInPutSPL%d",chnnum);
//			sprintf(stu.time, "%04d/%02d/%02d-%02d:%02d:%02d", tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
//			update_stu_db(reg_db, &stu); //add 到数据库 

			
			sprintf(stu.name,"Audio_MotivationVal0");
			find_stu_db(reg_db, &stu, DataBuf);
			//解析
			char *carg[40] = {};
			for (int i = 0; i < 40; i ++)
			{
				 carg[i] = (char *)calloc(1, strlen(DataBuf)+1);
				 if(NULL == carg[i]) 
				 {
				  mLogError("calloc is error\n");
				  return eInterfaceResCodeError;
				}
			 }
			sscanf(DataBuf, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s",\
			carg[0], carg[1], carg[2], carg[3], carg[4], carg[5], carg[6], carg[7], carg[8], carg[9],\
			carg[10], carg[11], carg[12], carg[13], carg[14], carg[15], carg[16], carg[17], carg[18], carg[19],\
			carg[20], carg[21], carg[22], carg[23], carg[24], carg[25], carg[26], carg[27], carg[28], carg[29], \
			carg[30], carg[31], carg[32], carg[33], carg[34], carg[35], carg[36], carg[37], carg[38], carg[39]);
			//修改相应通道的值
			int chn = atoi(arg[3]) + 3;
			carg[chn] = arg[4];
			//36个
			sprintf(DataBuf,"%s", carg[0]);
			for(int i = 1; i <= 16; i++)
			{					
				buf = DataBuf + strlen(DataBuf);
			
				sprintf(buf,",%s", carg[i]);
				
			}
			mLogDebug("DataBuf = "<<DataBuf);
			//更新数据
			stu.id = 14;
			sprintf(stu.val,DataBuf);
			sprintf(stu.name,"Audio_MotivationVal0");
			sprintf(stu.time, "%04d/%02d/%02d-%02d:%02d:%02d", tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
			update_stu_db(reg_db, &stu); //add 到数据库 
			
			mLogDebug("Set SPL Success !!!\n");
			return eInterfaceResCodeSuccess;
		}
		else
		{
			mLogError("SetWebAudioSPL failed");
			return eInterfaceResCodeError;
		}
	

	sResult = ComposeResult();
	cJSON_Delete(_result);
	return eInterfaceResCodeSuccess;
}

bool CSetWebAudioConf::SetWebAudioMotValInner(int ChnNum, int MotVal, CData& oResult, char* cResult)
{
	mLogDebug("run SetWebAudioConfInner(...)");

	//获取状态
	int SerialID = 2;//串口透传，串口1
	int  cCmdLength   = 0;
	char cCmdSend[1024]   = {0};

	sprintf( cCmdSend, "<14,1,1,%d,%d>", ChnNum, MotVal); 
	cCmdLength = strlen(cCmdSend);
	
	MPSOperationRes opResCode = eMPSResultOK;
	//发送cCmdSend信息给服务器
	ResponseCode resCode = _mpsClient->GetSerialNew(SerialID, cCmdSend, cCmdLength, opResCode, oResult,cResult);
	if(resCode != eResponseCodeSuccess){
		mLogError("GetConsoleNew(...) error:"<<resCode);
		return false;
	}
	
	return true;

}
InterfaceResCode CSetWebAudioConf::SetWebAudioMotTime(std::string& sResult,int ChnNum,int MotTime)
{
	mLogInfo("SetWebAudioMotTime...");

	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };
	if(ChnNum == 13||ChnNum == 14)
	{
		ChnNum = 12;
	}

	if (!SetWebAudioMotTimeInner( ChnNum, MotTime, oResult, cResult))
	{
		mLogError("SetWebAudioMotTimeInner failed !!!");
		return eInterfaceResCodeError;
	}
	
	sqlite3 *reg_db = NULL;
	char DataBuf[1024];
	char *buf;
	char *register_db = "Audio_manage.db";
	 
	STU stu;
	time_t cur_time = time(NULL);
	struct tm *tm_ptr;
	tm_ptr = localtime(&cur_time);
	reg_db = (sqlite3 *)create_db(register_db); //创建数据库
	create_db_table(reg_db);  //创建注册表
	//解析数据
	char *arg[MAX_ARG_NUM] = {};
	for (int i = 0; i < MAX_ARG_NUM; i ++)
	{
		arg[i] = (char *)calloc(1, strlen(cResult)+1);
		if(NULL == arg[i]) 
		{
		  mLogError("calloc is error\n");
		  return eInterfaceResCodeError;
		 }
	 }
	sscanf(cResult, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s", arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7], arg[8], arg[9]);
	//获取数据库数据修改并更新

	
	if(!strcmp(arg[5], "ok>"))
	{

//			arg[1] = "2";//设置为获取类型，以便获取操作直接读取
//			sprintf(DataBuf,"%s", arg[0]);
//			int chnnum = atoi(arg[3]) + 1;//音频通道默认从0开始
//			for(int i = 1; i <= 5; i++)
//			{					
//				buf = DataBuf + strlen(DataBuf);
//				sprintf(buf,",%s", arg[i]); 				
//			}
//			
//			stu.id = chnnum + 53;
//			sprintf(stu.val,DataBuf);
//			sprintf(stu.name,"Audio_MICInPutPower%d",chnnum);
//			sprintf(stu.time, "%04d/%02d/%02d-%02d:%02d:%02d", tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
//			update_stu_db(reg_db, &stu); //add 到数据库 
		
		sprintf(stu.name,"Audio_MotivationTime0");
		find_stu_db(reg_db, &stu, DataBuf);
		//解析
		char *carg[40] = {};
		for (int i = 0; i < 40; i ++)
		{
			 carg[i] = (char *)calloc(1, strlen(DataBuf)+1);
			 if(NULL == carg[i]) 
			 {
			  mLogError("calloc is error\n");
			  return eInterfaceResCodeError;
			}
		 }
		sscanf(DataBuf, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s",\
		carg[0], carg[1], carg[2], carg[3], carg[4], carg[5], carg[6], carg[7], carg[8], carg[9],\
		carg[10], carg[11], carg[12], carg[13], carg[14], carg[15], carg[16], carg[17], carg[18], carg[19],\
		carg[20], carg[21], carg[22], carg[23], carg[24], carg[25], carg[26], carg[27], carg[28], carg[29], \
		carg[30], carg[31], carg[32], carg[33], carg[34], carg[35], carg[36], carg[37], carg[38], carg[39]);
		//修改相应通道的值
//			int chn = atoi(arg[3]) + 3;
		carg[4] = arg[4];
		//36个
		sprintf(DataBuf,"%s", carg[0]);
		for(int i = 1; i <= 5; i++)
		{					
			buf = DataBuf + strlen(DataBuf);
		
			sprintf(buf,",%s", carg[i]);
			
		}
		mLogDebug("DataBuf = "<<DataBuf);
		//更新数据
		stu.id = 15;
		sprintf(stu.val,DataBuf);
		sprintf(stu.name,"Audio_MotivationTime0");
		sprintf(stu.time, "%04d/%02d/%02d-%02d:%02d:%02d", tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
		update_stu_db(reg_db, &stu); //add 到数据库 
		
		mLogDebug("Set Power Success !!!\n");
		return eInterfaceResCodeSuccess;
	}
	else
	{
		mLogError("SetWebAudioPower failed");
		return eInterfaceResCodeError;
	}
	

	sResult = ComposeResult();
	cJSON_Delete(_result);
	return eInterfaceResCodeSuccess;
}

bool CSetWebAudioConf::SetWebAudioMotTimeInner(int ChnNum, int MotTime, CData& oResult, char* cResult)
{
	mLogDebug("run SetWebAudioMotTimeInner(...)");

	//获取状态
	int SerialID = 2;//串口透传，串口1
	int  cCmdLength   = 0;
	char cCmdSend[1024]   = {0};
	char Mute[10];
	//<指令类型,操作类型,输入/输出,通道,值>
	sprintf( cCmdSend, "<15,1,2,1,%d>", MotTime); 
	cCmdLength = strlen(cCmdSend);
	
	MPSOperationRes opResCode = eMPSResultOK;
	//发送cCmdSend信息给服务器
	ResponseCode resCode = _mpsClient->GetSerialNew(SerialID, cCmdSend, cCmdLength, opResCode, oResult,cResult);
	if(resCode != eResponseCodeSuccess){
		mLogError("GetConsoleNew(...) error:"<<resCode);
		return false;
	}
	return true;

}
InterfaceResCode CSetWebAudioConf::SetWebAudioFeedback(std::string& sResult, int ChnNum, int IsFeedback)
{
	mLogInfo("SetWebAudioFeedback");
	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };
	if(ChnNum == 13||ChnNum == 14)
	{
		ChnNum = 12;
	}
		
	if (!SetWebAudioFeedbackInner( ChnNum, IsFeedback, oResult, cResult))
	{
		mLogError("SetWebAudioFeedbackInner failed !!!");
		return eInterfaceResCodeError;
	}
	
	sqlite3 *reg_db = NULL;
	char DataBuf[1024];
	char *buf;
	char *register_db = "Audio_manage.db";
	 
	STU stu;
	time_t cur_time = time(NULL);
	struct tm *tm_ptr;
	tm_ptr = localtime(&cur_time);
	reg_db = (sqlite3 *)create_db(register_db); //创建数据库
	create_db_table(reg_db);  //创建注册表
	//解析数据
	char *arg[MAX_ARG_NUM] = {};
	for (int i = 0; i < MAX_ARG_NUM; i ++)
	{
		arg[i] = (char *)calloc(1, strlen(cResult)+1);
		if(NULL == arg[i]) 
		{
		  mLogError("calloc is error\n");
		  return eInterfaceResCodeError;
		 }
	 }
	sscanf(cResult, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s", arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7], arg[8], arg[9]);
	//获取数据库数据修改并更新
	
		if(!strcmp(arg[5], "ok>"))
		{
	
//			arg[1] = "2";//设置为获取类型，以便获取操作直接读取
//			sprintf(DataBuf,"%s", arg[0]);
//			int chnnum = atoi(arg[3])+1;//音频通道默认从0开始
//			for(int i = 1; i <= 5; i++)
//			{					
//				buf = DataBuf + strlen(DataBuf);
//				sprintf(buf,",%s", arg[i]); 				
//			}
//			
//			stu.id = chnnum + 39;
//			sprintf(stu.val,DataBuf);
//			sprintf(stu.name,"Audio_MICInPutSPL%d",chnnum);
//			sprintf(stu.time, "%04d/%02d/%02d-%02d:%02d:%02d", tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
//			update_stu_db(reg_db, &stu); //add 到数据库 

			
			sprintf(stu.name,"Audio_IsFeedback");
			find_stu_db(reg_db, &stu, DataBuf);
			//解析
			char *carg[40] = {};
			for (int i = 0; i < 40; i ++)
			{
				 carg[i] = (char *)calloc(1, strlen(DataBuf)+1);
				 if(NULL == carg[i]) 
				 {
				  mLogError("calloc is error\n");
				  return eInterfaceResCodeError;
				}
			 }
			sscanf(DataBuf, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s",\
			carg[0], carg[1], carg[2], carg[3], carg[4], carg[5], carg[6], carg[7], carg[8], carg[9],\
			carg[10], carg[11], carg[12], carg[13], carg[14], carg[15], carg[16], carg[17], carg[18], carg[19],\
			carg[20], carg[21], carg[22], carg[23], carg[24], carg[25], carg[26], carg[27], carg[28], carg[29], \
			carg[30], carg[31], carg[32], carg[33], carg[34], carg[35], carg[36], carg[37], carg[38], carg[39]);
			//修改相应通道的值
//			int chn = atoi(arg[3]) + 3;
			carg[4] = arg[4];
			//36个
			sprintf(DataBuf,"%s", carg[0]);
			for(int i = 1; i <= 5; i++)
			{					
				buf = DataBuf + strlen(DataBuf);
			
				sprintf(buf,",%s", carg[i]);
				
			}
			mLogDebug("DataBuf = "<<DataBuf);
			//更新数据
			stu.id = 6;
			sprintf(stu.val,DataBuf);
			sprintf(stu.name,"Audio_IsFeedback");
			sprintf(stu.time, "%04d/%02d/%02d-%02d:%02d:%02d", tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
			update_stu_db(reg_db, &stu); //add 到数据库 
			
			mLogDebug("Set Audio_IsFeedback Success !!!\n");
			return eInterfaceResCodeSuccess;
		}
		else
		{
			mLogError("Audio_IsFeedback failed");
			return eInterfaceResCodeError;
		}
	

	sResult = ComposeResult();
	cJSON_Delete(_result);
	return eInterfaceResCodeSuccess;
}

bool CSetWebAudioConf::SetWebAudioFeedbackInner(int ChnNum, int IsFeedback, CData& oResult, char* cResult)
{
	mLogDebug("run SetWebAudioFeedbackInner(...)");

	//获取状态
	int SerialID = 2;//串口透传，串口1
	int  cCmdLength   = 0;
	char cCmdSend[1024]   = {0};
	char Mute[10];
	if(IsFeedback == 0)
	{
		sprintf(Mute,"true"); 

	}else{
		sprintf(Mute,"false"); 

	}

	sprintf( cCmdSend, "<4,1,2,0,%s>",Mute); 
	cCmdLength = strlen(cCmdSend);
	
	MPSOperationRes opResCode = eMPSResultOK;
	//发送cCmdSend信息给服务器
	ResponseCode resCode = _mpsClient->GetSerialNew(SerialID, cCmdSend, cCmdLength, opResCode, oResult,cResult);
	if(resCode != eResponseCodeSuccess){
		mLogError("GetConsoleNew(...) error:"<<resCode);
		return false;
	}
	
	return true;

}
InterfaceResCode CSetWebAudioConf::SetWebAudioAEC(std::string& sResult, int ChnNum, int IsAEC)
{
	mLogInfo("SetWebAudioAEC");
	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };
	if(ChnNum == 13||ChnNum == 14)
	{
		ChnNum = 12;
	}
		
	if (!SetWebAudioAECInner( ChnNum, IsAEC, oResult, cResult))
	{
		mLogError("SetWebAudioAECInner failed !!!");
		return eInterfaceResCodeError;
	}
	
	sqlite3 *reg_db = NULL;
	char DataBuf[1024];
	char *buf;
	char *register_db = "Audio_manage.db";
	 
	STU stu;
	time_t cur_time = time(NULL);
	struct tm *tm_ptr;
	tm_ptr = localtime(&cur_time);
	reg_db = (sqlite3 *)create_db(register_db); //创建数据库
	create_db_table(reg_db);  //创建注册表
	//解析数据
	char *arg[MAX_ARG_NUM] = {};
	for (int i = 0; i < MAX_ARG_NUM; i ++)
	{
		arg[i] = (char *)calloc(1, strlen(cResult)+1);
		if(NULL == arg[i]) 
		{
		  mLogError("calloc is error\n");
		  return eInterfaceResCodeError;
		 }
	 }
	sscanf(cResult, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s", arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7], arg[8], arg[9]);
	//获取数据库数据修改并更新
	
		if(!strcmp(arg[5], "ok>"))
		{
	
//			arg[1] = "2";//设置为获取类型，以便获取操作直接读取
//			sprintf(DataBuf,"%s", arg[0]);
//			int chnnum = atoi(arg[3])+1;//音频通道默认从0开始
//			for(int i = 1; i <= 5; i++)
//			{					
//				buf = DataBuf + strlen(DataBuf);
//				sprintf(buf,",%s", arg[i]); 				
//			}
//			
//			stu.id = chnnum + 39;
//			sprintf(stu.val,DataBuf);
//			sprintf(stu.name,"Audio_MICInPutSPL%d",chnnum);
//			sprintf(stu.time, "%04d/%02d/%02d-%02d:%02d:%02d", tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
//			update_stu_db(reg_db, &stu); //add 到数据库 

			
			sprintf(stu.name,"Audio_IsAEC");
			find_stu_db(reg_db, &stu, DataBuf);
			//解析
			char *carg[40] = {};
			for (int i = 0; i < 40; i ++)
			{
				 carg[i] = (char *)calloc(1, strlen(DataBuf)+1);
				 if(NULL == carg[i]) 
				 {
				  mLogError("calloc is error\n");
				  return eInterfaceResCodeError;
				}
			 }
			sscanf(DataBuf, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s",\
			carg[0], carg[1], carg[2], carg[3], carg[4], carg[5], carg[6], carg[7], carg[8], carg[9],\
			carg[10], carg[11], carg[12], carg[13], carg[14], carg[15], carg[16], carg[17], carg[18], carg[19],\
			carg[20], carg[21], carg[22], carg[23], carg[24], carg[25], carg[26], carg[27], carg[28], carg[29], \
			carg[30], carg[31], carg[32], carg[33], carg[34], carg[35], carg[36], carg[37], carg[38], carg[39]);
			//修改相应通道的值
//			int chn = atoi(arg[3]) + 3;
			carg[4] = arg[4];
			//36个
			sprintf(DataBuf,"%s", carg[0]);
			for(int i = 1; i <= 5; i++)
			{					
				buf = DataBuf + strlen(DataBuf);
			
				sprintf(buf,",%s", carg[i]);
				
			}
			mLogDebug("DataBuf = "<<DataBuf);
			//更新数据
			stu.id = 5;
			sprintf(stu.val,DataBuf);
			sprintf(stu.name,"Audio_IsAEC");
			sprintf(stu.time, "%04d/%02d/%02d-%02d:%02d:%02d", tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
			update_stu_db(reg_db, &stu); //add 到数据库 
			
			mLogDebug("Set Audio_IsAEC Success !!!\n");
			return eInterfaceResCodeSuccess;
		}
		else
		{
			mLogError("Audio_IsAEC failed");
			return eInterfaceResCodeError;
		}
	

	sResult = ComposeResult();
	cJSON_Delete(_result);
	return eInterfaceResCodeSuccess;
}

bool CSetWebAudioConf::SetWebAudioAECInner(int ChnNum, int IsAEC, CData& oResult, char* cResult)
{
	mLogDebug("run SetWebAudioAECInner(...)");

	//获取状态
	int SerialID = 2;//串口透传，串口1
	int  cCmdLength   = 0;
	char cCmdSend[1024]   = {0};
	char Mute[10];
	if(IsAEC == 0)
	{
		sprintf(Mute,"true"); 

	}else{
		sprintf(Mute,"false"); 

	}

	sprintf( cCmdSend, "<5,1,2,0,%s>", Mute); 
	cCmdLength = strlen(cCmdSend);
	
	MPSOperationRes opResCode = eMPSResultOK;
	//发送cCmdSend信息给服务器
	ResponseCode resCode = _mpsClient->GetSerialNew(SerialID, cCmdSend, cCmdLength, opResCode, oResult,cResult);
	if(resCode != eResponseCodeSuccess){
		mLogError("GetConsoleNew(...) error:"<<resCode);
		return false;
	}
	
	return true;

}
InterfaceResCode CSetWebAudioConf::SetWebAudioANS(std::string& sResult,int ChnNum,int IsANS)
{
	mLogInfo("SetWebAudioANS...");

	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };
	if(ChnNum == 13||ChnNum == 14)
	{
		ChnNum = 12;
	}

	if (!SetWebAudioANSInner( ChnNum, IsANS, oResult, cResult))
	{
		mLogError("SetWebAudioANSInner failed !!!");
		return eInterfaceResCodeError;
	}
	
	sqlite3 *reg_db = NULL;
	char DataBuf[1024];
	char *buf;
	char *register_db = "Audio_manage.db";
	 
	STU stu;
	time_t cur_time = time(NULL);
	struct tm *tm_ptr;
	tm_ptr = localtime(&cur_time);
	reg_db = (sqlite3 *)create_db(register_db); //创建数据库
	create_db_table(reg_db);  //创建注册表
	//解析数据
	char *arg[MAX_ARG_NUM] = {};
	for (int i = 0; i < MAX_ARG_NUM; i ++)
	{
		arg[i] = (char *)calloc(1, strlen(cResult)+1);
		if(NULL == arg[i]) 
		{
		  mLogError("calloc is error\n");
		  return eInterfaceResCodeError;
		 }
	 }
	sscanf(cResult, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s", arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7], arg[8], arg[9]);
	//获取数据库数据修改并更新

	
	if(!strcmp(arg[5], "ok>"))
	{

//			arg[1] = "2";//设置为获取类型，以便获取操作直接读取
//			sprintf(DataBuf,"%s", arg[0]);
//			int chnnum = atoi(arg[3]) + 1;//音频通道默认从0开始
//			for(int i = 1; i <= 5; i++)
//			{					
//				buf = DataBuf + strlen(DataBuf);
//				sprintf(buf,",%s", arg[i]); 				
//			}
//			
//			stu.id = chnnum + 53;
//			sprintf(stu.val,DataBuf);
//			sprintf(stu.name,"Audio_MICInPutPower%d",chnnum);
//			sprintf(stu.time, "%04d/%02d/%02d-%02d:%02d:%02d", tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
//			update_stu_db(reg_db, &stu); //add 到数据库 
		
		sprintf(stu.name,"Audio_IsANS");
		find_stu_db(reg_db, &stu, DataBuf);
		//解析
		char *carg[40] = {};
		for (int i = 0; i < 40; i ++)
		{
			 carg[i] = (char *)calloc(1, strlen(DataBuf)+1);
			 if(NULL == carg[i]) 
			 {
			  mLogError("calloc is error\n");
			  return eInterfaceResCodeError;
			}
		 }
		sscanf(DataBuf, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s",\
		carg[0], carg[1], carg[2], carg[3], carg[4], carg[5], carg[6], carg[7], carg[8], carg[9],\
		carg[10], carg[11], carg[12], carg[13], carg[14], carg[15], carg[16], carg[17], carg[18], carg[19],\
		carg[20], carg[21], carg[22], carg[23], carg[24], carg[25], carg[26], carg[27], carg[28], carg[29], \
		carg[30], carg[31], carg[32], carg[33], carg[34], carg[35], carg[36], carg[37], carg[38], carg[39]);
		//修改相应通道的值
//			int chn = atoi(arg[3]) + 3;
		carg[4] = arg[4];
		//36个
		sprintf(DataBuf,"%s", carg[0]);
		for(int i = 1; i <= 5; i++)
		{					
			buf = DataBuf + strlen(DataBuf);
		
			sprintf(buf,",%s", carg[i]);
			
		}
		mLogDebug("DataBuf = "<<DataBuf);
		//更新数据
		stu.id = 8;
		sprintf(stu.val,DataBuf);
		sprintf(stu.name,"Audio_IsANS");
		sprintf(stu.time, "%04d/%02d/%02d-%02d:%02d:%02d", tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
		update_stu_db(reg_db, &stu); //add 到数据库 
		
		mLogDebug("Set Audio_IsANS Success !!!\n");
		return eInterfaceResCodeSuccess;
	}
	else
	{
		mLogError("Audio_IsANS failed");
		return eInterfaceResCodeError;
	}
	

	sResult = ComposeResult();
	cJSON_Delete(_result);
	return eInterfaceResCodeSuccess;
}

bool CSetWebAudioConf::SetWebAudioANSInner(int ChnNum, int IsANS, CData& oResult, char* cResult)
{
	mLogDebug("run SetWebAudioANSInner(...)");

	//获取状态
	int SerialID = 2;//串口透传，串口1
	int  cCmdLength   = 0;
	char cCmdSend[1024]   = {0};
	char Mute[10];
	if(IsANS == 0)
	{
		sprintf(Mute,"true"); 

	}else{
		sprintf(Mute,"false"); 

	}
	//<指令类型,操作类型,输入/输出,通道,值>
	sprintf( cCmdSend, "<6,1,2,0,%s>", Mute); 
	cCmdLength = strlen(cCmdSend);

	MPSOperationRes opResCode = eMPSResultOK;
	//发送cCmdSend信息给服务器
	ResponseCode resCode = _mpsClient->GetSerialNew(SerialID, cCmdSend, cCmdLength, opResCode, oResult,cResult);
	if(resCode != eResponseCodeSuccess){
		mLogError("GetConsoleNew(...) error:"<<resCode);
		return false;
	}
	return true;

}
InterfaceResCode CSetWebAudioConf::SetWebAudioAGC(std::string& sResult, int ChnNum, int IsAGC)
{
	mLogInfo("SetWebAudioAGC");
	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };
	if(ChnNum == 13||ChnNum == 14)
	{
		ChnNum = 12;
	}
		
	if (!SetWebAudioAGCInner( ChnNum, IsAGC, oResult, cResult))
	{
		mLogError("SetWebAudioAGCInner failed !!!");
		return eInterfaceResCodeError;
	}
	
	sqlite3 *reg_db = NULL;
	char DataBuf[1024];
	char *buf;
	char *register_db = "Audio_manage.db";
	 
	STU stu;
	time_t cur_time = time(NULL);
	struct tm *tm_ptr;
	tm_ptr = localtime(&cur_time);
	reg_db = (sqlite3 *)create_db(register_db); //创建数据库
	create_db_table(reg_db);  //创建注册表
	//解析数据
	char *arg[MAX_ARG_NUM] = {};
	for (int i = 0; i < MAX_ARG_NUM; i ++)
	{
		arg[i] = (char *)calloc(1, strlen(cResult)+1);
		if(NULL == arg[i]) 
		{
		  mLogError("calloc is error\n");
		  return eInterfaceResCodeError;
		 }
	 }
	sscanf(cResult, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s", arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7], arg[8], arg[9]);
	//获取数据库数据修改并更新
	
		if(!strcmp(arg[5], "ok>"))
		{
	
//			arg[1] = "2";//设置为获取类型，以便获取操作直接读取
//			sprintf(DataBuf,"%s", arg[0]);
//			int chnnum = atoi(arg[3])+1;//音频通道默认从0开始
//			for(int i = 1; i <= 5; i++)
//			{					
//				buf = DataBuf + strlen(DataBuf);
//				sprintf(buf,",%s", arg[i]); 				
//			}
//			
//			stu.id = chnnum + 39;
//			sprintf(stu.val,DataBuf);
//			sprintf(stu.name,"Audio_MICInPutSPL%d",chnnum);
//			sprintf(stu.time, "%04d/%02d/%02d-%02d:%02d:%02d", tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
//			update_stu_db(reg_db, &stu); //add 到数据库 

			
			sprintf(stu.name,"Audio_IsAGC");
			find_stu_db(reg_db, &stu, DataBuf);
			//解析
			char *carg[40] = {};
			for (int i = 0; i < 40; i ++)
			{
				 carg[i] = (char *)calloc(1, strlen(DataBuf)+1);
				 if(NULL == carg[i]) 
				 {
				  mLogError("calloc is error\n");
				  return eInterfaceResCodeError;
				}
			 }
			sscanf(DataBuf, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s",\
			carg[0], carg[1], carg[2], carg[3], carg[4], carg[5], carg[6], carg[7], carg[8], carg[9],\
			carg[10], carg[11], carg[12], carg[13], carg[14], carg[15], carg[16], carg[17], carg[18], carg[19],\
			carg[20], carg[21], carg[22], carg[23], carg[24], carg[25], carg[26], carg[27], carg[28], carg[29], \
			carg[30], carg[31], carg[32], carg[33], carg[34], carg[35], carg[36], carg[37], carg[38], carg[39]);
			//修改相应通道的值
			int chn = atoi(arg[3]) + 3;
			carg[chn] = arg[4];
			//36个
			sprintf(DataBuf,"%s", carg[0]);
			for(int i = 1; i <= 27; i++)
			{					
				buf = DataBuf + strlen(DataBuf);
			
				sprintf(buf,",%s", carg[i]);
				
			}
			mLogDebug("DataBuf = "<<DataBuf);
			//更新数据
			stu.id = 6;
			sprintf(stu.val,DataBuf);
			sprintf(stu.name,"Audio_IsAGC");
			sprintf(stu.time, "%04d/%02d/%02d-%02d:%02d:%02d", tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
			update_stu_db(reg_db, &stu); //add 到数据库 
			
			mLogDebug("Set Audio_IsAGC Success !!!\n");
			return eInterfaceResCodeSuccess;
		}
		else
		{
			mLogError("Audio_IsAGC failed");
			return eInterfaceResCodeError;
		}
	

	sResult = ComposeResult();
	cJSON_Delete(_result);
	return eInterfaceResCodeSuccess;
}

bool CSetWebAudioConf::SetWebAudioAGCInner(int ChnNum, int IsAGC, CData& oResult, char* cResult)
{
	mLogDebug("run SetWebAudioAGCInner(...)");

	//获取状态
	int SerialID = 2;//串口透传，串口1
	int  cCmdLength   = 0;
	char cCmdSend[1024]   = {0};
	char Mute[10];
	if(IsAGC == 0)
	{
		sprintf(Mute,"true"); 

	}else{
		sprintf(Mute,"false"); 

	}

	sprintf( cCmdSend, "<3,1,2,%d,%s>" ,ChnNum, Mute); 
	cCmdLength = strlen(cCmdSend);
	
	MPSOperationRes opResCode = eMPSResultOK;
	//发送cCmdSend信息给服务器
	ResponseCode resCode = _mpsClient->GetSerialNew(SerialID, cCmdSend, cCmdLength, opResCode, oResult,cResult);
	if(resCode != eResponseCodeSuccess){
		mLogError("GetConsoleNew(...) error:"<<resCode);
		return false;
	}
	
	return true;

}
InterfaceResCode CSetWebAudioConf::SetWebAudioIncentives(std::string& sResult, int ChnNum, int IsIncentives)
{
	mLogInfo("SetWebAudioIncentives");
	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };
	if(ChnNum == 13||ChnNum == 14)
	{
		ChnNum = 12;
	}
		
	if (!SetWebAudioIncentivesInner( ChnNum, IsIncentives, oResult, cResult))
	{
		mLogError("SetWebAudioIncentivesInner failed !!!");
		return eInterfaceResCodeError;
	}
	
	sqlite3 *reg_db = NULL;
	char DataBuf[1024];
	char *buf;
	char *register_db = "Audio_manage.db";
	 
	STU stu;
	time_t cur_time = time(NULL);
	struct tm *tm_ptr;
	tm_ptr = localtime(&cur_time);
	reg_db = (sqlite3 *)create_db(register_db); //创建数据库
	create_db_table(reg_db);  //创建注册表
	//解析数据
	char *arg[MAX_ARG_NUM] = {};
	for (int i = 0; i < MAX_ARG_NUM; i ++)
	{
		arg[i] = (char *)calloc(1, strlen(cResult)+1);
		if(NULL == arg[i]) 
		{
		  mLogError("calloc is error\n");
		  return eInterfaceResCodeError;
		 }
	 }
	sscanf(cResult, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s", arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7], arg[8], arg[9]);
	//获取数据库数据修改并更新
	
		if(!strcmp(arg[5], "ok>"))
		{
	
//			arg[1] = "2";//设置为获取类型，以便获取操作直接读取
//			sprintf(DataBuf,"%s", arg[0]);
//			int chnnum = atoi(arg[3])+1;//音频通道默认从0开始
//			for(int i = 1; i <= 5; i++)
//			{					
//				buf = DataBuf + strlen(DataBuf);
//				sprintf(buf,",%s", arg[i]); 				
//			}
//			
//			stu.id = chnnum + 39;
//			sprintf(stu.val,DataBuf);
//			sprintf(stu.name,"Audio_MICInPutSPL%d",chnnum);
//			sprintf(stu.time, "%04d/%02d/%02d-%02d:%02d:%02d", tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
//			update_stu_db(reg_db, &stu); //add 到数据库 

			
			sprintf(stu.name,"Audio_IsIncentives");
			find_stu_db(reg_db, &stu, DataBuf);
			//解析
			char *carg[40] = {};
			for (int i = 0; i < 40; i ++)
			{
				 carg[i] = (char *)calloc(1, strlen(DataBuf)+1);
				 if(NULL == carg[i]) 
				 {
				  mLogError("calloc is error\n");
				  return eInterfaceResCodeError;
				}
			 }
			sscanf(DataBuf, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s",\
			carg[0], carg[1], carg[2], carg[3], carg[4], carg[5], carg[6], carg[7], carg[8], carg[9],\
			carg[10], carg[11], carg[12], carg[13], carg[14], carg[15], carg[16], carg[17], carg[18], carg[19],\
			carg[20], carg[21], carg[22], carg[23], carg[24], carg[25], carg[26], carg[27], carg[28], carg[29], \
			carg[30], carg[31], carg[32], carg[33], carg[34], carg[35], carg[36], carg[37], carg[38], carg[39]);
			//修改相应通道的值
			int chn = atoi(arg[3]) + 3;
			carg[chn] = arg[4];
			//36个
			sprintf(DataBuf,"%s", carg[0]);
			for(int i = 1; i <= 16; i++)
			{					
				buf = DataBuf + strlen(DataBuf);
			
				sprintf(buf,",%s", carg[i]);
				
			}
			mLogDebug("DataBuf = "<<DataBuf);
			//更新数据
			stu.id = 5;
			sprintf(stu.val,DataBuf);
			sprintf(stu.name,"Audio_IsIncentives");
			sprintf(stu.time, "%04d/%02d/%02d-%02d:%02d:%02d", tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
			update_stu_db(reg_db, &stu); //add 到数据库 
			
			mLogDebug("Set Audio_IsIncentives Success !!!\n");
			return eInterfaceResCodeSuccess;
		}
		else
		{
			mLogError("Audio_IsIncentives failed");
			return eInterfaceResCodeError;
		}
	

	sResult = ComposeResult();
	cJSON_Delete(_result);
	return eInterfaceResCodeSuccess;
}

bool CSetWebAudioConf::SetWebAudioIncentivesInner(int ChnNum, int IsIncentives, CData& oResult, char* cResult)
{
	mLogDebug("run SetWebAudioIncentivesInner(...)");

	//获取状态
	int SerialID = 2;//串口透传，串口1
	int  cCmdLength   = 0;
	char cCmdSend[1024]   = {0};
	char Mute[10];
	if(IsIncentives == 1)
	{
		sprintf(Mute,"true"); 

	}else{
		sprintf(Mute,"false"); 

	}
	sprintf( cCmdSend, "<11,1,2,%d,%s>", ChnNum, Mute); 
	cCmdLength = strlen(cCmdSend);
	
	MPSOperationRes opResCode = eMPSResultOK;
	//发送cCmdSend信息给服务器
	ResponseCode resCode = _mpsClient->GetSerialNew(SerialID, cCmdSend, cCmdLength, opResCode, oResult,cResult);
	if(resCode != eResponseCodeSuccess){
		mLogError("GetConsoleNew(...) error:"<<resCode);
		return false;
	}
	
	return true;

}
bool CSetWebAudioConf::GetWebAudioBoardTypeInner(CData& oResult, char* cResult)
{
	mLogDebug("run GetWebAudioBoardTypeInner(...)");
	string	resolution,encodeMode,coderateType;

	char szReqBody[REQ_BODY_MAXLEN] = { 0 };
	sprintf(szReqBody,"sys.audio_type\nsys.audio_board_is_show\n");

	int realBodySize = strlen(szReqBody);

	char szReqCmd[REQ_CMD_MAXLEN] = { 0 };
	szReqCmd[0] = 0;
	szReqCmd[1] = 2;
	szReqCmd[2] = realBodySize >> 8;
	szReqCmd[3] = realBodySize & 0xff;

	memcpy(&szReqCmd[4], szReqBody, realBodySize);
#if 1
	MPSOperationRes opResCode = eMPSResultOK;
	ResponseCode resCode = _mpsClient->GetConfigNew(szReqCmd, 4 + realBodySize, opResCode, oResult, cResult);
	if (resCode != eResponseCodeSuccess)
	{
		mLogError("SetWebSyncTimeInner Run Failed !!!");
		return false;
	}
#endif

	return true;


}

InterfaceResCode CSetWebAudioConf::GetWebAudioBoardType(std::string& sResult)
{
	mLogInfo("GetWebAudioBoardType");
	int ChnNum,val;
	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };
	cJSON *tmp[15];
	string Method = "GetWebAudioBoardType";
	string IP;

	if (!GetWebAudioBoardTypeInner( oResult, cResult))
	{
		mLogError("GetWebAudioBoardType failed !!!");
		return eInterfaceResCodeError;
	}
	
	if (!_analyzePara.ConvertSTRING2CJSONGetWebParam(oResult, cResult, _result, _addressList, NULL, Method, IP, 0))
	{
		mLogError("Failed to run  ConvertSTRING2CJSONGetWebParam(...)");
		return eInterfaceResCodeError;
	}

	sResult = ComposeResult();
	cJSON_Delete(_result);
	return eInterfaceResCodeSuccess;
}


