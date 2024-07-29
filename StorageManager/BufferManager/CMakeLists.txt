# Nombre del subproyecto
project(BufferManager)

# StorageManager/BufferManager/CMakeLists.txt
file(GLOB SOURCES Sources/*.cpp)
file(GLOB HEADERS Header/*.h)

add_library(BufferManager ${SOURCES} ${HEADERS})
target_include_directories(BufferManager PUBLIC Header)
target_include_directories(BufferManager PUBLIC Sources)
