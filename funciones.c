#include "funciones.h"
#include <stdio.h>
// Función para limpiar la pantalla
void limpiar_pantalla() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Función para pausar la ejecución
void pausar() {
    printf("\nPresione Enter para continuar...");
    getchar(); // Capturar el Enter anterior
    getchar(); // Esperar nuevo Enter
}

// Función para mostrar el menú principal
int menu_principal() {
    int opcion;

    limpiar_pantalla();
    printf("\n--------------------------------------------------\n");
    printf("      SISTEMA DE MONITOREO DE CONTAMINACION       \n");
    printf("----------------------------------------------------\n\n");
    printf(" [1] Ver estado actual de contaminacion\n");
    printf(" [2] Ver predicciones para las proximas 24h\n");
    printf(" [3] Ver promedios historicos\n");
    printf(" [4] Ver recomendaciones de accion\n");
    printf(" [5] Ingresar nuevos datos de monitoreo\n");
    printf(" [6] Exportar informacion a archivo\n");
    printf(" [7] Agregar nueva ciudad\n");
    printf(" [0] Salir del sistema\n\n");
    printf("Seleccione una opcion: ");
    scanf("%d", &opcion);

    return opcion;
}

// Datos históricos aproximados para Ecuador en los últimos 30 días
const float datos_historicos_ecuador[DIAS_HISTORICO][4] = {
    {400.0, 20.0, 25.0, 15.0}, {410.0, 22.0, 26.0, 16.0}, {405.0, 21.0, 24.0, 14.5},
    {420.0, 23.0, 27.0, 17.0}, {415.0, 22.5, 26.5, 16.5}, {430.0, 24.0, 28.0, 18.0},
    {440.0, 25.0, 29.0, 19.0}, {435.0, 24.5, 28.5, 18.5}, {450.0, 26.0, 30.0, 20.0},
    {445.0, 25.5, 29.5, 19.5}, {460.0, 27.0, 31.0, 21.0}, {455.0, 26.5, 30.5, 20.5},
    {470.0, 28.0, 32.0, 21.0}, {465.0, 27.5, 31.5, 20.5}, {480.0, 29.0, 33.0, 22.0},
    {475.0, 28.5, 32.5, 21.5}, {490.0, 30.0, 34.0, 23.0}, {485.0, 29.5, 33.5, 22.5},
    {500.0, 31.0, 35.0, 24.0}, {495.0, 30.5, 34.5, 23.5}, {510.0, 32.0, 36.0, 25.0},
    {505.0, 31.5, 35.5, 24.5}, {520.0, 33.0, 37.0, 26.0}, {515.0, 32.5, 36.5, 25.5},
    {530.0, 34.0, 38.0, 27.0}, {525.0, 33.5, 37.5, 26.5}, {540.0, 35.0, 39.0, 28.0},
    {535.0, 34.5, 38.5, 27.5}, {550.0, 36.0, 40.0, 30.0}, {545.0, 35.5, 39.5, 29.5}
};

void cargar_limites_oms(struct Limites *limites) {
    // Valores oficiales según las guías de calidad del aire de la OMS (2021) para zonas urbanas, ya en unidades finales
    limites->co2 = 800.0;  // ppm (partes por millón) - Valor de referencia para ambientes urbanos
    limites->so2 = 40.0;   // ppb (partes por billón) - Valor directo para zonas urbanas
    limites->no2 = 40.0;    // ppb (partes por billón) - Valor directo para zonas urbanas (actualizado en 2021)
    limites->pm25 = 35.0;   // µg/m³ - Promedio anual para zonas urbanas (actualizado en 2021)
    
    guardar_limites(*limites);
}

void guardar_limites(struct Limites limites) {
    FILE *archivo = fopen("limites_oms.txt", "w");
    if (archivo == NULL) {
        printf("Error al abrir el archivo para guardar limites.\n");
        return;
    }
    
    fprintf(archivo, "%.2f\n%.2f\n%.2f\n%.2f\n", 
            limites.co2, limites.so2, limites.no2, limites.pm25);
    
    fclose(archivo);
    printf("Limites guardados correctamente en 'limites_oms.txt'.\n");
}

