#pragma once
#include <memory>
#include "Node.h"

class MyConverter
{
private:
	std::shared_ptr<Node> Tree;
	std::shared_ptr<Node> RecursiveXMLParser(const std::string&);
	void CSVParser(const std::string&);
public:
	MyConverter();
	MyConverter(const std::string&);

	void GetDataFromXML(const std::string&);
	void GetDataFromCSV(const std::string&);

	const std::string DataToJSON();
	const std::string DataToJSON(const std::string& csv_s);
	const std::string DataToJSON(const std::string& xml_s, const std::string& csv_s);
};
