#include <sstream>
#include "Node.h"

Node::Node() // init attributes
{
	Category = "";
	ForAll = false;
	ParentCategory = "";
}

const std::string Node::JSONPrint() // print node in json format
{
	std::ostringstream stream;
	// if node has no values - don't output it
	if (Values.size() > 0)
	{
		stream << "\t{" << std::endl;
		stream << "\t\t\"category\":" << "\"" << Category << "\"," << std::endl;
		stream << "\t\t\"forAll\":" << std::boolalpha << ForAll << "," << std::endl;
		stream << "\t\t\"parentCategory\":" << "\"" << ParentCategory << "\"," << std::endl;
		stream << "\t\t\"values\":[" << std::endl;

		for (unsigned i = 0; i < Values.size() - 1; i++)
			stream << "\t\t\t" << Values[i] << "," << std::endl;
		stream << "\t\t\t" << Values[Values.size() - 1] << "" << std::endl;

		stream << "\t\t]" << std::endl;
		stream << "\t}," << std::endl;
	}
	// if node has children
	if (Children.size() > 0)
	{
		// iterate through children
		for (std::vector<std::shared_ptr<Node>>::iterator it = Children.begin(); it != Children.end(); ++it)
		{
			// add info about parent category to child
			if (ParentCategory == "") (*it)->SetParentCategory(Category);
			else (*it)->SetParentCategory(ParentCategory + "." + Category);
			// output children
			stream << (*it)->JSONPrint();
		}
	}

	// return string in json
	return stream.str();
}

void Node::SetCategory(const std::string& newCategory)
{
	Category = newCategory;
}
void Node::SetForAll(bool newForAll)
{
	ForAll = newForAll;
}
void Node::SetParentCategory(const std::string& newParentCategory)
{
	ParentCategory = newParentCategory;
}
void Node::AddValue(const std::string& newValue)
{
	Values.push_back(newValue);
}
std::shared_ptr<Node> Node::CreateNewChild(const std::string& childCategory, bool childForAll) // return new child with set attr
{
	std::shared_ptr<Node> newNodePtr = std::make_shared<Node>();
	Children.push_back(newNodePtr);
	newNodePtr->SetCategory(childCategory);
	if (ParentCategory == "") newNodePtr->SetParentCategory(Category);
	else newNodePtr->SetParentCategory(ParentCategory + "." + Category);
	newNodePtr->SetForAll(childForAll);

	return newNodePtr;
}
void Node::AddExistingChild(std::shared_ptr<Node> child) // add existing node to children vector
{
	Children.push_back(child);
}

const std::string Node::GetCategory()
{
	return Category.c_str();
}
bool Node::GetForAll()
{
	return ForAll;
}
const std::string Node::GetParentCategory()
{
	return ParentCategory.c_str();
}
int Node::GetValueCount()
{
	return Values.size();
}
int Node::GetChildrenCount()
{
	return Children.size();
}
std::shared_ptr<Node> Node::SeekNode(std::vector<std::string>& path) // get ptr to node from path vector
{
	// if first string of "path" vector != this node "Category"
	if (Category == *path.begin())
	{
		// remove current category from path
		path.erase(path.begin());
		// if this is the desired node
		if (path.size() == 0) return shared_from_this();
		// if this node has children (and there still are strings in "path")
		if (Children.size() != 0)
			// iterate through vector to search for next node
			for (std::vector<std::shared_ptr<Node>>::iterator it = Children.begin(); it != Children.end(); ++it)
			{
				std::shared_ptr<Node> tmp;
				// search this child
				tmp = (*it)->SeekNode(path);
				// if this child contains desired node - return ptr to that node
				if (tmp != nullptr) return tmp;
			}
	}
	// if this is the wrong node - return nullptr
	return nullptr;
}
