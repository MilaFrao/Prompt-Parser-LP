//------------------------------------------ Segunda versión del Prompt Parser en C ---------------------------------------------------------
/*

Lexer + Parser LL(1) con listas de sinónimos por clave (arrays const char*)
- Soporta 4 perfiles: BRILLA, AVANZA, CREAR, FLUYE
- Cada clave puede ser cualquiera de los sinónimos definidos en los arrays
- advance() ignora líneas vacías; se valida texto no vacío
Compilar:
gcc PromptParser.c -o PromptParser
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <direct.h>   // _getcwd en Windows

/* ---------------------- Tipos y constantes ---------------------- */

typedef enum {
    TOKEN_CLAVE,
    TOKEN_NEWLINE,
    TOKEN_EOF,
    TOKEN_ERROR
} TokenType;

typedef struct {
    TokenType type;
    char key[64];
    char text[256];
    int line;
} Token;

/* Estado del lexer */
static FILE *g_file = NULL;
static int g_line = 0;
static char buffer[512];

/* Lookahead del parser */
static Token lookahead;

/* ---------------------- Sinónimos (arrays) ---------------------- */
/* BRILLA */
static const char *CLAVES_BREVE_CONTEXTO[] = {
    "Breve_contexto","Contexto_breve","Resumen_contexto",
    "Situacion_inicial","Descripcion_corta","Panorama_general",
    "Introduccion_breve"
};
static const int N_CLAVES_BREVE_CONTEXTO = sizeof(CLAVES_BREVE_CONTEXTO)/sizeof(CLAVES_BREVE_CONTEXTO[0]);

static const char *CLAVES_ROL_DESEADO[] = {
    "Rol_deseado","Rol_objetivo","Funcion_esperada",
    "Posicion_asumida","Identidad_del_rol","Papel_a_cumplir",
    "Perspectiva_del_rol"
};
static const int N_CLAVES_ROL_DESEADO = sizeof(CLAVES_ROL_DESEADO)/sizeof(CLAVES_ROL_DESEADO[0]);

static const char *CLAVES_INTENCION_META[] = {
    "Intencion_meta","Meta_principal","Objetivo_central",
    "Proposito_final","Intencion_clave","Resultado_esperado",
    "Meta_del_prompt"
};
static const int N_CLAVES_INTENCION_META = sizeof(CLAVES_INTENCION_META)/sizeof(CLAVES_INTENCION_META[0]);

static const char *CLAVES_LENGUAJE_TONO[] = {
    "Lenguaje_tono","Tono_estilo","Estilo_de_redaccion",
    "Registro_linguistico","Manera_de_hablar","Tono_deseado",
    "Estilo_comunicativo"
};
static const int N_CLAVES_LENGUAJE_TONO = sizeof(CLAVES_LENGUAJE_TONO)/sizeof(CLAVES_LENGUAJE_TONO[0]);

static const char *CLAVES_LOGICA_ESTRUCTURA[] = {
    "Logica_estructura","Estructura_logica","Organizacion_del_contenido",
    "Secuencia_del_mensaje","Esquema_de_redaccion","Metodo_expositivo",
    "Flujo_de_informacion"
};
static const int N_CLAVES_LOGICA_ESTRUCTURA = sizeof(CLAVES_LOGICA_ESTRUCTURA)/sizeof(CLAVES_LOGICA_ESTRUCTURA[0]);

static const char *CLAVES_ASPECTO_RESULTADO[] = {
    "Aspecto_resultado","Forma_del_resultado","Presentacion_final",
    "Tipo_de_salida","Formato_esperado","Especificacion_del_resultado",
    "Definicion_del_output"
};
static const int N_CLAVES_ASPECTO_RESULTADO = sizeof(CLAVES_ASPECTO_RESULTADO)/sizeof(CLAVES_ASPECTO_RESULTADO[0]);

/* AVANZA */
static const char *CLAVES_ANTECEDENTES[] = {
    "Antecedentes","Contexto_previos","Historia_breve",
    "Datos_previos","Informacion_anterior","Situacion_previa",
    "Marco_de_referencia"
};
static const int N_CLAVES_ANTECEDENTES = sizeof(CLAVES_ANTECEDENTES)/sizeof(CLAVES_ANTECEDENTES[0]);

