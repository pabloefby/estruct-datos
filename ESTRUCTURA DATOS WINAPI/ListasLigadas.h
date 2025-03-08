#pragma once
#include<iostream>

using namespace std;


class Usuarios { 
	public:
		char claveUsuario[22];
		char apellidoPaterno[22];
		char apellidoMaterno[22];
		char nombres[42];
		char NombreCompleto[80];
		char password[22];

		Usuarios(){}
};

class Medicos {
	public:
		char claveUsuario[20];

		int cedula;
		char cedulaTXT[10];

		char apellidoPaterno[22];
		char apellidoMaterno[22];
		char nombres[42];
		char NombreCompleto[80];

		char especialidad[30]; 

		char numconsultorio[4];

		int horario; //1 matutino 2 vespertino
		char foto[300];
		char telefono[12];

		bool lunes;
		bool martes;
		bool miercoles;
		bool jueves;
		bool viernes;

		Medicos(){}

};


class Pacientes {
public:
	char claveUsuario[20];

	char apellidoPaterno[22];
	char apellidoMaterno[22];
	char nombres[42];
	char NombreCompleto[80];
	int genero; //1 hombre 2 mujer
	char telefono[12];
	char referencia[200];
	SYSTEMTIME fechanacimiento;
	int diaNacimiento;
	int mesNacimiento;
	int añoNacimiento;

	int edad;
	char edadTXT[4];

	char NombreDoctor[80];

	Pacientes(){}

};

class Citas {

public:
	char claveUsuario[20];

	int numeroCita;
	char citaNumero[5];	

	char NombrePaciente[80];
	char telefono[10];
	char nombreEspecialidad[30];
	char NombreMedico[80];

	int hora;
	char horaTxt[5];
	int minutos;
	char minutosTxt[5];

	SYSTEMTIME fechacita;
	char numconsultorio[4];

	char foto[300];

	int estadoCita; //0 pendiente, 1 aceptada, 2 cancelada
	char descripcionCancelacion[300];

	Citas(){}

};


//CLASE TEMPLATE PARA NODOS
template<class U>class Nodo { 
public:

	U info;		
	Nodo* ant; 
	Nodo* sig; 


	Nodo(U data) { 
		this->info = data;
	}

	~Nodo() {}; 

	U getData() { return info; }; 
};


//CLASE TEMPLATE PARA CREAR LISTAS LIGADAS SEGUN QUE NODO

template <class T, class U> class ListaLigada { 


private:
	T* inicio; 
	T* final;  
	char archivo[255];  

	//BUSQUEDA BINARIA
	T* middle(T* start, T* last) {

		if (start == nullptr) {
			return nullptr;
		}

		T* slow = start;
		T* fast = start->sig;

		while (fast != last) {

			fast = fast->sig;
			if (fast != last) {
				slow = slow->sig;
				fast = fast->sig;
			}

		}
		return slow;

	}

	//QUICKSORT
	T*particion(T*low,T*high){
	
		int pivote = high->getData().cedula;

		T* i = low->ant;

		for (T* j = low; j != high; j = j->sig) {

			if (j->getData().cedula <= pivote) {
				i = (i == nullptr) ? low : i->sig;
				swap(i, j);
			}
		}

		i = (i == nullptr) ? low : i->sig;
		swap(i, high);
		return i;

	}
	void quicksortRecursivo(T* low, T* high){
	
		if (high != nullptr && low != high && low != high->sig) {

			T* p = particion(low, high);
			if (p != nullptr) {
				quicksortRecursivo(low, p->ant);
				quicksortRecursivo(p->sig, high);
			}

		}

	}
	void swap(T*a,T*b){
		U temp = a->getData();
		a->info = b->getData();
		b->info = temp;
	}

	//HEAPSORT

	void construirMonticulo(T*arr[],int n){
		for (int i = n / 2 - 1; i >= 0; i--) {
			heapify(arr, n, i);
		}
	}
	void swap2(T*&a,T*&b){
		T* temp = a;
		a = b;
		b = temp;
	}
	void heapify(T*arr[],int n,int i){
	
		int max = i;
		int left = 2 * i + 1;
		int right = 2 * i + 2;

		if (left < n && strcmp(arr[left]->info.NombreCompleto, arr[max]->info.NombreCompleto)>0) {
			max = left;
		}

		if (right < n && strcmp(arr[right]->info.NombreCompleto, arr[max]->info.NombreCompleto)>0) {
			max = right;
		}

		if (max != i) {
			swap2(arr[i], arr[max]);
			heapify(arr, n, max);
		}

	}

public: 

	ListaLigada(char* nomArchivo); 
	ListaLigada(char* nomArchivo,int n);

	T* getInicio();
	T* getFinal();
	void agregarNodo(T* nuevo);
	void eliminarNodo(T* eliminar);
	void modificarNodo(T* modificar, U nuevo);

	void GuardarArchivo();
	void GuardarArchivo(int n); 

	T* BusquedaBinaria(T* node, int dato);

	void quicksort();
	void heapsort();


	~ListaLigada();
};


//FUNCIONES TEMPLATES DEFINIDAS

