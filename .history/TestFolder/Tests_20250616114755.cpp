#include <cstdio>

#include "Tests.hpp"

Tests::Tests() {
    registerTests();
}

void Tests::registerTests(){
    testRegistry = {
        //String Tests
        {"get_Software_Version_Correctness", {[this]() { return get_Software_Version_Correctness(); }, false, false }},
        {"get_Hardware_Version_Correctness", {[this]() { return get_Hardware_Version_Correctness(); }, false, false }},
        {"get_Serial_Number_Correctness", {[this]() { return get_Serial_Number_Correctness(); }, false, false }},
        {"get_Battery_Status_Correctness", {[this]() { return get_Battery_Status_Correctness(); }, false, false }},
        {"get_Resolution_Information_Correctness", {[this]() { return get_Resolution_Information_Correctness(); }, false, false }},
        {"get_Device_Orientation_Correctness", {[this]() { return get_Device_Orientation_Correctness(); }, false, false }},
        {"get_Height_Information_Correctness", {[this]() { return get_Height_Information_Correctness(); }, false, false }},
        {"get_Device_Name_Correctness", {[this]() { return get_Device_Name_Correctness(); }, false, false }},
        //Display Tests
        {"update_Display_Functionality", {[this]() { return update_Display_Functionality(); }, false, false }},
        {"set_Display_Funcitonality", {[this]() { return set_Display_Funcitonality(); }, false, false }},
        {"clear_Display_Functionality", {[this]() { return clear_Display_Functionality(); }, false, false }},
        {"update_Single_Pixel_Functionality", {[this]() { return update_Single_Pixel_Functionality(); }, false, false }},
        {"update_Single_Row_Functionality", {[this]() { return update_Single_Row_Functionality(); }, false, false }},
        {"update_Single_Column_Functionality", {[this]() { return update_Single_Column_Functionality(); }, false, false }},
        //Status Tests
        {"get_ALL_Pixels_Position_Status_Correctness", {[this]() { return get_ALL_Pixels_Position_Status_Correctness(); }, false, false }},
        {"get_Single_Pixel_Position_Status_Correctness", {[this]() { return get_Single_Pixel_Position_Status_Correctness(); }, false, false }},
        {"get_Single_Row_Pixels_Position_Status_Correctness", {[this]() { return get_Single_Row_Pixels_Position_Status_Correctness(); }, false, false }},
        {"get_Single_Column_Pixels_Position_Status_Correctness", {[this]() { return get_Single_Column_Pixels_Position_Status_Correctness(); }, false, false }},
        //Key Test
        {"get_Next_KeyEvent_Functionality", {[this]() { return get_Next_KeyEvent_Functionality(); }, true, false }},
        //Draw Test
        {"get_Next_Draw_Event_Functionality", {[this]() { return get_Next_Draw_Event_Functionality(); }, false, true }},
        //Show Message Test
        {"show_Message_On_The_Device_Functionality", {[this]() { return show_Message_On_The_Device_Functionality(); }, false, false }},
        //Set Cursor Test
        {"set_Cursor_Functionality", {[this]() { return set_Cursor_Functionality(); }, false, false }},
        //Image Test
        {"send_Image_Functionality", {[this]() { return send_Image_Functionality(); }, false, false }},
        //Status Test
        {"get_Last_Touch_Point_Status_Functionality", {[this]() { return vibrator_Control_Command_Functionality(); }, false, false }},
        //Vibrator Test
        {"vibrator_Control_Command_Functionality", {[this]() { return vibrator_Control_Command_Functionality(); }, false, false }},
        //Clock Test
        {"get_Date_And_Time_Correctness", {[this]() { return get_Date_And_Time_Correctness(); }, false, false }},
        {"set_Date_And_Time_Correctness", {[this]() { return set_Date_And_Time_Correctness(); }, false, false }}
    };
}

bool Tests::startTesting(bool keyEvents, bool touchEvents, 
    std::function<bool()> func){
    graphiti = new GraphitiExtension();
    if(!graphiti->startUpVCP("COM4", keyEvents, touchEvents)){
        return false;
    }

    bool result = func();
    
    graphiti->shutDownVCP(keyEvents, touchEvents);
    delete graphiti;
    return result;
}

