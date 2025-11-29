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
