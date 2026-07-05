#include "funciones.h"
#include <stdio.h>

int main() {
    struct SistemaMonitoreo sistema;
    struct Limites limites;
    int opcion_limites;
    int opcion_menu;
    int ciudad_seleccionada;

    // Limpiar pantalla inicial
    limpiar_pantalla();

    // Banner de bienvenida
    printf("\n---------------------------------------------------\n");
    printf("     SISTEMA DE MONITOREO Y PREDICCION DE          \n");
    printf("   CONTAMINACION DEL AIRE PARA CIUDADES DE ECUADOR \n");
    printf(" AUTORES: NICKOLAS LANDETA, JUAN MOLINA, ARIANA RAMOS\n");
    printf("             MATERIA: PROGRAMACION I                \n");
    printf("             PROFESOR: CARLOS GUAITA                \n");
    printf("-----------------------------------------------------\n\n");

    // Inicializar límites de contaminación
    printf("Seleccione el tipo de limites de contaminacion a utilizar:\n");
    printf("1. Utilizar limites establecidos por la OMS\n");
    printf("2. Ingresar limites personalizados\n");
    printf("Seleccione una opcion: ");
    scanf("%d", &opcion_limites);

    if (opcion_limites == 1) {
        // Intentar cargar desde archivo, si no existe, usar valores predeterminados
        if (!cargar_limites_desde_archivo(&limites)) {
            cargar_limites_oms(&limites);
        } else {
            printf("Limites de la OMS cargados correctamente desde archivo.\n");
        }
    } else {
        ingresar_limites_personalizados(&limites);
    }

    // Inicializar el sistema de monitoreo
    inicializar_sistema(&sistema);

    // Si no hay ciudades guardadas, pedir al usuario que agregue una
    if (sistema.num_ciudades == 0) {
        printf("No hay ciudades registradas. Vamos a agregar una ciudad.\n");
        agregar_ciudad(&sistema, limites);
    }

    // Menú principal
    do {
        opcion_menu = menu_principal();

        // Para opciones que requieren una ciudad, primero seleccionarla
        if (opcion_menu >= 1 && opcion_menu <= 6) {
            ciudad_seleccionada = seleccionar_ciudad(sistema);
            if (ciudad_seleccionada == -1) {
                pausar();
                continue; // Volver al menú principal
            }
        }

        switch (opcion_menu) {
            case 1:
                mostrar_estado_actual(sistema.ciudades[ciudad_seleccionada], limites);
                pausar();
                break;

            case 2:
                mostrar_predicciones(sistema.ciudades[ciudad_seleccionada]);
                pausar();
                break;

            case 3:
                mostrar_promedios_historicos(sistema.ciudades[ciudad_seleccionada], limites);
                pausar();
                break;

            case 4:
                mostrar_recomendaciones(sistema.ciudades[ciudad_seleccionada]);
                pausar();
                break;

            case 5:
                // Actualizar datos para una zona específica
                int num_zona;
                printf("\nIngrese el numero de la zona a actualizar (1-%d): ", 
                       sistema.ciudades[ciudad_seleccionada].num_zonas);
                scanf("%d", &num_zona);

                if (num_zona >= 1 && num_zona <= sistema.ciudades[ciudad_seleccionada].num_zonas) {
                    ingresar_datos_zona(&sistema.ciudades[ciudad_seleccionada].zonas[num_zona-1]);
                    evaluar_niveles_contaminacion(&sistema.ciudades[ciudad_seleccionada].zonas[num_zona-1], limites);
                    predecir_niveles(&sistema.ciudades[ciudad_seleccionada].zonas[num_zona-1]);
                    generar_recomendaciones(&sistema.ciudades[ciudad_seleccionada].zonas[num_zona-1], limites);
                    printf("\nDatos de %s actualizados correctamente.\n", 
                           sistema.ciudades[ciudad_seleccionada].zonas[num_zona-1].nombre);

                    // Guardar los cambios en el archivo
                    guardar_ciudades(sistema);
                } else {
                    printf("\nNumero de zona invalido.\n");
                }
                pausar();
                break;

            case 6:
                exportar_informacion(sistema.ciudades[ciudad_seleccionada], limites);
                pausar();
                break;

            case 7:
                agregar_ciudad(&sistema, limites);
                pausar();
                break;

            case 0:
                // Guardar los datos de las ciudades antes de salir
                guardar_ciudades(sistema);
                printf("\n¡Gracias por utilizar el Sistema de Monitoreo de Contaminacion!\n");
                break;

            default:
                printf("\nOpcion no valida. Por favor, intente nuevamente.\n");
                pausar();
        }

    } while (opcion_menu != 0);

    return 0;
}
