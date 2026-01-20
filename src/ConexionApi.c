#include "ConexionApi.h"
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <direct.h>  // Para _getcwd en Windows

#ifdef _WIN32 //detecta si estas en windows
    #include <windows.h>  //Te permite usar Sleep
#else
    #include <unistd.h>  //Idem para Linux-Mac
#endif

//Funcion necesaria para el funcionamiento de CURL-API: ACUMULA LA RESPUESTA
size_t escribir_datos(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    HTTP_Response *resp = (HTTP_Response *)userp;

    char *ptr = realloc(resp->data, resp->size + realsize + 1);
    if (!ptr) return 0;

    resp->data = ptr;
    memcpy(&(resp->data[resp->size]), contents, realsize);
    resp->size += realsize;
    resp->data[resp->size] = '\0';

    return realsize;
}

//
char* llamar_rawg_api(const char *nombre_juego) {
    curl_global_init(CURL_GLOBAL_DEFAULT);
    CURL *curl;
    HTTP_Response respuesta = {0};

    // Verificar API key
    if (strlen(RAWG_API_KEY) < 10) {
        printf("ERROR: API key no configurada\n");
        return NULL;
    }

    curl = curl_easy_init(); //CREA OBJETO CURL
    if (!curl) {
        printf("ERROR: No se pudo inicializar CURL\n");
        return NULL;
    }

    // Construir URL
    char url[512];
    char nombre_escapado[256];
    int j = 0;

    // Regulacion de String recibido
    for (int i = 0; nombre_juego[i] && j < 255; i++) {
        if (nombre_juego[i] == ' ') {
            nombre_escapado[j++] = '%';
            nombre_escapado[j++] = '2';
            nombre_escapado[j++] = '0';
        } else {
            nombre_escapado[j++] = nombre_juego[i];
        }
    }
    nombre_escapado[j] = '\0';

    snprintf(url, sizeof(url), //Lo va a buscar con la API usando nombre del juego
             "https://api.rawg.io/api/games?key=%s&search=%s&page_size=5",
             RAWG_API_KEY, nombre_escapado);

    // Configurar CURL
    curl_easy_setopt(curl, CURLOPT_URL, url); //URL A LLAMAR
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, escribir_datos); //CALLBACK DE DATOS
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &respuesta); //DONDE GUARDA LOS DATOS
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L); //TIMEOUT

    // EXCLUSIVAS DE WINDOWS PARA EVITAR FALLO SSL
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

    // Ejecutar
    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        printf("ERROR API: %s\n", curl_easy_strerror(res));
        free(respuesta.data);
        respuesta.data = NULL;
    } else {
        // Verificar código HTTP
        long http_code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

        if (http_code != 200) {
            printf("ERROR HTTP: %ld\n", http_code);
            free(respuesta.data);
            respuesta.data = NULL;
        }
    }

    curl_easy_cleanup(curl); //LIBERA LA CONEXION
    curl_global_cleanup(); //LIBERA EL CURL
    return respuesta.data;
}

// ========== FUNCIONES LOGICAS DE API ==========

char* obtener_plataforma_api(const char *nombre_juego) {
    if (!nombre_juego || strlen(nombre_juego) == 0) {
        return strdup("Desconocida");
    }

    char *json = llamar_rawg_api(nombre_juego);
    if (!json) {
        return strdup("Desconocida");
    }

    // Buscar plataforma en el JSON
    char *plataforma = strdup("Desconocida");

    // Buscar "platforms":[{"platform":{"name":"..."}}]
    char *platforms_start = strstr(json, "\"platforms\":[");
    if (platforms_start) {
        char *name_start = strstr(platforms_start, "\"name\":\"");
        if (name_start) {
            name_start += 8;  // Saltar "\"name\":\""
            char *name_end = strchr(name_start, '"');
            if (name_end) {
                int len = name_end - name_start;
                free(plataforma); //libera "desconocida"
                plataforma = malloc(len + 1); //da la memoria necesaria para el string
                strncpy(plataforma, name_start, len);
                plataforma[len] = '\0';
            }
        }
    }

    free(json); //libera JSON
    return plataforma;
}




char* obtener_imagen_api(const char *nombre_juego) {
    if (!nombre_juego || strlen(nombre_juego) == 0) {
        return strdup("");
    }

    char *json = llamar_rawg_api(nombre_juego);
    if (!json) {
        return strdup("");
    }

    // Buscar "background_image":"URL"
    char *imagen = NULL;
    char *start = strstr(json, "\"background_image\":\"");

    if (start) {
        start += 20;  // Saltar "\"background_image\":\""
        char *end = strchr(start, '"');
        if (end) {
            int len = end - start;
            imagen = malloc(len + 1);
            strncpy(imagen, start, len); //IDEM DE LA ANTERIOR
            imagen[len] = '\0';
        }
    }

    free(json);

    if (!imagen || strlen(imagen) == 0) {
        free(imagen);
        return strdup("");
    }

    return imagen;
}





