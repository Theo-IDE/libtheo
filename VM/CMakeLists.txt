
set(LIBTHEO_VM_HEADERS
  include/instr.hpp
)

set(LIBTHEO_VM_SOURCES
  src/instr.cpp
)

add_library(TheoVM SHARED ${LIBTHEO_VM_HEADERS} ${LIBTHEO_VM_SOURCES})

target_include_directories(TheoVM PUBLIC
  include
  ${PROJECT_SOURCE_DIR}
)
