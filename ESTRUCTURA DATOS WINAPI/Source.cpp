//JORGE PABLO FLORES BLANCO 1923061
//ESTRUCTURA DE DATOS 051

#include<Windows.h>
#include<iostream>
#include<fstream>
#include"resource.h"
#include<CommDlg.h>
#include<string.h>
#include<CommCtrl.h>
#include<windowsx.h>
#include<string.h>

#include"ListasLigadas.h"
#include"ArbolBinario.h"

using namespace std;

//VARIABLES GLOBALES
HINSTANCE hInstGlobal;
char zFile[MAX_PATH];
int numCitas=0;

char HorasSelc[11][3] = { "7","8","9" ,"10" ,"11" ,"12","13","14","15","16","17" };
char MinutosSelec[3][3] = { "00","20","40" };

//PROCEDIMIENTOS DE VENTANA
BOOL CALLBACK MAINMENU(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam); //1
BOOL CALLBACK PROCMEDICOS(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam); //2
BOOL CALLBACK PROCPACEINTES(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);//3
BOOL CALLBACK PROCCITAS(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);//4
BOOL CALLBACK CITASTODAS(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);//5
BOOL CALLBACK CITASXMEDICO(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);//6
BOOL CALLBACK CITASXSEMANA(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);//7
BOOL CALLBACK CITASXESPECIALIDAD(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);//8
BOOL CALLBACK PROCUSUARIO(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);//9
BOOL CALLBACK PROCESPECIALIDADES(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam); //10
BOOL CALLBACK PROCINICIOSESION(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

//FUNCION PARA MENU
void menu(WPARAM wParam, HWND hwnd); //para que jale el menu
void UsuariosTxt();

//VALIDACIONES
bool validarNombres(char nombre[42]);
bool validarApellido(char apellido[22]);
bool validarUsuario(char usuario[22]);
bool validarTelefono(char telefono[12]);
bool validarNumero(char numero[10]);
bool validarEspecialidad(char espe[32]);

//nombres de archivos
//char fileUsuarios[] = "C:\\Users\\jpfb0\\Desktop\\ESTRUCTURA DATOS WINAPI\\ESTRUCTURA DATOS WINAPI\\Archivos\\Usuarios.txt";

char fileUsuarios[] = "Archivos\\Usuarios.txt";
char fileMedicos[] = "Archivos\\Medicos.bin";
char filePacientes[] = "Archivos\\Pacientes.bin";
char fileCitas[] = "Archivos\\Citas.bin";

//PARA CREAR LISTA LIGADA DE USUARIOS
ListaLigada<Nodo<Usuarios>, Usuarios> ListaUsuarios(fileUsuarios,1); //creamos la lista
Nodo<Usuarios>* auxUsuarios ,*sesionIniciada= nullptr; //puntero auxiliar 

//LISTA LIGADA DE MEDICOS
ListaLigada<Nodo<Medicos>, Medicos> ListaMedicos(fileMedicos);
Nodo<Medicos>* auxMedicos = nullptr;

//ARBOL BINARIO DE BUSQUEDA PARA ESPECIALIDADES
Arbol* ArbolEspecialidad = new Arbol();
NodoArbol* auxEspecialidad = nullptr;

//LISTA LIGADA DE MEDICOS
ListaLigada<Nodo<Pacientes>, Pacientes> ListaPacientes(filePacientes);
Nodo<Pacientes>* auxPacientes = nullptr;

//LISTA LIGADA DE CITAS
ListaLigada<Nodo<Citas>, Citas>ListaCitas(fileCitas);
Nodo<Citas>* auxCitas = nullptr;


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow) {

	ArbolEspecialidad->leerarchivo(); //lee archivo de especialidades

	if (ListaCitas.getInicio() != nullptr) {
		numCitas = (ListaCitas.getFinal()->getData().numeroCita) + 1;
	}

	HWND Programa;
	hInstGlobal = hInst;

	if (ListaUsuarios.getInicio() == nullptr) {
		Programa = CreateDialog(hInst, MAKEINTRESOURCE(REG_USUARIO), NULL, PROCUSUARIO);
	}
	else {
		Programa = CreateDialog(hInst, MAKEINTRESOURCE(INICIO_SESION), NULL, PROCINICIOSESION);
	}

	ShowWindow(Programa, SW_SHOW); //muesta la ventana

	MSG mensajes;
	ZeroMemory(&mensajes, sizeof(mensajes));

	while (GetMessage(&mensajes, 0, 0, 0)) {
		TranslateMessage(&mensajes);
		DispatchMessage(&mensajes);
	}

	UsuariosTxt();
	ListaUsuarios.GuardarArchivo(1);
	ArbolEspecialidad->guardararchivo(); 
	ListaMedicos.GuardarArchivo();
	ListaPacientes.GuardarArchivo();
	ListaCitas.GuardarArchivo();

	return mensajes.wParam;
}

BOOL CALLBACK MAINMENU(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {

	case WM_CLOSE: {
		int opc = MessageBox(hwnd, "¿Seguro que desea salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);
		switch (opc) {
		case IDYES: {
			DestroyWindow(hwnd);
			PostQuitMessage(0); //Deja de ejecutar el programa
		}break;
		}
	}break; //CLOSE

	case WM_COMMAND: {

		menu(wParam, hwnd);

	}break;

	case WM_INITDIALOG: {

		SetDlgItemText(hwnd, IDC_USUARIO, sesionIniciada->getData().NombreCompleto);


	}break; //Initdialog

	}//msg

	return 0;
}

BOOL CALLBACK PROCCITAS(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {

	case WM_CLOSE: {
		int opc = MessageBox(hwnd, "¿Seguro que desea salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);
		switch (opc) {
		case IDYES: {
			DestroyWindow(hwnd);
			PostQuitMessage(0); //Deja de ejecutar el programa
		}break;
		}
	}break; //CLOSE

	case WM_INITDIALOG: {

		SetDlgItemText(hwnd, IDC_USUARIO, sesionIniciada->getData().NombreCompleto);
		SetDlgItemText(hwnd, IDC_CLAVE, sesionIniciada->getData().claveUsuario);

		if (ListaCitas.getInicio() != nullptr) {
			numCitas = (ListaCitas.getFinal()->getData().numeroCita) + 1;
		}

		SetDlgItemInt(hwnd, IDC_NUMCITA, numCitas,false);

		auxCitas = ListaCitas.getInicio(); //llenamos modificar citas
		while (auxCitas != nullptr) {
			SendDlgItemMessage(hwnd, IDC_COMBO4, CB_INSERTSTRING, (WPARAM)0, (LPARAM)auxCitas->getData().citaNumero);
			auxCitas = auxCitas->sig;
		}

		auxPacientes = ListaPacientes.getInicio(); //llenamos pacientes

		while (auxPacientes != nullptr) {
			SendDlgItemMessage(hwnd, IDC_PACIENTE, CB_INSERTSTRING, (WPARAM)0, (LPARAM)auxPacientes->getData().NombreCompleto);

			auxPacientes = auxPacientes->sig;
		}

		ArbolEspecialidad->LlenarCombobox(GetDlgItem(hwnd, IDC_ESPECIALIDAD), ArbolEspecialidad->GetRaiz()); //llenamos especialidades

		auxMedicos = ListaMedicos.getInicio();

		while (auxMedicos != nullptr) { //llena el combobox con nombres de medicos
			SendDlgItemMessage(hwnd, IDC_MEDICO, CB_INSERTSTRING, (WPARAM)0, (LPARAM)auxMedicos->getData().NombreCompleto);
			auxMedicos = auxMedicos->sig;
		}

		for (int i = 0; i < 11; i++) {
			SendDlgItemMessage(hwnd, IDC_HORA, CB_INSERTSTRING, (WPARAM)i, (LPARAM)HorasSelc[i]);
		}

		for (int j = 0; j < 3; j++) {
			SendDlgItemMessage(hwnd, IDC_MINUTOS, CB_INSERTSTRING, (WPARAM)j, (LPARAM)MinutosSelec[j]);
		}


	}break;

	case WM_COMMAND: {

		menu(wParam, hwnd);

		switch (LOWORD(wParam)) {

		case IDGUARDAR: { //GUARDAR

			Citas temporal;

			bool citaCorrecta = true;

			GetDlgItemText(hwnd, IDC_CLAVE, temporal.claveUsuario, sizeof(temporal.claveUsuario));

			temporal.numeroCita = GetDlgItemInt(hwnd, IDC_NUMCITA, NULL, false);
			GetDlgItemText(hwnd, IDC_NUMCITA, temporal.citaNumero, sizeof(temporal.citaNumero));

			GetDlgItemText(hwnd, IDC_PACIENTE, temporal.NombrePaciente, sizeof(temporal.NombrePaciente));
			GetDlgItemText(hwnd, IDC_TELEFONO, temporal.telefono, sizeof(temporal.telefono));
			GetDlgItemText(hwnd, IDC_ESPECIALIDAD, temporal.nombreEspecialidad, sizeof(temporal.nombreEspecialidad));
			GetDlgItemText(hwnd, IDC_MEDICO, temporal.NombreMedico, sizeof(temporal.NombreMedico));

			GetDlgItemText(hwnd, IDC_HORA, temporal.horaTxt,sizeof(temporal.horaTxt));


			GetDlgItemText(hwnd, IDC_MINUTOS, temporal.minutosTxt, sizeof(temporal.minutosTxt));

			SYSTEMTIME fechadelacita;
			ZeroMemory(&fechadelacita, sizeof(fechadelacita));
			SendDlgItemMessage(hwnd, IDC_FECHITA, DTM_GETSYSTEMTIME, 0, (LPARAM)&fechadelacita);

			temporal.fechacita = fechadelacita;

			GetDlgItemText(hwnd, IDC_CONSULTORIO, temporal.numconsultorio, sizeof(temporal.numconsultorio));

			temporal.estadoCita = 0;

			strcpy_s(temporal.foto, zFile);

			strcpy_s(temporal.descripcionCancelacion, "");

			if (auxPacientes != nullptr && auxMedicos!=nullptr) {
				if (strcmp(auxPacientes->getData().NombreDoctor, "") == 0) {
					strcpy_s(auxPacientes->info.NombreDoctor, auxMedicos->getData().NombreCompleto);
				}
			}

			if (strlen(temporal.NombrePaciente) == 0) {
				citaCorrecta = false;
				MessageBox(hwnd, "Seleccione uno de los pacientes registrados.", "ERROR", MB_OK | MB_ICONINFORMATION);
			}

			if (strlen(temporal.nombreEspecialidad) == 0) {
				citaCorrecta = false;
				MessageBox(hwnd, "Seleccione una de las especialidades registrados.", "ERROR", MB_OK | MB_ICONINFORMATION);
			}

			if (strlen(temporal.NombreMedico) == 0) {
				citaCorrecta = false;
				MessageBox(hwnd, "Seleccione uno de los medicos registrados.", "ERROR", MB_OK | MB_ICONINFORMATION);
			}

			if (strlen(temporal.horaTxt) == 0) {
				citaCorrecta = false;
				MessageBox(hwnd, "Seleccione una hora para la cita.", "ERROR", MB_OK | MB_ICONINFORMATION);
			}
			else {
				temporal.hora = GetDlgItemInt(hwnd, IDC_HORA, NULL, false);

				if (auxMedicos != nullptr) {
					if (temporal.hora > 12 && auxMedicos->getData().horario == 1) {
						citaCorrecta = false;
						MessageBox(hwnd, "El medico seleccionado no esta disponible en este horario.", "ERROR", MB_OK | MB_ICONINFORMATION);
					}

					if (temporal.hora <= 12 && auxMedicos->getData().horario == 2) {
						citaCorrecta = false;
						MessageBox(hwnd, "El medico seleccionado no esta disponible en este horario.", "ERROR", MB_OK | MB_ICONINFORMATION);
					}
				}
			
			}

			if (strlen(temporal.minutosTxt) == 0) {
				citaCorrecta = false;
				MessageBox(hwnd, "Seleccione minutos para la cita.", "ERROR", MB_OK | MB_ICONINFORMATION);
			}
			else {
				temporal.minutos = GetDlgItemInt(hwnd, IDC_MINUTOS, NULL, false);

			}

			SYSTEMTIME fechahoy;
			ZeroMemory(&fechahoy, sizeof(fechahoy));
			GetLocalTime(&fechahoy);

			if (fechadelacita.wYear >= fechahoy.wYear && (fechadelacita.wYear > fechahoy.wYear || (fechadelacita.wYear == fechahoy.wYear && fechadelacita.wMonth > fechahoy.wMonth) || (fechadelacita.wYear == fechahoy.wYear && fechadelacita.wMonth == fechahoy.wMonth && fechadelacita.wDay >= fechahoy.wDay))) {

				if (fechadelacita.wDayOfWeek == 0 || fechadelacita.wDayOfWeek == 6) {
					citaCorrecta = false;
					MessageBox(hwnd, "Lo sentimos, no se atiende los fines de semana.", "ERROR", MB_OK | MB_ICONINFORMATION);
				}
				else {

					if (auxMedicos != nullptr) {

						if (fechadelacita.wDayOfWeek == 1 && auxMedicos->getData().lunes == true) {
							//no hace nada pero no se me ocurrio otra forma de hacerlo :3
						}
						else if (fechadelacita.wDayOfWeek == 2 && auxMedicos->getData().martes == true) {

						}
						else if (fechadelacita.wDayOfWeek == 3 && auxMedicos->getData().miercoles == true) {

						}
						else if (fechadelacita.wDayOfWeek == 4 && auxMedicos->getData().jueves == true) {

						}
						else if (fechadelacita.wDayOfWeek == 5 && auxMedicos->getData().viernes == true) {

						}else {
							citaCorrecta = false;
							MessageBox(hwnd, "Lo sentimos, el medico no esta disponible el dia seleccionado.", "ERROR", MB_OK | MB_ICONINFORMATION);
						}
					}


				}

			}
			else {

				citaCorrecta = false;
				MessageBox(hwnd, "Lo sentimos, seleccione una fecha valida.", "ERROR", MB_OK | MB_ICONINFORMATION);

			}

			auxCitas = ListaCitas.getInicio();

			while (auxCitas != nullptr) {

				if (auxCitas->getData().fechacita.wDay == temporal.fechacita.wDay && auxCitas->getData().fechacita.wMonth == temporal.fechacita.wMonth && auxCitas->getData().fechacita.wDay == temporal.fechacita.wDay && auxCitas->getData().hora == temporal.hora && auxCitas->getData().minutos == temporal.minutos && strcmp(auxCitas->getData().numconsultorio, temporal.numconsultorio) == 0) {
					citaCorrecta = false;
					MessageBox(hwnd, "Ya hay una cita programada para esta fecha y este horario en este consultorio.", "ERROR", MB_OK | MB_ICONINFORMATION);
				}

				auxCitas = auxCitas->sig;
			}


			if(citaCorrecta){
			
				auxCitas = new Nodo<Citas>(temporal);

				ListaCitas.agregarNodo(auxCitas);

				MessageBox(hwnd, "Cita registrada exitosamente", "AVISO", MB_OK | MB_ICONINFORMATION);

				numCitas++;

				SetDlgItemText(hwnd, IDC_NUMCITA, "");

				SendMessage(GetDlgItem(hwnd, IDC_PACIENTE), CB_RESETCONTENT, 0, 0);
				auxPacientes = ListaPacientes.getInicio(); //llenamos pacientes
				while (auxPacientes != nullptr) {
					SendDlgItemMessage(hwnd, IDC_PACIENTE, CB_INSERTSTRING, (WPARAM)0, (LPARAM)auxPacientes->getData().NombreCompleto);
					auxPacientes = auxPacientes->sig;
				}

				SetDlgItemText(hwnd, IDC_TELEFONO, "");

				SendMessage(GetDlgItem(hwnd, IDC_ESPECIALIDAD), CB_RESETCONTENT, 0, 0);
				ArbolEspecialidad->LlenarCombobox(GetDlgItem(hwnd, IDC_ESPECIALIDAD), ArbolEspecialidad->GetRaiz());

				SendMessage(GetDlgItem(hwnd, IDC_MEDICO), CB_RESETCONTENT, 0, 0);
				auxMedicos = ListaMedicos.getInicio();
				while (auxMedicos != nullptr) { //llena el combobox con nombres de medicos
					SendDlgItemMessage(hwnd, IDC_MEDICO, CB_INSERTSTRING, (WPARAM)0, (LPARAM)auxMedicos->getData().NombreCompleto);
					auxMedicos = auxMedicos->sig;
				}

				SendMessage(GetDlgItem(hwnd, IDC_HORA), CB_RESETCONTENT, 0, 0);
				for (int i = 0; i < 11; i++) {
					SendDlgItemMessage(hwnd, IDC_HORA, CB_INSERTSTRING, (WPARAM)i, (LPARAM)HorasSelc[i]);
				}

				SendMessage(GetDlgItem(hwnd, IDC_MINUTOS), CB_RESETCONTENT, 0, 0);
				for (int j = 0; j < 3; j++) {
					SendDlgItemMessage(hwnd, IDC_MINUTOS, CB_INSERTSTRING, (WPARAM)j, (LPARAM)MinutosSelec[j]);
				}

				SendDlgItemMessage(hwnd, IDC_FECHITA, DTM_SETSYSTEMTIME, (WPARAM)GDT_VALID, (LPARAM)&fechahoy);

				SetDlgItemText(hwnd, IDC_CONSULTORIO, "");

				SetDlgItemInt(hwnd, IDC_NUMCITA, numCitas, false);

				SendMessage(GetDlgItem(hwnd, IDC_COMBO4), CB_RESETCONTENT, 0, 0);
				auxCitas = ListaCitas.getInicio(); //llenamos modificar citas
				while (auxCitas != nullptr) {
					SendDlgItemMessage(hwnd, IDC_COMBO4, CB_INSERTSTRING, (WPARAM)0, (LPARAM)auxCitas->getData().citaNumero);
					auxCitas = auxCitas->sig;
				}

				SendDlgItemMessage(hwnd, IDC_FOTOMEDICO, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)NULL);
			}

		}break;//GUARDAR

		case IDC_PACIENTE: {

			switch (HIWORD(wParam)) {
			case CBN_SELCHANGE: {

				char PacienteNombre[80];

				GetDlgItemText(hwnd, IDC_PACIENTE, PacienteNombre, 80);

				auxPacientes = ListaPacientes.getInicio();
				while (auxPacientes != nullptr && strcmp(PacienteNombre, auxPacientes->getData().NombreCompleto) != 0) {
					auxPacientes = auxPacientes->sig;
				}

				SetDlgItemText(hwnd, IDC_TELEFONO, auxPacientes->getData().telefono);


			}break;
			}

		}break; //pacientes

		case IDC_ESPECIALIDAD: {

			switch (HIWORD(wParam)) {
			case CBN_SELCHANGE: {

				SendDlgItemMessage(hwnd, IDC_FOTOMEDICO, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)NULL);
				SetDlgItemText(hwnd, IDC_CONSULTORIO, "");

				char EspecialidadBuscar[30];

				GetDlgItemText(hwnd, IDC_ESPECIALIDAD, EspecialidadBuscar, 30);

				SendMessage(GetDlgItem(hwnd, IDC_MEDICO), CB_RESETCONTENT, 0, 0);
				auxMedicos = ListaMedicos.getInicio();
				while (auxMedicos != nullptr) { //llena el combobox con nombres de medicos
					if (strcmp(EspecialidadBuscar, auxMedicos->getData().especialidad) == 0) {
						SendDlgItemMessage(hwnd, IDC_MEDICO, CB_INSERTSTRING, (WPARAM)0, (LPARAM)auxMedicos->getData().NombreCompleto);
					}
					auxMedicos = auxMedicos->sig;
				}

			}break;
			}

		}break; //especialidad

		case IDC_MEDICO: {
			switch (HIWORD(wParam)) {
			case CBN_SELCHANGE: {

				char nombreMedico[80];

				GetDlgItemText(hwnd, IDC_MEDICO, nombreMedico, 80);

				auxMedicos = ListaMedicos.getInicio();
				while (auxMedicos->sig != nullptr && strcmp(nombreMedico, auxMedicos->getData().NombreCompleto) != 0) {
					auxMedicos = auxMedicos->sig;
				}

				ComboBox_SelectString(GetDlgItem(hwnd, IDC_ESPECIALIDAD), 0, auxMedicos->getData().especialidad);

				HBITMAP bmp; //cargamos foto
				bmp = (HBITMAP)LoadImage(NULL, auxMedicos->getData().foto, IMAGE_BITMAP, 150, 150, LR_LOADFROMFILE); //2
				strcpy_s(zFile, auxMedicos->getData().foto);
				SendDlgItemMessage(hwnd, IDC_FOTOMEDICO, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);

				SetDlgItemText(hwnd, IDC_CONSULTORIO, auxMedicos->getData().numconsultorio);

			}break;

			}
		}break;//combomedico

		case IDC_COMBO4: {

			switch (HIWORD(wParam)) {
			case CBN_SELCHANGE: {
			
				EnableWindow(GetDlgItem(hwnd, IDGUARDAR), false); //desactiva guardar
				EnableWindow(GetDlgItem(hwnd, IDMODIFICAR), true); //activa modificar y eliminar
				EnableWindow(GetDlgItem(hwnd, IDELIMINAR), true);

				char CitaBuscar[5];

				GetDlgItemText(hwnd, IDC_COMBO4, CitaBuscar, 5);

				auxCitas = ListaCitas.getInicio();
				while (auxCitas != nullptr && strcmp(CitaBuscar, auxCitas->getData().citaNumero) != 0) {
					auxCitas = auxCitas->sig;
				}

				SetDlgItemText(hwnd, IDC_NUMCITA, auxCitas->getData().citaNumero);

				ComboBox_SelectString(GetDlgItem(hwnd, IDC_PACIENTE), 0, auxCitas->getData().NombrePaciente);
				SetDlgItemText(hwnd, IDC_TELEFONO, auxCitas->getData().telefono);
				ComboBox_SelectString(GetDlgItem(hwnd, IDC_ESPECIALIDAD), 0, auxCitas->getData().nombreEspecialidad);
				ComboBox_SelectString(GetDlgItem(hwnd, IDC_MEDICO), 0, auxCitas->getData().NombreMedico);
				ComboBox_SelectString(GetDlgItem(hwnd, IDC_HORA), 0, auxCitas->getData().horaTxt);
				ComboBox_SelectString(GetDlgItem(hwnd, IDC_MINUTOS), 0, auxCitas->getData().minutosTxt);

				SYSTEMTIME nuevo;
				nuevo = auxCitas->getData().fechacita;
				SendDlgItemMessage(hwnd, IDC_FECHITA, DTM_SETSYSTEMTIME, (WPARAM)GDT_VALID, (LPARAM)&nuevo);

				SetDlgItemText(hwnd, IDC_CONSULTORIO, auxCitas->getData().numconsultorio);

				HBITMAP bmp; //cargamos foto
				bmp = (HBITMAP)LoadImage(NULL, auxCitas->getData().foto, IMAGE_BITMAP, 150, 150, LR_LOADFROMFILE); //2
				strcpy_s(zFile, auxCitas->getData().foto);
				SendDlgItemMessage(hwnd, IDC_FOTOMEDICO, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);

				SetDlgItemText(hwnd, IDC_CLAVE, auxCitas->getData().claveUsuario);
			
			}break;
			}

		}break;//combo de editar citas

		case IDELIMINAR: {

			int eliminar = MessageBox(hwnd, "¿Desea eliminar esta cita?", "AVISO", MB_YESNO | MB_ICONQUESTION);
			switch (eliminar) {
			case IDYES: {
			
				ListaCitas.eliminarNodo(auxCitas);

				MessageBox(hwnd, "Cita eliminada exitosamente.", "AVISO", MB_YESNO | MB_ICONQUESTION);

				SetDlgItemText(hwnd, IDC_NUMCITA, "");

				SendMessage(GetDlgItem(hwnd, IDC_PACIENTE), CB_RESETCONTENT, 0, 0);
				auxPacientes = ListaPacientes.getInicio(); //llenamos pacientes
				while (auxPacientes != nullptr) {
					SendDlgItemMessage(hwnd, IDC_PACIENTE, CB_INSERTSTRING, (WPARAM)0, (LPARAM)auxPacientes->getData().NombreCompleto);
					auxPacientes = auxPacientes->sig;
				}

				SetDlgItemText(hwnd, IDC_TELEFONO, "");

				SendMessage(GetDlgItem(hwnd, IDC_ESPECIALIDAD), CB_RESETCONTENT, 0, 0);
				ArbolEspecialidad->LlenarCombobox(GetDlgItem(hwnd, IDC_ESPECIALIDAD), ArbolEspecialidad->GetRaiz());

				SendMessage(GetDlgItem(hwnd, IDC_MEDICO), CB_RESETCONTENT, 0, 0);
				auxMedicos = ListaMedicos.getInicio();
				while (auxMedicos != nullptr) { //llena el combobox con nombres de medicos
					SendDlgItemMessage(hwnd, IDC_MEDICO, CB_INSERTSTRING, (WPARAM)0, (LPARAM)auxMedicos->getData().NombreCompleto);
					auxMedicos = auxMedicos->sig;
				}

				SendMessage(GetDlgItem(hwnd, IDC_HORA), CB_RESETCONTENT, 0, 0);
				for (int i = 0; i < 11; i++) {
					SendDlgItemMessage(hwnd, IDC_HORA, CB_INSERTSTRING, (WPARAM)i, (LPARAM)HorasSelc[i]);
				}

				SendMessage(GetDlgItem(hwnd, IDC_MINUTOS), CB_RESETCONTENT, 0, 0);
				for (int j = 0; j < 3; j++) {
					SendDlgItemMessage(hwnd, IDC_MINUTOS, CB_INSERTSTRING, (WPARAM)j, (LPARAM)MinutosSelec[j]);
				}

				SendMessage(GetDlgItem(hwnd, IDC_FECHITA), DTM_SETSYSTEMTIME, GDT_NONE, 0);

				SetDlgItemText(hwnd, IDC_CONSULTORIO, "");

				if (ListaCitas.getInicio() != nullptr) {
					numCitas = (ListaCitas.getFinal()->getData().numeroCita) + 1;
				}

				SetDlgItemInt(hwnd, IDC_NUMCITA, numCitas, false);

				SendDlgItemMessage(hwnd, IDC_FOTOMEDICO, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)NULL);

				SendMessage(GetDlgItem(hwnd, IDC_COMBO4), CB_RESETCONTENT, 0, 0);
				auxCitas = ListaCitas.getInicio(); //llenamos modificar citas
				while (auxCitas != nullptr) {
					SendDlgItemMessage(hwnd, IDC_COMBO4, CB_INSERTSTRING, (WPARAM)0, (LPARAM)auxCitas->getData().citaNumero);
					auxCitas = auxCitas->sig;
				}

				EnableWindow(GetDlgItem(hwnd, IDGUARDAR), true); 
				EnableWindow(GetDlgItem(hwnd, IDMODIFICAR), false); 
				EnableWindow(GetDlgItem(hwnd, IDELIMINAR), false);

			}break;
			}



		}break; //eliminar

		case IDMODIFICAR: {

			int modificar = MessageBox(hwnd, "¿Desea modificar esta cita?", "AVISO", MB_YESNO | MB_ICONQUESTION);
			switch (modificar) {
			case IDYES: {

				Citas modificado;

				bool citacorrectamod=true;

				GetDlgItemText(hwnd, IDC_CLAVE, modificado.claveUsuario, sizeof(modificado.claveUsuario));

				modificado.numeroCita = GetDlgItemInt(hwnd, IDC_NUMCITA, NULL, false);
				GetDlgItemText(hwnd, IDC_NUMCITA, modificado.citaNumero, sizeof(modificado.citaNumero));

				GetDlgItemText(hwnd, IDC_PACIENTE, modificado.NombrePaciente, sizeof(modificado.NombrePaciente));
				GetDlgItemText(hwnd, IDC_TELEFONO, modificado.telefono, sizeof(modificado.telefono));
				GetDlgItemText(hwnd, IDC_ESPECIALIDAD, modificado.nombreEspecialidad, sizeof(modificado.nombreEspecialidad));
				GetDlgItemText(hwnd, IDC_MEDICO, modificado.NombreMedico, sizeof(modificado.NombreMedico));

				modificado.hora = GetDlgItemInt(hwnd, IDC_HORA, NULL, false);
				GetDlgItemText(hwnd, IDC_HORA, modificado.horaTxt, sizeof(modificado.horaTxt));

				modificado.minutos = GetDlgItemInt(hwnd, IDC_MINUTOS, NULL, false);
				GetDlgItemText(hwnd, IDC_MINUTOS, modificado.minutosTxt, sizeof(modificado.minutosTxt));

				SYSTEMTIME fechadelacita;
				SendDlgItemMessage(hwnd, IDC_FECHITA, DTM_GETSYSTEMTIME, 0, (LPARAM)&fechadelacita);

				modificado.fechacita = fechadelacita;

				GetDlgItemText(hwnd, IDC_CONSULTORIO, modificado.numconsultorio, sizeof(modificado.numconsultorio));

				modificado.estadoCita = 0;

				strcpy_s(modificado.foto, zFile);

				strcpy_s(modificado.descripcionCancelacion, "");

				char nombreMedico[80];

				GetDlgItemText(hwnd, IDC_MEDICO, nombreMedico, 80);

				auxMedicos = ListaMedicos.getInicio();
				while (auxMedicos->sig != nullptr && strcmp(nombreMedico, auxMedicos->getData().NombreCompleto) != 0) {
					auxMedicos = auxMedicos->sig;
				}

				if (auxPacientes != nullptr && auxMedicos != nullptr) {
					if (strcmp(auxPacientes->getData().NombreDoctor, "") == 0) {
						strcpy_s(auxPacientes->info.NombreDoctor, auxMedicos->getData().NombreCompleto);
					}
				}

				if (strlen(modificado.NombrePaciente) == 0) {
					citacorrectamod = false;
					MessageBox(hwnd, "Seleccione uno de los pacientes registrados.", "ERROR", MB_OK | MB_ICONINFORMATION);
				}

				if (strlen(modificado.nombreEspecialidad) == 0) {
					citacorrectamod = false;
					MessageBox(hwnd, "Seleccione una de las especialidades registrados.", "ERROR", MB_OK | MB_ICONINFORMATION);
				}

				if (strlen(modificado.NombreMedico) == 0) {
					citacorrectamod = false;
					MessageBox(hwnd, "Seleccione uno de los medicos registrados.", "ERROR", MB_OK | MB_ICONINFORMATION);
				}

				if (strlen(modificado.horaTxt) == 0) {
					citacorrectamod = false;
					MessageBox(hwnd, "Seleccione una hora para la cita.", "ERROR", MB_OK | MB_ICONINFORMATION);
				}
				else {
					modificado.hora = GetDlgItemInt(hwnd, IDC_HORA, NULL, false);

					if (auxMedicos != nullptr) {
						if (modificado.hora > 12 && auxMedicos->getData().horario == 1) {
							citacorrectamod = false;
							MessageBox(hwnd, "El medico seleccionado no esta disponible en este horario.", "ERROR", MB_OK | MB_ICONINFORMATION);
						}

						if (modificado.hora <= 12 && auxMedicos->getData().horario == 2) {
							citacorrectamod = false;
							MessageBox(hwnd, "El medico seleccionado no esta disponible en este horario.", "ERROR", MB_OK | MB_ICONINFORMATION);
						}
					}

				}

				if (strlen(modificado.minutosTxt) == 0) {
					citacorrectamod = false;
					MessageBox(hwnd, "Seleccione minutos para la cita.", "ERROR", MB_OK | MB_ICONINFORMATION);
				}
				else {
					modificado.minutos = GetDlgItemInt(hwnd, IDC_MINUTOS, NULL, false);

				}

				SYSTEMTIME fechahoy;
				ZeroMemory(&fechahoy, sizeof(fechahoy));
				GetLocalTime(&fechahoy);

				if (fechadelacita.wYear >= fechahoy.wYear && (fechadelacita.wYear > fechahoy.wYear || (fechadelacita.wYear == fechahoy.wYear && fechadelacita.wMonth > fechahoy.wMonth) || (fechadelacita.wYear == fechahoy.wYear && fechadelacita.wMonth == fechahoy.wMonth && fechadelacita.wDay >= fechahoy.wDay))) {

					if (fechadelacita.wDayOfWeek == 0 || fechadelacita.wDayOfWeek == 6) {
						citacorrectamod = false;
						MessageBox(hwnd, "Lo sentimos, no se atiende los fines de semana.", "ERROR", MB_OK | MB_ICONINFORMATION);
					}
					else {

						if (auxMedicos != nullptr) {

							if (fechadelacita.wDayOfWeek == 1 && auxMedicos->getData().lunes == true) {
								//no hace nada pero no se me ocurrio otra forma de hacerlo :3
							}
							else if (fechadelacita.wDayOfWeek == 2 && auxMedicos->getData().martes == true) {

							}
							else if (fechadelacita.wDayOfWeek == 3 && auxMedicos->getData().miercoles == true) {

							}
							else if (fechadelacita.wDayOfWeek == 4 && auxMedicos->getData().jueves == true) {

							}
							else if (fechadelacita.wDayOfWeek == 5 && auxMedicos->getData().viernes == true) {

							}else {
								citacorrectamod = false;
								MessageBox(hwnd, "Lo sentimos, el medico no esta disponible el dia seleccionado.", "ERROR", MB_OK | MB_ICONINFORMATION);
							}
						}


					}

				}
				else {

					citacorrectamod = false;
					MessageBox(hwnd, "Lo sentimos, seleccione una fecha valida.", "ERROR", MB_OK | MB_ICONINFORMATION);

				}

				Nodo<Citas>* auxcitas222 = nullptr; //otro puntero para moverse y no afectar el que editamos

				auxcitas222 = ListaCitas.getInicio();

				while (auxcitas222 != nullptr) {

					if (auxcitas222->getData().fechacita.wDay == modificado.fechacita.wDay && auxcitas222->getData().fechacita.wMonth == modificado.fechacita.wMonth && auxcitas222->getData().fechacita.wDay == modificado.fechacita.wDay && auxcitas222->getData().hora == modificado.hora && auxcitas222->getData().minutos == modificado.minutos && strcmp(auxcitas222->getData().numconsultorio, modificado.numconsultorio) == 0) {
						citacorrectamod = false;
						MessageBox(hwnd, "Ya hay una cita programada para esta fecha y este horario en este consultorio.", "ERROR", MB_OK | MB_ICONINFORMATION);
					}

					auxcitas222 = auxcitas222->sig;
				}

				if(citacorrectamod){

					ListaCitas.modificarNodo(auxCitas, modificado);

					MessageBox(hwnd, "Cita modificada exitosamente", "AVISO", MB_OK | MB_ICONINFORMATION);

					SetDlgItemText(hwnd, IDC_NUMCITA, "");

					SendMessage(GetDlgItem(hwnd, IDC_PACIENTE), CB_RESETCONTENT, 0, 0);
					auxPacientes = ListaPacientes.getInicio(); //llenamos pacientes
					while (auxPacientes != nullptr) {
						SendDlgItemMessage(hwnd, IDC_PACIENTE, CB_INSERTSTRING, (WPARAM)0, (LPARAM)auxPacientes->getData().NombreCompleto);
						auxPacientes = auxPacientes->sig;
					}

					SetDlgItemText(hwnd, IDC_TELEFONO, "");

					SendMessage(GetDlgItem(hwnd, IDC_ESPECIALIDAD), CB_RESETCONTENT, 0, 0);
					ArbolEspecialidad->LlenarCombobox(GetDlgItem(hwnd, IDC_ESPECIALIDAD), ArbolEspecialidad->GetRaiz());

					SendMessage(GetDlgItem(hwnd, IDC_MEDICO), CB_RESETCONTENT, 0, 0);
					auxMedicos = ListaMedicos.getInicio();
					while (auxMedicos != nullptr) { //llena el combobox con nombres de medicos
						SendDlgItemMessage(hwnd, IDC_MEDICO, CB_INSERTSTRING, (WPARAM)0, (LPARAM)auxMedicos->getData().NombreCompleto);
						auxMedicos = auxMedicos->sig;
					}

					SendMessage(GetDlgItem(hwnd, IDC_HORA), CB_RESETCONTENT, 0, 0);
					for (int i = 0; i < 11; i++) {
						SendDlgItemMessage(hwnd, IDC_HORA, CB_INSERTSTRING, (WPARAM)i, (LPARAM)HorasSelc[i]);
					}

					SendMessage(GetDlgItem(hwnd, IDC_MINUTOS), CB_RESETCONTENT, 0, 0);
					for (int j = 0; j < 3; j++) {
						SendDlgItemMessage(hwnd, IDC_MINUTOS, CB_INSERTSTRING, (WPARAM)j, (LPARAM)MinutosSelec[j]);
					}

					SendDlgItemMessage(hwnd, IDC_FECHITA, DTM_SETSYSTEMTIME, (WPARAM)GDT_VALID, (LPARAM)&fechahoy);

					SetDlgItemText(hwnd, IDC_CONSULTORIO, "");

					SetDlgItemInt(hwnd, IDC_NUMCITA, numCitas, false);

					SendMessage(GetDlgItem(hwnd, IDC_COMBO4), CB_RESETCONTENT, 0, 0);
					auxCitas = ListaCitas.getInicio(); //llenamos modificar citas
					while (auxCitas != nullptr) {
						SendDlgItemMessage(hwnd, IDC_COMBO4, CB_INSERTSTRING, (WPARAM)0, (LPARAM)auxCitas->getData().citaNumero);
						auxCitas = auxCitas->sig;
					}

					SendDlgItemMessage(hwnd, IDC_FOTOMEDICO, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)NULL);

					EnableWindow(GetDlgItem(hwnd, IDGUARDAR), true);
					EnableWindow(GetDlgItem(hwnd, IDMODIFICAR), false);
					EnableWindow(GetDlgItem(hwnd, IDELIMINAR), false);
				}


			}break;
			}


		}break; //modificar

		}

	}break;

	}//msg

	return 0;
}