bool Tests::sleepAndTest(std::string expected){
    graphiti->sleep(2);
    auto output = graphiti->getNextOutputEvent();
    if(output.has_value()) {
        if(output.value() != expected) {
            std::cerr << output.value() << 
                " was not: " << expected << std::endl;
            return false;
        }
        return true;
    }
    return false;
}

bool Tests::get_Software_Version_Correctness() {
    graphiti->getSoftwareVersion();
    return sleepAndTest("B0.05.00.01r04");
}

bool Tests::get_Hardware_Version_Correctness() {
    graphiti->getHardWareVersion();
    return sleepAndTest("MB 4E");
}

bool Tests::get_Serial_Number_Correctness() {
    graphiti->getSerialNumber();
    return sleepAndTest("MB 4E A0027");
}

bool Tests::get_Battery_Status_Correctness() {
    graphiti->getBatteryStatus();
    return sleepAndTest("Charging 100%");
}

bool Tests::get_Resolution_Information_Correctness() {
    graphiti->getResolutionInformation();
    return sleepAndTest("60|40");
}

bool Tests::get_Device_Orientation_Correctness() {
    graphiti->getDeviceOrientation();
    return sleepAndTest("Landscape");
}

bool Tests::get_Height_Information_Correctness() {
    graphiti->getHeightInformation();
    return sleepAndTest("4");
}

bool Tests::get_Device_Name_Correctness() {
    graphiti->getDeviceName();
    return sleepAndTest("Graphiti");
}

//Display Tests (Test success/failure is obvserved)

bool Tests::update_Display_Functionality() {
    UpdateDisplay();
    return true;
}

bool Tests::set_Display_Funcitonality() {
    graphiti->setDisplay();
    graphiti->sleep(5);
    return true;
}

bool Tests::clear_Display_Functionality() {
    graphiti->clearDisplay();
    graphiti->sleep(2);
    return true;
}

bool Tests::update_Single_Pixel_Functionality() {
    graphiti->updateSinglePixel(20, 30, 2, 0);
    graphiti->sleep(2);
    return true;
}

bool Tests::update_Single_Row_Functionality() {
    UpdateRow(3);
    return true;
}

bool Tests::update_Single_Column_Functionality() {
    UpdateColumn(25);
    return true;
}

//Display Tests

bool Tests::get_ALL_Pixels_Position_Status_Correctness() {
    return statusFunctionResponse(
        "UpdateDisplay",
        "getAllPixelsPositionStatus"
    );
}


bool Tests::get_Single_Pixel_Position_Status_Correctness() {
    return statusFunctionResponse(
        "updateSinglePixel",
        "getSinglePixelPositionStatus",
        25, 30, 2, 0
    );
}


bool Tests::get_Single_Row_Pixels_Position_Status_Correctness() {
    return statusFunctionResponse(
        "UpdateRow",
        "getSingleRowPixelsPositionStatus",
        5
    );
}


bool Tests::get_Single_Column_Pixels_Position_Status_Correctness() {
    return statusFunctionResponse(
        "UpdateColumn",
        "getSingleColumnPixelsPositionStatus",
        35
    );
}

//Key Test

bool Tests::get_Next_KeyEvent_Functionality() {
    std::cout << "get_Next_KeyEvent_Functionality" << std::endl;

    loopCondition = true;

    graphiti->keyLoop(
        [this](const std::set<std::string>& keys, void* params) {
            OnePixel_KeyFunction(keys, params);
        },
        &loopCondition
    );

    return true;
}

//Draw Test

bool Tests::get_Next_Draw_Event_Functionality() {
    std::cout << "get_Next_Draw_Event_Functionality" << std::endl;

    loopCondition = true;

    graphiti->drawLoop([this](const Graphiti_API::DrawEvent& drawEvent, void* params) {
            PrintDraw_DrawFunction(drawEvent, params);
        }, &loopCondition
    );

    return true;
}

