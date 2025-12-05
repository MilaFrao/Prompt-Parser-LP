Instrucciones de Usuario — Prompt Parser (Lexer + Parser LL(1))

La entrega incluye:

Código fuente autodocumentado: PromptParser.c
Especificación formal en EBNF: README.md
Archivo de prueba: Prompt.pmt
Identificación del estudiante en el código (nombre, apellido, cédula, e-mail)

El programa puede compilarse en:

  * GCC / MinGW
  * Clang
  * MSVC (Visual Studio)

Compilación del Programa:
    A- Consola (GCC/MinGW)
    gcc PromptParser.c -o PromptParser

     B — Visual Studio Code (Utilizada durante el desarrollo)
    Abrir la carpeta del proyecto.
    Instalar extensiones:
    C/C++ (Microsoft)
    Code Runner
    Abrir "PromptParser.c"
    Ejecutar: "Ctrl + Alt + N"

Ejecución del Programa

    Ejecutar usando el archivo por defecto: C:\Users\HP\Documents\Workspace\Lenguajes de Programacion\Prompt-Parser-LP\Prompt.pmt


(Esta ruta puede y debe ajustarse en el código.)

**5. Formato del Archivo .PMT**

El archivo debe contener **un solo prompt** compuesto por:

* **6 líneas** → perfiles **BRILLA** o **AVANZA**
* **5 líneas** → perfiles **CREAR** o **FLUYE**

Cada línea debe ser:

* **Clave : Texto**

Ejemplo válido:

* **Contexto_breve: Un sistema embebido educativo.**
* **Rol_objetivo: Explicar arquitectura.**
* **Meta_principal: Guiar diseño.**
* **Tono_estilo: Formal y técnico.**
* **Estructura_logica: Paso a paso.**
* **Forma_del_resultado: Resumen claro.**


---

# Comportamiento del Programa

 **1. Lexer**

* Verifica el formato `Clave: Texto`
* Separa clave/texto
* Elimina espacios extra
* Rechaza texto vacío
* Rechaza líneas sin `:`
* Rechaza líneas vacías internas

 **2. Parser LL(1)**

* Determina el perfil según la primera clave o su sinónimo
* Valida cada clave en el orden correcto
* Verifica:

  * clave válida (o sinónimo permitido)
  * texto no vacío
  * estructura completa
  * ausencia de texto adicional tras el final

### **3. Salida**

Si todo es válido:


Analisis completado sin errores. Tipo de prompt: BRILLA

Si hay error, se indica:

* número de línea
* clave encontrada
* clave esperada
* tipo de error (léxico, sintáctico o semántico)



 **7. Pruebas Recomendadas**

 **Casos válidos:**

* Prompt BRILLA con sinónimos mezclados
* Prompt AVANZA con claves exactas
* Prompt CREAR con UTF-8 (acentos y ñ)
* Prompt FLUYE con espacios/tabs

### **Casos inválidos:**

* Línea sin :
* Clave fuera de orden
* Clave desconocida
* Texto vacío
* Líneas extra después del prompt
* EOF anticipado

 **8. Limpieza y Mantenimiento**

El programa no usa memoria dinámica
No genera archivos adicionales
No requiere dependencias externas

**9. Identificación del Estudiante**

(Se incluye directamente en los comentarios del código)

**10. Prompts probados durante el desarrollo**

Ejemplo 1 — BRILLA con sinónimos mezclados
Contexto_breve: Sistema de monitoreo médico para UCI.
Rol_objetivo: Explicar funcionamiento del módulo cardíaco.
Meta_principal: Aclarar arquitectura.
Tono_estilo: Técnico y directo.
Estructura_logica: Paso a paso.
Forma_del_resultado: Informe resumido.

Ejemplo 2 — BRILLA con claves exactas
Breve_contexto: Proyecto IoT para control ambiental.
Rol_deseado: Ingeniero biomédico asesor.
Intencion_meta: Detallar requisitos.
Lenguaje_tono: Formal.
Logica_estructura: Listas y explicación técnica.
Aspecto_resultado: Documento técnico.

Ejemplo 1 — AVANZA con sinónimos
Historia_previa: Problema de latencia en sensores.
Meta_general: Optimizar comunicaciones.
Tarea_especifica: Explicar solución.
Profundidad_requerida: Nivel intermedio.
Area_clave: Canal SPI.
Representacion_visual: Diagrama y resumen.

