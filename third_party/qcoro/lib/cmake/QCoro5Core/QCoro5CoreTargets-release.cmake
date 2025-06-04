#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "QCoro5::Core" for configuration "Release"
set_property(TARGET QCoro5::Core APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(QCoro5::Core PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/QCoro5Core.lib"
  )

list(APPEND _cmake_import_check_targets QCoro5::Core )
list(APPEND _cmake_import_check_files_for_QCoro5::Core "${_IMPORT_PREFIX}/lib/QCoro5Core.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
