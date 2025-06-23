// graphiti_capi.h
#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Forward declaration (opaque pointer pattern)
typedef struct GraphitiHandle GraphitiHandle;

// Constructor/Destructor
GraphitiHandle* graphiti_create();
GraphitiHandle* graphiti_createWithConnection(void* connection);
void graphiti_destroy(GraphitiHandle* handle);

// Connection Management
void graphiti_SetConnection(GraphitiHandle* handle, void* connection);

// Thread Control
void graphiti_startResponseThread(GraphitiHandle* handle);
void graphiti_runResponseLoop(GraphitiHandle* handle);

// Event Getters
const char* graphiti_get_next_output_event(GraphitiHandle* handle);
const char* graphiti_get_next_gesture_event(GraphitiHandle* handle);

// Device Commands (simplified subset)
void graphiti_get_software_version(GraphitiHandle* handle);
void graphiti_get_battery_status(GraphitiHandle* handle);
void graphiti_update_display(GraphitiHandle* handle, const uint8_t* screen_data, int length);
void graphiti_set_touch_event(GraphitiHandle* handle, int enabled);

#ifdef __cplusplus
}
#endif