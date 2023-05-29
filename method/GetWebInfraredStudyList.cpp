#include "GetWebInfraredStudyList.h"
#include "MPSClient.h"

CGetWebInfraredStudyList::CGetWebInfraredStudyList(const std::string& ip, unsigned short port, int timeOut)
	:_mpsClient(new MPSClient(ip, port, timeOut))
	, _result(cJSON_CreateObject())
	, _ParamList(cJSON_CreateArray())
{
}

CGetWebInfraredStudyList::~CGetWebInfraredStudyList()
{
}


bool CGetWebInfraredStudyList::ComposeResult(cJSON* result)
{
	// 	if (_result == NULL || result == NULL) {
	// 		mLogError("cJSON result == NULL.");
	// 		result = NULL;
	// 		return false;
	// 	}
	// 
	// 	if (_nbTaskList == NULL) {
	// 		mLogWarn("cJSON _nbTaskList == NULL.");
	// 		return false;
	// 	}
	// 
	// 	//result中添加节点
	// 
	// 	cJSON_AddItemToObject(_result, GET_HOST_NBINFO_TASKLIST, _nbTaskList);
	return true;
}

std::string CGetWebInfraredStudyList::ComposeResult()
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

	if (_ParamList == NULL)
	{
		mLogError("_videoParamList is NULL !!!");
		return "";
	}

	cJSON_AddItemToObject(_result, "ParamList", _ParamList);

	char* cRes = cJSON_Print(_result);
	std::string res = cRes;
	//cJSON_Delete(result);
	return res;
}

InterfaceResCode CGetWebInfraredStudyList::GetWebInfraredStudyList(string& sResult, int port, const std::string& fileName)
{
	mLogInfo("获取 WebInfraredStudyList...");

	char cResult[RES_BUF_MAXLEN] = { 0 };
	CData oResult = SVSMAP();

	//1. 调用类内部方法进一步封装业务请求
	if (!GetWebInfraredStudyListInner(port, fileName, oResult, cResult))
	{
		mLogError("Failed to run  GetWebInfraredStudyListInner(...)");
		return eInterfaceResCodeError;
	}
#if 0
	//2. 解析响应,存入类型为cJson对象的成员变量中
	if (!_analyPara.ConvertSTRING2CJSONGetWebInfraredStudyList(oResult, cResult, _result, _ParamList)) {
		mLogError("Failed to run  ConvertSTRING2CJSONGetWebInfraredStudyList(...)");
		return eInterfaceResCodeError;
	}
#endif
	//3. 将step2中获取的响应格式化为Json字符串
	sResult = ComposeResult();

	return eInterfaceResCodeSuccess;
}
xmlXPathObjectPtr CGetWebInfraredStudyList::get_nodeset(xmlDocPtr doc, const xmlChar *szXpath) 
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

