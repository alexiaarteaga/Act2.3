/*
Autor(es):
- Cesar Augusto Ramirez Davila | A01712439
*/

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;
/////////////////////////////////////////////////////////////////////
// almacena los registros de la bitacora
struct Registro
{
  string fecha;   // Fecha
  string ip;      // Direccion ip
  string mensaje; // Mensaje del registro
};
/////////////////////////////////////////////////////////////////////

struct Node
{
  Registro data; // cada nodo contiene un registro de la bitacora
  Node *sig;     // apunta al siguiente nodo
  Node *prev;    // apunta al nodo previo

  // constructor del nodo
  Node(const Registro &registro) : data(registro), sig(nullptr), prev(nullptr) {}
};
/////////////////////////////////////////////////////////////////////

class ListaDoble
{
private:
  // primer y ultimo nodo de la lista respectivamente
  Node *head;
  Node *tail;

public:
  // constructor que inicializa una lista vacia
  ListaDoble() : head(nullptr), tail(nullptr) {}

  // Destructor para liberar la memoria de los nodos
  ~ListaDoble()
  {
    Node *current = head; // empezamos desde el primer nodo de la lista
    while (current)       // mientras la lista aun tenga nodos
    {
      Node *temp = current;   // guardamos el valor del nodo actual
      current = current->sig; // avanza al siguiente nodo
      delete temp;            // se borra el nodo guardado
    }
  }
  // Método para obtener el puntero al primer nodo de la lista
  Node *getHead() const
  {
    return head; // Devuelve el puntero al primer nodo
  }

  /////////////////////////////////////////////////////////////////////
  // agregar registros a la lista
  void create(const Registro &registro)
  {
    Node *nuevo = new Node(registro); // crea nodos con los registros dados
    if (!head)                        // si la lista esta vacia
    {
      head = tail = nuevo; // el nuevo nodo se vuelve el primer y ultimo nodo
    }
    else
    {
      tail->sig = nuevo;  // se conecta el nuevo nodo como el ultimo nodo
      nuevo->prev = tail; // el nodo previo al nuevo nodo se vuelve el que era tail
      tail = nuevo;       // se actualiza el ultimo nodo como tail
    }
  }

  /////////////////////////////////////////////////////////////////////

  void printRegistros() const
  {
    Node *current = head; // empieza desde la cabeza de la lista
    while (current)       // recorre todos los nodos de la lista
    {
      // imprime el registro en el nodo actual
      cout << current->data.fecha << " " << current->data.ip << " " << current->data.mensaje << endl;
      current = current->sig; // avanza al siguiente nodo
    }
  }

  void nextNode();
  void prevNode();
};

/////////////////////////////////////////////////////////////////////
unsigned long ipToNum(const string &ip)
{
  size_t pos = ip.find(':'); // busca los ':' en la ip para no tomar en cuenta eso y lo que sigue eliminando el puerto
  string ipSinPuerto = (pos != string::npos) ? ip.substr(0, pos) : ip;

  unsigned long resultado = 0; // guarda la ip en formato numerico
  istringstream ss(ipSinPuerto);
  string segmento; // guarda los segmentos de la ip
  int factor = 24; // Desplazamiento inicial

  while (getline(ss, segmento, '.'))
  {
    resultado += stoul(segmento) << factor; // converite cada segmento y los desplaza
    factor -= 8;                            // quita desplazamiento para el siguiente segmento
  }

  return resultado; // la ip como numero
}

