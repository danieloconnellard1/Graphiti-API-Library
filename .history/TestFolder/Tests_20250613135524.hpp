#pragma once
#include <cstdio>
#include <Graphiti/Extension.hpp>

class Tests {
public:
    Tests();

    GraphitiExtension* graphiti = nullptr;

    struct TestEntry {
        std::function<bool()> func;
        bool keyEvents;
        bool touchEvents;
    };

    std::unordered_map<std::string, TestEntry> testRegistry;

    void registerTests();

    bool startTesting(bool keyEvents, 
        bool touchEvents, 
        std::function<bool()> func);

    //String tests

    bool get_Software_Version_Correctness();

    bool get_Hardware_Version_Correctness();

    bool get_Serial_Number_Correctness();

    bool get_Battery_Status_Correctness();

    bool get_Resolution_Information_Correctness();

    bool get_Device_Orientation_Correctness();

    bool get_Height_Information_Correctness();

    bool get_Device_Name_Correctness();

    //Display Tests

    bool update_Display_Functionality();

    bool set_Display_Funcitonality();

    bool clear_Display_Functionality();

    bool update_Single_Pixel_Functionality();

    bool update_Single_Row_Functionality();

    bool update_Single_Column_Functionality();

    //Status Tests

    bool get_ALL_Pixels_Position_Status_Correctness();

    bool get_Single_Pixel_Position_Status_Correctness();

    bool get_Single_Row_Pixels_Position_Status_Correctness();

    bool get_Single_Column_Pixels_Position_Status_Correctness();

    //Key Test

    bool get_Next_KeyEvent_Functionality();

    void OnePixel_KeyFunction(
        const std::set<std::string>& keyEvent,
        void* parameters
    );

    //Draw Test

    bool get_Next_Draw_Event_Functionality();

    void PrintDraw_DrawFunction(
        const Graphiti_API::DrawEvent& drawEvent, 
        void* parameters
    );

    //Show Message Test

    bool show_Message_On_The_Device_Functionality();

    //Set Cursor Test

    bool set_Cursor_Functionality();

    //Image Test

    bool send_Image_Functionality();

private:
    
    bool loopCondition = true;

    bool sleepAndTest(std::string expected);

    /**
     * @brief Update Display
     * 
     * Calls update dislay with 2 height 0 blinkRate
     * 
     * @return std::vector<uint8_t> screen vector it was set to
     */
    std::vector<uint8_t> UpdateDisplay();

    /**
     * @brief Update Row
     * 
     * Calls updateSingleRow with 2 height and 0 blink rate
     * 
     * @return std::vector<uint8_t> row vector it was set to
     */
    std::vector<uint8_t> UpdateRow(uint8_t row);

    /**
     * @brief Update Column
     * 
     * Calls updateSingleColumn with 2 height and 0 blink rate
     * 
     * @return std::vector<uint8_t> column vector it was set to
     */
    std::vector<uint8_t> UpdateColumn(uint8_t column);

    std::vector<uint8_t> displayFunctionWrapper(
        std::string funcitonName, 
        int arg1, int arg2, int arg3, int arg4);

    void statusFunctionWrapper(
        std::string funcitonName, int arg1, int arg2);

    bool statusFunctionResponse(
        std::string displayFunctionName,
        std::string statusFunctionName,
        int arg1 = 0, int arg2 = 0, int arg3 = 0, int arg4 = 0
    );
};