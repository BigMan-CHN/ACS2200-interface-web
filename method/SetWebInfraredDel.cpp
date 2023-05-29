#include "SetWebInfraredDel.h"
#include "MPSClient.h"

CSetWebInfraredDel::CSetWebInfraredDel(const std::string& ip, unsigned short port, int timeOut)
	:_mpsClient(new MPSClient(ip, port, timeOut))
	,_result(cJSON_CreateObject())
{

}

CSetWebInfraredDel::~CSetWebInfraredDel()
{
	
}

bool CSetWebInfraredDel::ComposeResult(cJSON* result)
{
// 	if (_result == NULL || result == NULL) {
// 		mLogError("cJSON result == NULL.");
// 		result = NULL;
// 		return false;
// 	}

	return true;
}

std::string CSetWebInfraredDel::ComposeResult()
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


InterfaceResCode CSetWebInfraredDel::SetWebInfraredDel(std::string& sResult, int port,
	const std::string& fileName, int num, const std::string& order, const std::string& describe)
{
	mLogInfo("设置 WebInfraredDel...");

	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };

	if (!SetWebInfraredDelInner(port, fileName, num, order, describe, oResult, cResult))
	{
		mLogError("SetWebInfraredDelInner failed !!!");
		return eInterfaceResCodeError;
	}

	//解析响应, 默认不处理
	if (!_analyzePara.ConvertSTRING2CJSONSetWebInfraredDel(oResult, cResult)) {
		mLogError("Failed to run  ConvertSTRING2CJSONSetWebInfraredDel(...)");
		return eInterfaceResCodeError;
	}

	sResult = ComposeResult();
	return eInterfaceResCodeSuccess;
}
xmlXPathObjectPtr CSetWebInfraredDel::get_nodeset(xmlDocPtr doc, const xmlChar *szXpath) 
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

bool CSetWebInfraredDel::SetWebInfraredDelInner(int port, const std::string& fileName, 
	int num, const std::string& order, const std::string& describe, CData& oResult, char* cResult)
{
	mLogDebug("run SetWebInfraredDelInner(...)");

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
	sprintf( cmd_num, "CMD%d", num);
	//查询并得到root节点的所有子节点
	xmlXPathObjectPtr app_result = get_nodeset(doc,BAD_CAST ("/root/*"));  
		
	if (NULL == app_result) 
	{
		mLogError("app_result is NULL\n");
		xmlFreeDoc(doc);
		return false; 
	}
	
	if(app_result) 
	{	//获得子节点集合
		xmlNodeSetPtr nodeset = app_result->nodesetval;
		for (int i = 0; i < nodeset->nodeNr; i++) //nodeset->nodeNr：子节点的总个数
		{	//取出子节点
			curNode = nodeset->nodeTab[i];
			//sprintf("curNode->name:%s\n",curNode->name);

			if(curNode != NULL) 
			{					
				if (!xmlStrcmp(curNode->name, BAD_CAST cmd_num))
				{
					//断开节点链接
					xmlUnlinkNode(curNode);
					//释放节点
					xmlFreeNode(curNode);
					break;
				}
			}
		}
		
		xmlXPathFreeObject (app_result);
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

