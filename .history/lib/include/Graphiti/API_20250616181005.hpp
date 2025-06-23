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
     * @param connection connection object for reading device output
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
     */
    void startResponseThread();

    /**
     * @brief Stop Response Thread
     * 
     * Stops response loop that reads from Graphiti output 
     */
    void stopResponseThread();

    /**
     * @brief Get the Next Output Event object
     * 
     * @return std::optional<std::string> optional string of event
     */
    std::optional<std::string> getNextOutputEvent();

    /**
     * @brief Get the Next Display Status Event object
     * 
     * @return std::optional<std::vector<uint8_t>> optional event (nullopt when empty)
     */
    std::optional<std::vector<uint8_t>> getNextDisplayStatusEvent();

    /**
     * @brief Get the Next Key Event object
     * 
     * @return std::optional<std::set<std::string>> string option of key event 
     */
    std::optional<std::set<std::string>> getNextKeyEvent();

    /**
     * @brief Get the Next Gesture Event object
     * 
     * @return std::string gesture
     */
    std::optional<std::string> getNextGestureEvent();

    /**
     * @struct Pin Information
     * 
     * @brief Holds pin information
     */
    struct PinInfo {
        /**
         * @brief rowID - row ID 1 to 40
         * 
         */
        int rowID;

        /**
         * @brief columnID - column ID 1 to 60
         * 
         */
        int columnID;

        /**
         * @brief height - pin height 0 to 4
         * 
         */
        int height;

        /**
         * @brief blinkRate - blink rate 0 to 50 (5 seconds)
         * 
         */
        int blinkRate;
    };

    /**
     * @struct Draw Event
     * 
     * @brief Holds draw Event informaiton
     */
    struct DrawEvent {
        /**
         * @brief length - amount of pins
         * 
         */
        int length;

        /**
         * @brief pins - pins of PinInfo
         * 
         */
        std::vector<PinInfo> pins;
    };

    /**
     * @brief Get the Next Draw Event object
     * 
     * @return std::optional<DrawEvent> draw event option
     * 
     * null output when no event
     */
    std::optional<DrawEvent> getNextDrawEvent();

    /**
     * @brief 4.1 ACK request
     * 
     * You can send this command to the device to inform that your
     * received data is correct.
     */
    void sendACK();

    /**
     * @brief 4.1 NACK request
     * 
     * Whenever the checksum error is detected in the received data
     * at the host side, you can send NACK to the device so that the device will send
     * the same data again. The host application must send NACK command within
     * 300ms of the previous response, otherwise the device will not serve it.
     */
    void sendNACK();

    /**
     * @brief 4.2.1 Get Software Version
     * 
     * The 'Get Software Version' command retrieves the version of the current
     * firmware in the device.
     */
    void getSoftwareVersion();

    /**
     * @brief 4.2.2 Get Hardware Version
     * 
     * The 'Get Hardware Version' command retrieves the current version of the device
     * hardware.
     */
    void getHardWareVersion();

    /**
     * @brief 4.2.3 Get Unit Serial Number
     * 
     * The 'Get Unit Serial Number' command retrieves the serial number of the
     * connected device.
     */
    void getSerialNumber();

    /**
     * @brief 4.2.4 Get Battery Status
     * 
     * The 'Get Battery Status' command retrieves the current battery state of the
     * device including the charging status.
     */
    void getBatteryStatus();

    /**
     * @brief 4.2.5 Get Resolution Information
     * 
     * The 'Get Resolution Information' command retrieves the details of horizontal and
     * vertical resolution supported by the device.
     */
    void getResolutionInformation();

    /**
     * @brief 4.2.6 Get Device Orientation
     * 
     * The 'Get Device Orientation' command retrieves the current orientation of the
     * device. Presently the device supports landscape orientation only.
     */
    void getDeviceOrientation();

    /**
     * @brief 4.2.7 Get Height Information
     * 
     * The 'Get Height Information' command retrieves information about the number of
     * height levels supported by the device for all the pins.
     */
    void getHeightInformation();

    /**
     * @brief 4.2.8 Get Device Name
     * 
     * The 'Get Device Name' command is used to retrieve the name of the device.
     */
    void getDeviceName();

    /**
     * @brief 4.3 Display Access Commands
     * 
     * The response to all the commands in this category is received only after the 
     * display has been updated. Heights vary from 0x00 to 0x04. 
     * Blinkrates vary from 0 - No blinking, 1 - 100 ms to 50 - 5 seconds
     */

    /**
     * @brief 4.3.1 Update Display
     * 
     * The 'Update Display' command is used to configure the pins to the required 
     * height.
     * 
     * @param screen screen data of pixel heights and blinking rates
     */
    void updateDisplay(std::vector<uint8_t> screen);

    /**
     * @brief 4.3.2 Set Display
     * 
     * The 'Set Display' command is used to set or reset all the pins of the
     * display to the highest height position.
     */
    void setDisplay();

    /**
     * @brief 4.3.2 Clear Display
     * 
     * The 'Clear Display' command is used to set or reset all the pins of the
     * display to the lowest (in line with the surface) position.
     */
    void clearDisplay();

    /**
     * @brief 4.3.3 Update Single pixel on Display
     * 
     * The 'Update Single Pixel' command is used to configure a single pin to the
     * desired height position and blink rate interval.
     * 
     * @param row row ID 1 to 40 
     * @param column column ID 1 to 60
     * @param height height 0 to 4
     * @param blinkRate blink rate 0 to 50 (5 seconds)
     */
    void updateSinglePixel(uint8_t row, uint8_t column, uint8_t height, uint8_t blinkRate);

    /**
     * @brief 4.3.4 Update Single Row on Display
     * 
     * The 'Update Single Row' command is used to configure a single row to a desired
     * height position. To update the entire row, the pixel value and blinking rate for
     * each pixel in a row (60 pixels) needs to be provided. 
     * 
     * @param row row ID 1 to 40 
     * @param rowData 
     */
    void updateSingleRow(uint8_t row, std::vector<uint8_t> rowData);

    /**
     * @brief 4.3.5 Update Single Column on Display
     * 
     * The 'Update Single Column' command is used to configure a single column to a
     * desired height position. To update the entire column, the pixel value and blinking
     * rate for each pixel in a column (40 pixels) needs to be provided.
     * 
     * @param column column ID 1 to 60 
     * @param columnData column data with pixel value 0 to 4 height and blinking rate 0 to 50
     */
    void updateSingleColumn(uint8_t column, std::vector<uint8_t> columnData);

    /**
     * @brief 4.3.6 Get All Pixels' Position Status
     * 
     * The 'Get All Pixels Position Status' command is used to retrieve the present
     * status (position) of each pixel of the entire display (40x60) in a single response. 
     */
    void getAllPixelsPositionStatus();

    /**
     * @brief 4.3.7 Get Single Pixel Position Status
     * 
     * The 'Get Single Pixel Position Status' command is used to retrieve the present
     * status of the requested pixel.
     * 
     * @param row row ID 1 to 40 
     * @param column column ID 1 to 60 
     */
    void getSinglePixelPositionStatus(uint8_t row, uint8_t column);

    /**
     * @brief 4.3.8 Get Single Row Pixels Position Status
     * 
     * The 'Get Single Row Pixels Position Status' command is used to retrieve the
     * present status of each pixel in the requested row.
     * 
     * @param row row ID 1 to 40 
     */
    void getSingleRowPixelPositionStatus(uint8_t row);

    /**
     * @brief 4.3.9 Get Single Column Pixels Position Status
     * 
     * The 'Get Single Column Pixels Position Status' command is used to retrieve the
     * present status of each pixel in the requested column.
     * 
     * @param column column ID 1 to 60 
     */
    void getSingleColumnPixelPositionStatus(uint8_t column);

    /**
     * @brief 4.3.10  Show Message on the Device
     * 
     * @param message string message up to 20 characters
     * @param selectFlags int array of flags of 0, 1, 2 for nothing, underlining, 
     * and display cursor (up to 20 flags)
     * @param blinkRate blink rate 0 to 50 (5 seconds) 
     */
    void showMessage(std::string message, std::vector<uint8_t> selectFlags, uint8_t blinkRate);

    /**
     * @brief 4.3.11 Set Cursor
     * 
     * The 'Set Cursor' command is used to set the cursor on the display. Here, you
     * need to give the cursor position and size of the cursor.
     * 
     * @param row row ID 1 to 40
     * @param column column ID 1 to 60
     * @param height height 1 to 4
     * @param length length 1 to 60
     * @param blinkRate blinkRate 0 to 50 (5 seconds) 
     */
    void setCursor(uint8_t row, uint8_t column, uint8_t height, uint8_t length, uint8_t blinkRate);

    /**
     * @brief 4.3.12 Send Image (Interruptible)
     * 
     * This command is used to send and display an image file, such as a BMP, PNG,
     * or JPG file on the device. It is possible to terminate the data transfer of an image
     * in this API and send a new image. (Read Graphiti API Spec for more details)
     * 
     * @param name name of image including file extension 
     * @param filepath filepath to image(including name and file extension)
     */
    void sendImageInterruptible(std::string name, const std::string& filepath);

    /**
     * @brief 4.3.13 Send Image (Blocking)
     * 
     * This command is used to display the actual image such as a BMP, PNG, or JPG
     * file on the device. (Read Graphiti API Spec for more details).
     * 
     * @param name name of image including file extension
     * @param filepath filepath to image(including name and file extension)
     */
    void sendImageBlocking(std::string name, const std::string& filepath);

    /*4.4 User Keys Access Commands*/

    /**
     * @brief 4.4.1 Set Key Event
     * 
     * The 'Set Key Event' command is used to enable or disable the key press event.
     * When enabled, you will be able to get the information of each key press of the
     * device keypad. This information will include the key value and its event type. 
     * Note: Key cobinaitons that are reserved are: (5 + 6 + 8), (8 + Down), (7 + 8)
     * 
     * @param enabled true when enabled, false when disabled 
     */
    void setKeyEvent(bool enabled);

    /**
     * @brief 4.5.1 Set Touch Event
     * 
     * The 'Set Touch Event' command is used to enable or disable the touch event.
     * When this API is enabled, you will be able to get information of each touch event
     * (after enabling the respective mode: either gesture mode or draw mode) on the
     * touch panel. (See more info on Graphiti API Specification)
     * API for touch coordinates is supported in Draw mode only as of Version 0.22.
     * 
     * @param enabled true when enabled, false when disabled 
     */
    void setTouchEvent(bool enabled);

    /**
     * @brief 4.5.2 Get Last Touch Point Status
     * 
     * The 'Get Last Touch Point Status' is used to get the height of the pin which was
     * last touched.  To get last touch point status, you must first touch the pin and then
     * transmit command from the host. 
     */
    void getLastTouchPointStatus();
    
    /**
     * @brief 4.6 Vibrator Control Command
     * 
     * This command is used to drive vibrators available in the device with different
     * frequency, duty cycle and duration.
     * 
     * @param frequencyRange frequency 10 - 100 kHz
     * @param dutyCycle duty cycle 40 - 100 percent
     * @param duration duration 100 - 1000 ms
     */
    void vibratorControlCommand(int frequencyRange, int dutyCycle, int duration);

    /**
     * @brief 4.7.1 Get Date and Time
     * 
     * The 'Get Date and Time' command is used to get the date and time information
     * of the device. The device provides clock information in the 24-hour format. 
     */
    void getDateAndTime();

    /**
     * @brief 4.7.2 Set Date and Time
     * 
     * The 'Set Date and Time' command is used to set the device date and time in 24
     * hour format. The device will accept data in 24hr format.
     * 
     * @param day day byte
     * @param month month byte
     * @param year year byte
     * @param hour hour byte
     * @param minute minute byte
     * @param second second byte
     */
    void setDateAndTime(int day, int month, int year, int hour, int minute, int second);

