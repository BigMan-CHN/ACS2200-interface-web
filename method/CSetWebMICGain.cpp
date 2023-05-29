#include "CSetWebMICGain.h"
#include <boost/lexical_cast.hpp>

CSetWebMICGain::CSetWebMICGain(string ip, int port, int timeOut)
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

bool CSetWebMICGain::ComposeResult(cJSON *result)
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

string CSetWebMICGain::ComposeResult()
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
bool CSetWebMICGain::SetWebMICGainInner(CData& oResult, char* cResult,int MicVal, int ChnNam)
{
	mLogDebug("run SetWebAudioConfInner(...)");

	 int SerialID = 1;//串口透传，串口1
	 int  cCmdLength   = 0;
	 char cCmdSend[1024]   = {0};
	 
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
	 //string Cmd[8] = {"<1,2,2,0,0>","<2,2,2,0,0>","<3,2,2,0,0>","<4,2,2,0,0>","<5,2,2,0,0>","<6,2,2,0,0>","<7,2,2,0,0>"};
	 //cCmdLength = strlen(Cmd.c_str());
	 //memcpy(cCmdSend, Cmd.c_str(), strlen(Cmd.c_str()));
	 float Valtmp = MicVal;
	 float GainVal = Valtmp/10;
	 //<指令类型,操作类型,输入/输出,通道,值>
	 sprintf(cCmdSend,"<8,1,2,%d,%.1f>", ChnNam, GainVal); 
	 cCmdLength = strlen(cCmdSend);
	 
	 MPSOperationRes opResCode = eMPSResultOK;
	 ResponseCode resCode = _client->GetSerialNew(SerialID, cCmdSend, cCmdLength, opResCode, oResult,cResult);
	 if(resCode != eResponseCodeSuccess){
		 mLogError("GetSerialNew(...) error:"<<resCode);
		 return false;
	 }
	//判断是否设置变声调节成功
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
	 if (strcmp("ok>", arg[5]))
	 {
		 mLogError("SetWebMICGainInner failed");
		 return false;
	 }
//	 sprintf(DataBuf,"<8,2,2,%d,%d,ok>",ChnNam-1, MicVal);
//	 stu.id = 25 + ChnNam;
//	 sprintf(stu.val,DataBuf);
//	 sprintf(stu.name,"Audio_MICWhineGain%d",ChnNam);
//	 sprintf(stu.time, "%04d/%02d/%02d-%02d:%02d:%02d", tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
//	 update_stu_db(reg_db, &stu); //add 到数据库 
	 
	 sprintf(stu.name,"Audio_MICWhineGain0");
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
	 stu.id = 11;
	 sprintf(stu.val,DataBuf);
	 sprintf(stu.name,"Audio_MICWhineGain0");
	 sprintf(stu.time, "%04d/%02d/%02d-%02d:%02d:%02d", tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
	 update_stu_db(reg_db, &stu); //add 到数据库 

	 return true;


}

InterfaceResCode CSetWebMICGain::SetWebMICGain(string &sResult,int MicVal, int ChnNam)
{
    //TODO
    mLogInfo("SetWebMICGain...");
		int cmd,val;
		cJSON* temList[10];
		char cResult[RES_BUF_MAXLEN] = { 0 };
		CData oResult = SVSMAP();

		//1. 调用类内部方法进一步封装业务请求
		if (!SetWebMICGainInner(oResult, cResult, MicVal, ChnNam))
		{
			mLogError("Failed to run  GetWebAudioConfInner(...)");
			return eInterfaceResCodeError;
		}
#if 0
		if (!_AnalyParas.ConvertSTRING2CJSONParseSerialCmdParam(oResult, cResult, _result, _addressList, NULL)
		{
		 mLogError("Failed to run  ConvertSTRING2CJSONParseSerialCmdParam(...)");
		 return eInterfaceResCodeError;
		}
#endif
		//将响应格式化为Json字符串
		sResult = ComposeResult();
		cJSON_Delete(_result);
		return eInterfaceResCodeSuccess;

}


