#include "CGetSerialCmd.h"
#include <boost/lexical_cast.hpp>

CGetSerialCmd::CGetSerialCmd(string ip, int port, int timeOut)
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

bool CGetSerialCmd::ComposeResult(cJSON *result)
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

string CGetSerialCmd::ComposeResult()
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
bool CGetSerialCmd::GetSerialCmdParamInner(CData& oResult, char* cResult, int ChnNum, int CtrlType)
{
		mLogDebug("run GetSerialCmdParamInner(...)");
#if 0			
		//获取状态
		int SerialID = 1;//串口透传，串口1
		//add fwb 20201119
		int val = 0;
		int  cCmdLength   = 0;
		char cCmdSend[1024]   = {0};
		
		//<指令类型,操作类型,输入/输出,通道,值>
		if(ChnNum > 32)
		{
			mLogError("GetSerialCmdParamInner ChnNum ="<<ChnNum-1);
       		return false;
		}
		sprintf(cCmdSend,INTERFACE_GET_AUDIO_GAIN_CONF_SENDCMD, CtrlType, ChnNum-1, val);
		
		cCmdLength = strlen(cCmdSend);
		
		MPSOperationRes opResCode = eMPSResultOK;
		//发送cCmdSend信息给服务器
		ResponseCode resCode = _client->GetSerialNew(SerialID, cCmdSend, cCmdLength, opResCode, oResult,cResult);
		//printf("cResult:%s",cResult);
		mLogDebug("run GetSerialCmdParamInner  cResult="<<cResult);
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
	memset(DataBuf, 0, 1024);
	
	sprintf(stu.name,"Audio_AllDefultGain%d",CtrlType);
	find_stu_db(reg_db, &stu, DataBuf);
	
	char *arg[40] = {};
	for (int i = 0; i < 40; i ++)
	{
		arg[i] = (char *)calloc(1, strlen(DataBuf)+1);
		if(NULL == arg[i]) 
		{
		   mLogError("calloc is error\n");
		   return false;
		}
	}
	
	//mLogDebug("cResult:"<<cResult);
	
	//命令格式cResult: "<9（指令类型）,2（操作类型，1:SET，2:GET）,2（输入2/输出1）,1（通道0~31）,80（值）,ok（状态）>"
	sscanf(DataBuf, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s",\
	arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7], arg[8], arg[9],\
	arg[10], arg[11], arg[12], arg[13], arg[14], arg[15], arg[16], arg[17], arg[18], arg[19],\
	arg[20], arg[21], arg[22], arg[23], arg[24], arg[25], arg[26], arg[27], arg[28], arg[29], \
	arg[30], arg[31], arg[32], arg[33], arg[34], arg[35], arg[36], arg[37], arg[38], arg[39]);

	if(CtrlType == 2)
	{
	
		int chn = ChnNum + 3;
		char tmp[10];
		float Valtmp;
		sprintf(tmp,"%s",arg[chn]);
		sscanf(tmp,"%f",&Valtmp);
		int Val = Valtmp*10;//转换为web音量取值范围-720-120
		sprintf(cResult,"<9,2,2,%d,%s,ok>", ChnNum, arg[chn]);
//		sprintf(cResult,"<9,2,2,%d,%d,ok>", ChnNum, Val);
		
		mLogDebug("cResult = "<<cResult);

	}
	else//1:输出
	{
		int chn = ChnNum + 3;
		char tmp[10];
		float Valtmp;
		sprintf(tmp,"%s",arg[chn]);
		sscanf(tmp,"%f",&Valtmp);
		int Val = Valtmp*10;//转换为web音量取值范围-720-120
		
		sprintf(cResult,"<9,2,1,%d,%s,ok>", ChnNum, arg[chn]);
//		sprintf(cResult,"<9,2,1,%d,%d,ok>", ChnNum, Val);


		mLogDebug("cResult = "<<cResult);

	}
	
	return true;
#endif
}

InterfaceResCode CGetSerialCmd::GetSerialCmd(string &sResult, int ChnNum, int CtrlType)
{
	mLogInfo("获取 SerialCmd 参数...");
	//cJSON* node;
	char cResult[RES_BUF_MAXLEN] = { 0 };
	CData oResult = SVSMAP();

	   //1. 调用类内部方法进一步封装业务请求
	   if (!GetSerialCmdParamInner(oResult, cResult, ChnNum, CtrlType))
	   {
	  	 mLogError("Failed to run  GetSerialCmdParamInner(...)");
		 return eInterfaceResCodeError;
	   }
	    //2. 解析响应,存入类型为cJson对象的成员变量中
	    if (!_AnalyParas.ConvertSTRING2CJSONParseSerialCmdParam(oResult, cResult, _result, _addressList, NULL))
	   {
		mLogError("Failed to run  ConvertSTRING2CJSONParseSerialCmdParam(...)");
		return eInterfaceResCodeError;
	   }

	//3. 将step2中获取的响应格式化为Json字符串
	sResult = ComposeResult();
	cJSON_Delete(_result);

	return eInterfaceResCodeSuccess;

}