BOOL CALLBACK CITASXESPECIALIDAD(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {

	case WM_CLOSE: {
		int opc = MessageBox(hwnd, "¿Seguro que desea salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);
		switch (opc) {
		case IDYES: {
			DestroyWindow(hwnd);
			PostQuitMessage(0); //Deja de ejecutar el programa
		}break;
		}
	}break; //CLOSE

	case WM_INITDIALOG: {

		SetDlgItemText(hwnd, IDC_USUARIO, sesionIniciada->getData().NombreCompleto);

		char numerodeconsultorios[4][3] = { "1","2","3","4" };
		for (int i = 0; i < 4; i++) {
			SendDlgItemMessage(hwnd, IDC_CONSULTORIO, CB_INSERTSTRING, (WPARAM)i, (LPARAM)numerodeconsultorios[i]);
		}

		ArbolEspecialidad->LlenarCombobox(GetDlgItem(hwnd, IDC_ESPECIALIDAD), ArbolEspecialidad->GetRaiz());

	}break; //Initdialog

	case WM_COMMAND: {

		SYSTEMTIME fechahoy;
		ZeroMemory(&fechahoy, sizeof(fechahoy));
		GetLocalTime(&fechahoy);

		SYSTEMTIME semanabuscar;
		ZeroMemory(&semanabuscar, sizeof(semanabuscar));
		SendDlgItemMessage(hwnd, IDC_SEMANACITA, DTM_GETSYSTEMTIME, 0, (LPARAM)&semanabuscar);

		menu(wParam, hwnd);

		switch (LOWORD(wParam)) {

		case IDC_BUSCAR: {

			SetDlgItemText(hwnd, IDC_NCITA, "");
			SetDlgItemText(hwnd, IDC_NOMPACIENTE, "");
			SetDlgItemText(hwnd, IDC_TELEFONO, "");
			SetDlgItemText(hwnd, IDC_ESPE2, "");
			SetDlgItemText(hwnd, IDC_MED2, "");
			SetDlgItemText(hwnd, IDC_HORA, "");
			SetDlgItemText(hwnd, IDC_MINUTO, "");
			SetDlgItemText(hwnd, IDC_ESTCITA, "");
			SetDlgItemText(hwnd, IDC_CONSULT, "");
			SetDlgItemText(hwnd, IDC_DESCR, "");
			SendDlgItemMessage(hwnd, IDC_FECHAS, DTM_SETSYSTEMTIME, (WPARAM)GDT_VALID, (LPARAM)&fechahoy);
			EnableWindow(GetDlgItem(hwnd, IDC_ATENDIDA), false);
			EnableWindow(GetDlgItem(hwnd, IDC_DESCR), false);
			EnableWindow(GetDlgItem(hwnd, IDC_CANCELAR), false);

			SendMessage(GetDlgItem(hwnd, IDC_COMBOCITAS), CB_RESETCONTENT, 0, 0);

			SendMessage(GetDlgItem(hwnd, IDC_CONSULTORIO), CB_RESETCONTENT, 0, 0);
			char numerodeconsultorios[4][3] = { "1","2","3","4" };
			for (int i = 0; i < 4; i++) {
				SendDlgItemMessage(hwnd, IDC_CONSULTORIO, CB_INSERTSTRING, (WPARAM)i, (LPARAM)numerodeconsultorios[i]);
			}

			if (semanabuscar.wDayOfWeek != 1) {
				MessageBox(hwnd, "No ha seleccionado un dia lunes de la semana.", "ERROR", MB_OK | MB_ICONINFORMATION);
			}
			else {
				EnableWindow(GetDlgItem(hwnd, IDC_ESPECIALIDAD), true);

			}

		}break;

		case IDC_ESPECIALIDAD: {

			switch (HIWORD(wParam)) {

			case CBN_SELCHANGE: {

				EnableWindow(GetDlgItem(hwnd, IDC_CONSULTORIO), true);

				SetDlgItemText(hwnd, IDC_NCITA, "");
				SetDlgItemText(hwnd, IDC_NOMPACIENTE2, "");
				SetDlgItemText(hwnd, IDC_TELEFONO, "");
				SetDlgItemText(hwnd, IDC_ESPE2, "");
				SetDlgItemText(hwnd, IDC_MED2, "");
				SetDlgItemText(hwnd, IDC_HORA, "");
				SetDlgItemText(hwnd, IDC_MINUTO, "");
				SetDlgItemText(hwnd, IDC_ESTCITA, "");
				SetDlgItemText(hwnd, IDC_CONSULT, "");
				SetDlgItemText(hwnd, IDC_DESCR, "");
				SendDlgItemMessage(hwnd, IDC_FECHAS, DTM_SETSYSTEMTIME, (WPARAM)GDT_VALID, (LPARAM)&fechahoy);
				EnableWindow(GetDlgItem(hwnd, IDC_ATENDIDA), false);
				EnableWindow(GetDlgItem(hwnd, IDC_DESCR), false);
				EnableWindow(GetDlgItem(hwnd, IDC_CANCELAR), false);

				SendMessage(GetDlgItem(hwnd, IDC_COMBOCITAS), CB_RESETCONTENT, 0, 0);

				SendMessage(GetDlgItem(hwnd, IDC_CONSULTORIO), CB_RESETCONTENT, 0, 0);
				char numerodeconsultorios[4][3] = { "1","2","3","4" };
				for (int i = 0; i < 4; i++) {
					SendDlgItemMessage(hwnd, IDC_CONSULTORIO, CB_INSERTSTRING, (WPARAM)i, (LPARAM)numerodeconsultorios[i]);
				}




			}break;
			}

		}break;

		case IDC_CONSULTORIO: {

			switch (HIWORD(wParam)) {

			case CBN_SELCHANGE: {

				char consultorio[5];
				char especialidadbuscar[30];

				GetDlgItemText(hwnd, IDC_CONSULTORIO, consultorio, 5);
				GetDlgItemText(hwnd, IDC_ESPECIALIDAD, especialidadbuscar, 30);

				SendMessage(GetDlgItem(hwnd, IDC_COMBOCITAS), CB_RESETCONTENT, 0, 0);

				int diacita = semanabuscar.wDay;
				int diacita2 = semanabuscar.wDay;
				int mescita = semanabuscar.wMonth;
				int mescita2 = semanabuscar.wMonth;

				auxCitas = ListaCitas.getInicio(); //llenamos registradas citas
				while (auxCitas != nullptr) {

					SYSTEMTIME citapaciente;
					citapaciente = auxCitas->getData().fechacita;

					for (int i = diacita2; i <= (diacita2 + 6); i++) {
						if (strcmp(consultorio, auxCitas->getData().numconsultorio) == 0 && citapaciente.wDay == diacita && citapaciente.wMonth == mescita && strcmp(especialidadbuscar,auxCitas->getData().nombreEspecialidad)==0) {
							SendDlgItemMessage(hwnd, IDC_COMBOCITAS, CB_INSERTSTRING, (WPARAM)0, (LPARAM)auxCitas->getData().citaNumero);
						}
						diacita++;
						if (diacita == 31) {
							diacita = 1;
							mescita++;
						}

					}
					diacita = diacita2;
					mescita = mescita2;
					auxCitas = auxCitas->sig;
				}

				SetDlgItemText(hwnd, IDC_NCITA, "");
				SetDlgItemText(hwnd, IDC_NOMPACIENTE, "");
				SetDlgItemText(hwnd, IDC_TELEFONO, "");
				SetDlgItemText(hwnd, IDC_ESPE2, "");
				SetDlgItemText(hwnd, IDC_MED2, "");
				SetDlgItemText(hwnd, IDC_HORA, "");
				SetDlgItemText(hwnd, IDC_MINUTO, "");
				SetDlgItemText(hwnd, IDC_ESTCITA, "");
				SetDlgItemText(hwnd, IDC_CONSULT, "");
				SetDlgItemText(hwnd, IDC_DESCR, "");
				SendDlgItemMessage(hwnd, IDC_FECHAS, DTM_SETSYSTEMTIME, (WPARAM)GDT_VALID, (LPARAM)&fechahoy);
				EnableWindow(GetDlgItem(hwnd, IDC_ATENDIDA), false);
				EnableWindow(GetDlgItem(hwnd, IDC_DESCR), false);
				EnableWindow(GetDlgItem(hwnd, IDC_CANCELAR), false);


			}break;


			}

		}break;

		case IDC_COMBOCITAS: {

			switch (HIWORD(wParam)) {

			case CBN_SELCHANGE: {

				char citas[5];

				GetDlgItemText(hwnd, IDC_COMBOCITAS, citas, 5);

				auxCitas = ListaCitas.getInicio();
				while (auxCitas != nullptr && strcmp(citas, auxCitas->getData().citaNumero) != 0) {
					auxCitas = auxCitas->sig;
				}

				SetDlgItemText(hwnd, IDC_NCITA, auxCitas->getData().citaNumero);
				SetDlgItemText(hwnd, IDC_NOMPACIENTE2, auxCitas->getData().NombrePaciente);
				SetDlgItemText(hwnd, IDC_TELEFONO, auxCitas->getData().telefono);
				SetDlgItemText(hwnd, IDC_ESPE2, auxCitas->getData().nombreEspecialidad);
				SetDlgItemText(hwnd, IDC_MED2, auxCitas->getData().NombreMedico);
				SetDlgItemText(hwnd, IDC_HORA, auxCitas->getData().horaTxt);
				SetDlgItemText(hwnd, IDC_MINUTO, auxCitas->getData().minutosTxt);

				SYSTEMTIME nuevo;
				nuevo = auxCitas->getData().fechacita;
				SendDlgItemMessage(hwnd, IDC_FECHAS, DTM_SETSYSTEMTIME, (WPARAM)GDT_VALID, (LPARAM)&nuevo);

				SetDlgItemText(hwnd, IDC_CONSULT, auxCitas->getData().numconsultorio);
				SetDlgItemText(hwnd, IDC_DESCR, auxCitas->getData().descripcionCancelacion);

				if (auxCitas->getData().estadoCita == 0) {

					SetDlgItemText(hwnd, IDC_ESTCITA, "PENDIENTE");
					EnableWindow(GetDlgItem(hwnd, IDC_ATENDIDA), true);
					EnableWindow(GetDlgItem(hwnd, IDC_DESCR), true);
					EnableWindow(GetDlgItem(hwnd, IDC_CANCELAR), true);

				}
				else if (auxCitas->getData().estadoCita == 1) {

					SetDlgItemText(hwnd, IDC_ESTCITA, "ATENDIDA");
					EnableWindow(GetDlgItem(hwnd, IDC_ATENDIDA), false);
					EnableWindow(GetDlgItem(hwnd, IDC_DESCR), false);
					EnableWindow(GetDlgItem(hwnd, IDC_CANCELAR), false);
				}
				else if (auxCitas->getData().estadoCita == 2) {

					SetDlgItemText(hwnd, IDC_ESTCITA, "CANCELADA");

				}

			}break;
			}


		}break;

		case IDC_ATENDIDA: {

			int atender = MessageBox(hwnd, "¿Desea poner como atendida esta cita?", "AVISO", MB_YESNO | MB_ICONQUESTION);
			switch (atender) {
			case IDYES: {

				auxCitas->info.estadoCita = 1;

				SetDlgItemText(hwnd, IDC_ESTCITA, "ATENDIDA");
				EnableWindow(GetDlgItem(hwnd, IDC_ATENDIDA), false);
				EnableWindow(GetDlgItem(hwnd, IDC_DESCR), false);
				EnableWindow(GetDlgItem(hwnd, IDC_CANCELAR), false);

			}break;
			}


		}break;

		case IDC_CANCELAR: {

			int cancelar = MessageBox(hwnd, "¿Desea cancelar esta cita?", "AVISO", MB_YESNO | MB_ICONQUESTION);
			switch (cancelar) {
			case IDYES: {

				char Motivo[300];

				GetDlgItemText(hwnd, IDC_DESCR, Motivo, 300);

				if (strlen(Motivo) == 0) {

					MessageBox(hwnd, "Por favor, añada un motivo a su cancelacion para continuar.", "AVISO", MB_OK | MB_ICONINFORMATION);

				}
				else {
					auxCitas->info.estadoCita = 2;


					SetDlgItemText(hwnd, IDC_ESTCITA, "CANCELADA");
					strcpy_s(auxCitas->info.descripcionCancelacion, Motivo);

					EnableWindow(GetDlgItem(hwnd, IDC_ATENDIDA), false);
					EnableWindow(GetDlgItem(hwnd, IDC_DESCR), false);
					EnableWindow(GetDlgItem(hwnd, IDC_CANCELAR), false);

				}


			}break;//yes


			}//switchcancelar


		}break;

		case IDC_ARCHIVO: {

			int report = MessageBox(hwnd, "¿Desea generar un reporte de las citas?", "AVISO", MB_YESNO | MB_ICONQUESTION);
			switch (report) {
			case IDYES: {

				ofstream archivoCitasSemana;

				int cita = semanabuscar.wDay;
				int cita2 = semanabuscar.wDay;
				int citames = semanabuscar.wMonth;
				int citames2 = semanabuscar.wMonth;
				char consultorios[5];
				char especialidadbuscar2[30];

				GetDlgItemText(hwnd, IDC_CONSULTORIO, consultorios, 5);
				GetDlgItemText(hwnd, IDC_ESPECIALIDAD, especialidadbuscar2, 30);

				archivoCitasSemana.open("C:\\Users\\jpfb0\\Desktop\\ESTRUCTURA DATOS WINAPI\\ESTRUCTURA DATOS WINAPI\\Reportes\\CitasSemanaEspecialidad.txt", ios::out | ios::trunc);

				if (archivoCitasSemana.is_open()) {

					archivoCitasSemana << "CITAS REGISTRADAS" <<endl <<endl;

					auxCitas = ListaCitas.getInicio();

					while (auxCitas != nullptr) {

						SYSTEMTIME citapacienteRep;
						citapacienteRep = auxCitas->getData().fechacita;

						for (int i = cita2; i <= (cita2 + 6); i++) {
							if (strcmp(consultorios, auxCitas->getData().numconsultorio) == 0 && citapacienteRep.wDay == cita && citapacienteRep.wMonth == citames&&strcmp(auxCitas->getData().nombreEspecialidad, especialidadbuscar2)==0) {

								archivoCitasSemana << "N° de cita: " << auxCitas->getData().citaNumero << endl;
								archivoCitasSemana << "Paciente: " << auxCitas->getData().NombrePaciente << endl;
								archivoCitasSemana << "Telefono: " << auxCitas->getData().telefono << endl;
								archivoCitasSemana << "Especialidad: " << auxCitas->getData().nombreEspecialidad << endl;
								archivoCitasSemana << "Medico: " << auxCitas->getData().NombreMedico << endl;
								archivoCitasSemana << "Hora: " << auxCitas->getData().horaTxt << " : " << auxCitas->getData().minutosTxt << endl;
								archivoCitasSemana << "Fecha: " << auxCitas->getData().fechacita.wDay << " / " << auxCitas->getData().fechacita.wMonth << " / " << auxCitas->getData().fechacita.wYear << endl;
								archivoCitasSemana << "Consultorio: " << auxCitas->getData().numconsultorio << endl;
								if (auxCitas->getData().estadoCita == 0) {
									archivoCitasSemana << "Estado: PENDIENTE" << endl;
								}
								else if (auxCitas->getData().estadoCita == 1) {
									archivoCitasSemana << "Estado: ATENDIDA" << endl;
								}
								else if (auxCitas->getData().estadoCita == 2) {
									archivoCitasSemana << "Estado: CANCELADA" << endl;
									archivoCitasSemana << "Motivo de cancelacion: " << auxCitas->getData().descripcionCancelacion << endl;
								}

								archivoCitasSemana << endl << endl;

							}
							cita++;
							if (cita == 31) {
								cita = 1;
								citames++;
								if (citames == 12) {
									citames = 1;
								}
							}

						}
						cita = cita2;
						citames = citames2;
						auxCitas = auxCitas->sig;

					}

					MessageBox(hwnd, "Archivo creado en la siguiente direccion: C:\\Users\\jpfb0\\Desktop\\ESTRUCTURA DATOS WINAPI\\ESTRUCTURA DATOS WINAPI\\Reportes\\CitasSemanaEspecialidad.txt", "ARCHIVO CREADO", MB_OK | MB_ICONINFORMATION);

				}


			}break;

			}



		}break;

		}


	}break;

	}//msg

	return 0;
}

BOOL CALLBACK CITASXMEDICO(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {

	case WM_CLOSE: {
		int opc = MessageBox(hwnd, "¿Seguro que desea salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);
		switch (opc) {
		case IDYES: {
			DestroyWindow(hwnd);
			PostQuitMessage(0); //Deja de ejecutar el programa
		}break;
		}
	}break; //CLOSE

	case WM_INITDIALOG: {

		SetDlgItemText(hwnd, IDC_USUARIO, sesionIniciada->getData().NombreCompleto);

		ListaMedicos.quicksort();

	}break;

	case WM_COMMAND: {

		int cedulabusca=0;
		SYSTEMTIME fechahoy;
		ZeroMemory(&fechahoy, sizeof(fechahoy));
		GetLocalTime(&fechahoy);

		menu(wParam, hwnd);

		switch (LOWORD(wParam)) {

		case IDC_BUSCA: {

			char cedulabuscar[10];

			SetDlgItemText(hwnd, IDC_NCITA, "");
			SetDlgItemText(hwnd, IDC_NOMPACIENTE, "");
			SetDlgItemText(hwnd, IDC_TELEFONO, "");
			SetDlgItemText(hwnd, IDC_ESPE2, "");
			SetDlgItemText(hwnd, IDC_MED2, "");
			SetDlgItemText(hwnd, IDC_HORA, "");
			SetDlgItemText(hwnd, IDC_MINUTO, "");
			SetDlgItemText(hwnd, IDC_ESTCITA, "");
			SetDlgItemText(hwnd, IDC_CONSULT, "");
			SetDlgItemText(hwnd, IDC_DESCR, "");
			SendDlgItemMessage(hwnd, IDC_FECHAS, DTM_SETSYSTEMTIME, (WPARAM)GDT_VALID, (LPARAM)&fechahoy);
			SendMessage(GetDlgItem(hwnd, IDC_COMBOCITAS), CB_RESETCONTENT, 0, 0);
			EnableWindow(GetDlgItem(hwnd, IDC_ATENDIDA), false);
			EnableWindow(GetDlgItem(hwnd, IDC_DESCR), false);
			EnableWindow(GetDlgItem(hwnd, IDC_CANCELAR), false);

			GetDlgItemText(hwnd, IDC_CEDULA, cedulabuscar, 10);

			if (validarNumero(cedulabuscar) == false) {

				MessageBox(hwnd, "Unicamente puede ingresar un dato numerico.", "ERROR", MB_OK | MB_ICONINFORMATION);

			}
			else {

				cedulabusca = GetDlgItemInt(hwnd, IDC_CEDULA, NULL, false);
				auxMedicos = ListaMedicos.BusquedaBinaria(ListaMedicos.getInicio(), cedulabusca);

				if (auxMedicos != nullptr) {

					SetDlgItemText(hwnd, IDC_NOMBREMEDICO, auxMedicos->getData().NombreCompleto);
					SetDlgItemText(hwnd, IDC_NOMBREESPECIA, auxMedicos->getData().especialidad);
					EnableWindow(GetDlgItem(hwnd, IDC_MES), true);

					SendMessage(GetDlgItem(hwnd, IDC_MES), CB_RESETCONTENT, 0, 0);
					char meses[12][20] = { "Enero","Febrero","Marzo","Abril","Mayo","Junio","Julio","Agosto","Septiembre","Octubre","Noviembre","Diciembre" };
					for (int i = 0; i < 12; i++) {
						SendDlgItemMessage(hwnd, IDC_MES, CB_INSERTSTRING, (WPARAM)i, (LPARAM)meses[i]);
					}

				}
				else {
					MessageBox(hwnd, "No se encontro el medico, intentelo nuevamente", "ERROR", MB_OK | MB_ICONINFORMATION);
					SetDlgItemText(hwnd, IDC_NOMBREMEDICO, "");
					SetDlgItemText(hwnd, IDC_NOMBREESPECIA, "");
					EnableWindow(GetDlgItem(hwnd, IDC_MES), false);
					SendMessage(GetDlgItem(hwnd, IDC_MES), CB_RESETCONTENT, 0, 0);

				}


			}

		}break;

		case IDC_MES: {

			switch (HIWORD(wParam)) {

			case CBN_SELCHANGE: {

				SendMessage(GetDlgItem(hwnd, IDC_COMBOCITAS), CB_RESETCONTENT, 0, 0);

				int mesSeleccionado = 0;
				char medicoBuscada[80];

				mesSeleccionado = SendDlgItemMessage(hwnd, IDC_MES, CB_GETCURSEL, 0, 0);

				mesSeleccionado++;

				GetDlgItemText(hwnd, IDC_NOMBREMEDICO, medicoBuscada, 80);

				auxCitas = ListaCitas.getInicio();

				while (auxCitas != nullptr) {

					if (strcmp(medicoBuscada, auxCitas->getData().NombreMedico) == 0 && auxCitas->getData().fechacita.wMonth == mesSeleccionado) {
						SendDlgItemMessage(hwnd, IDC_COMBOCITAS, CB_INSERTSTRING, (WPARAM)0, (LPARAM)auxCitas->getData().citaNumero);
					}
					auxCitas = auxCitas->sig;
				}

				SetDlgItemText(hwnd, IDC_NCITA, "");
				SetDlgItemText(hwnd, IDC_NOMPACIENTE, "");
				SetDlgItemText(hwnd, IDC_TELEFONO, "");
				SetDlgItemText(hwnd, IDC_ESPE2, "");
				SetDlgItemText(hwnd, IDC_MED2, "");
				SetDlgItemText(hwnd, IDC_HORA, "");
				SetDlgItemText(hwnd, IDC_MINUTO, "");
				SetDlgItemText(hwnd, IDC_ESTCITA, "");
				SetDlgItemText(hwnd, IDC_CONSULT, "");
				SetDlgItemText(hwnd, IDC_DESCR, "");
				SendDlgItemMessage(hwnd, IDC_FECHAS, DTM_SETSYSTEMTIME, (WPARAM)GDT_VALID, (LPARAM)&fechahoy);
				EnableWindow(GetDlgItem(hwnd, IDC_ATENDIDA), false);
				EnableWindow(GetDlgItem(hwnd, IDC_DESCR), false);
				EnableWindow(GetDlgItem(hwnd, IDC_CANCELAR), false);
			}break;

			}
			

		}break;

		case IDC_COMBOCITAS: {

			switch (HIWORD(wParam)) {

			case CBN_SELCHANGE: {

				char citas[5];

				GetDlgItemText(hwnd, IDC_COMBOCITAS, citas, 5);

				auxCitas = ListaCitas.getInicio();
				while (auxCitas != nullptr && strcmp(citas, auxCitas->getData().citaNumero) != 0) {
					auxCitas = auxCitas->sig;
				}

				SetDlgItemText(hwnd, IDC_NCITA, auxCitas->getData().citaNumero);
				SetDlgItemText(hwnd, IDC_NOMPACIENTE, auxCitas->getData().NombrePaciente);
				SetDlgItemText(hwnd, IDC_TELEFONO, auxCitas->getData().telefono);
				SetDlgItemText(hwnd, IDC_ESPE2, auxCitas->getData().nombreEspecialidad);
				SetDlgItemText(hwnd, IDC_MED2, auxCitas->getData().NombreMedico);
				SetDlgItemText(hwnd, IDC_HORA, auxCitas->getData().horaTxt);
				SetDlgItemText(hwnd, IDC_MINUTO, auxCitas->getData().minutosTxt);

				SYSTEMTIME nuevo;
				nuevo = auxCitas->getData().fechacita;
				SendDlgItemMessage(hwnd, IDC_FECHAS, DTM_SETSYSTEMTIME, (WPARAM)GDT_VALID, (LPARAM)&nuevo);

				SetDlgItemText(hwnd, IDC_CONSULT, auxCitas->getData().numconsultorio);
				SetDlgItemText(hwnd, IDC_DESCR, auxCitas->getData().descripcionCancelacion);

				if (auxCitas->getData().estadoCita == 0) {

					SetDlgItemText(hwnd, IDC_ESTCITA, "PENDIENTE");
					EnableWindow(GetDlgItem(hwnd, IDC_ATENDIDA), true);
					EnableWindow(GetDlgItem(hwnd, IDC_DESCR), true);
					EnableWindow(GetDlgItem(hwnd, IDC_CANCELAR), true);

				}
				else if (auxCitas->getData().estadoCita == 1) {

					SetDlgItemText(hwnd, IDC_ESTCITA, "ATENDIDA");
					EnableWindow(GetDlgItem(hwnd, IDC_ATENDIDA), false);
					EnableWindow(GetDlgItem(hwnd, IDC_DESCR), false);
					EnableWindow(GetDlgItem(hwnd, IDC_CANCELAR), false);
				}
				else if (auxCitas->getData().estadoCita == 2) {

					SetDlgItemText(hwnd, IDC_ESTCITA, "CANCELADA");

				}

			}break;
			}


		}break;

		case IDC_ATENDIDA: {

			int atender = MessageBox(hwnd, "¿Desea poner como atendida esta cita?", "AVISO", MB_YESNO | MB_ICONQUESTION);
			switch (atender) {
			case IDYES: {

				auxCitas->info.estadoCita = 1;

				SetDlgItemText(hwnd, IDC_ESTCITA, "ATENDIDA");
				EnableWindow(GetDlgItem(hwnd, IDC_ATENDIDA), false);
				EnableWindow(GetDlgItem(hwnd, IDC_DESCR), false);
				EnableWindow(GetDlgItem(hwnd, IDC_CANCELAR), false);

			}break;
			}

		}break;

		case IDC_CANCELAR: {

			int cancelar = MessageBox(hwnd, "¿Desea cancelar esta cita?", "AVISO", MB_YESNO | MB_ICONQUESTION);
			switch (cancelar) {
			case IDYES: {

				char Motivo[300];

				GetDlgItemText(hwnd, IDC_DESCR, Motivo, 300);

				if (strlen(Motivo) == 0) {

					MessageBox(hwnd, "Por favor, añada un motivo a su cancelacion para continuar.", "AVISO", MB_OK | MB_ICONINFORMATION);

				}
				else {
					auxCitas->info.estadoCita = 2;


					SetDlgItemText(hwnd, IDC_ESTCITA, "CANCELADA");
					strcpy_s(auxCitas->info.descripcionCancelacion, Motivo);

					EnableWindow(GetDlgItem(hwnd, IDC_ATENDIDA), false);
					EnableWindow(GetDlgItem(hwnd, IDC_DESCR), false);
					EnableWindow(GetDlgItem(hwnd, IDC_CANCELAR), false);

				}


			}break;//yes


			}//switchcancelar


		}break;

		case IDC_ARCHIVO: {

			int report = MessageBox(hwnd, "¿Desea generar un reporte de las citas?", "AVISO", MB_YESNO | MB_ICONQUESTION);
			switch (report) {
			case IDYES: {

				char MedicoBusca[80];
				int mesBusca=0;

				ofstream archivoMesMedico;

				mesBusca= SendDlgItemMessage(hwnd, IDC_MES, CB_GETCURSEL, 0, 0);
				mesBusca++;
				GetDlgItemText(hwnd, IDC_NOMBREMEDICO, MedicoBusca, 80);

				archivoMesMedico.open("C:\\Users\\jpfb0\\Desktop\\ESTRUCTURA DATOS WINAPI\\ESTRUCTURA DATOS WINAPI\\Reportes\\CitasMesMedico.txt", ios::out | ios::trunc);

				if (archivoMesMedico.is_open()) {

					archivoMesMedico << "CITAS REGISTRADAS" << endl<<endl;

					auxCitas = ListaCitas.getInicio();

					while (auxCitas != nullptr) {

						if (auxCitas->getData().fechacita.wMonth == mesBusca && strcmp(MedicoBusca, auxCitas->getData().NombreMedico) == 0) {

							archivoMesMedico << "N° de cita: " << auxCitas->getData().citaNumero << endl;
							archivoMesMedico << "Paciente: " << auxCitas->getData().NombrePaciente << endl;
							archivoMesMedico << "Telefono: " << auxCitas->getData().telefono << endl;
							archivoMesMedico << "Especialidad: " << auxCitas->getData().nombreEspecialidad << endl;
							archivoMesMedico << "Medico: " << auxCitas->getData().NombreMedico << endl;
							archivoMesMedico << "Hora: " << auxCitas->getData().horaTxt << " : " << auxCitas->getData().minutosTxt << endl;
							archivoMesMedico << "Fecha: " << auxCitas->getData().fechacita.wDay << " / " << auxCitas->getData().fechacita.wMonth << " / " << auxCitas->getData().fechacita.wYear << endl;
							archivoMesMedico << "Consultorio: " << auxCitas->getData().numconsultorio << endl;
							if (auxCitas->getData().estadoCita == 0) {
								archivoMesMedico << "Estado: PENDIENTE" << endl;
							}
							else if (auxCitas->getData().estadoCita == 1) {
								archivoMesMedico << "Estado: ATENDIDA" << endl;
							}
							else if (auxCitas->getData().estadoCita == 2) {
								archivoMesMedico << "Estado: CANCELADA" << endl;
								archivoMesMedico << "Motivo de cancelacion: " << auxCitas->getData().descripcionCancelacion << endl;
							}

							archivoMesMedico << endl << endl;


						}

						auxCitas = auxCitas->sig;

					}

					MessageBox(hwnd, "Archivo creado en la siguiente direccion: C:\\Users\\jpfb0\\Desktop\\ESTRUCTURA DATOS WINAPI\\ESTRUCTURA DATOS WINAPI\\Reportes\\CitasMesMedico.txt", "ARCHIVO CREADO", MB_OK | MB_ICONINFORMATION);

				}

				
			}break;
			}

		}break;

		}

	}break;

	}//msg

	return 0;
}

BOOL CALLBACK CITASXSEMANA(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {

	case WM_CLOSE: {
		int opc = MessageBox(hwnd, "¿Seguro que desea salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);
		switch (opc) {
		case IDYES: {
			DestroyWindow(hwnd);
			PostQuitMessage(0); //Deja de ejecutar el programa
		}break;
		}
	}break; //CLOSE

	case WM_INITDIALOG: {

		SetDlgItemText(hwnd, IDC_USUARIO, sesionIniciada->getData().NombreCompleto);

		char numerodeconsultorios[4][3] = { "1","2","3","4" };
		for (int i = 0; i < 4; i++) {
			SendDlgItemMessage(hwnd, IDC_CONSULTORIO, CB_INSERTSTRING, (WPARAM)i, (LPARAM)numerodeconsultorios[i]);
		}

	}break; //Initdialog

	case WM_COMMAND: {

		SYSTEMTIME fechahoy;
		ZeroMemory(&fechahoy, sizeof(fechahoy));
		GetLocalTime(&fechahoy);

		SYSTEMTIME semanabuscar;
		ZeroMemory(&semanabuscar, sizeof(semanabuscar));
		SendDlgItemMessage(hwnd, IDC_SEMANACITA, DTM_GETSYSTEMTIME, 0, (LPARAM)&semanabuscar);

		menu(wParam, hwnd);

		switch (LOWORD(wParam)) {

		case IDC_BUSCAR: {

			SetDlgItemText(hwnd, IDC_NCITA, "");
			SetDlgItemText(hwnd, IDC_NOMPACIENTE, "");
			SetDlgItemText(hwnd, IDC_TELEFONO, "");
			SetDlgItemText(hwnd, IDC_ESPE2, "");
			SetDlgItemText(hwnd, IDC_MED2, "");
			SetDlgItemText(hwnd, IDC_HORA, "");
			SetDlgItemText(hwnd, IDC_MINUTO, "");
			SetDlgItemText(hwnd, IDC_ESTCITA, "");
			SetDlgItemText(hwnd, IDC_CONSULT, "");
			SetDlgItemText(hwnd, IDC_DESCR, "");
			SendDlgItemMessage(hwnd, IDC_FECHAS, DTM_SETSYSTEMTIME, (WPARAM)GDT_VALID, (LPARAM)&fechahoy);
			EnableWindow(GetDlgItem(hwnd, IDC_ATENDIDA), false);
			EnableWindow(GetDlgItem(hwnd, IDC_DESCR), false);
			EnableWindow(GetDlgItem(hwnd, IDC_CANCELAR), false);

			SendMessage(GetDlgItem(hwnd, IDC_COMBOCITAS), CB_RESETCONTENT, 0, 0);

			SendMessage(GetDlgItem(hwnd, IDC_CONSULTORIO), CB_RESETCONTENT, 0, 0);
			char numerodeconsultorios[4][3] = { "1","2","3","4" };
			for (int i = 0; i < 4; i++) {
				SendDlgItemMessage(hwnd, IDC_CONSULTORIO, CB_INSERTSTRING, (WPARAM)i, (LPARAM)numerodeconsultorios[i]);
			}
			

			if (semanabuscar.wDayOfWeek != 1) {

				MessageBox(hwnd, "No ha seleccionado un dia lunes de la semana.", "ERROR", MB_OK | MB_ICONINFORMATION);

			}
			else {

				EnableWindow(GetDlgItem(hwnd, IDC_CONSULTORIO), true);


			}



		}break;

		case IDC_CONSULTORIO: {

			switch (HIWORD(wParam)) {

			case CBN_SELCHANGE: {

				char consultorio[5];

				GetDlgItemText(hwnd, IDC_CONSULTORIO, consultorio, 5);

				SendMessage(GetDlgItem(hwnd, IDC_COMBOCITAS), CB_RESETCONTENT, 0, 0);

				int diacita = semanabuscar.wDay;
				int diacita2 = semanabuscar.wDay;
				int mescita = semanabuscar.wMonth;
				int mescita2 = semanabuscar.wMonth;

				auxCitas = ListaCitas.getInicio(); //llenamos registradas citas
				while (auxCitas != nullptr) {

					SYSTEMTIME citapaciente;
					citapaciente = auxCitas->getData().fechacita;

					for (int i = diacita2; i <= (diacita2 + 6); i++) {
						if (strcmp(consultorio, auxCitas->getData().numconsultorio) == 0 && citapaciente.wDay == diacita && citapaciente.wMonth == mescita) {
							SendDlgItemMessage(hwnd, IDC_COMBOCITAS, CB_INSERTSTRING, (WPARAM)0, (LPARAM)auxCitas->getData().citaNumero);
						}
						diacita++;
						if (diacita == 31) {
							diacita = 1;
							mescita++;
						}

					}
					diacita = diacita2;
					mescita = mescita2;
					auxCitas = auxCitas->sig;
				}

				SetDlgItemText(hwnd, IDC_NCITA, "");
				SetDlgItemText(hwnd, IDC_NOMPACIENTE, "");
				SetDlgItemText(hwnd, IDC_TELEFONO, "");
				SetDlgItemText(hwnd, IDC_ESPE2, "");
				SetDlgItemText(hwnd, IDC_MED2, "");
				SetDlgItemText(hwnd, IDC_HORA, "");
				SetDlgItemText(hwnd, IDC_MINUTO, "");
				SetDlgItemText(hwnd, IDC_ESTCITA, "");
				SetDlgItemText(hwnd, IDC_CONSULT, "");
				SetDlgItemText(hwnd, IDC_DESCR, "");
				SendDlgItemMessage(hwnd, IDC_FECHAS, DTM_SETSYSTEMTIME, (WPARAM)GDT_VALID, (LPARAM)&fechahoy);
				EnableWindow(GetDlgItem(hwnd, IDC_ATENDIDA), false);
				EnableWindow(GetDlgItem(hwnd, IDC_DESCR), false);
				EnableWindow(GetDlgItem(hwnd, IDC_CANCELAR), false);


			}break;


			}

		}break;

		case IDC_COMBOCITAS: {

			switch (HIWORD(wParam)) {

			case CBN_SELCHANGE: {

				char citas[5];

				GetDlgItemText(hwnd, IDC_COMBOCITAS, citas, 5);

				auxCitas = ListaCitas.getInicio();
				while (auxCitas != nullptr && strcmp(citas, auxCitas->getData().citaNumero) != 0) {
					auxCitas = auxCitas->sig;
				}

				SetDlgItemText(hwnd, IDC_NCITA, auxCitas->getData().citaNumero);
				SetDlgItemText(hwnd, IDC_NOMPACIENTE, auxCitas->getData().NombrePaciente);
				SetDlgItemText(hwnd, IDC_TELEFONO, auxCitas->getData().telefono);
				SetDlgItemText(hwnd, IDC_ESPE2, auxCitas->getData().nombreEspecialidad);
				SetDlgItemText(hwnd, IDC_MED2, auxCitas->getData().NombreMedico);
				SetDlgItemText(hwnd, IDC_HORA, auxCitas->getData().horaTxt);
				SetDlgItemText(hwnd, IDC_MINUTO, auxCitas->getData().minutosTxt);

				SYSTEMTIME nuevo;
				nuevo = auxCitas->getData().fechacita;
				SendDlgItemMessage(hwnd, IDC_FECHAS, DTM_SETSYSTEMTIME, (WPARAM)GDT_VALID, (LPARAM)&nuevo);

				SetDlgItemText(hwnd, IDC_CONSULT, auxCitas->getData().numconsultorio);
				SetDlgItemText(hwnd, IDC_DESCR, auxCitas->getData().descripcionCancelacion);

				if (auxCitas->getData().estadoCita == 0) {

					SetDlgItemText(hwnd, IDC_ESTCITA, "PENDIENTE");
					EnableWindow(GetDlgItem(hwnd, IDC_ATENDIDA), true);
					EnableWindow(GetDlgItem(hwnd, IDC_DESCR), true);
					EnableWindow(GetDlgItem(hwnd, IDC_CANCELAR), true);

				}
				else if (auxCitas->getData().estadoCita == 1) {

					SetDlgItemText(hwnd, IDC_ESTCITA, "ATENDIDA");
					EnableWindow(GetDlgItem(hwnd, IDC_ATENDIDA), false);
					EnableWindow(GetDlgItem(hwnd, IDC_DESCR), false);
					EnableWindow(GetDlgItem(hwnd, IDC_CANCELAR), false);
				}
				else if (auxCitas->getData().estadoCita == 2) {

					SetDlgItemText(hwnd, IDC_ESTCITA, "CANCELADA");

				}

			}break;
			}


		}break;

		case IDC_ATENDIDA: {

			int atender = MessageBox(hwnd, "¿Desea poner como atendida esta cita?", "AVISO", MB_YESNO | MB_ICONQUESTION);
			switch (atender) {
			case IDYES: {

				auxCitas->info.estadoCita = 1;

				SetDlgItemText(hwnd, IDC_ESTCITA, "ATENDIDA");
				EnableWindow(GetDlgItem(hwnd, IDC_ATENDIDA), false);
				EnableWindow(GetDlgItem(hwnd, IDC_DESCR), false);
				EnableWindow(GetDlgItem(hwnd, IDC_CANCELAR), false);

			}break;
			}


		}break;

		case IDC_CANCELAR: {

			int cancelar = MessageBox(hwnd, "¿Desea cancelar esta cita?", "AVISO", MB_YESNO | MB_ICONQUESTION);
			switch (cancelar) {
			case IDYES: {

				char Motivo[300];

				GetDlgItemText(hwnd, IDC_DESCR, Motivo, 300);

				if (strlen(Motivo) == 0) {

					MessageBox(hwnd, "Por favor, añada un motivo a su cancelacion para continuar.", "AVISO", MB_OK | MB_ICONINFORMATION);

				}
				else {
					auxCitas->info.estadoCita = 2;


					SetDlgItemText(hwnd, IDC_ESTCITA, "CANCELADA");
					strcpy_s(auxCitas->info.descripcionCancelacion, Motivo);

					EnableWindow(GetDlgItem(hwnd, IDC_ATENDIDA), false);
					EnableWindow(GetDlgItem(hwnd, IDC_DESCR), false);
					EnableWindow(GetDlgItem(hwnd, IDC_CANCELAR), false);

				}


			}break;//yes


			}//switchcancelar


		}break;

		case IDC_ARCHIVO: {

			int report = MessageBox(hwnd, "¿Desea generar un reporte de las citas?", "AVISO", MB_YESNO | MB_ICONQUESTION);
			switch (report) {
			case IDYES: {

				ofstream archivoCitasSemana;

				int cita = semanabuscar.wDay;
				int cita2 = semanabuscar.wDay;
				int citames = semanabuscar.wMonth;
				int citames2 = semanabuscar.wMonth;
				char consultorios[5];

				GetDlgItemText(hwnd, IDC_CONSULTORIO, consultorios, 5);

				archivoCitasSemana.open("C:\\Users\\jpfb0\\Desktop\\ESTRUCTURA DATOS WINAPI\\ESTRUCTURA DATOS WINAPI\\Reportes\\CitasSemana.txt", ios::out | ios::trunc);

				if (archivoCitasSemana.is_open()) {

					archivoCitasSemana << "CITAS REGISTRADAS"<<endl<<endl;

					auxCitas = ListaCitas.getInicio();

					while (auxCitas != nullptr) {

						SYSTEMTIME citapacienteRep;
						citapacienteRep = auxCitas->getData().fechacita;

						for (int i = cita2; i <= (cita2 + 6); i++) {
							if (strcmp(consultorios, auxCitas->getData().numconsultorio) == 0 && citapacienteRep.wDay == cita && citapacienteRep.wMonth == citames) {
								
								archivoCitasSemana << "N° de cita: "<<auxCitas->getData().citaNumero << endl;
								archivoCitasSemana << "Paciente: " <<auxCitas->getData().NombrePaciente<< endl;
								archivoCitasSemana << "Telefono: " << auxCitas->getData().telefono<< endl;
								archivoCitasSemana << "Especialidad: " << auxCitas->getData().nombreEspecialidad << endl;
								archivoCitasSemana << "Medico: " << auxCitas->getData().NombreMedico << endl;
								archivoCitasSemana << "Hora: " << auxCitas->getData().horaTxt<<" : "<< auxCitas->getData().minutosTxt<<endl;
								archivoCitasSemana << "Fecha: " << auxCitas->getData().fechacita.wDay<<" / "<<auxCitas->getData().fechacita.wMonth<<" / "<<auxCitas->getData().fechacita.wYear << endl;
								archivoCitasSemana << "Consultorio: " <<auxCitas->getData().numconsultorio<< endl;
								if (auxCitas->getData().estadoCita == 0) {
									archivoCitasSemana << "Estado: PENDIENTE" << endl;
								}
								else if (auxCitas->getData().estadoCita == 1) {
									archivoCitasSemana << "Estado: ATENDIDA" << endl;
								}
								else if (auxCitas->getData().estadoCita == 2) {
									archivoCitasSemana << "Estado: CANCELADA" << endl;
									archivoCitasSemana << "Motivo de cancelacion: " << auxCitas->getData().descripcionCancelacion << endl;
								}

								archivoCitasSemana << endl << endl;
							}
							cita++;
							if (cita == 31) {
								cita = 1;
								citames++;
								if (citames == 12) {
									citames = 1;
								}
							}

						}
						cita = cita2;
						citames = citames2;
						auxCitas = auxCitas->sig;

					}

					MessageBox(hwnd, "Archivo creado en la siguiente direccion: C:\\Users\\jpfb0\\Desktop\\ESTRUCTURA DATOS WINAPI\\ESTRUCTURA DATOS WINAPI\\Reportes\\CitasSemana.txt", "ARCHIVO CREADO", MB_OK | MB_ICONINFORMATION);

				}

			
			}break;

			}



		}break;

		}


	}break;

	}//msg

	return 0;
}

BOOL CALLBACK CITASTODAS(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {

	case WM_CLOSE: {
		int opc = MessageBox(hwnd, "¿Seguro que desea salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);
		switch (opc) {
		case IDYES: {
			DestroyWindow(hwnd);
			PostQuitMessage(0); //Deja de ejecutar el programa
		}break;
		}
	}break; //CLOSE

	case WM_INITDIALOG: {

		SetDlgItemText(hwnd, IDC_USUARIO, sesionIniciada->getData().NombreCompleto);

		auxCitas = ListaCitas.getInicio(); //llenamos registradas citas
		while (auxCitas != nullptr) {
			SendDlgItemMessage(hwnd, IDC_COMBOCITAS, CB_INSERTSTRING, (WPARAM)0, (LPARAM)auxCitas->getData().citaNumero);
			auxCitas = auxCitas->sig;
		}

	}break;

	case WM_COMMAND: {

		menu(wParam, hwnd);

		switch (LOWORD(wParam)) {

		case IDC_COMBOCITAS: {

			switch (HIWORD(wParam)) {

			case CBN_SELCHANGE: {

				char citaBusca[5];

				GetDlgItemText(hwnd, IDC_COMBOCITAS, citaBusca, 5);

				auxCitas = ListaCitas.getInicio();
				while (auxCitas != nullptr && strcmp(citaBusca, auxCitas->getData().citaNumero) != 0) {
					auxCitas = auxCitas->sig;
				}

				SetDlgItemText(hwnd, IDC_NCITA, auxCitas->getData().citaNumero);
				SetDlgItemText(hwnd, IDC_NOMPACIENTE, auxCitas->getData().NombrePaciente);
				SetDlgItemText(hwnd, IDC_TELEFONO, auxCitas->getData().telefono);
				SetDlgItemText(hwnd, IDC_ESPE, auxCitas->getData().nombreEspecialidad);
				SetDlgItemText(hwnd, IDC_MED, auxCitas->getData().NombreMedico);
				SetDlgItemText(hwnd, IDC_HORA, auxCitas->getData().horaTxt);
				SetDlgItemText(hwnd, IDC_MINUTO, auxCitas->getData().minutosTxt);

				SYSTEMTIME nuevo;
				nuevo = auxCitas->getData().fechacita;
				SendDlgItemMessage(hwnd, IDC_FECHAS, DTM_SETSYSTEMTIME, (WPARAM)GDT_VALID, (LPARAM)&nuevo);

				SetDlgItemText(hwnd, IDC_CONSULT, auxCitas->getData().numconsultorio);
				SetDlgItemText(hwnd, IDC_DESCR, auxCitas->getData().descripcionCancelacion);

				if (auxCitas->getData().estadoCita == 0) {

					SetDlgItemText(hwnd, IDC_ESTCITA, "PENDIENTE");
					EnableWindow(GetDlgItem(hwnd, IDC_ATENDIDA), true);
					EnableWindow(GetDlgItem(hwnd, IDC_DESCR), true);
					EnableWindow(GetDlgItem(hwnd, IDC_CANCELAR), true);

				}
				else if (auxCitas->getData().estadoCita == 1) {

					SetDlgItemText(hwnd, IDC_ESTCITA, "ATENDIDA");
					EnableWindow(GetDlgItem(hwnd, IDC_ATENDIDA), false);
					EnableWindow(GetDlgItem(hwnd, IDC_DESCR), false);
					EnableWindow(GetDlgItem(hwnd, IDC_CANCELAR), false);
				}
				else if (auxCitas->getData().estadoCita == 2) {

					SetDlgItemText(hwnd, IDC_ESTCITA, "CANCELADA");

				}


			}break;

			}

		}break;

		case IDC_ATENDIDA: {

			int atender = MessageBox(hwnd, "¿Desea poner como atendida esta cita?", "AVISO", MB_YESNO | MB_ICONQUESTION);
			switch (atender) {
			case IDYES: {

				auxCitas->info.estadoCita = 1;

				SetDlgItemText(hwnd, IDC_ESTCITA, "ATENDIDA");
				EnableWindow(GetDlgItem(hwnd, IDC_ATENDIDA), false);
				EnableWindow(GetDlgItem(hwnd, IDC_DESCR), false);
				EnableWindow(GetDlgItem(hwnd, IDC_CANCELAR), false);

			}break;
			}


		}break;

		case IDC_CANCELAR: {

			int cancelar = MessageBox(hwnd, "¿Desea cancelar esta cita?", "AVISO", MB_YESNO | MB_ICONQUESTION);
			switch (cancelar) {
			case IDYES: {

				char Motivo[300];

				GetDlgItemText(hwnd, IDC_DESCR, Motivo, 300);

				if (strlen(Motivo) == 0) {

					MessageBox(hwnd, "Por favor, añada un motivo a su cancelacion para continuar.", "AVISO", MB_OK | MB_ICONINFORMATION);

				}
				else {
					auxCitas->info.estadoCita = 2;


					SetDlgItemText(hwnd, IDC_ESTCITA, "CANCELADA");
					strcpy_s(auxCitas->info.descripcionCancelacion, Motivo);

					EnableWindow(GetDlgItem(hwnd, IDC_ATENDIDA), false);
					EnableWindow(GetDlgItem(hwnd, IDC_DESCR), false);
					EnableWindow(GetDlgItem(hwnd, IDC_CANCELAR), false);

				}



			}break;


			}


		}break;

		}
	}break;

	}//msg
	return 0;
}

BOOL CALLBACK PROCESPECIALIDADES(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {

	case WM_CLOSE: {
		int opc = MessageBox(hwnd, "¿Seguro que desea salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);
		switch (opc) {
		case IDYES: {
			DestroyWindow(hwnd);
			PostQuitMessage(0); //Deja de ejecutar el programa
		}break;
		}
	}break; //CLOSE

	case WM_INITDIALOG: {

		SetDlgItemText(hwnd, IDC_USUARIO, sesionIniciada->getData().NombreCompleto);
		SetDlgItemText(hwnd, IDC_CLAVE, sesionIniciada->getData().claveUsuario);

		ArbolEspecialidad->LlenarCombobox(GetDlgItem(hwnd,IDC_COMBO1), ArbolEspecialidad->GetRaiz());

	}break; //Initdialog

	case WM_COMMAND: {

		menu(wParam, hwnd);

		switch (LOWORD(wParam)) {

		case IDOK: { //guardar

			Especialidades temporal;

			bool especialidadCorrecto=true;

			GetDlgItemText(hwnd, IDC_EDIT3, temporal.nombre, sizeof(temporal.nombre)); //nombre de especialidad
			GetDlgItemText(hwnd, IDC_EDIT2, temporal.claveTXT, sizeof(temporal.claveTXT));
			GetDlgItemText(hwnd, IDC_EDIT1, temporal.descripcion, sizeof(temporal.descripcion));
			GetDlgItemText(hwnd, IDC_CLAVE, temporal.claveUsuario, sizeof(temporal.claveUsuario));

			if (validarEspecialidad(temporal.nombre)==false) {
				especialidadCorrecto = false;
				MessageBox(hwnd, "El nombre de la especialidad debe contener de 5 a 30 caracteres, y unicamente letras", "ERROR", MB_OK | MB_ICONINFORMATION);

			}

			if (validarNumero(temporal.claveTXT) == false) {
				especialidadCorrecto = false;
				MessageBox(hwnd, "La clave debe ser un contener numeros", "ERROR", MB_OK | MB_ICONINFORMATION);
			}

			if (strlen(temporal.descripcion) == 0) {
				especialidadCorrecto = false;
				MessageBox(hwnd, "Ingrese una descripcion a la especialidad", "ERROR", MB_OK | MB_ICONINFORMATION);
			}

			if(especialidadCorrecto){

				temporal.clave = GetDlgItemInt(hwnd, IDC_EDIT2, NULL, false);

				ArbolEspecialidad->InsertarNodo(temporal);

				MessageBox(hwnd, "Especialdiad registrada exitosamente", "AVISO", MB_OK | MB_ICONINFORMATION);

				SendMessage(GetDlgItem(hwnd, IDC_COMBO1), CB_RESETCONTENT, 0, 0);

				SetDlgItemText(hwnd, IDC_EDIT3, "");
				SetDlgItemText(hwnd, IDC_EDIT2, "");
				SetDlgItemText(hwnd, IDC_EDIT1, "");
				SetDlgItemText(hwnd, IDC_CLAVE, sesionIniciada->getData().claveUsuario);

				ArbolEspecialidad->LlenarCombobox(GetDlgItem(hwnd, IDC_COMBO1), ArbolEspecialidad->GetRaiz());
			}

		}break; //guardar

		case IDELIMINAR: { //eliminar

			int opc = MessageBox(hwnd, "¿Desea eliminar esta especialidad?", "AVISO", MB_YESNO | MB_ICONQUESTION);
			switch (opc) {
			case IDYES: {

				ArbolEspecialidad->EliminarNodo(auxEspecialidad->data.clave);

				MessageBox(hwnd, "Especialidad eliminada exitosamente", "AVISO", MB_OK | MB_ICONINFORMATION);

				SendMessage(GetDlgItem(hwnd, IDC_COMBO1), CB_RESETCONTENT, 0, 0); //vacia el combobox

				SetDlgItemText(hwnd, IDC_EDIT3, "");
				SetDlgItemText(hwnd, IDC_EDIT2, "");
				SetDlgItemText(hwnd, IDC_EDIT1, "");
				SetDlgItemText(hwnd, IDC_CLAVE, sesionIniciada->getData().claveUsuario);

				ArbolEspecialidad->LlenarCombobox(GetDlgItem(hwnd, IDC_COMBO1), ArbolEspecialidad->GetRaiz());

				EnableWindow(GetDlgItem(hwnd, IDOK), true); //activa guardar
				EnableWindow(GetDlgItem(hwnd, IDMODIFICAR), false); //desactiva modificar y eliminar
				EnableWindow(GetDlgItem(hwnd, IDELIMINAR), false);

			}break;

			}


		}break;

		case IDC_COMBO1: {

			switch (HIWORD(wParam)) {

				case CBN_SELCHANGE: {

					EnableWindow(GetDlgItem(hwnd, IDOK), false); //desactiva guardar
					EnableWindow(GetDlgItem(hwnd, IDMODIFICAR), true); //activa modificar y eliminar
					EnableWindow(GetDlgItem(hwnd, IDELIMINAR), true);
				
					char EspecialidadBuscar[30] = { 0 }; //para guardar la clave de usuario del combobox

					GetDlgItemText(hwnd, IDC_COMBO1, EspecialidadBuscar, 30);

					auxEspecialidad = ArbolEspecialidad->BuscarEsp(EspecialidadBuscar);

					SetDlgItemText(hwnd, IDC_EDIT3, auxEspecialidad->data.nombre);//nombre
					SetDlgItemText(hwnd, IDC_EDIT2, auxEspecialidad->data.claveTXT);
					SetDlgItemText(hwnd, IDC_EDIT1, auxEspecialidad->data.descripcion);//descripcion
					SetDlgItemText(hwnd, IDC_CLAVE, auxEspecialidad->data.claveUsuario);//claveusuario

				}break;

			}

		}break;

		case IDMODIFICAR: {

			Especialidades temporal;
			bool especialidadCorrectoMod = true;

			GetDlgItemText(hwnd, IDC_EDIT3, temporal.nombre, sizeof(temporal.nombre)); //nombre de especialidad
			GetDlgItemText(hwnd, IDC_EDIT2, temporal.claveTXT, sizeof(temporal.claveTXT));
			GetDlgItemText(hwnd, IDC_EDIT1, temporal.descripcion, sizeof(temporal.descripcion));
			GetDlgItemText(hwnd, IDC_CLAVE, temporal.claveUsuario, sizeof(temporal.claveUsuario));

			if (validarEspecialidad(temporal.nombre) == false) {
				especialidadCorrectoMod = false;
				MessageBox(hwnd, "El nombre de la especialidad debe contener de 5 a 30 caracteres, y unicamente letras", "ERROR", MB_OK | MB_ICONINFORMATION);

			}

			if (validarNumero(temporal.claveTXT) == false) {
				especialidadCorrectoMod = false;
				MessageBox(hwnd, "La clave debe ser un contener numeros", "ERROR", MB_OK | MB_ICONINFORMATION);
			}

			if (strlen(temporal.claveTXT) == 0) {
				especialidadCorrectoMod = false;
				MessageBox(hwnd, "Ingrese una descripcion a la especialidad", "ERROR", MB_OK | MB_ICONINFORMATION);
			}

			if (especialidadCorrectoMod){

				temporal.clave = GetDlgItemInt(hwnd, IDC_EDIT2, NULL, false);

				ArbolEspecialidad->EliminarNodo(auxEspecialidad->data.clave);

				ArbolEspecialidad->InsertarNodo(temporal);

				MessageBox(hwnd, "Especialidad modificada exitosamente", "AVISO", MB_OK | MB_ICONINFORMATION);

				SendMessage(GetDlgItem(hwnd, IDC_COMBO1), CB_RESETCONTENT, 0, 0); //vacia el combobox

				SetDlgItemText(hwnd, IDC_EDIT3, "");
				SetDlgItemText(hwnd, IDC_EDIT2, "");
				SetDlgItemText(hwnd, IDC_EDIT1, "");
				SetDlgItemText(hwnd, IDC_CLAVE, sesionIniciada->getData().claveUsuario);

				ArbolEspecialidad->LlenarCombobox(GetDlgItem(hwnd, IDC_COMBO1), ArbolEspecialidad->GetRaiz());

				EnableWindow(GetDlgItem(hwnd, IDOK), true); //activa guardar
				EnableWindow(GetDlgItem(hwnd, IDMODIFICAR), false); //desactiva modificar y eliminar
				EnableWindow(GetDlgItem(hwnd, IDELIMINAR), false);
			}

		}break;
		}//switchLoword(wparam)

	}break;

	}//msg

	return 0;
}

BOOL CALLBACK PROCINICIOSESION(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {

	case WM_CLOSE: {
		int opc = MessageBox(hwnd, "¿Seguro que desea salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);
		switch (opc) {
		case IDYES: {
			DestroyWindow(hwnd);
			PostQuitMessage(0); //Deja de ejecutar el programa
		}break;
		}
	}break; //CLOSE

	case WM_COMMAND: {

		switch (LOWORD(wParam)) {
		
		case IDC_BUTTON1: {

			char clave[20] = { 0 };
			char contra[20] = { 0 };

			GetDlgItemText(hwnd, IDC_EDIT1, clave, 20);
			GetDlgItemText(hwnd, IDC_EDIT2, contra, 20);

			auxUsuarios = ListaUsuarios.getInicio();

			while (auxUsuarios != nullptr && strcmp(clave, auxUsuarios->getData().claveUsuario) != 0) {
				auxUsuarios = auxUsuarios->sig;
			}

			if (auxUsuarios == nullptr) {
				MessageBox(hwnd, "El nombre de usuario es incorrecto o no esta registrado.", "AVISO", MB_OK | MB_ICONERROR);
			}
			else {

				if (strcmp(contra, auxUsuarios->getData().password) == 0) {

					sesionIniciada = auxUsuarios;
					EndDialog(hwnd, 0);
					DialogBox(hInstGlobal, MAKEINTRESOURCE(PRINCIPAL), hwnd, MAINMENU);

				}
				else {
					MessageBox(hwnd, "Contraseña incorrecta, intentelo nuevamente", "AVISO", MB_OK | MB_ICONERROR);
				}

			}
			}break; //boton 
		
		}

	}break;//wmcommand

	}//msg

	return 0;
}

BOOL CALLBACK PROCMEDICOS(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {

	case WM_CLOSE: {

		int salir = MessageBox(hwnd, "¿Seguro que desea salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);

		switch (salir) {

		case IDYES: {
			DestroyWindow(hwnd);
			PostQuitMessage(0); //Deja de ejecutar el programa
		}break;

		}

	}break; //CLOSE

	case WM_INITDIALOG: {

		SetDlgItemText(hwnd, IDC_USUARIO, sesionIniciada->getData().NombreCompleto);
		SetDlgItemText(hwnd, IDC_CLAVE, sesionIniciada->getData().claveUsuario);

		auxMedicos = ListaMedicos.getInicio();

		while (auxMedicos != nullptr) { //llena el combobox con nombres de medicos
			SendDlgItemMessage(hwnd, IDC_COMBO2, CB_INSERTSTRING, (WPARAM)0, (LPARAM)auxMedicos->getData().NombreCompleto);
			auxMedicos = auxMedicos->sig;
		}

		ArbolEspecialidad->LlenarCombobox(GetDlgItem(hwnd, IDC_ESPECIAL), ArbolEspecialidad->GetRaiz());

		char numerodeconsultorios[4][3] = { "1","2","3","4" };

		for (int i = 0; i < 4; i++) {
			SendDlgItemMessage(hwnd, IDC_CONSULTORIO, CB_INSERTSTRING, (WPARAM)i, (LPARAM)numerodeconsultorios[i]);
		}

	}break; //Initdialog

	case WM_COMMAND: {

		menu(wParam, hwnd);

		switch (LOWORD(wParam)) {
		
		case IDOK: { //guardar

			Medicos temporal;

			bool MedicoCorrecto = true;

			char NombreCompletoMed[80] = { 0 };

			GetDlgItemText(hwnd, IDC_EDIT1, temporal.cedulaTXT, sizeof(temporal.cedulaTXT));
			GetDlgItemText(hwnd, IDC_EDIT4, temporal.apellidoPaterno, sizeof(temporal.apellidoPaterno));//apellido paterno
			GetDlgItemText(hwnd, IDC_EDIT7, temporal.apellidoMaterno, sizeof(temporal.apellidoMaterno));//apellido materno
			GetDlgItemText(hwnd, IDC_EDIT2, temporal.nombres, sizeof(temporal.nombres)); //nombres
			
			strcat_s(NombreCompletoMed, temporal.apellidoPaterno);
			strcat_s(NombreCompletoMed, " ");
			strcat_s(NombreCompletoMed, temporal.apellidoMaterno);
			strcat_s(NombreCompletoMed, " ");
			strcat_s(NombreCompletoMed, temporal.nombres);

			strcpy_s(temporal.NombreCompleto, NombreCompletoMed);

			GetDlgItemText(hwnd, IDC_ESPECIAL, temporal.especialidad, sizeof(temporal.especialidad));//especialidad
			GetDlgItemText(hwnd, IDC_CONSULTORIO, temporal.numconsultorio, sizeof(temporal.numconsultorio)); //numero consultorio

			
			if (IsDlgButtonChecked(hwnd, IDC_RADIO1) == BST_CHECKED) {
				temporal.horario = 1; //matutino
			}
			else if (IsDlgButtonChecked(hwnd, IDC_RADIO2) == BST_CHECKED) {

				temporal.horario = 2; //vespertino
			}

			strcpy_s(temporal.foto, zFile); //guardar direccion de foto

			GetDlgItemText(hwnd, IDC_EDIT12, temporal.telefono, sizeof(temporal.telefono)); //telefono

			//revisa dias
			//lunes
			if (IsDlgButtonChecked(hwnd, IDC_LUNES) == BST_CHECKED) {
				temporal.lunes = true;
			}
			else if (IsDlgButtonChecked(hwnd, IDC_LUNES) == BST_UNCHECKED) {
				temporal.lunes = false;
			}

			//martes
			if (IsDlgButtonChecked(hwnd, IDC_MARTES) == BST_CHECKED) {
				temporal.martes = true;
			}
			else if (IsDlgButtonChecked(hwnd, IDC_MARTES) == BST_UNCHECKED) {
				temporal.martes = false;
			}

			//miercoles
			if (IsDlgButtonChecked(hwnd, IDC_MIERCOLES) == BST_CHECKED) {
				temporal.miercoles = true;
			}
			else if (IsDlgButtonChecked(hwnd, IDC_MIERCOLES) == BST_UNCHECKED) {
				temporal.miercoles = false;
			}

			//jueves
			if (IsDlgButtonChecked(hwnd, IDC_JUEVES) == BST_CHECKED) {
				temporal.jueves = true;
			}
			else if (IsDlgButtonChecked(hwnd, IDC_JUEVES) == BST_UNCHECKED) {
				temporal.jueves = false;
			}

			//viernes
			if (IsDlgButtonChecked(hwnd, IDC_VIERNES) == BST_CHECKED) {
				temporal.viernes = true;
			}
			else if (IsDlgButtonChecked(hwnd, IDC_VIERNES) == BST_UNCHECKED) {
				temporal.viernes = false;
			}

			GetDlgItemText(hwnd, IDC_CLAVE, temporal.claveUsuario, sizeof(temporal.claveUsuario)); //clave usuario

			if (validarNumero(temporal.cedulaTXT) == false) {
				MedicoCorrecto = false;
				MessageBox(hwnd, "La cedula debe tener unicamente numeros.", "ERROR", MB_OK | MB_ICONINFORMATION);
			}

			if (validarApellido(temporal.apellidoPaterno) == false || validarApellido(temporal.apellidoMaterno) == false) {
				MedicoCorrecto = false;
				MessageBox(hwnd, "Los apellidos unicamente pueden contener letras y tener hasta 20 caracteres.", "ERROR", MB_OK | MB_ICONINFORMATION);
				 
			}

			if (validarNombres(temporal.nombres) == false) {
				MedicoCorrecto = false;
				MessageBox(hwnd, "Los nombres unicamente pueden contener letras y tener hasta 40 caracteres", "ERROR", MB_OK | MB_ICONINFORMATION);
			}

			if (strlen(temporal.especialidad) == 0) {
				MedicoCorrecto = false;
				MessageBox(hwnd, "Selecciona una especialidad.", "ERROR", MB_OK | MB_ICONINFORMATION);
			}

			if (strlen(temporal.numconsultorio) == 0) {
				MedicoCorrecto = false;
				MessageBox(hwnd, "Selecciona una consultorio.", "ERROR", MB_OK | MB_ICONINFORMATION);
			}

			if (temporal.horario !=1 && temporal.horario != 2) {
				MedicoCorrecto = false;
				MessageBox(hwnd, "Selecciona una horario de consulta.", "ERROR", MB_OK | MB_ICONINFORMATION);
			}

			if (strlen(temporal.foto) == 0) {
				MedicoCorrecto = false;
				MessageBox(hwnd, "Selecciona una foto.", "ERROR", MB_OK | MB_ICONINFORMATION);
			}

			if (validarTelefono(temporal.telefono) == false) {
				MedicoCorrecto = false;
				MessageBox(hwnd, "El numero de telefono debe tener 10 caracteres y unicamente numeros.", "ERROR", MB_OK | MB_ICONINFORMATION);
			}

			if (temporal.lunes == false && temporal.martes == false && temporal.miercoles == false && temporal.jueves == false && temporal.viernes == false) {
				MedicoCorrecto = false;
				MessageBox(hwnd, "Selecciona al menos un dia de consulta.", "ERROR", MB_OK | MB_ICONINFORMATION);
			}


			auxMedicos = ListaMedicos.getInicio();

			while (auxMedicos != nullptr) {

				if (auxMedicos->getData().lunes == true && temporal.lunes && true && auxMedicos->getData().horario == temporal.horario && strcmp(auxMedicos->getData().numconsultorio,temporal.numconsultorio)==0) {
					MedicoCorrecto = false;
					MessageBox(hwnd, "Este consultorio esta ocupado el dia lunes en el horario seleccionado.", "ERROR", MB_OK | MB_ICONINFORMATION);

				}

				if (auxMedicos->getData().martes == true && temporal.martes == true && auxMedicos->getData().horario == temporal.horario && strcmp(auxMedicos->getData().numconsultorio, temporal.numconsultorio) == 0) {
					MedicoCorrecto = false;
					MessageBox(hwnd, "Este consultorio esta ocupado el dia martes en el horario seleccionado.", "ERROR", MB_OK | MB_ICONINFORMATION);

				}

				if (auxMedicos->getData().miercoles == true && temporal.miercoles == true && auxMedicos->getData().horario == temporal.horario && strcmp(auxMedicos->getData().numconsultorio, temporal.numconsultorio) == 0) {
					MedicoCorrecto = false;
					MessageBox(hwnd, "Este consultorio esta ocupado el dia miercoles en el horario seleccionado.", "ERROR", MB_OK | MB_ICONINFORMATION);

				}

				if (auxMedicos->getData().jueves == true && temporal.jueves == true && auxMedicos->getData().horario == temporal.horario && strcmp(auxMedicos->getData().numconsultorio, temporal.numconsultorio) == 0) {
					MedicoCorrecto = false;
					MessageBox(hwnd, "Este consultorio esta ocupado el dia jueves en el horario seleccionado.", "ERROR", MB_OK | MB_ICONINFORMATION);

				}

				if (auxMedicos->getData().viernes == true && temporal.viernes == true && auxMedicos->getData().horario == temporal.horario && strcmp(auxMedicos->getData().numconsultorio, temporal.numconsultorio) == 0) {
					MedicoCorrecto = false;
					MessageBox(hwnd, "Este consultorio esta ocupado el dia viernes en el horario seleccionado.", "ERROR", MB_OK | MB_ICONINFORMATION);

				}
				auxMedicos = auxMedicos->sig;
			}


			if(MedicoCorrecto){

				temporal.cedula = GetDlgItemInt(hwnd, IDC_EDIT1, NULL, false); //cedulaperoint

				auxMedicos = new Nodo<Medicos>(temporal);

				ListaMedicos.agregarNodo(auxMedicos);

				MessageBox(hwnd, "Medico registrado exitosamente", "AVISO", MB_OK | MB_ICONINFORMATION);

				SendMessage(GetDlgItem(hwnd, IDC_COMBO2), CB_RESETCONTENT, 0, 0); //borramos combobox de modifcar medicos

				auxMedicos = ListaMedicos.getInicio();

				while (auxMedicos != nullptr) { //llena el combobox con nombres de medicos
					SendDlgItemMessage(hwnd, IDC_COMBO2, CB_INSERTSTRING, (WPARAM)0, (LPARAM)auxMedicos->getData().NombreCompleto);
					auxMedicos = auxMedicos->sig;
				}

				//borramos todo
				SetDlgItemText(hwnd, IDC_EDIT1, ""); //
				SetDlgItemText(hwnd, IDC_EDIT4, "");
				SetDlgItemText(hwnd, IDC_EDIT7, "");
				SetDlgItemText(hwnd, IDC_EDIT2, "");

				SendMessage(GetDlgItem(hwnd, IDC_ESPECIAL), CB_RESETCONTENT, 0, 0);
				ArbolEspecialidad->LlenarCombobox(GetDlgItem(hwnd, IDC_ESPECIAL), ArbolEspecialidad->GetRaiz());

				SendMessage(GetDlgItem(hwnd, IDC_CONSULTORIO), CB_RESETCONTENT, 0, 0);

				char numerodeconsultorios[4][3] = { "1","2","3","4" };
				for (int i = 0; i < 4; i++) {
					SendDlgItemMessage(hwnd, IDC_CONSULTORIO, CB_INSERTSTRING, (WPARAM)i, (LPARAM)numerodeconsultorios[i]);
				}

				SetDlgItemText(hwnd, IDC_EDIT12, "");

				SendDlgItemMessage(hwnd, FOTOMED, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)NULL);
				SendDlgItemMessage(hwnd, IDC_RADIO1, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);  //matutino
				SendDlgItemMessage(hwnd, IDC_RADIO2, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);  //vespertino
				SendDlgItemMessage(hwnd, IDC_LUNES, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
				SendDlgItemMessage(hwnd, IDC_MARTES, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
				SendDlgItemMessage(hwnd, IDC_MIERCOLES, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
				SendDlgItemMessage(hwnd, IDC_JUEVES, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
				SendDlgItemMessage(hwnd, IDC_VIERNES, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);

				SetDlgItemText(hwnd, IDC_CLAVE, sesionIniciada->getData().claveUsuario);
			}


		}break;//guardar

		case IDC_BUTTON1: { //guardar foto

			OPENFILENAME ofn;

			ZeroMemory(&ofn, sizeof(ofn));

			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFilter = "Todos\0 * .*\0ImagenBMP\0 * .bmp\0";
			ofn.lpstrFile = zFile;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(zFile);
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			ofn.nFilterIndex = 2;

			if (GetOpenFileName(&ofn) == TRUE) {
				HBITMAP bmp; //1

				bmp = (HBITMAP)LoadImage(NULL, zFile, IMAGE_BITMAP, 150, 150, LR_LOADFROMFILE); //2

				SendDlgItemMessage(hwnd, FOTOMED, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp); //3 

			}
			else {
				MessageBox(hwnd, "No seleccionó una fotografia", "AVISO", MB_OK | MB_ICONERROR);
			}


		}break;//foto

		case IDC_COMBO2: { //combobox

			switch (HIWORD(wParam)) {

			case CBN_SELCHANGE: {

				EnableWindow(GetDlgItem(hwnd, IDOK), false); //desactiva guardar
				EnableWindow(GetDlgItem(hwnd, IDMODIFICAR), true); //activa modificar y eliminar
				EnableWindow(GetDlgItem(hwnd, IDBORRAR), true);

				char NombreCompletoBuscar[80];

				GetDlgItemText(hwnd, IDC_COMBO2, NombreCompletoBuscar, 80); //guarda el nombre completo

				auxMedicos = ListaMedicos.getInicio();

				while (auxMedicos->sig != nullptr && strcmp(NombreCompletoBuscar, auxMedicos->getData().NombreCompleto) != 0) {

					auxMedicos = auxMedicos->sig;
				}

				SetDlgItemText(hwnd, IDC_EDIT1, auxMedicos->getData().cedulaTXT);
				SetDlgItemText(hwnd, IDC_EDIT4, auxMedicos->getData().apellidoPaterno); //paterno
				SetDlgItemText(hwnd, IDC_EDIT7, auxMedicos->getData().apellidoMaterno); //materno
				SetDlgItemText(hwnd, IDC_EDIT2, auxMedicos->getData().nombres); //nombres

				//falta el 
				/*SetDlgItemText(hwnd, IDC_ESPECIAL, auxMedicos->getData().especialidad);*/
				ComboBox_SelectString(GetDlgItem(hwnd, IDC_ESPECIAL), 0, auxMedicos->getData().especialidad);
				ComboBox_SelectString(GetDlgItem(hwnd, IDC_CONSULTORIO), 0, auxMedicos->getData().numconsultorio);

				/*SetDlgItemText(hwnd, IDC_CONSULTORIO, auxMedicos->getData().numconsultorio);*/

				if (auxMedicos->getData().horario == 1) { //matutino
					SendDlgItemMessage(hwnd, IDC_RADIO1, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
					SendDlgItemMessage(hwnd, IDC_RADIO2, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
				}
				else if (auxMedicos->getData().horario == 2) { //vespertino
					SendDlgItemMessage(hwnd, IDC_RADIO2, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
					SendDlgItemMessage(hwnd, IDC_RADIO1, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
				}

				HBITMAP bmp; //cargamos foto
				bmp = (HBITMAP)LoadImage(NULL, auxMedicos->getData().foto, IMAGE_BITMAP, 150, 150, LR_LOADFROMFILE); //2
				strcpy_s(zFile, auxMedicos->getData().foto);
				SendDlgItemMessage(hwnd, FOTOMED, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);

				SetDlgItemText(hwnd, IDC_EDIT12, auxMedicos->getData().telefono); //telefono

				SendDlgItemMessage(hwnd, IDC_LUNES, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
				SendDlgItemMessage(hwnd, IDC_MARTES, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
				SendDlgItemMessage(hwnd, IDC_MIERCOLES, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
				SendDlgItemMessage(hwnd, IDC_JUEVES, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
				SendDlgItemMessage(hwnd, IDC_VIERNES, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);

				if (auxMedicos->getData().lunes == true) {
					SendDlgItemMessage(hwnd, IDC_LUNES, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
				}

				if (auxMedicos->getData().martes == true) {
					SendDlgItemMessage(hwnd, IDC_MARTES, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
				}

				if (auxMedicos->getData().miercoles == true) {
					SendDlgItemMessage(hwnd, IDC_MIERCOLES, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
				}

				if (auxMedicos->getData().jueves == true) {
					SendDlgItemMessage(hwnd, IDC_JUEVES, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
				}

				if (auxMedicos->getData().viernes == true) {
					SendDlgItemMessage(hwnd, IDC_VIERNES, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
				}

				SetDlgItemText(hwnd, IDC_CLAVE, auxMedicos->getData().claveUsuario);
				

			}break;

			}

		}break; //combobox

		case IDBORRAR: { //eliminar

			int qst = MessageBox(hwnd, "¿Desea eliminar este medico?", "AVISO", MB_YESNO | MB_ICONQUESTION);
			switch (qst) {
			case IDYES: {

				ListaMedicos.eliminarNodo(auxMedicos);

				MessageBox(hwnd, "Medico eliminado exitosamente", "AVISO", MB_OK | MB_ICONINFORMATION);

				//borramos combobox de modifcar medicos

				//borramos todo
				SetDlgItemText(hwnd, IDC_EDIT1, ""); 
				SetDlgItemText(hwnd, IDC_EDIT4, "");
				SetDlgItemText(hwnd, IDC_EDIT7, "");
				SetDlgItemText(hwnd, IDC_EDIT2, "");

				SendMessage(GetDlgItem(hwnd, IDC_ESPECIAL), CB_RESETCONTENT, 0, 0);
				ArbolEspecialidad->LlenarCombobox(GetDlgItem(hwnd, IDC_ESPECIAL), ArbolEspecialidad->GetRaiz());

				SendMessage(GetDlgItem(hwnd, IDC_CONSULTORIO), CB_RESETCONTENT, 0, 0);

				char numerodeconsultorios[4][3] = { "1","2","3","4" };

				for (int i = 0; i < 4; i++) {
					SendDlgItemMessage(hwnd, IDC_CONSULTORIO, CB_INSERTSTRING, (WPARAM)i, (LPARAM)numerodeconsultorios[i]);
				}

				SetDlgItemText(hwnd, IDC_CONSULTORIO, "");
				SetDlgItemText(hwnd, IDC_EDIT12, "");
				SendDlgItemMessage(hwnd, FOTOMED, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)NULL);
				SendDlgItemMessage(hwnd, IDC_RADIO1, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);  //matutino
				SendDlgItemMessage(hwnd, IDC_RADIO2, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);  //vespertino
				SendDlgItemMessage(hwnd, IDC_LUNES, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
				SendDlgItemMessage(hwnd, IDC_MARTES, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
				SendDlgItemMessage(hwnd, IDC_MIERCOLES, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
				SendDlgItemMessage(hwnd, IDC_JUEVES, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
				SendDlgItemMessage(hwnd, IDC_VIERNES, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);

				SetDlgItemText(hwnd, IDC_CLAVE, sesionIniciada->getData().claveUsuario);

				SendMessage(GetDlgItem(hwnd, IDC_COMBO2), CB_RESETCONTENT, 0, 0);

				auxMedicos = ListaMedicos.getInicio();

				while (auxMedicos != nullptr) { //llena el combobox con nombres de medicos
					SendDlgItemMessage(hwnd, IDC_COMBO2, CB_INSERTSTRING, (WPARAM)0, (LPARAM)auxMedicos->getData().NombreCompleto);
					auxMedicos = auxMedicos->sig;
				}

				EnableWindow(GetDlgItem(hwnd, IDOK), true); //desactiva guardar
				EnableWindow(GetDlgItem(hwnd, IDMODIFICAR), false); //activa modificar y eliminar
				EnableWindow(GetDlgItem(hwnd, IDBORRAR), false);


			}break;
			}


		}break; //eliminar

		case IDMODIFICAR: { //modificar

			int opcion = MessageBox(hwnd, "¿Desea modificar este médico?", "AVISO", MB_YESNO | MB_ICONQUESTION);
			switch (opcion) {
			case IDYES: {

				Medicos temporal;
				
				bool MedicoCorrectoMod = true;

				char NombreCompletoMed[80] = { 0 };

				GetDlgItemText(hwnd, IDC_EDIT1, temporal.cedulaTXT, sizeof(temporal.cedulaTXT));
				GetDlgItemText(hwnd, IDC_EDIT4, temporal.apellidoPaterno, sizeof(temporal.apellidoPaterno));//apellido paterno
				GetDlgItemText(hwnd, IDC_EDIT7, temporal.apellidoMaterno, sizeof(temporal.apellidoMaterno));//apellido materno
				GetDlgItemText(hwnd, IDC_EDIT2, temporal.nombres, sizeof(temporal.nombres)); //nombres

				strcat_s(NombreCompletoMed, temporal.apellidoPaterno);
				strcat_s(NombreCompletoMed, " ");
				strcat_s(NombreCompletoMed, temporal.apellidoMaterno);
				strcat_s(NombreCompletoMed, " ");
				strcat_s(NombreCompletoMed, temporal.nombres);

				strcpy_s(temporal.NombreCompleto, NombreCompletoMed);

				GetDlgItemText(hwnd, IDC_ESPECIAL, temporal.especialidad, sizeof(temporal.especialidad));//especialidad
				GetDlgItemText(hwnd, IDC_CONSULTORIO, temporal.numconsultorio, sizeof(temporal.numconsultorio));


				if (IsDlgButtonChecked(hwnd, IDC_RADIO1) == BST_CHECKED) {

					temporal.horario = 1; //matutino
				}
				else if (IsDlgButtonChecked(hwnd, IDC_RADIO2) == BST_CHECKED) {

					temporal.horario = 2; //vespertino
				}

				strcpy_s(temporal.foto, zFile); //guardar direccion de foto

				GetDlgItemText(hwnd, IDC_EDIT12, temporal.telefono, sizeof(temporal.telefono)); //telefono

				//revisa dias
				//lunes
				if (IsDlgButtonChecked(hwnd, IDC_LUNES) == BST_CHECKED) {
					temporal.lunes = true;
				}
				else if (IsDlgButtonChecked(hwnd, IDC_LUNES) == BST_UNCHECKED) {
					temporal.lunes = false;
				}

				//martes
				if (IsDlgButtonChecked(hwnd, IDC_MARTES) == BST_CHECKED) {
					temporal.martes = true;
				}
				else if (IsDlgButtonChecked(hwnd, IDC_MARTES) == BST_UNCHECKED) {
					temporal.martes = false;
				}

				//miercoles
				if (IsDlgButtonChecked(hwnd, IDC_MIERCOLES) == BST_CHECKED) {
					temporal.miercoles = true;
				}
				else if (IsDlgButtonChecked(hwnd, IDC_MIERCOLES) == BST_UNCHECKED) {
					temporal.miercoles = false;
				}

				//jueves
				if (IsDlgButtonChecked(hwnd, IDC_JUEVES) == BST_CHECKED) {
					temporal.jueves = true;
				}
				else if (IsDlgButtonChecked(hwnd, IDC_JUEVES) == BST_UNCHECKED) {
					temporal.jueves = false;
				}

				//viernes

				if (IsDlgButtonChecked(hwnd, IDC_VIERNES) == BST_CHECKED) {
					temporal.viernes = true;
				}
				else if (IsDlgButtonChecked(hwnd, IDC_VIERNES) == BST_UNCHECKED) {
					temporal.viernes = false;
				}

				GetDlgItemText(hwnd, IDC_CLAVE, temporal.claveUsuario, sizeof(temporal.claveUsuario)); //clave usuario


				if (validarNumero(temporal.cedulaTXT) == false) {
					MedicoCorrectoMod = false;
					MessageBox(hwnd, "La cedula debe tener unicamente numeros.", "ERROR", MB_OK | MB_ICONINFORMATION);
				}

				if (validarApellido(temporal.apellidoPaterno) == false || validarApellido(temporal.apellidoMaterno) == false) {
					MedicoCorrectoMod = false;
					MessageBox(hwnd, "Los apellidos unicamente pueden contener letras y tener hasta 20 caracteres.", "ERROR", MB_OK | MB_ICONINFORMATION);
				}

				if (validarNombres(temporal.nombres) == false) {
					MedicoCorrectoMod = false;
					MessageBox(hwnd, "Los nombres unicamente pueden contener letras y tener hasta 40 caracteres", "ERROR", MB_OK | MB_ICONINFORMATION);
				}

				if (strlen(temporal.especialidad) == 0) {
					MedicoCorrectoMod = false;
					MessageBox(hwnd, "Selecciona una especialidad.", "ERROR", MB_OK | MB_ICONINFORMATION);
				}

				if (strlen(temporal.numconsultorio) == 0) {
					MedicoCorrectoMod = false;
					MessageBox(hwnd, "Selecciona una consultorio.", "ERROR", MB_OK | MB_ICONINFORMATION);
				}

				if (temporal.horario != 1 && temporal.horario != 2) {
					MedicoCorrectoMod = false;
					MessageBox(hwnd, "Selecciona una horario de consulta.", "ERROR", MB_OK | MB_ICONINFORMATION);
				}

				if (validarTelefono(temporal.telefono) == false) {
					MedicoCorrectoMod = false;
					MessageBox(hwnd, "El numero de telefono debe tener 10 caracteres y unicamente numeros.", "ERROR", MB_OK | MB_ICONINFORMATION);
				}

				if (temporal.lunes == false && temporal.martes == false && temporal.miercoles == false && temporal.jueves == false && temporal.viernes == false) {
					MedicoCorrectoMod = false;
					MessageBox(hwnd, "Selecciona al menos un dia de consulta.", "ERROR", MB_OK | MB_ICONINFORMATION);
				}

				Nodo<Medicos>* auxMedicos222 = nullptr;

				auxMedicos222 = ListaMedicos.getInicio();

				while (auxMedicos222 != nullptr) {

					if (auxMedicos222->getData().lunes == true && temporal.lunes && true && auxMedicos222->getData().horario == temporal.horario && strcmp(auxMedicos222->getData().numconsultorio, temporal.numconsultorio) == 0) {
						MedicoCorrectoMod = false;
						MessageBox(hwnd, "Este consultorio esta ocupado el dia lunes en el horario seleccionado.", "ERROR", MB_OK | MB_ICONINFORMATION);

					}

					if (auxMedicos222->getData().martes == true && temporal.martes == true && auxMedicos222->getData().horario == temporal.horario && strcmp(auxMedicos222->getData().numconsultorio, temporal.numconsultorio) == 0) {
						MedicoCorrectoMod = false;
						MessageBox(hwnd, "Este consultorio esta ocupado el dia martes en el horario seleccionado.", "ERROR", MB_OK | MB_ICONINFORMATION);

					}

					if (auxMedicos222->getData().miercoles == true && temporal.miercoles == true && auxMedicos222->getData().horario == temporal.horario && strcmp(auxMedicos222->getData().numconsultorio, temporal.numconsultorio) == 0) {
						MedicoCorrectoMod = false;
						MessageBox(hwnd, "Este consultorio esta ocupado el dia miercoles en el horario seleccionado.", "ERROR", MB_OK | MB_ICONINFORMATION);

					}

					if (auxMedicos222->getData().jueves == true && temporal.jueves == true && auxMedicos222->getData().horario == temporal.horario && strcmp(auxMedicos222->getData().numconsultorio, temporal.numconsultorio) == 0) {
						MedicoCorrectoMod = false;
						MessageBox(hwnd, "Este consultorio esta ocupado el dia jueves en el horario seleccionado.", "ERROR", MB_OK | MB_ICONINFORMATION);

					}

					if (auxMedicos222->getData().viernes == true && temporal.viernes == true && auxMedicos222->getData().horario == temporal.horario && strcmp(auxMedicos222->getData().numconsultorio, temporal.numconsultorio) == 0) {
						MedicoCorrectoMod = false;
						MessageBox(hwnd, "Este consultorio esta ocupado el dia viernes en el horario seleccionado.", "ERROR", MB_OK | MB_ICONINFORMATION);

					}
					auxMedicos222 = auxMedicos222->sig;
				}


				if(MedicoCorrectoMod){

					temporal.cedula = GetDlgItemInt(hwnd, IDC_EDIT1, NULL, false); //cedula

					ListaMedicos.modificarNodo(auxMedicos, temporal);

					MessageBox(hwnd, "Medico modificado exitosamente", "AVISO", MB_OK | MB_ICONINFORMATION);

					SendMessage(GetDlgItem(hwnd, IDC_COMBO2), CB_RESETCONTENT, 0, 0); //borramos combobox de modifcar medicos

					//borramos todo
					SetDlgItemText(hwnd, IDC_EDIT1, ""); //
					SetDlgItemText(hwnd, IDC_EDIT4, "");
					SetDlgItemText(hwnd, IDC_EDIT7, "");
					SetDlgItemText(hwnd, IDC_EDIT2, "");

					SendMessage(GetDlgItem(hwnd, IDC_ESPECIAL), CB_RESETCONTENT, 0, 0);
					ArbolEspecialidad->LlenarCombobox(GetDlgItem(hwnd, IDC_ESPECIAL), ArbolEspecialidad->GetRaiz());

					SendMessage(GetDlgItem(hwnd, IDC_CONSULTORIO), CB_RESETCONTENT, 0, 0);
					char numerodeconsultorios[4][3] = { "1","2","3","4" };
					for (int i = 0; i < 4; i++) {
						SendDlgItemMessage(hwnd, IDC_CONSULTORIO, CB_INSERTSTRING, (WPARAM)i, (LPARAM)numerodeconsultorios[i]);
					}

					SetDlgItemText(hwnd, IDC_EDIT12, "");
					SendDlgItemMessage(hwnd, FOTOMED, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)NULL);
					SendDlgItemMessage(hwnd, IDC_RADIO1, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);  //matutino
					SendDlgItemMessage(hwnd, IDC_RADIO2, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);  //vespertino
					SendDlgItemMessage(hwnd, IDC_LUNES, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
					SendDlgItemMessage(hwnd, IDC_MARTES, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
					SendDlgItemMessage(hwnd, IDC_MIERCOLES, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
					SendDlgItemMessage(hwnd, IDC_JUEVES, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
					SendDlgItemMessage(hwnd, IDC_VIERNES, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);

					SetDlgItemText(hwnd, IDC_CLAVE, sesionIniciada->getData().claveUsuario);

					auxMedicos = ListaMedicos.getInicio();

					while (auxMedicos != nullptr) { //llena el combobox con nombres de medicos
						SendDlgItemMessage(hwnd, IDC_COMBO2, CB_INSERTSTRING, (WPARAM)0, (LPARAM)auxMedicos->getData().NombreCompleto);
						auxMedicos = auxMedicos->sig;
					}

					EnableWindow(GetDlgItem(hwnd, IDOK), true); //desactiva guardar
					EnableWindow(GetDlgItem(hwnd, IDMODIFICAR), false); //activa modificar y eliminar
					EnableWindow(GetDlgItem(hwnd, IDBORRAR), false);
				}


			}break;
			}

		}break; //modificar

		case IDREPORTE: {

			int report = MessageBox(hwnd, "¿Desea generar un reporte de medicos?", "AVISO", MB_YESNO | MB_ICONQUESTION);
			switch (report) {
			case IDYES: {

				if (ListaMedicos.getInicio() != nullptr) {

					ListaMedicos.quicksort();

					auxMedicos = ListaMedicos.getInicio();

					ofstream ReporteMedicos;

					ReporteMedicos.open("C:\\Users\\jpfb0\\Desktop\\ESTRUCTURA DATOS WINAPI\\ESTRUCTURA DATOS WINAPI\\Reportes\\ReporteMedicos.csv", ios::out | ios::trunc);

					if (ReporteMedicos.is_open()) {

						ReporteMedicos << "Usuario registro" << "," << "Cedula" << "," << "Nombre completo" << "," << "Especialidad" << "," << "N° de consultorio" << "," << "Horario" << "," << "Telefono" << endl;

						while (auxMedicos != nullptr) {

							ReporteMedicos << auxMedicos->getData().claveUsuario << "," << auxMedicos->getData().cedulaTXT << "," << auxMedicos->getData().NombreCompleto << "," << auxMedicos->getData().especialidad << "," << auxMedicos->getData().numconsultorio<<",";

							if (auxMedicos->getData().horario == 1) {
								ReporteMedicos << "MATUTINO"<<",";
							}
							else {
								ReporteMedicos << "VESPERTINO"<<",";
							}
							ReporteMedicos << auxMedicos->getData().telefono<<endl;

							auxMedicos = auxMedicos->sig;
						}

					}

					MessageBox(hwnd, "Reporte creado en la siguiente direccion: C:\\Users\\jpfb0\\Desktop\\ESTRUCTURA DATOS WINAPI\\ESTRUCTURA DATOS WINAPI\\Reportes\\ReporteMedicos.csv", "REPORTE CREADO", MB_OK | MB_ICONINFORMATION);

					SendMessage(GetDlgItem(hwnd, IDC_COMBO2), CB_RESETCONTENT, 0, 0); //borramos combobox de modifcar medicos

					auxMedicos = ListaMedicos.getInicio();

					while (auxMedicos != nullptr) { //llena el combobox con nombres de medicos
						SendDlgItemMessage(hwnd, IDC_COMBO2, CB_INSERTSTRING, (WPARAM)0, (LPARAM)auxMedicos->getData().NombreCompleto);
						auxMedicos = auxMedicos->sig;
					}

					//borramos todo
					SetDlgItemText(hwnd, IDC_EDIT1, "");
					SetDlgItemText(hwnd, IDC_EDIT4, "");
					SetDlgItemText(hwnd, IDC_EDIT7, "");
					SetDlgItemText(hwnd, IDC_EDIT2, "");

					SendMessage(GetDlgItem(hwnd, IDC_ESPECIAL), CB_RESETCONTENT, 0, 0);
					ArbolEspecialidad->LlenarCombobox(GetDlgItem(hwnd, IDC_ESPECIAL), ArbolEspecialidad->GetRaiz());

					SendMessage(GetDlgItem(hwnd, IDC_CONSULTORIO), CB_RESETCONTENT, 0, 0);
					char numerodeconsultorios[4][3] = { "1","2","3","4" };
					for (int i = 0; i < 4; i++) {
						SendDlgItemMessage(hwnd, IDC_CONSULTORIO, CB_INSERTSTRING, (WPARAM)i, (LPARAM)numerodeconsultorios[i]);
					}

					SetDlgItemText(hwnd, IDC_EDIT12, "");

					SendDlgItemMessage(hwnd, FOTOMED, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)NULL);
					SendDlgItemMessage(hwnd, IDC_RADIO1, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);  //matutino
					SendDlgItemMessage(hwnd, IDC_RADIO2, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);  //vespertino
					SendDlgItemMessage(hwnd, IDC_LUNES, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
					SendDlgItemMessage(hwnd, IDC_MARTES, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
					SendDlgItemMessage(hwnd, IDC_MIERCOLES, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
					SendDlgItemMessage(hwnd, IDC_JUEVES, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
					SendDlgItemMessage(hwnd, IDC_VIERNES, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);

					SetDlgItemText(hwnd, IDC_CLAVE, sesionIniciada->getData().claveUsuario);

					EnableWindow(GetDlgItem(hwnd, IDOK), true); //activa guardar
					EnableWindow(GetDlgItem(hwnd, IDMODIFICAR), false); //desactiva modificar y eliminar
					EnableWindow(GetDlgItem(hwnd, IDBORRAR), false);
				}
				else {
					MessageBox(hwnd, "Para generar un reporte debe tener medicos registrados.", "ERROR", MB_OK | MB_ICONINFORMATION);
				}
				

			}break;
			}

		}break;

		}//loword

	}break;

	}//msg

	return 0;
}

BOOL CALLBACK PROCPACEINTES(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {

	case WM_CLOSE: {
		int opc = MessageBox(hwnd, "¿Seguro que desea salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);
		switch (opc) {
		case IDYES: {
			DestroyWindow(hwnd);
			PostQuitMessage(0); //Deja de ejecutar el programa
		}break;
		}
	}break; //CLOSE

	case WM_INITDIALOG: {

		SetDlgItemText(hwnd, IDC_USUARIO, sesionIniciada->getData().NombreCompleto);
		SetDlgItemText(hwnd, IDC_CLAVE, sesionIniciada->getData().claveUsuario);

		auxPacientes = ListaPacientes.getInicio();

		while (auxPacientes != nullptr) {
			SendDlgItemMessage(hwnd, IDC_COMBO2, CB_INSERTSTRING, (WPARAM)0, (LPARAM)auxPacientes->getData().NombreCompleto);

			auxPacientes = auxPacientes->sig;
		}


	}break; //Initdialog

	case WM_COMMAND: {

		menu(wParam, hwnd);

		switch (LOWORD(wParam)) {
		
		case IDC_GUARDAR: {//guardar

			Pacientes tempoPacientes;

			bool PacienteCorrecto = true;

			char NombreCompletoPaciente[80] = { 0 };

			GetDlgItemText(hwnd, IDC_CLAVE, tempoPacientes.claveUsuario,sizeof(tempoPacientes.claveUsuario));

			GetDlgItemText(hwnd, IDC_PATERNO,tempoPacientes.apellidoPaterno , sizeof(tempoPacientes.apellidoPaterno));
			GetDlgItemText(hwnd, IDC_MATERNO, tempoPacientes.apellidoMaterno, sizeof(tempoPacientes.apellidoMaterno));
			GetDlgItemText(hwnd, IDC_NOMBRE, tempoPacientes.nombres, sizeof(tempoPacientes.nombres));
			
			strcat_s(NombreCompletoPaciente, tempoPacientes.apellidoPaterno);
			strcat_s(NombreCompletoPaciente, " ");
			strcat_s(NombreCompletoPaciente, tempoPacientes.apellidoMaterno);
			strcat_s(NombreCompletoPaciente, " ");
			strcat_s(NombreCompletoPaciente, tempoPacientes.nombres);

			strcpy_s(tempoPacientes.NombreCompleto, NombreCompletoPaciente);

			if (IsDlgButtonChecked(hwnd, IDC_RADIO1) == BST_CHECKED) {
				tempoPacientes.genero = 1; //hombre
			}
			else if (IsDlgButtonChecked(hwnd, IDC_RADIO2) == BST_CHECKED) {
				tempoPacientes.genero = 2; // vespertino
			}

			GetDlgItemText(hwnd, IDC_TELEFONO, tempoPacientes.telefono, sizeof(tempoPacientes.telefono));
			GetDlgItemText(hwnd, IDC_REFE, tempoPacientes.referencia, sizeof(tempoPacientes.referencia));

			SYSTEMTIME nacimiento;
			ZeroMemory(&nacimiento, sizeof(nacimiento));
			SendDlgItemMessage(hwnd, IDC_FECHA, DTM_GETSYSTEMTIME, 0, (LPARAM)&nacimiento);

			tempoPacientes.fechanacimiento = nacimiento;

			tempoPacientes.diaNacimiento = nacimiento.wDay;
			tempoPacientes.mesNacimiento = nacimiento.wMonth;
			tempoPacientes.añoNacimiento = nacimiento.wYear;

			strcpy_s(tempoPacientes.NombreDoctor, "");

			if (validarApellido(tempoPacientes.apellidoPaterno) == false || validarApellido(tempoPacientes.apellidoMaterno) == false) {
				PacienteCorrecto = false;
				MessageBox(hwnd, "Los apellidos unicamente pueden contener letras y tener hasta 20 caracteres.", "ERROR", MB_OK | MB_ICONINFORMATION);
			}

			if (validarNombres(tempoPacientes.nombres) == false) {
				PacienteCorrecto = false;
				MessageBox(hwnd, "Los nombres unicamente pueden contener letras y tener hasta 40 caracteres.", "ERROR", MB_OK | MB_ICONINFORMATION);
			}

			if (tempoPacientes.genero != 1 && tempoPacientes.genero != 2) {
				PacienteCorrecto = false;
				MessageBox(hwnd, "Selecciona el genero del paciente.", "ERROR", MB_OK | MB_ICONINFORMATION);
			}

			if (validarTelefono(tempoPacientes.telefono) == false) {
				PacienteCorrecto = false;
				MessageBox(hwnd, "El numero de telefono debe tener al menos 10 caracteres y unicamente numeros.", "ERROR", MB_OK | MB_ICONINFORMATION);
			}

			if (strlen(tempoPacientes.referencia) == 0) {
				PacienteCorrecto = false;
				MessageBox(hwnd, "Ingrese una referencia de como llego a este hospital.", "ERROR", MB_OK | MB_ICONINFORMATION);
			}

			SYSTEMTIME fechahoy;
			ZeroMemory(&fechahoy, sizeof(fechahoy));
			GetLocalTime(&fechahoy);

			if (tempoPacientes.añoNacimiento <= fechahoy.wYear && (tempoPacientes.añoNacimiento < fechahoy.wYear||(tempoPacientes.añoNacimiento == fechahoy.wYear && tempoPacientes.mesNacimiento < fechahoy.wMonth)||(tempoPacientes.añoNacimiento == fechahoy.wYear && tempoPacientes.mesNacimiento == fechahoy.wMonth && tempoPacientes.diaNacimiento<=fechahoy.wDay))) {
																																											
				tempoPacientes.edad = fechahoy.wYear - tempoPacientes.añoNacimiento;

				if (fechahoy.wMonth < tempoPacientes.mesNacimiento || (fechahoy.wMonth == tempoPacientes.mesNacimiento && fechahoy.wDay < tempoPacientes.diaNacimiento)) {
					tempoPacientes.edad--;
				}
			}
			else {
				PacienteCorrecto = false;
				MessageBox(hwnd, "Ingrese una fecha válida.", "ERROR", MB_OK | MB_ICONINFORMATION);
			}


			if(PacienteCorrecto){

				SetDlgItemInt(hwnd, IDC_EDAD, tempoPacientes.edad, false);
				GetDlgItemText(hwnd, IDC_EDAD, tempoPacientes.edadTXT, sizeof(tempoPacientes.edadTXT));

				tempoPacientes.edad = GetDlgItemInt(hwnd, IDC_EDAD, NULL, false);

				auxPacientes = new Nodo<Pacientes>(tempoPacientes);

				ListaPacientes.agregarNodo(auxPacientes);

				MessageBox(hwnd, "Paciente registrado exitosamente", "AVISO", MB_OK | MB_ICONINFORMATION);

				SendMessage(GetDlgItem(hwnd, IDC_COMBO2), CB_RESETCONTENT, 0, 0);

				auxPacientes = ListaPacientes.getInicio();

				while (auxPacientes != nullptr) {
					SendDlgItemMessage(hwnd, IDC_COMBO2, CB_INSERTSTRING, (WPARAM)0, (LPARAM)auxPacientes->getData().NombreCompleto);
					auxPacientes = auxPacientes->sig;
				}

				SetDlgItemText(hwnd, IDC_PATERNO, "");
				SetDlgItemText(hwnd, IDC_MATERNO, "");
				SetDlgItemText(hwnd, IDC_NOMBRE, "");
				SendDlgItemMessage(hwnd, IDC_RADIO1, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
				SendDlgItemMessage(hwnd, IDC_RADIO2, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
				SetDlgItemText(hwnd, IDC_TELEFONO, "");
				SetDlgItemText(hwnd, IDC_REFE, "");
				SendDlgItemMessage(hwnd, IDC_FECHA, DTM_SETSYSTEMTIME, (WPARAM)GDT_VALID, (LPARAM)&fechahoy);
				SetDlgItemText(hwnd, IDC_EDAD, "");
			}

		}break;//gaurdar

		case IDC_COMBO2:{ //combo

			switch (HIWORD(wParam)) {
			case CBN_SELCHANGE: {

				EnableWindow(GetDlgItem(hwnd, IDC_GUARDAR), false); //desactiva guardar
				EnableWindow(GetDlgItem(hwnd, IDC_MODIFICAR), true); //activa modificar y eliminar
				EnableWindow(GetDlgItem(hwnd, IDC_ELIMINAR), true);

				char NombrePacienteBuscar[80];

				GetDlgItemText(hwnd, IDC_COMBO2, NombrePacienteBuscar, 80);

				auxPacientes = ListaPacientes.getInicio();

				while (auxPacientes != nullptr && strcmp(NombrePacienteBuscar, auxPacientes->getData().NombreCompleto) != 0) {

					auxPacientes = auxPacientes->sig;
				}

				SetDlgItemText(hwnd, IDC_PATERNO,auxPacientes->getData().apellidoPaterno);
				SetDlgItemText(hwnd, IDC_MATERNO,auxPacientes->getData().apellidoMaterno);
				SetDlgItemText(hwnd, IDC_NOMBRE, auxPacientes->getData().nombres);
				
				if (auxPacientes->getData().genero == 1) {
					SendDlgItemMessage(hwnd, IDC_RADIO1, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
					SendDlgItemMessage(hwnd, IDC_RADIO2, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);

				}
				else if(auxPacientes->getData().genero==2){
					SendDlgItemMessage(hwnd, IDC_RADIO2, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
					SendDlgItemMessage(hwnd, IDC_RADIO1, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);

				}

				SetDlgItemText(hwnd, IDC_TELEFONO, auxPacientes->getData().telefono);
				SetDlgItemText(hwnd, IDC_REFE, auxPacientes->getData().referencia);

				SYSTEMTIME fecha;
				ZeroMemory(&fecha, sizeof(fecha));

				fecha = auxPacientes->getData().fechanacimiento;

				SendDlgItemMessage(hwnd, IDC_FECHA, DTM_SETSYSTEMTIME, (WPARAM)GDT_VALID, (LPARAM)&fecha);

				SetDlgItemText(hwnd, IDC_EDAD, auxPacientes->getData().edadTXT);
				
				SetDlgItemText(hwnd, IDC_NOMMEDICO, auxPacientes->getData().NombreDoctor);

				SetDlgItemText(hwnd, IDC_CLAVE, auxPacientes->getData().claveUsuario);

			}break;
			}

		}break; //combo
		
		case IDC_ELIMINAR: { //

			int question = MessageBox(hwnd, "¿Desea eliminar este paciente?", "AVISO", MB_YESNO | MB_ICONQUESTION);
			switch (question) {
			case IDYES: {

				ListaPacientes.eliminarNodo(auxPacientes);

				MessageBox(hwnd, "Paciente eliminado exitosamente", "AVISO", MB_OK | MB_ICONINFORMATION);

				SendMessage(GetDlgItem(hwnd, IDC_COMBO2), CB_RESETCONTENT, 0, 0);

				auxPacientes = ListaPacientes.getInicio();

				while (auxPacientes != nullptr) {
					SendDlgItemMessage(hwnd, IDC_COMBO2, CB_INSERTSTRING, (WPARAM)0, (LPARAM)auxPacientes->getData().NombreCompleto);
					auxPacientes = auxPacientes->sig;
				}

				SYSTEMTIME fechahoy;
				ZeroMemory(&fechahoy, sizeof(fechahoy));
				GetLocalTime(&fechahoy);

				SetDlgItemText(hwnd, IDC_PATERNO, "");
				SetDlgItemText(hwnd, IDC_MATERNO, "");
				SetDlgItemText(hwnd, IDC_NOMBRE, "");
				SendDlgItemMessage(hwnd, IDC_RADIO1, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
				SendDlgItemMessage(hwnd, IDC_RADIO2, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
				SetDlgItemText(hwnd, IDC_TELEFONO, "");
				SetDlgItemText(hwnd, IDC_REFE, "");
				SendDlgItemMessage(hwnd, IDC_FECHA, DTM_SETSYSTEMTIME, (WPARAM)GDT_VALID, (LPARAM)&fechahoy);
				SetDlgItemText(hwnd, IDC_EDAD, "");

				EnableWindow(GetDlgItem(hwnd, IDC_GUARDAR), true); //desactiva guardar
				EnableWindow(GetDlgItem(hwnd, IDC_MODIFICAR), false); //activa modificar y eliminar
				EnableWindow(GetDlgItem(hwnd, IDC_ELIMINAR), false);


			}break;
			
			}

		}break; //eliminar

		case IDC_MODIFICAR: {

			int pregunta = MessageBox(hwnd, "¿Desea modificar este paciente?", "AVISO", MB_YESNO | MB_ICONQUESTION);
			switch (pregunta) {
			case IDYES: {

				Pacientes nuevomodif;

				bool PacienteCorrectoMod = true;
				char NombreCompletoPaciente[80] = { 0 };

				GetDlgItemText(hwnd, IDC_CLAVE, nuevomodif.claveUsuario, sizeof(nuevomodif.claveUsuario));

				GetDlgItemText(hwnd, IDC_PATERNO, nuevomodif.apellidoPaterno, sizeof(nuevomodif.apellidoPaterno));
				GetDlgItemText(hwnd, IDC_MATERNO, nuevomodif.apellidoMaterno, sizeof(nuevomodif.apellidoMaterno));
				GetDlgItemText(hwnd, IDC_NOMBRE, nuevomodif.nombres, sizeof(nuevomodif.nombres));

			
				strcat_s(NombreCompletoPaciente, nuevomodif.apellidoPaterno);
				strcat_s(NombreCompletoPaciente, " ");
				strcat_s(NombreCompletoPaciente, nuevomodif.apellidoMaterno);
				strcat_s(NombreCompletoPaciente, " ");
				strcat_s(NombreCompletoPaciente, nuevomodif.nombres);
				
				strcpy_s(nuevomodif.NombreCompleto, NombreCompletoPaciente);

				if (IsDlgButtonChecked(hwnd, IDC_RADIO1) == BST_CHECKED) {

					nuevomodif.genero = 1; //hombre
				}
				else if (IsDlgButtonChecked(hwnd, IDC_RADIO2) == BST_CHECKED) {

					nuevomodif.genero = 2; // vespertino
				}

				GetDlgItemText(hwnd, IDC_TELEFONO, nuevomodif.telefono, sizeof(nuevomodif.telefono));
				GetDlgItemText(hwnd, IDC_REFE, nuevomodif.referencia, sizeof(nuevomodif.referencia));

				SYSTEMTIME nacimiento{};
				SendDlgItemMessage(hwnd, IDC_FECHA, DTM_GETSYSTEMTIME, 0, (LPARAM)&nacimiento);

				nuevomodif.fechanacimiento = nacimiento;

				nuevomodif.diaNacimiento = nacimiento.wDay;
				nuevomodif.mesNacimiento = nacimiento.wMonth;
				nuevomodif.añoNacimiento = nacimiento.wYear;

				nuevomodif.edad = GetDlgItemInt(hwnd, IDC_EDAD, NULL, false);
				GetDlgItemText(hwnd, IDC_EDAD, nuevomodif.edadTXT, sizeof(nuevomodif.edadTXT));

				GetDlgItemText(hwnd, IDC_NOMMEDICO, nuevomodif.NombreDoctor, sizeof(nuevomodif.NombreDoctor));

				if (validarApellido(nuevomodif.apellidoPaterno) == false || validarApellido(nuevomodif.apellidoMaterno) == false) {
					PacienteCorrectoMod = false;
					MessageBox(hwnd, "Los apellidos unicamente pueden contener letras y tener hasta 20 caracteres.", "ERROR", MB_OK | MB_ICONINFORMATION);
				}

				if (validarNombres(nuevomodif.nombres) == false) {
					PacienteCorrectoMod = false;
					MessageBox(hwnd, "Los nombres unicamente pueden contener letras y tener hasta 40 caracteres.", "ERROR", MB_OK | MB_ICONINFORMATION);
				}

				if (nuevomodif.genero != 1 && nuevomodif.genero != 2) {
					PacienteCorrectoMod = false;
					MessageBox(hwnd, "Selecciona el genero del paciente.", "ERROR", MB_OK | MB_ICONINFORMATION);
				}

				if (validarTelefono(nuevomodif.telefono) == false) {
					PacienteCorrectoMod = false;
					MessageBox(hwnd, "El numero de telefono debe tener al menos 10 caracteres y unicamente numeros.", "ERROR", MB_OK | MB_ICONINFORMATION);
				}

				if (strlen(nuevomodif.referencia) == 0) {
					PacienteCorrectoMod = false;
					MessageBox(hwnd, "Ingrese una referencia de como llego a este hospital.", "ERROR", MB_OK | MB_ICONINFORMATION);
				}

				SYSTEMTIME fechahoy;
				ZeroMemory(&fechahoy, sizeof(fechahoy));
				GetLocalTime(&fechahoy);

				if (nuevomodif.añoNacimiento <= fechahoy.wYear && (nuevomodif.añoNacimiento < fechahoy.wYear || (nuevomodif.añoNacimiento == fechahoy.wYear && nuevomodif.mesNacimiento < fechahoy.wMonth) || (nuevomodif.añoNacimiento == fechahoy.wYear && nuevomodif.mesNacimiento == fechahoy.wMonth && nuevomodif.diaNacimiento <= fechahoy.wDay))) {

					nuevomodif.edad = fechahoy.wYear - nuevomodif.añoNacimiento;

					if (fechahoy.wMonth < nuevomodif.mesNacimiento || (fechahoy.wMonth == nuevomodif.mesNacimiento && fechahoy.wDay < nuevomodif.diaNacimiento)) {
						nuevomodif.edad--;
					}
				}
				else {
					PacienteCorrectoMod = false;
					MessageBox(hwnd, "Ingrese una fecha válida.", "ERROR", MB_OK | MB_ICONINFORMATION);
				}

				if(PacienteCorrectoMod){

					SetDlgItemInt(hwnd, IDC_EDAD, nuevomodif.edad, false);
					GetDlgItemText(hwnd, IDC_EDAD, nuevomodif.edadTXT, sizeof(nuevomodif.edadTXT));

					ListaPacientes.modificarNodo(auxPacientes, nuevomodif);

					MessageBox(hwnd, "Paciente modificado exitosamente", "AVISO", MB_OK | MB_ICONINFORMATION);

					SendMessage(GetDlgItem(hwnd, IDC_COMBO2), CB_RESETCONTENT, 0, 0);

					auxPacientes = ListaPacientes.getInicio();

					while (auxPacientes != nullptr) {
						SendDlgItemMessage(hwnd, IDC_COMBO2, CB_INSERTSTRING, (WPARAM)0, (LPARAM)auxPacientes->getData().NombreCompleto);
						auxPacientes = auxPacientes->sig;
					}

					SetDlgItemText(hwnd, IDC_PATERNO, "");
					SetDlgItemText(hwnd, IDC_MATERNO, "");
					SetDlgItemText(hwnd, IDC_NOMBRE, "");
					SendDlgItemMessage(hwnd, IDC_RADIO1, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
					SendDlgItemMessage(hwnd, IDC_RADIO2, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
					SetDlgItemText(hwnd, IDC_TELEFONO, "");
					SetDlgItemText(hwnd, IDC_REFE, "");
					SendDlgItemMessage(hwnd, IDC_FECHA, DTM_SETSYSTEMTIME, (WPARAM)GDT_VALID, (LPARAM)&fechahoy);
					SetDlgItemText(hwnd, IDC_EDAD, "");
					SetDlgItemText(hwnd, IDC_NOMMEDICO, "");

					EnableWindow(GetDlgItem(hwnd, IDC_GUARDAR), true); //activa guardar
					EnableWindow(GetDlgItem(hwnd, IDC_MODIFICAR), false); //desactiva modificar y eliminar
					EnableWindow(GetDlgItem(hwnd, IDC_ELIMINAR), false);
				}


			}break;

			}

		
		}break; //modificar
		
		case IDREPORTEE: {

			int report = MessageBox(hwnd, "¿Desea generar un reporte de pacientes?", "AVISO", MB_YESNO | MB_ICONQUESTION);
			switch (report) {
			case IDYES: {

				if (ListaPacientes.getInicio() != nullptr) {

					ListaPacientes.heapsort();

					auxPacientes = ListaPacientes.getInicio();

					ofstream ReportePacientes;

					ReportePacientes.open("C:\\Users\\jpfb0\\Desktop\\ESTRUCTURA DATOS WINAPI\\ESTRUCTURA DATOS WINAPI\\Reportes\\ReportePacientes.csv", ios::out | ios::trunc);

					if (ReportePacientes.is_open()) {

						ReportePacientes << "Usuario registro" << "," << "Nombre completo" << "," << "Genero" << "," << "Telefono" << "," << "Referencia" << "," << "Dia de nacimiento" << "," << "Mes de nacimiento" << "," << "Año de nacimiento" << "," << "Edad"<<"," << "Primer medico que lo atendio" << endl;

						while (auxPacientes != nullptr) {

							ReportePacientes << auxPacientes->getData().claveUsuario << "," << auxPacientes->getData().NombreCompleto << ",";

							if (auxPacientes->getData().genero == 1) {
								ReportePacientes << "Hombre" << ",";
							}
							else {
								ReportePacientes << "Mujer" << ",";
							}
							ReportePacientes << auxPacientes->getData().telefono << "," << auxPacientes->getData().referencia << "," << auxPacientes->getData().diaNacimiento << "," << auxPacientes->getData().mesNacimiento << "," << auxPacientes->getData().añoNacimiento << "," << auxPacientes->getData().edad << "," << auxPacientes->getData().NombreDoctor << endl;

							auxPacientes = auxPacientes->sig;

						}

					}

					MessageBox(hwnd, "Reporte creado en la siguiente direccion: C:\\Users\\jpfb0\\Desktop\\ESTRUCTURA DATOS WINAPI\\ESTRUCTURA DATOS WINAPI\\Reportes\\ReportePacientes.csv", "REPORTE CREADO", MB_OK | MB_ICONINFORMATION);

					SendMessage(GetDlgItem(hwnd, IDC_COMBO2), CB_RESETCONTENT, 0, 0);

					auxPacientes = ListaPacientes.getInicio();

					while (auxPacientes != nullptr) {
						SendDlgItemMessage(hwnd, IDC_COMBO2, CB_INSERTSTRING, (WPARAM)0, (LPARAM)auxPacientes->getData().NombreCompleto);
						auxPacientes = auxPacientes->sig;
					}

					SYSTEMTIME fechaactual;
					ZeroMemory(&fechaactual, sizeof(fechaactual));
					GetLocalTime(&fechaactual);

					SetDlgItemText(hwnd, IDC_PATERNO, "");
					SetDlgItemText(hwnd, IDC_MATERNO, "");
					SetDlgItemText(hwnd, IDC_NOMBRE, "");
					SendDlgItemMessage(hwnd, IDC_RADIO1, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
					SendDlgItemMessage(hwnd, IDC_RADIO2, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
					SetDlgItemText(hwnd, IDC_TELEFONO, "");
					SetDlgItemText(hwnd, IDC_REFE, "");
					SendDlgItemMessage(hwnd, IDC_FECHA, DTM_SETSYSTEMTIME, (WPARAM)GDT_VALID, (LPARAM)&fechaactual);
					SetDlgItemText(hwnd, IDC_EDAD, "");

					EnableWindow(GetDlgItem(hwnd, IDC_GUARDAR), true); 
					EnableWindow(GetDlgItem(hwnd, IDC_MODIFICAR), false); 
					EnableWindow(GetDlgItem(hwnd, IDC_ELIMINAR), false);

				}
				else {
					MessageBox(hwnd, "Para generar un reporte debe tener medicos registrados.", "ERROR", MB_OK | MB_ICONINFORMATION);
				}


			}break;
			}

		}break;

		}

	}break;

	}//msg

	return 0;
}

BOOL CALLBACK PROCUSUARIO(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {

	case WM_CLOSE: {
		int opc = MessageBox(hwnd, "¿Seguro que desea salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);
		switch (opc) {
		case IDYES: {
			DestroyWindow(hwnd);
			PostQuitMessage(0); //Deja de ejecutar el programa
		}break;
		}
	}break; //CLOSE

	case WM_INITDIALOG: {

		auxUsuarios = ListaUsuarios.getInicio(); 
		while (auxUsuarios != nullptr) { //llena el combobox con las claves de usuarios
			SendDlgItemMessage(hwnd, IDC_COMBO1, CB_INSERTSTRING, (WPARAM)0, (LPARAM)auxUsuarios->getData().claveUsuario);
			auxUsuarios = auxUsuarios->sig;
		}

	}break; //Initdialog

	case WM_COMMAND: {

		menu(wParam, hwnd);

		switch (LOWORD(wParam)) {

		case IDC_GUARDAR: { //BOTON GUARDAR

			Usuarios tempUsuario; 

			bool UsuarioCorrecto = true;

			char NombreCompletoUsu[80] = { 0 };

			GetDlgItemText(hwnd, IDC_EDIT1, tempUsuario.claveUsuario, sizeof(tempUsuario.claveUsuario));
			GetDlgItemText(hwnd, IDC_PATERNO, tempUsuario.apellidoPaterno, sizeof(tempUsuario.apellidoPaterno));
			GetDlgItemText(hwnd, IDC_MATERNO, tempUsuario.apellidoMaterno, sizeof(tempUsuario.apellidoMaterno));
			GetDlgItemText(hwnd, IDC_NOMBRE, tempUsuario.nombres, sizeof(tempUsuario.nombres));
			GetDlgItemText(hwnd, IDC_EDIT3, tempUsuario.password, sizeof(tempUsuario.password));

			if (validarUsuario(tempUsuario.claveUsuario) == false) {
				UsuarioCorrecto = false;
				MessageBox(hwnd, "La clave de usuario debe contener de 5 a 20 caracteres, y unicamente caracteres alfanumericos", "ERROR", MB_OK | MB_ICONINFORMATION);

			}

			if (validarApellido(tempUsuario.apellidoPaterno)==false || validarApellido(tempUsuario.apellidoMaterno) == false) {
				UsuarioCorrecto = false;
				MessageBox(hwnd, "Los apellidos unicamente pueden contener letras y tener hasta 20 caracteres", "ERROR", MB_OK | MB_ICONINFORMATION);
			}


			if (validarNombres(tempUsuario.nombres) == false) {
				UsuarioCorrecto = false;
				MessageBox(hwnd, "Los nombres unicamente pueden contener letras y tener hasta 40 caracteres", "ERROR", MB_OK | MB_ICONINFORMATION);
			}

			if (strlen(tempUsuario.password) == 0) {
				UsuarioCorrecto = false;
				MessageBox(hwnd, "Ingrese una contraseña por favor", "ERROR", MB_OK | MB_ICONINFORMATION);
			}

			strcat_s(NombreCompletoUsu, tempUsuario.nombres);
			strcat_s(NombreCompletoUsu, " ");
			strcat_s(NombreCompletoUsu, tempUsuario.apellidoPaterno);
			strcat_s(NombreCompletoUsu, " ");
			strcat_s(NombreCompletoUsu, tempUsuario.apellidoMaterno);
			
			strcpy_s(tempUsuario.NombreCompleto, NombreCompletoUsu);

			if(UsuarioCorrecto==true){
				//en caso de que cumpla validaciones
				auxUsuarios = new Nodo<Usuarios>(tempUsuario); //crea el nodo con el usuario

				ListaUsuarios.agregarNodo(auxUsuarios); //mete el nodo a la lista ligada

				MessageBox(hwnd, "Usuario registrado exitosamente", "AVISO", MB_OK | MB_ICONINFORMATION);

				SendMessage(GetDlgItem(hwnd, IDC_COMBO1), CB_RESETCONTENT, 0, 0); //vacia el combobox

				SetDlgItemText(hwnd, IDC_EDIT1, ""); //borramos todo
				SetDlgItemText(hwnd, IDC_PATERNO, "");
				SetDlgItemText(hwnd, IDC_MATERNO, "");
				SetDlgItemText(hwnd, IDC_NOMBRE, "");
				SetDlgItemText(hwnd, IDC_EDIT3, "");

				auxUsuarios = ListaUsuarios.getInicio();

				if (sesionIniciada == nullptr) {
					sesionIniciada = auxUsuarios;
					MessageBox(hwnd, "Se ha iniciado sesion con el usuario registrado", "AVISO", MB_OK | MB_ICONINFORMATION);
				}

				while (auxUsuarios != nullptr) { //actualizamos combobox con la lista nueva
					SendDlgItemMessage(hwnd, IDC_COMBO1, CB_INSERTSTRING, (WPARAM)0, (LPARAM)auxUsuarios->getData().claveUsuario);
					auxUsuarios = auxUsuarios->sig;
				}
			}

		}break; //GUARDAR

		case IDC_COMBO1: { //combobox

			switch (HIWORD(wParam)) { 

				case CBN_SELCHANGE: { //si seleccionamos combo

					EnableWindow(GetDlgItem(hwnd, IDC_GUARDAR), false); //desactiva guardar
					EnableWindow(GetDlgItem(hwnd, IDC_BUTTON2), true); //activa modificar y eliminar
					EnableWindow(GetDlgItem(hwnd, IDC_BUTTON3), true);

					char UsuarioClave[20] = { 0 }; //para guardar la clave de usuario del combobox

					GetDlgItemText(hwnd, IDC_COMBO1, UsuarioClave, 20); //guarda la clave

					auxUsuarios = ListaUsuarios.getInicio(); //aux al principio de la lista

					while (auxUsuarios->sig != nullptr && strcmp(UsuarioClave, auxUsuarios->getData().claveUsuario) != 0) {
						auxUsuarios=auxUsuarios->sig; //busca la clave y la encuentra para estar en ese nodo
					}

					SetDlgItemText(hwnd, IDC_EDIT1, auxUsuarios->getData().claveUsuario); //llenamos todos los datos
					SetDlgItemText(hwnd, IDC_PATERNO, auxUsuarios->getData().apellidoPaterno);
					SetDlgItemText(hwnd, IDC_MATERNO, auxUsuarios->getData().apellidoMaterno);
					SetDlgItemText(hwnd, IDC_NOMBRE, auxUsuarios->getData().nombres);
					SetDlgItemText(hwnd, IDC_EDIT3, auxUsuarios->getData().password);


				}break;

			}//hiword

		}break; //combo

		case IDC_BUTTON3: { //BOTON ELIMINAR

			int question = MessageBox(hwnd, "¿Desea eliminar este usuario?", "AVISO", MB_YESNO | MB_ICONQUESTION);
			switch (question) {
			case IDYES: {

				ListaUsuarios.eliminarNodo(auxUsuarios); //elimina el nodo 

				MessageBox(hwnd, "Usuario eliminado exitosamente", "AVISO", MB_OK | MB_ICONINFORMATION);

				SendMessage(GetDlgItem(hwnd, IDC_COMBO1), CB_RESETCONTENT, 0, 0); //vacia el combobox

				auxUsuarios = ListaUsuarios.getInicio();

				while (auxUsuarios != nullptr) { //actualizamos combobox con la lista nueva
					SendDlgItemMessage(hwnd, IDC_COMBO1, CB_INSERTSTRING, (WPARAM)0, (LPARAM)auxUsuarios->getData().claveUsuario);
					auxUsuarios = auxUsuarios->sig;
				}

				SetDlgItemText(hwnd, IDC_EDIT1, ""); //borramos todo
				SetDlgItemText(hwnd, IDC_PATERNO, "");
				SetDlgItemText(hwnd, IDC_MATERNO, "");
				SetDlgItemText(hwnd, IDC_NOMBRE, "");
				SetDlgItemText(hwnd, IDC_EDIT3, "");

				EnableWindow(GetDlgItem(hwnd, IDC_GUARDAR), true); //activa guardar
				EnableWindow(GetDlgItem(hwnd, IDC_BUTTON2), false); //desactiva modificar y eliminar
				EnableWindow(GetDlgItem(hwnd, IDC_BUTTON3), false);

			}break;

			}

		}break;//eliminar

		case IDC_BUTTON2: { //boton MODIFICAR

			int opc = MessageBox(hwnd, "¿Desea modificar este usuario?", "AVISO", MB_YESNO | MB_ICONQUESTION);
			switch (opc) {
			case IDYES: {

				Usuarios temporal;
				bool usuariocorrectomod = true;
				char NombreCompletoUsu[80] = { 0 };

				GetDlgItemText(hwnd, IDC_EDIT1, temporal.claveUsuario, sizeof(temporal.claveUsuario));//clave de usuario
				GetDlgItemText(hwnd, IDC_PATERNO, temporal.apellidoPaterno, sizeof(temporal.apellidoPaterno));
				GetDlgItemText(hwnd, IDC_MATERNO, temporal.apellidoMaterno, sizeof(temporal.apellidoMaterno));
				GetDlgItemText(hwnd, IDC_NOMBRE, temporal.nombres, sizeof(temporal.nombres));
				GetDlgItemText(hwnd, IDC_EDIT3, temporal.password, sizeof(temporal.password));

				strcat_s(NombreCompletoUsu, temporal.nombres);
				strcat_s(NombreCompletoUsu, " ");
				strcat_s(NombreCompletoUsu, temporal.apellidoPaterno);
				strcat_s(NombreCompletoUsu, " ");
				strcat_s(NombreCompletoUsu, temporal.apellidoMaterno);

				strcpy_s(temporal.NombreCompleto, NombreCompletoUsu);

				if (validarUsuario(temporal.claveUsuario) == false) {
					usuariocorrectomod = false;
					MessageBox(hwnd, "La clave de usuario debe contener de 5 a 20 caracteres, y unicamente caracteres alfanumericos", "ERROR", MB_OK | MB_ICONINFORMATION);

				}

				if (validarApellido(temporal.apellidoPaterno) == false || validarApellido(temporal.apellidoMaterno) == false) {
					usuariocorrectomod = false;
					MessageBox(hwnd, "Los apellidos unicamente pueden contener letras y tener hasta 20 caracteres", "ERROR", MB_OK | MB_ICONINFORMATION);
				}


				if (validarNombres(temporal.nombres) == false) {
					usuariocorrectomod = false;
					MessageBox(hwnd, "Los nombres unicamente pueden contener letras y tener hasta 40 caracteres", "ERROR", MB_OK | MB_ICONINFORMATION);
				}

				if (strlen(temporal.password) == 0) {
					usuariocorrectomod = false;
					MessageBox(hwnd, "Ingrese una contraseña por favor", "ERROR", MB_OK | MB_ICONINFORMATION);
				}

				if(usuariocorrectomod){

					ListaUsuarios.modificarNodo(auxUsuarios, temporal);

					MessageBox(hwnd, "Usuario modificado exitosamente", "AVISO", MB_OK | MB_ICONINFORMATION);

					SendMessage(GetDlgItem(hwnd, IDC_COMBO1), CB_RESETCONTENT, 0, 0); //vacia el combobox

					auxUsuarios = ListaUsuarios.getInicio();

					while (auxUsuarios != nullptr) { //actualizamos combobox con la lista nueva
						SendDlgItemMessage(hwnd, IDC_COMBO1, CB_INSERTSTRING, (WPARAM)0, (LPARAM)auxUsuarios->getData().claveUsuario);
						auxUsuarios = auxUsuarios->sig;
					}

					SetDlgItemText(hwnd, IDC_EDIT1, ""); //borramos todo
					SetDlgItemText(hwnd, IDC_PATERNO, "");
					SetDlgItemText(hwnd, IDC_MATERNO, "");
					SetDlgItemText(hwnd, IDC_NOMBRE, "");
					SetDlgItemText(hwnd, IDC_EDIT3, "");

					EnableWindow(GetDlgItem(hwnd, IDC_GUARDAR), true); //activa guardar
					EnableWindow(GetDlgItem(hwnd, IDC_BUTTON2), false); //desactiva modificar y eliminar
					EnableWindow(GetDlgItem(hwnd, IDC_BUTTON3), false);
				}

			}break;

			}//op

		}break; //modificar

		}//switchLOWORD

	}break;//wm_command

	}//msg

	return 0;
}

//VALIDACIONES
bool validarNombres(char nombre[42]) {

	bool largo = false;
	bool letra = false;
	if (strlen(nombre) >= 1 && strlen(nombre) <= 40) {
		largo = true;
	}

	for (int i = 0; i < strlen(nombre); i++) {
		if (isalpha(nombre[i])) {
			letra = true;
		}
	}

	for (int i = 0; i < strlen(nombre); i++) {
		if (isdigit(nombre[i])) {
			return false;
		}
	}

	for (int i = 0; i < strlen(nombre); i++) {
		if (ispunct(nombre[i])) {
			return false;
		}
	}

	if (largo && letra) {
		return true;
	}
	else {
		return false;
	}
}
bool validarApellido(char nombre[22]) {

	bool largo = false;
	bool letra = false;
	if (strlen(nombre) >= 1 && strlen(nombre) <= 20) {
		largo = true;
	}

	for (int i = 0; i < strlen(nombre); i++) {
		if (isalpha(nombre[i])) {
			letra = true;
		}
	}

	for (int i = 0; i < strlen(nombre); i++) {
		if (isdigit(nombre[i])) {
			return false;
		}
	}

	for (int i = 0; i < strlen(nombre); i++) {
		if (ispunct(nombre[i])) {
			return false;
		}
	}

	if (largo && letra) {
		return true;
	}
	else {
		return false;
	}
}
bool validarUsuario(char usuario[22]) {

	bool largo = false;

	if (strlen(usuario) >= 5 && strlen(usuario) <= 20) {
		largo = true;
	}

	for (int i = 0; i < strlen(usuario); i++) {
		if (ispunct(usuario[i])) {
			return false;
		}
	}

	if (largo) {
		return true;
	}
	else {

		return false;
	}
}
bool validarTelefono(char telefono[12]) {

	bool largo = false;
	bool numero = false;

	if (strlen(telefono) == 10) {
		largo = true;
	}

	for (int i = 0; i < strlen(telefono); i++) {
		if (ispunct(telefono[i])) {
			return false;
		}
	}

	for (int i = 0; i < strlen(telefono); i++) {

		if (isalpha(telefono[i])) {
			return false;
		}
	}

	for (int i = 0; i < strlen(telefono); i++) {
		if (isdigit(telefono[i])) {
			numero = true;
		}
	}

	if (largo && numero) {
		return true;
	}
	else {

		return false;
	}
}
bool validarEspecialidad(char espe[32]) {
	bool largo = false;
	bool letra = false;
	if (strlen(espe) >= 1 && strlen(espe) <= 30) {
		largo = true;
	}

	for (int i = 0; i < strlen(espe); i++) {
		if (isalpha(espe[i])) {
			letra = true;
		}
	}

	for (int i = 0; i < strlen(espe); i++) {
		if (isdigit(espe[i])) {
			return false;
		}
	}

	for (int i = 0; i < strlen(espe); i++) {
		if (ispunct(espe[i])) {
			return false;
		}
	}

	if (largo && letra) {
		return true;
	}
	else {
		return false;
	}

}
bool validarNumero(char numero[10]) {

	int lenght = strlen(numero);

	bool numeros = false;
	bool largo = false;

	if (lenght > 0) {
		largo = true;
	}

	for (int i = 0; i < lenght; i++) {

		if (isalpha(numero[i])) {
			return false;
		}

		if (ispunct(numero[i])) {
			return false;
		}

		if (isdigit(numero[i])) {
			numeros = true;
		}

	}

	if (numeros && largo) {
		return true;
	}
	else {
		return false;

	}
}

void UsuariosTxt() {

	ofstream usuarios;

	usuarios.open("Archivos\\UsuariosTXT.txt",ios::out|ios::trunc);

	if (usuarios.is_open()) {

		usuarios << "LISTA DE USUARIOS" << endl << endl;

		auxUsuarios = ListaUsuarios.getInicio();

		while (auxUsuarios != nullptr) {

			usuarios << "Clave de usuario: " << auxUsuarios->getData().claveUsuario << endl;
			usuarios << "Nombre completo:  " << auxUsuarios->getData().NombreCompleto << endl;
			usuarios << "Contraseña: " << auxUsuarios->getData().password << endl << endl;

			auxUsuarios = auxUsuarios->sig;
		}

		usuarios.close();

	}

};

//MENU
void menu(WPARAM wParam, HWND hwnd) {

	switch (LOWORD(wParam)) {

	case ID_MENUPRINCIPAL: { //1

		EndDialog(hwnd, 0);
		DialogBox(hInstGlobal, MAKEINTRESOURCE(PRINCIPAL), hwnd, MAINMENU);
		
	}break;


	case ID_MEDICOS: { //2

		EndDialog(hwnd, 0);
		DialogBox(hInstGlobal, MAKEINTRESOURCE(MEDICOS), hwnd, PROCMEDICOS);

	}break;


	case ID_PACIENTES: { //3

		EndDialog(hwnd, 0);
		DialogBox(hInstGlobal, MAKEINTRESOURCE(PACIENTE), hwnd, PROCPACEINTES);

	}break;

	
	case ID_CITAS: { //4

		EndDialog(hwnd, 0);
		DialogBox(hInstGlobal, MAKEINTRESOURCE(CITAS), hwnd, PROCCITAS);

	}break;


	case ID_CONSULTARCITAS_CITASENGENERAL:{ //5

		EndDialog(hwnd, 0);
		DialogBox(hInstGlobal, MAKEINTRESOURCE(CITAS_TODAS), hwnd, CITASTODAS);

	}break;


	case ID_CONSULTARCITAS_PORMEDICO: {//6

		EndDialog(hwnd, 0);
		DialogBox(hInstGlobal, MAKEINTRESOURCE(CITAS_MEDICO), hwnd, CITASXMEDICO);

	}break;


	case ID_CONSULTARCITAS_PORSEMANA: { //7

		EndDialog(hwnd, 0);
		DialogBox(hInstGlobal, MAKEINTRESOURCE(CITAS_SEMANA), hwnd, CITASXSEMANA);

	}break;


	case ID_CONSULTARCITAS_PORESPECIALIDAD: {//8

		EndDialog(hwnd, 0);
		DialogBox(hInstGlobal, MAKEINTRESOURCE(CITAS_ESPECIALIDAD), hwnd, CITASXESPECIALIDAD);

	}break;


	case ID_USUARIOS: { //9

		EndDialog(hwnd, 0);
		DialogBox(hInstGlobal, MAKEINTRESOURCE(REG_USUARIO), hwnd, PROCUSUARIO);

	}break;

	case ID_ESPECIALIDADES: { //10

		EndDialog(hwnd, 0);
		DialogBox(hInstGlobal, MAKEINTRESOURCE(ESPECIALIDADES), hwnd, PROCESPECIALIDADES);

	}break;

	
	}//SWITCH

}//FUNCION MENU