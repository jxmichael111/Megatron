# Nombre del subproyecto
project(StorageManager)

# Añadir las subcarpetas de AccessMethods, BufferManager y DiskManager
add_subdirectory(AccessMethods)
add_subdirectory(BufferManager)
add_subdirectory(DiskManager)

# Incluir los archivos de encabezado
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Añadir los archivos fuente
set(SOURCES
    StorageManager.cpp
    megatron.cpp
)

# Crear la librería StorageManager
add_library(StorageManager ${SOURCES})

# Enlazar las sublibrerías
target_link_libraries(StorageManager BufferManager DiskManager AccessMethods)
