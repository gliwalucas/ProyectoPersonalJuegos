#include <stdio.h>
#include <string.h>
#include "FormatoExcel.h"

void crear_excel_desde_csv(const char *csv_file, const char *excel_file) {
    lxw_workbook *workbook = workbook_new(excel_file);
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, "Ranking");

    // ===== FORMATOS EXACTOS (igual que antes) =====

    // 1. TÍTULO PRINCIPAL
    lxw_format *fmt_titulo = workbook_add_format(workbook);
    //format_set_bold(fmt_titulo); Es ponerlo en negrita
    //format_set_font_size(fmt_titulo, 16); Tamanio letra
    format_set_align(fmt_titulo, LXW_ALIGN_CENTER); //Alineacion horizontal a centro
    format_set_align(fmt_titulo, LXW_ALIGN_VERTICAL_CENTER); //vertical
    format_set_bg_color(fmt_titulo, 0xFFCC99); //Color Fondo
    format_set_border(fmt_titulo, LXW_BORDER_MEDIUM); //marco grueso negro
    format_set_border_color(fmt_titulo, LXW_COLOR_BLACK); //color marco

    // 2. FORMATO FILA 2 - Col C y H
    lxw_format *fmt_fila2_especial = workbook_add_format(workbook);
    format_set_font_color(fmt_fila2_especial, LXW_COLOR_RED);
    format_set_bg_color(fmt_fila2_especial, 0x202020);
    format_set_border(fmt_fila2_especial, LXW_BORDER_MEDIUM);
    format_set_border_color(fmt_fila2_especial, LXW_COLOR_BLACK);

    format_set_align(fmt_fila2_especial, LXW_ALIGN_CENTER);
    format_set_align(fmt_fila2_especial, LXW_ALIGN_VERTICAL_CENTER);

    // 3. FORMATO NORMAL (Resto de fila 2)          MODIFICAR VARIABLE A FMT_NORMAL
    lxw_format *fmt_normalDos = workbook_add_format(workbook);
    format_set_align(fmt_normalDos, LXW_ALIGN_CENTER);
    format_set_border(fmt_normalDos, LXW_BORDER_MEDIUM);
    format_set_border_color(fmt_normalDos, LXW_COLOR_BLACK);

    // 3. FORMATO TOP (top fila 3)              MODIFICAR VARIABLE A FMT_TOP
    lxw_format *fmt_normal = workbook_add_format(workbook);
    format_set_align(fmt_normal, LXW_ALIGN_CENTER);
    format_set_border(fmt_normal, LXW_BORDER_MEDIUM);
    format_set_border_color(fmt_normal, LXW_COLOR_BLACK);
    format_set_bg_color(fmt_normal, 0xFFFF99);

    // 4. ENCABEZADOS TOP(NUMERO 1 2 3 FILA 4)     MODIFICAR VARIABLE A FMT_ENCABEZADO
    lxw_format *fmt_top = workbook_add_format(workbook);
    format_set_bold(fmt_top);
    format_set_align(fmt_top, LXW_ALIGN_CENTER);
    format_set_align(fmt_top, LXW_ALIGN_VERTICAL_CENTER);
    format_set_border(fmt_top, LXW_BORDER_MEDIUM);
    format_set_border_color(fmt_top, LXW_COLOR_BLACK);

    // 5. FÓRMULAS DE IMÁGENES
    lxw_format *fmt_imagen = workbook_add_format(workbook);
    format_set_align(fmt_imagen, LXW_ALIGN_CENTER);
    format_set_align(fmt_imagen, LXW_ALIGN_VERTICAL_CENTER);
    format_set_border(fmt_imagen, LXW_BORDER_THIN);

    format_set_bg_color(fmt_imagen, 0x404040);

    // 6. ENCABEZADOS TABLA
    lxw_format *fmt_header_tabla = workbook_add_format(workbook);
    format_set_bold(fmt_header_tabla);
    format_set_align(fmt_header_tabla, LXW_ALIGN_CENTER);
    format_set_bg_color(fmt_header_tabla, 0x808080);
    format_set_font_color(fmt_header_tabla, LXW_COLOR_WHITE);
    format_set_border(fmt_header_tabla, LXW_BORDER_THIN);

    // 7. DATOS TABLA
    lxw_format *fmt_datos_tabla = workbook_add_format(workbook);
    format_set_border(fmt_datos_tabla, LXW_BORDER_THIN);

    format_set_align(fmt_datos_tabla, LXW_ALIGN_CENTER);

    // ===== ANCHOS DE COLUMNA EXACTOS =====
    worksheet_set_column(worksheet, 0, 2, 40.0, NULL);   // A, B, C = 40.0
    worksheet_set_column(worksheet, 3, 3, 15.5, NULL);   // D = 15.5
    worksheet_set_column(worksheet, 4, 4, 17.5, NULL);   // E = 17.5
    worksheet_set_column(worksheet, 5, 5, 20.5, NULL);   // F = 20.5
    worksheet_set_column(worksheet, 6, 6, 81.5, NULL);   // G = 81.5
    worksheet_set_column(worksheet, 7, 7, 45.0, NULL);   // H = 45.0
    worksheet_set_column(worksheet, 8, 8, 20.0, NULL);   // I = 35.0

    // ===== ALTURAS DE FILA =====
    worksheet_set_row_pixels(worksheet, 4, 200, NULL);   // Fila 5 = 200px

    //---------------------------------------------YA ESTAN TODOS LOS FORMATOS DE CELDA, PASO A LOS FORMATOS DE CONDICIONALES ------------------------------------------------------------
    //NO ACEPTA FORMULAS EN ESPANIOL, COMO LOS ODIO
    //1-Regla Max Juego Puntuacion Personal
