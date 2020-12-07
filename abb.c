#define ABB_RECORRER_INORDEN   0
#define ABB_RECORRER_PREORDEN  1
#define ABB_RECORRER_POSTORDEN 2

#include <stdbool.h>
#include <stdlib.h>
#include "abb.h"

// -------------------------- FUNCIONES CREACION -------------------------- //

/*
 * Crea el arbol y reserva la memoria necesaria de la estructura.
 * Comparador se utiliza para comparar dos elementos.
 * Destructor es invocado sobre cada elemento que sale del arbol,
 * puede ser NULL indicando que no se debe utilizar un destructor.
 *
 * Devuelve un puntero al arbol creado o NULL en caso de error.
 */
abb_t* arbol_crear(abb_comparador comparador, abb_liberar_elemento destructor){
    if(!comparador) return NULL;
    if(!destructor) return NULL;
    abb_t* arbol = calloc(1,sizeof(abb_t));
	if(!arbol) return NULL;
	arbol->comparador= comparador;
	arbol->destructor= destructor;
    return arbol;
}

// -------------------------- FUNCIONES INSERCIÓN -------------------------- //

/*
 * Función recursiva para insertar nodos en el ABB.
 */

void aux_insertar_nodo(abb_t* arbol,nodo_abb_t* actual,nodo_abb_t* nuevo_nodo,void* elemento_nuevo){
    int comparador = arbol->comparador(actual->elemento, elemento_nuevo);
    if((comparador == 0)||(comparador == 1)){ //Primero mayor al segundo o son iguales. Me muevo a la izquierda
        if(!(actual->izquierda)){
            actual->izquierda = nuevo_nodo;
            return;
        }
        actual = actual->izquierda;
    }else{ // (comparador == -1) el primer elemento es menor al segundo, me muevo a derecha
        if(!(actual->derecha)){
            actual->derecha = nuevo_nodo;
            return;
        }
        actual = actual->derecha;
    }
    aux_insertar_nodo(arbol,actual,nuevo_nodo,elemento_nuevo);
}


/*
 * Inserta un elemento en el arbol.
 * Devuelve 0 si pudo insertar o -1 si no pudo.
 * El arbol admite elementos con valores repetidos.
 */

int arbol_insertar(abb_t* arbol, void* elemento){
    if(!arbol) return -1;
    nodo_abb_t* nuevo_nodo= calloc(1, sizeof(nodo_abb_t));
 	if(!nuevo_nodo) return -1;
 	nuevo_nodo->elemento=elemento;

    if(arbol_vacio(arbol)){
        arbol->nodo_raiz = nuevo_nodo;
        return 0;
    }
    aux_insertar_nodo(arbol,arbol->nodo_raiz,nuevo_nodo,elemento);
    return 0;
}

// -------------------------- FUNCIONES BÚSQUEDA -------------------------- //

/*
 * Función recursiva para buscar nodos en el ABB.
 */
void* aux_buscar_nodo(abb_t* arbol,nodo_abb_t* actual, void* elemento_buscado){
    if(!arbol) return NULL;
    if(!actual) return NULL;

    int comparador = arbol->comparador(actual->elemento, elemento_buscado);
    if(comparador == 0){
        return actual->elemento;
    }else if(comparador == 1){ //Me muevo a izquierda
            actual = actual->izquierda;
    }else{ //comparador = -1. Me muevo a derecha
        actual = actual->derecha;
    }       
    return aux_buscar_nodo(arbol,actual,elemento_buscado);
}


/*
 * Busca en el arbol un elemento igual al provisto (utilizando la
 * funcion de comparación).
 *
 * Devuelve el elemento encontrado o NULL si no lo encuentra.
 */
void* arbol_buscar(abb_t* arbol, void* elemento){
    if(!arbol) return NULL;
    if(arbol_vacio(arbol)) return NULL;

    return aux_buscar_nodo(arbol,arbol->nodo_raiz,elemento);
}


// -------------------------- FUNCIONES BORRAR -------------------------- //

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
 * Función recursiva para borrar nodos en el ABB.
 */

void aux_borrar_nodo(abb_t* arbol,nodo_abb_t* actual,void* elemento_borrar){
    int comparador = arbol->comparador(actual->elemento, elemento_borrar);
    if(comparador == 0){ // nodo_raiz tiene al elemento buscado
        int cantidad_hijos = aux_cantidad_hijos(actual);
        if(cantidad_hijos == 0){
            arbol->destructor(actual->elemento);
            free(actual);
            return;
        }else if(cantidad_hijos == 1){
            if(!actual->derecha){ //no hay hijo derecho. Opero con el izquierdo.
               actual->elemento = actual->izquierda->elemento;
               actual = actual->izquierda;
            }else{ //no hay hijo izquierdo. Opero con el derecho
                actual->elemento = actual->derecha->elemento;
                actual = actual->derecha;
            }
        }else{ //2 hijos
            nodo_abb_t* menor_subarbol_derecho = aux_buscar_menor_subarbol_derecho(actual->derecha);
            actual->elemento = menor_subarbol_derecho->elemento;
            actual = menor_subarbol_derecho;
        }
        elemento_borrar = actual->elemento;

    }else if(comparador == 1){ //Primero mayor al segundo. Me muevo a la izquierda.
        actual = actual->izquierda;
    }else{ //comparador == -1. Primero menor al segundo. Me muevo a la derecha.
        actual = actual->derecha;
    }
    aux_borrar_nodo(arbol,actual,elemento_borrar);
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
    if(arbol_vacio(arbol)) return -1;
    if((arbol_buscar(arbol,elemento) == NULL) || (arbol_buscar(arbol,elemento) != elemento)) return -1;

    aux_borrar_nodo(arbol,arbol->nodo_raiz,elemento);
    return 0;
}

