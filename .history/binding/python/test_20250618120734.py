# test.py
import graphiti
from pathlib import Path

# Load the library
dll_path = Path(__file__).parent.parent / 'lib' / 'build' / 'libGraphiti_C.dll'
handle = Graphiti(dll_path)

if not handle.start_vcp("COM4", False, False):
    raise RuntimeError("Connection failed")


Graphiti.Graphiti.

print(f"Handle: {hex(handle)}")