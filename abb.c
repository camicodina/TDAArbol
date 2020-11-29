#define ABB_RECORRER_INORDEN   0
#define ABB_RECORRER_PREORDEN  1
#define ABB_RECORRER_POSTORDEN 2

#include <stdbool.h>
#include <stdlib.h>
#include "abb.h"

/*
 * Crea el arbol y reserva la memoria necesaria de la estructura.
 * Comparador se utiliza para comparar dos elementos.
 * Destructor es invocado sobre cada elemento que sale del arbol,
 * puede ser NULL indicando que no se debe utilizar un destructor.
 *
 * Devuelve un puntero al arbol creado o NULL en caso de error.
 */
abb_t* arbol_crear(abb_comparador comparador, abb_liberar_elemento destructor){
    abb_t* arbol = malloc(sizeof(abb_t));
    if(!arbol) return NULL;
    arbol->nodo_raiz=NULL;
    arbol->comparador=comparador;
    arbol->destructor=destructor;
    return arbol;
}

/*
 * Crea un nodo vacío.
 */

nodo_abb_t* aux_crear_nodos(void* elemento){
    nodo_abb_t* nuevo_nodo = malloc(sizeof(nodo_abb_t));
    if(!nuevo_nodo) return NULL;
    nuevo_nodo->derecha = NULL;
    nuevo_nodo->izquierda = NULL;
    nuevo_nodo->elemento = elemento;
    return nuevo_nodo;
}

/*
 * Inserta un elemento en el arbol.
 * Devuelve 0 si pudo insertar o -1 si no pudo.
 * El arbol admite elementos con valores repetidos.
 */

int arbol_insertar(abb_t* arbol, void* elemento){
    if(!arbol) return -1;
    if(!elemento) return -1;
    int comparador = arbol->comparador(arbol->nodo_raiz->elemento, elemento);
    nodo_abb_t* nuevo_nodo = aux_crear_nodos(elemento);
    if(!nuevo_nodo) return -1;

    if(!arbol->nodo_raiz){
        arbol->nodo_raiz = nuevo_nodo;
        return 0;
    }
    // nodo_abb_t* nodo_padre = arbol->nodo_raiz;
    
    if((comparador == 0)||(comparador == 1)){ //primero mayor al segundo o son iguales, me muevo a la izquierda
        if(arbol->nodo_raiz->izquierda != NULL){
            free(nuevo_nodo);
             arbol_insertar(arbol->nodo_raiz->izquierda, elemento);
        }
        arbol->nodo_raiz->izquierda = nuevo_nodo;
    }else{ // (comparador == -1) el primer elemento es menor al segundo, me muevo a derecha
        if(arbol->nodo_raiz->derecha != NULL){
            free(nuevo_nodo);
            arbol_insertar(arbol->nodo_raiz->derecha, elemento);
        }
        arbol->nodo_raiz->derecha = nuevo_nodo;
    }
    return 0;
}

/*
 * Para un elemento dado devuelve:
 * - 0 si no tiene hijos o no existe el nodo
 * - 1 si tiene un hijo
 * - 2 si tiene dos hijos.
 */
int aux_cantidad_hijos(nodo_abb_t* nodo){
	if((!nodo) || (!nodo->izquierda && !nodo->derecha)){
        return 0;
    }else if((nodo->derecha && !(nodo->izquierda)) || (!(nodo->derecha) && nodo->izquierda)){
        return 1;
    }else{ //nodo->derecha && nodo->izquierda
        return 2;
    }
}

/*
 * Busca el menor elemento dentro del subarbol derecho
 */
nodo_abb_t* aux_buscar_menor_subarbol_derecho(nodo_abb_t* nodo_menor_subarbol){
	if(!nodo_menor_subarbol->izquierda){
        return nodo_menor_subarbol;
    }else{
        return aux_buscar_menor_subarbol_derecho(nodo_menor_subarbol->izquierda);
    }
}

/*
 * Busca en el arbol un elemento igual al provisto (utilizando la
 * funcion de comparación) y si lo encuentra lo quita del arbol.
 * Adicionalmente, si encuentra el elemento, invoca el destructor con
 * dicho elemento.
 * Devuelve 0 si pudo eliminar el elemento o -1 en caso contrario.
 */