// ===== FORMATOS =====
lxw_format *fmt_max         = workbook_add_format(workbook);
lxw_format *fmt_menor5      = workbook_add_format(workbook);
lxw_format *fmt_menor7      = workbook_add_format(workbook);
lxw_format *fmt_menor8      = workbook_add_format(workbook);
lxw_format *fmt_menor9      = workbook_add_format(workbook);
lxw_format *fmt_mayor9      = workbook_add_format(workbook);
lxw_format *fmt_sinDefinir  = workbook_add_format(workbook);
lxw_format *fmt_sinPuntaje  = workbook_add_format(workbook);
lxw_format *fmt_rejugar     = workbook_add_format(workbook);
lxw_format *fmt_NOrejugar   = workbook_add_format(workbook);

// Colores
format_set_bg_color(fmt_max,        0xADD8E6);
format_set_bg_color(fmt_menor5,     0xFF0000);
format_set_bg_color(fmt_menor7,     0xFFCCCC);
format_set_bg_color(fmt_menor8,     0xFFFF99);
format_set_bg_color(fmt_menor9,     0xCCFFCC);
format_set_bg_color(fmt_mayor9,     0x99FF99);
format_set_bg_color(fmt_sinDefinir, 0x9900FF);
format_set_bg_color(fmt_sinPuntaje, 0x202020);
format_set_font_color(fmt_rejugar, LXW_COLOR_GREEN);
format_set_font_color(fmt_NOrejugar, LXW_COLOR_RED);

lxw_conditional_format cond_max        = {0};
lxw_conditional_format cond_menor5     = {0};
lxw_conditional_format cond_menor7     = {0};
lxw_conditional_format cond_menor8     = {0};
lxw_conditional_format cond_menor9     = {0};
lxw_conditional_format cond_mayor9     = {0};
lxw_conditional_format cond_sinDefinir = {0};
lxw_conditional_format cond_sinPuntaje = {0};
lxw_conditional_format cond_rejugar    = {0};
lxw_conditional_format cond_NOrejugar  = {0};

// MAX
cond_max.type         = LXW_CONDITIONAL_TYPE_FORMULA;
cond_max.value_string= "=$E7=MAX($E$7:$E$156)";
cond_max.format      = fmt_max;

// <5
cond_menor5.type         = LXW_CONDITIONAL_TYPE_FORMULA;
cond_menor5.value_string= "=AND($E7<5,$E7<>0)";
cond_menor5.format      = fmt_menor5;

// <7
cond_menor7.type         = LXW_CONDITIONAL_TYPE_FORMULA;
cond_menor7.value_string= "=AND($E7>=5,$E7<7)";
cond_menor7.format      = fmt_menor7;

// <8
cond_menor8.type         = LXW_CONDITIONAL_TYPE_FORMULA;
cond_menor8.value_string= "=AND($E7>=7,$E7<8)";
cond_menor8.format      = fmt_menor8;

// <9
cond_menor9.type         = LXW_CONDITIONAL_TYPE_FORMULA;
cond_menor9.value_string= "=AND($E7>=8,$E7<9)";
cond_menor9.format      = fmt_menor9;

