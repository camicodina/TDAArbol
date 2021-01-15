#define ABB_RECORRER_INORDEN   0
#define ABB_RECORRER_PREORDEN  1
#define ABB_RECORRER_POSTORDEN 2

#define EXITO 0
#define FALLA -1

#define IZQUIERDA -1
#define DERECHA 1
#define IGUALES 0

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
    if((comparador == IGUALES)||(comparador == IZQUIERDA)){ // Me muevo a la izquierda
        if(!(actual->izquierda)){
            actual->izquierda = nuevo_nodo;
            return;
        }
        actual = actual->izquierda;
    }else{ // (comparador == DERECHA) Me muevo a derecha
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
    if(!arbol) return FALLA;
    if(!arbol->comparador) return FALLA;
    nodo_abb_t* nuevo_nodo= calloc(1, sizeof(nodo_abb_t));
 	if(!nuevo_nodo) return FALLA;
 	nuevo_nodo->elemento=elemento;

    if(arbol_vacio(arbol)){
        arbol->nodo_raiz = nuevo_nodo;
        return EXITO;
    }
    aux_insertar_nodo(arbol,arbol->nodo_raiz,nuevo_nodo,elemento);
    return EXITO;
}

// -------------------------- FUNCIONES BÚSQUEDA -------------------------- //

/*
 * Función recursiva para buscar nodos en el ABB.
 */
