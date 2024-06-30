# Nombre del subproyecto
project(AccessMethods)

# StorageManager/AccessMethods/CMakeLists.txt
file(GLOB SOURCES Sources/*.cpp)
file(GLOB HEADERS Header/*.h)

add_library(AccessMethods ${SOURCES} ${HEADERS})
target_include_directories(AccessMethods PUBLIC Header)