static const char *CLAVES_VISION_PROPOSITO[] = {
    "Vision_proposito","Proposito_general","Vision_del_objetivo",
    "Objetivo_macro","Direccion_deseada","Meta_amplia","Intencion_global"
};
static const int N_CLAVES_VISION_PROPOSITO = sizeof(CLAVES_VISION_PROPOSITO)/sizeof(CLAVES_VISION_PROPOSITO[0]);

static const char *CLAVES_ACCION_CONCRETA[] = {
    "Accion_concreta","Accion_especifica","Tarea_directa",
    "Paso_principal","Actividad_clave","Instruccion_central",
    "Movimiento_requerido"
};
static const int N_CLAVES_ACCION_CONCRETA = sizeof(CLAVES_ACCION_CONCRETA)/sizeof(CLAVES_ACCION_CONCRETA[0]);

static const char *CLAVES_NIVEL_DETALLE[] = {
    "Nivel_detalle","Grado_de_detalle","Profundidad_deseada",
    "Extension_explicativa","Detallado_o_resumido","Especificidad_requerida",
    "Amplitud_de_descripcion"
};
static const int N_CLAVES_NIVEL_DETALLE = sizeof(CLAVES_NIVEL_DETALLE)/sizeof(CLAVES_NIVEL_DETALLE[0]);

static const char *CLAVES_ZONA_ENFOQUE[] = {
    "Zona_enfoque","Area_de_atencion","Punto_central",
    "Foco_tematico","Region_del_problema","Enfoque_principal",
    "Aspecto_critico"
};
static const int N_CLAVES_ZONA_ENFOQUE = sizeof(CLAVES_ZONA_ENFOQUE)/sizeof(CLAVES_ZONA_ENFOQUE[0]);

static const char *CLAVES_ASPECTO_VISUAL[] = {
    "Aspecto_visual","Presentacion_visual","Formato_grafico",
    "Estilo_de_visualizacion","Apariencia_general","Diseno_de_salida",
    "Imagen_esperada"
};
static const int N_CLAVES_ASPECTO_VISUAL = sizeof(CLAVES_ASPECTO_VISUAL)/sizeof(CLAVES_ASPECTO_VISUAL[0]);

/* CREAR */
static const char *CLAVES_CONTEXTO[] = {
    "Contexto","Situacion","Problema",
    "Tema","Descripcion_inicial","Marco_referencial","Escenario"
};
static const int N_CLAVES_CONTEXTO = sizeof(CLAVES_CONTEXTO)/sizeof(CLAVES_CONTEXTO[0]);

static const char *CLAVES_ROL[] = {
    "Rol","Papel","Funcion","Identidad","Perspectiva",
    "Posicion","Actuacion_estimada"
};
static const int N_CLAVES_ROL = sizeof(CLAVES_ROL)/sizeof(CLAVES_ROL[0]);

static const char *CLAVES_ENFOQUE[] = {
    "Enfoque","Perspectiva","Direccion","Orientacion",
    "Vision","Punto_de_vista","Linea_argumental"
};
static const int N_CLAVES_ENFOQUE = sizeof(CLAVES_ENFOQUE)/sizeof(CLAVES_ENFOQUE[0]);

static const char *CLAVES_ACCION[] = {
    "Accion","Tarea","Paso","Movimiento","Actividad",
    "Instruccion","Operacion"
};
static const int N_CLAVES_ACCION = sizeof(CLAVES_ACCION)/sizeof(CLAVES_ACCION[0]);

static const char *CLAVES_RESULTADO[] = {
    "Resultado","Meta","Objetivo","Salida_esperada",
    "Producto_final","Desenlace","Efecto_deseado"
};
static const int N_CLAVES_RESULTADO = sizeof(CLAVES_RESULTADO)/sizeof(CLAVES_RESULTADO[0]);

