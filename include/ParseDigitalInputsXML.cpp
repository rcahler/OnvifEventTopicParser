#include "ParseDigitalInputsXML.hpp"
#include <iostream>
#include "split.h"

std::vector<std::string> ParseDigitalInputsXML(char * string)
{
	std::string xml = std::string(string);
	xml.find("<tds:GetDigitalInputsResponse>");
	std::string init = "<tds:GetDigitalInputsResponse>";
	std::string fin = "</tds:GetDigitalInputsResponse>";

	std::size_t begin_index = xml.find(init) + init.length();
	std::size_t end_index = xml.find(fin);

	std::string diXML(end_index - begin_index, ' ');
	for (size_t i = begin_index; i < end_index; ++i) {
		diXML[i - begin_index] = xml[i];
	}
	diXML = diXML.substr(1, diXML.length());
	
	std::vector<std::string> inputs = split(diXML, '<');
	std::vector<std::string> parsed_inputs;

	for (size_t i = 0; i < inputs.size(); ++i) {
		std::string begin = "token=\"";
		std::string end = "\">";
		begin_index = inputs[i].find(begin);
		end_index = inputs[i].find(end);
		if ((begin_index != -1)||(end_index != -1)) {
			std::string string(end_index - begin_index - begin.length(), 'x');
			for (size_t j = begin_index + begin.length(); j < end_index; ++j) {
				string[j - begin_index - begin.length()] = inputs[i][j];
			}
			parsed_inputs.push_back(string);
		}
	}

	return parsed_inputs;
}
