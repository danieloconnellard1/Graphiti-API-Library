#pragma once
#include <Graphiti/Connection/Connection.hpp>
#include <thread>
#include <atomic>
#include <string>
#include <vector>
#include <cstdint>
#include <iostream>
#include <functional>
#include <iomanip>
#include <optional>
#include <map>
#include <set>
#include <sstream>
#include <fstream>

#include "ThreadSafeQueue.hpp"

#define MAX_ROW_DATA 60
#define MAX_COLUMN_DATA 40

#define ROW_COUNT 40
#define COLUMN_COUNT 60

class Graphiti_API {
public:
    /**
     * @brief Construct a new Graphiti_API object
     * 
     */
    Graphiti_API();

    /**
     * @brief Construct a new Graphiti_API object with connection
     * 
     * @param connection 
     */
    Graphiti_API(GraphitiConnection* connection);

    /**
     * @brief Destroy the Graphiti_API object
     * 
     */
    ~Graphiti_API();

    /**
     * @brief Set Connection
     * 
     * Set the Connection object for the Graphiti
     * 
     * Used in conjunction with constructor that does not set the connection object or otherwise
     * 
     * @param connection 
     */
    void setConnection(GraphitiConnection* connection);

    /**
     * @brief Start Response Thread
     * 
     * Starts response loop to read from Graphiti output
     * 
     */
    void startResponseThread();

    /**
     * @brief Stop Response Thread
     * 
     * Stops response loop that reads from Graphiti output
     * 
     */
    void stopResponseThread();

    /**
     * @brief Run Response Loop
     * 
     * Pings the device in a loop
     */
    void runResponseLoop();

    /**
     * @brief Get the Next Output Event object
     * 
     * @return std::optional<std::string> optional string of event
     */
    std::optional<std::string> getNextOutputEvent();

    /*4.2.1 Read Software Version
    Return of 'Get Software Version' command that has the version of the current
    firmware in the device.*/
    void readSoftwareVersion();

    /*4.2.2 Read Hardware Version
    Return of 'Get Hardware Version' command that has the current version of the device
    hardware.*/
    void readHardwareVersion();

    /*4.2.3 Read Unit Serial Number
    Return of 'Read Unit Serial Number' command that has the serial number of the
    connected device.*/
    void readSerialNumber();

    /*4.2.4 Read Battery Status
    Return of 'Get Battery Status' command that has the current battery state of the
    device including the charging status.*/
    void readBatteryStatus();

    /*4.2.5 Read Resolution Information
    Return of 'Get Resolution Information' command that has the details of horizontal and
    vertical resolution supported by the device.*/
    void readResolution();

    /*4.2.6 Read Device Orientation
    Return of 'Get Device Orientation' command that has the current orientation of the
    device. Presently the device supports landscape orientation only.*/
    void readOrientation();

    /*4.2.7 Read Height Information
    Return of 'Get Height Information' command that has information about the number of
    height levels supported by the device for all the pins.*/
    void readHeight();

    /*4.2.8 Read Device Name
    Return of 'Get Device Name' command that has the name of the device.*/
    void readDeviceName();

    std::optional<std::vector<uint8_t>> getNextDisplayStatusEvent();

    /*4.4.1 Read Key Event Status
    Return of 'Set Key Event' command that is used to enable or disable the key press event.
    When enabled, you will be able to get the information of each key press of the
    device keypad. This information will include the key value and its event type. */
    void readKeyEventStatus();

    /* 4.4.1 Read Key Event 
    Responses from enable 'Set Key Event' command*/
    void readKeyEvent();

    /*Get Next Key Event
    Gets the next key event from the key event queue*/
    std::optional<std::set<std::string>> getNextKeyEvent();

    /**
     * @brief 4.5.1 Read Touch Event Status
     * 
     * Return of 'Set Touch Event' command of enabled or disabled after update
     */
    void readTouchEventStatus();

    /**
     * @brief 4.5.1 Read Gesture Event
     * 
     * Resonses from enabled 'Set Touch Event' command is used to enable or disable the touch event.
     */
    void readGestureEvent();

    /**
     * @brief Get the Next Gesture Event object
     * 
     * @return std::string gesture
     */
    std::optional<std::string> getNextGestureEvent();

    struct PinInfo {
        int rowID;
        int columnID;
        int height;
        int blinkRate;
    };

    struct DrawEvent {
        int length;
        std::vector<PinInfo> pins;
    };

    /**
     * @brief 4.5.1 Read Draw Event
     * 
     * Resonses from enabled 'Set Touch Event' command is used to enable or disable the touch event.
     */
    void readDrawEvent();

