# Nombre del subproyecto
project(AccessMethods)

# StorageManager/AccessMethods/CMakeLists.txt
file(GLOB SOURCES Sources/*.cpp)
file(GLOB HEADERS Header/*.h)

add_library(AccessMethods ${SOURCES} ${HEADERS})
target_include_directories(AccessMethods PUBLIC Header)



# Incluir los archivos de encabezado
# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Header)

# Añadir los archivos fuente
# file(GLOB SOURCES
#     "${CMAKE_CURRENT_SOURCE_DIR}/Sources/*.cpp"
# )

# Crear la librería AccessMethods
# add_library(AccessMethods ${SOURCES})
