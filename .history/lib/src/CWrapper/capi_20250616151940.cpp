#include <vector>
#include <set>
#include <string>
#include <cstring>

#include <Graphiti/CWrapper/capi.hpp>
#include <Graphiti/API.hpp>

// Opaque handle = C++ class
struct GraphitiHandle {
    Graphiti_API api;
};

// Constructor/Destructor
extern "C" {
    GraphitiHandle* graphiti_create() {
        return new GraphitiHandle();
    }

    GraphitiHandle* graphiti_create_with_connection(void* connection) {
        return new GraphitiHandle{static_cast<GraphitiConnection*>(connection)};
    }

    void graphiti_destroy(GraphitiHandle* handle) {
        delete handle;
    }

    // Connection
    void graphiti_setConnection(GraphitiHandle* handle, void* connection) {
        handle->api.setConnection(static_cast<GraphitiConnection*>(connection));
    }

    // Thread Control
    void graphiti_startResponseThread(GraphitiHandle* handle) {
        handle->api.startResponseThread();
    }

    void graphiti_stopResponseThread(GraphitiHandle* handle) {
        handle->api.stopResponseThread();
    }

    char* graphiti_getNextOutputEvent(GraphitiHandle* handle) {
        auto event = handle->api.getNextOutputEvent();
        return event ? strdup(event->c_str()) : nullptr; // strdup = malloc + copy
    }

    void graphiti_freeString(char* str) {
        free(str); // Matches strdup
    }

    DisplayStatusEvent_C graphiti_getNextDisplayStatusEvent(GraphitiHandle* handle) {
        DisplayStatusEvent_C result = {nullptr, 0, false};
        auto event = handle->api.getNextDisplayStatusEvent();
        
        if (event) {
            result.has_value = true;
            result.length = event->size();
            result.data = new uint8_t[result.length];
            memcpy(result.data, event->data(), result.length * sizeof(uint8_t));
        }
        
        return result;
    }

    void graphiti_freeDisplayStatusEvent(DisplayStatusEvent_C* event) {
        if (event->data) {
                delete[] event->data;
                event->data = nullptr;
            }
        }

        KeyEvent_C graphiti_get_next_key_event(GraphitiHandle* handle) {
        KeyEvent_C result = {nullptr, 0, false};
        auto event = handle->api.getNextKeyEvent();
        
        if (event) {
            result.has_value = true;
            result.count = event->size();
            result.keys = new char*[result.count];
            
            int i = 0;
            for (const auto& key : *event) {
                result.keys[i] = new char[key.size() + 1];
                strcpy(result.keys[i], key.c_str());
                i++;
            }
        }
        
        return result;
    }

    void graphiti_freeKeyEvent(KeyEvent_C* event) {
        if (event->keys) {
            for (int i = 0; i < event->count; i++) {
                delete[] event->keys[i];
            }
            delete[] event->keys;
            event->keys = nullptr;
        }
    }

    char* graphiti_getNextGestureEvent(GraphitiHandle* handle) {
        auto event = handle->api.getNextGestureEvent();
        return event ? strdup(event->c_str()) : nullptr; // strdup = malloc + copy
    }//Uses free string

    DrawEvent_C graphiti_getNextDrawEvent(GraphitiHandle* handle) {
        DrawEvent_C result = {nullptr, 0, false};
        auto event = handle->api.getNextDrawEvent();
        
        if (event) {
            result.has_value = true;
            result.length = event->pins.size();
            result.pins = new PinInfo_C[result.length];
            
            for (int i = 0; i < result.length; i++) {
                result.pins[i].rowID = event->pins[i].rowID;
                result.pins[i].columnID = event->pins[i].columnID;
                result.pins[i].height = event->pins[i].height;
                result.pins[i].blinkRate = event->pins[i].blinkRate;
            }
        }
        
        return result;
    }

    void graphiti_freeDrawEvent(DrawEvent_C* event) {
        if (event->pins) {
            delete[] event->pins;
            event->pins = nullptr;
        }
    }

    void graphiti_sendACK(GraphitiHandle* handle) {
        handle->api.sendACK();
    }

    void graphiti_sendNACK(GraphitiHandle* handle) {
        handle->api.sendNACK();
    }

    void graphiti_getSoftwareVersion(GraphitiHandle* handle) {
        handle->api.getSoftwareVersion();
    }

    void graphiti_updateDisplay(GraphitiHandle* handle, const uint8_t* screen_data, int length) {
        std::vector<uint8_t> data(screen_data, screen_data + length);
        handle->api.updateDisplay(data);
    }

    void graphiti_setDisplay(GraphitiHandle* handle) {
        handle->api.setDisplay();
    }

    void graphiti_clearDisplay(GraphitiHandle* handle) {
        handle->api.clearDisplay();
    }

    void graphiti_updateSinglePixel(GraphitiHandle* handle, int row, int column, int height, int blinkRate) {
        handle->api.updateSinglePixel(row, column, height, blinkRate);
    }

    void graphiti_updateSingleRow(GraphitiHandle* handle, int row, const uint8_t* rowData, int length) {
        std::vector<uint8_t> data(rowData, rowData + length);
        handle->api.updateSingleRow(row, data);
    }

    void graphiti_updateSingleColumn(GraphitiHandle* handle, int column, const uint8_t* columnData, int length) {
        std::vector<uint8_t> data(columnData, columnData + length);
        handle->api.updateSingleColumn(column, data);
    }

    void graphiti_getAllPixelsPositionStatus(GraphitiHandle* handle) {
        handle->api.getAllPixelsPositionStatus();
    }

    void graphiti_getSinglePixelPositionStatus(GraphitiHandle* handle, int row, int column) {
        handle->api.getSinglePixelPositionStatus(row, column);
    }

    void graphiti_getSingleRowPixelPositionStatus(GraphitiHandle* handle, uint8_t row) {
        handle->api.getSingleRowPixelPositionStatus(row);
    }

    void graphiti_getSingleColumnPixelPositionStatus(GraphitiHandle* handle, uint8_t column) {
        handle->api.getSingleColumnPixelPositionStatus(column);
    }

    void graphiti_showMessage(GraphitiHandle* handle, const char* message, const uint8_t* selectFlags, int flagLength, int blinkRate) {
        std::string msg(message);
        std::vector<uint8_t> flags(selectFlags, selectFlags + flagLength);
        handle->api.showMessage(msg, flags, blinkRate);
    }

    void graphiti_setCursor(GraphitiHandle* handle, int row, int column, int height, int length, int blinkRate) {
        handle->api.setCursor(row, column, height, length, blinkRate);
    }

    void graphiti_sendImageInterruptible(GraphitiHandle* handle, const char* name, const char* filepath) {
        handle->api.sendImageInterruptible(std::string(name), std::string(filepath));
    }

    void graphiti_sendImageBlocking(GraphitiHandle* handle, const char* name, const char* filepath) {
        handle->api.sendImageBlocking(std::string(name), std::string(filepath));
    }

    void graphiti_setKeyEvent(GraphitiHandle* handle, bool enabled) {
        handle->api.setKeyEvent(enabled);
    }

    void graphiti_setTouchEvent(GraphitiHandle* handle, bool enabled) {
        handle->api.setTouchEvent(enabled);
    }

    void graphiti_getLastTouchPointStatus(GraphitiHandle* handle) {
        handle->api.getLastTouchPointStatus();
    }

    void graphiti_vibratorControlCommand(GraphitiHandle* handle, int frequencyRange, int dutyCycle, int duration) {
        handle->api.vibratorControlCommand(frequencyRange, dutyCycle, duration);
    }

    void graphiti_getDateAndTime(GraphitiHandle* handle) {
        handle->api.getDateAndTime();
    }

    void graphiti_setDateAndTime(GraphitiHandle* handle, int day, int month, int year, int hour, int minute, int second) {
        handle->api.setDateAndTime(day, month, year, hour, minute, second);
    }
}