#----------------------------------------------------------------
# Generated CMake target import file for configuration "MinSizeRel".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "RocksDB::rocksdb" for configuration "MinSizeRel"
set_property(TARGET RocksDB::rocksdb APPEND PROPERTY IMPORTED_CONFIGURATIONS MINSIZEREL)
set_target_properties(RocksDB::rocksdb PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_MINSIZEREL "CXX"
  IMPORTED_LOCATION_MINSIZEREL "${_IMPORT_PREFIX}/lib/librocksdb.a"
  )

list(APPEND _cmake_import_check_targets RocksDB::rocksdb )
list(APPEND _cmake_import_check_files_for_RocksDB::rocksdb "${_IMPORT_PREFIX}/lib/librocksdb.a" )

# Import target "RocksDB::rocksdb-shared" for configuration "MinSizeRel"
set_property(TARGET RocksDB::rocksdb-shared APPEND PROPERTY IMPORTED_CONFIGURATIONS MINSIZEREL)
set_target_properties(RocksDB::rocksdb-shared PROPERTIES
  IMPORTED_LOCATION_MINSIZEREL "${_IMPORT_PREFIX}/lib/librocksdb.6.4.0.dylib"
  IMPORTED_SONAME_MINSIZEREL "@rpath/librocksdb.6.dylib"
  )

list(APPEND _cmake_import_check_targets RocksDB::rocksdb-shared )
list(APPEND _cmake_import_check_files_for_RocksDB::rocksdb-shared "${_IMPORT_PREFIX}/lib/librocksdb.6.4.0.dylib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
