//------------------------------------------ Primera versión del Prompt Parser en C ---------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include<direct.h>


typedef enum
{  
    TOKEN_CLAVE,
    TOKEN_NEWLINE,
    TOKEN_EOF,
    TOKEN_ERROR
} TokenType;

typedef struct 
{
    TokenType type;
    char key[64];
    char text[256];
    int line;
} Token;

static FILE *g_file = NULL;
static int g_line = 0;
static char buffer[512];
static Token lookahead;

static void trim(char *s)
{
    int len = strlen(s);
    while (len > 0 && (s[len-1] == '\n' || s[len-1] == '\r'))
    {
        s[len-1] = '\0';
        len--;
    }
}

void initLexer(FILE *f)
{
    g_file = f;
    g_line = 0;
}

Token getNextToken()
{
    Token tok;
    tok.type = TOKEN_ERROR;
    tok.key[0] = '\0';
    tok.text[0] = '\0';
    tok.line = g_line + 1;

    if (!g_file) return tok;

    if(!fgets(buffer, sizeof(buffer), g_file))
    {
        tok.type = TOKEN_EOF;
        tok.line = g_line;
        return tok;
    }
    
    g_line++;
    tok.line = g_line;
    trim(buffer);

    int i = 0;
    while(buffer[i] && isspace((unsigned char)buffer[i])) i++;
    if (buffer[i] == '\0')
    {
        tok.type = TOKEN_NEWLINE;
        return tok;
    }

    char *colon=strchr(buffer, ':');
    if(!colon)
    {
        tok.type = TOKEN_ERROR;
        return tok;
    }

    int keylen = colon - buffer;
    if(keylen > 63) keylen = 63;
    strncpy(tok.key, buffer, keylen);
    tok.key[keylen] = '\0';

    int start = 0;
    while(tok.key[start] && isspace((unsigned char)tok.key[start])) start++;
    int end = strlen(tok.key) - 1;
    while(end >= start && isspace((unsigned char)tok.key[end])) end--;
    int len = (end >= start) ? (end - start + 1) : 0;
    memmove(tok.key, tok.key + start, len);
    tok.key[len] = '\0';

    char *txt = colon + 1;
    while (*txt == ' ') txt++;
    strncpy(tok.text, txt, 255);
    tok.text[255] = '\0';

    tok.type = TOKEN_CLAVE;
    return tok;
}

void advance()
{
    lookahead = getNextToken();
}

void error(const char *msg)
{
    printf("Error en la linea %d: %s (Token encontrado: '%s')\n", lookahead.line, msg, lookahead.key);
    exit(1);
}

void esperarClaveExacta(const char *k)
{
    if (lookahead.type != TOKEN_CLAVE)
        error("Se esperaba una clave");

    if (strcmp(lookahead.key, k) != 0)
        error(k);

    advance();
}

void parserBRILLA()
{
    esperarClaveExacta("Breve_contexto");  
    esperarClaveExacta("Rol_deseado");
    esperarClaveExacta("Intencion_meta");       
    esperarClaveExacta("Lenguaje_tono");          
    esperarClaveExacta("Logica_estructura");          
    esperarClaveExacta("Aspecto_resultado");                       
}

void parserAVANZA()
{
    esperarClaveExacta("Antecedentes");
    esperarClaveExacta("Vision_proposito");
    esperarClaveExacta("Accion_concreta");
    esperarClaveExacta("Nivel_detalle");
    esperarClaveExacta("Zona_enfoque");
    esperarClaveExacta("Aspecto_visual");
}

void parserCREAR()
{
    esperarClaveExacta("Contexto");
    esperarClaveExacta("Rol");
    esperarClaveExacta("Enfoque");
    esperarClaveExacta("Accion");
    esperarClaveExacta("Resultado");
}

void parserFLUYE()
{
    esperarClaveExacta("Foco");
    esperarClaveExacta("Lugar");
    esperarClaveExacta("Usuario");
    esperarClaveExacta("Yo_interior");
    esperarClaveExacta("Ejercicio");
}

void Promptparser()
{
    advance();
    if(lookahead.type != TOKEN_CLAVE) error("Se esperaba una clave inicial");

    if (strcmp(lookahead.key, "Breve_contexto") == 0)
        parserBRILLA();
    else if (strcmp(lookahead.key, "Antecedentes") == 0)
        parserAVANZA();
    else if (strcmp(lookahead.key, "Contexto") == 0)
        parserCREAR();
    else if (strcmp(lookahead.key, "Foco") == 0)
        parserFLUYE();
    else
        error("Clave inicial no válida");

    if (lookahead.type != TOKEN_EOF)
        error("Texto extra después del final del prompt");

}


int main (int argc, char *argv[])
{
    char cwd[512];
    _getcwd(cwd, sizeof(cwd));
    printf("Directorio de trabajo actual: %s\n", cwd);

    FILE *f=NULL;
    if(argc ==2)
    {
        f = fopen(argv[1], "r");
    }
    else
    {
        f = fopen("C:\\Users\\HP\\Documents\\Workspace\\Lenguajes de Programacion\\Prompt-Parser-LP\\Prompt.pmt", "r");

        printf("Usando archivo por defecto: Prompt.pmt\n");
    }
    if(!f)
    {
        printf("No se pudo abrir el archivo.\n");
        return 1;
    }
    initLexer(f);
    Promptparser();
    
    printf("Analisis completado sin errores.\n");
    
    fclose(f);    
    return 0;
}
// Este es un analizador léxico simple en C que lee un archivo de texto línea por línea y tokeniza cada línea en tokens de tipo CLAVE, NEWLINE, EOF o ERROR. Utiliza funciones estándar de C para manejar archivos y cadenas, y sigue el estándar C17.
//Al menos eso dice GitCopilot.
//Sigo el dia siguiente 
// Me falta:
// 1. 
// 2. parserr
// 3. Ejemplos de promts para el pmt
// 
