#pragma once
#include<iostream>
#include<fstream>
#include<Windows.h>
#include<CommCtrl.h>

using namespace std;


class Especialidades {

public:

	char claveUsuario[20];
	char nombre[32];

	int clave;
	char claveTXT[10];

	char descripcion[100];

};


class NodoArbol {

public:
	Especialidades data;
	NodoArbol* izq;
	NodoArbol* der;

	NodoArbol(Especialidades nuevo) {

		data = nuevo;
		izq = nullptr;
		der = nullptr;

	}

};


class Arbol {

private:

	NodoArbol* raiz;

	//sobrecarga de funciones

	void InsertarNodo(Especialidades nuevo, NodoArbol* nodo) { //sobrecarga

		if (nuevo.clave < nodo->data.clave) {

			if (nodo->izq == nullptr) {
				nodo->izq = new NodoArbol(nuevo);
			}
			else {
				InsertarNodo(nuevo, nodo->izq);
			}

		}
		else {

			if (nodo->der == nullptr) {
				nodo->der = new NodoArbol(nuevo);
			}
			else {
				InsertarNodo(nuevo, nodo->der);
			}

		}


	}

	NodoArbol* Buscar(int dato, NodoArbol* nodo) {

		if (nodo == nullptr) {
			return nullptr;
		}

		if (dato == nodo->data.clave) {
			return nodo;
		}

		if (dato < nodo->data.clave) {
			return Buscar(dato, nodo->izq);
		}
		else {
			return Buscar(dato, nodo->der);
		}

	}//fin de buscar

	NodoArbol* BuscarEsp(const char* nombre, NodoArbol* nodo) {

		if (nodo == nullptr||strcmp(nodo->data.nombre,nombre)==0) {
			return nodo;
		}

		NodoArbol* izquierda = BuscarEsp(nombre,nodo->izq);

		if (izquierda != nullptr) {
			return izquierda;
		}

		return BuscarEsp(nombre, nodo->der);

	}

	void guardarNodoArchivo(NodoArbol* nodo, ofstream& archivo) {

		if (nodo) {

			archivo.write((char*)&nodo->data, sizeof(Especialidades));
			guardarNodoArchivo(nodo->izq, archivo);
			guardarNodoArchivo(nodo->der, archivo);

		}

	}

	NodoArbol* EliminarNodo(NodoArbol* nodo, int dato) {

		if (nodo == nullptr) {
			return nodo;
		}

		if (dato < nodo->data.clave) {
			nodo->izq = EliminarNodo(nodo->izq, dato);
		}
		else if (dato > nodo->data.clave) {
			nodo->der = EliminarNodo(nodo->der, dato);
		}
		else {
			// Nodo encontrado para eliminar
			if (nodo->izq == nullptr) { //si no tiene hijo izquierdo
				NodoArbol* temp = nodo->der;
				delete nodo;
				return temp;
			}
			else if (nodo->der == nullptr) { //si no tiene hijo derecho
				NodoArbol* temp = nodo->izq;
				delete nodo;
				return temp;
			}
			else { //si tiene amobos hijos
				NodoArbol* sucesor = EncontrarMinimo(nodo->der);
				nodo->data = sucesor->data;
				nodo->der = EliminarNodo(nodo->der, sucesor->data.clave);
			}
		}
		return nodo;
	}

	NodoArbol* EncontrarMinimo(NodoArbol* nodo) {
		while (nodo->izq != nullptr) {
			nodo = nodo->izq;
		}
		return nodo;
	}

public:

	Arbol() {
		raiz = nullptr;
	}

	bool Vacio() {

		return raiz == nullptr;

	}

	NodoArbol* GetRaiz() {
		return raiz;
	}

	void leerarchivo() {

		raiz = nullptr;
		NodoArbol* aux = nullptr;
		Especialidades temp;

		ifstream archivo;

		archivo.open("Archivos\\Especialidades.bin", ios::in | ios::binary);

		if (archivo.is_open()) {

			archivo.read((char*)&temp, sizeof(Especialidades));

			while (!archivo.eof()) {

				InsertarNodo(temp);
				archivo.read((char*)&temp, sizeof(Especialidades));
			}
			archivo.close();

		}


	}

	void guardararchivo() {

		ofstream archivo;
		NodoArbol* aux = raiz;

		archivo.open("Archivos\\Especialidades.bin", ios::binary | ios::trunc);

		if (archivo.is_open()) {

			guardarNodoArchivo(aux, archivo);


		}
		archivo.close();

	}

	void InsertarNodo(Especialidades nuevo) {

		if (Vacio()) {

			raiz = new NodoArbol(nuevo);

		}
		else {

			InsertarNodo(nuevo, raiz);

		}

	}

	NodoArbol* Buscar(int dato) {
		return Buscar(dato, raiz);
	}

	NodoArbol* BuscarEsp(const char* nombre) {
		return BuscarEsp(nombre, raiz);
	}

	void EliminarNodo(int dato) {
		raiz = EliminarNodo(raiz, dato);
	}

	void LlenarCombobox(HWND combobox, NodoArbol* nodo) {

		if (nodo == nullptr) {
			return;
		}

		LlenarCombobox(combobox, nodo->izq);
		SendMessage(combobox, CB_ADDSTRING, 0, (LPARAM)nodo->data.nombre);
		LlenarCombobox(combobox, nodo->der);

	}
	

};