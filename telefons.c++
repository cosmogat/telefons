// 24-08-2011
// billy
// telefons.c++
#include <iostream>
#include <cstdlib>
#include <mysql++.h>
#include <string>
using namespace std;
using namespace mysqlpp;

int main (int num_arg, char * vec_arg[]) {
  string cerca, inserta_nom, inserta_tel, elimina;
  bool cercar = false, insertar = false, eliminar = false;
  int i = 0;
  while (i < num_arg && (!cercar || !insertar || !eliminar )) {
    if ((string) vec_arg[i] == "-s") {
      cerca = i + 1 < num_arg ? (string) vec_arg[i + 1] : "";
      cercar = true;
    }
    if ((string) vec_arg[i] == "-i") {
      if (i + 2 < num_arg) {
	inserta_nom = (string) vec_arg[i + 1];
	inserta_tel = (string) vec_arg[i + 2];
	insertar = true;
      }
      else
	insertar = false;
    }
    if ((string) vec_arg[i] == "-d") {
      if (i + 1 < num_arg) {
	elimina = (string) vec_arg[i + 1];
	eliminar = true;
      }
      else
	eliminar = false;
    }
    i++;
  }
  if (!cercar && !insertar && !eliminar) {
    cout << "\033[36mUtilitzeu:\033[0m" << endl;
    cout << "\033[37m" << vec_arg[0] << " opcio valor\033[0m" << endl;
    cout << "\033[32mopcio:\033[0m" << endl;
    cout << "\t \033[33m-s:\033[0m\t cercar" << endl;
    cout << "\t \033[33m-i:\033[0m\t afegir" << endl;
    cout << "\t \033[33m-d:\033[0m\t eliminar" << endl;
    cout << "\033[32mvalor:\033[0m" << endl;
    cout << "\t nom i/o número de telèfon" << endl;
    cout << "\033[32mExemples:\033[0m" << endl;
    cout << "\033[37m" << vec_arg[0] << " -s\033[0m                                   mostrar tots els telèfons" << endl;
    cout << "\033[37m" << vec_arg[0] << " -s \"Billy McFly\"\033[0m                     mostrar tots els telèfons de les persones que s'anomenen \"Billy McFly\"" << endl;
    cout << "\033[37m" << vec_arg[0] << " -i \"Billy McFly\" 123456789\033[0m           afegir \"Billy McFly\" amb el telèfon 123456789" << endl;
    cout << "\033[37m" << vec_arg[0] << " -d \"Billy McFly\"\033[0m                     borrarà l'entrada de \"Billy McFly\"" << endl;
    exit (1);
  }
  try {
    Connection conn(false);
    conn.connect("bdtelefons", "cosmogat_server", "cosmogat_user", "cosmogat_pass");
    if (cercar) {
      Query cons = conn.query();
      cons << "SELECT * FROM Telefons WHERE Nom LIKE '%" << cerca << "%' OR Telefon LIKE '%" << cerca << "%' ORDER BY Nom";
      StoreQueryResult vec = cons.store();
      for (size_t i = 0; i < vec.num_rows(); i++)
	cout << vec[i]["Nom"] << ": " << vec[i]["Telefon"] << endl;
    }
    else if (insertar) {
      Query prova = conn.query();
      prova << "SELECT * FROM Telefons WHERE Nom = '" << inserta_nom << "'";
      StoreQueryResult vec = prova.store();
      if (vec.num_rows() == 0) {
	Query cons = conn.query();
	cons << "INSERT INTO Telefons (Nom, Telefon) VALUES ('" << inserta_nom << "', '" << inserta_tel << "')";
	cons.execute();
	cout << "S'ha afegit " << inserta_nom << " amb el telèfon " << inserta_tel << endl;
      }
      else {
	cout << "La persona " << inserta_nom << " ja existeix" << endl;
	exit (-2);
      }
    }
    else if (eliminar) {
      Query prova = conn.query();
      prova << "SELECT * FROM Telefons WHERE Nom = '" << elimina << "' OR Telefon = '" << elimina << "'"; 
      StoreQueryResult vec = prova.store();
      if (vec.num_rows() != 0) {
	Query cons = conn.query();
	cons << "DELETE FROM Telefons WHERE Nom = '" << elimina << "' OR Telefon = '" << elimina << "'";
	cons.execute();
	cout << "S'han eliminat les següents persones:" << endl;
	for (size_t i = 0; i < vec.num_rows(); i++)
	  cout << vec[i]["Nom"] << ": " << vec[i]["Telefon"] << endl;
      }
      else {
	cout << "No hi ha ningú amb " << elimina << " com a nom o com a número de telèfon " << endl;
	exit (-3);
      }
    }
  }
  catch (BadQuery er) {
    cerr << "Error: " << er.what() << endl;
    return -1;
  }
  catch (const BadConversion& er) {
    cerr << "Error de conversió: " << er.what() << endl <<
      "\ttamany de data consultada: " << er.retrieved <<
      ", tamany actual: " << er.actual_size << endl;
    return -1;
  }
  catch (const Exception& er) {
    cerr << "Error: " << er.what() << endl;
    return -1;
  }
  
  return 0;
}
