#ifndef CONFIG_H
#define CONFIG_H

// Definir las rutas según el desarrollador
#ifdef ANLU
    #define RUTA_BASE "F:\\UNSA\\2024-A\\Base de Datos II\\Megatron\\usr\\db\\"
#elif defined(MICHAEL)
    #define RUTA_BASE "C:\\Users\\Michael\\Documents\\University\\5to semestre\\BDII\\Megatron\\"
#else
    #define RUTA_BASE "C:\\Users\\"
#endif

#endif // CONFIG_H
