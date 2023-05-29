#include "CGetWebStartRec.h"
#include <boost/lexical_cast.hpp>

CGetWebStartRec::CGetWebStartRec(string ip, int port, int timeOut)
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

bool CGetWebStartRec::ComposeResult(cJSON *result)
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
    //cJSON_AddItemToObject(_result,"VideoEncChnnelList",_addressList);	
    return true;
}

string CGetWebStartRec::ComposeResult()
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
bool CGetWebStartRec::GetWebStartRecInner(CData& oResult, char* cResult, int VideoEncChnnel)
{
			mLogDebug("run GetWebStartRecInner(...)");
		
			//InterfaceResCode resCode = eInterfaceResCodeSuccess;
		
			//1. 封装业务请求
			//char szReqBody[REQ_BODY_MAXLEN] = { 0 };
			char szReqBody[2048] = { 0 };
			char *ReqBody;
			int VideoChnnel;
			memset(szReqBody,0,2048);
			sprintf(szReqBody, "eth1.conf\ncomp1.osd1.content\nnet_send100.rec_is_select\nvid_cap100.ch1.osd1.content\n");

//			sprintf(szReqBody, "eth1.conf\ncomp1.osd1.content\n");
//			for(VideoChnnel=1; VideoChnnel <= 12; VideoChnnel++)
//			{
//				ReqBody = szReqBody + strlen(szReqBody);
//				sprintf(ReqBody, "net_send%d.rec_is_select\n", VideoChnnel);
//			}
//			
//			for(VideoChnnel=1; VideoChnnel <= 8; VideoChnnel++)
//			{
//				ReqBody = szReqBody + strlen(szReqBody);
//				sprintf(ReqBody, "vid_cap%d.ch1.osd1.content\n", VideoChnnel);
//			}

			//mLogDebug("ReqBody = "<<szReqBody);
			int realBodySize = strlen(szReqBody);
		
			char szReqCmd[REQ_CMD_MAXLEN] = { 0 };
			szReqCmd[0] = 0;  //不加密
			szReqCmd[1] = 2;  //Get请求, 3是SET, 4是SYS
			szReqCmd[2] = realBodySize >> 8;
			szReqCmd[3] = realBodySize;
			memcpy(&szReqCmd[4], szReqBody, realBodySize);
		
#if 1
			//2. 发送socket请求
			MPSOperationRes opResCode = eMPSResultOK; //??
			ResponseCode resCode = _client->GetConfigNew(szReqCmd, 4 + realBodySize, opResCode, oResult, cResult);
			if (resCode != eResponseCodeSuccess) {
				mLogError("GetConfig(...) error:" << resCode);
				return false;
			}
			
#else
			//dummy response
#endif
		
			return true;

}
bool CGetWebStartRec::GetWebVideoMatrixInner( CData& oResult, char* cResult, cJSON* temList[])
{
	mLogDebug("run GetWebVideoMatrixInner(...)");

	//获取状态
	int SerialID = 0;//串口0
	int  cCmdLength   = 0;
	char cCmdSend[1024]   = {0};
	char Sendbuf[1024]   = {0};
	//cCmdLength = strlen(Cmd.c_str());
	//memcpy(cCmdSend, Cmd.c_str(), strlen(Cmd.c_str()));
	//index：命令索引，从1开始，1~50有效。

	//<指令类型,操作类型,输入/输出,通道,值>
	sprintf( cCmdSend, INTERFACE_GETWEB_SWITCH_VIDEO_SENDCMD); 
	cCmdLength = strlen(cCmdSend);
#if 1	
	MPSOperationRes opResCode = eMPSResultOK;
	//发送cCmdSend信息给服务器
	ResponseCode resCode = _client->GetSerialNew(SerialID, cCmdSend, cCmdLength, opResCode, oResult,cResult);
	if(resCode != eResponseCodeSuccess){
		mLogError("GetSerialNew(...) error:"<<resCode);
		return false;
	}

	//判断是否关机成功
	char *arg[20] = {};
	for (int i = 0; i < 20; i ++)
	{
		arg[i] = (char *)calloc(1, strlen(cResult)+1);
		if(NULL == arg[i]) 
		{
		  mLogError("calloc is error\n");
		  return false;
		 }
	 }
	mLogDebug("cResult = "<<cResult);
	sscanf(cResult, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s", arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7], arg[8], arg[9], arg[10], arg[11], arg[12], arg[13], arg[14], arg[15], arg[16], arg[17], arg[18]);
	if (strcmp("<GetVideoStatus", arg[0]))
	{
		mLogError("GetWebVideoMatrix failed \n");
		return false;
	}
	
	sqlite3 *reg_db = NULL;
	char buf[1024];
	char *register_db = "Audio_manage.db";
	
	STU stu;
	time_t cur_time = time(NULL);
	struct tm *tm_ptr;
	char find_table[1024];
	char *table = "create table MatrixName (id INTEGER, name TEXT, val TEXT, time TEXT)";
	tm_ptr = localtime(&cur_time);
	reg_db = (sqlite3 *)create_db(register_db); //创建数据库,如已创建则忽略
	create_self_db_table(reg_db, table);  //创建注册表,如已创建则忽略
	
	for(int j = 1; j < 9; j++)
	{
		int num = atoi(arg[j]);
		switch(num)
		{
			case 1:
				//cJSON_AddItemToObject(temList[j], "VideoSource", cJSON_CreateString("卡槽1-1"));
				sprintf(stu.name,"InPutChnnelName%d",num);
				sprintf(find_table,"select * from MatrixName where name='%s'", stu.name);
				break;
			case 2:
				//cJSON_AddItemToObject(temList[j], "VideoSource", cJSON_CreateString("卡槽1-2"));
				sprintf(stu.name,"InPutChnnelName%d",num);
				sprintf(find_table,"select * from MatrixName where name='%s'", stu.name);
				break;
			case 3:
				//cJSON_AddItemToObject(temList[j], "VideoSource", cJSON_CreateString("卡槽1-3"));
				sprintf(stu.name,"InPutChnnelName%d",num);
				sprintf(find_table,"select * from MatrixName where name='%s'", stu.name);
				break;
			case 4:
				//cJSON_AddItemToObject(temList[j], "VideoSource", cJSON_CreateString("卡槽1-4"));
				sprintf(stu.name,"InPutChnnelName%d",num);
				sprintf(find_table,"select * from MatrixName where name='%s'", stu.name);
				break;
			case 5:
				//cJSON_AddItemToObject(temList[j], "VideoSource", cJSON_CreateString("卡槽2-1"));
				sprintf(stu.name,"InPutChnnelName%d",num);
				sprintf(find_table,"select * from MatrixName where name='%s'", stu.name);
				break;
			case 6:
				//cJSON_AddItemToObject(temList[j], "VideoSource", cJSON_CreateString("卡槽2-2"));
				sprintf(stu.name,"InPutChnnelName%d",num);
				sprintf(find_table,"select * from MatrixName where name='%s'", stu.name);
				break;
			case 7:
				//cJSON_AddItemToObject(temList[j], "VideoSource", cJSON_CreateString("卡槽2-3"));
				sprintf(stu.name,"InPutChnnelName%d",num);
				sprintf(find_table,"select * from MatrixName where name='%s'", stu.name);
				break;
			case 8:
				//cJSON_AddItemToObject(temList[j], "VideoSource", cJSON_CreateString("卡槽2-4"));
				sprintf(stu.name,"InPutChnnelName%d",num);
				sprintf(find_table,"select * from MatrixName where name='%s'", stu.name);
				break;
			case 9:
				//cJSON_AddItemToObject(temList[j], "VideoSource", cJSON_CreateString("卡槽3-1"));
				sprintf(stu.name,"InPutChnnelName%d",num);
				sprintf(find_table,"select * from MatrixName where name='%s'", stu.name);
				break;
			case 10:
				//cJSON_AddItemToObject(temList[j], "VideoSource", cJSON_CreateString("卡槽3-2"));
				sprintf(stu.name,"InPutChnnelName%d",num);
				sprintf(find_table,"select * from MatrixName where name='%s'", stu.name);
				break;
			case 11:
				//cJSON_AddItemToObject(temList[j], "VideoSource", cJSON_CreateString("卡槽3-3"));
				sprintf(stu.name,"InPutChnnelName%d",num);
				sprintf(find_table,"select * from MatrixName where name='%s'", stu.name);
				break;
			case 12:
				//cJSON_AddItemToObject(temList[j], "VideoSource", cJSON_CreateString("卡槽3-4"));
				sprintf(stu.name,"InPutChnnelName%d",num);
				sprintf(find_table,"select * from MatrixName where name='%s'", stu.name);
				break;
		}			
		find_self_stu_db(reg_db, find_table, buf);
		cJSON_AddItemToObject(temList[j], "VideoSource", cJSON_CreateString(buf));	
	}
#endif	

	return true;

}

InterfaceResCode CGetWebStartRec::GetWebStartRec(string &sResult)
{
	   //TODO
			mLogInfo("GetWebStartRec...");
			cJSON* temList[15];
			int VideoEncChnnel = 12;
			
			char cResult[RES_BUF_MAXLEN] = { 0 };
			char VResult[RES_BUF_MAXLEN] = { 0 };
			string Method = "GetWebStartRec";
			CData oResult = SVSMAP();
			 for(int i = 1; i <= VideoEncChnnel; i++)
			{//用于生成节点，存储解析后的参数
				temList[i] = cJSON_CreateObject();
			 }
			   //1. 调用类内部方法进一步封装业务请求
			   if (!GetWebStartRecInner(oResult, cResult, VideoEncChnnel))
			   {
				 mLogError("Failed to run  GetWebStartRecInner(...)");
				 return eInterfaceResCodeError;
			   }
			   
//			   	if (!GetWebVideoMatrixInner( oResult, VResult,temList))
//				{
//					mLogError("GetWebVideoMatrixInner failed !!!");
//					return eInterfaceResCodeError;
//				}

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
					
					mLogDebug("cResult2 = "<<cResult[i]);
					
				}
			}