/* FLUYE */
static const char *CLAVES_FOCO[] = {
    "Foco","Enfoque_principal","Punto_de_atencion",
    "Centro_del_mensaje","Nucleo_tematico","Aspecto_relevante",
    "Direccion_principal"
};
static const int N_CLAVES_FOCO = sizeof(CLAVES_FOCO)/sizeof(CLAVES_FOCO[0]);

static const char *CLAVES_LUGAR[] = {
    "Lugar","Ambiente","Escenario","Contexto_espacial",
    "Entorno","Espacio","Locacion"
};
static const int N_CLAVES_LUGAR = sizeof(CLAVES_LUGAR)/sizeof(CLAVES_LUGAR[0]);

static const char *CLAVES_USUARIO[] = {
    "Usuario","Persona_objetivo","Publico","Receptor",
    "Audiencia","Perfil_destino","Lector"
};
static const int N_CLAVES_USUARIO = sizeof(CLAVES_USUARIO)/sizeof(CLAVES_USUARIO[0]);

static const char *CLAVES_YO_INTERIOR[] = {
    "Yo_interior","Reflexion_personal","Estado_emocional",
    "Introspeccion","Autoanalisis","Pensamiento_interno",
    "Perspectiva_personal"
};
static const int N_CLAVES_YO_INTERIOR = sizeof(CLAVES_YO_INTERIOR)/sizeof(CLAVES_YO_INTERIOR[0]);

static const char *CLAVES_EJERCICIO[] = {
    "Ejercicio","Tarea_practica","Actividad","Accion_reflexiva",
    "Accion_a_realizar","Practica_orientada","Paso_de_aplicacion"
};
static const int N_CLAVES_EJERCICIO = sizeof(CLAVES_EJERCICIO)/sizeof(CLAVES_EJERCICIO[0]);

/* ---------------------- Utilidades de cadena ---------------------- */

static void trim_crlf(char *s) {
    int len = (int)strlen(s);
    while (len > 0 && (s[len-1] == '\n' || s[len-1] == '\r')) {
        s[len-1] = '\0';
        len--;
    }
}

static void trim_spaces_inplace(char *s) {
    int start = 0;
    while (s[start] && isspace((unsigned char)s[start])) start++;
    int end = (int)strlen(s) - 1;
    while (end >= start && isspace((unsigned char)s[end])) end--;
    int len = (end >= start) ? (end - start + 1) : 0;
    if (len > 0) memmove(s, s + start, len);
    s[len] = '\0';
}

static int is_text_empty(const char *s) {
    for (const unsigned char *p = (const unsigned char*)s; *p; ++p)
        if (!isspace(*p)) return 0;
    return 1;
}

/* ---------------------- Lexer ---------------------- */

void initLexer(FILE *f) {
    g_file = f;
    g_line = 0;
}

Token getNextToken() {
    Token tok;
    tok.type = TOKEN_ERROR;
    tok.key[0] = '\0';
    tok.text[0] = '\0';
    tok.line = g_line + 1;

    if (!g_file) return tok;

    if (!fgets(buffer, sizeof(buffer), g_file)) {
        tok.type = TOKEN_EOF;
        tok.line = g_line;
        return tok;
    }

    g_line++;
    tok.line = g_line;
    trim_crlf(buffer);

    int i = 0;
    while (buffer[i] && isspace((unsigned char)buffer[i])) i++;
    if (buffer[i] == '\0') {
        tok.type = TOKEN_NEWLINE;
        return tok;
    }

    char *colon = strchr(buffer, ':');
    if (!colon) {
        tok.type = TOKEN_ERROR;
        return tok;
    }

    int keylen = (int)(colon - buffer);
    if (keylen > (int)sizeof(tok.key) - 1) keylen = (int)sizeof(tok.key) - 1;
    strncpy(tok.key, buffer, keylen);
    tok.key[keylen] = '\0';
    trim_spaces_inplace(tok.key);

    char *txt = colon + 1;
    while (*txt && isspace((unsigned char)*txt)) txt++;
    strncpy(tok.text, txt, sizeof(tok.text) - 1);
    tok.text[sizeof(tok.text) - 1] = '\0';

    tok.type = TOKEN_CLAVE;
    return tok;
}

/* ---------------------- Parser utilidades ---------------------- */

