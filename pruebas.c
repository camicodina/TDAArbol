#include "abb.h"
#include "pa2mm.h"
#include <stdio.h>

typedef struct pagina_web_t{
    int clave;
    char tamanio[10];
}pagina_web_t;

pagina_web_t* crear_contenido(int clave){
    pagina_web_t* pagina = (pagina_web_t*)malloc(sizeof(pagina_web_t));
    if(pagina)
        pagina->clave = clave;
    return pagina;
}

void destruir_contenido(pagina_web_t* pagina){
    free(pagina);
}

int comparar_contenido(void* elemento1, void* elemento2){
    if(!elemento1 || !elemento2)
        return 0;

    if(((pagina_web_t*)elemento1)->clave>((pagina_web_t*)elemento2)->clave)
        return 1;
    if(((pagina_web_t*)elemento1)->clave<((pagina_web_t*)elemento2)->clave)
        return -1;
    return 0;
}

void destructor_de_contenido(void* elemento){
    if(!elemento) return;
    destruir_contenido((pagina_web_t*)elemento);
}

// -------------------------- PRUEBAS ARBOL -------------------------- //

void probar_crear_arbol(){
    abb_t* arbol_inicial = NULL;
    int elemento_prueba = 20;

    pa2m_afirmar((arbol_inicial=arbol_crear(comparar_contenido, destructor_de_contenido))!=NULL, "Puedo crear un árbol");
    pa2m_afirmar(arbol_inicial->nodo_raiz == NULL, "Su inicio es NULL");
    pa2m_afirmar(arbol_raiz(arbol_inicial) == NULL, "No tiene nodo raíz");
    pa2m_afirmar(arbol_vacio(arbol_inicial) == true, "Está vacío");
    pa2m_afirmar(arbol_borrar(arbol_inicial, &elemento_prueba) == -1, "No se puede borrar el elemento 20");

    arbol_destruir(arbol_inicial);
}


void probar_insertar_y_busqueda_en_arbol(){
    pagina_web_t* title= crear_contenido(1);
    pagina_web_t* head= crear_contenido(2);
    pagina_web_t* meta= crear_contenido(3);
    pagina_web_t* link= crear_contenido(4);
    pagina_web_t* html= crear_contenido(5);
    pagina_web_t* li1= crear_contenido(8);
    pagina_web_t* ol= crear_contenido(9);
    pagina_web_t* li2= crear_contenido(10);
    pagina_web_t* div= crear_contenido(11);
    pagina_web_t* body= crear_contenido(15);
    abb_t* arbol_pagina = NULL;

    pa2m_afirmar((arbol_pagina=arbol_crear(comparar_contenido, destructor_de_contenido))!=NULL, "Se crea un árbol");
    pa2m_afirmar(arbol_vacio(arbol_pagina) == true, "El nuevo árbol está vacío");
    pa2m_afirmar(arbol_insertar(arbol_pagina,html) == 0, "Se inserta nodo raíz");
    pa2m_afirmar(((pagina_web_t*)arbol_raiz(arbol_pagina))->clave == html->clave, "La clave del nodo raíz es correcta.");
    pa2m_afirmar(arbol_vacio(arbol_pagina) == false, "El nuevo árbol ya no está vacío");

    pa2m_afirmar(arbol_insertar(arbol_pagina,head) == 0, "Se inserta nodo head");
    pa2m_afirmar(arbol_insertar(arbol_pagina,body) == 0, "Se inserta nodo body");
    pa2m_afirmar(arbol_insertar(arbol_pagina,title) == 0, "Se inserta nodo title");
    pa2m_afirmar(arbol_insertar(arbol_pagina,meta) == 0, "Se inserta nodo meta");
    pa2m_afirmar(arbol_insertar(arbol_pagina,div) == 0, "Se inserta nodo div");
    pa2m_afirmar(arbol_insertar(arbol_pagina,link) == 0, "Se inserta nodo link");
    pa2m_afirmar(arbol_insertar(arbol_pagina,ol) == 0, "Se inserta nodo ol");
    pa2m_afirmar(arbol_insertar(arbol_pagina,li1) == 0, "Se inserta nodo primer li");
    pa2m_afirmar(arbol_insertar(arbol_pagina,li2) == 0, "Se inserta nodo segundo li");

    pa2m_afirmar(((pagina_web_t*)arbol_buscar(arbol_pagina,head))->clave == head->clave, "Se encontró head y es correcto");
    pa2m_afirmar(((pagina_web_t*)arbol_buscar(arbol_pagina,body))->clave == body->clave, "Se encontró body y es correcto");
    pa2m_afirmar(((pagina_web_t*)arbol_buscar(arbol_pagina,title))->clave == title->clave, "Se encontró title y es correcto");
    pa2m_afirmar(((pagina_web_t*)arbol_buscar(arbol_pagina,meta))->clave == meta->clave, "Se encontró meta y es correcto");
    pa2m_afirmar(((pagina_web_t*)arbol_buscar(arbol_pagina,div))->clave == div->clave, "Se encontró div y es correcto");
    pa2m_afirmar(((pagina_web_t*)arbol_buscar(arbol_pagina,link))->clave == link->clave, "Se encontró link y es correcto");
    pa2m_afirmar(((pagina_web_t*)arbol_buscar(arbol_pagina,ol))->clave == ol->clave, "Se encontró ordered list y es correcto");
    pa2m_afirmar(((pagina_web_t*)arbol_buscar(arbol_pagina,li1))->clave == li1->clave, "Se encontró primer list item y es correcto");
    pa2m_afirmar(((pagina_web_t*)arbol_buscar(arbol_pagina,li2))->clave == li2->clave, "Se encontró segundo list item y es correcto");

    arbol_destruir(arbol_pagina);
}

