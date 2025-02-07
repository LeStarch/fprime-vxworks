####
# VxWorks-Posix.cmake:
#
# VxWorks posix platform file for standard vxworks targets.
####

choose_fprime_implementation(Os/Console Os/Console/Posix)
choose_fprime_implementation(Os/Cpu Os/Cpu/Stub)
choose_fprime_implementation(Os/File Os/File/Posix)
choose_fprime_implementation(Os/Memory Os/Memory/Stub)
choose_fprime_implementation(Os/Mutex Os/Mutex/Posix)
choose_fprime_implementation(Os/Queue Os/Generic/PriorityQueue)
choose_fprime_implementation(Os/RawTime Os/RawTime/Stub)
choose_fprime_implementation(Os/Task Os/Task/Posix)

add_compile_definitions(TGT_OS_TYPE_VXWORKS)
set(FPRIME_USE_POSIX ON)
set(FPRIME_HAS_SOCKETS ON)

# Add VxWorks specific headers into the system
include_directories(SYSTEM "${CMAKE_CURRENT_LIST_DIR}/types")