// Función para ingresar límites personalizados
void ingresar_limites_personalizados(struct Limites *limites) {
    printf("\nIngrese los limites de contaminacion personalizados:\n");
    
    printf("Limite de CO2 (ppm): ");
    scanf("%f", &limites->co2);
    
    printf("Limite de SO2 (ppb): ");
    scanf("%f", &limites->so2);
    
    printf("Limite de NO2 (ppb): ");
    scanf("%f", &limites->no2);
    
    printf("Limite de PM2.5 (ug/m3): ");
    scanf("%f", &limites->pm25);
    
    // Guardar estos valores en el archivo
    guardar_limites(*limites);
}

// Función para cargar los límites desde un archivo
int cargar_limites_desde_archivo(struct Limites *limites) {
    FILE *archivo = fopen("limites_oms.txt", "r");
    if (archivo == NULL) {
        return 0; // No se pudo abrir el archivo
    }
    
    if (fscanf(archivo, "%f\n%f\n%f\n%f\n", 
               &limites->co2, &limites->so2, &limites->no2, &limites->pm25) != 4) {
        fclose(archivo);
        return 0; // Error al leer los datos
    }
    
    fclose(archivo);
    return 1; // Éxito
}

// Función para ingresar datos de una zona
void ingresar_datos_zona(struct Zona *zona) {
    printf("\nIngrese los niveles actuales de contaminacion para la zona %s:\n", zona->nombre);
    
    printf("Nivel de CO2 (ppm): ");
    scanf("%f", &zona->niveles_actuales.co2);
    
    printf("Nivel de SO2 (ppb): ");
    scanf("%f", &zona->niveles_actuales.so2);
    
    printf("Nivel de NO2 (ppb): ");
    scanf("%f", &zona->niveles_actuales.no2);
    
    printf("Nivel de PM2.5 (ug/m3): ");
    scanf("%f", &zona->niveles_actuales.pm25);
    
    printf("\nIngrese las condiciones climaticas actuales:\n");
    
    printf("Temperatura (C): ");
    scanf("%f", &zona->clima_actual.temperatura);
    
    printf("Humedad (%%): ");
    scanf("%f", &zona->clima_actual.humedad);
    
    printf("Velocidad del viento (km/h): ");
    scanf("%f", &zona->clima_actual.velocidad_viento);
}


void evaluar_niveles_contaminacion(struct Zona *zona, struct Limites limites) {
    // Variables para contar los niveles de cada categoría
    int nivel_bajo = 0;
    int nivel_medio = 0;
    int nivel_alto = 0;
    int nivel_critico = 0;
    
    // Evaluar CO2
    if (zona->niveles_actuales.co2 > limites.co2 * 2.0) {
        nivel_critico++;
    } else if (zona->niveles_actuales.co2 > limites.co2 * 1.5) {
        nivel_alto++;
    } else if (zona->niveles_actuales.co2 > limites.co2) {
        nivel_medio++;
    } else {
        nivel_bajo++;
    }
    
    // Evaluar SO2
    if (zona->niveles_actuales.so2 > limites.so2 * 2.0) {
        nivel_critico++;
    } else if (zona->niveles_actuales.so2 > limites.so2 * 1.5) {
        nivel_alto++;
    } else if (zona->niveles_actuales.so2 > limites.so2) {
        nivel_medio++;
    } else {
        nivel_bajo++;
    }
    
    // Evaluar NO2
    if (zona->niveles_actuales.no2 > limites.no2 * 2.0) {
        nivel_critico++;
    } else if (zona->niveles_actuales.no2 > limites.no2 * 1.5) {
        nivel_alto++;
    } else if (zona->niveles_actuales.no2 > limites.no2) {
        nivel_medio++;
    } else {
        nivel_bajo++;
    }
    
    // Evaluar PM2.5
    if (zona->niveles_actuales.pm25 > limites.pm25 * 2.0) {
        nivel_critico++;
    } else if (zona->niveles_actuales.pm25 > limites.pm25 * 1.5) {
        nivel_alto++;
    } else if (zona->niveles_actuales.pm25 > limites.pm25) {
        nivel_medio++;
    } else {
        nivel_bajo++;
    }
    
    // Asignar estado según la categoría predominante o la más grave
    if (nivel_critico > 0) {
        strcpy(zona->niveles_actuales.estado, "Critico");
    } else if (nivel_alto > 0) {
        strcpy(zona->niveles_actuales.estado, "Alto");
    } else if (nivel_medio > 0) {
        strcpy(zona->niveles_actuales.estado, "Medio");
    } else {
        strcpy(zona->niveles_actuales.estado, "Bajo");
    }
}