// -------------------------- OTRAS FUNCIONES -------------------------- //

/*
 * Devuelve el elemento almacenado como raiz o NULL si el árbol está
 * vacío o no existe.
 */
void* arbol_raiz(abb_t* arbol){
    if(!arbol) return NULL;
    if(!arbol_vacio(arbol)) return arbol->nodo_raiz->elemento;
    return NULL;
}

/*
 * Determina si el árbol está vacío.
 * Devuelve true si está vacío o el arbol es NULL, false si el árbol tiene elementos.
 */
bool arbol_vacio(abb_t* arbol){
    if(!arbol || !arbol->nodo_raiz) return true;
    return false;
}


// -------------------------- FUNCIONES RECORRIDO -------------------------- //

/*
 * - Recorrido en inorden en el subárbol izquierdo.
 * - Visita el nodo raíz.
 * - Recorrido en inorden del subárbol derecho.
*/
void aux_recorrido_inorden(nodo_abb_t* actual, size_t* contador, void** array, size_t tamanio_array){
    if(!actual) return;
    if(*contador == tamanio_array) return;

    aux_recorrido_inorden(actual->izquierda,contador, array, tamanio_array);
    if(*contador==tamanio_array) return;
    array[*contador] = actual->elemento;
    (*contador)++;
    aux_recorrido_inorden(actual->derecha, contador, array, tamanio_array);
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
    if(!arbol) return 0;
    if(arbol_vacio(arbol)) return 0;
    if(!array) return 0;
    if(tamanio_array == 0) return 0;
    size_t contador = 0;
    aux_recorrido_inorden(arbol->nodo_raiz, &contador, array, tamanio_array);
    return contador;
}

/*
 * - Visita primero el nodo raíz.
 * - Recorrido en preorden del subárbol izquierdo.
 * - Recorrido en preorden del subárbol derecho.
*/
void aux_recorrido_preorden(nodo_abb_t* actual, size_t* contador, void** array, size_t tamanio_array){
    if(!actual) return;
    if(*contador == tamanio_array) return;

    array[*contador] = actual->elemento;
    (*contador)++;
    aux_recorrido_preorden(actual->izquierda,contador, array, tamanio_array);
    aux_recorrido_preorden(actual->derecha, contador, array, tamanio_array);
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
    if(!arbol) return 0;
    if(arbol_vacio(arbol)) return 0;
    if(!array) return 0;
    if(tamanio_array == 0) return 0;
    size_t contador = 0;
    aux_recorrido_preorden(arbol->nodo_raiz, &contador, array, tamanio_array);
    return contador;

}


/*
 * - Recorrido en postorden del subárbol izquierdo.
 * - Recorrido en preorden del subárbol derecho.
 * - Visita al nodo raíz.
*/
void aux_recorrido_postorden(nodo_abb_t* actual, size_t* contador, void** array, size_t tamanio_array){
    if(!actual) return;
    if(*contador == tamanio_array) return;
    
    aux_recorrido_postorden(actual->izquierda,contador, array, tamanio_array);
    aux_recorrido_postorden(actual->derecha, contador, array, tamanio_array);

    if(*contador == tamanio_array) return;
    array[*contador] = actual->elemento;
    (*contador)++;
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
    if(!arbol) return 0;
    if(arbol_vacio(arbol)) return 0;
    if(!array) return 0;
    if(tamanio_array == 0) return 0;
    size_t contador = 0;
    aux_recorrido_postorden(arbol->nodo_raiz, &contador, array, tamanio_array);
    return contador;

}

// -------------------------- FUNCIONES DESTRUIR -------------------------- //

/*
 * Función recursiva para destruir los nodos en el ABB.
 */
void aux_destruir_nodos(abb_t* arbol, nodo_abb_t* nodo_a_destruir){
    if(!arbol) return;
    if(!nodo_a_destruir) return;

    aux_destruir_nodos(arbol, nodo_a_destruir->izquierda);
    aux_destruir_nodos(arbol, nodo_a_destruir->derecha);

	arbol->destructor(nodo_a_destruir->elemento);
	free(nodo_a_destruir);
}


/*
 * Destruye el arbol liberando la memoria reservada por el mismo.
 * Adicionalmente invoca el destructor con cada elemento presente en
 * el arbol.
 */
void arbol_destruir(abb_t* arbol){
    if(!arbol) return;
    if(!arbol_vacio(arbol)){
        aux_destruir_nodos(arbol,arbol->nodo_raiz);
        }
    free(arbol);
}


// -------------------------- ITERADOR INTERNO -------------------------- //

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
    if(!arbol) return 0;
    if(!arbol->nodo_raiz) return 0;
    if(!funcion) return 0;
    if(!extra) return 0;

    size_t tamanio_array = 999;
	void* array[tamanio_array];

    bool sigo_recorriendo = false;
    size_t cantidad_elementos_recorridos=0;
    size_t contador = 0;

    if(recorrido == ABB_RECORRER_INORDEN){
        contador = arbol_recorrido_inorden(arbol,array,tamanio_array);
    }
    if(recorrido == ABB_RECORRER_PREORDEN){
        contador = arbol_recorrido_preorden(arbol,array,tamanio_array);
    }
    if(recorrido == ABB_RECORRER_POSTORDEN){
        contador = arbol_recorrido_postorden(arbol,array,tamanio_array);
    }
    while(cantidad_elementos_recorridos < contador && !sigo_recorriendo){
        sigo_recorriendo = funcion(array[cantidad_elementos_recorridos],extra);
        cantidad_elementos_recorridos++;
    }
    return cantidad_elementos_recorridos;
}
