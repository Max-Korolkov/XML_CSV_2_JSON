#pragma once
#include <memory>
#include "Node.h"

// MyConverter class
//
// Stores ptr to root of parse tree
//
// Main methods: 
//     GetDataFromXML() - gets data from XML file (string)
//                        and creates parse tree
//     GetDataFromCSV() - gets data from CSV file (string)
//                        and puts into existing parse tree
//     DataToJSON()     - returns output string in JSON
//                        using data stored in parse tree

class MyConverter
{
private:
	std::shared_ptr<Node> Tree;
	std::shared_ptr<Node> RecursiveXMLParser(const std::string&);
	void CSVParser(const std::string&);
public:
	MyConverter();

	void GetDataFromXML(const std::string&);
	void GetDataFromCSV(const std::string&);

	const std::string DataToJSON();
	const std::string DataToJSON(const std::string& csv_s);
	const std::string DataToJSON(const std::string& xml_s, const std::string& csv_s);
};
