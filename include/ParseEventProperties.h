#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <soapH.h>
#include "split.h"
#include "Topic.h"

std::vector<Topic> ParseEventProperties(std::vector<soap_dom_element> dom);
