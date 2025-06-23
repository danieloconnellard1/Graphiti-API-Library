from ctypes import CDLL, c_void_p, c_char_p, c_bool
from pathlib import Path

class Graphiti:
    def __init__(self):
        self._lib = CDLL('libGraphiti_C.dll')
        self._setup_types()
        
    def _setup_types(self):
        self._lib.graphiti_create.restype = c_void_p
        self._lib.graphiti_createWithConnection.restype = c_void_p
        self._lib.graphiti_startUpVCP.argtypes = [c_void_p, c_char_p, c_bool, c_bool]
        self._lib.graphiti_startUpVCP.restype = c_bool
        
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