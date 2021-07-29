#include <string>
#include <sstream>
#include <vector>
#include "myConverter.h"
#include "StringUtils.h"

MyConverter::MyConverter()
{
	Tree = nullptr;
}
MyConverter::MyConverter(const std::string& xml_s) // same, but creates tree from xml
{
	GetDataFromXML(xml_s);
}

void MyConverter::GetDataFromXML(const std::string& xml_s) // parse xml data (string) into tree
{
	// create node for parse tree
	std::shared_ptr<Node> Root = std::make_shared<Node>();
	// format input data to make sure it isn't one string
	std::string formatXML = xml_s;
	for (size_t pos = 0; pos < formatXML.size(); ++pos)
	{
		if (formatXML[pos] == '>') formatXML.insert(++pos, "\n");
		if (formatXML[pos] == '<') formatXML.insert(pos++, "\n");
	}
	// send string to parser
	Root = RecursiveXMLParser(formatXML);
	// profit
	Tree = Root;
}
std::shared_ptr<Node> MyConverter::RecursiveXMLParser(const std::string& xml_s) // recursively create nodes for all objects
{
	bool ForAll;			// ForAll atribute of Node
	std::string Category;   // Category attribute of Node
	
	std::vector<std::string> tokens;		//
	std::string token;						//	parsing input string
	std::istringstream tokenStream(xml_s);  //

	tokens = getTokens(tokenStream, '\n');

	// processing category opening and closing tags
	if (PopTagFromTokens(tokens, 0, "<Category everyone=\"\">") != "") ForAll = true;
	else if (PopTagFromTokens(tokens, 0, "<Category>") != "") ForAll = false;
	else return nullptr;
	if (PopTagFromTokens(tokens, tokens.size() - 1, "</Category>") == "") return nullptr;

	// create a node for new category and set ForAll
	std::shared_ptr<Node> newNodePtr = std::make_shared<Node>();
	newNodePtr->SetForAll(ForAll);

	// processing name for the new category
	if (PopTagFromTokens(tokens, 0, "<Name>") == "") return nullptr;
	if ((Category = PopTagFromTokens(tokens, 0, "")) == "") return nullptr;
	newNodePtr->SetCategory(Category);
	if (PopTagFromTokens(tokens, 0, "</Name>") == "") return nullptr;

	// if this category doesn't contain any children - return pointer to node
	if (tokens.size() == 0) return newNodePtr;

	// processing children opening and closing tags
	if (PopTagFromTokens(tokens, 0, "<Children>") == "") return nullptr;
	if (PopTagFromTokens(tokens, tokens.size() - 1, "</Children>") == "") return nullptr;

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

	std::vector<std::string> tokens;		//
	std::string token;						//	parsing input string
	std::istringstream tokenStream(csv_s);  //

	// parsing input string into vector of strings
	tokens = getTokens(tokenStream, '\n');

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

	std::vector<std::string> tokens;
	std::istringstream tokenStream(path);

	tokens = getTokens(tokenStream, '.');

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
