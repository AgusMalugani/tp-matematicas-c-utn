#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <ctype.h>

#define FILAS 5                // CANTIDAD FILAS MATRIZ
#define COLUMNAS 5             // CANTIDAD COLUMNAS MATRIZ
#define FILAS_PRIORITARIAS 2   // CANTIDAD FILAS PRIORITARIAS
#define FILA_INICIO_GENERAL 2  // INICIO FILAS GENERALES MATRIZ
#define CANT_FILAS_GENERALES 3 // FIN FILAS GENERALES


// Estructura que define cada usuario del sistema
typedef struct
{
    int dni;
    char nombre[30];
    char apellido[30];
    char genero;
    int jubilado;      // J: subconjunto de jubilados
    int embarazada;    // E: subconjunto de embarazadas
    int discapacitado; // D: subconjunto de discapacitados
    int filaLugar;
    int columnaLugar;
    int tieneVehiculo; // 1 si tiene vehiculo, 0 si no tiene
} Usuario;

void matrizEstacionamientoTest(int matriz[FILAS][COLUMNAS]);
int lugaresGeneralesLibres(int matriz[FILAS][COLUMNAS]);
int lugaresPrioritariosLibres(int matriz[FILAS][COLUMNAS]);
Usuario usuarioSinSesion();
int sesionActiva(Usuario user);
Usuario registrarClienteNuevo();
Usuario logueo(Usuario clientes[], int cantidadUsuarios);
void matrizEstacionamiento(int matriz[FILAS][COLUMNAS]);
Usuario asignarLugar(Usuario user, int matriz[FILAS][COLUMNAS]);
Usuario retirarVehiculo(Usuario user, int matriz[FILAS][COLUMNAS]);
int lugaresGeneralesLibres(int matriz[FILAS][COLUMNAS]);
int lugaresPrioritariosLibres(int matriz[FILAS][COLUMNAS]);
void mostrarMatriz(int matriz[FILAS][COLUMNAS]);
void actualizarListaClientes(Usuario clientes[], int cantidadUsuarios, Usuario user);

// validaciones
int esNumero(char cadena[]);
int pedirBooleano(char mensaje[]);
int validarDniCreacion(int dni, Usuario clientes[], int cantidadUsuarios);
int validacionString(char string[30]);
int validacionGenero(char nuevoGenero);
int validacionConjuntoP(int nuevoJubilado);
int validarDni(int dni);

