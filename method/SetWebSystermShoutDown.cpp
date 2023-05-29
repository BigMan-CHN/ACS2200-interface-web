#include "SetWebSystermShoutDown.h"
#include "MPSClient.h"


CSetWebSystermShoutDown::CSetWebSystermShoutDown(const std::string& ip, unsigned short port, int timeOut)
	:_mpsClient(new MPSClient(ip, port, timeOut))
	,_result(cJSON_CreateObject())
{

}

CSetWebSystermShoutDown::~CSetWebSystermShoutDown()
{
	
}

bool CSetWebSystermShoutDown::ComposeResult(cJSON* result)
{
// 	if (_result == NULL || result == NULL) {
// 		mLogError("cJSON result == NULL.");
// 		result = NULL;
// 		return false;
// 	}

	return true;
}

std::string CSetWebSystermShoutDown::ComposeResult()
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


InterfaceResCode CSetWebSystermShoutDown::SetWebSystermShoutDown(std::string& sResult)
{
	mLogInfo("设置 SetWebSystermShoutDown...");

	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };

	if (!SetWebSystermShoutDownInner( oResult, cResult))
	{
		mLogError("SetWebInfraredStudyInner failed !!!");
		return eInterfaceResCodeError;
	}

	sResult = ComposeResult();
	cJSON_Delete(_result);
	return eInterfaceResCodeSuccess;
}


bool CSetWebSystermShoutDown::SetWebSystermShoutDownInner( CData& oResult, char* cResult)
{
	mLogDebug("run SetWebSystermShoutDownInner(...)");

	//获取状态
	int SerialID = 0;//串口0
	int  cCmdLength   = 0;
	char cCmdSend[1024]   = {0};
	char Sendbuf[1024]   = {0};
	//cCmdLength = strlen(Cmd.c_str());
	//memcpy(cCmdSend, Cmd.c_str(), strlen(Cmd.c_str()));
	//index：命令索引，从1开始，1~50有效。

	//<指令类型,操作类型,输入/输出,通道,值>
	sprintf( cCmdSend, INTERFACE_GETWEB_SHUTDOWN_SENDCMD); 
	cCmdLength = strlen(cCmdSend);
#if 1	
	MPSOperationRes opResCode = eMPSResultOK;
	//发送cCmdSend信息给服务器
	ResponseCode resCode = _mpsClient->GetSerialNew(SerialID, cCmdSend, cCmdLength, opResCode, oResult,cResult);
	if(resCode != eResponseCodeSuccess){
		mLogError("GetSerialNew(...) error:"<<resCode);
		return false;
	}

	//判断是否关机成功
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
	if (strcmp("ok>", arg[1]))
	{
		mLogError("shutdown failed \n");
		return false;
	}
#endif	

	return true;

}
InterfaceResCode CSetWebSystermShoutDown::SetWebSystermReboot(std::string& sResult)
{
	mLogInfo("SetWebSystermReboot...");

	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };

	if (!SetWebSystermRebootInner( oResult, cResult))
	{
		mLogError("SetWebSystermRebootInner failed !!!");
		return eInterfaceResCodeError;
	}

	sResult = ComposeResult();
	cJSON_Delete(_result);
	return eInterfaceResCodeSuccess;
}


bool CSetWebSystermShoutDown::SetWebSystermRebootInner( CData& oResult, char* cResult)
{
	mLogDebug("run SetWebSystermRebootInner(...)");

	//获取状态
	int SerialID = 0;//串口0
	int  cCmdLength   = 0;
	char cCmdSend[1024]   = {0};
	char Sendbuf[1024]   = {0};
	//cCmdLength = strlen(Cmd.c_str());
	//memcpy(cCmdSend, Cmd.c_str(), strlen(Cmd.c_str()));
	//index：命令索引，从1开始，1~50有效。

	//<指令类型,操作类型,输入/输出,通道,值>
	sprintf( cCmdSend, "<Reboot>"); 
	cCmdLength = strlen(cCmdSend);
	system("reboot");
#if 0	
	MPSOperationRes opResCode = eMPSResultOK;
	//发送cCmdSend信息给服务器
	ResponseCode resCode = _mpsClient->GetSerialNew(SerialID, cCmdSend, cCmdLength, opResCode, oResult,cResult);
	if(resCode != eResponseCodeSuccess){
		mLogError("GetSerialNew(...) error:"<<resCode);
		return false;
	}

	//判断是否关机成功
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
	if (strcmp("ok>", arg[1]))
	{
		mLogError("reboot failed \n");
		return false;
	}
#endif	

	return true;

}

