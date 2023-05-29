#ifndef __XML_OPERA_H__
#define __XML_OPERA_H__

#include "tinyxml2.h"
#include "XMLDefines.h"

#include <string>
#include <cJSON.h>
#include <errno.h>
using namespace tinyxml2;

enum XMLOperErroCode
{
	XML_ERR_NONE,
	XML_ERR_LOAD_FILE,
	XML_ERR_PARSE_FILE,
	XML_ERR_PARTY_NOT_FOUND,
};

/*
* encapsulated class for xml operation
*/

class CXMLOpera
{
public:
	static void SetFilePath(const std::string& xmlFileName)
	{
		_xmlFileName = xmlFileName;
	}

	/**
	 * @brief LoadPartyList 加载多方xml文件内容
	 * @param fileContent	文件内容, 返回给前端
	 * @return XMLOperErroCode.
	 */
	static XMLOperErroCode LoadPartyList(cJSON* result, cJSON* partyList);

	/**
	 * @brief InsertOneParty 向多方xml文件中插入一方信息
	 * @param name
	 * @param address
	 * @param protocol
	 * @param bitRate
	 * @param bufferLength
	 * @param transport
	 * @return XMLOperErroCode.
	 */
	static XMLOperErroCode InsertOneParty(const std::string& name,
		const std::string& address,
		const std::string& protocol,
		const std::string& bitRate,
		const std::string& bufferLength,
		const std::string& transport);

	/**
	 * @brief DelOneParty	从多方xml文件中删除一方信息
	 * @param id			代表某方信息的唯一标识
	 * @return XMLOperErroCode.
	 */
	static XMLOperErroCode DelOneParty(int id);

private:
	/**
	 * @brief IsFileExist	判断xml文件是否存在			
	 * @return true: exist; false: not found.
	 */
	static bool IsFileExist();
	/**
	 * @brief CreateXMLFile	创建XML文件
	 * @return true: create success; false: create failed.
	 */
	static bool CreateXMLFile();
	/**
	 * @brief GetXMLFileHandle	获取xml文件句柄
	 * @return fp: file handle; NULL: get failed.
	 */
	static FILE* GetXMLFileHandle();
	/**
	 * @brief GetAddressElement	获取多方xml文件列表AddressList节点
	 * @param doc			xmldoc对象
	 * @return AddressList element.
	 */
	static XMLElement* GetAddressElement(XMLDocument& doc);
	/**
	 * @brief DeleteParty	从多方xml文件中删除一方信息
	 * @param id			代表某方信息的唯一标识
	 * @return XMLOperErroCode.
	 */
	static bool DeleteParty(XMLElement* addressList, int id);
	/**
	 * @brief InsertParty   向多方xml文件中插入一方信息
	 * @param parent		parent element
	 * @param child			child element
	 * @return true success; false failed.
	 */
	static bool InsertParty(XMLElement* parent, XMLElement* child);
	/**
	 * @brief InsertParty   或许多方xml文件内容
	 * @param result		
	 * @param partyList			
	 * @return true success; false failed.
	 */
	static bool GetPartyList(XMLElement* addressList, cJSON* result, cJSON* partyList);


private:
	static std::string _xmlFileName;
};


#endif //__XML_OPERA_H__