// Función para predecir niveles de contaminación
void predecir_niveles(struct Zona *zona) {
    float factor_viento = 1.0 - (zona->clima_actual.velocidad_viento / 100.0);
    float factor_humedad = 1.0 + (zona->clima_actual.humedad / 200.0);
    float factor_temperatura = 1.0 + (zona->clima_actual.temperatura / 100.0);

    zona->prediccion.niveles_predichos.co2 = zona->historico.promedios_co2 * factor_temperatura * factor_humedad * factor_viento;
    zona->prediccion.niveles_predichos.so2 = zona->historico.promedios_so2 * factor_temperatura * factor_humedad * factor_viento;
    zona->prediccion.niveles_predichos.no2 = zona->historico.promedios_no2 * factor_temperatura * factor_humedad * factor_viento;
    zona->prediccion.niveles_predichos.pm25 = zona->historico.promedios_pm25 * factor_humedad * factor_viento;

    if (zona->prediccion.niveles_predichos.co2 > zona->historico.promedios_co2 ||
        zona->prediccion.niveles_predichos.so2 > zona->historico.promedios_so2 ||
        zona->prediccion.niveles_predichos.no2 > zona->historico.promedios_no2 ||
        zona->prediccion.niveles_predichos.pm25 > zona->historico.promedios_pm25) {

        sprintf(zona->prediccion.alerta, "ALERTA: Se preve un incremento en los niveles en la zona %s", zona->nombre);
    } else {
        strcpy(zona->prediccion.alerta, "No hay alertas previstas para las proximas 24 horas");
    }
}

// Función para simular datos históricos
void simular_datos_historicos(struct Zona *zona, struct Limites limites, int indice_zona) {
    for (int i = 0; i < DIAS_HISTORICO; i++) {
        float factor_zona = 1.0 + (indice_zona * 0.05); // Factor único por zona

        zona->historico.datos[i].co2 = datos_historicos_ecuador[i][0] * factor_zona;
        zona->historico.datos[i].so2 = datos_historicos_ecuador[i][1] * factor_zona;
        zona->historico.datos[i].no2 = datos_historicos_ecuador[i][2] * factor_zona;
        zona->historico.datos[i].pm25 = datos_historicos_ecuador[i][3] * factor_zona;
    }
}

// Función para calcular promedios históricos
void calcular_promedios_historicos(struct Zona *zona) {
    float suma_co2 = 0, suma_so2 = 0, suma_no2 = 0, suma_pm25 = 0;
    
    for (int i = 0; i < DIAS_HISTORICO; i++) {
        suma_co2 += zona->historico.datos[i].co2;
        suma_so2 += zona->historico.datos[i].so2;
        suma_no2 += zona->historico.datos[i].no2;
        suma_pm25 += zona->historico.datos[i].pm25;
    }
    
    zona->historico.promedios_co2 = suma_co2 / DIAS_HISTORICO;
    zona->historico.promedios_so2 = suma_so2 / DIAS_HISTORICO;
    zona->historico.promedios_no2 = suma_no2 / DIAS_HISTORICO;
    zona->historico.promedios_pm25 = suma_pm25 / DIAS_HISTORICO;
}

