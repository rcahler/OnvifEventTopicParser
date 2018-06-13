#pragma once

#ifndef _PUMPJSON_
#define _PUMPJSON



#include "ValidateCred.hpp"
#include "device.hpp"
#include "event.hpp"

void PumpDataJson(Camera cam, DeviceData deviceData, EventData eventData);
vector<vector<string>> ParseEventProperties(vector<soap_dom_element> dom);
class Topic;

#endif // !_PUMPJSON_