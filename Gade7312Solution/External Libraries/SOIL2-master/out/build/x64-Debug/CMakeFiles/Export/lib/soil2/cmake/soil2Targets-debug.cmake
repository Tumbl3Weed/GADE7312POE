#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "soil2::soil2" for configuration "Debug"
set_property(TARGET soil2::soil2 APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(soil2::soil2 PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C;CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/soil2.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS soil2::soil2 )
list(APPEND _IMPORT_CHECK_FILES_FOR_soil2::soil2 "${_IMPORT_PREFIX}/lib/soil2.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
