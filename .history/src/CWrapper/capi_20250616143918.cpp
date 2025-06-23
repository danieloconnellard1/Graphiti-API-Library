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
    void graphiti_SetConnection(GraphitiHandle* handle, void* connection) {
        handle->api.setConnection(static_cast<GraphitiConnection*>(connection));
    }

    // Thread Control
    void graphiti_startResponseThread(GraphitiHandle* handle) {
        handle->api.startResponseThread();
    }

    char* graphiti_getNextOutputEvent(GraphitiHandle* handle) {
        auto event = handle->api.getNextOutputEvent();
        return event ? strdup(event->c_str()) : nullptr; // strdup = malloc + copy
    }

    void graphiti_free_string(char* str) {
        free(str); // Matches strdup
    }

    DisplayStatusEvent_C graphiti_get_next_display_status_event(GraphitiHandle* handle) {
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

    void graphiti_free_display_status_event(DisplayStatusEvent_C* event) {
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

    void graphiti_free_key_event(KeyEvent_C* event) {
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

    DrawEvent_C graphiti_get_next_draw_event(GraphitiHandle* handle) {
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

    void graphiti_free_draw_event(DrawEvent_C* event) {
        if (event->pins) {
            delete[] event->pins;
            event->pins = nullptr;
        }
    }

    // Device Commands
    void graphiti_get_software_version(GraphitiHandle* handle) {
        handle->api.getSoftwareVersion();
    }

    void graphiti_update_display(GraphitiHandle* handle, const uint8_t* screen_data, int length) {
        std::vector<uint8_t> data(screen_data, screen_data + length);
        handle->api.updateDisplay(data);
    }

    void graphiti_update_single_pixel(GraphitiHandle* handle, int row, int column, int height, int blinkRate) {
        handle->api.updateSinglePixel(row, column, height, blinkRate);
    }

    void graphiti_update_single_row(GraphitiHandle* handle, int row, const uint8_t* rowData, int length) {
        std::vector<uint8_t> data(rowData, rowData + length);
        handle->api.updateSingleRow(row, data);
    }

    void graphiti_update_single_column(GraphitiHandle* handle, int column, const uint8_t* columnData, int length) {
        std::vector<uint8_t> data(columnData, columnData + length);
        handle->api.updateSingleColumn(column, data);
    }

    void graphiti_get_all_pixels_position_status(GraphitiHandle* handle) {
        handle->api.getAllPixelsPositionStatus();
    }

    void graphiti_get_single_pixel_position_status(GraphitiHandle* handle, int row, int column) {
        handle->api.getSinglePixelPositionStatus(row, column);
    }

    void graphiti_get_single_row_pixel_position_status(GraphitiHandle* handle, uint8_t row) {
        handle->api.getSingleRowPixelPositionStatus(row);
    }

    void graphiti_get_single_column_pixel_position_status(GraphitiHandle* handle, uint8_t column) {
        handle->api.getSingleColumnPixelPositionStatus(column);
    }

    void graphiti_show_message(GraphitiHandle* handle, const char* message, const uint8_t* selectFlags, int flagLength, int blinkRate) {
        std::string msg(message);
        std::vector<uint8_t> flags(selectFlags, selectFlags + flagLength);
        handle->api.showMessage(msg, flags, blinkRate);
    }

    void graphiti_set_cursor(GraphitiHandle* handle, int row, int column, int height, int length, int blinkRate) {
        handle->api.setCursor(row, column, height, length, blinkRate);
    }

    void graphiti_send_image_interruptible(GraphitiHandle* handle, const char* name, const char* filepath) {
        handle->api.sendImageInterruptible(std::string(name), std::string(filepath));
    }

    void graphiti_send_image_blocking(GraphitiHandle* handle, const char* name, const char* filepath) {
        handle->api.sendImageBlocking(std::string(name), std::string(filepath));
    }

    void graphiti_set_key_event(GraphitiHandle* handle, bool enabled) {
        handle->api.setKeyEvent(enabled);
    }

    void graphiti_set_touch_event(GraphitiHandle* handle, bool enabled) {
        handle->api.setTouchEvent(enabled);
    }

    void graphiti_get_last_touch_point_status(GraphitiHandle* handle) {
        handle->api.getLastTouchPointStatus();
    }

    void graphiti_vibrator_control_command(GraphitiHandle* handle, int frequencyRange, int dutyCycle, int duration) {
        handle->api.vibratorControlCommand(frequencyRange, dutyCycle, duration);
    }

    void graphiti_get_date_and_time(GraphitiHandle* handle) {
        handle->api.getDateAndTime();
    }

    void graphiti_set_date_and_time(GraphitiHandle* handle, int day, int month, int year, int hour, int minute, int second) {
        handle->api.setDateAndTime(day, month, year, hour, minute, second);
    }

    void graphiti_print_key_event(GraphitiHandle* handle, const char** keyEvents, int count) {
        std::set<std::string> keys;
        for (int i = 0; i < count; ++i) {
            keys.insert(std::string(keyEvents[i]));
        }
        handle->api.printKeyEvent(keys);
    }
}