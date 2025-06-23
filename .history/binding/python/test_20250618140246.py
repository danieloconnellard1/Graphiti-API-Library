# test.py
from graphiti import Graphiti
from pathlib import Path

# Load the library
dll_path = Path(__file__).parent.parent / 'lib' / 'build' / "libGraphiti_C.dll"
handle = Graphiti(dll_path)

handle.graphiti_create()

if not handle.start_vcp("COM4", False, False):
    raise RuntimeError("Connection failed")

handle.get_software_version()

strOpt = handle.get_next_output_event()
if strOpt != None:
    print(strOpt)

handle.shutdown_vcp(False, False)
