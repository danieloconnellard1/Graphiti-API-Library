# test.py
from ctypes import CDLL, c_void_p
from pathlib import Path

# Load the library
dll_path = Path(__file__).parent.parent / 'lib' / 'build' / 'libGraphiti_C.dll'
lib = CDLL(str(dll_path))

# Specify argument/return types
lib.graphiti_create.restype = c_void_p

# Call C functions
handle = lib.graphiti_create()

lib.graphiti
print(f"Handle: {hex(handle)}")