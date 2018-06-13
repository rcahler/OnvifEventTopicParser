#include <string>
#include <vector>
#include <sstream>
#include <soapH.h>
#include "split.h"
#include "ParseEventProperties.h"

using namespace std;

//Each of the string vectors returned has the Topic name, the name of the data and the data type as seperate strings
vector<vector<string>> ParseEventProperties(vector<soap_dom_element> dom) {

	stringstream stream;
	//Parse
	for (int i = 0; i < dom.size(); i++) {

		//This clearly is not the most effiecient way of doing this, but soap_dom does not provide a distance class
		int depth = 0;
		for (soap_dom_element::iterator it = dom[i].begin(); it != dom[i].end(); ++it) {
			if (it->depth() > depth) { depth = it->depth(); }
		}
		string* Topic = new string[depth];
		// print attribute tags
		for (soap_dom_element::iterator it = dom[i].begin(); it != dom[i].end(); ++it) {
			string itTag(it->tag());
			Topic[it->depth() - 1] = itTag;
			if (itTag.find("tt:SimpleItemDescription") != string::npos) { //Get the attributes of the element
				if (it->atts) {
					//There has to be a better way to do this but I can't figure it out
					soap_dom_attribute dom = *it->atts;
					soap_dom_attribute domNext = *it->atts->next;

					for (int j = 0; j < depth; j++) {
						if (Topic[j].find("tt:") == string::npos) {
							//Make look better
							if (Topic[j].find(':') != string::npos) { stream << split(Topic[j], ':')[1] << "/"; }
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
							stream << " " << it->get_text() << endl;
						}
					}
				}
			}
		}
	}
	vector<string> splitByLine = split(stream.str(), '\n');
	vector<vector<string>> splitBySpace;

	for (int i = 0; i < splitByLine.size(); i++) {
		vector<string> vec1 = split(splitByLine[i], ' ');
		bool boo = true;
		for (int j = 0; j < splitBySpace.size(); j++) {
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
			vector<string> v = splitBySpace[splitBySpace.size() - 1]; //Should return last element in vector
			splitBySpace.pop_back();
			v.push_back(vec1[1]);
			v.push_back(vec1[2]);
			splitBySpace.push_back(v);
		}
	}

	return splitBySpace;
}