void probar_borrar_en_arbol(){
    abb_t* arbol_borrado =arbol_crear(comparar_contenido,NULL);
    int a=5,l=2,g=15,o1=1,r=3,i=11,t=4,m=9,o2=8,s=10;

    arbol_insertar(arbol_borrado,&a);
    arbol_insertar(arbol_borrado,&l);
    arbol_insertar(arbol_borrado,&g);
    arbol_insertar(arbol_borrado,&o1);
    arbol_insertar(arbol_borrado,&r);
    arbol_insertar(arbol_borrado,&i);
    arbol_insertar(arbol_borrado,&t);
    arbol_insertar(arbol_borrado,&m);
    arbol_insertar(arbol_borrado,&o2);
    arbol_insertar(arbol_borrado,&s);

    pa2m_afirmar(arbol_borrar(arbol_borrado,&a) == 0, "Se borra nodo padre con 2 hijos");
    pa2m_afirmar(arbol_buscar(arbol_borrado,&a) == NULL, "No está el elemento a en el árbol");
    pa2m_afirmar(arbol_raiz(arbol_borrado) == &t, "Lo reemplaza la t");

    pa2m_afirmar(arbol_borrar(arbol_borrado,&s) == 0, "Se borra nodo s que no tenía hijos");
    pa2m_afirmar(arbol_buscar(arbol_borrado,&s) == NULL, "No está el elemento s en el árbol");

    pa2m_afirmar(arbol_borrar(arbol_borrado,&i) == 0, "Se borra nodo i que tenía 1 hijo");
    pa2m_afirmar(arbol_buscar(arbol_borrado,&i) == NULL, "No está el elemento i en el árbol");

    pa2m_afirmar(arbol_borrar(arbol_borrado,&m) == 0, "Se borra nodo t");
    pa2m_afirmar(arbol_borrar(arbol_borrado,&g) == 0, "Se borra nodo s");
    pa2m_afirmar(arbol_borrar(arbol_borrado,&t) == 0, "Se borra nodo o1");
    pa2m_afirmar(arbol_borrar(arbol_borrado,&r) == 0, "Se borra nodo g");
    pa2m_afirmar(arbol_borrar(arbol_borrado,&o1) == 0, "Se borra nodo a");
    pa2m_afirmar(arbol_borrar(arbol_borrado,&l) == 0, "Se borra nodo l");
    pa2m_afirmar(arbol_borrar(arbol_borrado,&o2) == 0, "Se borra nodo padre");

    pa2m_afirmar(arbol_vacio(arbol_borrado) == true, "El árbol está vacío");
    
    arbol_destruir(arbol_borrado);
}