#endif 
		
				//2. 解析响应,存入类型为cJson对象的成员变量中
				if (!_AnalyParas.ConvertSTRING2CJSONGetWebStartRecParam(oResult, cResult,Method, _result, _addressList,temList,VideoEncChnnel, _ip))
			   {
				mLogError("Failed to run  ConvertSTRING2CJSONGetWebStartRecParam(...)");
				return eInterfaceResCodeError;
			   }
			
			 cJSON_AddItemToObject(_result, "total", cJSON_CreateNumber((VideoEncChnnel)));
			 cJSON_AddItemToObject(_result, "list", _addressList);
			//3. 将step2中获取的响应格式化为Json字符串
			sResult = ComposeResult();
			cJSON_Delete(_result);
			return eInterfaceResCodeSuccess;



#if 0	
	cJSON *listnode1 = NULL;
	cJSON *listnode2 = NULL;
	
	listnode1 = cJSON_CreateObject();
	listnode2 = cJSON_CreateObject();
	
	cJSON_AddNumberToObject(listnode1, "MosaicChn", 1);	
	cJSON_AddNumberToObject(listnode1, "MosaicPost", 0);
	cJSON_AddNumberToObject(listnode1, "MosaicNum", 8);
	cJSON_AddNumberToObject(listnode1, "MosaicType", 3);
	
	cJSON_AddNumberToObject(listnode2, "MosaicChn", 1);	
	cJSON_AddNumberToObject(listnode2, "MosaicPost", 0);
	cJSON_AddNumberToObject(listnode2, "MosaicNum", 7);
	cJSON_AddNumberToObject(listnode2, "MosaicType", 3);
	

	cJSON_AddNumberToObject(_result, "Total", 6);
	cJSON_AddItemToArray(_addressList, listnode1);
    cJSON_AddItemToArray(_addressList, listnode2);
#endif

}