// Función para generar recomendaciones
void generar_recomendaciones(struct Zona *zona, struct Limites limites) {
    char recomendaciones[500] = "";
    
    // Usar el estado evaluado para dar recomendaciones específicas
    if (strcmp(zona->niveles_actuales.estado, "Bajo") == 0) {
        strcat(recomendaciones, "- Mantener el monitoreo regular.\n");
        strcat(recomendaciones, "- Continuar con medidas preventivas habituales.\n");
    } 
    else if (strcmp(zona->niveles_actuales.estado, "Medio") == 0) {
        // Verificar qué contaminantes están en nivel medio
        if (zona->niveles_actuales.pm25 > limites.pm25) {
            strcat(recomendaciones, "- Personas sensibles deben limitar actividades al aire libre.\n");
        }
        if (zona->niveles_actuales.no2 > limites.no2) {
            strcat(recomendaciones, "- Considerar reduccion voluntaria del uso de vehiculos.\n");
        }
        if (zona->niveles_actuales.so2 > limites.so2) {
            strcat(recomendaciones, "- Monitoreo especial en areas industriales.\n");
        }
        if (zona->niveles_actuales.co2 > limites.co2) {
            strcat(recomendaciones, "- Mejorar ventilacion en espacios cerrados.\n");
        }
    } 
    else if (strcmp(zona->niveles_actuales.estado, "Alto") == 0) {
        // Verificar qué contaminantes están en nivel alto
        if (zona->niveles_actuales.pm25 > limites.pm25 * 1.5) {
            strcat(recomendaciones, "- Evitar actividades al aire libre para toda la poblacion.\n");
            strcat(recomendaciones, "- Uso recomendado de mascarillas en exteriores.\n");
        }
        if (zona->niveles_actuales.no2 > limites.no2 * 1.5) {
            strcat(recomendaciones, "- Restriccion parcial de circulacion vehicular.\n");
            strcat(recomendaciones, "- Promover uso de transporte publico.\n");
        }
        if (zona->niveles_actuales.so2 > limites.so2 * 1.5) {
            strcat(recomendaciones, "- Reducir actividad en industrias contaminantes.\n");
        }
        if (zona->niveles_actuales.co2 > limites.co2 * 1.5) {
            strcat(recomendaciones, "- Verificar fuentes de emision en la zona.\n");
        }
    } 
    else if (strcmp(zona->niveles_actuales.estado, "Critico") == 0) {
        // Para nivel crítico, medidas drásticas
        strcat(recomendaciones, "- ALERTA: Niveles de contaminacion peligrosos.\n");
        
        if (zona->niveles_actuales.pm25 > limites.pm25 * 2.0) {
            strcat(recomendaciones, "- Suspender todas las actividades al aire libre.\n");
            strcat(recomendaciones, "- Uso obligatorio de mascarillas en exteriores.\n");
        }
        if (zona->niveles_actuales.no2 > limites.no2 * 2.0) {
            strcat(recomendaciones, "- Restriccion total de vehiculos particulares.\n");
        }
        if (zona->niveles_actuales.so2 > limites.so2 * 2.0) {
            strcat(recomendaciones, "- Cierre temporal de industrias contaminantes.\n");
        }
        if (zona->niveles_actuales.co2 > limites.co2 * 2.0) {
            strcat(recomendaciones, "- Evaluacion de evacuacion en areas criticas.\n");
        }
    }
    
    // Si no hay recomendaciones específicas
    if (strlen(recomendaciones) == 0) {
        strcpy(zona->recomendaciones, "No se requieren medidas especiales.");
    } else {
        strcpy(zona->recomendaciones, recomendaciones);
    }
}

// Función para inicializar una ciudad
void inicializar_ciudad(struct Ciudad *ciudad) {
    printf("Ingrese el nombre de la ciudad a monitorear: ");
    scanf("%s", ciudad->nombre);
    
    do {
        printf("Ingrese el numero de zonas a monitorear (minimo 1, maximo %d): ", MAX_ZONAS);
        scanf("%d", &ciudad->num_zonas);
    } while (ciudad->num_zonas < 1 || ciudad->num_zonas > MAX_ZONAS);
    
    // Ingresar nombres de las zonas
    for (int i = 0; i < ciudad->num_zonas; i++) {
        printf("Ingrese el nombre de la zona %d: ", i + 1);
        scanf("%s", ciudad->zonas[i].nombre);
    }
}