template<class T, class U>
ListaLigada<T, U>::ListaLigada(char* nomArchivo) { 

	this->inicio = nullptr;
	this->final = nullptr;
	strcpy_s(archivo, nomArchivo);
	T* aux = nullptr;
	U temp;

	if (strcmp(archivo, "") != 0) {

		ifstream file;

		file.open(archivo, ios::in | ios::binary);

		if (file.is_open()) {

			file.read((char*)&temp, sizeof(U));
			while (!file.eof()) {

				aux = new T(temp);
				agregarNodo(aux);
				file.read((char*)&temp, sizeof(U));
			}
			file.close();
		}
	}
}

template<class T, class U>
ListaLigada<T, U>::ListaLigada(char* nomArchivo,int n) { 

	this->inicio = nullptr;
	this->final = nullptr;
	strcpy_s(archivo, nomArchivo);
	T* aux = nullptr;
	U temp;

	if (strcmp(archivo, "") != 0) {

		ifstream file;

		file.open(archivo, ios::in);

		if (file.is_open()) {

			file.read((char*)&temp, sizeof(U));
			while (!file.eof()) {

				aux = new T(temp);
				agregarNodo(aux);
				file.read((char*)&temp, sizeof(U));
			}
			file.close();
		}
	}
}



template<class T, class U>
T* ListaLigada<T, U>::getInicio() { //obtenemos el puntero del inicio de la lista 
	return this->inicio;
}

template<class T, class U>
T* ListaLigada<T, U>::getFinal() { //obtenemos el puntero del inicio de la lista 
	return this->final;
}



template<class T, class U>
void ListaLigada<T, U>::agregarNodo(T* nuevo) { 

	nuevo->sig = nullptr;
	nuevo->ant = nullptr;

	if (this->inicio == nullptr) { 

		this->inicio = nuevo;
		this->final = this->inicio;

	}
	else { 
		this->final->sig = nuevo;
		nuevo->ant = this->final;
		this->final = nuevo;

	}

}

template<class T, class U>
void ListaLigada<T, U>::eliminarNodo(T* eliminar) { 


	if (eliminar == this->inicio) {
		inicio = eliminar->sig;
	}

	if (eliminar == final) {
		final = eliminar->ant;
	}
	if (eliminar->ant != nullptr) {
		eliminar->ant->sig = eliminar->sig;
	}
	if (eliminar->sig != nullptr) {
		eliminar->sig->ant = eliminar->ant;
	}

	delete eliminar;

}

template <class T, class U>
void ListaLigada<T, U>::modificarNodo(T* modificar, U nuevo) { 

	modificar->info = nuevo;

}



template<class T, class U>
void ListaLigada<T, U>::GuardarArchivo() {

	ofstream file;

	file.open(archivo,ios::out| ios::binary | ios::trunc);

	if (file.is_open()) {

		while (this->inicio != nullptr) {
			T* next = this->inicio->sig;

			file.write((char*)&inicio->info, sizeof(U));
			delete this->inicio;
			this->inicio = next;

		}
		file.close();

	}

}

template<class T, class U>
void ListaLigada<T, U>::GuardarArchivo(int n) {

	ofstream file;

	file.open(archivo, ios::out|ios::trunc);

	if (file.is_open()) {

		while (this->inicio != nullptr) {
			T* next = this->inicio->sig;

			file.write((char*)&inicio->info, sizeof(U));
			delete this->inicio;
			this->inicio = next;

		}
		file.close();

	}

}



template<class T, class U>
void ListaLigada<T, U>::quicksort() {
	quicksortRecursivo(this->inicio, this->final);
}

template<class T, class U>
void ListaLigada<T, U>::heapsort() {

	int n = 0;

	T* temp = inicio;

	while (temp != nullptr) {
		n++;
		temp = temp->sig;
	}

	T** arr = new T * [n];
	temp = inicio;

	for (int i = 0; i < n; i++) {
		arr[i] = temp;
		temp = temp->sig;
	}

	construirMonticulo(arr, n);

	for (int i = n - 1; i > 0; i--) {
		swap2(arr[0], arr[i]);
		heapify(arr, i, 0);
	}

	inicio = arr[0];
	final = arr[n - 1];

	for (int i = 0; i < n - 1; i++)
		arr[i]->sig = arr[i + 1];

	arr[n - 1]->sig = nullptr;

}




template<class T, class U>
T* ListaLigada<T, U>::BusquedaBinaria(T* node, int dato) {

	T* start = node;
	T* last = nullptr;

	do {

		T* mid = middle(start, last);

		if (mid == nullptr) {
			return nullptr;
		}

		if (mid->getData().cedula == dato) {
			return mid;
		}
		else if (mid->getData().cedula < dato) {
			start = mid->sig;
		}
		else {
			last = mid;
		}

	} while (last == nullptr || last != start);

	return nullptr;

}


template<class T, class U>
ListaLigada<T, U>::~ListaLigada() { //destructor guarda todo en archivo

	//ofstream file;

	//file.open(archivo, ios::binary | ios::trunc);

	//if (file.is_open()) {

	//	while (this->inicio != nullptr) {
	//		T* next = this->inicio->sig;

	//		file.write((char*)&inicio->info, sizeof(U));
	//		delete this->inicio;
	//		this->inicio = next;

	//	}
	//	file.close();

	//}

}
