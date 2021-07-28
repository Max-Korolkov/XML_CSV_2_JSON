#include <string>
#include <sstream>
#include <vector>
#include "myConverter.h"

std::string trim(const std::string& str)
{
	// useless symbols
	const std::string WHITESPACE = " \t\n\v\f\r";

	std::string trimmedStr = "";

	// removing unnecessary symbols before and after the relevant part
	size_t start = str.find_first_not_of(WHITESPACE);
	trimmedStr = (start == std::string::npos) ? "" : str.substr(start);
	size_t end = trimmedStr.find_last_not_of(WHITESPACE);
	trimmedStr = (end == std::string::npos) ? "" : trimmedStr.substr(0, end + 1);

	return trimmedStr;
}

MyConverter::MyConverter()
{
	Tree = nullptr;
}
MyConverter::MyConverter(const std::string& xml_s)
{
	GetDataFromXML(xml_s);
}

void MyConverter::GetDataFromXML(const std::string& xml_s) // parse xml data (string) into tree
{
	// create node for parse tree
	std::shared_ptr<Node> Root = std::make_shared<Node>();
	// send string to parser
	Root = RecursiveXMLParser(xml_s);
	// profit
	Tree = Root;
}
std::shared_ptr<Node> MyConverter::RecursiveXMLParser(const std::string& xml_s) // recursively create nodes for all objects
{
	// useless symbols
	const std::string WHITESPACE = " \t\n\v\f\r";

	bool ForAll;			// ForAll atribute of Node
	std::string Category;   // Category attribute of Node
	
	std::vector<std::string> tokens;		//
	std::string token;						//	parsing input string
	std::istringstream tokenStream(xml_s);  //

	// parsing input string into vector of strings
	while (std::getline(tokenStream, token, '\n'))
	{
		// removing unnecessary symbols before and after the relevant part
		token = trim(token);

		// empty strings are ignored
		if (token != "") tokens.push_back(token);
	}

	// first tag must always begin with a "<Category ...:
	if ((*tokens.begin()).find("<Category") != std::string::npos)
	{
		if ((*tokens.begin()) == "<Category everyone=\"\">") ForAll = true;
		else if ((*tokens.begin()) == "<Category>") ForAll = false;
		else return nullptr; // if tag is unknown - error
	}
	else return nullptr; // if not - error

	// remove processed tags
	tokens.erase(tokens.begin());
	// if last tag isn't a closing "</Category>" - error
	if (*(tokens.end()-1) != "</Category>") return nullptr;
	//remove last tag
	tokens.erase(tokens.end() - 1);

	// create a node for new category
	std::shared_ptr<Node> newNodePtr = std::make_shared<Node>();
	// set ForAll boolean which we got from opening tag name
	newNodePtr->SetForAll(ForAll);

	/*
	What if not <Name> ... </Name>
	But
	<Name>
		...
	</Name>
	????????????????
	*/

	// checking first element of a vector for the name of a category
	token = (*tokens.begin());
	// if this line doesn't contain opening and closing tags - error
	if ((token.find("<Name>") == std::string::npos) || (token.find("</Name>") == std::string::npos)) return nullptr;
	// get Category name from the string...
	Category = token.substr(token.find("<Name>") + strlen("<Name>"), token.find("</Name>") - token.find("<Name>") - strlen("<Name>"));

	// removing unnecessary symbols before and after the relevant part
	Category = trim(Category);
	// .. and give it to the newly created node
	newNodePtr->SetCategory(Category);

	// remove processed tag
	tokens.erase(tokens.begin());
	// if this category doesn't contain any children - return pointer to node
	if (tokens.size() == 0) return newNodePtr;

	// if first tag isn't an expected one - error
	if (*(tokens.begin()) != "<Children>") return nullptr;
	// remove processed tag
	tokens.erase(tokens.begin());
	// if last tag isn't an expected one - error
	if (*(tokens.end() - 1) != "</Children>") return nullptr;
	// remove processed tag
	tokens.erase(tokens.end() - 1);

	// while there are unprocessed tags..
	while(tokens.size() > 0)
	{
		// searching for children of new category
		std::vector<std::string>::iterator ChildBegin;
		std::vector<std::string>::iterator ChildEnd;

		// first tag must always begin with a "<Category ...:
		if ((*tokens.begin()).find("<Category") == std::string::npos) return nullptr;

		// first tag is part of child category
		ChildBegin = tokens.begin();

		// flag for when we find the child
		bool FoundNewChild = false;

		// stream for converting vector elements back into string
		std::ostringstream ChildStream;

		// need to keep track of opening and closing category tags inside child
		int pairCount = -1;
		// cycle through tags
		for (ChildEnd = ChildBegin; ChildEnd != tokens.end(); ++ChildEnd)
		{
			// send tag into the stream
			ChildStream << (*ChildEnd) << "\n";

			// new opening category tag increases pairCount by 1
			if ((*ChildEnd).find("<Category") != std::string::npos) pairCount += 1;
			// if closing tag had been encountered
			if ((*ChildEnd) == "</Category>")
			{
				// if this tag is a pair to ChildBegin
				if (pairCount == 0)
				{
					FoundNewChild = true;
					break;
				}
				else pairCount -= 1; // if not, reduce pairCount by 1
			}
		}
		// if no children were found when expected - error
		if (!FoundNewChild) return nullptr;
		// create new child
		std::shared_ptr<Node> Child = std::make_shared<Node>();
		// send child to parser; if error occurrs - also return error
		if ((Child = RecursiveXMLParser(ChildStream.str())) == nullptr) return nullptr;
		// connect parent node to child
		newNodePtr->AddExistingChild(Child);
		// remove tags which belong child
		tokens.erase(ChildBegin, ChildEnd + 1);
	}
	// no error occurred - return node
	return newNodePtr;
}
void MyConverter::GetDataFromCSV(const std::string& csv_s) // parse csv data (string) into existing tree
{
	// if no parse tree exists - error
	if (Tree == nullptr) return;

	// useless symbols
	const std::string WHITESPACE = " \t\n\v\f\r";

	std::vector<std::string> tokens;		//
	std::string token;						//	parsing input string
	std::istringstream tokenStream(csv_s);  //

	// parsing input string into vector of strings
	while (std::getline(tokenStream, token, '\n'))
	{
		// removing unnecessary symbols before and after the relevant part
		token = trim(token);

		// empty strings are ignored
		if (token != "") tokens.push_back(token);
	}
	// sending each string from vector to parser
	for (std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); ++it)
	{
		CSVParser(*it);
	}
}
void MyConverter::CSVParser(const std::string& csv_s) // parse ONE csv string into existing tree
{
	// ";" is a deliminator between left and right parts of a string
	std::string path;			// left part
	std::string value;			// right part

	// get left and right substrings
	size_t pos = csv_s.find(";");
	
	// find right substring
	value = csv_s.substr(pos + 1);
	// removing unnecessary symbols before and after the relevant part
	value = trim(value);

	// find left substring
	path = csv_s.substr(0, pos);

	std::shared_ptr<Node> node;

	std::vector<std::string> tokens;		//
	std::string token;						//	parsing input string
	std::istringstream tokenStream(path);   //

	// parsing input string into vector of strings
	// "." is a deliminator between category names in path
	while (std::getline(tokenStream, token, '.'))
	{
		// removing unnecessary symbols before and after the relevant part
		token = trim(token);
		// empty strings are ignored
		if (token != "") tokens.push_back(token);
	}

	// searching for correct node to put value into
	node = Tree->SeekNode(tokens);
	// if found, put value in it
	if (node != nullptr) node->AddValue(value);
}
const std::string MyConverter::DataToJSON() // return tree data as json string
{
	std::string buf = "";
	if (Tree != nullptr) buf = Tree->JSONPrint();
	if(buf.size() != 0) buf.erase(buf.end() - 2);	// delete the unnecessary comma
	return "[\n" + buf + "]\n";
}
const std::string MyConverter::DataToJSON(const std::string& csv_s) // return tree data as json string
{
	GetDataFromCSV(csv_s);
	return DataToJSON();
}
const std::string MyConverter::DataToJSON(const std::string& xml_s, const std::string& csv_s) // return tree data as json string
{
	GetDataFromXML(xml_s);
	GetDataFromCSV(csv_s);
	return DataToJSON();
}

Node::Node()
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
std::shared_ptr<Node> Node::CreateNewChild(const std::string& childCategory, bool childForAll)
{
	std::shared_ptr<Node> newNodePtr = std::make_shared<Node>();
	Children.push_back(newNodePtr);
	newNodePtr->SetCategory(childCategory);
	if (ParentCategory == "") newNodePtr->SetParentCategory(Category);
	else newNodePtr->SetParentCategory(ParentCategory + "." + Category);
	newNodePtr->SetForAll(childForAll);

	return newNodePtr;
}
void Node::AddExistingChild(std::shared_ptr<Node> child)
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
std::shared_ptr<Node> Node::SeekNode(std::vector<std::string>& path)
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
