#include <cstdio>
#include <map>
#include <functional>
#include <chrono>
#include <iomanip>

#include <Graphiti/Extension.hpp>

/**
 * @brief This file is depreciatted
 * 
 */

GraphitiExtension* graphiti = nullptr;
bool loopCondition = true;

/**
 * @brief One Pixel is a key function that does operations from key output
 * 
 * @param keyEvent keys being pressed at the same time
 * @param context other input
 */
void OnePixel_KeyFunction(const std::set<std::string>& keyEvent, void* context) {
    if(keyEvent.contains("1")){
        std::cout << "Key Event 1 on Main found" << std::endl;
        graphiti->updateSinglePixel(10, 30, 2, 0); //Put up
    } else {
        graphiti->updateSinglePixel(10, 30, 0, 0); //Put down
    }

    if(keyEvent.contains("2")){
        std::vector<uint8_t> rowData(MAX_ROW_DATA * 2, 0);
        uint8_t row = 3;

        for (size_t i = 0; i < MAX_ROW_DATA; i++)
        {
            rowData[i*2] = 2;//Pixel height
            rowData[i*2 + 1] = 0;//Blinking rate
        }

        graphiti->updateSingleRow(row, rowData);
        graphiti->sleep(2);
    }

    if(keyEvent.contains("3")){
        std::vector<uint8_t> columnData(MAX_COLUMN_DATA * 2, 0);
        uint8_t column = 25;

        for (size_t i = 0; i < MAX_COLUMN_DATA; i++)
        {
            columnData[i*2] = 2;//Pixel height
            columnData[i*2 + 1] = 0;//Blinking rate
        }

        graphiti->updateSingleColumn(column, columnData);
        graphiti->sleep(4);
    }

    if(keyEvent.contains("Select")) {
        loopCondition = false; //Ends the loop
    }
}

void PrintDraw_DrawFunction(const Graphiti_API::DrawEvent& drawEvent, void* context) {
    Graphiti_API::PinInfo pin;

    for (size_t i = 0; i < drawEvent.length; i++)
    {
        pin = drawEvent.pins[i];
        std::cout 
            << "row: " << pin.rowID << " "
            << "col: " << pin.columnID << " "
            << "height: " << pin.height << " "
            << "blinkRate: " << pin.blinkRate << std::endl;
    }
}

void sleepAndOutput(){
    graphiti->sleep(2);
    auto output = graphiti->getNextOutputEvent();
    if(output.has_value()) {
        std::cout << output.value() << std::endl;
    }
}

/**
 * @brief Run with key responses off
 * 
 */
void ReadOuptputDemo() {
    graphiti->getSoftwareVersion();
    sleepAndOutput();

    graphiti->getHardWareVersion();
    sleepAndOutput();

    graphiti->getSerialNumber();
    sleepAndOutput();

    graphiti->getBatteryStatus();
    sleepAndOutput();

    graphiti->getResolutionInformation();
    sleepAndOutput();

    graphiti->getDeviceOrientation();
    sleepAndOutput();

    graphiti->getHeightInformation();
    sleepAndOutput();

    graphiti->getDeviceName();
    sleepAndOutput();
}

/**
 * @brief Update Display
 * 
 * Calls update dislay with 2 height 0 blinkRate
 * 
 * @return std::vector<uint8_t> screen vector it was set to
 */
std::vector<uint8_t> UpdateDisplay() {
    std::vector<uint8_t> screen(MAX_COLUMN_DATA * MAX_ROW_DATA * 2, 0);
    std::vector<uint8_t> pinHeightsForTest(MAX_COLUMN_DATA * MAX_ROW_DATA, 0);

    for (size_t i = 0; i < MAX_COLUMN_DATA* MAX_ROW_DATA; i++)
    {
        screen[i*2] = 2;//Pixel height
        pinHeightsForTest[i] = 2;
        screen[i*2 + 1] = 0;//Blinking rate
    }

    graphiti->updateDisplay(screen);

    return pinHeightsForTest;
}

/**
 * @brief Update Row
 * 
 * Calls updateSingleRow with 2 height and 0 blink rate
 * 
 * @return std::vector<uint8_t> row vector it was set to
 */
