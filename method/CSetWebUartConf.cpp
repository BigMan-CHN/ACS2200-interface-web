#include "CSetWebUartConf.h"
#include <boost/lexical_cast.hpp>

CSetWebUartConf::CSetWebUartConf(string ip, int port, int timeOut)
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

bool CSetWebUartConf::ComposeResult(cJSON *result)
{
    if(_result == NULL || result == NULL){
        mLogError("cJSON result == NULL.");
        result = NULL;
        return false;
    }

    //result中添加节点
	//cJSON_AddItemToObject(_result,"Total",_total);
    //cJSON_AddItemToObject(_result,"AddressList",_addressList);	
    return true;
}

string CSetWebUartConf::ComposeResult()
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
bool CSetWebUartConf::SetWebUartConfInner(int          UartPort, int BaudRate, int DateBit, int StopBit, int ParityBit, CData& oResult, char* cResult)
{
	mLogDebug("run SetWebUartConfInner(...)");
		
	char szReqBody[REQ_BODY_MAXLEN] = { 0 };
	int bitRate;
	switch(BaudRate)
  {
    case 1:
	   	 bitRate = 2400;
	   	 break;
	case 2:
		 bitRate = 4800;
		 break;
	case 3:
		 bitRate = 7200;
		 break;
	case 4:
		 bitRate = 9600;
		 break;
	case 5:
		 bitRate = 115200;
		 break;
	case 6:
		 bitRate = 14400;
		 break;
	case 7:
		 bitRate = 19200;
		 break;
	default:
		 mLogError("please input the right bitRate!!!\n");
		 return false;
	}
	sprintf(szReqBody,INTERFACE_SETWEB_UART_CONF_SENDCMD, UartPort, bitRate, UartPort, DateBit, UartPort, StopBit, UartPort, ParityBit);
		
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
		mLogError("SetWebUartConfInner Run Failed !!!");
		return false;
	}
