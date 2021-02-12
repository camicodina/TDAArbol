
<h1 align="center">TDA Arbol Binario de Búsqueda</h1>

<h2 align="center">Algoritmos y Programación II <br> Méndez</h2><br>


_TDA Nº2 de Algoritmos y Programación II, Cátedra Mendez, Facultad de Ingeniería de la Universidad de Buenos Aires._
<br>

## Introducción 🚀 

Implementación de un Arbol Binario de Búsqueda con un iterador interno.

## Sección 1
_Explicación de lo entregado, como compilarlo (línea de compilación), como ejecutarlo (línea de ejecución) y el funcionamiento particular de la implementación_

### Modo de Uso 📝

Comandos de compilación y ejecución:

> $ gcc *.c -o abb -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O0  
> $ ./abb

Comandos de checkeo de memoria:

> $ valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./abb

Hay dos archivos de pruebas: "abb_minipruebas" y "pruebas". Uno de ellos se encuentra en formato txt. 
Si se desea probar el otro, convierta el .c a .txt y viceversa.

### Funcionamiento de la implementación... ⚙

**1. ABB:**

- El arbol se creará bajo las bases de 4 definiciones en el archivo "abb.h": "abb_comparador", "abb_liberar_elemento", "nodo_abb" y "abb".
    a. nodo_abb: esta estructura guarda referencia de un elemento que va a componer al nodo y de los valores a su izquierda y a su derecha.
    b. abb_comparador: recibe 2 punteros void por parámetro y puede devolver 1 de 3 valores enteros: 0 (en caso que los elementos comparados sean iguales), 1 (en caso que el primer elemento comparado sea mayor al segundo) o -1 (en el caso contrario).
    c. abb_liberar_elemento: dado un elemento, lo libera. Lo llamaremos destructor.
    d. abb: utiliza un nodo, que funcionará como nodo padre, el comparador y el destructor para componer el árbol 

```C
typedef int (*abb_comparador)(void*, void*);

typedef void (*abb_liberar_elemento)(void*);

typedef struct nodo_abb {
  void* elemento;
  struct nodo_abb* izquierda;
  struct nodo_abb* derecha;
} nodo_abb_t;

typedef struct abb{
  nodo_abb_t* nodo_raiz;
  abb_comparador comparador;
  abb_liberar_elemento destructor;
} abb_t;
```

- El árbol se crea sin ningún nodo, apuntando ya al comparador y al destructor pasados por parámetro.

- Para insertar un nodo en el árbol, primero verifico exista un nodo raíz y, de no ser así, lo agrego llamando a una función auxiliar para crear los nodos del árbol. Después utilizo una función recursiva: en caso de que el valor del nodo padre sea mayor al valor del elemento que quiero insertar o si son iguales, me muevo a la izquierda mientras la izquierda no sea NULL; en caso de que el valor del nodo padre sea menor al valor del elemento que quiero insertar, me muevo a la derecha mientras la derecha no sea NULL.

- Para las funciones de búsqueda se usa una lógica similar: comparando el elemento nodo padre y el elemento buscado, me muevo recursivamente a izquierda o a derecha hasta que el comparador de 0 (elementos son iguales).

- Para borrar un nodo del árbol primero verifico que el elemento del nodo a borrar exista dentro del árbol y, de existir varias veces, borra únicamente la primera aparición al recorrer el árbol. Habiendo aclarado esto, el recorrido también es similar al empleado en las anteriores funciones: me muevo a izquierda o derecha utilizando una función recursiva hasta que el valor del elemento buscado coincida con el elemento del nodo considerado en la iteración. Cuando coinciden los elementos, verifico la cantidad de hijos...
    - _0 hijos_: este sería el más sencillo de los casos ya que solo debo utilizar el destructor, de existir este, y liberar el nodo.
    - _1 hijo_: verifico de qué lado está el hijo, ¿izquierda o derecha? Asigno al elemento del nodo padre el elemento del nodo hijo y vuelvo a llamar a la función de manera recursiva para que acomode los demás nodos en caso de que tenga más hijos.
    - _2 hijos_: en este caso debo buscar cuál es el mayor elemento dentro del subarbol izquierdo (el predecesor inorden) ya que lo pondré como elemento del nodo padre que quiero borrar. 

- El árbol ofrece 3 opciones de recorrido:
    - _Inorden_: utiliza una función recursiva auxiliar que, de haber nodo izquierdo, avanza sobre él y sino agrega el elemento actual al array en la posición dada por el contador. Luego avanza a derecha si ésta no es NULL.
    - _Preorden_: utiliza una función recursiva auxiliar que agrega el elemento actual al array en la posición dada por el contador. De haber nodo izquierdo, avanza sobre él y luego avanza a derecha si ésta no es NULL.
    - _Postorden_: utiliza una función recursiva auxiliar que, de haber nodo izquierdo, avanza sobre él y sino agrega el elemento actual al array en la posición dada por el contador. Luego avanza a derecha si no es NULL.

**2. Iterador interno:**

El iterador interno permite que pasemos la acción a realizar sobre cada elemento del árbol por parámetro sin manejar memoria. Aumenta un contador de cantidad de elementos recorridos mientras que ésta cantidad sea menor a la cantidad de elementos en el árbol según una secuencia inorden, preorden o postorden y mientras el puntero función recibido por parámentro devuelva false.


## Sección 2

_Desarrollo los siguientes conceptos teóricos..._

