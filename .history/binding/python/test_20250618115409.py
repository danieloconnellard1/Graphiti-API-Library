# test.py
import graphiti
from pathlib import Path

# Load the library
dll_path = Path(__file__).parent.parent / 'lib' / 'build' / 'libGraphiti_C.dll'
handle = graphiti(dll_path)

lib.graphiti
print(f"Handle: {hex(handle)}")