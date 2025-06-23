#include <cstdio>

#include <Graphiti/API.hpp>

/**
 * @brief Construct a new Graphiti_API::Graphiti_API object
 * Without connection
 */
Graphiti_API::Graphiti_API(){
    initMaps();
}

/**
 * @brief Construct a new Graphiti_API::Graphiti_API object
 * @details Sets up maps for use in Graphiti_API
 * 
 * @param connection connection for GraphitiConnection read calls
 */
Graphiti_API::Graphiti_API(GraphitiConnection* connection)
    : conn_(connection) {
    initMaps();
}

/**
 * @brief Destroy the Graphiti_API::Graphiti_API object
 * @details Stops thread responses
 */
Graphiti_API::~Graphiti_API() {
    stopResponseThread();
}

void Graphiti_API::initMaps(){
    setUpResponseMap();
    setUpKeyMap();
    setUpGestureMap();
    setUpCommonResponseMap();
}

void Graphiti_API::setConnection(GraphitiConnection* connection){
    conn_ = connection;
}

void Graphiti_API::startResponseThread() {
    if (!running.load()) {
        running.store(true);
        try {
            responseThread = std::thread(&Graphiti_API::runResponseLoop, this);
        }
        catch (const std::exception& e) {
            std::cerr << "Error starting thread: " << e.what() << "\n";
        }
    }
}

void Graphiti_API::runResponseLoop() {
    while (running.load()) {
        try {
            getResponse();
        } catch (const std::exception& e) {
            std::cerr << "Exception in response loop: " << e.what() << "\n";
        } catch (...) {
            std::cerr << "Unknown exception in response loop.\n";
        }
    }
}

void Graphiti_API::stopResponseThread() {
    running.store(false);
    if (responseThread.joinable()) {
        responseThread.join();
    }
}

void Graphiti_API::getResponse() {
    auto res = conn_->read(1);
    if (res.empty()) {
        //No response
        std::cerr << "Response Empty" << std::endl;
        return;
    }
    debugByte(res[0]);
    
    if (res[0] != 0x1B) {
        std::cerr << "Missing start-of-frame marker" << std::endl;
        return;
    }
    auto cmd = conn_->read(1);
    if (cmd.empty()) {
        std::cerr << "command id byte not recieved" << std::endl;
        return;
    }
    debugByte(cmd[0]);
    // Map command with response map
    auto it = responseSelectFunc.find(cmd[0]);
    if (it != responseSelectFunc.end()) {
        it->second();
        #ifdef DEBUG
            std::cerr << "Response selected" << std::endl;
        #endif
    }
}

void Graphiti_API::setUpResponseMap() {
    responseSelectFunc = {
        {0x01, [this]() { readCommand(0x01, "readSoftwareVersion", "Software Version: ", 15); }},
        {0x02, [this]() { readCommand(0x02, "readHardwareVersion", "Hardware Version: ", 15); }},
        {0x04, [this]() { readCommand(0x04, "readSerialNumber", "Serial Number: ", 15); }},
        {0x05, [this]() { readCommand(0x05, "readBatterStatus", "Battery Status: ", 15); }},
        {0x07, [this]() { readCommand(0x07, "readResolution", "Resolution: ", 5); }},
        {0x08, [this]() { readCommand(0x08, "readOrientation", "Orientation: ", 10); }},
        {0x09, [this]() { readCommand(0x09, "readHeight", "Height: ", 1); }},
        {0x0A, [this]() { readCommand(0x0A, "readDevicename", "Name: ", 30); }},
        {0x20, [this]() { readStatus(0x20, "readAllPixelsPositionStatus", 2400); }},
        {0x21, [this]() { readStatus(0x21, "readSinglePixelPositionStatus", 3); }},
        {0x22, [this]() { readStatus(0x22, "readSingleRowPixelsPositionStatus", 61); }},
        {0x23, [this]() { readStatus(0x23, "readSingleColumnPixelsPositionStatus", 41); }},
        {0x31, [this]() { readKeyEventStatus(); }},
        {0x32, [this]() { readKeyEvent(); }},
        {0x41, [this]() { readTouchEventStatus(); }},
        {0x42, [this]() { readGestureEvent(); }},
        {0x43, [this]() { readDrawEvent(); }},
        {0x44, [this]() { readStatus(0x44, "readLastTouchPointStatus", 3); }},
        {0x53, [this]() { readCommonResponse(); }},
        {0x65, [this]() { readCommand(0x65, "readDateAndTime", "Date & Time: ", 19); }}
    };
}

