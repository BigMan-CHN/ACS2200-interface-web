#include "SetWebDSPMatrix.h"
#include "MPSClient.h"


CSetWebDSPMatrix::CSetWebDSPMatrix(const std::string& ip, unsigned short port, int timeOut)
	:_mpsClient(new MPSClient(ip, port, timeOut))
	,_result(cJSON_CreateObject())
{

}

CSetWebDSPMatrix::~CSetWebDSPMatrix()
{
	
}

bool CSetWebDSPMatrix::ComposeResult(cJSON* result)
{
// 	if (_result == NULL || result == NULL) {
// 		mLogError("cJSON result == NULL.");
// 		result = NULL;
// 		return false;
// 	}

	return true;
}

std::string CSetWebDSPMatrix::ComposeResult()
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


InterfaceResCode CSetWebDSPMatrix::SetWebDSPMatrix(std::string& sResult, string InPutChnnel, int OutputType)
{
	mLogInfo("SetWebDSPMatrix...");

	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };

	if (!SetWebDSPMatrixInner( oResult, cResult, InPutChnnel, OutputType))
	{
		mLogError("SetWebDSPMatrixInner failed !!!");
		return eInterfaceResCodeError;
	}

	sResult = ComposeResult();
	cJSON_Delete(_result);
	return eInterfaceResCodeSuccess;
}


bool CSetWebDSPMatrix::SetWebDSPMatrixInner( CData& oResult, char* cResult, string InPutChnnel, int OutputType)
{
	mLogDebug("run SetWebDSPMatrixInner(...)");

		//1. 封装业务请求
		char szReqBody[REQ_BODY_MAXLEN] = { 0 };
				
		sprintf(szReqBody, INTERFACE_SETWEB_SWITCH_DSP_SENDCMD, OutputType, InPutChnnel.c_str());
		int realBodySize = strlen(szReqBody);
			
		char szReqCmd[REQ_CMD_MAXLEN] = { 0 };
		szReqCmd[0] = 0;  //不加密
		szReqCmd[1] = 3;  //Get请求, 3是SET, 4是SYS
		szReqCmd[2] = realBodySize >> 8;
		szReqCmd[3] = realBodySize;
		memcpy(&szReqCmd[4], szReqBody, realBodySize);
			
		//2. 发送socket请求
		MPSOperationRes opResCode = eMPSResultOK; //??
		ResponseCode resCode = _mpsClient->GetConfigNew(szReqCmd, 4 + realBodySize, opResCode, oResult, cResult);
		if (resCode != eResponseCodeSuccess) {
			mLogError("GetConfig(...) error:" << resCode);
			return false;
		}

	return true;

}

