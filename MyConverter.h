#pragma once
#include <vector>
#include <memory>

class Node : public std::enable_shared_from_this<Node>
{
private:
	std::string Category;
	bool ForAll;
	std::string ParentCategory;

	std::vector<std::string> Values;
	std::vector<std::shared_ptr<Node>> Children;
public:
	Node();

	const std::string JSONPrint();

	void SetCategory(const std::string&);
	void SetForAll(bool);
	void SetParentCategory(const std::string&);
	void AddValue(const std::string&);
	std::shared_ptr<Node> CreateNewChild(const std::string&, bool);
	void AddExistingChild(std::shared_ptr<Node>);

	const std::string GetCategory();
	bool GetForAll();
	const std::string GetParentCategory();
	int GetValueCount();
	int GetChildrenCount();
	std::shared_ptr<Node> SeekNode(std::vector<std::string>&);
};

using ParseTree = std::shared_ptr<Node>;

class MyConverter
{
private:
	ParseTree Tree;
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