/* advance: obtiene siguiente token y SKIP NEWLINE */
void advance() {
    do {
        lookahead = getNextToken();
    } while (lookahead.type == TOKEN_NEWLINE);
}

void error_general(const char *msg) {
    printf("Error en la linea %d: %s\n", lookahead.line, msg);
    if (lookahead.type == TOKEN_CLAVE)
        printf("  Clave encontrada: '%s'  Texto: '%s'\n", lookahead.key, lookahead.text);
    else if (lookahead.type == TOKEN_EOF)
        printf("  Se encontró EOF inesperado.\n");
    else if (lookahead.type == TOKEN_ERROR)
        printf("  Error léxico: falta ':' o línea mal formada.\n");
    exit(EXIT_FAILURE);
}

void error_clave_esperada(const char *esperada) {
    printf("Error en la linea %d:\n", lookahead.line);
    printf("  Se esperaba una clave del tipo: %s\n", esperada);
    if (lookahead.type == TOKEN_CLAVE)
        printf("  Se encontró: '%s'\n", lookahead.key);
    else if (lookahead.type == TOKEN_EOF)
        printf("  Se encontró EOF inesperado.\n");
    else
        printf("  Token no válido en lugar de clave.\n");
    exit(EXIT_FAILURE);
}

/* claveEnLista: busca exact match en lista (case-sensitive) */
int claveEnLista(const char *clave, const char *lista[], int n) {
    for (int i = 0; i < n; ++i) {
        if (strcmp(clave, lista[i]) == 0) return 1;
    }
    return 0;
}

/* esperarClaveEnLista: valida que lookahead sea una clave válida dentro de lista; valida texto no vacío y avanza */
void esperarClaveEnLista(const char *lista[], int n, const char *nombre_descriptivo) {
    if (lookahead.type == TOKEN_EOF) {
        error_clave_esperada(nombre_descriptivo);
    }
    if (lookahead.type != TOKEN_CLAVE) {
        error_general("Se esperaba una clave (token no es CLAVE).");
    }
    if (!claveEnLista(lookahead.key, lista, n)) {
        error_clave_esperada(nombre_descriptivo);
    }
    if (is_text_empty(lookahead.text)) {
        printf("Error semántico en la linea %d: La clave '%s' tiene texto vacío.\n", lookahead.line, lookahead.key);
        exit(EXIT_FAILURE);
    }
    advance();
}

/* ---------------------- Parsers por perfil (usando listas) ---------------------- */

void parserBRILLA() {
    esperarClaveEnLista(CLAVES_BREVE_CONTEXTO, N_CLAVES_BREVE_CONTEXTO, "Breve_contexto");
    esperarClaveEnLista(CLAVES_ROL_DESEADO, N_CLAVES_ROL_DESEADO, "Rol_deseado");
    esperarClaveEnLista(CLAVES_INTENCION_META, N_CLAVES_INTENCION_META, "Intencion_meta");
    esperarClaveEnLista(CLAVES_LENGUAJE_TONO, N_CLAVES_LENGUAJE_TONO, "Lenguaje_tono");
    esperarClaveEnLista(CLAVES_LOGICA_ESTRUCTURA, N_CLAVES_LOGICA_ESTRUCTURA, "Logica_estructura");
    esperarClaveEnLista(CLAVES_ASPECTO_RESULTADO, N_CLAVES_ASPECTO_RESULTADO, "Aspecto_resultado");
}

void parserAVANZA() {
    esperarClaveEnLista(CLAVES_ANTECEDENTES, N_CLAVES_ANTECEDENTES, "Antecedentes");
    esperarClaveEnLista(CLAVES_VISION_PROPOSITO, N_CLAVES_VISION_PROPOSITO, "Vision_proposito");
    esperarClaveEnLista(CLAVES_ACCION_CONCRETA, N_CLAVES_ACCION_CONCRETA, "Accion_concreta");
    esperarClaveEnLista(CLAVES_NIVEL_DETALLE, N_CLAVES_NIVEL_DETALLE, "Nivel_detalle");
    esperarClaveEnLista(CLAVES_ZONA_ENFOQUE, N_CLAVES_ZONA_ENFOQUE, "Zona_enfoque");
    esperarClaveEnLista(CLAVES_ASPECTO_VISUAL, N_CLAVES_ASPECTO_VISUAL, "Aspecto_visual");
}

