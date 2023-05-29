/**************************************************************
* author		: libx
* function		: a serial of operations for xml file
* create date	: 2022/02/26
**************************************************************/

#include "XMLOpera.h"
#include "XMLDefines.h"
//#include "interfaceDefines.h"
#include <log.h>
#include <unistd.h>
#include <sys/file.h>

std::string CXMLOpera::_xmlFileName = "";


bool CXMLOpera::IsFileExist()
{
	if (access(_xmlFileName.c_str(), 0) == 0)
	{
		return true;
	}

	return false;

}

bool CXMLOpera::CreateXMLFile()
{
	bool ret = false;
	FILE* fp_xml  = NULL; 
	
	do 
	{
		fp_xml = fopen(_xmlFileName.c_str(), "w");
		if (fp_xml == NULL)
		{
			mLogError("create xml file: " << errno);
			return false;
		}
 		int nfd = fileno(fp_xml);
 		mLogDebug("111 wait xml file okay ... ");
 		flock(nfd, LOCK_EX/* | LOCK_NB*/); //wait
 		mLogDebug("111 file is okay !!!");

		XMLDocument doc;
		XMLDeclaration* declaration = doc.NewDeclaration();
		doc.InsertFirstChild(declaration);

		XMLElement* rootElement = doc.NewElement(ROOT_LEVEL_ELEMENT);
		XMLElement* addressList = doc.NewElement(ADDRESS_LEVEL_ELEMENT);

		if (rootElement == NULL || addressList == NULL)
		{
			ret = false;
			break;
		}

		rootElement->InsertEndChild(addressList);
		doc.InsertEndChild(rootElement);
		doc.SaveFile(fp_xml);

		ret = true;
	} while (false);
	
	if (fp_xml != NULL)
	{
		//int fd = fileno(fp_xml);
		//flock(fd, LOCK_UN);
		fclose(fp_xml);
	}

	return ret;
	
}

/*
	找到前一个位置preElement，
	将child插入到preElement的后边
	如果preElement不是parent的子节点, 那返回空
	如果插入成功, 就返回child
*/
bool CXMLOpera::InsertParty(XMLElement* parent, XMLElement* child)
{
	if (parent->NoChildren())
	{
		child->SetAttribute(DEL_WEB_PARTY_PARAMS_ID, 1);
		parent->InsertEndChild(child);
		return true;
	}

	//1. 找到preElement
	int id = 0;
	int childCount = 0;
	XMLElement* preElement = NULL;
	XMLElement* currentElement = NULL;
	XMLElement* firstElement = parent->FirstChildElement();
	const XMLAttribute* firstIdAttr = firstElement->FindAttribute(DEL_WEB_PARTY_PARAMS_ID);
	int firstId = firstIdAttr->IntValue();
	if (firstId > 1)
	{
		//直接插入
		child->SetAttribute(DEL_WEB_PARTY_PARAMS_ID, 1);
		XMLNode* childElement = parent->InsertFirstChild(child);
		if (childElement != child)
		{
			mLogError("insert after child is faild!!!");
			return false;
		}
		return true;
	}

	for (currentElement = firstElement;
		currentElement; currentElement = currentElement->NextSiblingElement())
	{
		if (preElement != NULL)
		{
			const XMLAttribute* preIdAttr = preElement->FindAttribute(DEL_WEB_PARTY_PARAMS_ID);
			int preId = preIdAttr->IntValue();

			const XMLAttribute* curIdAttr = currentElement->FindAttribute(DEL_WEB_PARTY_PARAMS_ID);
			int curId = curIdAttr->IntValue();

			if ((preId + 1) != curId)
			{
				id = preId + 1;
				break;
			}
		}

		preElement = currentElement;

		childCount++;
	}

	if (NULL == currentElement)
	{
		const XMLAttribute* lastIdAttr = preElement->FindAttribute(DEL_WEB_PARTY_PARAMS_ID);
		int curId = lastIdAttr->IntValue();
		id = curId + 1;

		child->SetAttribute(DEL_WEB_PARTY_PARAMS_ID, id);
		XMLNode* childElement = parent->InsertEndChild(child);
		if (childElement != child)
		{
			mLogError("insert after child is faild!!!");
			return false;
		}
	}
	else
	{
		child->SetAttribute(DEL_WEB_PARTY_PARAMS_ID, id);
		XMLNode* childElement = parent->InsertAfterChild(preElement, child);
		if (childElement != child)
		{
			mLogError("insert after child is faild!!!");
			return false;
		}
	}

// 	if (id == 1)
// 	{
// 		mLogError("insert party id=" << id << ", invalid !!!");
// 		return false;
// 	}



	return true;
}