    /**
     * @brief Get the Next Draw Event object
     * 
     * @return std::optional<DrawEvent> draw event option
     * 
     * null output when no event
     */
    std::optional<DrawEvent> getNextDrawEvent();

    /**
     * @brief 4.5.2 Read Last Touch Point Status
     * 
     * Return of ‘Get Last Touch Point Status’ that has the height of the pin which was 
     * last touched.  To get last touch point status, you must first touch the pin and then 
     * transmit command from the host.
     * 
     * Note: This API will work in touch review mode only. 
     */
    void readLastTouchPointStatus();

    /**
     * @brief 4.7.1 Get Date and Time
     * 
     * Return of ‘Get Date and Time’ command that has the date and time information 
     * of the device. The device provides clock information in the 24-hour format.
     */
    void readDateAndTime();


    /*6.1 Common responses reader */
    void readCommonResponse();

    /*ACK command:
    You can send this command to the device to inform that your
    received data is correct.*/
    void sendACK();

    /*NACK command:
    Whenever the checksum error is detected in the received data
    at the host side, you can send NACK to the device so that the device will send
    the same data again. The host application must send NACK command within
    300ms of the previous response, otherwise the device will not serve it.*/
    void sendNACK();

    /*4.2.1 Get Software Version
    The 'Get Software Version' command retrieves the version of the current
    firmware in the device.*/
    void getSoftwareVersion();

    /*4.2.2 Get Hardware Version
    The 'Get Hardware Version' command retrieves the current version of the device
    hardware.*/
    void getHardWareVersion();

    /*4.2.3 Get Unit Serial Number
    The 'Get Unit Serial Number' command retrieves the serial number of the
    connected device.*/
    void getSerialNumber();

    /*4.2.4 Get Battery Status
    The 'Get Battery Status' command retrieves the current battery state of the
    device including the charging status.*/
    void getBatteryStatus();

    /*4.2.5 Get Resolution Information
    The 'Get Resolution Information' command retrieves the details of horizontal and
    vertical resolution supported by the device.*/
    void getResolutionInformation();

    /*4.2.6 Get Device Orientation
    The 'Get Device Orientation' command retrieves the current orientation of the
    device. Presently the device supports landscape orientation only.*/
    void getDeviceOrientation();

    /*4.2.7 Get Height Information
    The 'Get Height Information' command retrieves information about the number of
    height levels supported by the device for all the pins.*/
    void getHeightInformation();

    /*4.2.8 Get Device Name
    The 'Get Device Name' command is used to retrieve the name of the device.*/
    void getDeviceName();

    /* 4.3 Display Access Commands 
    The response to all the commands in this category is received only after the 
    display has been updated. Heights vary from 0x00 to 0x04. 
    Blinkrates vary from 0 - No blinking, 1 - 100 ms to 50 - 5 seconds */

    /*4.3.1 Update Display 
    The 'Update Display' command is used to configure the pins to the required 
    height.*/
    void updateDisplay(std::vector<uint8_t> screen);

    /*4.3.2 Set Display
    The 'Set Display' command is used to set or reset all the pins of the
    display to the highest height position.*/
    void setDisplay();

    /*4.3.2 Clear Display
    The 'Clear Display' command is used to set or reset all the pins of the
    display to the lowest (in line with the surface) position.*/
    void clearDisplay();

    /*4.3.3 Update Single pixel on Display
    The 'Update Single Pixel' command is used to configure a single pin to the
    desired height position and blink rate interval.*/
    void updateSinglePixel(uint8_t row, uint8_t column, uint8_t height, uint8_t blinkRate);

    /*4.3.4 Update Single Row on Display
    The 'Update Single Row' command is used to configure a single row to a desired
    height position. To update the entire row, the pixel value and blinking rate for
    each pixel in a row (60 pixels) needs to be provided. */
    void updateSingleRow(uint8_t row, std::vector<uint8_t> rowData);
    
    /*4.3.5 Update Single Column on Display
    The 'Update Single Column' command is used to configure a single column to a
    desired height position. To update the entire column, the pixel value and blinking
    rate for each pixel in a column (40 pixels) needs to be provided.*/
    void updateSingleColumn(uint8_t column, std::vector<uint8_t> columnData);
    
    /*4.3.6 Get All Pixels' Position Status
    The 'Get All Pixels Position Status' command is used to retrieve the present
    status (position) of each pixel of the entire display (40x60) in a single response.*/
    void getAllPixelsPositionStatus();

