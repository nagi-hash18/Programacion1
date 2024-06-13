#include <mysql.h>
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

class Vuelo {
public:
    int id_vuelo;
    string nombre_pasajero;
    string lugar_salida;
    string lugar_llegada;
    string fecha;
    string hora_salida;
    string hora_llegada;
    string num_vuelo;

    Vuelo(int id_vuelo, const string& nombre_pasajero, const string& lugar_salida, const string& lugar_llegada, const string& fecha,
        const string& hora_salida, const string& hora_llegada, const string& num_vuelo)
        : id_vuelo(id_vuelo), nombre_pasajero(nombre_pasajero), lugar_salida(lugar_salida), lugar_llegada(lugar_llegada),
        fecha(fecha), hora_salida(hora_salida), hora_llegada(hora_llegada), num_vuelo(num_vuelo) {}

    void mostrar() const {
        cout << "\033[38;5;230m" << setw(10) << id_vuelo << setw(20) << nombre_pasajero << setw(20) << lugar_salida
            << setw(20) << lugar_llegada << setw(15) << fecha << setw(15) << hora_salida
            << setw(15) << hora_llegada << setw(15) << num_vuelo << "\033[0m" << endl;
    }
};

class CRUDVuelos {
private:
    MYSQL* connector;

public:
    CRUDVuelos() {
        connector = mysql_init(0);
        connector = mysql_real_connect(connector, "localhost", "root", "nagi1808@etz", "project", 3306, NULL, 0);
        if (connector) {
            cout << "La conexión ha sido un éxito..." << endl;
        }
        else {
            cout << "Error en la conexión..." << endl;
            exit(1);
        }
    }

    ~CRUDVuelos() {
        if (connector) {
            mysql_close(connector);
        }
    }

    void crear_vuelo(int id_vuelo, const string& nombre_pasajero, const string& lugar_salida, const string& lugar_llegada,
        const string& fecha, const string& hora_salida, const string& hora_llegada, const string& num_vuelo) {
        string query = "INSERT INTO flights (`ID Vuelo`, `Pasajero`, `Lugar de Salida`, `Lugar de Llegada`, `Fecha`, `Hora de salida`, `Hora de llegada`, `Vuelo`) VALUES ("
            + to_string(id_vuelo) + ", '" + nombre_pasajero + "', '" + lugar_salida + "', '" + lugar_llegada + "', '"
            + fecha + "', '" + hora_salida + "', '" + hora_llegada + "', '" + num_vuelo + "')";
        const char* q = query.c_str();
        if (mysql_query(connector, q)) {
            cerr << "Error al insertar el vuelo: " << mysql_error(connector) << endl;
        }
        else {
            cout << "Vuelo creado exitosamente" << endl;
        }
    }

    void leer_vuelos() const {
        if (mysql_query(connector, "SELECT * FROM flights")) {
            cerr << "Error al obtener vuelos: " << mysql_error(connector) << endl;
            return;
        }
        MYSQL_RES* res = mysql_store_result(connector);
        if (res == NULL) {
            cerr << "Error al almacenar resultado: " << mysql_error(connector) << endl;
            return;
        }
        MYSQL_ROW row;
        // Colores: Celeste para cabecera, Azul marino para línea, Beige para registros
        cout << "\033[1;36m" << setw(10) << "ID" << setw(20) << "Pasajero" << setw(20) << "Salida"
            << setw(20) << "Llegada" << setw(15) << "Fecha" << setw(15) << "Salida"
            << setw(15) << "Llegada" << setw(15) << "Vuelo" << "\033[0m" << endl;
        cout << "\033[38;5;18m" << string(140, '-') << "\033[0m" << endl;
        while ((row = mysql_fetch_row(res))) {
            cout << "\033[38;5;230m" << setw(10) << row[0] << setw(20) << row[1] << setw(20) << row[2]
                << setw(20) << row[3] << setw(15) << row[4] << setw(15) << row[5]
                << setw(15) << row[6] << setw(15) << row[7] << "\033[0m" << endl;
        }
        mysql_free_result(res);
    }

