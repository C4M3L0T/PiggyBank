#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>

//Definimos la estructura de la transaccion
typedef struct{
    char *fecha;
    char *hora;
    char *tipo;
    char *usuario;
} Transaccion;

// Definimos el nodo para la lista enlazada de transaccion
typedef struct Nodo {
    Transaccion transaccion;
    struct Nodo *siguiente;
} ListaTransaccion;

// Definimos la estructura del usuario
typedef struct{
    char *usuario;
    char *pass;
    float saldo;
    ListaTransaccion transacciones;
    char *meta;
} Usuario;

// Funcion para inicializar la lista
ListaTransaccion *Listatransaccion(ListaTransaccion *Lista){
    Lista = NULL;
    return Lista;
}

// Funcion para agregar datos en la lista
ListaTransaccion *AgregarTransaccion(ListaTransaccion *Lista,char *fecha,char *hora,char *tipo, char *usuario){
    //Definimos la transaccion
    Transaccion nuevaTransaccion;
    nuevaTransaccion.fecha = fecha;
    nuevaTransaccion.hora = hora;
    nuevaTransaccion.tipo = tipo;
    nuevaTransaccion.usuario = usuario;

    //Crear nodo y asignar transaccion
    ListaTransaccion *nuevoNodo = (ListaTransaccion*)malloc(sizeof(ListaTransaccion));
    nuevoNodo->transaccion = nuevaTransaccion;
    nuevoNodo->siguiente = NULL;
    
    //Verificamos si es el primer nodo de la lista
    if(Lista == NULL){
        Lista = nuevoNodo;
    }else{
        // Encontrar el último nodo en la lista
        ListaTransaccion *ultimoNodo = Lista;
        while (ultimoNodo->siguiente != NULL) {
            ultimoNodo = ultimoNodo->siguiente;
        }
        // Agregar el nuevo nodo al final
        ultimoNodo->siguiente = nuevoNodo;
    }
    return Lista;
}

// Funcion para imprimir la lista
void ImprimirListaTransaccion(ListaTransaccion *Lista){
    while (Lista != NULL)
    {
        printf("Transaccion de %s\n",Lista->transaccion.tipo);
        printf("Fecha: %s\n",Lista->transaccion.fecha);
        printf("Hora: %s\n",Lista->transaccion.hora);
        printf("La transaccion fue hecha por el usuario: %s\n",Lista->transaccion.usuario);
        Lista = Lista->siguiente;
    }
}

// Funciones para imprimir los menus

void MenuInicio(){
    printf("******** Bienvenid@ a la alcancia inteligente *************\n");
    printf("a) Iniciar sesión\n");
    printf("b) Salir\n");
    printf("Elige una opcion:\n");
}

void MenuCRUD(Usuario usuario){
    printf("************** Bienvenid@ de nuevo %s ***************\n",usuario.usuario);
    printf("1) Ver saldo.\n");
    printf("2) Retirar saldo.\n");
    printf("3) Agregar saldo.\n");
    printf("4) Ver historial.\n");
    printf("5) Ver meta.\n");
    printf("6) Cerrar sesion.\n");
    printf("Seleccione una opcion:\n");
}

// Funcion Inicio de Sesion

// Función para obtener la fecha actual
void obtenerFechaHora(char *fecha, char *hora) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    strftime(fecha, 20, "%Y-%m-%d", &tm);
    strftime(hora, 10, "%H:%M:%S", &tm);
}

// Funcion para guardar Usuario

void Guardar(Usuario usuario){
    FILE *archivo = fopen("Usuarios.bin","w");
    fwrite(&usuario, sizeof(Usuario),1, archivo);
    fclose(archivo);
}

int main() {
    Usuario usuario;
    char opc;
    int opciones;
    bool salir = 1;
    char fecha[20];
    char hora[10];

    // Definimos el usuario 
    usuario.pass = "pass";
    usuario.usuario = "Angel";
    usuario.meta = "Silla de escritorio";
    usuario.saldo = 0;

    // Definimos la Lista enlazada
    ListaTransaccion *Lista = Listatransaccion(Lista);

    //Lista = AgregarTransaccion(Lista,fecha,hora,"retiro",usuario.usuario);
    while(1){
        MenuInicio();
        scanf("%c",&opc);

        if(opc == 'a' || opc == 'A'){
            //IniciarSesion();
            do{          
                MenuCRUD(usuario);
                scanf("%i",&opciones);
                switch(opciones){
                    case 1:
                        //VerSaldo();
                    break;

                    case 2:
                        //Retirar;
                        obtenerFechaHora(fecha,hora);
                        Lista = AgregarTransaccion(Lista,fecha,hora,"Retiro",usuario.usuario);

                    break;

                    case 3:
                        //Agregar;
                        obtenerFechaHora(fecha,hora);
                        Lista = AgregarTransaccion(Lista,fecha,hora,"Deposito",usuario.usuario);                        
                    break;

                    case 4:
                        ImprimirListaTransaccion(Lista);
                    break;

                    case 5:
                        //VerMeta();
                    break;

                    case 6:
                        printf("Cerrando sesión...\n");
                        salir = 0;
                    break;

                    default:
                        printf("Opcion no valida\n");
                    break;
                    }
                //Guardar(usuario);
            }while(salir);
            salir = 1;
            
        }else if(opc == 'b' || opc == 'B'){
            printf("Hasta luego!\n");
            break;
        }else{
            printf("Esa opcion no es correcta\n");
        }
    }
    
    return 0;
}