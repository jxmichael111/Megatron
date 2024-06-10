cmake_minimum_required(VERSION 3.15)
project(Megatron)

# Agregar las subcarpetas
add_subdirectory(StorageManager)

# Agregar el ejecutable principal
add_executable(megatron main.cpp)

# Enlazar con el StorageManager
target_link_libraries(megatron StorageManager)

# Incluir la carpeta StorageManager en las rutas de inclusión
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/StorageManager)