std::optional<std::string> Graphiti_API::bytesToString(std::vector<uint8_t> &data, int length) {
    std::string outputString;

    for (int i = 0; i < length; i++) {
        auto byte = conn_->read(1);
        if (!byte.empty()) {
            data[i] = byte[0];
            if(byte[0] != 0x00) {
                outputString += static_cast<char>(byte[0]);
            }
        }
        else {
            // Error Case
            std::cerr << "bytesToString timed out" << std::endl;
            data[i] = 0;
            return std::nullopt;
        }
    }

    return outputString;
}

bool Graphiti_API::checkEndByte(std::vector<uint8_t> data, uint8_t startByte) {
    data.insert(data.begin(), startByte);
    auto checksum = calcChecksum(data);
    
    auto byte = conn_->read(1);

    bool passed = false;

    if (!byte.empty()) {
        if(byte[0] == 0x1B && byte[0] == checksum[0]){
            byte = conn_->read(1);
            passed = byte[0] == checksum[1];
        } else {
            passed = byte[0] == checksum[0];
        }
    }

    if(passed) {
        sendACK();
    } else {
        //printVectorHex(data);
        sendNACK();
    }

    return passed;
}

void Graphiti_API::readCommand(uint8_t startByte, std::string funtion_name,
     std::string event, int length_bytes){
    std::vector<uint8_t> data(length_bytes, 0);

    std::string stringOutput;

    auto stringOutputOption = bytesToString(data, length_bytes);

    if(stringOutputOption.has_value()) {
        stringOutput = stringOutputOption.value();
    }

    if(!checkEndByte(data, startByte)){
        std::cerr << funtion_name << " checksum failed" << std::endl;
        return;
    }

    #ifdef DEBUG
        std::cerr << event << stringOutput << "\n" << std::endl;
    #endif

    outputEventQueue.push(stringOutput);
}

std::optional<std::string> Graphiti_API::getNextOutputEvent() {
    std::string output;
    //Pop ooutput from Output Event Queue
    if (outputEventQueue.pop(output)) {
        return output;
    }
    // Queue is empty
    return std::nullopt;
}

bool Graphiti_API::readBytes(std::vector<uint8_t> &data, int length) {
    for (int i = 0; i < length; i++) {
        auto byte = conn_->read(1);
        if (!byte.empty()) {
            data[i] = byte[0];
        }
        else {
            // Error Case
            std::cerr << "readBytes timed out" << std::endl;
            data[i] = 0;
            return false;
        }
    }

    return true;
}

void Graphiti_API::readStatus(uint8_t startByte, std::string returnName, int length) {
    std::vector<uint8_t> data(length, 0);

    if(!readBytes(data, length)){
        std::cerr << returnName << " timed out" << std::endl;
    }

    if(!checkEndByte(data, startByte)){
        std::cerr << returnName << " checksum failed" << std::endl;
        return;
    }

    //Add to Dispaly Status Queue
    displayStatusQueue.push(data);
}

std::optional<std::vector<uint8_t>> Graphiti_API::getNextDisplayStatusEvent() {
    std::vector<uint8_t> display;
    //Pop ooutput from Output Event Queue
    if (displayStatusQueue.pop(display)) {
        return display;
    }
    // Queue is empty
    return std::nullopt;
}