//Show Message Test
//Press Space + 3 to enable message bar
bool Tests::show_Message_On_The_Device_Functionality() {
    graphiti->showMessage("abcdefghijklmnopqr",{1,2,2},2);
    graphiti->sleep(2);

    return true;
}

//Set Cursor Test

bool Tests::set_Cursor_Functionality() {
    graphiti->setCursor(20, 30, 2, 10, 3);
    graphiti->sleep(2);

    return true;
}


//Image Test

bool Tests::send_Image_Functionality() {
    const std::string name = "blueline.jpg";

    const std::string fullPath =
    "C:\\Users\\Daniel\\Visual Studio Code Projects\\"
    "Graphiti_API_Library_0_1\\images\\" + name;

    const std::string relativePath = "images\\" + name;

    std::string path = relativePath;

    try {
        graphiti->sendImageBlocking(name, path);
        graphiti->sleep(10);
        graphiti->sendImageInterruptible(name, path);
        graphiti->sleep(10);

        path = fullPath;

        graphiti->sendImageBlocking(name, path);
        graphiti->sleep(10);
        graphiti->sendImageInterruptible(name, path);
        graphiti->sleep(10);
    } catch (const std::exception& e) {
        std::cerr << "[EXCEPTION] " << e.what() << "\n";
        return false;
    } catch (...) {
        std::cerr << "[EXCEPTION] Unknown crash occurred" 
            << "in send_Image_Functionality\n";
        return false;
    }
    return true;
}

//Status Test

bool Tests::get_Last_Touch_Point_Status_Functionality() {
    graphiti->getLastTouchPointStatus();
    graphiti->sleep(2);

    auto displayOption = graphiti->getNextDisplayStatusEvent();
    if(displayOption.has_value()){
        auto bytes = displayOption.value();
        return bytes.size() == 3;
    }
    return false;
}

//Vibrator Test

bool Tests::vibrator_Control_Command_Functionality() {

    graphiti->vibratorControlCommand(10,40,100);
    graphiti->sleep(2);

    return true;
}

//Clock Test

bool Tests::get_Date_And_Time_Correctness() {
    graphiti->getDateAndTime();

    auto now = std::chrono::system_clock::now();

    std::time_t now_time = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;

    ss << std::put_time(std::localtime(&now_time), "%m-%d-%Y %H:%M:%S");

    return sleepAndTest(ss.str());
}


bool Tests::set_Date_And_Time_Correctness() {
    auto now = std::chrono::system_clock::now();

    std::time_t now_time = std::chrono::system_clock::to_time_t(now);

    std::tm* local_time = std::localtime(&now_time);

    return true;
}


std::vector<uint8_t> Tests::UpdateDisplay() {
    std::vector<uint8_t> screen(MAX_COLUMN_DATA * MAX_ROW_DATA * 2, 0);
    std::vector<uint8_t> pinHeightsForTest(MAX_COLUMN_DATA * MAX_ROW_DATA, 0);

    for (size_t i = 0; i < MAX_COLUMN_DATA* MAX_ROW_DATA; i++)
    {
        screen[i*2] = 2;//Pixel height
        pinHeightsForTest[i] = 2;
        screen[i*2 + 1] = 0;//Blinking rate
    }

    graphiti->updateDisplay(screen);
    graphiti->sleep(5);

    return pinHeightsForTest;
}

std::vector<uint8_t> Tests::UpdateRow(uint8_t row) {
    std::vector<uint8_t> rowData(MAX_ROW_DATA * 2, 0);
    std::vector<uint8_t> pinHeightsForTest(MAX_ROW_DATA, 0);

    for (size_t i = 0; i < MAX_ROW_DATA; i++)
    {
        rowData[i*2] = 2;//Pixel height
        pinHeightsForTest[i] = 2;
        rowData[i*2 + 1] = 0;//Blinking rate
    }

    graphiti->updateSingleRow(row, rowData);
    graphiti->sleep(2);

    pinHeightsForTest.insert(pinHeightsForTest.begin(), row);

    return pinHeightsForTest;
}

