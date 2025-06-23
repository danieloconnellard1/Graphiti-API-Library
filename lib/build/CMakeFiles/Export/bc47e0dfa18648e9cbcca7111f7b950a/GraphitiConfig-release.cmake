#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Graphiti::Graphiti" for configuration "Release"
set_property(TARGET Graphiti::Graphiti APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Graphiti::Graphiti PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/libGraphiti.dll.a"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/libGraphiti.dll"
  )

list(APPEND _cmake_import_check_targets Graphiti::Graphiti )
list(APPEND _cmake_import_check_files_for_Graphiti::Graphiti "${_IMPORT_PREFIX}/lib/libGraphiti.dll.a" "${_IMPORT_PREFIX}/bin/libGraphiti.dll" )

# Import target "Graphiti::Graphiti_C" for configuration "Release"
set_property(TARGET Graphiti::Graphiti_C APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Graphiti::Graphiti_C PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/libGraphiti_C.dll.a"
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "Graphiti::Graphiti"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/libGraphiti_C.dll"
  )

list(APPEND _cmake_import_check_targets Graphiti::Graphiti_C )
list(APPEND _cmake_import_check_files_for_Graphiti::Graphiti_C "${_IMPORT_PREFIX}/lib/libGraphiti_C.dll.a" "${_IMPORT_PREFIX}/bin/libGraphiti_C.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