#endif
		
	return true;

}
InterfaceResCode CSetWebUartConf::SetWebUartConf(string &sResult,int UartPort,int BaudRate,int DateBit,int StopBit,int ParityBit)
{
    //TODO
	mLogInfo("设置 Uart 参数...");
	
	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };
	
	if (!SetWebUartConfInner( UartPort, BaudRate, DateBit, StopBit, ParityBit, oResult, cResult))
	{
		mLogError("SetWebUartConfInner failed !!!");
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

bool CSetWebUartConf::SetWebOpenUartInner(int UartPort,int BaudRate,int DateBit,int StopBit,int ParityBit, CData& oResult, char* cResult)
{
	mLogDebug("run SetWebOpenUartInner(...)");
		
	//获取状态
	int SerialID = 0;//串口透传，串口1
	int  cCmdLength   = 0;
	int  DataLength   = 0;
	char cCmdSend[1024]   = {0};
	int bitRate;

	switch(ParityBit)
   	{
	    case 0:
		   	 DateBit = 8;
		   	 break;
		case 1:
			 DateBit = 9;
			 break;
		default:
			 mLogError("please input the right ParityBit!!!\n");
			 return false;
	}
	
	 //<OpenComCmd,num,baudrate,parity,databits,stopbits,END>
	 sprintf(cCmdSend,"<OpenComCmd,%d,%d,%d,%d,%d,END>", UartPort, BaudRate, ParityBit, DateBit, StopBit);
	 //sprintf(cCmdSend,"<SetComCmd,%d,%s,%s,%d,%d,%d,%s>", UartPort, BaudRate.c_str(), ParityBit.c_str(), DateBit, StopBit, DateType, Data.c_str());
	 cCmdLength = strlen(cCmdSend);
	 
	 mLogDebug("cCmdSend = "<<cCmdSend);
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
InterfaceResCode CSetWebUartConf::SetWebOpenUart(string &sResult,int UartPort, int BaudRate,int DateBit,int StopBit, int ParityBit)
{
    //TODO
	mLogInfo("SetWebOpenUart...");
	
	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };
	
	if (!SetWebOpenUartInner( UartPort, BaudRate, DateBit, StopBit, ParityBit, oResult, cResult))
	{
		mLogError("SetWebOpenUartInner failed !!!");
		return eInterfaceResCodeError;
	}

	//解析	<OpenComCmd,ok>	
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

	if (strcmp("ok>", arg[1]))
	{
		mLogError("SetWebOpenUartInner recv failed");
		return eInterfaceResCodeError;
	}

    sResult = ComposeResult();
	cJSON_Delete(_result);

    return eInterfaceResCodeSuccess;
}
bool CSetWebUartConf::SetWebSendANDRecvUartInner(int UartPort, int DateType, string Data, CData& oResult, char* cResult)
{
	mLogDebug("run SetWebOpenUartInner(...)");
		
	//获取状态
	int SerialID = 0;//串口透传，串口1
	int  cCmdLength   = 0;
	int  DataLength   = 0;
	char cCmdSend[1024]   = {0};
	char *Datastr;
	int bitRate;

	switch(DateType)
   	{
	    case 0:
		   	 DataLength = Data.length();
		   	 break;
		case 1:
			 DataLength = 1;
			 break;
		default:
			 mLogError("please input the right DateType!!!\n");
			 return false;
	}
	
	 //<SendComCmd,num,type,buflength,string,END>
	 sprintf(cCmdSend,"<SendComCmd,%d,%d,%d,%s,END>", UartPort, DateType, DataLength, Data.c_str());
	 //sprintf(cCmdSend,"<SetComCmd,%d,%s,%s,%d,%d,%d,%s>", UartPort, BaudRate.c_str(), ParityBit.c_str(), DateBit, StopBit, DateType, Data.c_str());
	 cCmdLength = strlen(cCmdSend);
	 
	 mLogDebug("cCmdSend = "<<cCmdSend);
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
InterfaceResCode CSetWebUartConf::SetWebSendANDRecvUart(string &sResult, int UartPort, int DateType, string Data)
{
    //TODO
	mLogInfo("SetWebSendANDRecvUart...");
	
	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };
	
	if (!SetWebSendANDRecvUartInner( UartPort, DateType, Data, oResult, cResult))
	{
		mLogError("SetWebSendANDRecvUartInner failed !!!");
		return eInterfaceResCodeError;
	}

	//解析	<RecvComCmd,num,type,buflength,recvbuf,ok>	
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

	if (strcmp("<RecvComCmd", arg[0]))
	{
		mLogError("SetWebSendANDRecvUartInner recv failed");
		return eInterfaceResCodeError;
	}

	char *recvbuf_head = cResult + strlen(arg[0]) + strlen(arg[1]) + strlen(arg[2]) + strlen(arg[3])+ 4;
	int buflength  = atoi(arg[3]);
	char recvbuf[RES_BUF_MAXLEN] = { 0 };
	memset(recvbuf , 0, RES_BUF_MAXLEN);
	memcpy(recvbuf, recvbuf_head, buflength);
	mLogDebug("SetWebSendANDRecvUartInner recvbuf = "<<recvbuf);

	cJSON_AddItemToObject(_result, "Data", cJSON_CreateString(recvbuf));
	cJSON_AddItemToObject(_result, "UartPort", cJSON_CreateNumber(atoi(arg[1])));
	cJSON_AddItemToObject(_result, "DateType", cJSON_CreateNumber(atoi(arg[2])));
	cJSON_AddItemToObject(_result, "Buflength", cJSON_CreateNumber(atoi(arg[3])));
	
    sResult = ComposeResult();
	cJSON_Delete(_result);

    return eInterfaceResCodeSuccess;
}

bool CSetWebUartConf::SetWebGPIOMCUInner(int GPIONum,int Value, CData& oResult, char* cResult)
{
	  mLogDebug("run SetWebGPIOMCUInner(...)");
		  
	  //获取状态
	  int SerialID = 0;//串口透传，串口1
	  int  cCmdLength	= 0;
	  char cCmdSend[1024]	= {0};
	  // cCmdLength = strlen(Cmd.c_str());
	  // memcpy(cCmdSend, Cmd.c_str(), strlen(Cmd.c_str()));
	   //<SetGpio,num,value>
	   sprintf(cCmdSend,"<SetGpio,%d,%d>", GPIONum, Value);
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
InterfaceResCode CSetWebUartConf::SetWebGPIOMCU(string &sResult,int GPIONum,int Value)
{
    //TODO
	mLogInfo("SetWebGPIOMCU...");
	
	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };
	
	if (!SetWebGPIOMCUInner( GPIONum, Value,oResult, cResult))
	{
		mLogError("SetWebGPIOMCUInner failed !!!");
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
	 sscanf(cResult, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s", arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7], arg[8], arg[9]);
	 //mLogInfo("arg[2] ="<<arg[2]<<"arg[3] ="<<arg[3]);

	 if(strcmp(arg[3],"ok>"))
	 {
		 mLogError("open or off button failed");
		 return eInterfaceResCodeError;
	 }
    sResult = ComposeResult();
	cJSON_Delete(_result);

    return eInterfaceResCodeSuccess;
}
bool CSetWebUartConf::UartSetConfigInner(int num, int baudrate,int parity,int databits, int stopbits, string requestStr, CData& oResult, char* cResult)
{
	mLogDebug("run UartSetConfigInner(...)");
		
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
		 mLogError("GetConsoleNew(...) error:"<<resCode);
		 return false;
	 }
	
	 return true;
}
InterfaceResCode CSetWebUartConf::UartSetConfig(string &sResult,int num, int baudrate,int parity,int databits, int stopbits, string requestStr)
{
    //TODO
	mLogInfo("UartSetConfig...");
	
	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };
	
	if (!UartSetConfigInner( num, baudrate, parity, databits, stopbits,requestStr, oResult, cResult))
	{
		mLogError("UartSetConfigInner failed !!!");
		return eInterfaceResCodeError;
	}
	//解析返回的JSON数据
	cJSON *root = cJSON_Parse(cResult);
	cJSON *Params = cJSON_GetObjectItem(root,"params");
	cJSON *status = cJSON_GetObjectItem(Params,"status");

	if (strcmp("ok", status->valuestring))
	{
		mLogError("UartSetConfigInner recv failed");
		return eInterfaceResCodeError;
	}
	cJSON_Delete(root);
    sResult = ComposeResult();
	cJSON_Delete(_result);
    return eInterfaceResCodeSuccess;
}
bool CSetWebUartConf::UartSendDataInner(int num, int type, int buflength, string sendstring, string requestStr, CData& oResult, char* cResult)
{
	mLogDebug("run UartSendDataInner(...)");
		
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
		 mLogError("GetConsoleNew(...) error:"<<resCode);
		 return false;
	 }
	
	 return true;
}
InterfaceResCode CSetWebUartConf::UartSendData(string &sResult, int num, int type, int buflength, string sendstring, string requestStr)
{
    //TODO
	mLogInfo("UartSetConfig...");
	
	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };
	
	if (!UartSendDataInner( num, type, buflength, sendstring, requestStr, oResult, cResult))
	{
		mLogError("UartSendDataInner failed !!!");
		return eInterfaceResCodeError;
	}

	//解析返回的JSON数据
	cJSON *root = cJSON_Parse(cResult);
	cJSON *Params = cJSON_GetObjectItem(root,"params");
	cJSON *status = cJSON_GetObjectItem(Params,"status");
	cJSON *datatype = cJSON_GetObjectItem(Params,"type");
	cJSON *recvstring = cJSON_GetObjectItem(Params,"string");
	cJSON *recvHex = cJSON_GetObjectItem(Params,"hex");

	if (strcmp("ok", status->valuestring))
	{
		mLogError("UartSendDataInner recv failed");
		return eInterfaceResCodeError;
	}
	cJSON_AddItemToObject(_result,"type",cJSON_CreateNumber(datatype->valueint));
	cJSON_AddItemToObject(_result,"string",cJSON_CreateString(recvstring->valuestring));
	cJSON_AddItemToObject(_result,"hexbuf",cJSON_CreateString(recvHex->valuestring));
	cJSON_Delete(root);
    sResult = ComposeResult();
	cJSON_Delete(_result);

    return eInterfaceResCodeSuccess;
}
bool CSetWebUartConf::UartGetConfigInner(int num, string requestStr, CData& oResult, char* cResult)
{
	mLogDebug("run UartGetConfigInner(...)");
		
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
		 mLogError("GetConsoleNew(...) error:"<<resCode);
		 return false;
	 }
	
	 return true;
}
InterfaceResCode CSetWebUartConf::UartGetConfig(string &sResult,int num, string requestStr)
{
    //TODO
	mLogInfo("UartGetConfig...");
	
	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };
	
	if (!UartGetConfigInner( num, requestStr, oResult, cResult))
	{
		mLogError("UartGetConfigInner failed !!!");
		return eInterfaceResCodeError;
	}

	//解析返回的JSON数据
	cJSON *root = cJSON_Parse(cResult);//将char型数据转换为JSON数据
	cJSON *Params = cJSON_GetObjectItem(root,"params");//从root下获取params数据

	cJSON *uartnum = cJSON_GetObjectItem(Params,"num");
	cJSON *baudrate = cJSON_GetObjectItem(Params,"baudrate");
	cJSON *parity = cJSON_GetObjectItem(Params,"parity");
	cJSON *databits = cJSON_GetObjectItem(Params,"databits");
	cJSON *stopbits = cJSON_GetObjectItem(Params,"stopbits");

	cJSON_AddItemToObject(_result,"num",cJSON_CreateNumber(uartnum->valueint));
	cJSON_AddItemToObject(_result,"baudrate",cJSON_CreateNumber(baudrate->valueint));
	cJSON_AddItemToObject(_result,"parity",cJSON_CreateNumber(parity->valueint));
	cJSON_AddItemToObject(_result,"databits",cJSON_CreateNumber(databits->valueint));
	cJSON_AddItemToObject(_result,"stopbits",cJSON_CreateNumber(stopbits->valueint));
	cJSON_Delete(root);
    sResult = ComposeResult();
	cJSON_Delete(_result);

    return eInterfaceResCodeSuccess;
}
bool CSetWebUartConf::UartCreateTableInner(string dev_name, int dev_type, int dev_protocol, string requestStr, CData& oResult, char* cResult)
{
	mLogDebug("run UartCreateTableInner(...)");
		
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
InterfaceResCode CSetWebUartConf::UartCreateTable(string &sResult, string dev_name, int dev_type, int dev_protocol, string requestStr)
{
    //TODO
	mLogInfo("UartCreateTable...");
	
	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };
	
	if (!UartCreateTableInner(dev_name, dev_type, dev_protocol, requestStr, oResult, cResult))
	{
		mLogError("UartCreateTableInner failed !!!");
		return eInterfaceResCodeError;
	}
	//解析返回的JSON数据
	cJSON *root = cJSON_Parse(cResult);
	cJSON *Params = cJSON_GetObjectItem(root,"params");
	
	cJSON *status = cJSON_GetObjectItem(Params,"status");

	if (strcmp("ok", status->valuestring))
	{
		mLogError("UartCreateTableInner recv failed");
		return eInterfaceResCodeError;
	}
	
	cJSON_Delete(root);
    sResult = ComposeResult();
	cJSON_Delete(_result);

    return eInterfaceResCodeSuccess;
}
bool CSetWebUartConf::UartDelTableInner( string dev_name, string requestStr, CData& oResult, char* cResult)
{
	mLogDebug("run UartDelTableInner(...)");
		
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
InterfaceResCode CSetWebUartConf::UartDelTable(string &sResult, string dev_name, string requestStr)
{
    //TODO
	mLogInfo("UartDelTable...");
	
	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };
	
	if (!UartDelTableInner( dev_name, requestStr, oResult, cResult))
	{
		mLogError("UartDelTableInner failed !!!");
		return eInterfaceResCodeError;
	}

	//解析返回的JSON数据
	cJSON *root = cJSON_Parse(cResult);
	cJSON *Params = cJSON_GetObjectItem(root,"params");
	
	cJSON *status = cJSON_GetObjectItem(Params,"status");

	if (strcmp("ok", status->valuestring))
	{
		mLogError("UartDelTableInner recv failed");
		return eInterfaceResCodeError;
	}
	
	cJSON_Delete(root);

    sResult = ComposeResult();
	cJSON_Delete(_result);

    return eInterfaceResCodeSuccess;
}
bool CSetWebUartConf::UartSearchTableInner(string requestStr, CData& oResult, char* cResult)
{
	mLogDebug("run UartSearchTableInner(...)");
		
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
InterfaceResCode CSetWebUartConf::UartSearchTable(string &sResult, string requestStr)
{
    //TODO
	mLogInfo("UartSearchTable...");
	
	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };
	
	if (!UartSearchTableInner( requestStr, oResult, cResult))
	{
		mLogError("UartSearchTableInner failed !!!");
		return eInterfaceResCodeError;
	}

	//解析返回的JSON数据
	cJSON *root = cJSON_Parse(cResult);//将char型数据转换为JSON数据
	cJSON *Params = cJSON_GetObjectItem(root,"params");//从root下获取params数组数据
//	int arraysize = cJSON_GetArraySize(Params);//数组大小
//	for(int i = 0; i < arraysize; i++)
//	{ 
//		cJSON *dev_name = cJSON_GetObjectItem(Params,"dev_name");
//		cJSON *dev_type = cJSON_GetObjectItem(Params,"dev_type");
//		cJSON *dev_protocol = cJSON_GetObjectItem(Params,"dev_protocol");
//		cJSON_AddItemToArray(_addressList, dev_name);
//		cJSON_AddItemToArray(_addressList, dev_type);
//		cJSON_AddItemToArray(_addressList, dev_protocol);
//	}

	cJSON_AddItemToObject(_result,"list",Params);
//	cJSON_Delete(root);
    sResult = ComposeResult();
	cJSON_Delete(_result);

    return eInterfaceResCodeSuccess;
}
bool CSetWebUartConf::UartAlterTableInner(string dev_name, string new_dev_name, int dev_type,int dev_protocol, string requestStr, CData& oResult, char* cResult)
{
	mLogDebug("run UartAlterTableInner(...)");
		
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
InterfaceResCode CSetWebUartConf::UartAlterTable(string &sResult, string dev_name, string new_dev_name, int dev_type,int dev_protocol, string requestStr)
{
    //TODO
	mLogInfo("UartAlterTable...");
	
	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };
	
	if (!UartAlterTableInner( dev_name, new_dev_name, dev_type, dev_protocol, requestStr, oResult, cResult))
	{
		mLogError("UartAlterTableInner failed !!!");
		return eInterfaceResCodeError;
	}
	//解析返回的JSON数据
	cJSON *root = cJSON_Parse(cResult);
	cJSON *Params = cJSON_GetObjectItem(root,"params");
	
	cJSON *status = cJSON_GetObjectItem(Params,"status");

	if (strcmp("ok", status->valuestring))
	{
		mLogError("UartAlterTableInner recv failed");
		return eInterfaceResCodeError;
	}
	
	cJSON_Delete(root);

    sResult = ComposeResult();
	cJSON_Delete(_result);

    return eInterfaceResCodeSuccess;
}
bool CSetWebUartConf::UartAddTableControlCmdInner( string dev_name, string cmd_recv_code, string cmd_fun_des, string requestStr, CData& oResult, char* cResult)
{
	mLogDebug("run UartAddTableControlCmdInner(...)");
		
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
InterfaceResCode CSetWebUartConf::UartAddTableControlCmd(string &sResult, string dev_name, string cmd_recv_code, string cmd_fun_des, string requestStr)
{
    //TODO
	mLogInfo("UartAddTableControlCmd...");
	
	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };
	
	if (!UartAddTableControlCmdInner( dev_name, cmd_recv_code, cmd_fun_des, requestStr, oResult, cResult))
	{
		mLogError("UartAddTableControlCmdInner failed !!!");
		return eInterfaceResCodeError;
	}

	//解析返回的JSON数据
	cJSON *root = cJSON_Parse(cResult);
	cJSON *Params = cJSON_GetObjectItem(root,"params");
	
	cJSON *status = cJSON_GetObjectItem(Params,"status");

	if (strcmp("ok", status->valuestring))
	{
		mLogError("UartAddTableControlCmdInner recv failed");
		return eInterfaceResCodeError;
	}
	
	cJSON_Delete(root);

    sResult = ComposeResult();
	cJSON_Delete(_result);

    return eInterfaceResCodeSuccess;
}
bool CSetWebUartConf::UartAddTableCenterControlCmdInner(string dev_name, string cmd_recv_code, int cmd_type, string data_buf, int protocol_type,  string cmd_name, string requestStr, CData& oResult, char* cResult)
{
	mLogDebug("run UartAddTableCenterControlCmdInner(...)");
		
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
InterfaceResCode CSetWebUartConf::UartAddTableCenterControlCmd(string &sResult, string dev_name, string cmd_recv_code, int cmd_type, string data_buf, int protocol_type,  string cmd_name, string requestStr)
{
    //TODO
	mLogInfo("UartAddTableControlCmd...");
	
	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };
	
	if (!UartAddTableCenterControlCmdInner( dev_name, cmd_recv_code, cmd_type, data_buf, protocol_type, cmd_name, requestStr, oResult, cResult))
	{
		mLogError("UartAddTableCenterControlCmdInner failed !!!");
		return eInterfaceResCodeError;
	}

	//解析返回的JSON数据
	cJSON *root = cJSON_Parse(cResult);
	cJSON *Params = cJSON_GetObjectItem(root,"params");
	
	cJSON *status = cJSON_GetObjectItem(Params,"status");

	if (strcmp("ok", status->valuestring))
	{
		mLogError("UartAddTableCenterControlCmdInner recv failed");
		return eInterfaceResCodeError;
	}
	
	cJSON_Delete(root);

    sResult = ComposeResult();
	cJSON_Delete(_result);

    return eInterfaceResCodeSuccess;
}
bool CSetWebUartConf::UartAddTableCameraCmdInner(string dev_name, string cmd_name, int protocol_type, int cmd_protocol_type, int cmd_holder_addr,string cmd_correlation_vi,string requestStr, CData& oResult, char* cResult)
{
	mLogDebug("run UartAddTableCameraCmdInner(...)");
		
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
		 mLogError("GetConsoleNew(...) error:"<<resCode);
		 return false;
	 }
	
	 return true;
}
InterfaceResCode CSetWebUartConf::UartAddTableCameraCmd(string &sResult, string dev_name, string cmd_name, int protocol_type, int cmd_protocol_type, int cmd_holder_addr,string cmd_correlation_vi,string requestStr)
{
    //TODO
	mLogInfo("UartAddTableCameraCmd...");
	
	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };
	
	if (!UartAddTableCameraCmdInner( dev_name, cmd_name, protocol_type, cmd_protocol_type, cmd_holder_addr, cmd_correlation_vi, requestStr, oResult, cResult))
	{
		mLogError("UartAddTableCameraCmdInner failed !!!");
		return eInterfaceResCodeError;
	}

	//解析返回的JSON数据
	cJSON *root = cJSON_Parse(cResult);
	cJSON *Params = cJSON_GetObjectItem(root,"params");
	
	cJSON *status = cJSON_GetObjectItem(Params,"status");

	if (strcmp("ok", status->valuestring))
	{
		mLogError("UartAddTableCameraCmdInner recv failed");
		return eInterfaceResCodeError;
	}
	
	cJSON_Delete(root);

    sResult = ComposeResult();
	cJSON_Delete(_result);

    return eInterfaceResCodeSuccess;
}


