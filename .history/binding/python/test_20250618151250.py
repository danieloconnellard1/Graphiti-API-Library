# test.py
from graphiti import Graphiti
from pathlib import Path

dll_dir = r"C:/Users/Daniel/graphiti/bin"

# Load the library
dll_path = Path(r"C:/Users/Daniel/graphiti/bin/libGraphiti_C.dll")
print(f"Looking for DLL at: {dll_path}")
print(f"DLL exists: {dll_path.exists()}")

handle = Graphiti(dll_path, dll_dir)

handle.graphiti_create()

if not handle.start_vcp("COM4", False, False):
    raise RuntimeError("Connection failed")

handle.get_software_version()

strOpt = handle.get_next_output_event()
if strOpt != None:
    print(strOpt)

handle.shutdown_vcp(False, False)