Antecedentes: Módulo WiFi con errores.
Vision_proposito: Mejorar conectividad.
Accion_concreta: Identificar causas.
Nivel_detalle: Extenso.
Zona_enfoque: Firmware de red.
Aspecto_visual: Gráfico comparativo.

Ejemplo 1 — CREAR (UTF-8, acentos y ñ)
Contexto: Análisis de consumo energético.
Rol: Ingeniero electrónico.
Enfoque: Optimizar carga inversa.
Accion: Explicar estrategias.
Resultado: Informe técnico claro.

Ejemplo 2 — CREAR con sinónimos
Ambito: Diseño de sensores portátiles.
Papel: Redactor técnico.
Direccion: Reducir ruido analógico.
Tarea: Describir pasos.
Producto_final: Guía técnica.

Ejemplo 1 — FLUYE con valores exactos
Foco: Importancia del diseño.
Lugar: Laboratorio principal.
Usuario: Estudiante de ingeniería.
Yo_interior: Motivación personal.
Ejercicio: Reflexión escrita.

Ejemplo 2 — FLUYE con sinónimos
Punto_central: Concentración del proyecto.
Ambiente: Aula experimental.
Persona: Usuario en formación.
Estado_interno: Intención profesional.
Actividad: Redacción guiada.





(* ====================================================== *)
(*  LENGUAJE FORMAL PARA ARCHIVOS .PMT                    *)
(*  Estructuras: BRILLA, AVANZA, CREAR, FLUYE            *)
(* ====================================================== *)

<Archivo> ::= <Prompt> <EOF> ;

<Prompt> ::= <BRILLA> | <AVANZA> | <CREAR> | <FLUYE> ;

(* ------------------------- BRILLA ------------------------- *)
<BRILLA> ::=
    <Breve_contexto>
    <Rol_deseado>
    <Intencion_meta>
    <Lenguaje_tono>
    <Logica_estructura>
    <Aspecto_resultado> ;

Breve_contexto      ::= <Clave_Breve_contexto>      ":" <espacio>? <texto> <nl> ;
Rol_deseado         ::= <Clave_Rol_deseado>         ":" <espacio>? <texto> <nl> ;
Intencion_meta      ::= <Clave_Intencion_meta>      ":" <espacio>? <texto> <nl> ;
Lenguaje_tono       ::= <Clave_Lenguaje_tono>       ":" <espacio>? <texto> <nl> ;
Logica_estructura   ::= <Clave_Logica_estructura>   ":" <espacio>? <texto> <nl> ;
Aspecto_resultado   ::= <Clave_Aspecto_resultado>   ":" <espacio>? <texto> <nl> ;

(* ------------------------ AVANZA -------------------------- *)
<AVANZA> ::=
    <Antecedentes>
    <Vision_proposito>
    <Accion_concreta>
    <Nivel_detalle>
    <Zona_enfoque>
    <Aspecto_visual> ;

Antecedentes       ::= <Clave_Antecedentes>       ":" <espacio>? <texto> <nl> ;
Vision_proposito   ::= <Clave_Vision_proposito>   ":" <espacio>? <texto> <nl> ;
Accion_concreta    ::= <Clave_Accion_concreta>    ":" <espacio>? <texto> <nl> ;
Nivel_detalle      ::= <Clave_Nivel_detalle>      ":" <espacio>? <texto> <nl> ;
Zona_enfoque       ::= <Clave_Zona_enfoque>       ":" <espacio>? <texto> <nl> ;
Aspecto_visual     ::= <Clave_Aspecto_visual>     ":" <espacio>? <texto> <nl> ;

(* ------------------------ CREAR --------------------------- *)
<CREAR> ::=
    <Contexto>
    <Rol>
    <Enfoque>
    <Accion>
    <Resultado> ;

Contexto  ::= <Clave_Contexto> ":" <espacio>? <texto> <nl> ;
Rol        ::= <Clave_Rol>      ":" <espacio>? <texto> <nl> ;
Enfoque    ::= <Clave_Enfoque>  ":" <espacio>? <texto> <nl> ;
Accion     ::= <Clave_Accion>   ":" <espacio>? <texto> <nl> ;
Resultado  ::= <Clave_Resultado> ":" <espacio>? <texto> <nl> ;

