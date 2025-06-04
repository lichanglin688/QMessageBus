#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "QCoro5::Core" for configuration "Debug"
set_property(TARGET QCoro5::Core APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(QCoro5::Core PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/QCoro5Cored.lib"
  )

list(APPEND _cmake_import_check_targets QCoro5::Core )
list(APPEND _cmake_import_check_files_for_QCoro5::Core "${_IMPORT_PREFIX}/lib/QCoro5Cored.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
