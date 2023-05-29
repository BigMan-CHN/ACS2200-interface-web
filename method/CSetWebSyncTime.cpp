#include "CSetWebSyncTime.h"
#include <boost/lexical_cast.hpp>

CSetWebSyncTime::CSetWebSyncTime(string ip, int port, int timeOut)
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

bool CSetWebSyncTime::ComposeResult(cJSON *result)
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

string CSetWebSyncTime::ComposeResult()
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
bool CSetWebSyncTime:: SetWebSyncTimeInner(string NtpIp, string SysTime, int Mode, CData& oResult, char* cResult)
{
		mLogDebug("run SetWebSyncTimeInner(...)");
		
		char sys_time[64];
		char szReqBody[REQ_BODY_MAXLEN] = { 0 };
	
		struct tm rtc_time;
		struct tm _tm;
		struct timeval tv;
		time_t timep;

		if(Mode == 0)
		{
			sprintf(szReqBody,INTERFACE_SETWEB_SYNC_TIME_SENDCMD, NtpIp.c_str());
		}
		else if(Mode == 1)
		{
			mLogDebug("SysTime ="<<SysTime.c_str());
#if 0			
			sprintf(sys_time,"date -s \"%s\"", SysTime.c_str());
			mLogDebug("sys_time ="<<sys_time);
			system(sys_time);
#else		
			//sys_time = 	SysTime.c_str();
			//sscanf("2006-03-18 2006:04:18.23", "%[^-]-%[^-]-%[^-,^ ] %d:%d:%d.%d",sztime6,sztime7,sztime8,&a,&b,&c,&d);
			//注意空格
			sscanf(SysTime.c_str(), "%d-%d-%d %d:%d:%d", &rtc_time.tm_year,&rtc_time.tm_mon, &rtc_time.tm_mday,&rtc_time.tm_hour,&rtc_time.tm_min, &rtc_time.tm_sec);
			mLogDebug("tm_year"<<rtc_time.tm_year<<"tm_mon"<<rtc_time.tm_mon<<"tm_mday"<<rtc_time.tm_mday<<"tm_hour"<<rtc_time.tm_hour<<"tm_min"<<rtc_time.tm_min<<"tm_sec"<<rtc_time.tm_sec);
			_tm.tm_sec = rtc_time.tm_sec;
			_tm.tm_min = rtc_time.tm_min;
			_tm.tm_hour = rtc_time.tm_hour;
			_tm.tm_mday = rtc_time.tm_mday;
			_tm.tm_mon = rtc_time.tm_mon - 1;
			_tm.tm_year = rtc_time.tm_year - 1900;
		 
			timep = mktime(&_tm);
			tv.tv_sec = timep;
			tv.tv_usec = 0;
#if 0			
			if(settimeofday (&tv, (struct timezone *) NULL) < 0)
			{
				mLogError("Set system datatime error!/n");
				return false;
		
			}
#else
			sprintf(sys_time,"%d/%02d/%02d %02d:%02d:%02d", rtc_time.tm_year,rtc_time.tm_mon,rtc_time.tm_mday,rtc_time.tm_hour,rtc_time.tm_min,rtc_time.tm_sec);
			mLogDebug("sys_time ="<<sys_time);
			sprintf(szReqBody,"sys.time=%s\n", sys_time);
			//mLogDebug("sys_time ="<<szReqBody);
#endif			
#endif		
			//return true;
		}
		int realBodySize = strlen(szReqBody);
	
		char szReqCmd[REQ_CMD_MAXLEN] = { 0 };
		szReqCmd[0] = 0;
		szReqCmd[1] = 3;
		szReqCmd[2] = realBodySize >> 8;
		szReqCmd[3] = realBodySize & 0xff;
	
		memcpy(&szReqCmd[4], szReqBody, realBodySize);
#if 1
		MPSOperationRes opResCode = eMPSResultOK;
		ResponseCode resCode = _client->GetConfigNew(szReqCmd, 4 + realBodySize, opResCode, oResult, cResult);
		if (resCode != eResponseCodeSuccess)
		{
			mLogError("SetWebSyncTimeInner Run Failed !!!");
			return false;
		}
#endif
	
		return true;



}

InterfaceResCode CSetWebSyncTime::SetWebSyncTime(string &sResult, string NtpIp, string SysTime, int Mode)
{
		mLogInfo("SetWebSyncTime...");
		
		CData oResult = SVSMAP();
		char cResult[RES_BUF_MAXLEN] = { 0 };
		
		if (!SetWebSyncTimeInner(NtpIp, SysTime, Mode, oResult, cResult))
		{
			mLogError("SetWebSyncTimeInner failed !!!");
			return eInterfaceResCodeError;
		}
#if 0		
		//解析响应, 默认不处理
		if (!_analyzePara.ConvertSTRING2CJSONSetWebVideoParam(oResult, cResult)) {
			mLogError("Failed to run  ConvertSTRING2CJSONSetWebVideoParam(...)");
			return eInterfaceResCodeError;
		}

#endif 
		
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
bool CSetWebSyncTime:: GetWebSyncTimeInner(CData& oResult, char* cResult)
{
		mLogDebug("run GetWebSyncTimeInner(...)");
		
		string	resolution,encodeMode,coderateType;
	
		char szReqBody[REQ_BODY_MAXLEN] = { 0 };
		sprintf(szReqBody,"ntp.server_ip\n");
	
		int realBodySize = strlen(szReqBody);
	
		char szReqCmd[REQ_CMD_MAXLEN] = { 0 };
		szReqCmd[0] = 0;
		szReqCmd[1] = 2;
		szReqCmd[2] = realBodySize >> 8;
		szReqCmd[3] = realBodySize & 0xff;
	
		memcpy(&szReqCmd[4], szReqBody, realBodySize);
#if 1
		MPSOperationRes opResCode = eMPSResultOK;
		ResponseCode resCode = _client->GetConfigNew(szReqCmd, 4 + realBodySize, opResCode, oResult, cResult);
		if (resCode != eResponseCodeSuccess)
		{
			mLogError("SetWebSyncTimeInner Run Failed !!!");
			return false;
		}
#endif
	
		return true;



}

InterfaceResCode CSetWebSyncTime::GetWebSyncTime(string &sResult)
{
	mLogInfo("GetWebSyncTime...");
	
	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };
	string Method = "GetWebSyncTime";
	string IP;
	//获取主机系统时间
	time_t timep;
	struct tm *LocalTime;
	char SysTime[80];
	time(&timep);
	LocalTime = std::localtime(&timep);
//	LocalTime->tm_hour = LocalTime->tm_hour + 8;//UTC时间转换为中国时间
//	LocalTime->tm_hour = (LocalTime->tm_hour + 8)%24;//UTC时间转换为中国时间

	strftime(SysTime, 80, "%Y-%m-%d %H:%M:%S", LocalTime);
	
	if (!GetWebSyncTimeInner( oResult, cResult))
	{
		mLogError("GetWebSyncTime failed !!!");
		return eInterfaceResCodeError;
	}
#if 1		
	//解析响应, 默认不处理
	if (!_AnalyParas.ConvertSTRING2CJSONGetWebParam(oResult, cResult, _result, _addressList, NULL, Method, IP, 0))
	{
		mLogError("Failed to run  ConvertSTRING2CJSONGetWebParam(...)");
		return eInterfaceResCodeError;
	}

#endif 
	cJSON_AddItemToObject(_result, "DevTime", cJSON_CreateString(SysTime));
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