std::vector<uint8_t> Tests::UpdateColumn(uint8_t column) {
    std::vector<uint8_t> columnData(MAX_COLUMN_DATA * 2, 0);
    std::vector<uint8_t> pinHeightsForTest(MAX_COLUMN_DATA, 0);

    for (size_t i = 0; i < MAX_COLUMN_DATA; i++)
    {
        columnData[i*2] = 2;//Pixel height
        pinHeightsForTest[i] = 2;
        columnData[i*2 + 1] = 0;//Blinking rate
    }

    graphiti->updateSingleColumn(column, columnData);
    graphiti->sleep(4);

    pinHeightsForTest.insert(pinHeightsForTest.begin(), column);

    return pinHeightsForTest;
}

std::vector<uint8_t> Tests::displayFunctionWrapper(std::string funcitonName, 
        int arg1, int arg2, int arg3, int arg4) 
{
    if(funcitonName == "UpdateDisplay") 
    {
        return UpdateDisplay();
    } 
    else if (funcitonName == "updateSinglePixel") 
    {
        graphiti->updateSinglePixel(arg1, arg2, arg3, arg4);
        graphiti->sleep(2);
        return {static_cast<uint8_t>(arg1), 
            static_cast<uint8_t>(arg2), 
            static_cast<uint8_t>(arg3)};
    }
    else if (funcitonName == "UpdateRow") 
    {
        return UpdateRow(arg1);
    }
    else if (funcitonName == "UpdateColumn") 
    {
        return UpdateColumn(arg1);
    }

    return {0x00};//Error case
}

void Tests::statusFunctionWrapper(std::string funcitonName, int arg1, int arg2) {
    if(funcitonName == "getAllPixelsPositionStatus") 
    {
        graphiti->getAllPixelsPositionStatus();
    } 
    else if (funcitonName == "getSinglePixelPositionStatus") 
    {
        graphiti->getSinglePixelPositionStatus(arg1, arg2);
    }
    else if (funcitonName == "getSingleRowPixelsPositionStatus") 
    {
        graphiti->getSingleRowPixelPositionStatus(arg1);
    }
    else if (funcitonName == "getSingleColumnPixelsPositionStatus") 
    {
        graphiti->getSingleColumnPixelPositionStatus(arg1);
    }
}

bool Tests::statusFunctionResponse(
    std::string displayFunctionName,
    std::string statusFunctionName,
    int arg1, int arg2, int arg3, int arg4
) {
    auto expected = displayFunctionWrapper(
        displayFunctionName, arg1, arg2, arg3, arg4);

    statusFunctionWrapper(statusFunctionName, arg1, arg2);
    graphiti->sleep(2);

    auto displayOption = graphiti->getNextDisplayStatusEvent();
    graphiti->sleep(2);

    graphiti->clearDisplay();
    graphiti->sleep(5);

    if (!displayOption.has_value()) {
        return false;
    } else {
        auto display = displayOption.value();
        //printVectorHex(display);
        return expected == display;
    }
}

void Tests::OnePixel_KeyFunction(
    const std::set<std::string>& keyEvent,
    void* parameters
) {
    if(keyEvent.contains("1")){
        std::cout << "Key Event 1 on Main found" << std::endl;
        graphiti->updateSinglePixel(10, 30, 2, 0); //Put up
    } else {
        graphiti->updateSinglePixel(10, 30, 0, 0); //Put down
    }

    if(keyEvent.contains("Select")) {
        loopCondition = false; //Ends the loop
    }
}

void Tests::PrintDraw_DrawFunction(
    const Graphiti_API::DrawEvent& drawEvent, 
    void* parameters
) {
    Graphiti_API::PinInfo pin;

    for (size_t i = 0; i < drawEvent.length; i++)
    {
        pin = drawEvent.pins[i];
        std::cout 
            << "row: " << pin.rowID << " "
            << "col: " << pin.columnID << " "
            << "height: " << pin.height << " "
            << "blinkRate: " << pin.blinkRate << std::endl;

        if(drawEvent.pins[i].rowID == 40 
            and drawEvent.pins[i].columnID == 60 ){
            loopCondition = false;
        }
    }
}