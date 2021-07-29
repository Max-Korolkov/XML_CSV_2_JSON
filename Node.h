#pragma once
#include <memory>
#include <string>
#include <vector>

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