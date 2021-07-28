#include <string>
#include <string_view>
#include <iostream>
#include <fstream>
#include <sstream>

#include "MyConverter.h"

int main()
{
	std::ostringstream buf;

	std::string xml, csv, json;

	std::ifstream fin;
	std::ofstream fout;

	MyConverter Converter;

	// open XML and get it's contents as a string
	fin.open("structure.xml");
	if (!fin) exit(-1);
	buf.str("");
	buf << fin.rdbuf();
	xml = buf.str();
	fin.close();

	// parse XML data into new tree
	Converter.GetDataFromXML(xml);

	// open CSV and get it's contents as a string
	fin.open("values.csv");
	if (!fin) exit(-1);
	buf.str("");
	buf << fin.rdbuf();
	csv = buf.str();
	fin.close();

	// parse CSV data into existing tree
	Converter.GetDataFromCSV(csv);

	// generate output string from tree
	json = Converter.DataToJSON();

	// open JSON and output resulting string
	fout.open("values.json", std::ios_base::trunc);
	if (!fout) exit(-1);
	fout << json;
	fout.close();

	return 0;
}
