#include <iostream>
#include <unordered_map>

// Nodo: define un solo nodo, contiene un car ́acter de una clave y 
//una bandera es final que ayuda a identificar el el final de una clave.

class Nodo {
private:
    //atributos:
    char caracter;
    bool _es_final;
    std::unordered_map<char, Nodo*> SElementos;
    //pares clave-valor

public:
    //metodos:
    //constructor
    Nodo(char ct) {
        caracter = ct;
        _es_final = false;
    }

    
    bool _es_FinalNodo() const {
        return _es_final;
    }

    void set_Es_Final(bool valor) {
        _es_final = valor;
    }

    char getCaracter() const {
        return caracter;
    }

    std::unordered_map<char, Nodo*>& getSElementos() {
        return SElementos;
    }
    //referncia 


};

//Hoja: La clase Hoja hereda de Nodo y representa un nodo terminal que puede contener valores 
//de tipo T arbitrario. Su bandera es final siempre es verdadera.

template <typename T>
class Hoja : public Nodo {

private:
    T valorH;

public:
    Hoja(char c, T valor): Nodo(c) {   
        valorH = valor;
        set_Es_Final(true);
    }

    T getValorHoja() const {
        return valorH;
    }
};

//ArbolDigital: contiene las funciones insertar, obtenerValor y eliminar. El nodo ra ́ız es el nodo 
//de iniciode todas las claves y no debe almacenar ning ́un car ́acter clave.

template <typename T>

class ArbolDigital {
private:
    Nodo* root;

public:
    ArbolDigital() {
        root = new Nodo('\0');
    }


    void insertar(const std::string& clave, T valor) {
        
        Nodo* current = root;
        for (char ct : clave) {
            if (current->_es_FinalNodo()) {
                return;
            }

            Nodo* next = nullptr;
            if (current->getSElementos().count(ct) == 0) {
                next = new Nodo(ct);
                current->getSElementos()[ct] = next;
            } else {
                next = current->getSElementos()[ct];
            }

            current = next;
        }

        if(!current->_es_FinalNodo()){
            Hoja<T>* hoja = new Hoja<T>('\0', valor);
            current->getSElementos()['\0'] = hoja;
            current->set_Es_Final(true);
        }
        

    }

    T obtenerValor(const std::string& clave) {
        Nodo* current = root;
        for (char ct : clave) {
            if (current->getSElementos().count(ct) == 0) {
                std::cout << "No se encontro la clave" << std::endl;
                return T();
            }

            current = current->getSElementos()[ct];
        }

        if (!current->_es_FinalNodo()) {
            std::cout << "No se encontro la clave" << std::endl;
            return T();
        }

        Hoja<T>* hoja = static_cast<Hoja<T>*>(current->getSElementos()['\0']);
        //conversiones
        //retornar el valor
        return hoja->getValorHoja();
    }

    void eliminar(const std::string& clave) {
        eliminar_r(root, clave, 0);
    }

        void eliminar_r(Nodo* current, const std::string& clave, int level) {
        if (current == nullptr)
            return;
    
        if (level == clave.length()) {
            if (current->_es_FinalNodo()) {
                current->set_Es_Final(false);
                delete current->getSElementos()['\0'];
                current->getSElementos().erase('\0');
                //\0 = caracter nulos
            }
            return;
        }


        char ct = clave[level];
        if (current->getSElementos().count(ct) == 0)
        //count(c) devuelve el número de elementos en el mapa con la clave c
        //cuantas veses apararece la letra  
            return;

        Nodo* next = current->getSElementos()[ct];

        eliminar_r(next, clave, level + 1);

        if (next->getSElementos().empty() && !next->_es_FinalNodo()) {
            delete next;
            current->getSElementos().erase(ct);
            //elimina el nodo SElementos asociado al carácter c 
        }
    }

};
