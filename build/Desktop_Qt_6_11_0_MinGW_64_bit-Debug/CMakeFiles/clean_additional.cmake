# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\TextEidt_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\TextEidt_autogen.dir\\ParseCache.txt"
  "TextEidt_autogen"
  )
endif()
