# Megatron
## Requisitos Previos

- Cmake

## Instalación

1.  Clonar repositorio
    git clone https://github.com/jxmichael111/Megatron.git

## Uso

1. Para compilar el proyecto por primera vez, crea la carpeta build, configura y compila el proyecto:
    - mkdir build
    - cd build
    - cmake ..
    - cmake --build .

    Linux:
    - solo make
    

2. Para compilar el proyecto después de la configuración inicial:
    - cmake --build .
    - make

3. Configurar la variable de entorno 'Ruta_Base'
    En la carpeta config.h modificar las variables de entorno y cambiar la ruta base 

    - Establecer variables de entorno en PowerShell:
        $env:<nombre_colaborador> = "1" Ejm. $env:ANLU = "1"
    - Establecer variables de entorno en Linux:
        export <nombre_colaborador>=1 Ejm. export ANLU=1

## Autor

1. Anlu
2. Melanie
3. Michael