// ========== FUNCIÓN DE APLICACION API ==========
int actualizarPlataformaApi(Juego *juego) {
    if (!juego || strlen(juego->Nombre) == 0) {
        return 0;  // Error
    }

    printf("Consultando API para: %s\n", juego->Nombre);

    //Obtener plataforma
    char *plataforma_api = obtener_plataforma_api(juego->Nombre);
    if (plataforma_api && strlen(plataforma_api) > 0) {
        strncpy(juego->Plataforma, plataforma_api, 29);  // 30-1 para null terminator
        juego->Plataforma[29] = '\0';
        printf("  Plataforma: %s\n", juego->Plataforma);
    } else {
        strcpy(juego->Plataforma, "Desconocido");
    }
    free(plataforma_api);

    //Obtener imagen (FotoUrl)
    char *imagen_api = obtener_imagen_api(juego->Nombre);
    if (imagen_api && strlen(imagen_api) > 0) {
        strncpy(juego->FotoUrl, imagen_api, 799);  // 800-1 para null terminator
        juego->FotoUrl[799] = '\0';
        printf("  Imagen obtenida\n");
    } else {
        strcpy(juego->FotoUrl, "");
    }
    free(imagen_api);

    return 1;
}




void actualizar_db_con_api(sqlite3 *db) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT id_juego, nombre FROM juego WHERE plataforma IS NULL OR imagen_url IS NULL;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error al preparar consulta: %s\n", sqlite3_errmsg(db));
        return;
    }

    int actualizados = 0;
    printf("\n=== ACTUALIZANDO DATOS DESDE API ===\n");

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char *nombre = (const char*)sqlite3_column_text(stmt, 1);

        if (!nombre) continue;

        printf("\nProcesando: %s (ID: %d)\n", nombre, id);

        // Crear objeto Juego temporal
        Juego juego_temp;
        memset(&juego_temp, 0, sizeof(Juego));
        juego_temp.ID = id;
        strncpy(juego_temp.Nombre, nombre, 199);
        juego_temp.Nombre[199] = '\0';

        // Obtener datos de API
        if (actualizarPlataformaApi(&juego_temp)) {
            // Actualizar en DB
            const char *update_sql = "UPDATE juego SET plataforma = ?, imagen_url = ? WHERE id_juego = ?;";
            sqlite3_stmt *update_stmt;

            if (sqlite3_prepare_v2(db, update_sql, -1, &update_stmt, NULL) == SQLITE_OK) {
                sqlite3_bind_text(update_stmt, 1, juego_temp.Plataforma, -1, SQLITE_TRANSIENT);
                sqlite3_bind_text(update_stmt, 2, juego_temp.FotoUrl, -1, SQLITE_TRANSIENT);
                sqlite3_bind_int(update_stmt, 3, id);

                if (sqlite3_step(update_stmt) == SQLITE_DONE) {
                    actualizados++;
                    printf("Actualizado en DB\n");
                } else {
                    printf("Error al actualizar DB\n");
                }

                sqlite3_finalize(update_stmt);
            }
        }

        // Pequeña pausa para no saturar la API
        #ifdef _WIN32
            Sleep(1000);  // 200ms en Windows (EN TEORIA PUEDE SATURAR LA API Y BLOQUEARME EL PASO, LO AUMENTE A 1000ms)
        #else
            usleep(200000);  // 200ms en Linux/Mac
        #endif
    }

    sqlite3_finalize(stmt);
    printf("\n=== ACTUALIZACION COMPLETADA ===\n");
    printf("Juegos actualizados por API: %d\n", actualizados);
}






