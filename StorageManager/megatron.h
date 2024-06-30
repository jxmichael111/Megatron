#ifndef MEGATRON_H
#define MEGATRON_H

#include "diskManager.h"
#include "BufferManager.h"

#include <iostream>
#include <string>
#include <sstream>

#define RUTA_BASE "F:\\UNSA\\2024-A\\Base de Datos II\\Megatron\\usr\\db\\"

class Megatron {
    private:
        DiskManager controladorDisco;
        BufferManager gestor;

        std::string esquemaActual; //Nombre del esquema actual
        



        int nroEncabezados; //contador de encabezados ATRIBUTOS
        
        std::string ruta; // Nombre del txt
        std::string nombreFile; //Nombre del archivo que se leerá 
        std::string nEsquema; //Nombre del archivo en el que se guardan las relaciones
        std::string esquema; //

    public:

        /*
        @author Andrea Cuela Y Michael Ticona
        */
        Megatron(); //constructor por defecto

        /*
        @author Andrea Cuela Y Michael Ticona
        */
        Megatron(bool longitud, int nroPlatos, int nroPistas, int nroSectores, int bytesxSector, int sectoresxBloque, int frames); //constructor
        
        // Sets ----------------------------------------------------------------------------

        /*
        @author Andrea Cuela Y Michael Ticona
        */
        void setEsquema(const std::string);

        /*
        @author Andrea Cuela Y Michael Ticona
        */
        void setRelacion(const std::string);

        // Esquema -------------------------------------------------------------------------

        /*
        @author Andrea Cuela Y Michael Ticona
        */
        void crearEsquemaDesdeCsv(const std::string, int); //Crea ESQUEMA y RELACIÓN desde un .CSV

        /*
        @author Andrea Cuela Y Michael Ticona
        */
        void agregarEsquemaManual(); //Agregar esquema de forma MANUAL

        //void eliminarEsquema(); //POR IMPLEMENTARSE

        // Relaciones ----------------------------------------------------------------------

        /*
        @author Andrea Cuela Y Michael Ticona
        */
        void ingresarRelacionManual(bool); //Ingresa el registro manual para el esquema

        /*
        @author Andrea Cuela Y Michael Ticona
        */
        void ingresarDesdeArchivoCsv(const std::string, int); //Ingresa registro desde un csv


        //Extras ----------------------------------------------------------------------------
        /*
        @author Andrea Cuela Y Michael Ticona
        */
        std::string crearArchivoRelacion(std::string); //Crea el archivo de la relación R1, R2, ....  

        //bool eliminarRegistro(std::string);

        /*
        @author Andrea Cuela
        */
        void menuDisco();

        /*
        @author Michael Ticona
        */
        void menuBuffer(); 

        
        //~Megatron();
};

#endif