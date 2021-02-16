#[===================================================================[
   install stuff
#]===================================================================]

install (
  TARGETS
    ed25519-donna
    common
    opts
    MUSO_syslibs
    MUSO_boost
    MUSOl_core
  EXPORT MUSOExports
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include)

if(${INSTALL_SECP256K1})
install (
  TARGETS
    secp256k1
  EXPORT MUSOExports
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include)
endif()

install (EXPORT MUSOExports
  FILE MUSOTargets.cmake
  NAMESPACE MUSO::
  DESTINATION lib/cmake/MUSO)
include (CMakePackageConfigHelpers)
write_basic_package_version_file (
  MUSOConfigVersion.cmake
  VERSION ${MUSO_version}
  COMPATIBILITY SameMajorVersion)

if (is_root_project)
  install (TARGETS MUSO RUNTIME DESTINATION bin)
  set_target_properties(MUSO PROPERTIES INSTALL_RPATH_USE_LINK_PATH ON)
  install (
    FILES
      ${CMAKE_CURRENT_SOURCE_DIR}/Builds/CMake/MUSOConfig.cmake
      ${CMAKE_CURRENT_BINARY_DIR}/MUSOConfigVersion.cmake
    DESTINATION lib/cmake/MUSO)
  # sample configs should not overwrite existing files
  # install if-not-exists workaround as suggested by
  # https://cmake.org/Bug/view.php?id=12646
  install(CODE "
    macro (copy_if_not_exists SRC DEST NEWNAME)
      if (NOT EXISTS \"\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/\${DEST}/\${NEWNAME}\")
        file (INSTALL FILE_PERMISSIONS OWNER_READ OWNER_WRITE DESTINATION \"\${CMAKE_INSTALL_PREFIX}/\${DEST}\" FILES \"\${SRC}\" RENAME \"\${NEWNAME}\")
      else ()
        message (\"-- Skipping : \$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/\${DEST}/\${NEWNAME}\")
      endif ()
    endmacro()
    copy_if_not_exists(\"${CMAKE_CURRENT_SOURCE_DIR}/cfg/MUSO-example.cfg\" etc MUSO.cfg)
    copy_if_not_exists(\"${CMAKE_CURRENT_SOURCE_DIR}/cfg/validators-example.txt\" etc validators.txt)
  ")
endif ()