// >=9
cond_mayor9.type         = LXW_CONDITIONAL_TYPE_FORMULA;
cond_mayor9.value_string= "=$E7>=9";
cond_mayor9.format      = fmt_mayor9;

// Sin definir
cond_sinDefinir.type         = LXW_CONDITIONAL_TYPE_FORMULA;
cond_sinDefinir.value_string= "=AND($D7=0,$E7=0)";
cond_sinDefinir.format      = fmt_sinDefinir;

// Sin puntaje
cond_sinPuntaje.type         = LXW_CONDITIONAL_TYPE_FORMULA;
cond_sinPuntaje.value_string= "=OR(ISBLANK($D7),ISBLANK($E7))";
cond_sinPuntaje.format      = fmt_sinPuntaje;

// Rejugar (C **DEBE** ser fecha real)
cond_rejugar.type         = LXW_CONDITIONAL_TYPE_FORMULA;
cond_rejugar.value_string= "=$C7<TODAY()-3600";
cond_rejugar.format      = fmt_rejugar;

// No rejugar
cond_NOrejugar.type         = LXW_CONDITIONAL_TYPE_FORMULA;
cond_NOrejugar.value_string= "=$C7>=TODAY()-3600";
cond_NOrejugar.format      = fmt_NOrejugar;
    //PROBLEMA NUEVO ENCONTRADO, LOS VALORES NO LLEGAN CON SU FORMATO DATE DECIMAL SEGUN EL TIPO DE DATO
    //SOLUCION: MAS FORMATOS
    lxw_format *fmt_fecha = workbook_add_format(workbook);
    format_set_num_format(fmt_fecha, "yyyy-mm-dd"); // o "dd/mm/yyyy"
    format_set_border(fmt_fecha, LXW_BORDER_THIN);
    format_set_align(fmt_fecha, LXW_ALIGN_CENTER);


    lxw_format *fmt_puntaje = workbook_add_format(workbook);
    format_set_num_format(fmt_puntaje, "0.0"); // 1 decimal
    format_set_border(fmt_fecha, LXW_BORDER_THIN);
    format_set_align(fmt_fecha, LXW_ALIGN_CENTER);

    // ===== LEER CSV Y ESCRIBIR EN EXCEL =====
    FILE *csv = fopen(csv_file, "r");
    if (!csv) {
        printf("Error: No se puede abrir %s\n", csv_file);
        workbook_close(workbook);
        return;
    }

    char buffer[1024];
    int excel_fila = 0;  // Fila en Excel (0-based)
    int last_row = 5;   // última fila con datos (empieza antes de la tabla)



    while (fgets(buffer, sizeof(buffer), csv)) {
        // Eliminar salto de línea
        buffer[strcspn(buffer, "\n")] = '\0';

        // Copiar para usar strtok
        char linea[1024];
        strcpy(linea, buffer);

    //Esto de abajo es extra para probar algo, si no funciona BORRRRRRRARRRRRRR
         // Limpiar comas al final para filas 1 y 3
    if (excel_fila == 0 || excel_fila == 2) {
        int len = strlen(linea);
        while (len > 0 && linea[len-1] == ',') {
            linea[len-1] = '\0';
            len--;
            }
        }


        // Procesar cada línea según su número
        switch (excel_fila) {
            case 0:  // Fila 1: Título
                worksheet_merge_range(worksheet, 0, 0, 0, 8, linea, fmt_titulo);
                break;

            case 1:  // Fila 2: Estadísticas
                procesar_fila_estadisticas(worksheet, linea, fmt_normalDos, fmt_fila2_especial);
                break;

            case 2:  // Fila 3: TOP
                //procesar_fila_instrucciones(worksheet, linea, fmt_normal);  BORRAR?
                worksheet_merge_range(worksheet, 2, 0, 2, 8, linea, fmt_normal);
                break;

            case 3:  // Fila 4: TOP Numero 1 numero 2 numero 3
                procesar_fila_top(worksheet, linea, fmt_top);
                break;

            case 4:  // Fila 5: Fórmulas de imágenes
                procesar_fila_imagenes(worksheet, linea, fmt_imagen);
                break;

            case 5:  // Fila 7: Encabezados tabla
                procesar_encabezados_tabla(worksheet, linea, fmt_header_tabla);
                break;

            default:  // Fila 8+: Datos de juegos
                if (excel_fila >= 6) {
                    procesar_fila_datos(worksheet, excel_fila, linea, fmt_datos_tabla,fmt_fecha);
                    last_row = excel_fila;   // <-- guardar última fila usada
                }
                break;
        }

        excel_fila++;
    }
    //IMPORTANTE, YA ESTA TODO CARGADO, LO QUE ME PERMITE SABER DONDE ESTA LA FILA FINAL
    //AHORA PUEDO APLICAR FORMATO TABLA
    //AHORA PUEDO APLICAR REGLAS PARA FORMATO CONDICIONAL
