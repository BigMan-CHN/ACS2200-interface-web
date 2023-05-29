#include "SetWebInfraredSend.h"
#include "MPSClient.h"

CSetWebInfraredSend::CSetWebInfraredSend(const std::string& ip, unsigned short port, int timeOut)
	:_mpsClient(new MPSClient(ip, port, timeOut))
	,_result(cJSON_CreateObject())
{

}

CSetWebInfraredSend::~CSetWebInfraredSend()
{
	
}

bool CSetWebInfraredSend::ComposeResult(cJSON* result)
{
// 	if (_result == NULL || result == NULL) {
// 		mLogError("cJSON result == NULL.");
// 		result = NULL;
// 		return false;
// 	}

	return true;
}

std::string CSetWebInfraredSend::ComposeResult()
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


InterfaceResCode CSetWebInfraredSend::SetWebInfraredSend(std::string& sResult, int port,
	const std::string& fileName, int num, const std::string& order, const std::string& describe)
{
	mLogInfo("设置 WebInfraredSend...");

	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };

	if (!SetWebInfraredSendInner(port, fileName, num, order, describe, oResult, cResult))
	{
		mLogError("SetWebInfraredSendInner failed !!!");
		return eInterfaceResCodeError;
	}

	//解析响应, 默认不处理
	if (!_analyzePara.ConvertSTRING2CJSONSetWebInfraredSend(oResult, cResult)) {
		mLogError("Failed to run  ConvertSTRING2CJSONSetWebInfraredSend(...)");
		return eInterfaceResCodeError;
	}

	sResult = ComposeResult();
	return eInterfaceResCodeSuccess;
}

bool CSetWebInfraredSend::SetWebInfraredSendInner(int port, const std::string& fileName,
	int num, const std::string& order, const std::string& describe, CData& oResult, char* cResult)
{
		mLogDebug("run SetWebInfraredSendInner(...)");

		//获取状态
		int SerialID = port;//串口0
		int  cCmdLength   = 0;
		char cCmdSend[1024]   = {0};

		//cCmdLength = strlen(Cmd.c_str());
		//memcpy(cCmdSend, Cmd.c_str(), strlen(Cmd.c_str()));
		//index：命令索引，从1开始，1~50有效。
		if(num > 50){
			mLogError("SetWebInfraredSendInner num > 50 error:"<<num);
			return false;
		}
		//<指令类型,操作类型,输入/输出,通道,值>
		sprintf( cCmdSend, INTERFACE_GETWEBINFRAREDSEND_SENDCMD, num); 
		cCmdLength = strlen(cCmdSend);
#if 1	
		MPSOperationRes opResCode = eMPSResultOK;
		//发送cCmdSend信息给服务器
		ResponseCode resCode = _mpsClient->GetSerialNew(SerialID, cCmdSend, cCmdLength, opResCode, oResult,cResult);
		if(resCode != eResponseCodeSuccess){
			mLogError("GetSerialNew(...) error:"<<resCode);
			return false;
		}
		//判断返回值
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
		//printf("arg[2]:%s",arg[2]);
		if (strcmp("ok>", arg[2]))
		{
			mLogError("SetWebInfraredSendInner Send failed \n");
			return false;
		}

		return true;
#endif
}