void escribir_formula_en_celda_corregida(const char *csv_file, int fila, int columna, const char *url) {
    printf("DEBUG: Escribiendo en [%d,%d]: %s\n", fila, columna, url);

    FILE *f = fopen(csv_file, "r");
    if (!f) {
        printf("ERROR: No existe %s\n", csv_file);
        return;
    }

    // Contar líneas
    char buffer[1024];
    int total_lineas = 0;
    while (fgets(buffer, sizeof(buffer), f)) total_lineas++;
    rewind(f);

    // Leer todas las líneas
    char **lineas = malloc(total_lineas * sizeof(char*));
    for (int i = 0; i < total_lineas; i++) {
        lineas[i] = malloc(1024);
        fgets(lineas[i], 1024, f);
        lineas[i][strcspn(lineas[i], "\n")] = '\0';
    }
    fclose(f);

    // 2. Asegurar que existe la fila
    if (fila > total_lineas) {
        lineas = realloc(lineas, fila * sizeof(char*));
        for (int i = total_lineas; i < fila; i++) {
            lineas[i] = malloc(1024);
            strcpy(lineas[i], "");
        }
        total_lineas = fila;
    }

    // 3. Modificar SOLO la celda específica
    int idx = fila - 1;
    char *linea = lineas[idx];

    // Convertir línea a array de campos
    char *campos[256] = {0};
    int num_campos = 0;

    if (strlen(linea) > 0) {
        // Copiar para usar strtok
        char linea_copia[1024];
        strcpy(linea_copia, linea);

        // Separar por comas
        char *token = strtok(linea_copia, ",");
        while (token != NULL && num_campos < 256) {
            campos[num_campos] = malloc(strlen(token) + 1);
            strcpy(campos[num_campos], token);
            num_campos++;
            token = strtok(NULL, ",");
        }
    }

    // Asegurar que tenemos campo para nuestra columna
 while (num_campos <= columna - 1) {
    campos[num_campos] = malloc(1);
    campos[num_campos][0] = '\0';
    num_campos++;
}

    // Crear fórmula
    char formula[256];
    sprintf(formula, "=IMAGEN(\"%s\")", url);

    // Reemplazar el campo específico
    free(campos[columna - 1]);
    campos[columna - 1] = malloc(strlen(formula) + 1);
    strcpy(campos[columna - 1], formula);

    // Reconstruir línea
    char nueva_linea[1024] = "";
    for (int i = 0; i < num_campos; i++) {
        if (i > 0) strcat(nueva_linea, ",");
        strcat(nueva_linea, campos[i]);
        free(campos[i]);
    }

    // 4. Reemplazar línea en el array
    free(lineas[idx]);
    lineas[idx] = malloc(strlen(nueva_linea) + 1);
    strcpy(lineas[idx], nueva_linea);

    // 5. Escribir todo de vuelta
    f = fopen(csv_file, "w");
    if (!f) {
        printf("ERROR: No se puede escribir\n");
        return;
    }

    for (int i = 0; i < total_lineas; i++) {
        fprintf(f, "%s\n", lineas[i]);
        free(lineas[i]);
    }
    free(lineas);
    fclose(f);

    printf("Formula escrita en fila %d, columna %d\n", fila, columna);
}





void actualizar_top3_con_formulas(const char *csv_file, sqlite3 *db) {
    printf("\n=== ACTUALIZANDO TOP 3 CON FORMULAS EXCEL ===\n");

    // 1. Obtener URLs del TOP 3
    char urls[3][256] = {0};
    int count = 0;

    sqlite3_stmt *stmt;
    const char *sql = "SELECT imagen_url FROM juego "
                      "WHERE puntaje_personal > 0 AND imagen_url IS NOT NULL "
                      "ORDER BY puntaje_personal DESC LIMIT 3;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error consultando TOP 3: %s\n", sqlite3_errmsg(db));
        return;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW && count < 3) {
        const char *url_db = (const char*)sqlite3_column_text(stmt, 0);
        if (url_db && strlen(url_db) > 10) {  // URL válida
            strncpy(urls[count], url_db, 255);
            printf("TOP %d: %s\n", count+1, urls[count]);
            count++;
        }
    }
    sqlite3_finalize(stmt);

    if (count == 0) {
        printf("No hay URLs disponibles para TOP 3\n");
        return;
    }

        if (count >= 1) {
            escribir_formula_en_celda_corregida(csv_file, TOP1_FILA, TOP1_COL, urls[0]);
        }
        if (count >= 2) {
            escribir_formula_en_celda_corregida(csv_file, TOP2_FILA, TOP2_COL, urls[1]);
        }
        if (count >= 3) {
            escribir_formula_en_celda_corregida(csv_file, TOP3_FILA, TOP3_COL, urls[2]);
        }
            printf("=== TOP 3 ACTUALIZADO (FORMULAS EXCEL) ===\n");
            printf("Abre el CSV en Excel para ver las imagenes automaticamente.\n");
}


// ==========Prueba de API ==========
void probar_api(void) {
    printf("\n=== PROBANDO API ===\n");

    char *plataforma = obtener_plataforma_api("Persona 3");
    if (plataforma) {
        printf("Plataforma para Persona 3: %s\n", plataforma);
        free(plataforma);
    }

    char *imagen = obtener_imagen_api("Persona 3");
    if (imagen) {
        printf("Imagen para Persona 3: %s\n", imagen);
        free(imagen);
    }

    printf("=== PRUEBA COMPLETADA ===\n\n");
}

// ==========La Funcion PRINCIPAL==========
void procesar_api_completo(sqlite3 *db, const char *csv_file) {

    actualizar_db_con_api(db); //guardo valores URL y PLATAFORMA en cada uno de los registros de la DB que lo tengan como NULL

    actualizar_top3_con_formulas(csv_file, db); //Busco TOP 3 por puntaje en la DB y me traigo su URL, luego la coloco en xy columna definida por MACRO para que aparezcan en el EXCEL como imagenes


    printf("========== PROCESAMIENTO COMPLETADO ==========\n\n");
}