bool CSetWebUartConf::UartDelTableControlCmdInner(string dev_name, string cmd_recv_code, string requestStr, CData& oResult, char* cResult)
{
	mLogDebug("run UartDelTableControlCmdInner(...)");
		
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
InterfaceResCode CSetWebUartConf::UartDelTableControlCmd(string &sResult, string dev_name, string cmd_recv_code, string requestStr)
{
    //TODO
	mLogInfo("UartDelTableControlCmd...");
	
	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };
	
	if (!UartDelTableControlCmdInner( dev_name, cmd_recv_code, requestStr, oResult, cResult))
	{
		mLogError("UartDelTableControlCmdInner failed !!!");
		return eInterfaceResCodeError;
	}

	//解析返回的JSON数据
	cJSON *root = cJSON_Parse(cResult);
	cJSON *Params = cJSON_GetObjectItem(root,"params");
	
	cJSON *status = cJSON_GetObjectItem(Params,"status");

	if (strcmp("ok", status->valuestring))
	{
		mLogError("UartDelTableControlCmdInner recv failed");
		return eInterfaceResCodeError;
	}
	
	cJSON_Delete(root);

    sResult = ComposeResult();
	cJSON_Delete(_result);

    return eInterfaceResCodeSuccess;
}
bool CSetWebUartConf::UartDelTableControlExecuteCmdInner(string dev_name, string cmd_recv_code, string cmd_name, string requestStr, CData& oResult, char* cResult)
{
	mLogDebug("run UartDelTableControlExecuteCmdInner(...)");
		
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
InterfaceResCode CSetWebUartConf::UartDelTableControlExecuteCmd(string &sResult, string dev_name, string cmd_recv_code, string cmd_name, string requestStr)
{
    //TODO
	mLogInfo("UartDelTableControlCmd...");
	
	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };
	
	if (!UartDelTableControlExecuteCmdInner( dev_name, cmd_recv_code, cmd_name, requestStr, oResult, cResult))
	{
		mLogError("UartDelTableControlExecuteCmdInner failed !!!");
		return eInterfaceResCodeError;
	}

	//解析返回的JSON数据
	cJSON *root = cJSON_Parse(cResult);
	cJSON *Params = cJSON_GetObjectItem(root,"params");
	
	cJSON *status = cJSON_GetObjectItem(Params,"status");

	if (strcmp("ok", status->valuestring))
	{
		mLogError("UartDelTableControlExecuteCmdInner recv failed");
		return eInterfaceResCodeError;
	}
	
	cJSON_Delete(root);

    sResult = ComposeResult();
	cJSON_Delete(_result);

    return eInterfaceResCodeSuccess;
}
bool CSetWebUartConf::UartDelTableControlCameraCmdInner(string dev_name, string cmd_name, string requestStr, CData& oResult, char* cResult)
{
	mLogDebug("run UartDelTableControlCameraCmdInner(...)");
		
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
InterfaceResCode CSetWebUartConf::UartDelTableControlCameraCmd(string &sResult, string dev_name, string cmd_name, string requestStr)
{
    //TODO
	mLogInfo("UartDelTableControlCameraCmd...");
	
	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };
	
	if (!UartDelTableControlCameraCmdInner( dev_name, cmd_name, requestStr, oResult, cResult))
	{
		mLogError("UartDelTableControlCameraCmdInner failed !!!");
		return eInterfaceResCodeError;
	}

	//解析返回的JSON数据
	cJSON *root = cJSON_Parse(cResult);
	cJSON *Params = cJSON_GetObjectItem(root,"params");
	
	cJSON *status = cJSON_GetObjectItem(Params,"status");

	if (strcmp("ok", status->valuestring))
	{
		mLogError("UartDelTableControlCameraCmdInner recv failed");
		return eInterfaceResCodeError;
	}
	
	cJSON_Delete(root);

    sResult = ComposeResult();
	cJSON_Delete(_result);

    return eInterfaceResCodeSuccess;
}


