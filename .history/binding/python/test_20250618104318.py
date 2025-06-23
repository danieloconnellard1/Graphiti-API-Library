# test.py
import ctypes
from pathlib import Path

# Load the library
dll_path = Path(__file__).parent.parent / 'lib' / 'build' / 'libGraphiti_C.dll'
lib = CDLL(str(dll_path))

# Specify argument/return types
lib.graphiti_create.restype = c_void_p
lib.graphiti_startUpVCP.argtypes = [
    c_void_p,    # GraphitiHandle
    c_char_p,    # const char* port
    c_bool,      # keyEventsBool
    c_bool       # touchEventsBool
]
lib.graphiti_startUpVCP.restype = c_bool

# Call C functions
handle = lib.graphiti_create()

lib.graphiti
print(f"Handle: {hex(handle)}")