# test.py
from ctypes import CDLL, c_void_p

# Load the library
lib = CDLL('./libGraphiti_C.dll')  # or 'graphiti.dll' on Windows

# Specify argument/return types
lib.graphiti_create.restype = c_void_p

# Call C functions
handle = lib.graphiti_create()
print(f"Handle: {hex(handle)}")