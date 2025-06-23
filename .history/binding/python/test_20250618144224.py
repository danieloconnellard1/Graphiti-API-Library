# test.py
from graphiti import Graphiti
from pathlib import Path
import os

os.add_dll_directory(r"C:/Users/Daniel/graphiti/bin")

handle = Graphiti()

handle.graphiti_create()

if not handle.start_vcp("COM4", False, False):
    raise RuntimeError("Connection failed")

handle.get_software_version()

strOpt = handle.get_next_output_event()
if strOpt != None:
    print(strOpt)

handle.shutdown_vcp(False, False)
