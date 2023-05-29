#include "GetWebDSPMatrix.h"
#include "MPSClient.h"


CGetWebDSPMatrix::CGetWebDSPMatrix(const std::string& ip, unsigned short port, int timeOut)
	:_mpsClient(new MPSClient(ip, port, timeOut))
	,_result(cJSON_CreateObject())
	,_addressList(cJSON_CreateArray())
{

}

CGetWebDSPMatrix::~CGetWebDSPMatrix()
{
	
}

bool CGetWebDSPMatrix::ComposeResult(cJSON* result)
{
// 	if (_result == NULL || result == NULL) {
// 		mLogError("cJSON result == NULL.");
// 		result = NULL;
// 		return false;
// 	}

	return true;
}

std::string CGetWebDSPMatrix::ComposeResult()
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


InterfaceResCode CGetWebDSPMatrix::GetWebDSPMatrix(std::string& sResult)
{
	mLogInfo("GetWebDSPMatrix...");

	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };
	cJSON* temList[3];
	for(int OutputType = 1; OutputType < 3; OutputType++)
	{
		if (!GetWebDSPMatrixInner( oResult, cResult, OutputType))
		{
			mLogError("GetWebDSPMatrixInner failed !!!");
			return eInterfaceResCodeError;
		}
		//用于生成节点，存储解析后的参数
		temList[OutputType] = cJSON_CreateObject();
		if (!_analyzePara.ConvertSTRING2CJSONGetWebDSPMatrixParam(oResult, cResult, _result, _addressList, temList[OutputType], OutputType ))
		{
	 		mLogError("Failed to run  ConvertSTRING2CJSONGetWebDSPMatrixParam(...)");
		 	return eInterfaceResCodeError;
		}
		
	}
	cJSON_AddItemToObject(_result, "DSPMatrixList", _addressList);
	sResult = ComposeResult();
	cJSON_Delete(_result);
	return eInterfaceResCodeSuccess;
}


bool CGetWebDSPMatrix::GetWebDSPMatrixInner( CData& oResult, char* cResult, int OutputType)
{
	mLogDebug("run GetWebDSPMatrixInner(...)");
		//1. 封装业务请求
		char szReqBody[REQ_BODY_MAXLEN] = { 0 };
				
		sprintf(szReqBody, INTERFACE_GETWEB_SWITCH_DSP_SENDCMD, OutputType);
		int realBodySize = strlen(szReqBody);
			
		char szReqCmd[REQ_CMD_MAXLEN] = { 0 };
		szReqCmd[0] = 0;  //不加密
		szReqCmd[1] = 2;  //Get请求, 3是SET, 4是SYS
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

