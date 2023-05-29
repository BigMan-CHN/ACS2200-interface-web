#include "GetWebAudioConf.h"
#include "MPSClient.h"

CGetWebAudioConf::CGetWebAudioConf(const std::string& ip, unsigned short port, int timeOut)
	:_result(cJSON_CreateObject())
	,_audioParamList(cJSON_CreateArray())
	, _mpsClient(new MPSClient(ip, port, timeOut))
{

}

CGetWebAudioConf::~CGetWebAudioConf()
{
}

bool CGetWebAudioConf::ComposeResult(cJSON* result)
{
// 	if (_result == NULL || result == NULL) {
// 		mLogError("cJSON result == NULL.");
// 		result = NULL;
// 		return false;
// 	}
// 
// 	if (_nbTaskList == NULL) {
// 		mLogWarn("cJSON _nbTaskList == NULL.");
// 		return false;
// 	}
// 
// 	//result中添加节点
// 
// 	cJSON_AddItemToObject(_result, GET_HOST_NBINFO_TASKLIST, _nbTaskList);
	return true;
}

std::string CGetWebAudioConf::ComposeResult()
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

	if (_audioParamList == NULL)
	{
		mLogError("_audioParamList is NULL !!!");
		return "";
	}

	//cJSON_AddItemToObject(_result, GET_WEB_AUDIO_CONF_RES_AUDIOPARAMLIST, _audioParamList);

	char* cRes = cJSON_Print(_result);
	std::string res = cRes;
	//cJSON_Delete(result);
	return res;
}

InterfaceResCode CGetWebAudioConf::GetWebAudioConf(string& sResult)
{
	mLogInfo("GetWebAudioConf...");
	int cmd,val;
	cJSON* temList[10];
	char cResult[RES_BUF_MAXLEN] = { 0 };
	CData oResult = SVSMAP();
	//cmd从4开始是因为灵敏度和幻象电源采用单通道获取方式，此处去掉获取灵敏度和幻象电源
	 for(cmd = 4 ;cmd <= 6; cmd++)
	{
		memset(cResult, 0, RES_BUF_MAXLEN);
		//1. 调用类内部方法进一步封装业务请求
		if (!GetWebAudioConfInner(oResult, cResult, cmd))
		{
			mLogError("Failed to run  GetWebAudioConfInner(...)");
			return eInterfaceResCodeError120;
		}
		//用于生成节点，存储解析后的参数
		//temList[cmd] = cJSON_CreateObject();
		if (!_analyPara.ConvertSTRING2CJSONParseSerialCmdParam(oResult, cResult, _result, _audioParamList,NULL))
		{
		 mLogError("Failed to run  ConvertSTRING2CJSONParseSerialCmdParam(...)");
		 return eInterfaceResCodeError121;
		}

	}
	if (!GetWebAudioMotivationTimeInner(oResult, cResult, 0))
	{
		mLogError("Failed to run  GetWebAudioMotivationTimeInner(...)");
		return eInterfaceResCodeError120;
	}
	//3. 将step2中获取的响应格式化为Json字符串
	sResult = ComposeResult();

	return eInterfaceResCodeSuccess;
}
bool CGetWebAudioConf::GetWebAudioMotivationTimeInner(CData& oResult, char* cResult, int ChnNum)
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

	cJSON_AddItemToObject(_result, "MotTime", cJSON_CreateNumber(atoi(carg[4])));

	return true;

}

bool CGetWebAudioConf::GetWebAudioConfInner(CData& oResult, char* cResult,int cmd)
{
	mLogDebug("run GetWebAudioConfInner(...)");
#if 0
	 //获取状态
	 int SerialID = 1;//串口透传，串口1
	 int  cCmdLength   = 0;
	 char cCmdSend[1024]   = {0};
	 //string Cmd[8] = {"<1,2,2,0,0>","<2,2,2,0,0>","<3,2,2,0,0>","<4,2,2,0,0>","<5,2,2,0,0>","<6,2,2,0,0>","<7,2,2,0,0>"};
	 //cCmdLength = strlen(Cmd.c_str());
	 //memcpy(cCmdSend, Cmd.c_str(), strlen(Cmd.c_str()));

	 //<指令类型,操作类型,输入/输出,通道,值>
	//语音激励开关
	 if(cmd == 7)
	 	cmd = 11;
	 sprintf(cCmdSend,INTERFACE_GETWEBAUDIOCONF_SENDCMD,cmd); 
	 cCmdLength = strlen(cCmdSend);
	 
	 MPSOperationRes opResCode = eMPSResultOK;
	 //发送cCmdSend信息给服务器
	 ResponseCode resCode = _mpsClient->GetSerialNew(SerialID, cCmdSend, cCmdLength, opResCode, oResult,cResult);
	 if(resCode != eResponseCodeSuccess){
		 mLogError("GetConsoleNew(...) error:"<<resCode);
		 return false;
	 }
	 return true;
#else
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
	switch(cmd)
	{
		case 1:
			sprintf(stu.name,"Audio_SPL");
			break;
		case 2:
			sprintf(stu.name,"Audio_IsPower");
			break;
		case 3:
			sprintf(stu.name,"Audio_IsAGC");
			break;
		case 4:
			sprintf(stu.name,"Audio_IsFeedback");
			break;
		case 5:
			sprintf(stu.name,"Audio_IsAEC");
			break;
		case 6:
			sprintf(stu.name,"Audio_IsANS");
			break;
		case 7:
			sprintf(stu.name,"Audio_IsIncentives");
			break;

	}
	
	find_stu_db(reg_db, &stu, cResult);
	return true;
#endif
}