/////////////////////////////////////////////////////////////////////
void merge(vector<Registro> &registros, int left, int middle, int right)
{
  vector<Registro> temp(right - left + 1); // vector temporal que almacena los registros ordenadoss
  int i = left, j = middle + 1, k = 0;     // indices para las mitades izquierda y derecha

  // combina las mitades
  while (i <= middle && j <= right) // mientras los indices no sobre pasen sus limites
  {
    if (ipToNum(registros[i].ip) <= ipToNum(registros[j].ip))
    {
      temp[k++] = registros[i++]; // se agrega el elemento de la izquierda al vector temporal
    }
    else
    {
      temp[k++] = registros[j++]; // Agrega el elemento de la derecha al vector temporal
    }
  }

  // agrega elementos restantes de la mitad izquierda
  while (i <= middle)
  {
    temp[k++] = registros[i++];
  }

  // agregar elementos restandes de la mitad derecha
  while (j <= right)
  {
    temp[k++] = registros[j++];
  }

  // copia los elementos ordenados de regreso al vector original
  for (int l = 0; l < temp.size(); l++)
  {
    registros[left + l] = temp[l];
  }
}

/////////////////////////////////////////////////////////////////////
// ordena el vector de registros por IP
void mergeSort(vector<Registro> &registros, int left, int right)
{
  if (left < right) // si tiene mas de un elemento
  {
    int middle = (left + right) / 2;         // punto medio
    mergeSort(registros, left, middle);      // ordena la mitad izquierda
    mergeSort(registros, middle + 1, right); // ordena la mitad derecha
    merge(registros, left, middle, right);   // combinar ambas mitades
  }
}

/////////////////////////////////////////////////////////////////////
void readFile(const string &filename, vector<Registro> &registros)
{
  ifstream file(filename);
  if (!file)
  {
    cout << "Error: No se puede abrir el archivo " << endl;
    return;
  }

  string linea;
  while (getline(file, linea))
  {
    istringstream ss(linea);
    Registro reg_aux;

    // lee la fecha
    string mes, dia, hora;
    ss >> mes >> dia >> hora;
    reg_aux.fecha = mes + " " + dia + " " + hora;

    // lee la ip con todo y puerto
    string ipConPuerto;
    ss >> ipConPuerto;

    // separa la ip del puerto
    size_t pos = ipConPuerto.find(':');
    if (pos != string::npos)
    {
      reg_aux.ip = ipConPuerto.substr(0, pos); // solo agarra la ip sin puerto
    }
    else
    {
      reg_aux.ip = ipConPuerto; // si algo falla pues toma todo como ip
    }

    // toma lo que queda como el mensaje
    getline(ss, reg_aux.mensaje);
    reg_aux.mensaje.erase(0, 1); // quita el espacio del inicio del mensaje

    // mete el registro encontrado al vector
    registros.push_back(reg_aux);
  }

  file.close(); // cierra el archivo
  cout << "Registros cargados correctamente " << endl;
}

/////////////////////////////////////////////////////////////////////

void SaveOnFile(string &filename, vector<Registro> &registros)
{
  ofstream file(filename); // abre el archivo en modo escritura
  if (!file)               // si el archivo no se abrio correctamente
  {
    cout << "No se pudo abrir el archivo" << endl;
    return;
  }

  for (const auto &registro : registros) // para todos los registros
  {
    // escribe cada registro en el archivo
    file << registro.fecha << " " << registro.ip << " " << registro.mensaje << endl;
  }

  file.close(); // cierra el archivo
  cout << "Registro ordenado y guardado exitosamente" << endl;
}

/////////////////////////////////////////////////////////////////////

ListaDoble searchByIp(const vector<Registro> &registros, const string &startIp, const string &endIp)
{
  ListaDoble listaOrdenada;                    // Lista que almacena los resultados
  unsigned long startNumIp = ipToNum(startIp); // convierte la ip de inicio a numero
  unsigned long endNumIp = ipToNum(endIp);     // convierte la ip de fin a numero

  for (const auto &registro : registros) // recorre todos los registros
  {
    unsigned long currentIp = ipToNum(registro.ip);       // convierte la ip actual a numero
    if (currentIp >= startNumIp && currentIp <= endNumIp) // si se encuentra dentro del rango
    {
      listaOrdenada.create(registro); // pasa los registros dentro del rango a la lista
    }
  }
  return listaOrdenada; // devuelve la lista ordenada con los registros
}

#endif