bool CSetWebUartConf::UartSearchTableControlCmdInner(string dev_name, string requestStr, CData& oResult, char* cResult)
{
	mLogDebug("run UartSearchTableControlCmdInner(...)");
		
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
		 mLogError("GetConsoleNew(...) error:"<<resCode);
		 return false;
	 }
	
	 return true;
}
InterfaceResCode CSetWebUartConf::UartSearchTableControlCmd(string &sResult, string dev_name, string requestStr)
{
    //TODO
	mLogInfo("UartSearchTableControlCmd...");
	
	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };
	
	if (!UartSearchTableControlCmdInner( dev_name, requestStr, oResult, cResult))
	{
		mLogError("UartSearchTableControlCmdInner failed !!!");
		return eInterfaceResCodeError;
	}

	//解析返回的JSON数据
	cJSON *root = cJSON_Parse(cResult);//将char型数据转换为JSON数据
	cJSON *Params = cJSON_GetObjectItem(root,"params");//从root下获取params数组数据
//	int arraysize = cJSON_GetArraySize(Params);//数组大小
//	for(int i = 0; i < arraysize; i++)
//	{ 
//		cJSON *dev_name = cJSON_GetObjectItem(Params,"dev_name");
//		cJSON *dev_type = cJSON_GetObjectItem(Params,"dev_type");
//		cJSON *dev_protocol = cJSON_GetObjectItem(Params,"dev_protocol");
//		cJSON_AddItemToArray(_addressList, dev_name);
//		cJSON_AddItemToArray(_addressList, dev_type);
//		cJSON_AddItemToArray(_addressList, dev_protocol);
//	}

	cJSON_AddItemToObject(_result,"list",Params);
