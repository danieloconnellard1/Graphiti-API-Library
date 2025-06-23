// graphiti_capi.cpp
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

    // Event Getters (simplified to return raw strings)
    const char* graphiti_getNextOutputEvent(GraphitiHandle* handle) {
        static std::string result; // Static to persist beyond function call
        auto event = handle->api.getNextOutputEvent();
        if (event) {
            result = *event;
            return result.c_str();
        }
        return nullptr;
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