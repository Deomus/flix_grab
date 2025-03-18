#include "hardware_logs.h"
#include "hwid_tests.h"
#include "uniconv.h"
#include <hwid.h>

#include <iostream>

using namespace std;

bool WriteLog(const string& file) {
    MapProps props, props2;

    GetHardwareProperties(props);

    WriteHardwareProperties(props, file);
    ReadHardwareProperties(file, props2);
    return IsEqual(props, props2);
}

bool ReadLogAndTest(const string& file) {
    USES_CONVERSION;
    MapProps props;
    ReadHardwareProperties(file, props);

    const char_t* whid = GetCurrentHardwareId(props);
    
    string hid = W2UTF8(whid);

    if (props.at(HID).front() == hid) {
        return MatchCurrentHardwareId(whid, props);
    }
    
    return false;
}


int main(int argc, char *argv[]) {

    if (argc <= 1)
        std::cout << "Hardware Log gathering " << (WriteLog("hardware.dat") ? "completed!" : "failed!") << endl;
    else
        std::cout << "Hardware Log Analyzing " << (ReadLogAndTest(argv[1]) ? "completed!" : "failed!") << endl;

    return 0;

}