/*
    //REGLA 1
    worksheet_conditional_format_range(worksheet, 6, 4, 155, 4, cond_max); // E7:E156
    worksheet_conditional_format_range(worksheet, 6, 1, 155, 1, cond_max); // B7:B156
    //REGLA 2
    worksheet_conditional_format_range(worksheet, 6, 4, 155, 4, cond_menorCinco); // E7:E156
    worksheet_conditional_format_range(worksheet, 6, 1, 155, 1, cond_menorCinco); // B7:B156
    //REGLA 3
    worksheet_conditional_format_range(worksheet, 6, 4, 155, 4, cond_menorSiete); // E7:E156
    worksheet_conditional_format_range(worksheet, 6, 1, 155, 1, cond_menorSiete); // B7:B156
    //REGLA 4
    worksheet_conditional_format_range(worksheet, 6, 4, 155, 4, cond_menorOcho); // E7:E156
    worksheet_conditional_format_range(worksheet, 6, 1, 155, 1, cond_menorOcho); // B7:B156
    //REGLA 5
    worksheet_conditional_format_range(worksheet, 6, 4, 155, 4, cond_menorNueve); // E7:E156
    worksheet_conditional_format_range(worksheet, 6, 1, 155, 1, cond_menorNueve); // B7:B156
    //REGLA 6
    worksheet_conditional_format_range(worksheet, 6, 4, 155, 4, cond_mayorNueve); // E7:E156
    worksheet_conditional_format_range(worksheet, 6, 1, 155, 1, cond_mayorNueve); // B7:B156
    //REGLA 7
    worksheet_conditional_format_range(worksheet, 6, 1, 155, 1, cond_sinDefinir); // B7:B156
    //REGLA 8
    worksheet_conditional_format_range(worksheet, 6, 3, 155, 4, cond_sinPuntaje); // D7:E156
    worksheet_conditional_format_range(worksheet, 6, 1, 155, 1, cond_sinPuntaje); // B7:B156
    //REGLA 9
    worksheet_conditional_format_range(worksheet, 6, 7, 155, 7, cond_rejugar); // H7:H156
    //REGLA 10
    worksheet_conditional_format_range(worksheet, 6, 7, 155, 7, cond_Norejugar); // H7:H156
*/



int fila_ini = 6;
int fila_fin = last_row;

// === MAX ===
worksheet_conditional_format_range(worksheet, fila_ini, 1, fila_fin, 1, &cond_max);
worksheet_conditional_format_range(worksheet, fila_ini, 4, fila_fin, 4, &cond_max);

// === PUNTAJE ===
lxw_conditional_format *reglas[] = {
    &cond_menor5, &cond_menor7, &cond_menor8,
    &cond_menor9, &cond_mayor9,
    &cond_sinDefinir, &cond_sinPuntaje
};

for (int i = 0; i < 7; i++) {
    worksheet_conditional_format_range(worksheet, fila_ini, 1, fila_fin, 1, reglas[i]);
    worksheet_conditional_format_range(worksheet, fila_ini, 4, fila_fin, 4, reglas[i]);
}

// === REJUGAR ===
worksheet_conditional_format_range(worksheet, fila_ini, 7, fila_fin, 7, &cond_rejugar);
worksheet_conditional_format_range(worksheet, fila_ini, 7, fila_fin, 7, &cond_NOrejugar);
    //FORMATO TABLA
    worksheet_autofilter(worksheet, 5, 0, last_row, 8); //Como ya tengo todo cargado, detecta hasta la ultima fila con datos desde la fila 6 y le aplica formato tabla

    fclose(csv);
    workbook_close(workbook);

    printf("Excel creado desde CSV: %s\n", excel_file);
}

// ===== FUNCIONES AUXILIARES =====