(* ------------------------ FLUYE --------------------------- *)
<FLUYE> ::=
    <Foco>
    <Lugar>
    <Usuario>
    <Yo_interior>
    <Ejercicio> ;

Foco        ::= <Clave_Foco>        ":" <espacio>? <texto> <nl> ;
Lugar       ::= <Clave_Lugar>       ":" <espacio>? <texto> <nl> ;
Usuario     ::= <Clave_Usuario>     ":" <espacio>? <texto> <nl> ;
Yo_interior ::= <Clave_Yo_interior> ":" <espacio>? <texto> <nl> ;
Ejercicio   ::= <Clave_Ejercicio>   ":" <espacio>? <texto> <nl> ;

(* ====================================================== *)
(*           TERMINALES PARA SINÓNIMOS DE CLAVES          *)
(* ====================================================== *)

(* Nota: Cada grupo se corresponde con el array C: const char* CLAVE_Breve_contexto[] = {...} *)

<Clave_Breve_contexto> ::=
      "Breve_contexto" | "Contexto_breve" | "Resumen_contexto"
    | "Situacion_inicial" | "Descripcion_corta" | "Panorama_general"
    | "Introduccion_breve" ;

<Clave_Rol_deseado> ::=
      "Rol_deseado" | "Rol_objetivo" | "Funcion_esperada"
    | "Posicion_asumida" | "Identidad_del_rol" | "Papel_a_cumplir"
    | "Perspectiva_del_rol" ;

<Clave_Intencion_meta> ::=
      "Intencion_meta" | "Meta_principal" | "Objetivo_central"
    | "Proposito_final" | "Intencion_clave" | "Resultado_esperado"
    | "Meta_del_prompt" ;

<Clave_Lenguaje_tono> ::=
      "Lenguaje_tono" | "Tono_estilo" | "Estilo_de_redaccion"
    | "Registro_linguistico" | "Manera_de_hablar" | "Tono_deseado"
    | "Estilo_comunicativo" ;

<Clave_Logica_estructura> ::=
      "Logica_estructura" | "Estructura_logica" | "Organizacion_del_contenido"
    | "Secuencia_del_mensaje" | "Esquema_de_redaccion" | "Metodo_expositivo"
    | "Flujo_de_informacion" ;

<Clave_Aspecto_resultado> ::=
      "Aspecto_resultado" | "Forma_del_resultado" | "Presentacion_final"
    | "Tipo_de_salida" | "Formato_esperado" | "Especificacion_del_resultado"
    | "Definicion_del_output" ;

(* AVANZA, CREAR y FLUYE continúan igual, omitidos aquí por espacio,
   pero van exactamente igual que BRILLA. *)

(* ====================================================== *)
(*                   SÍMBOLOS TERMINALES                   *)
(* ====================================================== *)

(* Separador clave/valor *)
":" ::= ":" ;

(* Espacio opcional después de los dos puntos *)
<espacio> ::= " " | "\t" | <espacio> <espacio> ;

(* Fin de línea *)
<nl> ::= "\n" | "\r\n" ;

(* Contenido libre del usuario *)
<texto> ::= { <caracter_texto> } ;

(* Caracteres válidos dentro de <texto> *)
<caracter_texto> ::=
      letra
    | digito
    | signo_puntuacion
    | simbolo
    | espacio
    | caracter_utf8 ;

(* Definiciones básicas *)
letra ::= "A" | "B" | ... | "Z"
        | "a" | "b" | ... | "z"
        | caracteres_acentuados ;

digito ::= "0" | "1" | ... | "9" ;

signo_puntuacion ::= "." | "," | ";" | "?" | "!" | "-" | "_" ;

simbolo ::= "(" | ")" | "[" | "]" | "{" | "}" | "/" | "\" | "#"
          | "+" | "*" | "=" | "~" | "%" | "&" | "|" | "<" | ">" | "'" | "\"" ;

espacio ::= " " | "\t" ;

(*   
   NOTA: Los caracteres acentuados y UTF-8 no se listan uno por uno 
   porque el lenguaje los permite en <texto> sin restricciones.
*)