void parserCREAR() {
    esperarClaveEnLista(CLAVES_CONTEXTO, N_CLAVES_CONTEXTO, "Contexto");
    esperarClaveEnLista(CLAVES_ROL, N_CLAVES_ROL, "Rol");
    esperarClaveEnLista(CLAVES_ENFOQUE, N_CLAVES_ENFOQUE, "Enfoque");
    esperarClaveEnLista(CLAVES_ACCION, N_CLAVES_ACCION, "Accion");
    esperarClaveEnLista(CLAVES_RESULTADO, N_CLAVES_RESULTADO, "Resultado");
}

void parserFLUYE() {
    esperarClaveEnLista(CLAVES_FOCO, N_CLAVES_FOCO, "Foco");
    esperarClaveEnLista(CLAVES_LUGAR, N_CLAVES_LUGAR, "Lugar");
    esperarClaveEnLista(CLAVES_USUARIO, N_CLAVES_USUARIO, "Usuario");
    esperarClaveEnLista(CLAVES_YO_INTERIOR, N_CLAVES_YO_INTERIOR, "Yo_interior");
    esperarClaveEnLista(CLAVES_EJERCICIO, N_CLAVES_EJERCICIO, "Ejercicio");
}

/* ---------------------- Regla raíz ---------------------- */

void parsePrompt(char *tipo_detectado, size_t tipo_len) {
    advance(); /* primer token útil */

    if (lookahead.type != TOKEN_CLAVE) error_general("Se esperaba una clave inicial");

    if (claveEnLista(lookahead.key, CLAVES_BREVE_CONTEXTO, N_CLAVES_BREVE_CONTEXTO)) {
        strncpy(tipo_detectado, "BRILLA", tipo_len-1); tipo_detectado[tipo_len-1] = '\0';
        parserBRILLA();
    }
    else if (claveEnLista(lookahead.key, CLAVES_ANTECEDENTES, N_CLAVES_ANTECEDENTES)) {
        strncpy(tipo_detectado, "AVANZA", tipo_len-1); tipo_detectado[tipo_len-1] = '\0';
        parserAVANZA();
    }
    else if (claveEnLista(lookahead.key, CLAVES_CONTEXTO, N_CLAVES_CONTEXTO)) {
        strncpy(tipo_detectado, "CREAR", tipo_len-1); tipo_detectado[tipo_len-1] = '\0';
        parserCREAR();
    }
    else if (claveEnLista(lookahead.key, CLAVES_FOCO, N_CLAVES_FOCO)) {
        strncpy(tipo_detectado, "FLUYE", tipo_len-1); tipo_detectado[tipo_len-1] = '\0';
        parserFLUYE();
    }
    else {
        error_clave_esperada("Breve_contexto | Antecedentes | Contexto | Foco (o sus sinónimos)");
    }

    if (lookahead.type != TOKEN_EOF) {
        error_general("Texto extra después del final del prompt");
    }
}

/* ---------------------- main ---------------------- */

int main(int argc, char *argv[]) {
    char cwd[512];
    _getcwd(cwd, sizeof(cwd));
    printf("Directorio de trabajo actual: %s\n", cwd);

    FILE *f = NULL;
    if (argc == 2) {
        f = fopen(argv[1], "r");
    } else {
        /* Ajusta la ruta por defecto según tu entorno */
        f = fopen("C:\\Users\\HP\\Documents\\Workspace\\Lenguajes de Programacion\\Prompt-Parser-LP\\Prompt.pmt", "r");
        printf("Usando archivo por defecto: Prompt.pmt\n");
    }

    if (!f) {
        perror("No se pudo abrir el archivo");
        return 1;
    }

    initLexer(f);

    char tipo[16] = {0};
    parsePrompt(tipo, sizeof(tipo));
    printf("Analisis completado sin errores. Tipo de prompt: %s\n", tipo);

    fclose(f);
    return 0;
}
