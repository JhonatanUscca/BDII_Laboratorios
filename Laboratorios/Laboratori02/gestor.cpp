#include <iostream>
#include <fstream>
#include <cstring>

const int PAGE_SIZE = 4096;
//tamaño de la pagina

class GestorAlMAcenamientoS {
public:
    //constructor
    //Inicializa :
    // numTotalPags_ = 0
    // posPag_ = 0
    GestorAlMAcenamientoS(const char* filename) : filename_(filename), numTotalPags_(0), posPag_(0) {
        //Se abre el archivo en modo de lectura (std::ios::in) y en modo binario (std::ios::binary).
        std::fstream file(filename_, std::ios::in | std::ios::binary);
        //Se verifica si el archivo se abrió correctamente
        if (file.is_open()) {
            //Si el archivo se abrió correctamente, se lee un número determinado 
            //de bytes del archivo y se almacenan en la variable (numTotalPags_)
            file.read(reinterpret_cast<char*>(&numTotalPags_), sizeof(numTotalPags_));
            file.close();
        }
        else {
            // se crea un nuevo objeto std::fstream llamado newFile 
            // se abre el archivo en modo de escritura (std::ios::out) y en modo binario (std::ios::binary).
            // Se escribe un número determinado de bytes en el archivo utilizando la función 
            std::fstream newFile(filename_, std::ios::out | std::ios::binary);
            newFile.write(reinterpret_cast<char*>(&numTotalPags_), sizeof(numTotalPags_));
            newFile.close();
        }
    }

    //la función leerBloque abre un archivo, busca la posición adecuada para leer un bloque de datos, lee el 
    //bloque de datos en un búfer y luego cierra el archivo. Si el archivo no se pudo abrir, se imprime un 
    //mensaje de error.

    void leerBloque(int numPag, char* buffer) {
        // se realiza una búsqueda en el archivo utilizando la función seekg()
        // La posición de búsqueda se calcula multiplicando numPag por el tamaño 
        // de una página (PAGE_SIZE) y agregando el tamaño de numTotalPags_
        std::fstream file(filename_, std::ios::in | std::ios::binary);
        if (file.is_open()) {
            file.seekg(sizeof(numTotalPags_) + numPag * PAGE_SIZE);
            // seekg: se utiliza para establecer la posición del puntero de lectura en un archivo
            file.read(buffer, PAGE_SIZE);
            file.close();
        }
        else {
            std::cerr << "Error: no se pudo abrir el archivo " << filename_ << std::endl;
        }
    }

    int getPosBloque() const {
        return posPag_;
    }


    void leerPrimerBloque(char* buffer) {
        leerBloque(0, buffer);
    }

    void leerUltimoBloque(char* buffer) {
        leerBloque(numTotalPags_, buffer);
    }




    void leerBloqueActual(char* buffer) {
        leerBloque(posPag_, buffer);
    }

    void leerAnteriorBloque(char* buffer) {
        if (posPag_ > 0) {
            --posPag_;
            leerBloque(posPag_, buffer);
        }
        else {
            std::cerr << "Error: no se puede leer el bloque antes de la primera página " << std::endl;
        }
    }

    void leerSiguienteBloque(char* buffer) {
        if (posPag_ < numTotalPags_ - 1) {
            ++posPag_;
            leerBloque(posPag_, buffer);
        }
        else {
            std::cerr << "Error: no se puede leer el bloque después de la última página " << std::endl;
        }
    }

// la función escribirBloque abre un archivo en modo de lectura y escritura, busca la 
// posición adecuada para escribir un bloque de datos, escribe el bloque de datos en el 
// archivo y luego cierra el archivo. Si el archivo no se pudo abrir, se imprime un 
// mensaje de error.
    void escribirBloque(int numPag, const char* buffer) {
        std::fstream file(filename_, std::ios::in | std::ios::out | std::ios::binary);
        if (file.is_open()) {
            file.seekp(sizeof(numTotalPags_) + numPag * PAGE_SIZE);
            // seekp: se utiliza para establecer la posición del puntero de escritura en un archivo. 
            file.write(buffer, PAGE_SIZE);
            file.close();
        }
        else {
            std::cerr << "Error: no se pudo abrir el archivo " << filename_ << std::endl;
        }
    }

//La función agregarBloqueVacio abre un archivo en modo de lectura y escritura, mueve el puntero de 
//escritura al final del archivo, escribe un bloque vacío en el archivo, incrementa numTotalPags_ 
//para reflejar el nuevo bloque agregado, escribe el valor actualizado de numTotalPags_ en el archivo y 
//luego cierra el archivo. Si el archivo no se pudo abrir, se imprime un mensaje de error.
    void agregarBloqueVacio() {
        std::fstream file(filename_, std::ios::in | std::ios::out | std::ios::binary);
        if (file.is_open()) {
            file.seekp(0, std::ios::end);
            char emptyBlock[PAGE_SIZE] = {0};
            file.write(emptyBlock, PAGE_SIZE);
            ++numTotalPags_;
            file.seekp(0);
            file.write(reinterpret_cast<char*>(&numTotalPags_), sizeof(numTotalPags_));
            file.close();
        }
        else {
            std::cerr << "Error: no se pudo abrir el archivo " << filename_ << std::endl;
        }
    }

private:
    const char* filename_;
    int numTotalPags_;
    int posPag_;
};

#include <iostream>

int main() {
    GestorAlMAcenamientoS gestor("data.txt");

    char buffer[PAGE_SIZE] = "CAMA";
    char buffer2[PAGE_SIZE] = "MESA";
    gestor.escribirBloque(0, buffer);
    gestor.escribirBloque(1, buffer2);

 
    char readBuffer[PAGE_SIZE];

    gestor.leerBloqueActual(readBuffer);
    std::cout << gestor.getPosBloque() << std::endl;
    std::cout << readBuffer << std::endl;
 
    gestor.leerSiguienteBloque(readBuffer);
    std::cout << gestor.getPosBloque() << std::endl;
    
    
    std::cout << readBuffer << std::endl;

    gestor.agregarBloqueVacio();


    gestor.leerPrimerBloque(readBuffer);
    std::cout << readBuffer << std::endl;


    gestor.leerUltimoBloque(readBuffer);
    std::cout << readBuffer << std::endl;
    

    return 0;
}