std::vector<uint8_t> UpdateRow(uint8_t row) {
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

/**
 * @brief Update Column
 * 
 * Calls updateSingleColumn with 2 height and 0 blink rate
 * 
 * @return std::vector<uint8_t> column vector it was set to
 */
std::vector<uint8_t> UpdateColumn(uint8_t column) {
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

/**
 * @brief 4.3.1 - 4.3.5 Function tests
 * 
 */
void UpdatePixelsDemo() {
    UpdateDisplay();
    graphiti->sleep(5);

    graphiti->setDisplay();
    graphiti->sleep(5);

    graphiti->clearDisplay();
    graphiti->sleep(2);

    graphiti->updateSinglePixel(20, 30, 2, 0);
    graphiti->sleep(2);
    
    UpdateRow(3);

    UpdateColumn(25);
}

std::vector<uint8_t> displayFunctionWrapper(std::string funcitonName, 
        int arg1, int arg2, int arg3, int arg4) 
{
    if(funcitonName == "UpdateDisplay") 
    {
        return UpdateDisplay();
    } 
    else if (funcitonName == "updateSinglePixel") 
    {
        graphiti->updateSinglePixel(arg1, arg2, arg3, arg4);
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

void statusFunctionWrapper(std::string funcitonName, int arg1, int arg2) {
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

void printVectorHex(const std::vector<uint8_t>& vec) {
    std::cout << "[ ";
    for (uint8_t byte : vec) {
        std::cout << "0x" << std::hex << std::setw(2) << std::setfill('0') << (int)byte << " ";
    }
    std::cout << "]" << std::dec << std::endl; // Reset back to decimal
}

void statusFunctionResponse(
    std::string displayFunctionName, int displayDelay,
    std::string statusFunctionName,
    int arg1 = 0, int arg2 = 0, int arg3 = 0, int arg4 = 0
) {
    auto expected = displayFunctionWrapper(displayFunctionName, arg1, arg2, arg3, arg4);
    graphiti->sleep(displayDelay);

    statusFunctionWrapper(statusFunctionName, arg1, arg2);
    graphiti->sleep(2);

    auto displayOption = graphiti->getNextDisplayStatusEvent();
    if (!displayOption.has_value()) {
        std::cout 
            << statusFunctionName 
            << ": no display event received" 
            << std::endl;
    } else {
        auto display = displayOption.value();
        std::cout << statusFunctionName << ": ";
        if(expected == display) {
            std::cout << "passed";
        } else {
            std::cout << "failed";
            //printVectorHex(display);
        }

        std::cout << std:: endl;
    }
}



/**
 * @brief Get Status commands 4.3.6 - 4.3.9
 * 
 * Requires Update Pixels Demo to work
 * 
 */
void GetStatusDemo(){

    //Fails because 3 pixels do not go up on the device because they are broken
    statusFunctionResponse(
        "UpdateDisplay", 5,
        "getAllPixelsPositionStatus"
    );

    graphiti->clearDisplay();
    graphiti->sleep(5);

    statusFunctionResponse(
        "updateSinglePixel", 2,
        "getSinglePixelPositionStatus",
        25, 30, 2, 0
    );

    graphiti->clearDisplay();
    graphiti->sleep(5);

    statusFunctionResponse(
        "UpdateRow", 2,
        "getSingleRowPixelsPositionStatus",
        5
    );

    graphiti->clearDisplay();
    graphiti->sleep(5);

    statusFunctionResponse(
        "UpdateColumn", 4,
        "getSingleColumnPixelsPositionStatus",
        35
    );
}

void KeyDemo() {
    graphiti->keyLoop(OnePixel_KeyFunction, &loopCondition);
}

void DrawDemo() {
    #ifdef DEBUG
        std::cout << "Starting Draw Loop" << std::endl;
    #endif 

    graphiti->drawLoop(PrintDraw_DrawFunction, &loopCondition);
}

void ImageDemo() {
    const std::string name = "blueline.jpg";

    const std::string fullPath =
    "C:\\Users\\Daniel\\Visual Studio Code Projects\\"
    "Graphiti_API_Library_0_1\\images\\" + name;

    const std::string relativePath = "images\\" + name;

    const std::string path = relativePath;

    try {
        graphiti->sendImageBlocking(name, path);
        //graphiti->sendImageInterruptible(name, path);
        graphiti->sleep(10);
    } catch (const std::exception& e) {
        std::cerr << "[EXCEPTION] " << e.what() << "\n";
    } catch (...) {
        std::cerr << "[EXCEPTION] Unknown crash occurred\n";
    }
}

void createImage(){
    std::vector<uint8_t> image_data = { 
        0xFF,0xD8,0xFF,0xE0,0x00,0x10,0x4A,0x46,0x49,0x46,0x00,
        0x01,0x01,0x00,0x00,0x01,0x00,0x01,0x00,0x00,0xFF,0xDB,
        0x00,0x43,0x00,0x05,0x03,0x04,0x04,0x04,0x03,0x05,0x04,
        0x04,0x04,0x05,0x05,0x05,0x06,0x07,0x0C,0x08,0x07,0x07,
        0x07,0x07,0x0F,0x0B,0x0B,0x09,0x0C,0x11,0x0F,0x12,0x12,
        0x11,0x0F,0x11,0x11,0x13,0x16,0x1C,0x17,0x13,0x14,0x1A,
        0x15,0x11,0x11,0x18,0x21,0x18,0x1A,0x1D,0x1D,0x1F,0x1F,
        0x1F,0x13,0x17,0x22,0x24,0x22,0x1E,0x24,0x1C,0x1E,0x1F,
        0x1E,0xFF,0xDB,0x00,0x43,0x01,0x05,0x05,0x05,0x07,0x06,
        0x07,0x0E,0x08,0x08,0x0E,0x1E,0x14,0x11,0x14,0x1E,0x1E,
        0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,
        0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,
        0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,
        0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,
        0x1E,0x1E,0x1E,0x1E,0xFF,0xC0,0x00,0x11,0x08,0x00,0x28,
        0x00,0x3C,0x03,0x01,0x22,0x00,0x02,0x11,0x01,0x03,0x11,
        0x01,0xFF,0xC4,0x00,0x1F,0x00,0x00,0x01,0x05,0x01,0x01,
        0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,
        0x0B,0xFF,0xC4,0x00,0xB5,0x10,0x00,0x02,0x01,0x03,0x03,
        0x02,0x04,0x03,0x05,0x05,0x04,0x04,0x00,0x00,0x01,0x7D,
        0x01,0x02,0x03,0x00,0x04,0x11,0x05,0x12,0x21,0x31,0x41,
        0x06,0x13,0x51,0x61,0x07,0x22,0x71,0x14,0x32,0x81,0x91,
        0xA1,0x08,0x23,0x42,0xB1,0xC1,0x15,0x52,0xD1,0xF0,0x24,
        0x33,0x62,0x72,0x82,0x09,0x0A,0x16,0x17,0x18,0x19,0x1A,
        0x25,0x26,0x27,0x28,0x29,0x2A,0x34,0x35,0x36,0x37,0x38,
        0x39,0x3A,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x53,
        0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x63,0x64,0x65,0x66,
        0x67,0x68,0x69,0x6A,0x73,0x74,0x75,0x76,0x77,0x78,0x79,
        0x7A,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x92,0x93,
        0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0xA2,0xA3,0xA4,0xA5,
        0xA6,0xA7,0xA8,0xA9,0xAA,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,
        0xB8,0xB9,0xBA,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,
        0xCA,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xE1,
        0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xF1,0xF2,
        0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,0xFF,0xC4,0x00,
        0x1F,0x01,0x00,0x03,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
        0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x03,
        0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0xFF,0xC4,0x00,
        0xB5,0x11,0x00,0x02,0x01,0x02,0x04,0x04,0x03,0x04,0x07,
        0x05,0x04,0x04,0x00,0x01,0x02,0x77,0x00,0x01,0x02,0x03,
        0x11,0x04,0x05,0x21,0x31,0x06,0x12,0x41,0x51,0x07,0x61,
        0x71,0x13,0x22,0x32,0x81,0x08,0x14,0x42,0x91,0xA1,0xB1,
        0xC1,0x09,0x23,0x33,0x52,0xF0,0x15,0x62,0x72,0xD1,0x0A,
        0x16,0x24,0x34,0xE1,0x25,0xF1,0x17,0x18,0x19,0x1A,0x26,
        0x27,0x28,0x29,0x2A,0x35,0x36,0x37,0x38,0x39,0x3A,0x43,
        0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x53,0x54,0x55,0x56,
        0x57,0x58,0x59,0x5A,0x63,0x64,0x65,0x66,0x67,0x68,0x69,
        0x6A,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0x82,0x83,
        0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x92,0x93,0x94,0x95,
        0x96,0x97,0x98,0x99,0x9A,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,
        0xA8,0xA9,0xAA,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,
        0xBA,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xD2,
        0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xE2,0xE3,0xE4,
        0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xF2,0xF3,0xF4,0xF5,0xF6,
        0xF7,0xF8,0xF9,0xFA,0xFF,0xDA,0x00,0x0C,0x03,0x01,0x00,
        0x02,0x11,0x03,0x11,0x00,0x3F,0x00,0xFB,0x2E,0x8A,0x28,
        0xA0,0x02,0x8A,0x28,0xA0,0x02,0x8A,0x28,0xA0,0x02,0x8A,
        0x28,0xA0,0x0E,0x33,0xFB,0x4B,0xE2,0x9F,0xFD,0x09,0xBE,
        0x0C,0xFF,0x00,0xC2,0xAE,0xE7,0xFF,0x00,0x95,0xF4,0x7F,
        0x69,0x7C,0x53,0xFF,0x00,0xA1,0x37,0xC1,0x9F,0xF8,0x55,
        0xDC,0xFF,0x00,0xF2,0xBE,0xBB,0x3A,0x28,0x03,0x8C,0xFE,
        0xD2,0xF8,0xA7,0xFF,0x00,0x42,0x6F,0x83,0x3F,0xF0,0xAB,
        0xB9,0xFF,0x00,0xE5,0x7D,0x1F,0xDA,0x5F,0x14,0xFF,0x00,
        0xE8,0x4D,0xF0,0x67,0xFE,0x15,0x77,0x3F,0xFC,0xAF,0xAE,
        0xCE,0x8A,0x00,0xE3,0x3F,0xB4,0xBE,0x29,0xFF,0x00,0xD0,
        0x9B,0xE0,0xCF,0xFC,0x2A,0xEE,0x7F,0xF9,0x5F,0x47,0xF6,
        0x97,0xC5,0x3F,0xFA,0x13,0x7C,0x19,0xFF,0x00,0x85,0x5D,
        0xCF,0xFF,0x00,0x2B,0xEB,0xB3,0xA2,0x80,0x38,0xCF,0xED,
        0x2F,0x8A,0x7F,0xF4,0x26,0xF8,0x33,0xFF,0x00,0x0A,0xBB,
        0x9F,0xFE,0x57,0xD1,0xFD,0xA5,0xF1,0x4F,0xFE,0x84,0xDF,
        0x06,0x7F,0xE1,0x57,0x73,0xFF,0x00,0xCA,0xFA,0xEC,0xE8,
        0xA0,0x02,0x8A,0x28,0xA0,0x02,0x8A,0x28,0xA0,0x02,0x8A,
        0x28,0xA0,0x02,0x8A,0x28,0xA0,0x0F,0xFF,0xD9 };

    std::ofstream out("blueline.jpg", std::ios::binary);
    out.write(reinterpret_cast<const char*>(image_data.data()), image_data.size());
    out.close();
}

int main()
{
    bool keyEvents = false;
    bool touchEvents = false;

    std::cout << "___________________Starting up___________________" << std::endl;

    
    graphiti = new GraphitiExtension();


    if(!graphiti->startUpVCP("COM4", keyEvents, touchEvents)){
        return 1;
    }

    ReadOuptputDemo();

    //UpdatePixelsDemo();

    GetStatusDemo();

    //KeyDemo();

    //Sleep time for setting draw mode
    // std::cout << "Waiting 20, Press dot 1 + Space for draw mode" << std::endl;
    // std::this_thread::sleep_for(std::chrono::seconds(20));

    //DrawDemo();

    //ImageDemo();

    //createImage();

    //createOnePixelImage();

    //std::this_thread::sleep_for(std::chrono::seconds(20));

    //while(true) {} //for testing

    std::cout << "__________________Shutting down__________________" << std::endl;

    graphiti->shutDownVCP(keyEvents, touchEvents);

    delete graphiti;
    return 0;
}