void* aux_buscar_nodo(abb_t* arbol,nodo_abb_t* actual, void* elemento_buscado){
    if(!arbol) return NULL;
    if(!arbol->comparador) return NULL;
    if(!actual) return NULL;

    int comparador = arbol->comparador(actual->elemento, elemento_buscado);
    if(comparador == IGUALES){
        return actual->elemento;
    }else if(comparador == IZQUIERDA){ // Me muevo a izquierda
            actual = actual->izquierda;
    }else{ //comparador = DERECHA. Me muevo a derecha
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
 * Retorna el que será el nuevo nodo padre cuando sea borrado es el nodo raíz (nodo actual).
 */
nodo_abb_t* borrar_raiz(abb_t* arbol,nodo_abb_t* raiz){
    if(!arbol) return NULL;
    if(!raiz) return NULL;
    nodo_abb_t* nuevo_padre = NULL;
    int cantidad_hijos = aux_cantidad_hijos(raiz);

    if(cantidad_hijos == 1){
        if(!raiz->derecha){ //no hay hijo derecho. Opero con el izquierdo.
            nuevo_padre = raiz->izquierda;
        }else{ //no hay hijo izquierdo. Opero con el derecho
            nuevo_padre = raiz->derecha;
        }
    }
    if(cantidad_hijos == 2){
 		nuevo_padre = aux_buscar_menor_subarbol_derecho(raiz->derecha);

        if(raiz->derecha != nuevo_padre){
            nuevo_padre->derecha = raiz->derecha;
        }
        nuevo_padre->izquierda = raiz->izquierda;
    }
    return nuevo_padre;
}

/*
 * Función recursiva para borrar nodos en el ABB.
 */
nodo_abb_t* aux_borrar_nodo(abb_t* arbol,nodo_abb_t* actual,void* elemento_borrar,bool* fue_borrado){
    if(!arbol) return NULL;
    if(!arbol->comparador) return NULL;
    if(!arbol->destructor) return NULL;
    if(!actual) return NULL;
    int comparador = arbol->comparador(actual->elemento, elemento_borrar);

    if(comparador == IGUALES){ // nodo_raiz tiene al elemento buscado
        nodo_abb_t* aux_nodo_padre = borrar_raiz(arbol,actual);
        arbol->destructor(actual->elemento);
 		free(actual);
        *fue_borrado = true;
 		return aux_nodo_padre;
    }

    if(comparador == IZQUIERDA){ //Primero mayor al segundo. Me muevo a la izquierda.
        actual->derecha=aux_borrar_nodo(arbol,actual->izquierda,elemento_borrar,fue_borrado);
    }else{ //comparador == DERECHA. Primero menor al segundo. Me muevo a la derecha.
        actual->derecha=aux_borrar_nodo(arbol,actual->derecha,elemento_borrar,fue_borrado);
    }
    return actual;
}


/*
 * Busca en el arbol un elemento igual al provisto (utilizando la
 * funcion de comparación) y si lo encuentra lo quita del arbol.
 * Adicionalmente, si encuentra el elemento, invoca el destructor con
 * dicho elemento.
 * Devuelve 0 si pudo eliminar el elemento o -1 en caso contrario.
 */
int arbol_borrar(abb_t* arbol, void* elemento){
    if(!arbol) return FALLA;
    if(arbol_vacio(arbol)) return FALLA;
    bool fue_borrado=false;
    arbol->nodo_raiz = aux_borrar_nodo(arbol,arbol->nodo_raiz,elemento,&fue_borrado);
    if(fue_borrado) return EXITO;
    return FALLA;
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
size_t aux_recorrido_inorden(nodo_abb_t* actual, size_t contador, void** array, size_t tamanio_array){
    if(actual->izquierda){
        contador = aux_recorrido_inorden(actual->izquierda,contador, array, tamanio_array);
    }

    if(contador<tamanio_array){
        array[contador] = actual->elemento;
        contador++;
    }else{
        return contador;
    }
   
    if(actual->derecha){
        aux_recorrido_inorden(actual->derecha,contador,array,tamanio_array);
    }
    return contador;
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
    
    return aux_recorrido_inorden(arbol->nodo_raiz,contador,array,tamanio_array);
}

/*
 * - Visita primero el nodo raíz.
 * - Recorrido en preorden del subárbol izquierdo.
 * - Recorrido en preorden del subárbol derecho.
*/
size_t aux_recorrido_preorden(nodo_abb_t* actual, size_t contador, void** array, size_t tamanio_array){
    if(contador<tamanio_array){
        array[contador] = actual->elemento;
        contador++;
    }else{
        return contador;
    }

    if(actual->izquierda){
        aux_recorrido_preorden(actual->izquierda,contador, array, tamanio_array);
    }
    if(actual->derecha){
        aux_recorrido_preorden(actual->derecha,contador, array, tamanio_array);
    }
    return contador;
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
    return aux_recorrido_preorden(arbol->nodo_raiz,contador,array,tamanio_array);

}

/*
 * - Recorrido en postorden del subárbol izquierdo.
 * - Recorrido en preorden del subárbol derecho.
 * - Visita al nodo raíz.
*/
size_t aux_recorrido_postorden(nodo_abb_t* actual, size_t contador, void** array, size_t tamanio_array){
    if(contador >= tamanio_array) return contador;

    if(actual->izquierda){
    aux_recorrido_postorden(actual->izquierda,contador, array, tamanio_array);
    }
    if(actual->derecha){
    aux_recorrido_postorden(actual->derecha, contador, array, tamanio_array);
    }

    if(contador < tamanio_array){
        array[contador] = actual->elemento;
        contador++;
    }
    return contador;
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
    return aux_recorrido_postorden(arbol->nodo_raiz,contador,array,tamanio_array);

}

// -------------------------- FUNCIONES DESTRUIR -------------------------- //

/*
 * Función recursiva para destruir los nodos en el ABB.
 */
void aux_destruir_nodos(abb_t* arbol, nodo_abb_t* nodo_a_destruir){
    if(!arbol) return;
    if(!arbol->destructor) return;
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
    if(!arbol->destructor) return;
    if(!arbol_vacio(arbol)){
        aux_destruir_nodos(arbol,arbol->nodo_raiz);
        }
    free(arbol);
}


// -------------------------- ITERADOR INTERNO -------------------------- //

void iterador_recorrido_inorden(nodo_abb_t* nodo_raiz, bool (*funcion)(void*, void*), void* extra, bool* sigo, size_t* cantidad){
    if(!nodo_raiz) return true;

    if(!sigo && nodo_raiz->izquierda){
        iterador_recorrido_inorden(nodo_raiz->izquierda, funcion, extra, sigo, cantidad);
    }
    if(!sigo){
        *sigo = funcion(nodo_raiz->elemento, extra);
        (*cantidad)++;
    }
    if(!sigo && nodo_raiz->derecha){
        iterador_recorrido_inorden(nodo_raiz->derecha, funcion, extra, sigo, cantidad);
    }
}

void iterador_recorrido_preorden(nodo_abb_t* nodo_raiz, bool (*funcion)(void*, void*), void* extra, bool* sigo, size_t* cantidad){
    if(!nodo_raiz) return true;

    if(!sigo){
        *sigo = funcion(nodo_raiz->elemento, extra);
        (*cantidad)++;
    }
    if(!sigo && nodo_raiz->izquierda){
        iterador_recorrido_inorden(nodo_raiz->izquierda, funcion, extra, sigo, cantidad);
    }
    if(!sigo && nodo_raiz->derecha){
        iterador_recorrido_inorden(nodo_raiz->derecha, funcion, extra, sigo, cantidad);
    }
}

void iterador_recorrido_postorden(nodo_abb_t* nodo_raiz, bool (*funcion)(void*, void*), void* extra, bool* sigo, size_t* cantidad){
    if(!nodo_raiz) return true;

    if(!sigo && nodo_raiz->izquierda){
        iterador_recorrido_inorden(nodo_raiz->izquierda, funcion, extra, sigo, cantidad);
    }
    if(!sigo && nodo_raiz->derecha){
        iterador_recorrido_inorden(nodo_raiz->derecha, funcion, extra, sigo, cantidad);
    }
    if(!sigo){
        *sigo = funcion(nodo_raiz->elemento, extra);
        (*cantidad)++;
    }
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
    if(!arbol) return 0;
    if(arbol_vacio(arbol)) return 0;
    if(!funcion) return 0;

    bool sigo_recorriendo = false;
    size_t cantidad_elementos_recorridos = 0;

    if(recorrido == ABB_RECORRER_INORDEN){
        iterador_recorrido_inorden(arbol->nodo_raiz, funcion, extra, &sigo_recorriendo, &cantidad_elementos_recorridos);
    }
    if(recorrido == ABB_RECORRER_PREORDEN){
        iterador_recorrido_preorden(arbol->nodo_raiz, funcion, extra, &sigo_recorriendo, &cantidad_elementos_recorridos);
    }
    if(recorrido == ABB_RECORRER_POSTORDEN){
        iterador_recorrido_postorden(arbol->nodo_raiz, funcion, extra, &sigo_recorriendo, &cantidad_elementos_recorridos);
    }

    return cantidad_elementos_recorridos;
}