void Graphiti_API::readKeyEventStatus() {
    #ifdef DEBUG
        std::cerr << "readKeyEventStatus called" << std::endl;
    #endif

    std::vector<uint8_t> data(1, 0);
    
    auto bytes = conn_->read(1);
    if (!bytes.empty()) {
        data[0] = bytes[0];
    } else {
        // Error Case
        std::cerr << "readKeyEventStatus timed out" << std::endl;
        data[0] = 0;
    }

    if(!checkEndByte(data, 0x31)){
        std::cerr << "readKeyEventStatus" << " checksum failed" << std::endl;
        return;
    }

    std::cerr << "Key Event Status: "<< (data[0] == 0x01 ? "enabled": "disabled") << std::endl;
}

void Graphiti_API::setUpKeyMap(){
    keyMap = {
        {{0, 1}, "1"},
        {{0, 2}, "2"},
        {{0, 4}, "3"},
        {{0, 8}, "7"},
        {{1, 1}, "4"},
        {{1, 2}, "5"},
        {{1, 4}, "6"},
        {{1, 8}, "8"},
        {{2, 1}, "Up"},
        {{2, 2}, "Left"},
        {{2, 4}, "Down"},
        {{2, 8}, "Right"},
        {{3, 1}, "Select"},
        {{3, 2}, "Space"}
    };
}

void Graphiti_API::readKeyEvent() {
    std::vector<uint8_t> data(3, 0);
    int digits[4];
    int digitIndex = 0;

    for (int i = 0; i < 3; i++) {
        auto bytes = conn_->read(1);
        if (!bytes.empty()) {
            data[i] = bytes[0];
            if(digitIndex < 4) {
                digits[digitIndex] = (bytes[0] >> 4) & 0x0F;
                digitIndex++;
                digits[digitIndex] = bytes[0] & 0x0F;
                digitIndex++;
            }
        } else {
            // Error Case
            std::cerr << "readKeyEvent timed out" << std::endl;
            data[i] = 0;
            return;
        }
    }

    if(!checkEndByte(data, 0x32)){
        std::cerr << "readKeyEvent" << " checksum failed" << std::endl;
        return;
    }
    
    /*
    Key Event
    List of key presses. Keys pressed at same time will be in the same list.
    Dots 1-8 are 1-8 and special keys Up, Left, Down, Right, Select, Space
    are in order following 1-8 all as a list of strings
    */
    std::set<std::string> keyEvent;

    /*
    Iterate over the digits adding the key presses to the keyList
    Using modulus operator for safe subtraction so no negative numbers possible.
    i is the digit number, divisor is the digit corresponding to the key
    */
    for (int i = 0; i < 4; i++)
    {
        int digit = digits[i];
        int divisor = 8;
        while(digit > 0) {
            if(digit >= divisor) {
                digit = digit % divisor;
                keyEvent.insert(keyMap[{i,divisor}]);
            }
            divisor = divisor / 2;
        }
    }

    //Add keyList to queue
    keyEventQueue.push(keyEvent);
}

std::optional<std::set<std::string>> Graphiti_API::getNextKeyEvent() {
    std::set<std::string> keyEvent;
    //Pop key from Key Event Queue
    if (keyEventQueue.pop(keyEvent)) {
        #ifdef DEBUG
            printKeyEvent(keyEvent);
        #endif
        return keyEvent;
    }
    // Queue is empty
    return std::nullopt;
}

void Graphiti_API::printKeyEvent(std::set<std::string> keyEvent){
    bool first = true;
    std::ostringstream keyEventString;
    for (const auto& s : keyEvent) {
        if (!first) keyEventString << " ";
        keyEventString << s;
        first = false;
    }
    std::cerr << "Key(s): " << keyEventString.str() << "\n";
}

void Graphiti_API::readTouchEventStatus() {
    #ifdef DEBUG
        std::cerr << "readTouchEventStatus called" << std::endl;
    #endif

    std::vector<uint8_t> data(1, 0);
    
    auto bytes = conn_->read(1);
    if (!bytes.empty()) {
        data[0] = bytes[0];
    } else {
        // Error Case
        std::cerr << "readTouchEventStatus timed out" << std::endl;
        data[0] = 0;
    }

    if(!checkEndByte(data, 0x41)){
        std::cerr << "readTouchEventStatus" << " checksum failed" << std::endl;
        return;
    }

    std::cerr << "Touch Event Status: "<< (data[0] == 0x01 ? "enabled" : "disabled") << std::endl;
}

