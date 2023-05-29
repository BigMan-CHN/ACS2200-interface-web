#include "CSetWebFtpConf.h"
#include <boost/lexical_cast.hpp>

CSetWebFtpConf::CSetWebFtpConf(string ip, int port, int timeOut)
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

bool CSetWebFtpConf::ComposeResult(cJSON *result)
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

string CSetWebFtpConf::ComposeResult()
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
bool CSetWebFtpConf::SetWebFtpConfInner(string FtpServerIp,int FtpPort,string FtpName,string FtpPassword,string FtpPath, CData& oResult, char* cResult)
{
	mLogDebug("run SetWebFtpConfInner(...)");
					
	char szReqBody[REQ_BODY_MAXLEN] = { 0 };
	string Resolution;
		
	sprintf(szReqBody,INTERFACE_SETWEB_FTP_CONF_SENDCMD, FtpPort, FtpPassword.c_str(), FtpPath.c_str());
					
	int realBodySize = strlen(szReqBody);
					
	char szReqCmd[REQ_CMD_MAXLEN] = { 0 };
	szReqCmd[0] = 0;
	szReqCmd[1] = 3;//2Get请求, 3是SET, 4是SYS
	szReqCmd[2] = realBodySize >> 8;
	szReqCmd[3] = realBodySize & 0xff;
				
	memcpy(&szReqCmd[4], szReqBody, realBodySize);
#if 1
	MPSOperationRes opResCode = eMPSResultOK;
	ResponseCode resCode = _client->GetConfigNew(szReqCmd, 4 + realBodySize, opResCode, oResult, cResult);
	if (resCode != eResponseCodeSuccess)
	{
		mLogError("SetWebFtpConfInner Run Failed !!!");
		return false;
	}
#endif
					
	return true;

}
InterfaceResCode CSetWebFtpConf::SetWebFtpConf(string &sResult,string FtpServerIp,int FtpPort,string FtpName,string FtpPassword,string FtpPath)
{
		//TODO
		mLogInfo("设置 Ftp 参数...");
				
		CData oResult = SVSMAP();
		char cResult[RES_BUF_MAXLEN] = { 0 };
				
		if (!SetWebFtpConfInner(FtpServerIp, FtpPort, FtpName, FtpPassword, FtpPath, oResult, cResult))
		{
			mLogError("SetWebFtpConfInner failed !!!");
			return eInterfaceResCodeError;
		}
			
#if 0
					int iRecvLength = (cResult[2] << 8) + cResult[3] + 4;
					printf("getMPSConfig\n");
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
			
		sResult = ComposeResult();
		cJSON_Delete(_result);

		return eInterfaceResCodeSuccess;

}