private:

    /**
     * @brief conn_
     * 
     * Connection object
     */
    GraphitiConnection* conn_;

    /**
     * @brief Response Thread
     * 
     * Thread for listening to responses from device
     */
    std::thread responseThread;

    /**
     * @brief Running
     * 
     * Boolean condition for response loop
     */
    std::atomic<bool> running{ false };

    /**
     * @brief Run Response Loop
     * 
     * Pings the device in a loop
     */
    void runResponseLoop();

    /**
     * @brief Get the Response the Graphiti
     * 
     * Used in response loop to check for the start of fram byte or SOF.
     * Then checks the command byte and calls the corresponding command
     */
    void getResponse();

    /**
     * @brief Initialization of Maps
     * 
     * Initialization of Maps for constructors
     */
    void initMaps();

    /**
     * @brief Response Select Function
     * 
     * Mapping of command bytes to response functions
     */
    std::unordered_map<uint8_t, std::function<void()>> responseSelectFunc;

    /**
     * @brief Key Map
     * 
     * Mapping of int pair to string key name
     */
    std::map<std::pair<int, int>, std::string> keyMap;

    /**
     * @struct Response Info
     * 
     * @brief Type for mapping responses
     * 
     * funtion_name name of function.
     * event name of event for output.
     * length_bytes length in bytes of ascii response for decoding. 
     */
    struct ResponseInfo {
        std::string funtion_name;
        std::string event;
        int length_bytes;
    };

    /**
     * @brief Response Info Map
     * 
     * Maps Graphiti response to response data
     */
    std::unordered_map<uint8_t, ResponseInfo> responseInfoMap;

    /**
     * @brief Common Response Map
     * 
     * Maps start byte to common response
     */
    std::unordered_map<uint8_t, std::string> commonResponseMap;

    /**
     * @brief Gesture Map
     * 
     * Map of Gesure ID to Gesture Strings 
     */
    std::unordered_map<uint8_t, std::string> gestureMap;

    /**
     * @brief Key Event Queue
     * 
     * Queue of output key presses in bytes
     */
    ThreadSafeQueue<std::set<std::string>> keyEventQueue;

    /**
     * @brief Output Event Queue
     * 
     * Queue of output Strings
     */
    ThreadSafeQueue<std::string> outputEventQueue;

    /**
     * @brief Gesture Event Queue
     * 
     * Gesture events as "Double Tap", "Down Swipe", "Up Swipe", "Right Sipe",
     * "Left Swipe"
     */
    ThreadSafeQueue<std::string> gestureEventQueue;

    /**
     * @brief Draw Event Queue
     * 
     * Stores DrawEvents processed by readDrawEvent
     */
    ThreadSafeQueue<DrawEvent> drawEventQueue;

    /**
     * @brief Display Status Queue
     * 
     * Stores status events as std::vector<uint8_t> 
     */
    ThreadSafeQueue<std::vector<uint8_t>> displayStatusQueue;

    /**
     * @brief Reads bytes for types std::vector<uint8_t> 
     * 
     * Reads bytes and sets their values in data vector
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
     * getLastTouchPointStatus
     * 
     * @param data data place for storage
     * @param length length of input
     * @return true success
     * @return false failure
     */
    bool readBytes(std::vector<uint8_t> &data, int length);

    /**
     * @brief Read Status
     * 
     * Processes read status calls from graphiti
     * 
     * @param startByte startByte of read call
     * @param returnName name of read call
     * @param length length in bytes
     */
    void readStatus(uint8_t startByte, std::string returnName, int length);

    /**
     * @brief Set the Up Response Map object
     * 
     * Allows for mapping of different command bytes to different functions 
     * to handle the comannds. 
     */
    void setUpResponseMap();

    /**
     * @brief Set the Up Key Map object
     * 
     * Maps key values (decoded) to keys. First number is the digit number starting with zero.
     * Second number is the digit value.
     */
    void setUpKeyMap();

    /**
     * @brief Set the Up Gesture Map object
     * 
     * Maps gesture ID to gesture
     */
    void setUpGestureMap();

    /**
     * @brief Set the Up Common Response Map object
     * 
     * Maps start byte to common response
     */
    void setUpCommonResponseMap();

        /**
     * @brief 4.4.1 Read Key Event Status
     * 
     * Return of 'Set Key Event' command that is used to enable or disable the key press event.
     * 
     */
    void readKeyEventStatus();

    /**
     * @brief 4.4.1 Read Key Event 
     * 
     * Responses from enable 'Set Key Event' command.
     * Uses bit masks to get each digit then creates key event.
     * 
     */
    void readKeyEvent();

    /**
     * @brief 4.5.1 Read Touch Event Status
     * 
     * Return of 'Set Touch Event' command of enabled or disabled after update
     */
    void readTouchEventStatus();

    /**
     * @brief 4.5.1 Read Draw Event
     * 
     * Resonses from enabled 'Set Touch Event' command is used to enable or disable the touch event.
     */
    void readDrawEvent();

    /**
     * @brief 4.5.1 Read Gesture Event
     * 
     * Resonses from enabled 'Set Touch Event' command is used to enable or disable the touch event.
     */
    void readGestureEvent();

    /**
     * @brief 6.1 Common responses reader
     * 
     * Reads common responses and maps them to their output
     */
    void readCommonResponse();

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

    /**
     * @brief Read Command
     * 
     * Read command helper method that handles the startbyte, string, 
     * byte length of string message, and checksum calculation.
     * 
     * @param startByte command byte of command
     * @param name name of read function
     * @param event response start for queue
     * @param lengthBytes length of string in bytes
     */
    void readCommand(uint8_t startByte, std::string name,
     std::string event, int lengthBytes);

    /**
     * @brief Check End Byte
     * 
     * Checks the end byte against the checksum calculated from data.
     * True when correct. False when not
     * 
     * @param data 
     * @param startByte 
     * @return true 
     * @return false 
     */
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
     * @param bytes bytes of data to write to the graphiti
     */
    void Graphiti_IO_Write(std::vector<uint8_t> bytes);

    /**
     * @brief Start of Frame Check
     * 
     * Walks through the bytes checking for the SOF byte (0x1B) 
     * and repeating it when found. Also adds startbyte to beginning 
     * after handling the SOF
     * 
     * @param bytes bytes of data
     * @param startByte start byte for encoding
     * @return std::vector<uint8_t> bytes with startByte handled
     */
    std::vector<uint8_t> startOfFrameCheck(std::vector<uint8_t> bytes, uint8_t startByte);

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
     * @brief Print Key Event
     * Prints key events being called by getNextKeyEvent for debugging.
     * @param keyEvent set of key presses of poped key event
     */
    void printKeyEvent(std::set<std::string> keyEvent);

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
     * @param data data to print 
     */
    void printVectorHex(const std::vector<uint8_t>& data);
};