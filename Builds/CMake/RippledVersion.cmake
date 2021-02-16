#[===================================================================[
   read version from source
#]===================================================================]

file (STRINGS src/MUSO/protocol/impl/BuildInfo.cpp BUILD_INFO)
foreach (line_ ${BUILD_INFO})
  if (line_ MATCHES "versionString[ ]*=[ ]*\"(.+)\"")
    set (MUSO_version ${CMAKE_MATCH_1})
  endif ()
endforeach ()
if (MUSO_version)
  message (STATUS "MUSO version: ${MUSO_version}")
else ()
  message (FATAL_ERROR "unable to determine MUSO version")
endif ()
