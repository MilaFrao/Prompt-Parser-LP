# Prompt-Parser-LP

<promt> ::= <BRILLA> | <AVANZA> | <CREAR> | <FLUYE>;

<BRILLA> ::= <Breve_contexto> 
             <Rol_deseado> 
             <Intencion_meta> 
             <Lenguaje_tono> 
             <Logica_estructura> 
             <Aspecto_resultado>;
            
<Breve_contexto> ::= "Breve_contexto" ":" <texto> <nl>;
<Rol_deseado> ::= "Rol_deseado" ":" <texto> <nl>;
<Intencion_meta> ::= "Intencion_meta" ":" <texto> <nl>;
<Lenguaje_tono> ::= "Lenguaje_tono" ":" <texto> <nl>;
<Logica_estructura> ::= "Logica_estructura" ":" <texto> <nl>;
<Aspecto_resultado> ::= "Aspecto_resultado" ":" <texto> <nl>;

<Avanza> ::= <Antecedentes> 
             <Vision_proposito> 
             <Accion_concreta> 
             <Nivel_detalle> 
             <Zona_enfoque> 
             <Aspecto_visual>;

<Antecedentes> ::= "Antecedentes" ":" <texto> <nl>;
<Vision_proposito> ::= "Vision_proposito" ":" <texto> <nl>;
<Accion_concreta> ::= "Accion_concreta" ":" <texto> <nl>;
<Nivel_detalle> ::= "Nivel_detalle" ":" <texto> <nl>;
<Zona_enfoque> ::= "Zona_enfoque" ":" <texto> <nl>;
<Aspecto_visual> ::= "Aspecto_visual" ":" <texto> <nl>;

<CREAR> ::= <Contexto>
            <Rol>
            <Enfoque>
            <Accion>
            <Resultado> ;

<Contexto> ::= "Contexto" ":" <texto> <nl> ;
<Rol>      ::= "Rol"      ":" <texto> <nl> ;
<Enfoque>  ::= "Enfoque"  ":" <texto> <nl> ;
<Accion>   ::= "Accion"   ":" <texto> <nl> ;
<Resultado>::= "Resultado" ":" <texto> <nl> ;

<FLUYE> ::= <Foco>
            <Lugar>
            <Usuario>
            <Yo_interior>
            <Ejercicio> ;

<Foco>        ::= "Foco"        ":" <texto> <nl> ;
<Lugar>       ::= "Lugar"       ":" <texto> <nl> ;
<Usuario>     ::= "Usuario"     ":" <texto> <nl> ;
<Yo_interior> ::= "Yo_interior" ":" <texto> <nl> ;
<Ejercicio>   ::= "Ejercicio"   ":" <texto> <nl> ;

