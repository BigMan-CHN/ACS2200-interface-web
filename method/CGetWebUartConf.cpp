#include "CGetWebUartConf.h"
#include <boost/lexical_cast.hpp>

CGetWebUartConf::CGetWebUartConf(string ip, int port, int timeOut)
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

bool CGetWebUartConf::ComposeResult(cJSON *result)
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

    //result中添加节点
    //cJSON_AddItemToObject(_result,"UartPortList",_addressList);	
    return true;
}

string CGetWebUartConf::ComposeResult()
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
bool CGetWebUartConf::GetWebUartConfParamInner(CData& oResult, char* cResult, int chn_num)
{
			mLogDebug("run GetWebUartConfParamInner(...)");
		
			//InterfaceResCode resCode = eInterfaceResCodeSuccess;
		
			//1. 封装业务请求
			char szReqBody[REQ_BODY_MAXLEN] = { 0 };
			
			sprintf(szReqBody, INTERFACE_GETWEB_UART_CONF_SENDCMD, chn_num, chn_num, chn_num,chn_num);
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

InterfaceResCode CGetWebUartConf::GetWebUartConf(string &sResult)
{
	mLogInfo("获取 Uart参数...");
	cJSON* temList[5];
	int chn_num;

	char cResult[RES_BUF_MAXLEN] = { 0 };
	CData oResult = SVSMAP();
	 for(chn_num=1;chn_num<4;chn_num++)
	{

	   //1. 调用类内部方法进一步封装业务请求
	   if (!GetWebUartConfParamInner(oResult, cResult,chn_num))
	   {
	  	 mLogError("Failed to run  GetWebUartConfParamInner(...)");
		 return eInterfaceResCodeError;
	   }
		//用于生成节点，存储解析后的参数
		temList[chn_num] = cJSON_CreateObject();

	    //2. 解析响应,存入类型为cJson对象的成员变量中
	    if (!_AnalyParas.ConvertSTRING2CJSONGetWebUartConfListParam(oResult, cResult, _result, _addressList,temList[chn_num],chn_num))
	   {
		mLogError("Failed to run  ConvertSTRING2CJSONGetWebUartConfListParam(...)");
		return eInterfaceResCodeError;
	   }
	}
	 cJSON_AddItemToObject(_result, "total", cJSON_CreateNumber((chn_num-1)));
	 cJSON_AddItemToObject(_result,"UartPortList",_addressList);
	//3. 将step2中获取的响应格式化为Json字符串
	sResult = ComposeResult();
	cJSON_Delete(_result);

	return eInterfaceResCodeSuccess;

}
bool CGetWebUartConf::GetWebGPIOMCUInner(CData& oResult, char* cResult)
{
			mLogDebug("run SetWebGPIOMCUInner(...)");
				
			//获取状态
			int SerialID = 0;//串口透传，串口1
			int  cCmdLength   = 0;
			char cCmdSend[1024]   = {0};
			// cCmdLength = strlen(Cmd.c_str());
			// memcpy(cCmdSend, Cmd.c_str(), strlen(Cmd.c_str()));
			 //<SetGpio,num,value>
			 sprintf(cCmdSend,"<GetGpio>");
			 cCmdLength = strlen(cCmdSend);
			 
			 MPSOperationRes opResCode = eMPSResultOK;
			 //发送cCmdSend信息给服务器
			 ResponseCode resCode = _client->GetSerialNew(SerialID, cCmdSend, cCmdLength, opResCode, oResult,cResult);
			 mLogDebug("run SetSerialConfig  cResult="<<cResult);
			 if(resCode != eResponseCodeSuccess){
				 mLogError("GetConsoleNew(...) error:"<<resCode);
				 return false;
			 }
			
			 return true;


}

InterfaceResCode CGetWebUartConf::GetWebGPIOMCU(string &sResult)
{
		//TODO
		mLogInfo("GetWebGPIOMCU...");
		
		CData oResult = SVSMAP();
		char cResult[RES_BUF_MAXLEN] = { 0 };
		cJSON *tmp[10];
		if (!GetWebGPIOMCUInner(oResult, cResult))
		{
			mLogError("GetWebGPIOMCUInner failed !!!");
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
		//判断是否开启开关成功
		 char *arg[MAX_ARG_NUM] = {};
		 for (int i = 0; i < MAX_ARG_NUM; i ++)
		 {
			 arg[i] = (char *)calloc(1, strlen(cResult)+1);
			 if(NULL == arg[i]) 
			 {
			   mLogError("calloc is error\n");
			   return eInterfaceResCodeError;
			  }
		  }
		 sscanf(cResult, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s", arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7], arg[8], arg[9], arg[10]);
		 //mLogInfo("arg[9] ="<<arg[9]<<"arg[9] ="<<arg[8]);

		 if(strcmp(arg[9],"ok>"))
		 {
			 mLogError("GetWebGPIOMCUInner recv failed");
			 return eInterfaceResCodeError;
		 }
	
		for(int i = 0; i <= 7; i++)
		{
			tmp[i] = cJSON_CreateObject();
			cJSON_AddItemToObject(tmp[i], "GPIONum", cJSON_CreateNumber(i+1));
			cJSON_AddItemToObject(tmp[i], "Value", cJSON_CreateNumber(atoi(arg[i+1])));
			cJSON_AddItemToArray(_addressList, tmp[i]);
		}
	
		cJSON_AddItemToObject(_result,"AddressList",_addressList);
		sResult = ComposeResult();
		cJSON_Delete(_result);
	
		return eInterfaceResCodeSuccess;


}
bool CGetWebUartConf::GetWebUartMCUInner(CData& oResult, char* cResult, int UartPort)
{
			mLogDebug("run GetWebUartMCUInner(...)");
				
			//获取状态
			int SerialID = 0;//串口透传，串口1
			int  cCmdLength   = 0;
			char cCmdSend[1024]   = {0};
			// cCmdLength = strlen(Cmd.c_str());
			// memcpy(cCmdSend, Cmd.c_str(), strlen(Cmd.c_str()));
			 //<SetGpio,num,value>
			 sprintf(cCmdSend,"<GetComCmd,%d,END>",UartPort);
			 cCmdLength = strlen(cCmdSend);
			 
			 MPSOperationRes opResCode = eMPSResultOK;
			 //发送cCmdSend信息给服务器
			 ResponseCode resCode = _client->GetSerialNew(SerialID, cCmdSend, cCmdLength, opResCode, oResult,cResult);
			 mLogDebug("run SetSerialConfig  cResult="<<cResult);
			 if(resCode != eResponseCodeSuccess){
				 mLogError("GetConsoleNew(...) error:"<<resCode);
				 return false;
			 }
			
			 return true;


}

InterfaceResCode CGetWebUartConf::GetWebUartMCU(string &sResult,int UartPort)
{
			//TODO
			mLogInfo("GetWebUartMCU...");
			
			CData oResult = SVSMAP();
			char cResult[RES_BUF_MAXLEN] = { 0 };
			cJSON *tmp[10];
			if (!GetWebUartMCUInner(oResult, cResult, UartPort))
			{
				mLogError("GetWebUartMCUInner failed !!!");
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
			//判断是否开启开关成功
			 char *arg[MAX_ARG_NUM] = {};
			 for (int i = 0; i < MAX_ARG_NUM; i ++)
			 {
				 arg[i] = (char *)calloc(1, strlen(cResult)+1);
				 if(NULL == arg[i]) 
				 {
				   mLogError("calloc is error\n");
				   return eInterfaceResCodeError;
				  }
			  }
			 sscanf(cResult, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s", arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7], arg[8], arg[9], arg[10]);
			 if (strcmp("ok>", arg[7]))
			 {
				 mLogError("GetWebUartMCUInner recv failed");
				 return eInterfaceResCodeError;
			 }
			//<SetComCmd,num,baudrate,parity,databits,stopbits,ok>
/*			int bitRate = atoi(arg[2]);
			switch(bitRate)
  			{
    			case 9600:
	   	 			bitRate = 1;
	   	 			break;
				case 115200:
		 			bitRate = 2;
		 			break;
				default:
		 			mLogError("please input the right bitRate!!!\n");
		 			return eInterfaceResCodeError;
			}
*/
			cJSON_AddItemToObject(_result, "UartPort", cJSON_CreateNumber(atoi(arg[1])));
			cJSON_AddItemToObject(_result, "BaudRate", cJSON_CreateNumber(atoi(arg[2])));
			cJSON_AddItemToObject(_result, "ParityBit", cJSON_CreateNumber(atoi(arg[3])));
			cJSON_AddItemToObject(_result, "DataBit",  cJSON_CreateNumber(atoi(arg[4])));
			cJSON_AddItemToObject(_result, "StopBit",  cJSON_CreateNumber(atoi(arg[5])));
			cJSON_AddItemToObject(_result, "UartState",  cJSON_CreateNumber(atoi(arg[6])));
			//cJSON_AddItemToArray(_addressList, tmp[i]);
	
			//cJSON_AddItemToObject(_result,"AddressList",_addressList);
			sResult = ComposeResult();
			cJSON_Delete(_result);
		
			return eInterfaceResCodeSuccess;


}
bool CGetWebUartConf::SetWebCloseUartInner(CData& oResult, char* cResult, int UartPort)
{
			mLogDebug("run SetWebCloseUartInner(...)");
				
			//获取状态
			int SerialID = 0;//串口透传，串口1
			int  cCmdLength   = 0;
			char cCmdSend[1024]   = {0};
			// cCmdLength = strlen(Cmd.c_str());
			// memcpy(cCmdSend, Cmd.c_str(), strlen(Cmd.c_str()));
			 //<SetGpio,num,value>
			 sprintf(cCmdSend,"<CloseComCmd,%d,END>",UartPort);
			 cCmdLength = strlen(cCmdSend);
			 
			 MPSOperationRes opResCode = eMPSResultOK;
			 //发送cCmdSend信息给服务器
			 ResponseCode resCode = _client->GetSerialNew(SerialID, cCmdSend, cCmdLength, opResCode, oResult,cResult);
			 mLogDebug("run SetSerialConfig  cResult="<<cResult);
			 if(resCode != eResponseCodeSuccess){
				 mLogError("GetConsoleNew(...) error:"<<resCode);
				 return false;
			 }
			
			 return true;


}

InterfaceResCode CGetWebUartConf::SetWebCloseUart(string &sResult,int UartPort)
{
			//TODO
			mLogInfo("SetWebCloseUart...");
			
			CData oResult = SVSMAP();
			char cResult[RES_BUF_MAXLEN] = { 0 };
			cJSON *tmp[10];
			if (!SetWebCloseUartInner(oResult, cResult, UartPort))
			{
				mLogError("SetWebCloseUartInner failed !!!");
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
			//判断是否开启开关成功
			 char *arg[MAX_ARG_NUM] = {};
			 for (int i = 0; i < MAX_ARG_NUM; i ++)
			 {
				 arg[i] = (char *)calloc(1, strlen(cResult)+1);
				 if(NULL == arg[i]) 
				 {
				   mLogError("calloc is error\n");
				   return eInterfaceResCodeError;
				  }
			  }
			 sscanf(cResult, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s", arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7], arg[8], arg[9], arg[10]);
			 //<CloseComCmd,ok>
			 if (strcmp("ok>", arg[1]))
			 {
				 mLogError("SetWebCloseUartInner recv failed");
				 return eInterfaceResCodeError;
			 }

			sResult = ComposeResult();
			cJSON_Delete(_result);
		
			return eInterfaceResCodeSuccess;


}
bool CGetWebUartConf::GetGPIOConfigInner(CData& oResult, char* cResult, string requestStr)
{
	mLogDebug("run GetGPIOConfigInner(...)");
		
	//获取状态
	int SerialID = 4;//UDP端口：6872
	int  cCmdLength   = 0;
	char cCmdSend[2048]   = {0};

	 sprintf(cCmdSend,"%s", requestStr.c_str());
	 cCmdLength = strlen(cCmdSend);
	 
//	 mLogDebug("cCmdSend = "<<cCmdSend);

	 MPSOperationRes opResCode = eMPSResultOK;
	 //发送cCmdSend信息给服务器
	 ResponseCode resCode = _client->GetSerialNew(SerialID, cCmdSend, cCmdLength, opResCode, oResult,cResult);
//	 mLogDebug("run SetSerialConfig  cResult="<<cResult);
	 if(resCode != eResponseCodeSuccess){
		 mLogError("GetSerialNew(...) error:"<<resCode);
		 return false;
	 }
	
	 return true;
}
InterfaceResCode CGetWebUartConf::GetGPIOConfig(string &sResult, string requestStr)
{
    //TODO
	mLogInfo("SetGPIOConfig...");
	
	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };
	
	if (!GetGPIOConfigInner(oResult, cResult,requestStr))
	{
		mLogError("GetGPIOConfigInner failed !!!");
		return eInterfaceResCodeError;
	}

	//解析返回的JSON数据
	cJSON *root = cJSON_Parse(cResult);
	cJSON *Params = cJSON_GetObjectItem(root,"params");
	
	cJSON *value1 = cJSON_GetObjectItem(Params,"value1");
	cJSON *value2 = cJSON_GetObjectItem(Params,"value2");
	cJSON *value3 = cJSON_GetObjectItem(Params,"value3");
	cJSON *value4 = cJSON_GetObjectItem(Params,"value4");
	cJSON *value5 = cJSON_GetObjectItem(Params,"value5");
	cJSON *value6 = cJSON_GetObjectItem(Params,"value6");

	
	cJSON_AddItemToObject(_result,"GPIO1",cJSON_CreateNumber(value1->valueint));
	cJSON_AddItemToObject(_result,"GPIO2",cJSON_CreateNumber(value2->valueint));
	cJSON_AddItemToObject(_result,"GPIO3",cJSON_CreateNumber(value3->valueint));
	cJSON_AddItemToObject(_result,"GPIO4",cJSON_CreateNumber(value4->valueint));
	cJSON_AddItemToObject(_result,"GPIO5",cJSON_CreateNumber(value5->valueint));
	cJSON_AddItemToObject(_result,"GPIO6",cJSON_CreateNumber(value6->valueint));
	
	cJSON_Delete(root);

    sResult = ComposeResult();
	cJSON_Delete(_result);

    return eInterfaceResCodeSuccess;
}


