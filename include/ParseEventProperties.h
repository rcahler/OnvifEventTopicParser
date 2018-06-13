#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <soapH.h>
#include "split.h"

using namespace std;

vector<vector<string>> ParseEventProperties(vector<soap_dom_element> dom);
