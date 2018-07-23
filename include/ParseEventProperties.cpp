#include <string>
#include <vector>
#include <sstream>
#include <soapH.h>
#include "split.h"
#include "Topic.h"
#include "ParseEventProperties.h"

//gsoap bindings do not parse the topics, instead returning soap_dom_elements which must be manually parsed
std::vector<Topic> ParseEventProperties(std::vector<soap_dom_element> dom) {

	std::stringstream stream;
	
	for (size_t i = 0; i < dom.size(); i++) {
		size_t depth = 0;
		for (soap_dom_element::iterator it = dom[i].begin(); it != dom[i].end(); ++it) {
			if (it->depth() > depth) {
				depth = it->depth();
			}
		}

		std::cout << depth << std::endl;

		std::string* Topic = new std::string[depth];
		// print attribute tags
		for (soap_dom_element::iterator it = dom[i].begin(); it != dom[i].end(); ++it) {
			std::string itTag(it->tag());
			Topic[it->depth() - 1] = itTag;
			if (itTag.find("tt:SimpleItem") != std::string::npos) { //Get the attributes of the element
				if (it->atts) {
					//There has to be a better way to do this but I can't figure it out
					soap_dom_attribute dom = *it->atts;
					soap_dom_attribute domNext = *it->atts->next;

					for (size_t j = 0; j < depth; j++) {
						if (Topic[j].find("tt:") == std::string::npos) {
							//Make look better
							if (Topic[j].find(':') != std::string::npos) {
								stream << split(Topic[j], ':')[1] << "/";
							}
							else {
								stream << Topic[j] << "/"; 
							}
						}
					}

					for (xsd__anyAttribute::iterator it = dom.att_begin(); it != dom.att_end(); ++it) {
						stream << " " << it->get_text();
					}
					if (it->atts->next) {
						for (xsd__anyAttribute::iterator it = domNext.att_begin(); it != domNext.att_end(); ++it) {
							stream << " " << it->get_text() << std::endl;
						}
					}
				}
			}
		}
	}

	std::vector<std::string> splitByLine = split(stream.str(), '\n');
	std::vector<std::vector<std::string>> splitBySpace;

	for (size_t i = 0; i < splitByLine.size(); i++) {
		std::vector<std::string> vec1 = split(splitByLine[i], ' ');
		bool boo = true;
		for (size_t j = 0; j < splitBySpace.size(); j++) {
			if (splitBySpace[j][0] != vec1[0]) {
				boo = true;
			}
			else {
				boo = false;
			}
		}

		if (boo) {
			splitBySpace.push_back(vec1);
		}
		else {
			std::vector<std::string> v = splitBySpace[splitBySpace.size() - 1]; //Should return last element in vector
			splitBySpace.pop_back();
			v.push_back(vec1[1]);
			v.push_back(vec1[2]);
			splitBySpace.push_back(v);
		}
	}

	std::vector<Topic> topics;
	for (size_t i = 0; i < splitBySpace.size(); i++) {

		Topic topic;
		topic.name = splitBySpace[i][0];

		std::vector<std::pair<std::string, std::string>> elements;

		for (size_t k = 1; k < splitBySpace[i].size(); k += 2) {
			std::pair<std::string, std::string> pair = make_pair(splitBySpace[i][k], splitBySpace[i][k + 1]);
			elements.push_back(pair);
		}
		topic.elements = elements;

		topics.push_back(topic);

	}

	return topics;
}