// Función para exportar toda la información a un archivo
void exportar_informacion(struct Ciudad ciudad, struct Limites limites) {
    char nombre_archivo[100];
    sprintf(nombre_archivo, "informe_%s.txt", ciudad.nombre);
    
    FILE *archivo = fopen(nombre_archivo, "w");
    if (archivo == NULL) {
        printf("Error al crear el archivo de informe.\n");
        return;
    }
    
    // Encabezado
    fprintf(archivo, "+--------------------------------------------------+\n");
    fprintf(archivo, "|   INFORME DE MONITOREO DE CONTAMINACION DEL AIRE   |\n");
    fprintf(archivo, "+--------------------------------------------------+\n\n");
    
    // Información de la ciudad
    fprintf(archivo, "+--------------------------------------------------+\n");
    fprintf(archivo, "| Ciudad: %-41s |\n", ciudad.nombre);
    fprintf(archivo, "| Fecha de informe: %-32s |\n", __DATE__);
    fprintf(archivo, "+--------------------------------------------------+\n\n");
    
    // Límites establecidos
    fprintf(archivo, "+--------------------------------------------------+\n");
    fprintf(archivo, "|       LIMITES DE CONTAMINACION ESTABLECIDOS       |\n");
    fprintf(archivo, "+------------------+-------------------------------+\n");
    fprintf(archivo, "| CO2              | %-27.2f ppm |\n", limites.co2);
    fprintf(archivo, "| SO2              | %-27.2f ppb |\n", limites.so2);
    fprintf(archivo, "| NO2              | %-27.2f ppb |\n", limites.no2);
    fprintf(archivo, "| PM2.5            | %-27.2f ug/m3 |\n", limites.pm25);
    fprintf(archivo, "+------------------+-------------------------------+\n\n");
    
    // Información de cada zona
    for (int i = 0; i < ciudad.num_zonas; i++) {
        fprintf(archivo, "+--------------------------------------------------+\n");
        fprintf(archivo, "| ZONA: %-42s |\n", ciudad.zonas[i].nombre);
        fprintf(archivo, "+--------------------------------------------------+\n\n");
        
        // Niveles actuales
        fprintf(archivo, "+--------------------------------------------------+\n");
        fprintf(archivo, "|        NIVELES ACTUALES DE CONTAMINACION          |\n");
        fprintf(archivo, "+------------------+-------------------------------+\n");
        fprintf(archivo, "| CO2              | %-27.2f ppm |\n", ciudad.zonas[i].niveles_actuales.co2);
        fprintf(archivo, "| Estado           | %-27s |\n", ciudad.zonas[i].niveles_actuales.estado);
        fprintf(archivo, "| SO2              | %-27.2f ppb |\n", ciudad.zonas[i].niveles_actuales.so2);
        fprintf(archivo, "| NO2              | %-27.2f ppb |\n", ciudad.zonas[i].niveles_actuales.no2);
        fprintf(archivo, "| PM2.5            | %-27.2f ug/m3 |\n", ciudad.zonas[i].niveles_actuales.pm25);
        fprintf(archivo, "+------------------+-------------------------------+\n\n");
        
        // Condiciones climáticas
        fprintf(archivo, "+--------------------------------------------------+\n");
        fprintf(archivo, "|             CONDICIONES CLIMATICAS                |\n");
        fprintf(archivo, "+------------------+-------------------------------+\n");
        fprintf(archivo, "| Temperatura      | %-27.1f C |\n", ciudad.zonas[i].clima_actual.temperatura);
        fprintf(archivo, "| Humedad          | %-27.1f %% |\n", ciudad.zonas[i].clima_actual.humedad);
        fprintf(archivo, "| Velocidad viento | %-27.1f km/h |\n", ciudad.zonas[i].clima_actual.velocidad_viento);
        fprintf(archivo, "+------------------+-------------------------------+\n\n");
        
        // Predicciones
        fprintf(archivo, "+--------------------------------------------------+\n");
        fprintf(archivo, "|      PREDICCION PARA LAS PROXIMAS 24 HORAS        |\n");
        fprintf(archivo, "+------------------+-------------------------------+\n");
        fprintf(archivo, "| CO2              | %-27.2f ppm |\n", ciudad.zonas[i].prediccion.niveles_predichos.co2);
        fprintf(archivo, "| SO2              | %-27.2f ppb |\n", ciudad.zonas[i].prediccion.niveles_predichos.so2);
        fprintf(archivo, "| NO2              | %-27.2f ppb |\n", ciudad.zonas[i].prediccion.niveles_predichos.no2);
        fprintf(archivo, "| PM2.5            | %-27.2f ug/m3 |\n", ciudad.zonas[i].prediccion.niveles_predichos.pm25);
        fprintf(archivo, "+------------------+-------------------------------+\n");
        fprintf(archivo, "| Alerta: %-42s |\n", ciudad.zonas[i].prediccion.alerta);
        fprintf(archivo, "+--------------------------------------------------+\n\n");
        
        // Promedios históricos
        fprintf(archivo, "+--------------------------------------------------+\n");
        fprintf(archivo, "|      PROMEDIOS HISTORICOS (ULTIMOS 30 DIAS)       |\n");
        fprintf(archivo, "+------------------+-------------------------------+\n");
        fprintf(archivo, "| CO2              | %-27.2f ppm |\n", ciudad.zonas[i].historico.promedios_co2);
        fprintf(archivo, "| SO2              | %-27.2f ppb |\n", ciudad.zonas[i].historico.promedios_so2);
        fprintf(archivo, "| NO2              | %-27.2f ppb |\n", ciudad.zonas[i].historico.promedios_no2);
        fprintf(archivo, "| PM2.5            | %-27.2f ug/m3 |\n", ciudad.zonas[i].historico.promedios_pm25);
        fprintf(archivo, "+------------------+-------------------------------+\n\n");
        
        // Recomendaciones
        fprintf(archivo, "+--------------------------------------------------+\n");
        fprintf(archivo, "|                RECOMENDACIONES                    |\n");
        fprintf(archivo, "+--------------------------------------------------+\n");
        fprintf(archivo, "| %-48s |\n", ciudad.zonas[i].recomendaciones);
        fprintf(archivo, "+--------------------------------------------------+\n\n");
        
        fprintf(archivo, "+--------------------------------------------------+\n\n");
    }
    
    fclose(archivo);
    printf("\nInforme exportado correctamente a '%s'.\n", nombre_archivo);
}