void probar_recorrido(){
    abb_t* arbol_recorrer =arbol_crear(comparar_contenido,NULL);
    int c=6,a1=2,m=7,i=9,l=3,a2=1;

    arbol_insertar(arbol_recorrer,&c);
    arbol_insertar(arbol_recorrer,&a1);
    arbol_insertar(arbol_recorrer,&m);
    arbol_insertar(arbol_recorrer,&i);
    arbol_insertar(arbol_recorrer,&l);
    arbol_insertar(arbol_recorrer,&a2);

    size_t tamanio_inorden = 6;
    size_t tamanio_preorden = 10;
    size_t tamanio_postorden = 4;
    int* elementos[6];

    pa2m_afirmar(arbol_recorrido_inorden(arbol_recorrer, (void**)elementos, tamanio_inorden) == 6, "Recorrido inorden OK");
    pa2m_afirmar(arbol_recorrido_preorden(arbol_recorrer, (void**)elementos, tamanio_preorden) == 6, "Recorrido preorden OK");
    pa2m_afirmar(arbol_recorrido_postorden(arbol_recorrer, (void**)elementos, tamanio_postorden) == 4, "Recorrido postorden OK");

    arbol_destruir(arbol_recorrer);
}


// -------------------------- PRUEBAS ITERADOR INTERNO -------------------------- //

bool mostrar_elemento(void* elemento, void* extra){
    extra=extra; //para que no se queje el compilador, gracias -Werror -Wall
    if(elemento)
        printf("%i ", ((pagina_web_t*)elemento)->clave);
    return false;
}

void prueba_iterador_interno(){
    pagina_web_t* title= crear_contenido(1);
    pagina_web_t* head= crear_contenido(2);
    pagina_web_t* meta= crear_contenido(3);
    pagina_web_t* html= crear_contenido(5);
    pagina_web_t* body= crear_contenido(15);

    abb_t* arbol_iterador = arbol_crear(comparar_contenido, destructor_de_contenido);

    arbol_insertar(arbol_iterador,html);
    arbol_insertar(arbol_iterador,head);
    arbol_insertar(arbol_iterador,body);
    arbol_insertar(arbol_iterador,title);
    arbol_insertar(arbol_iterador,meta);

    pa2m_afirmar(abb_con_cada_elemento(arbol_iterador, ABB_RECORRER_INORDEN, mostrar_elemento, NULL) == 5, "Iterador Interno (inorden) OK");
    pa2m_afirmar(abb_con_cada_elemento(arbol_iterador, ABB_RECORRER_PREORDEN, mostrar_elemento, NULL) == 5, "Iterador Interno (preorden) OK");
    pa2m_afirmar(abb_con_cada_elemento(arbol_iterador, ABB_RECORRER_POSTORDEN, mostrar_elemento, NULL) == 5, "Iterador Interno (postorden) OK");

    arbol_destruir(arbol_iterador);
}

int main(){

    pa2m_nuevo_grupo("PRUEBAS DE ARBOL VACIO");
    probar_crear_arbol();
    
    pa2m_nuevo_grupo("PRUEBAS DE INSERCION Y BUSQUEDA");
    probar_insertar_y_busqueda_en_arbol();

    pa2m_nuevo_grupo("PRUEBAS DE BORRADO");
    probar_borrar_en_arbol();

    pa2m_nuevo_grupo("PRUEBAS DE RECORRIDO");
    probar_recorrido();
    
    pa2m_nuevo_grupo("PRUEBAS DE ITERADOR INTERNO");
    prueba_iterador_interno();

    pa2m_mostrar_reporte();
}

