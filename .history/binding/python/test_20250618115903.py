# test.py
import graphiti as Graphiti
from pathlib import Path

# Load the library
dll_path = Path(__file__).parent.parent / 'lib' / 'build' / 'libGraphiti_C.dll'
handle = Graphiti(dll_path)

handle.start_vcp()

print(f"Handle: {hex(handle)}")