// Función para mostrar el estado actual
void mostrar_estado_actual(struct Ciudad ciudad, struct Limites limites) {
    printf("\n+--------------------------------------------------+\n");
    printf("| ESTADO ACTUAL DE CONTAMINACION EN %-14s |\n", ciudad.nombre);
    printf("+--------------------------------------------------+\n\n");
    
    for (int i = 0; i < ciudad.num_zonas; i++) {
        printf("+--------------------------------------------------+\n");
        printf("| ZONA: %-42s |\n", ciudad.zonas[i].nombre);
        printf("| Estado: %-41s |\n", ciudad.zonas[i].niveles_actuales.estado);
        printf("+------------------+-------------+-----------------+\n");
        printf("| Contaminante     | Nivel       | Limite          |\n");
        printf("+------------------+-------------+-----------------+\n");
        printf("| CO2              | %-9.2f ppm | %-9.2f ppm    |\n", 
               ciudad.zonas[i].niveles_actuales.co2, limites.co2);
        printf("| SO2              | %-9.2f ppb | %-9.2f ppb    |\n", 
               ciudad.zonas[i].niveles_actuales.so2, limites.so2);
        printf("| NO2              | %-9.2f ppb | %-9.2f ppb    |\n", 
               ciudad.zonas[i].niveles_actuales.no2, limites.no2);
        printf("| PM2.5            | %-9.2f ug/m3 | %-9.2f ug/m3  |\n", 
               ciudad.zonas[i].niveles_actuales.pm25, limites.pm25);
        printf("+------------------+-------------+-----------------+\n\n");
    }
}