    /*4.3.7 Get Single Pixel Position Status
    The 'Get Single Pixel Position Status' command is used to retrieve the present
    status of the requested pixel.*/
    void getSinglePixelPositionStatus(uint8_t row, uint8_t column);

    /*4.3.8 Get Single Row Pixels Position Status
    The 'Get Single Row Pixels Position Status' command is used to retrieve the
    present status of each pixel in the requested row.*/
    void getSingleRowPixelPositionStatus(uint8_t row);

    /*4.3.9 Get Single Column Pixels Position Status
    The 'Get Single Column Pixels Position Status' command is used to retrieve the
    present status of each pixel in the requested column.*/
    void getSingleColumnPixelPositionStatus(uint8_t column);

    /*4.3.10  Show Message on the Device
    The 'Show Message' command is used to show the text message on the display.  */
    void showMessage(std::string message, uint8_t blinkRate);

    /*4.3.11 Set Cursor
    The 'Set Cursor' command is used to set the cursor on the display. Here, you
    need to give the cursor position and size of the cursor.*/
    void setCursor(uint8_t row, uint8_t column, uint8_t height, uint8_t length, uint8_t blinkRate);

    /*4.3.12 Send Image (Interruptible)
    This command is used to send and display an image file, such as a BMP, PNG,
    or JPG file on the device. It is possible to terminate the data transfer of an image
    in this API and send a new image. (Read Graphiti API Spec for more details)*/
    void sendImageInterruptible(std::string name, const std::string& filepath);

    /*4.3.13 Send Image (Blocking)
    This command is used to display the actual image such as a BMP, PNG, or JPG
    file on the device. (Read Graphiti API Spec for more details)*/
    void sendImageBlocking(std::string name, const std::string& filepath);

    /*4.4 User Keys Access Commands*/

    /*4.4.1 Set Key Event
    The 'Set Key Event' command is used to enable or disable the key press event.
    When enabled, you will be able to get the information of each key press of the
    device keypad. This information will include the key value and its event type. 
    Note: Key cobinaitons that are reserved are: (5 + 6 + 8), (8 + Down), (7 + 8)*/
    void setKeyEvent(bool enabled);

    /*4.5.1 Set Touch Event
    The 'Set Touch Event' command is used to enable or disable the touch event.
    When this API is enabled, you will be able to get information of each touch event
    (after enabling the respective mode: either gesture mode or draw mode) on the
    touch panel. (See more info on Graphiti API Specification)
    API for touch coordinates is supported in Draw mode only as of Version 0.22*/
    void setTouchEvent(bool enabled);

    /*4.5.2 Get Last Touch Point Status
    The 'Get Last Touch Point Status' is used to get the height of the pin which was
    last touched.  To get last touch point status, you must first touch the pin and then
    transmit command from the host.*/
    void getLastTouchPointStatus();

    /*4.6 Vibrator Control Command
    This command is used to drive vibrators available in the device with different
    frequency, duty cycle and duration. */
    void vibratorControlCommand(uint8_t frequencyRange, uint8_t dutyCycle, uint16_t duriation);

    /*4.7.1 Get Date and Time
    The 'Get Date and Time' command is used to get the date and time information
    of the device. The device provides clock information in the 24-hour format.*/
    void getDateAndTime();

    /*4.7.2 Set Date and Time
    The 'Set Date and Time' command is used to set the device date and time in 24
    hour format. The device will accept data in 24hr format.*/
    void getDateAndTime(uint8_t day, uint8_t month, uint16_t year);

    /**
     * @brief Print Key Event
     * Prints key events being called by getNextKeyEvent for debugging.
     * @param keyEvent set of key presses of poped key event
     */
    void printKeyEvent(std::set<std::string> keyEvent);

private:
    GraphitiConnection* conn_;
    std::thread responseThread;
    std::atomic<bool> running{ false };

    /**
     * @brief Initialization of Maps
     * 
     * Initialization of Maps for constructors
     */
    void initMaps();

    std::unordered_map<uint8_t, std::function<void()>> responseSelectFunc;

    std::map<std::pair<int, int>, std::string> keyMap;

    /**
     * @brief Response Info type for mapping responses
     * funtion_name: name of function
     * event: name of event for output
     * length_bytes: length in bytes of ascii response for decoding
     */
    struct ResponseInfo {
        std::string funtion_name;
        std::string event;
        int length_bytes;
    };

    /**
     * @brief Response Info Map
     * Maps Graphiti response to response data
     */
    std::unordered_map<uint8_t, ResponseInfo> responseInfoMap;

