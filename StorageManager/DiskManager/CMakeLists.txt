# Nombre del subproyecto
project(DiskManager)

# StorageManager/DiskManager/CMakeLists.txt
file(GLOB SOURCES Sources/*.cpp)
file(GLOB HEADERS Header/*.h)

add_library(DiskManager ${SOURCES} ${HEADERS})
target_include_directories(DiskManager PUBLIC Header)
