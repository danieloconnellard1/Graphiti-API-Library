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
    void graphiti_set_connection(GraphitiHandle* handle, void* connection) {
        handle->api.setConnection(static_cast<GraphitiConnection*>(connection));
    }

    // Thread Control
    void graphiti_start_response_thread(GraphitiHandle* handle) {
        handle->api.startResponseThread();
    }

    // Event Getters (simplified to return raw strings)
    const char* graphiti_get_next_output_event(GraphitiHandle* handle) {
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
}