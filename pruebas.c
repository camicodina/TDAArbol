#include "abb.h"
#include "pa2mm.h"
#include <stdio.h>

// -------------------------- PRUEBAS ARBOL -------------------------- //

void probar_crear_arbol(){
    abb_t* arbol_inicial = NULL;

    pa2m_afirmar((arbol_inicial=arbol_crear())!=NULL, "Puedo crear un árbol");
    pa2m_afirmar(arbol_inicial->nodo_raiz == NULL, "Su inicio es NULL");
    pa2m_afirmar(arbol_raiz(arbol_inicial) == NULL, "No tiene nodo raíz");
    pa2m_afirmar(arbol_vacio(arbol_inicial) == true, "Está vacío");
    pa2m_afirmar(arbol_borrar(arbol_inicial, 20) == -1, "No se puede borrar el elemento 20");

    arbol_destruir(arbol_inicial);
}


void probar_insertar_en_arbol(){
    char a='a',e='e',i='i',o='o',u='u';
    
}

void probar_borrar_en_arbol(){
    
}

void probar_buscar_en_arbol(){
    
}

void probar_recorrido_inorden(){
    
}

void probar_recorrido_preorden(){
    
}

void probar_recorrido_postorden(){
    
}

// -------------------------- PRUEBAS ITERADOR INTERNO -------------------------- //


void prueba_iterador_interno(){

}

// int main(){
//     pa2m_nuevo_grupo("PRUEBAS DEL ARBOL");
//     probar_crear_arbol();
//     probar_insertar_en_arbol();
//     probar_borrar_en_arbol();
//     probar_buscar_en_arbol();
//     probar_recorrido_inorden();
//     probar_recorrido_preorden();
//     probar_recorrido_postorden();
    
//     pa2m_nuevo_grupo("PRUEBAS DE ITERADOR INTERNO");
//     prueba_iterador_interno();

//     pa2m_mostrar_reporte();
// }