void Graphiti_API::setUpGestureMap(){
    gestureMap = {
        {0x02, "Double Tap"},
        {0x03, "Down Swipe"},
        {0x04, "Up Swipe"},
        {0x05, "Right Swipe"},
        {0x06, "Left Swipe"}
    };
}

void Graphiti_API::readGestureEvent() {
    std::vector<uint8_t> data(1, 0);
    
    auto bytes = conn_->read(1);
    if (!bytes.empty()) {
        data[0] = bytes[0];
    } else {
        // Error Case
        std::cerr << "readGestureEvent timed out" << std::endl;
        data[0] = 0;
        return;
    }

    if(!checkEndByte(data, 0x42)){
        std::cerr << "readGestureEvent" << " checksum failed" << std::endl;
        return;
    }

    gestureEventQueue.push(gestureMap[data[0]]);
}

std::optional<std::string> Graphiti_API::getNextGestureEvent() {
    std::string gesture;
    //Pop ooutput from Output Event Queue
    if (gestureEventQueue.pop(gesture)) {
        return gesture;
    }
    // Queue is empty
    return std::nullopt;
}

void Graphiti_API::readDrawEvent() {
    uint8_t length;

    #ifdef DEBUG
        std::cout << "readDrawEvent" << std::endl;
    #endif 

    auto byte = conn_->read(1);
    if (!byte.empty()) {
        length = byte[0];
    } else {
        // Error Case
        std::cerr << "readDrawEvent timed out" << std::endl;
        length = 0;
    }

    DrawEvent drawEvent;

    std::cerr << "Length byte: " << std::endl;
    debugByte(length);

    //Amt of pins(generally and can be less cause of multiple SOF(27) bytes as data )
    int pinDataAmt = static_cast<int>(length) / 4;

    drawEvent.length = pinDataAmt;
    std::cerr << "pinDataAmt " << pinDataAmt << std::endl;
    drawEvent.pins.resize(pinDataAmt);


    //Counter for total reads for resize after
    int pinReadCounter = 0;

    //Adding from beginning due to length byte doubling
    std::vector<uint8_t> data(length + 1, 0);
    data[0] = length;

    //Index of reading input from connection
    int dataIndex = 1;

    //Handle start byte doubling
    if(length == 0x1B) {
        data.insert(data.begin(), 0x1B);
        dataIndex++;
    } else if (length == 0x00)
    {
        if(!checkEndByte({length}, 0x43)) {
            std::cerr << "readDrawEvent clear" << " checksum failed" << std::endl;
            return;
        }
        return;
    }

    //Counter to put data to correct spaces
    int pinDataCounter = 0;

    //Pin Counter for saving pin data after every 4 reads
    int pinCounter = 0;

    //Data holder for every set of data
    std::vector<uint8_t> pinData(4, 0);

    //Using one for loop for handling efficiency due to 0x1B doubling problems
    for (size_t i = 0; i < length; i++){
        auto readOne = conn_->read(1);
        if (!readOne.empty()) {
            uint8_t readByte = readOne[0];
            debugByte(readByte);

            //std::cerr << "pinDataCounter " << pinDataCounter << std::endl;
            pinData[pinDataCounter] = readByte;
            //std::cerr << "pinData[pinDataCounter] " << static_cast<int>(pinData[pinDataCounter]) << std::endl;
            pinDataCounter++;

            if(pinDataCounter > 3) {
                drawEvent.pins[pinCounter].rowID = static_cast<int>(pinData[0]);
                drawEvent.pins[pinCounter].columnID = static_cast<int>(pinData[1]);
                drawEvent.pins[pinCounter].height = static_cast<int>(pinData[2]);
                drawEvent.pins[pinCounter].blinkRate = static_cast<int>(pinData[3]);
                pinDataCounter = 0;
                pinCounter++;
            }

            data[dataIndex] = readByte;
            dataIndex++;

            if(readByte == 0x1B){
                auto byte = conn_->read(1);
                data[dataIndex] = 0x1B;
                dataIndex++;
                i++;
            }
        } else {
            // Error Case
            std::cerr << "readDrawEvent timed out" << std::endl;
            return;
        }
    }

    drawEvent.pins.resize(pinCounter);
    drawEvent.length = pinCounter;

    if(!checkEndByte(data, 0x43)){
        std::cerr << "readDrawEvent" << " checksum failed" << std::endl;
        return;
    }

    drawEventQueue.push(drawEvent);
}

