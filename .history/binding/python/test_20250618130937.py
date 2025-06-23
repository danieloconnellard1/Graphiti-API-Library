# test.py
import graphiti
from pathlib import Path

# Load the library
dll_path = Path(__file__).parent.parent / 'lib' / 'build' 
handle = graphiti.Graphiti(dll_path)

handle.graphiti_create()

if not handle.start_vcp("COM4", False, False):
    raise RuntimeError("Connection failed")

handle.get_so

handle.shutdown_vcp(False, False)

print(f"Handle: {hex(handle)}")