// Función para mostrar predicciones
void mostrar_predicciones(struct Ciudad ciudad) {
    printf("\n+--------------------------------------------------+\n");
    printf("|   PREDICCIONES PARA LAS PROXIMAS 24 HORAS        |\n");
    printf("+--------------------------------------------------+\n\n");
    
    for (int i = 0; i < ciudad.num_zonas; i++) {
        printf("+--------------------------------------------------+\n");
        printf("| ZONA: %-42s |\n", ciudad.zonas[i].nombre);
        printf("+------------------+-------------------------------+\n");
        printf("| Prediccion CO2   | %-27.2f ppm |\n", 
               ciudad.zonas[i].prediccion.niveles_predichos.co2);
        printf("| Prediccion SO2   | %-27.2f ppb |\n", 
               ciudad.zonas[i].prediccion.niveles_predichos.so2);
        printf("| Prediccion NO2   | %-27.2f ppb |\n", 
               ciudad.zonas[i].prediccion.niveles_predichos.no2);
        printf("| Prediccion PM2.5 | %-27.2f ug/m3 |\n", 
               ciudad.zonas[i].prediccion.niveles_predichos.pm25);
        printf("+------------------+-------------------------------+\n");
        printf("| Alerta: %-42s |\n", ciudad.zonas[i].prediccion.alerta);
        printf("+--------------------------------------------------+\n\n");
    }
}

// Función para mostrar promedios históricos
void mostrar_promedios_historicos(struct Ciudad ciudad, struct Limites limites) {
    printf("\n+--------------------------------------------------+\n");
    printf("|     PROMEDIOS HISTORICOS (ULTIMOS 30 DIAS)       |\n");
    printf("+--------------------------------------------------+\n\n");
    
    for (int i = 0; i < ciudad.num_zonas; i++) {
        printf("+--------------------------------------------------+\n");
        printf("| ZONA: %-42s |\n", ciudad.zonas[i].nombre);
        printf("+------------------+-------------+-----------------+\n");
        printf("| Contaminante     | Promedio    | Limite          |\n");
        printf("+------------------+-------------+-----------------+\n");
        printf("| CO2              | %-9.2f ppm | %-9.2f ppm    |\n", 
               ciudad.zonas[i].historico.promedios_co2, limites.co2);
        printf("| SO2              | %-9.2f ppb | %-9.2f ppb    |\n", 
               ciudad.zonas[i].historico.promedios_so2, limites.so2);
        printf("| NO2              | %-9.2f ppb | %-9.2f ppb    |\n", 
               ciudad.zonas[i].historico.promedios_no2, limites.no2);
        printf("| PM2.5            | %-9.2f ug/m3 | %-9.2f ug/m3  |\n", 
               ciudad.zonas[i].historico.promedios_pm25, limites.pm25);
        printf("+------------------+-------------+-----------------+\n\n");
    }
}

// Función para mostrar recomendaciones
void mostrar_recomendaciones(struct Ciudad ciudad) {
    printf("\n+--------------------------------------------------+\n");
    printf("|            RECOMENDACIONES DE ACCION             |\n");
    printf("+--------------------------------------------------+\n\n");
    
    for (int i = 0; i < ciudad.num_zonas; i++) {
        printf("+--------------------------------------------------+\n");
        printf("| ZONA: %-42s |\n", ciudad.zonas[i].nombre);
        printf("+--------------------------------------------------+\n");
        printf("| Recomendaciones:                                 |\n");
        
        // Dividir las recomendaciones por líneas y mostrarlas en formato de tabla
        char recomendaciones_copy[500];
        strcpy(recomendaciones_copy, ciudad.zonas[i].recomendaciones);
        
        char *linea = strtok(recomendaciones_copy, "\n");
        while (linea != NULL) {
            printf("| %-48s |\n", linea);
            linea = strtok(NULL, "\n");
        }
        
        printf("+--------------------------------------------------+\n\n");
    }
}

// Función para inicializar el sistema de monitoreo
void inicializar_sistema(struct SistemaMonitoreo *sistema) {
    sistema->num_ciudades = 0;
    
    // Intentar cargar ciudades desde archivo
    if (!cargar_ciudades(sistema)) {
        printf("No se encontraron ciudades guardadas.\n");
    } else {
        printf("Se cargaron %d ciudades guardadas.\n", sistema->num_ciudades);
    }
}