//	cJSON_Delete(root);
    sResult = ComposeResult();
	cJSON_Delete(_result);

    return eInterfaceResCodeSuccess;
}
bool CSetWebUartConf::UartSearchTableControlExecuteCmdInner( string dev_name, string cmd_recv_code, string requestStr, CData& oResult, char* cResult)
{
	mLogDebug("run UartSearchTableControlExecuteCmdInner(...)");
		
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
InterfaceResCode CSetWebUartConf::UartSearchTableControlExecuteCmd(string &sResult, string dev_name, string cmd_recv_code, string requestStr)
{
    //TODO
	mLogInfo("UartSearchTableControlExecuteCmd...");
	
	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };
	
	if (!UartSearchTableControlExecuteCmdInner( dev_name, cmd_recv_code, requestStr, oResult, cResult))
	{
		mLogError("UartSearchTableControlExecuteCmdInner failed !!!");
		return eInterfaceResCodeError;
	}

	//解析返回的JSON数据
	cJSON *root = cJSON_Parse(cResult);//将char型数据转换为JSON数据
	cJSON *Params = cJSON_GetObjectItem(root,"params");//从root下获取params数组数据
//	int arraysize = cJSON_GetArraySize(Params);//数组大小
//	for(int i = 0; i < arraysize; i++)
//	{ 
//		cJSON *dev_name = cJSON_GetObjectItem(Params,"dev_name");
//		cJSON *dev_type = cJSON_GetObjectItem(Params,"dev_type");
//		cJSON *dev_protocol = cJSON_GetObjectItem(Params,"dev_protocol");
//		cJSON_AddItemToArray(_addressList, dev_name);
//		cJSON_AddItemToArray(_addressList, dev_type);
//		cJSON_AddItemToArray(_addressList, dev_protocol);
//	}

	cJSON_AddItemToObject(_result,"list",Params);
//	cJSON_Delete(root);
    sResult = ComposeResult();
	cJSON_Delete(_result);

    return eInterfaceResCodeSuccess;
}
bool CSetWebUartConf::UartSearchCameraTableControlCmdInner( string dev_name, string requestStr, CData& oResult, char* cResult)
{
	mLogDebug("run UartSearchCameraTableControlCmdInner(...)");
		
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
		 mLogError("GetConsoleNew(...) error:"<<resCode);
		 return false;
	 }
	
	 return true;
}
bool CSetWebUartConf::GetUartHttpUrlInner(CData &oResult, char *cResult)
{
    mLogDebug("run GetUartHttpUrlInner(...)");
    //获取状态

    //add fwb 20201110
    char cChnCmd[4096] = {0};
	char *ReqBody;
	int flag = 0;
#if 1
	sprintf(cChnCmd,"eth1.conf\n");

#endif 

    //add fwb 20201027
    char cCmdSend[1024] = {0};
    char cHeadCmd[4] = {0};
    //char *cTmpCmd = INTERFACE_GETHOSTCONFIG_SENDCMD;
    int  cCmdLength = 0;
    memset(cCmdSend, 0, sizeof(1024)); 

    cCmdLength = strlen(cChnCmd) + 4;
    cHeadCmd[0] = 0; //当Buf[0]为0x00时，表示网管协议不加密,当Buf[0]为0x01时，表示网管协议加密
    cHeadCmd[1] = 2; //获取参数指令（GET）
    cHeadCmd[2] = strlen(cChnCmd) >> 8;  
    cHeadCmd[3] = strlen(cChnCmd) & 0xff;

    memcpy(cCmdSend, cHeadCmd, 4);
    memcpy(cCmdSend+4, cChnCmd, strlen(cChnCmd));

    MPSOperationRes opResCode = eMPSResultOK;
    ResponseCode resCode = _client->GetConfigNew(cCmdSend,cCmdLength,opResCode,oResult,cResult);
    if(resCode != eResponseCodeSuccess){
        mLogError("GetConfig(...) error:"<<resCode);
        return false;
    }

    return true;
}

