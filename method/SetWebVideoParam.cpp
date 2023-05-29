#include "SetWebVideoParam.h"
#include "MPSClient.h"

CSetWebVideoParam::CSetWebVideoParam(const std::string& ip, unsigned short port, int timeOut)
	:_mpsClient(new MPSClient(ip, port, timeOut))
	,_result(cJSON_CreateObject())
{

}

CSetWebVideoParam::~CSetWebVideoParam()
{
	
}

bool CSetWebVideoParam::ComposeResult(cJSON* result)
{
 	if (_result == NULL || result == NULL) {
 		mLogError("cJSON result == NULL.");
 		result = NULL;
 		return false;
 	}

	return true;
}

std::string CSetWebVideoParam::ComposeResult()
{
 	cJSON* result = cJSON_CreateObject();
 	if (ComposeResult(result) == false) {
 		cJSON_Delete(result);
 		return "";
 	}
 	else {
 		char* cRes = cJSON_Print(_result);
 		std::string res = cRes;
 		cJSON_Delete(result);
 		return res;
 	}
/*
	//assume the setmethod's response code is always okay. -_-!
	char* cRes = cJSON_Print(_result);
	std::string res = cRes;
	return res;
*/
}


InterfaceResCode CSetWebVideoParam::SetWebVideoParam(std::string& sResult,
	int chnNum,string chnName, int resolutionSet, int codeMode,
	int rateType, int rateSize, int frameSet,
	int gopSet)
{
	mLogInfo("设置 web 视频参数...");

	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };

	if (!SetWebVideoParamInner(chnNum, chnName, resolutionSet, codeMode, rateType, rateSize, frameSet, gopSet, oResult, cResult))
	{
		mLogError("SetWebVideoParamInner failed !!!");
		return eInterfaceResCodeError;
	}
	
//	mLogInfo("cResult[5] = "<<cResult[5]);
//	printf("cResult[5] = %d , %d",cResult[4],cResult[5]);
	if(cResult[5] == 4)
	{
		mLogError("SetWebVideoParam param failed !!!");
		return eInterfaceResCodeError129;
	}
	if(cResult[5] != 0)
	{
		mLogError("SetWebVideoParam failed !!!");
		return eInterfaceResCodeError;
	}
	//解析响应, 默认不处理
//	if (!_analyzePara.ConvertSTRING2CJSONSetWebVideoParam(oResult, cResult)) {
//		mLogError("Failed to run  ConvertSTRING2CJSONSetWebVideoParam(...)");
//		return eInterfaceResCodeError;
//	}

	sResult = ComposeResult();
	return eInterfaceResCodeSuccess;
}

bool CSetWebVideoParam::SetWebVideoParamInner(int chnNum, string chnName, int resolutionSet, int codeMode,
	int rateType, int rateSize, int frameSet,
	int gopSet, CData& oResult, char* cResult)
{
	mLogDebug("run SetWebVideoParamInner(...)");
	string  resolution,encodeMode,coderateType;
	//分辨率选择
	switch(resolutionSet)
  {
    case 1:
	   	 resolution = "720P";
	   	 break;
	case 2:
		 resolution = "1080P";
		 break;
	case 3:
		 resolution = "4K";
		 break;
	default:
		 mLogError("please input the right resolution!!!\n");
		 return false;
	}
	//编码类型
	 switch(codeMode)
	{
	  case 1:
		   encodeMode = "H.264";
		   break;
	  case 2:
		   encodeMode = "H.265";
		   break;
	  default:
		   mLogError("please input the right encodeMode!!!\n");
		   return false;
	  }
	//码率类型  
	 switch(rateType)
	{
	  case 1:
		   coderateType = "CBR";
		   break;
	  case 2:
		   coderateType = "VBR";
		   break;
	  default:
		   mLogError("please input the right coderateType!!!\n");
		   return false;
	  }

	char szReqBody[REQ_BODY_MAXLEN] = { 0 };
	sprintf(szReqBody,"vid_enc%d.channel=%s\nvid_enc%d.resolution=%s\nvid_enc%d.vid_payload=%s\nvid_enc%d.bitrate_mode=%s\nvid_enc%d.fps=%d\nvid_enc%d.gop=%d\nvid_enc%d.bitrate=%d\n",
		chnNum, chnName.c_str(), chnNum,resolution.c_str(), chnNum,encodeMode.c_str(),chnNum, coderateType.c_str(), chnNum, frameSet, chnNum, gopSet, chnNum,rateSize);
	mLogDebug("String szReqBody ="<<szReqBody);

	int realBodySize = strlen(szReqBody);

	char szReqCmd[REQ_CMD_MAXLEN] = { 0 };
	szReqCmd[0] = 0;
	szReqCmd[1] = 3;
	szReqCmd[2] = realBodySize >> 8;
	szReqCmd[3] = realBodySize & 0xff;

	memcpy(&szReqCmd[4], szReqBody, realBodySize);
#if 1
	 MPSOperationRes opResCode = eMPSResultOK;
	ResponseCode resCode = _mpsClient->GetConfigNew(szReqCmd, 4 + realBodySize, opResCode, oResult, cResult);
	if (resCode != eResponseCodeSuccess)
	{
		mLogError("SetWebVideoParamInner Run Failed !!!");
		return false;
	}
#endif

	return true;
}