bool CGetWebInfraredStudyList::GetWebInfraredStudyListInner(int port, const std::string& fileName,
	CData& oResult, char* cResult)
{
	mLogDebug("run GetWebInfraredStudyListInner(...)");

	xmlDocPtr doc = NULL;			 //定义解析文档指针 
	xmlNodePtr RootNode= NULL;			
	xmlNodePtr curNode = NULL;		 //定义结点指针(你需要它为了在各个结点间移动) 
	char PathFile[1024]   = {0};
	char cmd_num[1024]   = {0};
	bool Flag = false;
	
	sprintf( PathFile, "/root/Communicate_Schedule_exe/config/hongwai/%s", fileName.c_str()); 

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
		mLogError("empty document. \n");
		xmlFreeDoc(doc); 
		return false; 
	} 
	//指令号码
	//sprintf( cmd, "cmd%d", num);
	//查询并得到root节点的所有子节点
	xmlXPathObjectPtr app_result = get_nodeset(doc,BAD_CAST ("/root/*"));  
		
	if (NULL == app_result) 
	{
		mLogError("app_result is NULL\n");
		xmlFreeDoc(doc);
		return false; 
	}
	xmlChar *szValue = NULL;
	xmlChar *attrValue = NULL;
	
	if(app_result) 
	{	//获得子节点集合
		xmlNodeSetPtr nodeset = app_result->nodesetval;
		cJSON_AddItemToObject(_result, GET_WEB_INFRARED_STUDYLIST_RES_TOTAL, cJSON_CreateNumber(nodeset->nodeNr));
		for (int i = 0; i < nodeset->nodeNr; i++) //nodeset->nodeNr：子节点的总个数
		{	//取出子节点
			curNode = nodeset->nodeTab[i];
			//sprintf("curNode->name:%s\n",curNode->name);

			if(curNode != NULL) 
			{			
				cJSON* arrayItem = cJSON_CreateObject();
					
				if (!xmlStrcmp(curNode->name, BAD_CAST "DevID"))
				{
					//cJSON_AddItemToObject(arrayItem, GET_WEB_INFRARED_STUDYLIST_RES_NUM, cJSON_CreateNumber(atoi((char*)(curNode->name))));
					szValue = xmlNodeGetContent(curNode);
					cJSON_AddItemToObject(_result, "DevID", cJSON_CreateString((char*)szValue));
					xmlFree(szValue);
					continue;
				}
				
				if (!xmlStrcmp(curNode->name, BAD_CAST "DevType"))
				{
					//cJSON_AddItemToObject(arrayItem, GET_WEB_INFRARED_STUDYLIST_RES_NUM, cJSON_CreateNumber(atoi((char*)(curNode->name))));
					szValue = xmlNodeGetContent(curNode);
					cJSON_AddItemToObject(_result, "DevType", cJSON_CreateString((char*)szValue));
					xmlFree(szValue);
					continue;
				}
				
				if (!xmlStrcmp(curNode->name, BAD_CAST "Manufacturer"))
				{
					//cJSON_AddItemToObject(arrayItem, GET_WEB_INFRARED_STUDYLIST_RES_NUM, cJSON_CreateNumber(atoi((char*)(curNode->name))));
					szValue = xmlNodeGetContent(curNode);
					cJSON_AddItemToObject(_result, "Manufacturer", cJSON_CreateString((char*)szValue));
					xmlFree(szValue);
					continue;
				}
				//获得节点值
				szValue = xmlNodeGetContent(curNode);
				if (szValue != NULL) 
				{
					cJSON_AddItemToObject(arrayItem, GET_WEB_INFRARED_STUDYLIST_RES_NUM, cJSON_CreateNumber(atoi((char*)szValue)));
					xmlFree(szValue);
				}

				//获得节点属性值
				attrValue = xmlGetProp(curNode,BAD_CAST GET_WEB_INFRARED_STUDYLIST_RES_DESCRIBE);
				szValue = xmlGetProp(curNode,BAD_CAST GET_WEB_INFRARED_STUDYLIST_RES_ORDER);
				if (attrValue != NULL) 
				{
					cJSON_AddItemToObject(arrayItem, GET_WEB_INFRARED_STUDYLIST_RES_DESCRIBE, cJSON_CreateString((char*)attrValue));
					xmlFree(attrValue);
				}
				
				if (szValue != NULL) 
				{
					cJSON_AddItemToObject(arrayItem, GET_WEB_INFRARED_STUDYLIST_RES_ORDER, cJSON_CreateString((char*)szValue));
					xmlFree(szValue);
				}
				cJSON_AddItemToArray(_ParamList, arrayItem);
			}
		}
		
		xmlXPathFreeObject (app_result);
	}


#if 0
	//2. 发送socket请求
	//MPSOperationRes opResCode = eMPSResultOK; //??
	ResponseCode resCode = = _mpsClient->GetConfigNew(szReqCmd, 4 + realBodySize, eMPSResultOK, oResult, cResult);
	if (resCode != eResponseCodeSuccess) {
		mLogError("GetConfig(...) error:" << resCode);
		return false;
	}
#else
	//dummy response
#endif

	return true;
}
