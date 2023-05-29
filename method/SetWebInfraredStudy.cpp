#include "SetWebInfraredStudy.h"
#include "MPSClient.h"


CSetWebInfraredStudy::CSetWebInfraredStudy(const std::string& ip, unsigned short port, int timeOut)
	:_mpsClient(new MPSClient(ip, port, timeOut))
	,_result(cJSON_CreateObject())
{

}

CSetWebInfraredStudy::~CSetWebInfraredStudy()
{
	
}

bool CSetWebInfraredStudy::ComposeResult(cJSON* result)
{
// 	if (_result == NULL || result == NULL) {
// 		mLogError("cJSON result == NULL.");
// 		result = NULL;
// 		return false;
// 	}

	return true;
}

std::string CSetWebInfraredStudy::ComposeResult()
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


InterfaceResCode CSetWebInfraredStudy::SetWebInfraredStudy(std::string& sResult, int port, 
	const std::string& fileName, int num, const std::string& order, const std::string& describe)
{
	mLogInfo("设置 WebInfraredStudy...");

	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };

	if (!SetWebInfraredStudyInner(port, fileName, num, order, describe, oResult, cResult))
	{
		mLogError("SetWebInfraredStudyInner failed !!!");
		return eInterfaceResCodeError;
	}

	//解析响应, 默认不处理
	if (!_analyzePara.ConvertSTRING2CJSONSetWebInfraredStudy(oResult, cResult)) {
		mLogError("Failed to run  ConvertSTRING2CJSONSetWebInfraredStudy(...)");
		return eInterfaceResCodeError;
	}

	sResult = ComposeResult();
	return eInterfaceResCodeSuccess;
}
xmlXPathObjectPtr CSetWebInfraredStudy::get_nodeset(xmlDocPtr doc, const xmlChar *szXpath) 
{
	xmlXPathContextPtr context;	    //XPATH上下文指针
	xmlXPathObjectPtr result;		//XPATH对象指针，用来存储查询结果
	
	context = xmlXPathNewContext(doc);		//创建一个XPath上下文指针
	if (context == NULL) 
	{	
		printf("context is NULL\n");
		return NULL; 
	}
	
	result = xmlXPathEvalExpression(szXpath, context); //查询XPath表达式，得到一个查询结果
	xmlXPathFreeContext(context);				//释放上下文指针
	if (result == NULL) 
	{
		printf("xmlXPathEvalExpression return NULL\n"); 
		return NULL;  
	}
	
	if (xmlXPathNodeSetIsEmpty(result->nodesetval))   //检查查询结果是否为空
	{
		xmlXPathFreeObject(result);
		printf("nodeset is empty\n");
		return NULL;
	}
	
	return result;	
}