std::optional<Graphiti_API::DrawEvent> Graphiti_API::getNextDrawEvent() {
    DrawEvent drawEvent;
    //Pop key from Key Event Queue
    if (drawEventQueue.pop(drawEvent)) {
        return drawEvent;
    }
    // Queue is empty
    return std::nullopt;
}

void Graphiti_API::setUpCommonResponseMap(){
    commonResponseMap = {
        {0x00, "Command Successful"},
        {0x01, "Command Error"},
        {0x02, "Communication Error"},
        {0x03, "Checksum Error"},
        {0x04, "Invalid Image API Error"},
        {0x05, "Image API Time Out Error"}
    };
}

void Graphiti_API::readCommonResponse() {
    std::vector<uint8_t> data(1, 0);

    auto bytes = conn_->read(1);
    if (!bytes.empty()) {
        data[0] = bytes[0];
    }
    else {
        // Error Case
        std::cerr << "readKeyEventStatus timed out" << std::endl;
        data[0] = 0;
        return;
    }

    if(!checkEndByte(data, 0x53)) {
        std::cerr << "readCommonResponse" << " checksum failed" << std::endl;
        return;
    }

    std::cerr << commonResponseMap[data[0]] << std::endl;
}

std::vector<uint8_t> Graphiti_API::calcChecksum(const std::vector<uint8_t>& data) {
    int sum = 0;
    for (auto b : data) {
        sum += b;
    }

    uint8_t ncs = static_cast<uint8_t>((-sum) & 0xFF);

    //SOF handling
    if (ncs == 0x1B) {
        return { 0x1B, 0x1B }; 
    } else {
        return { ncs };
    }
}

/*Helper function to write to Graphiti adding the checksum and Header Byte*/
void Graphiti_API::Graphiti_IO_Write(std::vector<uint8_t> bytes) {
    auto checksum = calcChecksum(bytes);
    bytes.insert(bytes.end(), checksum.begin(), checksum.end());  // add checksum to end
    bytes.insert(bytes.begin(), 0x1B); //Adds Header

    #ifdef DEBUG
        printVectorHex(bytes);
    #endif
    
    conn_->write(bytes);  // send full data
}

/*Helper function for API calls*/
std::vector<uint8_t> Graphiti_API::startOfFrameCheck(std::vector<uint8_t> bytes, uint8_t startByte) {
    for (size_t i = 0; i < bytes.size(); ++i) {
        if (bytes[i] == 0x1B) {
            bytes.insert(bytes.begin() + i, 0x1B);
            ++i;
        }
    }
    
    bytes.insert(bytes.begin(), startByte);
    
    return bytes;
}

void Graphiti_API::sendACK() {
    Graphiti_IO_Write({ 0x51 });
}

void Graphiti_API::sendNACK() {
    Graphiti_IO_Write({ 0x52 });
}

void Graphiti_API::getSoftwareVersion() {
    Graphiti_IO_Write({ 0x01 });
}

void Graphiti_API::getHardWareVersion() {
    Graphiti_IO_Write({ 0x02 });
}

void Graphiti_API::getSerialNumber() {
    Graphiti_IO_Write({ 0x04 });
}

void Graphiti_API::getBatteryStatus() {
    Graphiti_IO_Write({ 0x05 });
}

