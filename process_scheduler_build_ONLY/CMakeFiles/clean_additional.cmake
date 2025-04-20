# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles\\Process_Scheduler_Simulator_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Process_Scheduler_Simulator_autogen.dir\\ParseCache.txt"
  "Process_Scheduler_Simulator_autogen"
  )
endif()