bool CXMLOpera::DeleteParty(XMLElement* addressList, int id)
{
	if (id < 1) // the max size of list ???
	{
		mLogWarn("invalid id: " << id);
		return false;
	}

	if (addressList->NoChildren())
	{
		mLogWarn("no child found in xml file!!!");
		return true;
	}

	XMLElement* currentElement = NULL;
	for (currentElement = addressList->FirstChildElement();
		currentElement; currentElement = currentElement->NextSiblingElement())
	{
		const XMLAttribute* curIdAttr = currentElement->FindAttribute(DEL_WEB_PARTY_PARAMS_ID);
		if (id == curIdAttr->IntValue())
		{
			addressList->DeleteChild(currentElement);
			break;
		}

	}
	
	if (currentElement == NULL)
	{
		mLogError("no id:" << id << " found in xml file !!!");
		return false;
	}

	return true;
}


FILE* CXMLOpera::GetXMLFileHandle()
{
	FILE* fp_xml = NULL;

	do
	{
		if (!IsFileExist())
		{
			mLogDebug("xml file doesn't exist, create it.");
			if (!CreateXMLFile())
			{
				break;
			}
		}

		fp_xml = fopen(_xmlFileName.c_str(), "rb+"); //文件必须存在
		if (fp_xml == NULL)
		{
			mLogDebug("xml file open failed: " << errno);
			break;
		}

		int nfd = fileno(fp_xml);
		mLogDebug("222 wait xml file okay ... ");
		flock(nfd, LOCK_EX/* | LOCK_NB*/); //wait
		mLogDebug("222 file is okay !!!");

	} while (false);

	return fp_xml;
}

XMLElement* CXMLOpera::GetAddressElement(XMLDocument& doc)
{
	XMLElement* root = doc.FirstChildElement(ROOT_LEVEL_ELEMENT);
	if (root == NULL)
	{
		return NULL;
	}

	XMLElement* addressElement = root->FirstChildElement(ADDRESS_LEVEL_ELEMENT);
	if (addressElement == NULL)
	{
		return NULL;
	}

	return addressElement;
}

bool CXMLOpera::GetPartyList(XMLElement* addressList, cJSON* result, cJSON* partyList)
{
	int partyCount = 0;
	XMLElement* currentElement = NULL;
	for (currentElement = addressList->FirstChildElement();
		currentElement; currentElement = currentElement->NextSiblingElement())
	{

		cJSON* partyItem = cJSON_CreateObject();
		
		const XMLAttribute* idAttr = currentElement->FindAttribute(GET_WEB_ADDR_LIST_RES_ID);
		if (idAttr != NULL)
		{
			cJSON_AddItemToObject(partyItem, GET_WEB_ADDR_LIST_RES_ID, cJSON_CreateNumber(idAttr->IntValue()));
		}

		const char* nameValue = currentElement->Attribute(GET_WEB_ADDR_LIST_RES_NAME);
		if (nameValue != NULL)
		{
			cJSON_AddItemToObject(partyItem, GET_WEB_ADDR_LIST_RES_NAME, cJSON_CreateString(nameValue));
		}

		const char* protocolValue = currentElement->Attribute(GET_WEB_ADDR_LIST_RES_PROTOCOL);
		if (protocolValue != NULL)
		{
			cJSON_AddItemToObject(partyItem, GET_WEB_ADDR_LIST_RES_PROTOCOL, cJSON_CreateString(protocolValue));
		}

		const char* addressValue = currentElement->Attribute(GET_WEB_ADDR_LIST_RES_ADDRESS);
		if (addressValue != NULL)
		{
			cJSON_AddItemToObject(partyItem, GET_WEB_ADDR_LIST_RES_ADDRESS, cJSON_CreateString(addressValue));
		}

		const char* bitrateValue = currentElement->Attribute(GET_WEB_ADDR_LIST_RES_BITRATE);
		if (bitrateValue != NULL)
		{
			cJSON_AddItemToObject(partyItem, GET_WEB_ADDR_LIST_RES_BITRATE, cJSON_CreateString(bitrateValue));
		}

		const char* bufferLengthValue = currentElement->Attribute(GET_WEB_ADDR_LIST_RES_BUFFERLENGTH);
		if (bufferLengthValue != NULL)
		{
			cJSON_AddItemToObject(partyItem, GET_WEB_ADDR_LIST_RES_BUFFERLENGTH, cJSON_CreateString(bufferLengthValue));
		}

		const char* transportValue = currentElement->Attribute(GET_WEB_ADDR_LIST_RES_TRANSPORT);
		if (transportValue != NULL)
		{
			cJSON_AddItemToObject(partyItem, GET_WEB_ADDR_LIST_RES_TRANSPORT, cJSON_CreateString(transportValue));
		}
		
		cJSON_AddItemToArray(partyList, partyItem);
		partyCount++;
	}

	cJSON_AddItemToObject(result, GET_WEB_ADDR_LIST_RES_TOTAL, cJSON_CreateNumber(partyCount));

	return true;
}