void Graphiti_API::getResolutionInformation() {
    Graphiti_IO_Write({ 0x07 });
}

void Graphiti_API::getDeviceOrientation() {
    Graphiti_IO_Write({ 0x08 });
}

void Graphiti_API::getHeightInformation() {
    Graphiti_IO_Write({ 0x09 });
}

void Graphiti_API::getDeviceName() {
    Graphiti_IO_Write({ 0x0A });
}

void Graphiti_API::updateDisplay(std::vector<uint8_t> screen) {
    Graphiti_IO_Write(startOfFrameCheck(screen, 0x15));
}

void Graphiti_API::setDisplay() {
    Graphiti_IO_Write({ 0x16, 0x02 });
}

void Graphiti_API::clearDisplay() {
    Graphiti_IO_Write({ 0x16, 0x03 });
}

void Graphiti_API::updateSinglePixel(uint8_t row, uint8_t column, uint8_t height, uint8_t blinkRate) {
    Graphiti_IO_Write(startOfFrameCheck({ row, column, height, blinkRate }, 0x17));
}

void Graphiti_API::updateSingleRow(uint8_t row, std::vector<uint8_t> rowData) {
    rowData.insert(rowData.begin(), row);
    Graphiti_IO_Write(startOfFrameCheck(rowData, 0x18));
}

void Graphiti_API::updateSingleColumn(uint8_t column, std::vector<uint8_t> columnData) {
    columnData.insert(columnData.begin(), column);
    Graphiti_IO_Write(startOfFrameCheck(columnData, 0x19));
}

void Graphiti_API::getAllPixelsPositionStatus() {
    Graphiti_IO_Write({ 0x20 });
}

void Graphiti_API::getSinglePixelPositionStatus(uint8_t row, uint8_t column) {
    Graphiti_IO_Write(startOfFrameCheck({ row, column }, 0x21));
}

void Graphiti_API::getSingleRowPixelPositionStatus(uint8_t row) {
    Graphiti_IO_Write(startOfFrameCheck({ row }, 0x22));
}

void Graphiti_API::getSingleColumnPixelPositionStatus(uint8_t column) {
    Graphiti_IO_Write(startOfFrameCheck({ column }, 0x23));
}

void Graphiti_API::showMessage(std::string message, std::vector<uint8_t> selectFlags, int blinkRate) {
    if(message.length() > 20) {
        std::cout << "Message must be under 20 characters" << std::endl;
        return;
    }
    auto messageBytes = std::vector<uint8_t>(message.begin(), message.end());

    std::vector<uint8_t> data(20, 0);

    for (size_t i = 0; i < messageBytes.size(); i++)
    {
        data[i*2] = messageBytes[i];
        data[i*2 + 1] = selectFlags[i];
    }
    
    messageBytes.insert(messageBytes.end(), static_cast<uint8_t>(blinkRate));
    Graphiti_IO_Write(startOfFrameCheck(messageBytes, 0x24));
}

void Graphiti_API::setCursor(int row, int column, int height, int length, int blinkRate) {
    Graphiti_IO_Write(startOfFrameCheck({ 
        static_cast<uint8_t>(row), 
        static_cast<uint8_t>(column), 
        static_cast<uint8_t>(height), 
        static_cast<uint8_t>(length), 
        static_cast<uint8_t>(blinkRate) 
    }, 0x25));
}

/*4.3.12 Send Image (Interruptible) 
This command is used to send and display an image file, such as a BMP, PNG, 
or JPG file on the device. It is possible to terminate the data transfer of an image 
in this API and send a new image. (Read Graphiti API Spec for more details)*/
void Graphiti_API::sendImageInterruptible(std::string name, const std::string& filepath) {
    #ifdef DEBUG
        std::cout << "sendImageInterruptible" << std::endl;
    #endif 

    sendImage(0x2F, name, filepath);
}

/*4.3.13 Send Image (Blocking) 
This command is used to display the actual image such as a BMP, PNG, or JPG 
file on the device. (Read Graphiti API Spec for more details)
*/