bool CSetWebInfraredStudy::SetWebInfraredStudyInner(int port, const std::string& fileName, 
	int num, const std::string& order, const std::string& describe, CData& oResult, char* cResult)
{
	mLogDebug("run SetWebInfraredStudyInner(...)");

	//获取状态
	int SerialID = port;//串口0
	int  cCmdLength   = 0;
	char cCmdSend[1024]   = {0};
	char Sendbuf[1024]   = {0};
	//cCmdLength = strlen(Cmd.c_str());
	//memcpy(cCmdSend, Cmd.c_str(), strlen(Cmd.c_str()));
	//index：命令索引，从1开始，1~50有效。
	if(num > 50){
		mLogError("SetWebInfraredStudyInner num > 50 error:"<<num);
		return false;
	}
	//<指令类型,操作类型,输入/输出,通道,值>
	sprintf( cCmdSend, INTERFACE_GETWEBINFRAREDSTUDY_SENDCMD, num); 
	cCmdLength = strlen(cCmdSend);
#if 1	
	MPSOperationRes opResCode = eMPSResultOK;
	//发送cCmdSend信息给服务器
	ResponseCode resCode = _mpsClient->GetSerialNew(SerialID, cCmdSend, cCmdLength, opResCode, oResult,cResult);
	if(resCode != eResponseCodeSuccess){
		//退出学习模式
		memset(cCmdSend,0,1024);
		sprintf( cCmdSend, "<ExitLearn>"); 
		cCmdLength = strlen(cCmdSend);
		resCode = _mpsClient->GetSerialNew(SerialID, cCmdSend, cCmdLength, opResCode, oResult,Sendbuf);
		mLogDebug("false SetWebInfraredStudyInner Sendbuf "<<Sendbuf);
		mLogError("GetSerialNew(...) error:"<<resCode);
		return false;
	}
	//退出学习模式
	memset(cCmdSend,0,1024);
	sprintf( cCmdSend, "<ExitLearn>"); 
	cCmdLength = strlen(cCmdSend);
	resCode = _mpsClient->GetSerialNew(SerialID, cCmdSend, cCmdLength, opResCode, oResult, Sendbuf);
	mLogDebug("run SetWebInfraredStudyInner Sendbuf "<<Sendbuf);
	//判断是否学习成功
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
	if (strcmp("ok>", arg[2]))
	{
		mLogError("Study failed \n");
		return false;
	}
#endif	
	//学习成功，读写xml
	xmlDocPtr doc = NULL;			 //定义解析文档指针 
	xmlNodePtr RootNode= NULL;			
	xmlNodePtr curNode = NULL;		 //定义结点指针(你需要它为了在各个结点间移动) 
	char PathFile[1024]   = {0};
	char cmd[32]   = {0};
	bool Flag = false;
	char cmd_num[10] = {0};
	sprintf( PathFile, "/root/Communicate_Schedule_exe/config/hongwai/%s", fileName.c_str()); 
	sprintf( cmd, "CMD%d", num); 
	doc = xmlReadFile(PathFile,"UTF-8",XML_PARSE_RECOVER);  //解析文件 
	if (NULL == doc) 
	{ 	
		//printf("Document not parsed successfully.\n"); 
		mLogError("Document not parsed successfully.\n");
		return false; 
	} 
	//获得根节点
	RootNode = xmlDocGetRootElement(doc); 
	/*检查确认当前文档中包含内容*/ 
	if (NULL == RootNode) 
	{ 
		//printf("empty document\n");
		mLogError("empty document. \n");
		xmlFreeDoc(doc); 
		return false; 
	} 
	//curNode = RootNode;
	//curNode = curNode->children;
	sprintf( cmd_num, "%d", num);
	
	xmlXPathObjectPtr app_result = get_nodeset(doc,BAD_CAST ("/root/*"));  //查询并得到root节点的所有子节点
		
	if (NULL == app_result) 
	{
		mLogError("app_result is NULL\n"); 
		return false; 
	}
	xmlChar *szValue = NULL;
	if(app_result) 
	{	//获得子节点集合
		xmlNodeSetPtr nodeset = app_result->nodesetval;
		for (int i = 0; i < nodeset->nodeNr; i++) //nodeset->nodeNr：子节点的总个数
		{
			curNode = nodeset->nodeTab[i];//取出子节点

			if(curNode != NULL) 
			{	
				if (!xmlStrcmp(curNode->name, BAD_CAST cmd))
				{
					//节点存在则修改其属性
					Flag = true;
					xmlSetProp(curNode,BAD_CAST GET_WEB_INFRARED_STUDYLIST_RES_DESCRIBE, BAD_CAST describe.c_str());
					xmlSetProp(curNode,BAD_CAST GET_WEB_INFRARED_STUDYLIST_RES_ORDER, BAD_CAST order.c_str());
					break;
				}
			}
		}

		xmlXPathFreeObject (app_result);
	}

	//节点不存在则添加
	if(Flag != true) 
	{
		xmlNodePtr node = xmlNewNode(NULL,BAD_CAST cmd);
		xmlNodePtr content = xmlNewText(BAD_CAST cmd_num);
		xmlAddChild(node,content);
		//添加属性
		xmlNewProp(node,BAD_CAST GET_WEB_INFRARED_STUDYLIST_RES_DESCRIBE,BAD_CAST describe.c_str());
		xmlNewProp(node,BAD_CAST GET_WEB_INFRARED_STUDYLIST_RES_ORDER,BAD_CAST order.c_str());
		xmlAddChild(RootNode,node);
	}
	int nRel = xmlSaveFormatFileEnc(PathFile,doc,"UTF-8",1);
	if (nRel == -1)
	{
		mLogError("save xml file failed："<<fileName.c_str());
		return false;
	}
	mLogDebug("save xml file success："<<fileName.c_str());

	//释放文档内节点动态申请的内存
	xmlFreeDoc(doc);
	return true;

}