int arbol_borrar(abb_t* arbol, void* elemento){
    if(!arbol) return -1;
    if(!arbol->nodo_raiz) return -1;
    if(!elemento) return -1;
    if((!arbol_buscar(arbol,elemento)) || (arbol_buscar(arbol,elemento) != elemento)) return -1;

    int comparador = arbol->comparador(arbol->nodo_raiz->elemento, elemento);
    if((comparador == 0) && aux_cantidad_hijos(arbol->nodo_raiz) == 0){ // nodo_raiz tiene al elemento buscado y no tiene hijos
        arbol->destructor(arbol->nodo_raiz->elemento);
        arbol->nodo_raiz = NULL;
        return 0;
    }
    if(comparador == 1){
        if(arbol->nodo_raiz->izquierda){
            arbol_borrar(arbol->nodo_raiz->izquierda, elemento);
        }
    }else if(comparador == -1){
        if(arbol->nodo_raiz->derecha){
            arbol_borrar(arbol->nodo_raiz->derecha, elemento);
        }
    }else{
        if(aux_cantidad_hijos(arbol->nodo_raiz)==0){
            arbol->destructor(arbol->nodo_raiz->elemento);
            arbol->nodo_raiz = NULL;
        }else if(aux_cantidad_hijos(arbol->nodo_raiz)==1){
            if(!arbol->nodo_raiz->derecha){
                arbol->nodo_raiz->elemento = arbol->nodo_raiz->izquierda->elemento;
                arbol->nodo_raiz->izquierda = arbol_borrar(arbol->nodo_raiz->izquierda, arbol->nodo_raiz->elemento);
            }else{ //no hay hijo izquierdo
                arbol->nodo_raiz->elemento = arbol->nodo_raiz->derecha->elemento;
                arbol->nodo_raiz->derecha = arbol_borrar(arbol->nodo_raiz->derecha, arbol->nodo_raiz->elemento);
            }
        }else{ //2 hijos
            nodo_abb_t* menor_subarbol_derecho = aux_buscar_menor_subarbol_derecho(arbol->nodo_raiz->derecha);
            arbol->nodo_raiz->elemento = menor_subarbol_derecho->elemento;
            menor_subarbol_derecho = arbol_borrar(menor_subarbol_derecho, menor_subarbol_derecho->elemento);
        }
    }
    return 0;
}

/*
 * Busca en el arbol un elemento igual al provisto (utilizando la
 * funcion de comparación).
 *
 * Devuelve el elemento encontrado o NULL si no lo encuentra.
 */
void* arbol_buscar(abb_t* arbol, void* elemento){
    if(!arbol) return NULL;
    if(!arbol->nodo_raiz) return -1;
    if(!elemento) return NULL;
    int comparador = arbol->comparador(arbol->nodo_raiz, elemento);
    if(comparador == 1){ //me muevo a izquierda
        if(arbol->nodo_raiz->izquierda != NULL){
            arbol_buscar(arbol->nodo_raiz->izquierda, elemento);
        }
        return NULL;
    }else if(comparador == -1){ //me muevo a derecha
        if(arbol->nodo_raiz->derecha != NULL){
            arbol_buscar(arbol->nodo_raiz->derecha, elemento);
        }
        return NULL;
    }else{
        return elemento;
    }
}

/*
 * Devuelve el elemento almacenado como raiz o NULL si el árbol está
 * vacío o no existe.
 */
void* arbol_raiz(abb_t* arbol){
    if(!arbol) return NULL;
    if(arbol_vacio(arbol)==false){
        return arbol->nodo_raiz->elemento;
    }else{
        return NULL;
    }
}

/*
 * Determina si el árbol está vacío.
 * Devuelve true si está vacío o el arbol es NULL, false si el árbol tiene elementos.
 */
bool arbol_vacio(abb_t* arbol){
    if((arbol_raiz(arbol) == NULL) || (!arbol)) return true;
    return false;
}

/*
 * Llena el array del tamaño dado con los elementos de arbol
 * en secuencia inorden.
 * Devuelve la cantidad de elementos del array que pudo llenar (si el
 * espacio en el array no alcanza para almacenar todos los elementos,
 * llena hasta donde puede y devuelve la cantidad de elementos que
 * pudo poner).
 */
size_t arbol_recorrido_inorden(abb_t* arbol, void** array, size_t tamanio_array){

}

/*
 * Llena el array del tamaño dado con los elementos de arbol
 * en secuencia preorden.
 * Devuelve la cantidad de elementos del array que pudo llenar (si el
 * espacio en el array no alcanza para almacenar todos los elementos,
 * llena hasta donde puede y devuelve la cantidad de elementos que
 * pudo poner).
 */
size_t arbol_recorrido_preorden(abb_t* arbol, void** array, size_t tamanio_array){

}

/*
 * Llena el array del tamaño dado con los elementos de arbol
 * en secuencia postorden.
 * Devuelve la cantidad de elementos del array que pudo llenar (si el
 * espacio en el array no alcanza para almacenar todos los elementos,
 * llena hasta donde puede y devuelve la cantidad de elementos que
 * pudo poner).
 */
size_t arbol_recorrido_postorden(abb_t* arbol, void** array, size_t tamanio_array){
    
}

/*
 * Destruye el arbol liberando la memoria reservada por el mismo.
 * Adicionalmente invoca el destructor con cada elemento presente en
 * el arbol.
 */
void arbol_destruir(abb_t* arbol){
    if(!arbol) return NULL;
    while(arbol->nodo_raiz != NULL){
        arbol_borrar(arbol,arbol->nodo_raiz);
    }
    free(arbol);
}

/*
 * Iterador interno. Recorre el arbol e invoca la funcion con cada
 * elemento del mismo. El puntero 'extra' se pasa como segundo
 * parámetro a la función. Si la función devuelve true, se finaliza el
 * recorrido aun si quedan elementos por recorrer. Si devuelve false
 * se sigue recorriendo mientras queden elementos.
 * El recorrido se realiza de acuerdo al recorrido solicitado.  Los
 * recorridos válidos son: ABB_RECORRER_INORDEN, ABB_RECORRER_PREORDEN
 * y ABB_RECORRER_POSTORDEN.
 * Devuelve la cantidad de elementos que fueron recorridos.
*/
size_t abb_con_cada_elemento(abb_t* arbol, int recorrido, bool (*funcion)(void*, void*), void* extra){
    
}