InterfaceResCode CSetWebUartConf::UartSearchCameraTableControlCmd(string &sResult, string dev_name, string requestStr)
{
    //TODO
	mLogInfo("UartSearchCameraTableControlCmd...");
	
	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };
	char Result[RES_BUF_MAXLEN] = { 0 };
	string Method = "UartSearchCameraTableControlCmd";
	string IP;
    cJSON* temList[15];
	for(int i = 1; i <= 2; i++)
	{ //用于生成节点，存储解析后的参数
		temList[i] = cJSON_CreateObject();
	}
	if (!UartSearchCameraTableControlCmdInner( dev_name, requestStr, oResult, cResult))
	{
		mLogError("UartSearchCameraTableControlCmdInner failed !!!");
		return eInterfaceResCodeError;
	}
    if(GetUartHttpUrlInner(oResult, Result) == false){
        mLogError("Failed to run  GetUartHttpUrlInner(...)");
        return eInterfaceResCodeError;
    }
    if( _AnalyParas.ConvertSTRING2CJSONGetWebParam( oResult, Result, _result, NULL, temList,  Method, IP, 0) == false){
        mLogError("Failed to run  ConvertSTRING2CJSONGetWebParam(...)");
        return eInterfaceResCodeError;
    }

	//解析返回的JSON数据
	cJSON *root = cJSON_Parse(cResult);//将char型数据转换为JSON数据
	cJSON *Params = cJSON_GetObjectItem(root,"params");//从root下获取params数组数据
	int arraysize = cJSON_GetArraySize(Params);//数组大小
	cJSON* ArryNode[100];
	string  sValue;
	cJSON* temArry = cJSON_CreateArray();
	for(int i = 0; i < arraysize; i++)
	{ 
		cJSON* ArryNode = cJSON_CreateObject();
		cJSON* tmpNode = cJSON_CreateObject();
		ArryNode = cJSON_GetArrayItem(Params, i);
		//获取节点
		cJSON *protocol_type       = cJSON_GetObjectItem(ArryNode,"protocol_type");
		cJSON *cmd_name           = cJSON_GetObjectItem(ArryNode,"cmd_name");
		cJSON *cmd_protocol_type  = cJSON_GetObjectItem(ArryNode,"cmd_protocol_type");
		cJSON *cmd_holder_addr    = cJSON_GetObjectItem(ArryNode,"cmd_holder_addr");
		cJSON *cmd_correlation_vi = cJSON_GetObjectItem(ArryNode,"cmd_correlation_vi");
		//创建新节点
		cJSON_AddItemToObject(tmpNode, "protocol_type", cJSON_CreateNumber(protocol_type->valueint));
		cJSON_AddItemToObject(tmpNode, "cmd_name", cJSON_CreateString(cmd_name->valuestring));
		cJSON_AddItemToObject(tmpNode, "cmd_protocol_type", cJSON_CreateNumber(cmd_protocol_type->valueint));
		cJSON_AddItemToObject(tmpNode, "cmd_holder_addr", cJSON_CreateNumber(cmd_holder_addr->valueint));
		cJSON_AddItemToObject(tmpNode, "cmd_correlation_vi", cJSON_CreateString(cmd_correlation_vi->valuestring));

		char httpAddr[128]={0};
		memset(httpAddr, 0, 128);
		
		int len = strlen(cmd_correlation_vi->valuestring);
		memcpy(cmd_correlation_vi->valuestring,cmd_correlation_vi->valuestring+2,len-2);
		int num = atoi(cmd_correlation_vi->valuestring);
		
		switch(num)
		{
		case 1:
			sprintf(httpAddr,"http://%s:9696/live/vl1-1.flv", IP.c_str());
			cJSON_AddItemToObject(tmpNode, "HttpUrl", cJSON_CreateString(httpAddr));
			break;
		case 2:
			sprintf(httpAddr,"http://%s:9696/live/vl2-1.flv", IP.c_str());
			cJSON_AddItemToObject(tmpNode, "HttpUrl", cJSON_CreateString(httpAddr));
			break;
		case 3:
			sprintf(httpAddr,"http://%s:9696/live/vl3-1.flv", IP.c_str());
			cJSON_AddItemToObject(tmpNode, "HttpUrl", cJSON_CreateString(httpAddr));
			break;
		case 4:
			sprintf(httpAddr,"http://%s:9696/live/vl4-1.flv", IP.c_str());
			cJSON_AddItemToObject(tmpNode, "HttpUrl", cJSON_CreateString(httpAddr));
			break;
		case 5:
			sprintf(httpAddr,"http://%s:9696/live/vl5-1.flv", IP.c_str());
			cJSON_AddItemToObject(tmpNode, "HttpUrl", cJSON_CreateString(httpAddr));
			break;
		case 6:
			sprintf(httpAddr,"http://%s:9696/live/vl6-1.flv", IP.c_str());
			cJSON_AddItemToObject(tmpNode, "HttpUrl", cJSON_CreateString(httpAddr));
			break;
		case 7:
			sprintf(httpAddr,"http://%s:9696/live/vl7-1.flv", IP.c_str());
			cJSON_AddItemToObject(tmpNode, "HttpUrl", cJSON_CreateString(httpAddr));
			break;
		case 8:
			sprintf(httpAddr,"http://%s:9696/live/vl8-1.flv", IP.c_str());
			cJSON_AddItemToObject(tmpNode, "HttpUrl", cJSON_CreateString(httpAddr));
			break;
		}
		//若直接使用ArryNode，会将Params里的所有数组都加入到temArry
		cJSON_AddItemToArray(temArry, tmpNode);

	}

	cJSON_AddItemToObject(_result,"list",temArry);
