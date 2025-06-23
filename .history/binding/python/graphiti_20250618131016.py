from ctypes import (
    CDLL, c_void_p, c_bool, c_int, c_char_p, 
    POINTER, Structure, c_uint8, c_char, create_string_buffer,
    byref, cast
)
from typing import Optional, List, Dict, Union
from pathlib import Path
import os

class DisplayStatusEvent_C(Structure):
    _fields_ = [
        ("data", POINTER(c_uint8)),
        ("length", c_int),
        ("has_value", c_bool)
    ]

class KeyEvent_C(Structure):
    _fields_ = [
        ("keys", POINTER(c_char_p)),
        ("count", c_int),
        ("has_value", c_bool)
    ]

class PinInfo_C(Structure):
    _fields_ = [
        ("rowID", c_int),
        ("columnID", c_int),
        ("height", c_int),
        ("blinkRate", c_int)
    ]

class DrawEvent_C(Structure):
    _fields_ = [
        ("pins", POINTER(PinInfo_C)),
        ("length", c_int),
        ("has_value", c_bool)
    ]

class Graphiti:
    def __init__(self, path: Optional[Union[str, Path]] = 'libGraphiti_C.dll'):
        if isinstance(path, Path):
            path = str(path)
        self._lib = CDLL(path)
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

        # VCP calls
        self._lib.graphiti_startUpVCP.argtypes = [c_void_p, c_char_p, c_bool, c_bool]
        self._lib.graphiti_startUpVCP.restype = c_bool
        self._lib.shutDownVCP.argtypes = [c_void_p, c_bool, c_bool]
        self._lib.shutDownVCP.restype = None

        # Extension
        self._lib.index.argtypes = [c_void_p, c_int, c_int]
        self._lib.index.restype = c_int
        self._lib.setPin.argtypes = [c_void_p, c_int, c_int, c_int]
        self._lib.setPin.restype = None
        self._lib.graphiti_clearScreen.argtypes = [c_void_p]
        self._lib.graphiti_clearScreen.restype = None
        self._lib.graphiti_sleep.argtypes = [c_void_p, c_int]
        self._lib.setPin.restype = None

        # Response Thread
        self._lib.graphiti_startResponseThread.argtypes = [c_void_p]
        self._lib.graphiti_startResponseThread.restype = None
        self._lib.graphiti_stopResponseThread.argtypes = [c_void_p]
        self._lib.graphiti_stopResponseThread.restype = None

        # Event handling
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

        # Freeing for events
        self._lib.graphiti_freeString.argtypes = [c_char_p]
        self._lib.graphiti_freeString.restype = None
        self._lib.graphiti_freeDisplayStatusEvent.argtypes = [POINTER(DisplayStatusEvent_C)]
        self._lib.graphiti_freeDisplayStatusEvent.restype = None
        self._lib.graphiti_freeKeyEvent.argtypes = [POINTER(KeyEvent_C)]
        self._lib.graphiti_freeKeyEvent.restype = None
        self._lib.graphiti_freeDrawEvent.argtypes = [POINTER(DrawEvent_C)]
        self._lib.graphiti_freeDrawEvent.restype = None

        # API calls
        self._lib.graphiti_sendACK.argtypes = [c_void_p]
        self._lib.graphiti_sendACK.restype = None
        self._lib.graphiti_sendNACK.argtypes = [c_void_p]
        self._lib.graphiti_sendNACK.restype = None

        self._lib.graphiti_getSoftwareVersion.argtypes = [c_void_p]
        self._lib.graphiti_getSoftwareVersion.restype = None
        self._lib.graphiti_getHardWareVersion.argtypes = [c_void_p]
        self._lib.graphiti_getHardWareVersion.restype = None
        self._lib.graphiti_getSerialNumber.argtypes = [c_void_p]
        self._lib.graphiti_getSerialNumber.restype = None
        self._lib.graphiti_getBatteryStatus.argtypes = [c_void_p]
        self._lib.graphiti_getBatteryStatus.restype = None
        self._lib.graphiti_getResolutionInformation.argtypes = [c_void_p]
        self._lib.graphiti_getResolutionInformation.restype = None
        self._lib.graphiti_getDeviceOrientation.argtypes = [c_void_p]
        self._lib.graphiti_getDeviceOrientation.restype = None
        self._lib.graphiti_getHeightInformation.argtypes = [c_void_p]
        self._lib.graphiti_getHeightInformation.restype = None
        self._lib.graphiti_getDeviceName.argtypes = [c_void_p]
        self._lib.graphiti_getDeviceName.restype = None

        self._lib.graphiti_updateDisplay.argtypes = [c_void_p, POINTER(c_uint8), c_int]
        self._lib.graphiti_updateDisplay.restype = None
        self._lib.graphiti_setDisplay.argtypes = [c_void_p]
        self._lib.graphiti_setDisplay.restype = None
        self._lib.graphiti_clearDisplay.argtypes = [c_void_p]
        self._lib.graphiti_clearDisplay.restype = None
        self._lib.graphiti_updateSinglePixel.argtypes = [c_void_p, c_int, c_int, c_int, c_int]
        self._lib.graphiti_updateSinglePixel.restype = None
        self._lib.graphiti_updateSingleRow.argtypes = [c_void_p, c_int, POINTER(c_uint8), c_int]
        self._lib.graphiti_updateSingleRow.restype = None
        self._lib.graphiti_updateSingleColumn.argtypes = [c_void_p, c_int, POINTER(c_uint8), c_int]
        self._lib.graphiti_updateSingleColumn.restype = None

        # Status calls
        self._lib.graphiti_getAllPixelsPositionStatus.argtypes = [c_void_p]
        self._lib.graphiti_getAllPixelsPositionStatus.restype = None
        self._lib.graphiti_getSinglePixelPositionStatus.argtypes = [c_void_p, c_int, c_int]
        self._lib.graphiti_getSinglePixelPositionStatus.restype = None
        self._lib.graphiti_getSingleRowPixelPositionStatus.argtypes = [c_void_p, c_int]
        self._lib.graphiti_getSingleRowPixelPositionStatus.restype = None
        self._lib.graphiti_getSingleColumnPixelPositionStatus.argtypes = [c_void_p, c_int]
        self._lib.graphiti_getSingleColumnPixelPositionStatus.restype = None

        self._lib.graphiti_showMessage.argtypes = [c_void_p, c_char_p, POINTER(c_uint8), c_int, c_int]
        self._lib.graphiti_showMessage.restype = None
        self._lib.graphiti_setCursor.argtypes = [c_void_p, c_int, c_int, c_int, c_int, c_int]
        self._lib.graphiti_setCursor.restype = None
        self._lib.graphiti_sendImageInterruptible.argtypes = [c_void_p, c_char_p, c_char_p]
        self._lib.graphiti_sendImageInterruptible.restype = None
        self._lib.graphiti_sendImageBlocking.argtypes = [c_void_p, c_char_p, c_char_p]
        self._lib.graphiti_sendImageBlocking.restype = None

        self._lib.graphiti_setKeyEvent.argtypes = [c_void_p, c_bool]
        self._lib.graphiti_setKeyEvent.restype = None
        self._lib.graphiti_setTouchEvent.argtypes = [c_void_p, c_bool]
        self._lib.graphiti_setTouchEvent.restype = None
        self._lib.graphiti_getLastTouchPointStatus.argtypes = [c_void_p]
        self._lib.graphiti_getLastTouchPointStatus.restype = None
        self._lib.graphiti_vibratorControlCommand.argtypes = [c_void_p, c_int, c_int, c_int]
        self._lib.graphiti_vibratorControlCommand.restype = None

        self._lib.graphiti_getDateAndTime.argtypes = [c_void_p]
        self._lib.graphiti_getDateAndTime.restype = None
        self._lib.graphiti_setDateAndTime.argtypes = [c_void_p, c_int, c_int, c_int, c_int, c_int, c_int]
        self._lib.graphiti_setDateAndTime.restype = None

    def graphiti_create(self):
        self._lib.graphiti_create()

    def graphiti_create_with_connection(self, connection: c_void_p):
        self._lib.graphiti_createWithConnection(connection)

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        self._lib.graphiti_destroy(self._handle)

    # Handle management
    def set_connection(self, connection: c_void_p):
        self._lib.graphiti_setConnection(self._handle, connection)

    # VCP control
    def start_vcp(self, port: str, key_events: bool = True, touch_events: bool = True) -> bool:
        return self._lib.graphiti_startUpVCP(
            self._handle,
            port.encode('ascii'),
            c_bool(key_events),
            c_bool(touch_events)
        )

    def shutdown_vcp(self, key_events: bool = True, touch_events: bool = True):
        self._lib.graphiti_shutDownVCP(
            self._handle,
            c_bool(key_events),
            c_bool(touch_events)
        )

    # Extension screen management
    def index(self, row: int, col: int) -> int:
        return self._lib.graphiti_index(self._handle, row, col)

    def set_pin(self, row: int, col: int, height: int):
        self._lib.graphiti_setPin(self._handle, row, col, height)

    def clear_screen(self):
        self._lib.graphiti_clearScreen(self._handle)

    def sleep(self, seconds: int):
        self._lib.graphiti_sleep(self._handle, seconds)

    def start_response_thread(self):
        self._lib.graphiti_startResponseThread(self._handle)

    def stop_response_thread(self):
        self._lib.graphiti_stopResponseThread(self._handle)

    # Event handling
    def get_next_output_event(self) -> Optional[str]:
        ptr = self._lib.graphiti_getNextOutputEvent(self._handle)
        if not ptr:
            return None
        try:
            return ptr.decode('utf-8')
        finally:
            self._lib.graphiti_freeString(ptr)

    def get_next_display_status(self) -> Optional[Dict]:
        event = self._lib.graphiti_getNextDisplayStatusEvent(self._handle)
        try:
            if not event.has_value:
                return None
            return {
                'data': bytes(event.data[:event.length]),
                'length': event.length
            }
        finally:
            self._lib.graphiti_freeDisplayStatusEvent(byref(event))

    def get_next_key_event(self) -> Optional[List[str]]:
        event = self._lib.graphiti_get_next_key_event(self._handle)
        try:
            if not event.has_value:
                return None
            return [event.keys[i].decode('utf-8') for i in range(event.count)]
        finally:
            self._lib.graphiti_freeKeyEvent(byref(event))

    def get_next_gesture_event(self) -> Optional[str]:
        ptr = self._lib.graphiti_getNextGestureEvent(self._handle)
        if not ptr:
            return None
        try:
            return ptr.decode('utf-8')
        finally:
            self._lib.graphiti_freeString(ptr)

    def get_next_draw_event(self) -> Optional[List[Dict]]:
        event = self._lib.graphiti_getNextDrawEvent(self._handle)
        try:
            if not event.has_value:
                return None
            return [{
                'row': event.pins[i].rowID,
                'column': event.pins[i].columnID,
                'height': event.pins[i].height,
                'blink_rate': event.pins[i].blinkRate
            } for i in range(event.length)]
        finally:
            self._lib.graphiti_freeDrawEvent(byref(event))

    # Device control
    def send_ack(self):
        self._lib.graphiti_sendACK(self._handle)

    def send_nack(self):
        self._lib.graphiti_sendNACK(self._handle)

    def get_software_version(self):
        self._lib.graphiti_getSoftwareVersion(self._handle)


    def update_display(self, data: bytes):
        buf = (c_uint8 * len(data))(*data)
        self._lib.graphiti_updateDisplay(self._handle, buf, len(data))

    def set_display(self):
        self._lib.graphiti_setDisplay(self._handle)

    def update_pixel(self, row: int, column: int, height: int, blink_rate: int):
        self._lib.graphiti_updateSinglePixel(self._handle, row, column, height, blink_rate)

    def update_row(self, row: int, data: bytes):
        buf = (c_uint8 * len(data))(*data)
        self._lib.graphiti_updateSingleRow(self._handle, row, buf, len(data))

    def update_column(self, column: int, data: bytes):
        buf = (c_uint8 * len(data))(*data)
        self._lib.graphiti_updateSingleColumn(self._handle, column, buf, len(data))

    # UI functions
    def show_message(self, message: str, select_flags: bytes, blink_rate: int):
        flags_buf = (c_uint8 * len(select_flags))(*select_flags)
        self._lib.graphiti_showMessage(
            self._handle,
            message.encode('utf-8'),
            flags_buf,
            len(select_flags),
            blink_rate
        )

    def set_cursor(self, row: int, column: int, height: int, length: int, blink_rate: int):
        self._lib.graphiti_setCursor(self._handle, row, column, height, length, blink_rate)

    def send_image(self, name: str, filepath: str, blocking: bool = False):
        if not os.path.exists(filepath):
            raise FileNotFoundError(f"Image file not found: {filepath}")
        
        if blocking:
            self._lib.graphiti_sendImageBlocking(
                self._handle,
                name.encode('utf-8'),
                filepath.encode('utf-8')
            )
        else:
            self._lib.graphiti_sendImageInterruptible(
                self._handle,
                name.encode('utf-8'),
                filepath.encode('utf-8')
            )

    # Device settings
    def set_key_events(self, enabled: bool):
        self._lib.graphiti_setKeyEvent(self._handle, enabled)

    def set_touch_events(self, enabled: bool):
        self._lib.graphiti_setTouchEvent(self._handle, enabled)

    def vibrate(self, frequency: int, duty_cycle: int, duration_ms: int):
        self._lib.graphiti_vibratorControlCommand(self._handle, frequency, duty_cycle, duration_ms)

    def set_datetime(self, day: int, month: int, year: int, hour: int, minute: int, second: int):
        self._lib.graphiti_setDateAndTime(self._handle, day, month, year, hour, minute, second)
    