// Función para agregar una nueva ciudad al sistema
void agregar_ciudad(struct SistemaMonitoreo *sistema, struct Limites limites) {
    if (sistema->num_ciudades >= MAX_CIUDADES) {
        printf("Error: Se ha alcanzado el limite maximo de ciudades (%d).\n", MAX_CIUDADES);
        return;
    }
    
    struct Ciudad *nueva_ciudad = &sistema->ciudades[sistema->num_ciudades];
    
    // Inicializar la nueva ciudad
    inicializar_ciudad(nueva_ciudad);
    
    // Ingresar datos para cada zona
    for (int i = 0; i < nueva_ciudad->num_zonas; i++) {
        ingresar_datos_zona(&nueva_ciudad->zonas[i]);
        evaluar_niveles_contaminacion(&nueva_ciudad->zonas[i], limites);
        simular_datos_historicos(&nueva_ciudad->zonas[i], limites, i);
        calcular_promedios_historicos(&nueva_ciudad->zonas[i]);
        predecir_niveles(&nueva_ciudad->zonas[i]);
        generar_recomendaciones(&nueva_ciudad->zonas[i], limites);
    }
    
    sistema->num_ciudades++;
    printf("\nCiudad '%s' agregada correctamente al sistema.\n", nueva_ciudad->nombre);
    
    // Guardar las ciudades después de agregar una nueva
    guardar_ciudades(*sistema);
}

// Función para seleccionar una ciudad del sistema
int seleccionar_ciudad(struct SistemaMonitoreo sistema) {
    int seleccion;
    
    if (sistema.num_ciudades == 0) {
        printf("No hay ciudades registradas en el sistema.\n");
        return -1;
    }
    
    printf("\n------------------------------------------------\n");
    printf("             CIUDADES DISPONIBLES                 \n");
    printf("--------------------------------------------------\n\n");
    
    for (int i = 0; i < sistema.num_ciudades; i++) {
        printf(" [%d] %s (%d zonas)\n", i + 1, sistema.ciudades[i].nombre, sistema.ciudades[i].num_zonas);
    }
    
    printf("\nSeleccione una ciudad (1-%d): ", sistema.num_ciudades);
    scanf("%d", &seleccion);
    
    if (seleccion < 1 || seleccion > sistema.num_ciudades) {
        printf("Seleccion invalida.\n");
        return -1;
    }
    
    return seleccion - 1; // Restar 1 para obtener el índice real
}

// Función para guardar todas las ciudades en un archivo binario
void guardar_ciudades(struct SistemaMonitoreo sistema) {
    FILE *archivo = fopen("ciudades.dat", "wb");
    if (archivo == NULL) {
        printf("Error al abrir el archivo para guardar ciudades.\n");
        return;
    }
    
    // Guardar el número de ciudades
    fwrite(&sistema.num_ciudades, sizeof(int), 1, archivo);
    
    // Guardar cada ciudad
    for (int i = 0; i < sistema.num_ciudades; i++) {
        fwrite(&sistema.ciudades[i], sizeof(struct Ciudad), 1, archivo);
    }
    
    fclose(archivo);
}

// Función para cargar ciudades desde un archivo binario
int cargar_ciudades(struct SistemaMonitoreo *sistema) {
    FILE *archivo = fopen("ciudades.dat", "rb");
    if (archivo == NULL) {
        return 0; // No se pudo abrir el archivo
    }
    
    // Leer el número de ciudades
    if (fread(&sistema->num_ciudades, sizeof(int), 1, archivo) != 1) {
        fclose(archivo);
        return 0;
    }
    
    // Verificar que el número de ciudades sea válido
    if (sistema->num_ciudades <= 0 || sistema->num_ciudades > MAX_CIUDADES) {
        fclose(archivo);
        return 0;
    }
    
    // Leer cada ciudad
    for (int i = 0; i < sistema->num_ciudades; i++) {
        if (fread(&sistema->ciudades[i], sizeof(struct Ciudad), 1, archivo) != 1) {
            fclose(archivo);
            sistema->num_ciudades = i; // Actualizar con las ciudades que se pudieron leer
            return i > 0; // Devolver éxito si se leyó al menos una ciudad
        }
    }
    
    fclose(archivo);
    return 1; // Éxito
}
