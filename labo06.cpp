#include <iostream>
#include <iomanip>
#include <string>
#include <limits>
#include <algorithm>
using namespace std;

const int K = 5;

struct Direccion {
    string calle;
    int numero;
    string ciudad;
};

struct Estudiante {
    string nombre;
    int edad;
    float cal[K];
    Direccion dir;
};

void limpiarBuffer() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

string minusculas(string s) {
    transform(s.begin(), s.end(), s.begin(),
              [](unsigned char c){ return static_cast<char>(tolower(c)); });
    return s;
}

float promedio(const Estudiante* e) {
    float s = 0.0f;
    for (int i = 0; i < K; ++i) s += e->cal[i];
    return s / K;
}

void asegurarCapacidad(Estudiante *&arr, int &cap, int requerido) {
    if (requerido <= cap) return;
    int nuevaCap = max(cap * 2, requerido);
    Estudiante* nuevo = new Estudiante[nuevaCap];
    for (int i = 0; i < cap; ++i) nuevo[i] = arr[i];
    delete[] arr;
    arr = nuevo;
    cap = nuevaCap;
}

void leerEstudiante(Estudiante &e) {
    cout << "Nombre: ";
    getline(cin, e.nombre);
    while (e.nombre.empty()) {
        cout << "  (No vacio) Nombre: ";
        getline(cin, e.nombre);
    }

    cout << "Edad: ";
    while (!(cin >> e.edad) || e.edad < 0) {
        cout << "  Edad invalida. Intente de nuevo: ";
        cin.clear(); limpiarBuffer();
    }
    limpiarBuffer();

    cout << "Direccion ->\n";
    cout << "  Calle: "; getline(cin, e.dir.calle);
    while (e.dir.calle.empty()) {
        cout << "  (No vacio) Calle: ";
        getline(cin, e.dir.calle);
    }

    cout << "  Numero: ";
    while (!(cin >> e.dir.numero)) {
        cout << "  Numero invalido. Intente de nuevo: ";
        cin.clear(); limpiarBuffer();
    }
    limpiarBuffer();

    cout << "  Ciudad: "; getline(cin, e.dir.ciudad);
    while (e.dir.ciudad.empty()) {
        cout << "  (No vacio) Ciudad: ";
        getline(cin, e.dir.ciudad);
    }

    cout << "Ingrese " << K << " calificaciones (0 a 10):\n";
    for (int i = 0; i < K; ++i) {
        cout << "  Cal[" << i+1 << "]: ";
        while (!(cin >> e.cal[i]) || e.cal[i] < 0.0f || e.cal[i] > 10.0f) {
            cout << "    Valor invalido. Ingrese (0..10): ";
            cin.clear(); limpiarBuffer();
        }
    }
    limpiarBuffer();
}

void mostrarUno(const Estudiante* e, int idx) {
    cout << "\n[" << idx << "] " << e->nombre << " (edad " << e->edad << ")\n";
    cout << "  Direccion: " << e->dir.calle << " #" << e->dir.numero << ", " << e->dir.ciudad << "\n";
    cout << "  Calificaciones: ";
    cout << fixed << setprecision(2);
    for (int j = 0; j < K; ++j) cout << e->cal[j] << (j+1==K ? "" : "  ");
    cout << "\n  Promedio: " << promedio(e) << "\n";
}

void ingresarEstudiantes(Estudiante *&arr, int &n, int &cap) {
    int cuantos;
    cout << "¿Cuantos estudiantes desea ingresar? ";
    while (!(cin >> cuantos) || cuantos <= 0) {
        cout << "  Valor invalido. Intente de nuevo: ";
        cin.clear(); limpiarBuffer();
    }
    limpiarBuffer();

    asegurarCapacidad(arr, cap, n + cuantos);
    for (int i = 0; i < cuantos; ++i) {
        cout << "\n--- Estudiante #" << (n + 1) << " ---\n";
        leerEstudiante(arr[n]);
        ++n;
    }
    cout << "\n✓ Se ingresaron " << cuantos << " estudiante(s).\n";
}

void mostrarEstudiantes(const Estudiante *arr, int n) {
    if (n == 0) { cout << "No hay estudiantes registrados.\n"; return; }
    cout << "\n===== LISTA DE ESTUDIANTES (" << n << ") =====\n";
    for (int i = 0; i < n; ++i) mostrarUno(&arr[i], i);
    cout << "=============================================\n";
}

int buscarEstudiante(const Estudiante *arr, int n, const string &nombre) {
    string needle = minusculas(nombre);
    for (int i = 0; i < n; ++i) {
        if (minusculas(arr[i].nombre) == needle) return i;
    }
    return -1;
}

void modificarEstudiante(Estudiante *arr, int n) {
    if (n == 0) { cout << "No hay estudiantes para modificar.\n"; return; }

    string nombre;
    cout << "Ingrese el nombre del estudiante a modificar: ";
    getline(cin, nombre);

    int idx = buscarEstudiante(arr, n, nombre);
    if (idx == -1) {
        cout << "No se encontro el estudiante \"" << nombre << "\".\n";
        return;
    }

    cout << "\nEstudiante encontrado:\n";
    mostrarUno(&arr[idx], idx);

    cout << "\n--- Ingrese los NUEVOS datos (se reemplazaran) ---\n";
    leerEstudiante(arr[idx]);
    cout << "✓ Estudiante actualizado correctamente.\n";
}

void mostrarMenu() {
    cout << "\n=========== MENU ===========\n"
         << "1. Ingresar estudiantes\n"
         << "2. Mostrar estudiantes\n"
         << "3. Buscar estudiante\n"
         << "4. Modificar estudiante\n"
         << "5. Salir\n"
         << "Seleccione una opcion: ";
}

int main() {
    int cap = 2;
    int n = 0;
    Estudiante* alumnos = new Estudiante[cap];

    cout << fixed << setprecision(2);

    int opcion;
    do {
        mostrarMenu();
        if (!(cin >> opcion)) {
            cout << "Entrada invalida. Intente de nuevo.\n";
            cin.clear(); limpiarBuffer();
            continue;
        }
        limpiarBuffer();

        switch (opcion) {
            case 1:
                ingresarEstudiantes(alumnos, n, cap);
                break;
            case 2:
                mostrarEstudiantes(alumnos, n);
                break;
            case 3: {
                if (n == 0) { cout << "No hay estudiantes registrados.\n"; break; }
                string nombre;
                cout << "Nombre a buscar: ";
                getline(cin, nombre);
                if (nombre.empty()) { cout << "Nombre vacio.\n"; break; }
                int idx = buscarEstudiante(alumnos, n, nombre);
                if (idx == -1) cout << "No se encontro \"" << nombre << "\".\n";
                else mostrarUno(&alumnos[idx], idx);
                break;
            }
            case 4:
                modificarEstudiante(alumnos, n);
                break;
            case 5:
                cout << "Saliendo...\n";
                break;
            default:
                cout << "Opcion invalida.\n";
        }
    } while (opcion != 5);

    delete[] alumnos;
    return 0;
}