int main()
{
    int opciones;
    Usuario user = usuarioSinSesion();
    int cantidadUsuarios = 0;
    int matriz[FILAS][COLUMNAS];
    int modoTestEstacionamiento = 0;
    Usuario clientes[100];
    srand((unsigned int)time(NULL));

    if (modoTestEstacionamiento == 1) {
        matrizEstacionamientoTest(matriz);
        printf("Modo TEST: generales ocupados, prioritarios libres.\n");
    } else {
        matrizEstacionamiento(matriz);
    }
    mostrarMatriz(matriz);
    do
    {
        printf("----------------------------------------\n");
        printf("1: Registrar Cliente Nuevo\n");
        printf("2: Login\n");
        printf("3: Ingresar Vehiculo\n");
        printf("4: Retirar Vehiculo\n");
        printf("5: Limpiar pantalla \n");
        printf("6: Mostrar estacionamiento \n");
        printf("7: Salir\n");
        printf("----------------------------------------\n");
        scanf("%d", &opciones);
        switch (opciones)
        {
        case 1:
        {
            Usuario newUser = registrarClienteNuevo(clientes, cantidadUsuarios);
            clientes[cantidadUsuarios] = newUser;
            cantidadUsuarios++;
            break;
        }
        case 2:
        {
            user = logueo(clientes, cantidadUsuarios);
            break;
        }
        case 3:
        {
            printf("\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
            int teniaVehiculo = user.tieneVehiculo;
            if (sesionActiva(user) && teniaVehiculo == 1)
            {

                mostrarMatriz(matriz);
                printf("\n El usuario ya tiene un vehiculo ingresado en la posicion:\n");
                printf("Fila: %d\n", user.filaLugar + 1);
                printf("Columna: %d\n", user.columnaLugar + 1);
            }
            else
            {
                user = asignarLugar(user, matriz);

                if (sesionActiva(user) && user.tieneVehiculo == 1 && teniaVehiculo == 0)
                {
                    mostrarMatriz(matriz);
                    actualizarListaClientes(clientes, cantidadUsuarios, user);
                    printf("Vehiculo ingresado correctamente\n");
                    printf("Fila: %d\n", user.filaLugar + 1);
                    printf("Columna: %d\n", user.columnaLugar + 1);
                }
            }
            printf("\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
            break;
        }
        case 4:
        {
            printf("\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
            int teniaVehiculo = user.tieneVehiculo;
            user = retirarVehiculo(user, matriz);
            if (sesionActiva(user) && teniaVehiculo == 1 && user.tieneVehiculo == 0)
            {
                mostrarMatriz(matriz);
                actualizarListaClientes(clientes, cantidadUsuarios, user);
                printf("Vehiculo retirado correctamente\n");
            }
            printf("\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
            break;
        }
        case 5:
        {
            system("cls");
            printf("CONSOLA LIMPIA\n");
            break;
        }
        case 6:
        {
            printf("\n \n");
            mostrarMatriz(matriz);
            printf("\n \n");
            break;
        }
        case 7:
            printf("Saliendo del programa\n");
            break;
        default:
            printf("Opcion invalida. Intente nuevamente.\n");
        };

    } while (opciones != 7);

    return 0;
}

Usuario usuarioSinSesion()
{
    Usuario user = {0};
    user.dni = -1;
    user.filaLugar = -1;
    user.columnaLugar = -1;
    user.tieneVehiculo = 0;
    return user;
};

int sesionActiva(Usuario user)
{
    return user.dni > 0;
}

int esNumero(char cadena[])
{
    for (int i = 0; cadena[i] != '\0'; i++)
    {
        if (!isdigit(cadena[i]))
        {
            return 0;
        }
    }

    return 1;
}

int validarDniCreacion(int dni, Usuario clientes[], int cantidadUsuarios)
{
    int bandera = 0;
    if (dni < 1000000 || dni > 60000000)
    {
        printf("[ERROR], el dni debe tener entre 7 y 8 digitos\n");
    }
    else
    {
        if (cantidadUsuarios != 0)
        {
            for (int i = 0; i < cantidadUsuarios; i++)
            {
                if (dni == clientes[i].dni)
                {
                    printf("[ERROR] Ya existe un cliente con ese dni.");
                    return bandera;
                };
            }
        };
        bandera = 1;
    };

    return bandera;
};

int validacionString(char string[30])
{

    int bandera = 0;
    int lenghtString = strlen(string);

    if (lenghtString > 2 && lenghtString < 30)
    {
        bandera = 1;
    }
    else
    {
        printf("Debe ingresar almenos 2 caracteres y menos de 30 caracteres \n");
    };

    return bandera;
};

int validacionGenero(char nuevoGenero)
{
    int bandera = 0;
    int upper = toupper(nuevoGenero);
    if (upper != 'M' && upper != 'F')
    {
        printf("El genero debe ser M para masculino o F para feminino \n");
    }
    else
    {
        bandera = 1;
    }
    return bandera;
}

int validacionConjuntoP(int nuevoJubilado)
{
    int bandera = 0;
    if (nuevoJubilado < 0 || nuevoJubilado > 1)
    {
        printf(" La opcion elegida no es valida, debe ser 1 para Si o 2 para No\n");
    }
    else
    {
        bandera = 1;
    }
    return bandera;
};

int validarDni(int dni)
{
    int bandera = 0;
    if (dni < 1000000 || dni > 60000000)
    {
        printf("[ERROR], el dni debe tener entre 7 y 8 digitos\n");
    }
    else
    {
        bandera = 1;
    };
    return bandera;
};

Usuario registrarClienteNuevo(Usuario clientes[], int cantidadUsuarios)
{
    Usuario newUser;
    int nuevoDni, nuevoJubilado, nuevoEmbarazada, nuevoDiscapacitado;
    char nuevoNombre[30], nuevoApellido[30], nuevoGenero;

    printf("Registrar cliente \n");

    char dniStr[20];

    do
    {
        printf("DNI: ");
        scanf("%19s", dniStr);

        if (!esNumero(dniStr))
        {
            printf("[ERROR] El DNI debe contener solo numeros.\n");
            continue;
        }

        nuevoDni = atoi(dniStr);

    } while (validarDniCreacion(nuevoDni, clientes, cantidadUsuarios) == 0);

    do
    {
        printf("Nombre: ");
        scanf("%s", nuevoNombre);
    } while (validacionString(nuevoNombre) == 0);

    do
    {
        printf("Apellido :");
        scanf("%s", nuevoApellido);
    } while (validacionString(nuevoApellido) == 0);

    do
    {
        printf("Genero (M/F)");
        scanf(" %c", &nuevoGenero);
    } while (validacionGenero(nuevoGenero) == 0);

    nuevoJubilado = pedirBooleano("Es Jubilado? (1=SI / 0=NO): ");
    nuevoDiscapacitado = pedirBooleano("Posee alguna Discapacidad? (1=SI / 0=NO): ");

    if (nuevoGenero == 'F' || nuevoGenero == 'f')
    {
        nuevoEmbarazada = pedirBooleano("Esta embarazada? (1=SI / 0=NO): ");
    }
    else
    {
        nuevoEmbarazada = 0;
    }

    newUser.dni = nuevoDni;
    strcpy(newUser.nombre, nuevoNombre);
    strcpy(newUser.apellido, nuevoApellido);
    newUser.genero = nuevoGenero;
    newUser.jubilado = nuevoJubilado;
    newUser.embarazada = nuevoEmbarazada;
    newUser.discapacitado = nuevoDiscapacitado;

    newUser.filaLugar = -1;
    newUser.columnaLugar = -1;
    newUser.tieneVehiculo = 0;

    printf(" \ncliente registrado correctamente\n");

    return newUser;
};

int pedirBooleano(char mensaje[])
{
    char opcion[10];
    int valor;

    do
    {
        printf("%s", mensaje);
        scanf("%9s", opcion);

        if(!esNumero(opcion))
        {
            printf("[ERROR] Debe ingresar 0 o 1\n");
            continue;
        }

        valor = atoi(opcion);

        if(validacionConjuntoP(valor) == 0)
        {
            printf("[ERROR] Debe ingresar 0 o 1\n");
        }

    } while(!esNumero(opcion) || validacionConjuntoP(valor) == 0);

    return valor;
}

Usuario logueo(Usuario clientes[], int cantidadUsuarios)
{
    int dni;
    Usuario user = usuarioSinSesion();

    char dniStr[20];

    do
    {
        printf(" Ingrese DNI: ");
        scanf("%19s", dniStr);

        if (!esNumero(dniStr))
        {
            printf("[ERROR] El DNI debe contener solo numeros.\n");
            continue;
        }

        dni = atoi(dniStr);

    } while (validarDni(dni) == 0);

    for (int i = 0; i < cantidadUsuarios; i++)
    {
        if (clientes[i].dni == dni)
        {
            user = clientes[i];
            printf(" Login correcto \n");
            printf(" Bienvenido , %s \n ", user.nombre);
            return user;
        }
    }

    printf("Usuario no encontrado \n");
    return usuarioSinSesion();
};

void mostrarMatriz(int matriz[FILAS][COLUMNAS])
{
    for (int i = 0; i < FILAS; i++)
    {
        for (int j = 0; j < COLUMNAS; j++)
        {
            printf("[ %d ]", matriz[i][j]);
        }
        printf("\n");
    }
};

// Inicialización: rellena la matriz con ceros (todas las cocheras libres)
void matrizEstacionamiento(int matriz[FILAS][COLUMNAS])
{
    for (int i = 0; i < FILAS; i++)
    {
        for (int j = 0; j < COLUMNAS; j++)
        {
            // Mᵢⱼ = 0 → Estado libre (cochera disponible)
            matriz[i][j] = 0;
        }
    }
};

Usuario asignarLugar(Usuario user, int matriz[FILAS][COLUMNAS])
{
    int fila;
    int columna;
    int generalesLibres;
    int prioritariosLibres;

    if (!sesionActiva(user))
    {
        printf("Debe iniciar sesion primero.\n");
        return user;
    }

    if (user.tieneVehiculo == 1)
    {
        printf("Ya tiene un vehiculo estacionado en fila %d, columna %d\n",
               user.filaLugar + 1, user.columnaLugar + 1);
        return user;
    }

    generalesLibres = lugaresGeneralesLibres(matriz);
    prioritariosLibres = lugaresPrioritariosLibres(matriz);

    if (user.jubilado == 0 && user.embarazada == 0 && user.discapacitado == 0)
    {
        if (generalesLibres > 0)
        {
            do
            {
                fila = rand() % CANT_FILAS_GENERALES + FILA_INICIO_GENERAL;
                columna = rand() % COLUMNAS;
            } while (matriz[fila][columna] == 1);
            printf("\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
            printf("\nLugar NORMAL asignado\n");
            printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
        }
        else if (prioritariosLibres > 2)
        {
            do
            {
                fila = rand() % FILAS_PRIORITARIAS;
                columna = rand() % COLUMNAS;
            } while (matriz[fila][columna] == 1);
            printf("\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
            printf("\nLugar PRIORITARIO asignado (sin generales libres)\n");
            printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
        }
        else
        {
            printf("\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
            printf("No hay lugares disponibles.\n");
            printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
            return user;
        }
    }
    else
    {
        if (prioritariosLibres > 0)
        {
            do
            {
                fila = rand() % FILAS_PRIORITARIAS;
                columna = rand() % COLUMNAS;
            } while (matriz[fila][columna] == 1);
            printf("\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
            printf("\nLugar PRIORITARIO asignado\n");
            printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
        }
        else if (generalesLibres > 0)
        {
            do
            {
                fila = rand() % CANT_FILAS_GENERALES + FILA_INICIO_GENERAL;
                columna = rand() % COLUMNAS;
            } while (matriz[fila][columna] == 1);
            printf("\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
            printf("\nLugar GENERAL asignado (sin prioritarios libres)\n");
            printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
        }
        else
        {
            printf("\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
            printf("No hay lugares disponibles.\n");
            printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
            return user;
        }
    }

    matriz[fila][columna] = 1;
    user.filaLugar = fila;
    user.columnaLugar = columna;
    user.tieneVehiculo = 1;
    printf("\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
    printf("Fila: %d\n", fila + 1);
    printf("Columna: %d\n", columna + 1);
    printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
    return user;
};

Usuario retirarVehiculo(Usuario user, int matriz[FILAS][COLUMNAS])
{

    if (!sesionActiva(user))
    {
        printf("Debe iniciar sesion primero.\n");
        return user;
    }

    if (user.tieneVehiculo == 0)
    {
        printf("No tiene un vehiculo estacionado\n");
        return user;
    }
    else
    {
        printf("\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
        printf("Lugar liberado: fila %d, columna %d\n", user.filaLugar + 1, user.columnaLugar + 1);
        printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
        matriz[user.filaLugar][user.columnaLugar] = 0;
        user.filaLugar = -1;
        user.columnaLugar = -1;
        user.tieneVehiculo = 0;
        return user;
    }
};

// Cuenta los lugares libres en la Zona General (filas 2 a 4)
int lugaresGeneralesLibres(int matriz[FILAS][COLUMNAS])
{
    int suma = 0;

    for (int i = FILA_INICIO_GENERAL; i < FILAS; i++)
    {
        for (int j = 0; j < COLUMNAS; j++)
        {
            if (matriz[i][j] == 0)
            {
                suma++; // mᵢⱼ = 0 → libre
            }
        }
    }
    printf("Lugares generales libres: %d\n", suma);
    return suma;
};

int lugaresPrioritariosLibres(int matriz[FILAS][COLUMNAS])
{
    int suma = 0;

    for (int i = 0; i < FILAS_PRIORITARIAS; i++)
    {
        for (int j = 0; j < COLUMNAS; j++)
        {
            if (matriz[i][j] == 0)
            {
                suma++;
            }
        }
    }
    printf("Lugares prioritarios libres: %d\n", suma);
    return suma;
};

void actualizarListaClientes(Usuario clientes[], int cantidadUsuarios, Usuario user)
{

    for (int i = 0; i < cantidadUsuarios; i++)
    {
        if (clientes[i].dni == user.dni)
        {
            clientes[i] = user;
            break;
        }
    }
};

void matrizEstacionamientoTest(int matriz[FILAS][COLUMNAS])
{
    int fila;
    int columna;

    // Zona prioritaria libre (0)
    for (fila = 0; fila < FILAS_PRIORITARIAS; fila++) {
        for (columna = 0; columna < COLUMNAS; columna++) {
            matriz[fila][columna] = 0;
        }
    }

    // Zona general ocupada (1)
    for (fila = FILA_INICIO_GENERAL; fila < FILAS; fila++) {
        for (columna = 0; columna < COLUMNAS; columna++) {
            matriz[fila][columna] = 1;
        }
    }
}
