#include <iostream>
#include <vector>
#include <string>
#include "functions.h"

using namespace std;

int main()
{
  vector<Registro> registros;                  // Vector para almacenar los registros cargados
  string inputFile = "bitacora.txt";           // archivo de entrada
  string outputFile = "bitacora_ordenada.txt"; // archivo donde se guardan los registros ordenados

  // Carga los registros desde el archivo de entrada
  cout << "Cargando registros del archivo " << inputFile << endl;
  readFile(inputFile, registros); // Carga los registros al vector

  // Verificar si los registros fueron cargados correctamente
  if (registros.empty())
  {
    cout << "No se encontraron registros en el archivo o el archivo este vacio" << endl;
    return 1; // Termina el programa si no hay registros
  }

  // Ordena los registros por ip
  cout << "Ordenando registros por IP..." << endl;
  mergeSort(registros, 0, registros.size() - 1); // Ordena los registros
  cout << "Registros ordenados correctamente" << endl;

  // guardar los registros ordenados en el archivo
  SaveOnFile(outputFile, registros);

  // Solicita al usuario el rango de ips despues de ser ordenadoss
  string ipInicio, ipFin;
  cout << "Ingrese la IP de inicio (e.g., 192.168.1.1): ";
  getline(cin, ipInicio);
  cout << "Ingrese la IP de fin (e.g., 192.168.3.50): ";
  getline(cin, ipFin);

  // Busca los registros dentro del rango dado
  cout << "Buscando registros que coincidan con el rango..." << endl;
  ListaDoble resultados = searchByIp(registros, ipInicio, ipFin);

  // Verificar si se encontraron registros
  if (!resultados.getHead())
  {
    cout << "No se encontraron registros en el rango especificado.\n";
  }
  else
  {
    cout << "Registros encontrados" << endl;
  }

  // Menu de interaccion con la lista de registros en el rango
  int opcion;
  do
  {
    cout << "=== Menú de Navegación ===" << endl;
    cout << "1. Ver siguiente registro" << endl;
    cout << "2. Ver el registro anterior" << endl;
    cout << "3. Salir" << endl;
    cout << "Seleccione una opción: ";
    cin >> opcion;

    switch (opcion)
    {
    case '1':
      resultados.nextNode();
      break;
    case '2':
      resultados.prevNode();
      break;
    case '3':
      cout << "Saliendo del programa...\n";
      break;
    default:
      cout << "Opción no válida. Intente nuevamente.\n";
      break;
    }
  } while (opcion != '3');

  cout << "\nPrograma finalizado. Gracias por usar el sistema.\n";
  return 0;
}