XMLOperErroCode CXMLOpera::LoadPartyList(cJSON* result, cJSON* partyList)
{
	FILE* fp_xml = NULL;
	XMLOperErroCode ret = XML_ERR_NONE;

	do {
		fp_xml = GetXMLFileHandle();
		if (fp_xml == NULL)
		{
			ret = XML_ERR_LOAD_FILE;
			break;
		}

// 		fseek(fp_xml, 0, SEEK_END);
// 		int fileSize = ftell(fp_xml);
// 		char* pBuffer = new char[fileSize + 1];
// 		fread(pBuffer, 1, fileSize, fp_xml);
// 		fileContent = pBuffer;
// 		delete[] pBuffer;
// 		pBuffer = NULL;

		XMLDocument doc;
		int res = doc.LoadFile(fp_xml);
		if (res != XML_SUCCESS)
		{
			mLogError("load xml failed: " << res);
			ret = XML_ERR_LOAD_FILE;
			break;
		}

		XMLElement* addressList = GetAddressElement(doc);
		if (addressList == NULL)
		{
			mLogError("find element failed.");
			ret = XML_ERR_PARSE_FILE;
			break;
		}

		GetPartyList(addressList, result, partyList);

	}
	while (false);

	if (fp_xml != NULL)
	{
		fclose(fp_xml);
	}

	return ret;
}


XMLOperErroCode CXMLOpera::InsertOneParty(const std::string& name,
	const std::string& address,
	const std::string& protocol,
	const std::string& bitRate,
	const std::string& bufferLength,
	const std::string& transport)
{
	FILE* fp_xml = NULL;
	XMLOperErroCode ret = XML_ERR_NONE;

	do {
		fp_xml = GetXMLFileHandle();
		if (fp_xml == NULL)
		{
			ret = XML_ERR_LOAD_FILE;
			break;
		}

		XMLDocument doc;
		int res = doc.LoadFile(fp_xml);
		if (res != XML_SUCCESS)
		{
			mLogError("load xml failed: " << res);
			ret = XML_ERR_LOAD_FILE;
			break;
		}

		XMLElement* addressList = GetAddressElement(doc);
		if (addressList == NULL)
		{
			mLogError("find element failed.");
			ret = XML_ERR_PARSE_FILE;
			break;
		}

		XMLElement* partyItem = doc.NewElement(PARTY_ELEMENT);
		if (partyItem == NULL)
		{
			mLogError("new element failed.");
			ret = XML_ERR_PARSE_FILE;
			break;
		}

		InsertParty(addressList, partyItem);

		partyItem->SetAttribute("Name", name.c_str());
		partyItem->SetAttribute("Address", address.c_str());
		partyItem->SetAttribute("Protocol", protocol.c_str());
		partyItem->SetAttribute("BitRate", bitRate.c_str());
		partyItem->SetAttribute("BufferLength", bufferLength.c_str());
		partyItem->SetAttribute("Transport", transport.c_str());
// 		if (!InsertParty(addressList, partyItem))
// 		{
// 			mLogError("insert party failed.");
// 			ret = XML_ERR_PARSE_FILE;
// 			break;
// 		}
		int fd = fileno(fp_xml);
		if (ftruncate(fd, 0) != 0)
		{
			mLogError("truncate file failed: " << errno);
		}
		fseek(fp_xml, 0, SEEK_SET);
		doc.SaveFile(fp_xml);

	} while (false);

	if (fp_xml != NULL)
	{
		//int fd = fileno(fp_xml);
		//flock(fd, LOCK_UN);
		fclose(fp_xml);
	}

	return ret;
}

XMLOperErroCode CXMLOpera::DelOneParty(int id)
{
	FILE* fp_xml = NULL;
	XMLOperErroCode ret = XML_ERR_NONE;

	do {
		fp_xml = GetXMLFileHandle();
		if (fp_xml == NULL)
		{
			ret = XML_ERR_LOAD_FILE;
			break;
		}

		XMLDocument doc;
		int res = doc.LoadFile(fp_xml);
		if (res != XML_SUCCESS)
		{
			mLogError("load xml failed: " << res);
			ret = XML_ERR_LOAD_FILE;
			break;
		}

		XMLElement* addressList = GetAddressElement(doc);
		if (addressList == NULL)
		{
			mLogError("find element failed.");
			ret = XML_ERR_PARSE_FILE;
			break;
		}

		DeleteParty(addressList, id);
// 		if (!DeleteParty(addressList, id))
// 		{
// 			mLogError("delete party failed.");
// 			ret = XML_ERR_PARSE_FILE;
// 			break;
// 		}
		//ugly implemented (
		int fd = fileno(fp_xml);
		if (ftruncate(fd, 0) != 0)
		{
			mLogError("truncate file failed: " << errno);
		}
		fseek(fp_xml, 0, SEEK_SET);
		doc.SaveFile(fp_xml);

	} while (false);

	if (fp_xml != NULL)
	{
		fclose(fp_xml);
	}

	return ret;
}