    void buscar_pasajeros_por_vuelo(const string& num_vuelo) const {
        string query = "SELECT `ID Vuelo`, `Pasajero`, `Lugar de Salida`, `Lugar de Llegada`, `Fecha`, `Hora de salida`, `Hora de llegada`, `Vuelo` FROM flights WHERE `Vuelo` = '" + num_vuelo + "'";
        const char* q = query.c_str();
        if (mysql_query(connector, q)) {
            cerr << "Error al buscar pasajeros por vuelo: " << mysql_error(connector) << endl;
            return;
        }
        MYSQL_RES* res = mysql_store_result(connector);
        if (res == NULL) {
            cerr << "Error al almacenar resultado: " << mysql_error(connector) << endl;
            return;
        }
        MYSQL_ROW row;
        // Colores: Celeste para cabecera, Azul marino para línea, Beige para registros
        cout << "\033[1;36m" << setw(10) << "ID" << setw(20) << "Pasajero" << setw(20) << "Salida"
            << setw(20) << "Llegada" << setw(15) << "Fecha" << setw(15) << "Salida"
            << setw(15) << "Llegada" << setw(15) << "Vuelo" << "\033[0m" << endl;
        cout << "\033[38;5;18m" << string(140, '-') << "\033[0m" << endl;
        while ((row = mysql_fetch_row(res))) {
            cout << "\033[38;5;230m" << setw(10) << row[0] << setw(20) << row[1] << setw(20) << row[2]
                << setw(20) << row[3] << setw(15) << row[4] << setw(15) << row[5]
                << setw(15) << row[6] << setw(15) << row[7] << "\033[0m" << endl;
        }
        mysql_free_result(res);
    }

    void actualizar_vuelo(int id_vuelo, const string& nuevo_pasajero = "", const string& nuevo_salida = "",
        const string& nuevo_llegada = "", const string& nueva_fecha = "",
        const string& nueva_salida = "", const string& nueva_llegada = "",
        const string& nuevo_num_vuelo = "") {
        string query = "UPDATE flights SET ";
        bool first = true;
        if (!nuevo_pasajero.empty()) {
            query += " `Pasajero` = '" + nuevo_pasajero + "'";
            first = false;
        }
        if (!nuevo_salida.empty()) {
            if (!first) query += ", ";
            query += " `Lugar de Salida` = '" + nuevo_salida + "'";
            first = false;
        }
        if (!nuevo_llegada.empty()) {
            if (!first) query += ", ";
            query += " `Lugar de Llegada` = '" + nuevo_llegada + "'";
            first = false;
        }
        if (!nueva_fecha.empty()) {
            if (!first) query += ", ";
            query += " `Fecha` = '" + nueva_fecha + "'";
            first = false;
        }
        if (!nueva_salida.empty()) {
            if (!first) query += ", ";
            query += " `Hora de salida` = '" + nueva_salida + "'";
            first = false;
        }
        if (!nueva_llegada.empty()) {
            if (!first) query += ", ";
            query += " `Hora de llegada` = '" + nueva_llegada + "'";
            first = false;
        }
        if (!nuevo_num_vuelo.empty()) {
            if (!first) query += ", ";
            query += " `Vuelo` = '" + nuevo_num_vuelo + "'";
        }
        query += " WHERE `ID Vuelo` = " + to_string(id_vuelo);
        const char* q = query.c_str();
        if (mysql_query(connector, q)) {
            cerr << "Error al actualizar el vuelo: " << mysql_error(connector) << endl;
        }
        else {
            cout << "Vuelo actualizado exitosamente" << endl;
        }
    }

    void eliminar_vuelo(int id_vuelo) {
        string query = "DELETE FROM flights WHERE `ID Vuelo` = " + to_string(id_vuelo);
        const char* q = query.c_str();
        if (mysql_query(connector, q)) {
            cerr << "Error al eliminar el vuelo: " << mysql_error(connector) << endl;
        }
        else {
            cout << "Vuelo eliminado exitosamente" << endl;
        }
    }
};

