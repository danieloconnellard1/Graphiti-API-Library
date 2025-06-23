from ctypes import (
    CDLL, c_void_p, c_bool, c_int, c_char_p, 
    POINTER, Structure, c_uint8, c_char, c_ubyte,
    create_string_buffer
)
from pathlib import Path

class Graphiti:
    def __init__(self):
        self._lib = CDLL('libGraphiti_C.dll')
        self._setup_types()
        
    def _setup_types(self):
        # Handle Management
        self._lib.graphiti_create.restype = c_void_p
        self._lib.graphiti_createWithConnection.argtypes = [c_void_p]
        self._lib.graphiti_createWithConnection.restype = c_void_p
        self._lib.graphiti_destroy.argtypes = [c_void_p]
        self._lib.graphiti_destroy.restype = None
        self._lib.graphiti_setConnection.argtypes = [c_void_p, c_void_p]
        self._lib.graphiti_setConnection.restype = None

        # VCP Control
        self._lib.graphiti_startUpVCP.argtypes = [c_void_p, c_char_p, c_bool, c_bool]
        self._lib.graphiti_startUpVCP.restype = c_bool
        self._lib.shutDownVCP.argtypes = [c_void_p, c_bool, c_bool]
        self._lib.shutDownVCP.restype = None

        # ACK/NACK
        self._lib.graphiti_sendACK.argtypes = [c_void_p]
        self._lib.graphiti_sendACK.restype = None
        self._lib.graphiti_sendNACK.argtypes = [c_void_p]
        self._lib.graphiti_sendNACK.restype = None

        # Display Control
        self._lib.graphiti_updateDisplay.argtypes = [c_void_p, POINTER(c_uint8), c_int]
        self._lib.graphiti_updateDisplay.restype = None
        self._lib.graphiti_setDisplay.argtypes = [c_void_p]
        self._lib.graphiti_setDisplay.restype = None
        self._lib.graphiti_clearDisplay.argtypes = [c_void_p]
        self._lib.graphiti_clearDisplay.restype = None

        # Pixel Control
        self._lib.setPin.argtypes = [c_void_p, c_int, c_int, c_int]
        self._lib.setPin.restype = None
        self._lib.graphiti_updateSinglePixel.argtypes = [c_void_p, c_int, c_int, c_int, c_int]
        self._lib.graphiti_updateSinglePixel.restype = None
        self._lib.graphiti_updateSingleRow.argtypes = [c_void_p, c_int, POINTER(c_uint8), c_int]
        self._lib.graphiti_updateSingleRow.restype = None
        self._lib.graphiti_updateSingleColumn.argtypes = [c_void_p, c_int, POINTER(c_uint8), c_int]
        self._lib.graphiti_updateSingleColumn.restype = None

        # Event Handling
        self._lib.graphiti_getNextOutputEvent.argtypes = [c_void_p]
        self._lib.graphiti_getNextOutputEvent.restype = c_char_p
        self._lib.graphiti_getNextDisplayStatusEvent.argtypes = [c_void_p]
        self._lib.graphiti_getNextDisplayStatusEvent.restype = DisplayStatusEvent_C
        self._lib.graphiti_getNextKeyEvent.argtypes = [c_void_p]
        self._lib.graphiti_getNextKeyEvent.restype = KeyEvent_C
        self._lib.graphiti_getNextGestureEvent.argtypes = [c_void_p]
        self._lib.graphiti_getNextGestureEvent.restype = c_char_p
        self._lib.graphiti_getNextDrawEvent.argtypes = [c_void_p]
        self._lib.graphiti_getNextDrawEvent.restype = DrawEvent_C

        # Memory Management
        self._lib.graphiti_freeString.argtypes = [c_char_p]
        self._lib.graphiti_freeString.restype = None
        self._lib.graphiti_freeDisplayStatusEvent.argtypes = [POINTER(DisplayStatusEvent_C)]
        self._lib.graphiti_freeDisplayStatusEvent.restype = None
        self._lib.graphiti_freeKeyEvent.argtypes = [POINTER(KeyEvent_C)]
        self._lib.graphiti_freeKeyEvent.restype = None
        self._lib.graphiti_freeDrawEvent.argtypes = [POINTER(DrawEvent_C)]
        self._lib.graphiti_freeDrawEvent.restype = None

        # Device Info
        self._lib.graphiti_getSoftwareVersion.argtypes = [c_void_p]
        self._lib.graphiti_getSoftwareVersion.restype = None
        self._lib.graphiti_getHardWareVersion.argtypes = [c_void_p]
        self._lib.graphiti_getHardWareVersion.restype = None
        self._lib.graphiti_getSerialNumber.argtypes = [c_void_p]
        self._lib.graphiti_getSerialNumber.restype = None
        self._lib.graphiti_getBatteryStatus.argtypes = [c_void_p]
        self._lib.graphiti_getBatteryStatus.restype = None

        # UI Functions
        self._lib.graphiti_showMessage.argtypes = [c_void_p, c_char_p, POINTER(c_uint8), c_int, c_int]
        self._lib.graphiti_showMessage.restype = None
        self._lib.graphiti_setCursor.argtypes = [c_void_p, c_int, c_int, c_int, c_int, c_int]
        self._lib.graphiti_setCursor.restype = None
        self._lib.graphiti_sendImageInterruptible.argtypes = [c_void_p, c_char_p, c_char_p]
        self._lib.graphiti_sendImageInterruptible.restype = None
        self._lib.graphiti_sendImageBlocking.argtypes = [c_void_p, c_char_p, c_char_p]
        self._lib.graphiti_sendImageBlocking.restype = None

        # Device Control
        self._lib.graphiti_setKeyEvent.argtypes = [c_void_p, c_bool]
        self._lib.graphiti_setKeyEvent.restype = None
        self._lib.graphiti_setTouchEvent.argtypes = [c_void_p, c_bool]
        self._lib.graphiti_setTouchEvent.restype = None
        self._lib.graphiti_vibratorControlCommand.argtypes = [c_void_p, c_int, c_int, c_int]
        self._lib.graphiti_vibratorControlCommand.restype = None

        # Date/Time
        self._lib.graphiti_getDateAndTime.argtypes = [c_void_p]
        self._lib.graphiti_getDateAndTime.restype = None
        self._lib.graphiti_setDateAndTime.argtypes = [c_void_p, c_int, c_int, c_int, c_int, c_int, c_int]
        self._lib.graphiti_setDateAndTime.restype = None
        
    def start_vcp(self, port, key_events=True, touch_events=True):
        handle = self._lib.graphiti_create()
        if not handle:
            raise RuntimeError("Failed to create handle")
            
        success = self._lib.graphiti_startUpVCP(
            handle,
            port.encode('ascii'),  # Convert str to bytes
            key_events,
            touch_events
        )
        return success