void procesar_fila_estadisticas(lxw_worksheet *ws, char *linea,lxw_format *fmt_normalDos, lxw_format *fmt_especial) {
    // Separar por comas
    char *token = strtok(linea, ",");

    // Mapa: índice del token en CSV → columna de Excel
    int columnas_excel[] = {1, 2, 6, 7};  // B,C,G,H
    int csv_idx = 0;

    while (token != NULL) {
        if (strlen(token) > 0 && csv_idx < 4) {
            int col = columnas_excel[csv_idx];

            // Solo aplicar fmt_especial a C y H
            lxw_format *fmt = (col == 2 || col == 7) ? fmt_especial : fmt_normalDos;

            worksheet_write_string(ws, 1, col, token, fmt);
        }
        token = strtok(NULL, ",");
        csv_idx++;
    }
}



void procesar_fila_imagenes(lxw_worksheet *ws, char *linea, lxw_format *fmt) {
    char *token = strtok(linea, ",");
    int col = 0;

    while (token != NULL) {
        if (strlen(token) > 0) {
            // Si ya es una fórmula de Excel (=IMAGEN...), escribir tal cual
            if (token[0] == '=') {
                worksheet_write_formula(ws, 4, col, token, fmt);
            }
            // Si es una URL, crear fórmula
            else if (strstr(token, "http") == token) {
                char formula[512];
                sprintf(formula, "=IMAGEN(\"%s\",\"\",1)", token);
                worksheet_write_formula(ws, 4, col, formula, fmt);
            }
            // Texto normal
            else {
                worksheet_write_string(ws, 4, col, token, fmt);
            }
        } else {
            worksheet_write_blank(ws, 4, col, fmt);
        }

        token = strtok(NULL, ",");
        col++;
    }
}

void procesar_fila_datos(lxw_worksheet *ws, int excel_fila, char *linea, lxw_format *fmt,lxw_format *fmt_fecha) {
    char *token = strtok(linea, ",");
    int col = 0;

    while (token != NULL && col < 9) {
        if (strlen(token) > 0) {
            // Detectar tipo de dato
            if (col == 0 || col == 3 || col == 4) {  // ID, Puntajes (números)
                char *endptr;
                double num = strtod(token, &endptr);
                if (*endptr == '\0') {
                    worksheet_write_number(ws, excel_fila, col, num, fmt);
                } else {
                    worksheet_write_string(ws, excel_fila, col, token, fmt);
                }
            }
            else if (col == 2) {  // Fecha
                    int d, m, y;
                    if (sscanf(token, "%d/%d/%d", &d, &m, &y) == 3) {
                        lxw_datetime dt = {0};
                        dt.year = y;
                        dt.month = m;
                        dt.day = d;
                        worksheet_write_datetime(ws, excel_fila, col, &dt, fmt_fecha);
                    } else {
                        worksheet_write_string(ws, excel_fila, col, token, fmt);
                    }
               // worksheet_write_string(ws, excel_fila, col, token, fmt);
               /*lxw_datetime dt;
                sscanf(token, "%d-%d-%d", &dt.year, &dt.month, &dt.day);
                dt.hour = dt.min = dt.sec = 0.0;

                worksheet_write_datetime(ws, excel_fila, col, &dt, fmt_fecha);*/
            }
            else {  // Texto normal
                worksheet_write_string(ws, excel_fila, col, token, fmt);
            }
        } else {
            worksheet_write_blank(ws, excel_fila, col, fmt);
        }

        token = strtok(NULL, ",");
        col++;
    }
}

// Funciones simples para otras filas
void procesar_fila_instrucciones(lxw_worksheet *ws, char *linea, lxw_format *fmt) {
    char *token = strtok(linea, ",");
    int col = 0;
    while (token != NULL && col < 9) {
        if (strlen(token) > 0) {
            worksheet_write_string(ws, 2, col, token, fmt);
        }
        token = strtok(NULL, ",");
        col++;
    }
}

void procesar_fila_top(lxw_worksheet *ws, char *linea, lxw_format *fmt) {
    char *token = strtok(linea, ",");
    int col = 0;
    while (token != NULL && col < 9) {
        if (strlen(token) > 0) {
            worksheet_write_string(ws, 3, col, token, fmt);
        }
        token = strtok(NULL, ",");
        col++;
    }
}

void procesar_encabezados_tabla(lxw_worksheet *ws, char *linea, lxw_format *fmt) {
    char *token = strtok(linea, ",");
    int col = 0;
    while (token != NULL && col < 9) {
        if (strlen(token) > 0) {
            worksheet_write_string(ws, 5, col, token, fmt);
        }
        token = strtok(NULL, ",");
        col++;
    }
}