int main() {
    CRUDVuelos crud;
    int opcion;
    do {
        cout << "\n\033[38;5;48m=== Bienvenido al sistema de vuelos ===\033[0m\n";
        cout << "\033[38;5;82m1. Crear vuelo\033[0m\n";
        cout << "\033[38;5;82m2. Leer vuelos\033[0m\n";
        cout << "\033[38;5;82m3. Actualizar vuelo\033[0m\n";
        cout << "\033[38;5;82m4. Eliminar vuelo\033[0m\n";
        cout << "\033[38;5;82m5. Buscar pasajeros por vuelo\033[0m\n";
        cout << "\033[38;5;82m6. Salir\033[0m\n";
        cout << "Ingrese una opcion: ";
        cin >> opcion;
        cin.ignore();
        switch (opcion) {
        case 1: {
            int id;
            string pasajero, salida, llegada, fecha, h_salida, h_llegada, num_vuelo;
            cout << "Ingrese ID de vuelo: ";
            cin >> id;
            cin.ignore();
            cout << "Ingrese nombre del pasajero: ";
            getline(cin, pasajero);
            cout << "Ingrese lugar de salida: ";
            getline(cin, salida);
            cout << "Ingrese lugar de llegada: ";
            getline(cin, llegada);
            cout << "Ingrese fecha (AAAA-MM-DD): ";
            getline(cin, fecha);
            cout << "Ingrese hora de salida (HH:MM:SS): ";
            getline(cin, h_salida);
            cout << "Ingrese hora de llegada (HH:MM:SS): ";
            getline(cin, h_llegada);
            cout << "Ingrese numero de vuelo: ";
            getline(cin, num_vuelo);
            crud.crear_vuelo(id, pasajero, salida, llegada, fecha, h_salida, h_llegada, num_vuelo);
            break;
        }
        case 2: {
            crud.leer_vuelos();
            break;
        }
        case 3: {
            int id;
            string nuevo_pasajero, nuevo_salida, nuevo_llegada, nueva_fecha, nueva_salida, nueva_llegada, nuevo_num_vuelo;
            cout << "Ingrese ID del vuelo a actualizar: ";
            cin >> id;
            cin.ignore();
            cout << "Ingrese nuevo nombre del pasajero (o presione enter para omitir): ";
            getline(cin, nuevo_pasajero);
            cout << "Ingrese nuevo lugar de salida (o presione enter para omitir): ";
            getline(cin, nuevo_salida);
            cout << "Ingrese nuevo lugar de llegada (o presione enter para omitir): ";
            getline(cin, nuevo_llegada);
            cout << "Ingrese nueva fecha (o presione enter para omitir): ";
            getline(cin, nueva_fecha);
            cout << "Ingrese nueva hora de salida (o presione enter para omitir): ";
            getline(cin, nueva_salida);
            cout << "Ingrese nueva hora de llegada (o presione enter para omitir): ";
            getline(cin, nueva_llegada);
            cout << "Ingrese nuevo numero de vuelo (o presione enter para omitir): ";
            getline(cin, nuevo_num_vuelo);
            crud.actualizar_vuelo(id, nuevo_pasajero, nuevo_salida, nuevo_llegada, nueva_fecha, nueva_salida, nueva_llegada, nuevo_num_vuelo);
            break;
        }
        case 4: {
            int id;
            cout << "Ingrese ID del vuelo a eliminar: ";
            cin >> id;
            crud.eliminar_vuelo(id);
            break;
        }
        case 5: {
            string num_vuelo;
            cout << "Ingrese numero de vuelo: ";
            getline(cin, num_vuelo);
            crud.buscar_pasajeros_por_vuelo(num_vuelo);
            break;
        }
        case 6:
            cout << "Saliendo del sistema...\n";
            break;
        default:
            cout << "Opción no válida. Por favor, intenta de nuevo.\n";
        }
    } while (opcion != 6);

    return 0;
}