/**
 * @brief 4.3.13 Send Image (Blocking)
 * 
 * This command is used to display the actual image such as a BMP, PNG, or JPG 
 * file on the device. (Read Graphiti API Spec for more details)
 * 
 * @param name name of image with extension (.jpg)
 * @param size size of image
 * @param filepath exact or relative path to image
 */
void Graphiti_API::sendImageBlocking(std::string name, const std::string& filepath) {
    #ifdef DEBUG
        std::cout << "sendImageBlocking" << std::endl;
    #endif 

    sendImage(0x30, name, filepath);
}

/*Send Image function handling Interruptible and Blocking*/
void Graphiti_API::sendImage(uint8_t commandByte, std::string name, const std::string& filepath) {
    std::vector<uint8_t> data(name.begin(), name.end());
    data.insert(data.end(), 0x7C); // '|' character
    
    std::ifstream file(filepath, std::ios::binary);

    if (!file) throw std::runtime_error("Cannot open file");

    //Move file pointer for size
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.clear();
    file.seekg(0, std::ios::beg);

    data.insert(data.end(), {
        static_cast<uint8_t>((size >> 24) & 0xFF),
        static_cast<uint8_t>((size >> 16) & 0xFF),
        static_cast<uint8_t>((size >> 8) & 0xFF),
        static_cast<uint8_t>(size & 0xFF)
        });

    std::vector<uint8_t> imageData(size);
    file.read(reinterpret_cast<char*>(imageData.data()), size);

    data.insert(data.end(), imageData.begin(), imageData.end());

    if(commandByte == 0x30) {
        //Image Blocking does not send the data byte twice if it is a SOF(0x1B)
        //std::cerr << "Image Blocking" << "\n";
        data.insert(data.begin(), commandByte);
        Graphiti_IO_Write(data);
    } else {
        //Image Interuptable does
        Graphiti_IO_Write(startOfFrameCheck(data, commandByte));
    }
}


/*4.4 User Keys Access Commands*/

void Graphiti_API::setKeyEvent(bool enabled) {
    uint8_t enabledByte = (enabled ? 0x01 : 0x00);
    Graphiti_IO_Write({ 0x31,  enabledByte});
}

void Graphiti_API::setTouchEvent(bool enabled) {
    uint8_t enabledByte = (enabled ? 0x01 : 0x00);
    Graphiti_IO_Write({ 0x41, enabledByte});
}

void Graphiti_API::getLastTouchPointStatus() {
    Graphiti_IO_Write({ 0x44 });
}

void Graphiti_API::vibratorControlCommand(int frequencyRange, int dutyCycle, int duration) {
    Graphiti_IO_Write(startOfFrameCheck({ static_cast<uint8_t>(frequencyRange), static_cast<uint8_t>(dutyCycle), 
        static_cast<uint8_t>((duration >> 8) & 0xFF), 
        static_cast<uint8_t>(duration & 0xFF) }, 0x61));
}

void Graphiti_API::getDateAndTime() {
    Graphiti_IO_Write({ 0x65 });
}

void Graphiti_API::setDateAndTime(int day, int month, int year) {
    Graphiti_IO_Write(startOfFrameCheck({ 
        static_cast<uint8_t>(day), static_cast<uint8_t>(month),
        static_cast<uint8_t>((year >> 8) & 0xFF), 
        static_cast<uint8_t>(year & 0xFF) }, 0x66));
}

void Graphiti_API::debugByte(uint8_t byte) {
    #ifdef DEBUG
        std::cerr << "0x" << std::hex << std::uppercase 
            << std::setw(2) << std::setfill('0') 
            << static_cast<int>(byte) << std::endl;
    #endif
}

void Graphiti_API::printVectorHex(const std::vector<uint8_t>& data) {
    std::cerr << "[ ";
    for (uint8_t byte : data) {
        std::cerr << "0x" << std::hex << std::setw(2) << std::setfill('0') << (int)byte << " ";
    }
    std::cerr << "]" << std::dec << std::endl; // Reset back to decimal
}