#include <string>
#include <string_view>
#include <iostream>
#include <fstream>
#include <sstream>

#include "./include/MyConverter.h"

int main()
{
	std::ostringstream buf;

	std::string xml, csv, json;

	std::ifstream fin;
	std::ofstream fout;

	// tests
	for (char i = 0; i < 1; i++)
	{
		MyConverter Converter;

		// open XML and get it's contents as a string
		fin.open("./testdata/test" + std::to_string(i + 1) + "/test" + std::to_string(i + 1) + ".xml");
		if (!fin) 
			exit(-1 - (i + 1) * 10);
		buf.str("");
		buf << fin.rdbuf();
		xml = buf.str();
		fin.close();

		// open CSV and get it's contents as a string
		fin.open("./testdata/test" + std::to_string(i + 1) + "/test" + std::to_string(i + 1) + ".csv");
		if (!fin) 
			exit(-2 - (i + 1) * 10);
		buf.str("");
		buf << fin.rdbuf();
		csv = buf.str();
		fin.close();

		// generate output string from tree
		json = Converter.DataToJSON(xml, csv);

		// open JSON and output resulting string
		fout.open("./testdata/test" + std::to_string(i + 1) + "/result" + std::to_string(i + 1) + ".json");
		if (!fout) 
			exit(-3 - (i + 1) * 10);
		fout << json;
		fout.close();
	}

	MyConverter Converter;

	// open XML and get it's contents as a string
	fin.open("./data/structure.xml");
	if (!fin)
		exit(-1);
	buf.str("");
	buf << fin.rdbuf();
	xml = buf.str();
	fin.close();

	// parse XML data into new tree
	Converter.GetDataFromXML(xml);

	// open CSV and get it's contents as a string
	fin.open("./data/values.csv");
	if (!fin) 
		exit(-2);
	buf.str("");
	buf << fin.rdbuf();
	csv = buf.str();
	fin.close();

	// parse CSV data into existing tree
	Converter.GetDataFromCSV(csv);

	// generate output string from tree
	json = Converter.DataToJSON();

	// open JSON and output resulting string
	fout.open("./data/values.json", std::ios_base::trunc);
	if (!fout)
		exit(-3);
	fout << json;
	fout.close();
	
	return 0;
}
