#ifndef _Pump_
#define _Pump_



#include "ValidateCred.hpp"
#include "device.hpp"
#include "event.hpp"
using namespace std;

void PumpDataRaw(Camera cam, DeviceData deviceData, EventData eventData);
void PumpDataJson(Camera cam, DeviceData deviceData, EventData eventData);
#endif // !_Pump_