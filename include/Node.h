#pragma once
#include <memory>
#include <string>
#include <vector>

// Node class
//
// Each node stores info about each category
// from XML file.
// String Category - name of category
//
// If category opened with "<Category everyone="">" - ForAll true
// If category opened with "<Category>" - ForAll = false
//
// string ParentCategory stores node's path (separated by '.')
//
// Values vector stores node's data taken from CSV file
//
// Children vector stores ptrs to child nodes

// JSONPrint() prints this node in JSON format/
// Node's children are printed recursively by this same function

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