    /**
     * @brief Common Response Map
     * Maps start byte to common response
     */
    std::unordered_map<uint8_t, std::string> commonResponseMap;

    /**
     * @brief Map of Gesure ID to Gesture Strings
     * 
     */
    std::unordered_map<uint8_t, std::string> gestureMap;

    /*Queue of output key presses in bytes*/
    ThreadSafeQueue<std::set<std::string>> keyEventQueue;

    /*Queue of output Strings*/
    ThreadSafeQueue<std::string> outputEventQueue;

    /**
     * @brief Gesture Event Queue
     * 
     * Gesture events as "Double Tap", "Down Swipe", "Up Swipe", "Right Sipe",
     * "Left Swipe"
     */
    ThreadSafeQueue<std::string> gestureEventQueue;

    ThreadSafeQueue<DrawEvent> drawEventQueue;

    ThreadSafeQueue<std::vector<uint8_t>> displayStatusQueue;

    /**
     * @brief Reads bytes for types std::vector<uint8_t> 
     * 
     * For funtion returns of:
     * 
     * getAllPixelsPositionStatus
     * 
     * getSinglePixelPositionStatus
     * 
     * getSingleRowPixelsPositionStatus
     * 
     * getSingleColumnPixelsPositionStatus
     * 
     * @param data data place for storage
     * @param length length of input
     * @return true 
     * @return false 
     */
    bool readBytes(std::vector<uint8_t> &data, int length);

    void readStatus(uint8_t startByte, std::string returnName, int length);

    /* Setup Response Map
    Allows for mapping of different command bytes to different functions 
    to handle the comannds*/
    void setUpResponseMap();

    /**
     * @brief Set the Up Key Map object
     * Digit and number mapping to key string
     */
    void setUpKeyMap();

    /**
     * @brief Set the Up Gesture Map object
     * 
     */
    void setUpGestureMap();

    /**
     * @brief Set the Up Common Response Map object
     * Maps start byte to common response
     */
    void setUpCommonResponseMap();

    /**
     * @brief Get the Response the Graphiti
     * 
     * Used in response loop to check for the start of fram byte or SOF.
     * Then checks the command byte and calls the corresponding command
     */
    void getResponse();

    /**
     * @brief Bytes to String
     * 
     * Converts bytes from connection to string for given length 
     * and returns string made as an option.
     * 
     * @param data bytes of data
     * @param length length in bytes
     * @return std::optional<std::string> string convertion of bytes
     */
    std::optional<std::string> bytesToString(std::vector<uint8_t> &data, int length);

    /*Read Command
    Read command helper method that handles the startbyte, string, 
    byte length of string message, and checksum calculation*/

    /**
     * @brief Read Command
     * 
     * Read command helper method that handles the startbyte, string, 
     * 
     * byte length of string message, and checksum calculation.
     * 
     * @param startByte command byte of command
     * @param name name of read function
     * @param event response start for queue
     * @param lengthBytes length of string in bytes
     */
    void readCommand(uint8_t startByte, std::string name,
     std::string event, int lengthBytes);

    /*Check End Byte
    Checks the end byte against the checksum calculated from data.
    True when correct. False when not*/
    bool checkEndByte(std::vector<uint8_t> data, uint8_t startByte);

    /**
     * @brief Calculate Checksum
     * 
     * Calculates the Checksum from the data
     * 
     * @param data data encoding for sending to Graphiti
     * @return std::vector<uint8_t> of the checksum
     */
    std::vector<uint8_t> calcChecksum(const std::vector<uint8_t>& data);


    /**
     * @brief Graphiti Input/Output Write
     * 
     * @param bytes data to write to the graphiti
     * @param sofCheck 
     */
    void Graphiti_IO_Write(std::vector<uint8_t> bytes);

    std::vector<uint8_t> startOfFrameCheck(std::vector<uint8_t> bytes, uint8_t startByte = 0x00);

    /**
     * @brief Send Image
     * 
     * helper methods for the sendImage API calls
     * 
     * @param commandByte command byte of call
     * @param name name of image
     * @param filepath path to image
     */
    void sendImage(uint8_t commandByte, std::string name, const std::string& filepath);

    /**
     * @brief Debug Byte
     * 
     * Outputs byte during debug mode
     * 
     * @param byte byte
     */
    void debugByte(uint8_t byte);

    /**
     * @brief Print Vector Hex
     * 
     * Prints vector for debugging
     * 
     * @param vec 
     */
    void printVectorHex(const std::vector<uint8_t>& vec);
};