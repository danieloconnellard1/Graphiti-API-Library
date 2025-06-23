# test.py
from graphiti import Graphiti
from pathlib import Path
import os

os.add_dll_directory(r"C:/Users/Daniel/graphiti/bin")

graphitiLib = ctypes.CDLL("libGraphiti_C.dll")

# Load the library
dll_path = Path(__file__).parent.parent.parent / 'lib' / 'build' / "libGraphiti_C.dll"
print(f"Looking for DLL at: {dll_path}")
print(f"DLL exists: {dll_path.exists()}")

handle = Graphiti(dll_path)

handle.graphiti_create()

if not handle.start_vcp("COM4", False, False):
    raise RuntimeError("Connection failed")

handle.get_software_version()

strOpt = handle.get_next_output_event()
if strOpt != None:
    print(strOpt)

handle.shutdown_vcp(False, False)
