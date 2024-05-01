cmake_minimum_required(VERSION 3.15)
project(Megatron)

# Rutas completas a los archivos .cpp
set(SOURCES
    Disco.cpp
    StorageManager.cpp
    main.cpp

)

# Rutas completas a los archivos .h
set(HEADERS
    Disco.h
    StorageManager.h
)

# Crea el ejecutable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
