# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\GeldOpnemen_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\GeldOpnemen_autogen.dir\\ParseCache.txt"
  "GeldOpnemen_autogen"
  )
endif()