//	cJSON_Delete(root);
    sResult = ComposeResult();
	cJSON_Delete(_result);

    return eInterfaceResCodeSuccess;
}


bool CSetWebUartConf::UartAlterTableControlCmdInner(string dev_name, string cmd_recv_code, string new_cmd_recv_code, string cmd_fun_des, string requestStr, CData& oResult, char* cResult)
{
	mLogDebug("run UartAlterTableControlCmdInner(...)");
		
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
InterfaceResCode CSetWebUartConf::UartAlterTableControlCmd(string &sResult, string dev_name, string cmd_recv_code, string new_cmd_recv_code, string cmd_fun_des, string requestStr)
{
    //TODO
	mLogInfo("UartAlterTableControlCmd...");
	
	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };
	
	if (!UartAlterTableControlCmdInner( dev_name, cmd_recv_code, new_cmd_recv_code, cmd_fun_des, requestStr, oResult, cResult))
	{
		mLogError("UartAlterTableControlCmdInner failed !!!");
		return eInterfaceResCodeError;
	}

	//解析返回的JSON数据
	cJSON *root = cJSON_Parse(cResult);
	cJSON *Params = cJSON_GetObjectItem(root,"params");
	
	cJSON *status = cJSON_GetObjectItem(Params,"status");

	if (strcmp("ok", status->valuestring))
	{
		mLogError("UartAlterTableControlCmdInner recv failed");
		return eInterfaceResCodeError;
	}
	
	cJSON_Delete(root);

    sResult = ComposeResult();
	cJSON_Delete(_result);

    return eInterfaceResCodeSuccess;
}

