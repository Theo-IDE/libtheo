
set(LIBTHEO_VM_HEADERS
  include/instr.hpp
)

set(LIBTHEO_VM_SOURCES
  src/instr.cpp
)

add_library(libTheoVM SHARED ${LIBTHEO_VM_HEADERS} ${LIBTHEO_VM_SOURCES})

target_include_directories(libTheoVM PUBLIC
  include
  ${CMAKE_SOURCE_DIR}
)
