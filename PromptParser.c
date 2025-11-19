#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

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

int main ()
{
    FILE *f = fopen("archivo,pmt", "r");
    if (!f)
    {
        perror("No se pudo abrir el archivo");
        return 1;
    }
    initLexer(f);
    Token tok;

    do
    {
        tok = getNextToken();
        switch (tok.type)
        {
            case TOKEN_CLAVE:
                printf("Linea %d: CLAVE='%s', TEXTO='%s'\n", tok.line, tok.key, tok.text);
                break;
            case TOKEN_NEWLINE:
                printf("Linea %d: NEWLINE\n", tok.line);
                break;
            case TOKEN_EOF:
                printf("Linea %d: EOF\n", tok.line);
                break;
            case TOKEN_ERROR:
                printf("Linea %d: ERROR\n", tok.line);
                break;
        }
    } while (tok.type != TOKEN_EOF);

    fclose(f);
    return 0;
}
// Este es un analizador léxico simple en C que lee un archivo de texto línea por línea y tokeniza cada línea en tokens de tipo CLAVE, NEWLINE, EOF o ERROR. Utiliza funciones estándar de C para manejar archivos y cadenas, y sigue el estándar C17.
//Al menos eso dice GitCopilot.
//Sigo el dia siguiente 
// Me falta:
// 1. Definicion de EBNF
// 2. Parser
// 3. Ejemplos de promts para el pmt
// 4. Como pasarle archivo pmt al programa
// 5. Como leer los archivos pmt 