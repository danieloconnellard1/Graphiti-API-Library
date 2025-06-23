# test.py
import graphiti as Graphiti
from pathlib import Path

print(dir(graphiti.Graphiti)) 

# Load the library
dll_path = Path(__file__).parent.parent / 'lib' / 'build' / 'libGraphiti_C.dll'
handle = Graphiti(dll_path)

if not handle.start_vcp("COM4", False, False):
    raise RuntimeError("Connection failed")




print(f"Handle: {hex(handle)}")