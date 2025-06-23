#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct GraphitiHandle GraphitiHandle;

GraphitiHandle* graphiti_create();
GraphitiHandle* graphiti_createWithConnection(void* connection);
void graphiti_destroy(GraphitiHandle* handle);

void graphiti_SetConnection(GraphitiHandle* handle, void* connection);

void graphiti_startResponseThread(GraphitiHandle* handle);
void graphiti_runResponseLoop(GraphitiHandle* handle);

char* graphiti_getNextOutputEvent(GraphitiHandle* handle);
void graphiti_free_string(char* str);

typedef struct {
    uint8_t* data;    // Pointer to event data
    int length;       // Length of data
    bool has_value;   // Whether optional has value
} DisplayStatusEvent_C;

DisplayStatusEvent_C graphiti_getNextDisplayStatusEvent(GraphitiHandle* handle);
void graphiti_freeNextDisplayStatusEvent(DisplayStatusEvent_C* event);

// ---- Key Event ----
typedef struct {
    char** keys;      // Array of key strings
    int count;        // Number of keys
    bool has_value;   // Whether optional has value
} KeyEvent_C;

KeyEvent_C graphiti_getNextKeyEvent(GraphitiHandle* handle);
void graphiti_freeKeyEvent(KeyEvent_C* event);

char* graphiti_getNextGestureEvent(GraphitiHandle* handle);

// ---- Draw Event ----
typedef struct {
    int rowID;
    int columnID;
    int height;
    int blinkRate;
} PinInfo_C;

typedef struct {
    PinInfo_C* pins;  // Array of pins
    int length;       // Number of pins
    bool has_value;   // Whether optional has value
} DrawEvent_C;

DrawEvent_C graphiti_get_next_draw_event(GraphitiHandle* handle);
void graphiti_free_draw_event(DrawEvent_C* event);

// Device Commands (simplified subset)
void graphiti_getSoftwareVersion(GraphitiHandle* handle);
void graphiti_getHardWareVersion(GraphitiHandle* handle);
void graphiti_getSerialNumber(GraphitiHandle* handle);
void graphiti_getBatteryStatus(GraphitiHandle* handle);
void graphiti_getResolutionInformation(GraphitiHandle* handle);
void graphiti_getDeviceOrientation(GraphitiHandle* handle);
void graphiti_getHeightInformation(GraphitiHandle* handle);
void graphiti_getDeviceName(GraphitiHandle* handle);

void graphiti_updateDisplay(GraphitiHandle* handle, const uint8_t* screen_data, int length);

void graphiti_setDisplay(GraphitiHandle* handle);
void graphiti_clearDisplay(GraphitiHandle* handle);
void graphiti_updateSinglePixel(GraphitiHandle* handle, int row, int column, int height, int blinkRate);
void graphiti_updateSingleRow(GraphitiHandle* handle, int row, const uint8_t* rowData, int length);
void graphiti_updateSingleColumn(GraphitiHandle* handle, int column, const uint8_t* columnData, int length);
void graphiti_getAllPixelsPositionStatus(GraphitiHandle* handle);
void graphiti_getSinglePixelPositionStatus(GraphitiHandle* handle, int row, int column);
void graphiti_getSingleRowPixelPositionStatus(GraphitiHandle* handle, uint8_t row);
void graphiti_getSingleColumnPixelPositionStatus(GraphitiHandle* handle, uint8_t column);
void graphiti_showMessage(GraphitiHandle* handle, const char* message, const uint8_t* selectFlags, int flagLength, int blinkRate);
void graphiti_setCursor(GraphitiHandle* handle, int row, int column, int height, int length, int blinkRate);
void graphiti_sendImageInterruptible(GraphitiHandle* handle, const char* name, const char* filepath);
void graphiti_sendImageBlocking(GraphitiHandle* handle, const char* name, const char* filepath);
void graphiti_setKeyEvent(GraphitiHandle* handle, bool enabled);
void graphiti_setTouchEvent(GraphitiHandle* handle, bool enabled);
void graphiti_getLastTouchPointStatus(GraphitiHandle* handle);
void graphiti_vibratorControlCommand(GraphitiHandle* handle, int frequencyRange, int dutyCycle, int duration);
void graphiti_getDateAndTime(GraphitiHandle* handle);
void graphiti_setDateAndTime(GraphitiHandle* handle, int day, int month, int year, int hour, int minute, int second);
void graphiti_printKeyEvent(GraphitiHandle* handle, const char** keyEvents, int count);


const char* graphiti_get_next_gesture_event(GraphitiHandle* handle);

#ifdef __cplusplus
}
#endif