Un árbol es una estructura de datos capaz de reduri el tiempo de acceso a éstos. Es una colección de nodos (o vértices del árbol) que puede estar vacía o consiste en un nodo raíz y 0 o más sub-árboles no vacíos cuyo nodo raíz es denominado nodo hijo.
El árbol binario de búsqueda o ABB es un tipo de árbol.

* **Diferencia entre ABB y Árbol Binario**

Los árboles binarios son árboles que nos permiten tener una noción de izquierda y derecha 
Estos árboles utilizan distintos métodos:
1. Crear
2. Destruir
3. ¿Está vacío?
4. Insertar
5. Borrar
6. Recorrer
7. Buscar
Sin embargo, una gran desventaja de esta estructura de datos es que que carece de mucha utilidad pues no tiene especificado ningun tipo de regla para operar sobre ella.
Por ejemplo, si quisiera buscar un elemento particular en un árbol binario debería recorrer todo el árbol hasta obtener el nodo con el elemento deseado. En el peor de los casos, la complejidad algorítmica para buscar un elemento es O(n).

El árbol binario de búsqueda (ABB) solventa ese percance ya que permiten tener las ventajas de las listas enlazadas y obtener la velocidad de la búsqueda binaria mediante el aprovechamiento de la estructura misma del árbol.
Se diferencia en que los elementos del sub-árbol izquierdo son menores que el elemento raíz del árbol y todos los elementos del sub-árbol derecho son mayores que ésta.

* **ABB**

Entonces, al tratar con un un árbol de comparación, si nos movemos a un sub-árbol derecho es porque estamos insertando valores más grandes que el nodo raíz. Sino, nos movemos hacia el sub-árbol izquierdo. 

<p align="center"><img width=60% src="https://upload.wikimedia.org/wikipedia/commons/thumb/f/f0/ABBEJEM.svg/656px-ABBEJEM.svg.png"></p>

Las operaciones básicas de árboles binarios son:
1. Crear
2. Destruir
3. ¿Está vacío?
4. Insertar
5. Borrar
6. Recorrer (en secuencia preorden (NID), inorden (IND) o postorden (IDN))
7. Buscar

* **Función de Destrucción**

La función de destrucción le permite al usuario ingresar cualquier tipo de información en su nodo, y si éste es correcto, esa información será insertada en el árbol.

Si no tiene esta función, solo puede ingresar elementos que no requieran ningún procesamiento especial para eliminarlos. Por ejemplo, los elementos del montón no se pueden insertar porque no se puede liberar el espacio necesario para ellos. Por lo tanto, puede ver que la razón principal es que proporciona flexibilidad para la implementación.

* **Complejidad Algorítmica**

<p align="center"><img width=80% src="https://miro.medium.com/max/2544/1*FkQzWqqIMlAHZ_xNrEPKeA.png"></p>

_Funciones de complejidad O(1):_

Las funciones O(1) o de complejidad constante son aquellas que, independientemente del tamaño del input, se ejecutan en igual tiempo. En el TDA ABB son...

```h
abb_t* arbol_crear(abb_comparador comparador, abb_liberar_elemento destructor);

void* arbol_raiz(abb_t* arbol);

bool arbol_vacio(abb_t* arbol);
```

Al crear árboles se realiza el mismo proceso: se verifica que se recibe un comparador, se reserva memoria y se asignan los valores arbol->comparador y arbol->destructor a lo recibido por parámetro. 

En arbol_vacio, por su parte, se verifica la existencia del primer valor del árbol, por lo que no es necesario un recorrido. 

Finalmente, en arbol_raiz, hacemos algo parecido: leemos el elemento almacenado en el nodo raíz, de existir este.

Por esto, la complejidad en los 3 casos se mantiene constante. 



_Funciones de complejidad O(log(n)):_

Las funciones O(log(n)) o de complejidad logarítmica son aquellas donde el tiempo de cálculo crece lentamente según la cantidad de datos del input se incrementa exponencialmente.

```h
int arbol_insertar(abb_t* arbol, void* elemento);

int arbol_borrar(abb_t* arbol, void* elemento);

void* arbol_buscar(abb_t* arbol, void* elemento);
```

En estas tres funciones lo que hacemos es buscar/insertar/eliminar un elemento en específico, pero al tener las nociones de izquierda (menores a raíz) y derecha (mayores a raíz), podemos "descartar" n/2-elementos para cada subárbol hasta encontrar la posición indicada. 



_Funciones de complejidad O(n):_

Las funciones O(n) o de complejidad lineal son aquellas donde el tiempo de cálculo crece a igual ritmo.

```h
size_t arbol_recorrido_inorden(abb_t* arbol, void** array, size_t tamanio_array);

size_t arbol_recorrido_preorden(abb_t* arbol, void** array, size_t tamanio_array);

size_t arbol_recorrido_postorden(abb_t* arbol, void** array, size_t tamanio_array);

void arbol_destruir(abb_t* arbol);

size_t abb_con_cada_elemento(abb_t* arbol, int recorrido, bool (*funcion)(void*, void*), void* extra);
```

En las funciones de recorrido, en caso que el tamaño del array pasado por parámetro sea mayor a la cantidad de elementos del árbol (n), debemos acceder a los n-elementos. 

Luego, en abb_con_cada_elemento, también estamos usando estos recorridos por lo que la complejidad es la misma.

Al destruir el arbol, debemos recorrer la n-cantidad de nodos para eliminarlos de a uno.



---

_Camila Codina_
