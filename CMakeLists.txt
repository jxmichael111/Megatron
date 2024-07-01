cmake_minimum_required(VERSION 3.15)
project(Megatron)

# Definir macros globalmente
if(DEFINED ENV{ANLU})
    add_definitions(-DANLU)
elseif(DEFINED ENV{MELANY})
    add_definitions(-DMELANY)
elseif(DEFINED ENV{MICHAEL})
    add_definitions(-DMICHAEL)
endif()

# Agregar las subcarpetas
add_subdirectory(StorageManager)

# Agregar el ejecutable principal
add_executable(megatron main.cpp)

# Enlazar con el StorageManager
target_link_libraries(megatron StorageManager)

# Incluir la carpeta StorageManager en las rutas de inclusión
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/StorageManager)