bool CSetWebUartConf::UartAlterTableControlExecuteCmdInner(string dev_name, string cmd_recv_code, int cmd_type, string data_buf, int protocol_type, string cmd_name, string cmd_name_new, string requestStr, CData& oResult, char* cResult)
{
	mLogDebug("run UartAlterTableControlExecuteCmdInner(...)");
		
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
InterfaceResCode CSetWebUartConf::UartAlterTableControlExecuteCmd(string &sResult, string dev_name, string cmd_recv_code, int cmd_type, string data_buf, int protocol_type, string cmd_name, string cmd_name_new, string requestStr)
{
    //TODO
	mLogInfo("UartAlterTableControlExecuteCmd...");
	
	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };
	
	if (!UartAlterTableControlExecuteCmdInner( dev_name, cmd_recv_code, cmd_type, data_buf, protocol_type, cmd_name, cmd_name_new, requestStr, oResult, cResult))
	{
		mLogError("UartAlterTableControlExecuteCmdInner failed !!!");
		return eInterfaceResCodeError;
	}

	//解析返回的JSON数据
	cJSON *root = cJSON_Parse(cResult);
	cJSON *Params = cJSON_GetObjectItem(root,"params");
	
	cJSON *status = cJSON_GetObjectItem(Params,"status");

	if (strcmp("ok", status->valuestring))
	{
		mLogError("UartAlterTableControlExecuteCmdInner recv failed");
		return eInterfaceResCodeError;
	}
	
	cJSON_Delete(root);

    sResult = ComposeResult();
	cJSON_Delete(_result);

    return eInterfaceResCodeSuccess;
}
bool CSetWebUartConf::UartAlterTableControlCameraCmdInner(string dev_name, int protocol_type, string cmd_name, string cmd_name_new, int cmd_protocol_type, int cmd_holder_addr,string cmd_correlation_vi, string requestStr, CData& oResult, char* cResult)
{
	mLogDebug("run UartAlterTableControlCameraCmdInner(...)");
		
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
InterfaceResCode CSetWebUartConf::UartAlterTableControlCameraCmd(string &sResult, string dev_name, int protocol_type, string cmd_name, string cmd_name_new, int cmd_protocol_type, int cmd_holder_addr,string cmd_correlation_vi, string requestStr)
{
    //TODO
	mLogInfo("UartAlterTableControlCameraCmd...");
	
	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };
	
	if (!UartAlterTableControlCameraCmdInner( dev_name, protocol_type, cmd_name, cmd_name_new, cmd_protocol_type, cmd_holder_addr, cmd_correlation_vi, requestStr, oResult, cResult))
	{
		mLogError("UartAlterTableControlCameraCmdInner failed !!!");
		return eInterfaceResCodeError;
	}

	//解析返回的JSON数据
	cJSON *root = cJSON_Parse(cResult);
	cJSON *Params = cJSON_GetObjectItem(root,"params");
	
	cJSON *status = cJSON_GetObjectItem(Params,"status");

	if (strcmp("ok", status->valuestring))
	{
		mLogError("UartAlterTableControlCameraCmdInner recv failed");
		return eInterfaceResCodeError;
	}
	
	cJSON_Delete(root);

    sResult = ComposeResult();
	cJSON_Delete(_result);

    return eInterfaceResCodeSuccess;
}

bool CSetWebUartConf::UartPTZControlInner(string cmd_name,	   int move_spped, int control_type, string requestStr, CData& oResult, char* cResult)
{
	mLogDebug("run UartPTZControlInner(...)");
		
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
InterfaceResCode CSetWebUartConf::UartPTZControl(string &sResult,  string cmd_name,     int move_spped, int control_type, string requestStr)
{
    //TODO
	mLogInfo("UartPTZControl...");
	
	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };
	
	if (!UartPTZControlInner( cmd_name, move_spped, control_type, requestStr, oResult, cResult))
	{
		mLogError("UartPTZControlInner failed !!!");
		return eInterfaceResCodeError;
	}

	//解析返回的JSON数据
	cJSON *root = cJSON_Parse(cResult);
	cJSON *Params = cJSON_GetObjectItem(root,"params");
	
	cJSON *status = cJSON_GetObjectItem(Params,"status");

	if (strcmp("ok", status->valuestring))
	{
		mLogError("UartPTZControlInner recv failed");
		return eInterfaceResCodeError;
	}
	
	cJSON_Delete(root);

    sResult = ComposeResult();
	cJSON_Delete(_result);

    return eInterfaceResCodeSuccess;
}

bool CSetWebUartConf::UartCameraPresetControlInner( string cmd_name, int preset_num, int control_type, string requestStr, CData& oResult, char* cResult)
{
	mLogDebug("run UartCameraPresetControlInner(...)");
		
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
InterfaceResCode CSetWebUartConf::UartCameraPresetControl(string &sResult, string cmd_name, int preset_num, int control_type, string requestStr)
{
    //TODO
	mLogInfo("UartCameraPresetControl...");
	
	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };
	
	if (!UartCameraPresetControlInner( cmd_name, preset_num, control_type, requestStr, oResult, cResult))
	{
		mLogError("UartAlterTableControlCmdInner failed !!!");
		return eInterfaceResCodeError;
	}

	//解析返回的JSON数据
	cJSON *root = cJSON_Parse(cResult);
	cJSON *Params = cJSON_GetObjectItem(root,"params");
	
	cJSON *status = cJSON_GetObjectItem(Params,"status");

	if (strcmp("ok", status->valuestring))
	{
		mLogError("UartCameraPresetControlInner recv failed");
		return eInterfaceResCodeError;
	}
	
	cJSON_Delete(root);

    sResult = ComposeResult();
	cJSON_Delete(_result);

    return eInterfaceResCodeSuccess;
}
bool CSetWebUartConf::SetGPIOConfigInner(int channel, int Value, string requestStr, CData &oResult, char *cResult)
{
	mLogDebug("run SetGPIOConfigInner(...)");
		
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
InterfaceResCode CSetWebUartConf::SetGPIOConfig(string &sResult,int channel,int Value, string requestStr)
{
    //TODO
	mLogInfo("SetGPIOConfig...");
	
	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };
	
	if (!SetGPIOConfigInner( channel, Value, requestStr, oResult, cResult))
	{
		mLogError("SetGPIOConfigInner failed !!!");
		return eInterfaceResCodeError;
	}

	//解析返回的JSON数据
	cJSON *root = cJSON_Parse(cResult);
	cJSON *Params = cJSON_GetObjectItem(root,"params");
	
	cJSON *status = cJSON_GetObjectItem(Params,"status");

	if (strcmp("ok", status->valuestring))
	{
		mLogError("SetGPIOConfigInner recv failed");
		return eInterfaceResCodeError;
	}
	
	cJSON_Delete(root);

    sResult = ComposeResult();
	cJSON_Delete(_result);

    return eInterfaceResCodeSuccess;
}


