#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

//Definimos la estructura de la transaccion
typedef struct{
    char *fecha;
    char *hora;
    char *tipo;
    char *usuario;
    float cambioDeSaldo;
} Transaccion;

// Definimos el nodo para la lista enlazada de transaccion
typedef struct Nodo {
    Transaccion transaccion;
    struct Nodo *siguiente;
} ListaTransaccion;

// Funcion para inicializar la lista
ListaTransaccion *Listatransaccion(ListaTransaccion *Lista){
    Lista = NULL;
    return Lista;
}

// Definimos la estructura del usuario
typedef struct{
    char *usuario;
    char *pass;
    float saldo;
    ListaTransaccion *transacciones;
    char *meta;
} Usuario;



// Funcion para agregar datos en la lista
ListaTransaccion *AgregarTransaccion(ListaTransaccion *Lista,char fecha[20],char hora[10],char *tipo, char *usuario,float cambioDeSaldo){
    //Definimos la transaccion
    Transaccion nuevaTransaccion;
    nuevaTransaccion.fecha = strdup(fecha);
    nuevaTransaccion.hora = strdup(hora);
    nuevaTransaccion.tipo = tipo;
    nuevaTransaccion.usuario = usuario;
    nuevaTransaccion.cambioDeSaldo = cambioDeSaldo;

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

    printf("****************** Historial de transacciones ****************************\n");
    while (Lista != NULL)
    {
        printf("Transaccion de %s\n",Lista->transaccion.tipo);
        printf("Fecha: %s\n",Lista->transaccion.fecha);
        printf("Hora: %s\n",Lista->transaccion.hora);
        if(strcmp(Lista->transaccion.tipo, "Deposito") == 0){
            printf("Deposito realizado de $%f pesos.\n",Lista->transaccion.cambioDeSaldo);
        }else{
            printf("Retiro realizado de $%f pesos.\n",Lista->transaccion.cambioDeSaldo);
        }
        printf("La transaccion fue hecha por el usuario: %s\n",Lista->transaccion.usuario);
        if(Lista->siguiente != NULL){
            printf("**********************************************\n");
        }
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

bool IniciarSesion(char *name, char *password,Usuario usuario){
    if((strcmp(usuario.usuario, name) == 0) && strcmp(usuario.pass, password) == 0){
        return true;
    }else{
        return false;
    }
}

// Función para obtener la fecha actual
void obtenerFechaHora(char *fecha, char *hora) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    strftime(fecha, 20, "%Y-%m-%d", &tm);
    strftime(hora, 10, "%H:%M:%S", &tm);
}

// Funcion para leer archivo
Usuario* LeerUsuario(){
    FILE* file = fopen("Usuarios.bin", "rb");
    if (file == NULL) {
        printf("No se pudo abrir el archivo %s\n", "Usuarios.bin");
        return NULL;
    }
    Usuario* usuario = malloc(sizeof(Usuario));
    // Cargar los campos del usuario
    int length;
    fread(&length, sizeof(int), 1, file);
    usuario->usuario = malloc(sizeof(char) * length);
    fread(usuario->usuario, sizeof(char), length, file);

    fread(&length, sizeof(int), 1, file);
    usuario->pass = malloc(sizeof(char) * length);
    fread(usuario->pass, sizeof(char), length, file);

    fread(&(usuario->saldo), sizeof(float), 1, file);

    fread(&length, sizeof(int), 1, file);
    usuario->meta = malloc(sizeof(char) * length);
    fread(usuario->meta, sizeof(char), length, file);

    // Cargar las transacciones
    usuario->transacciones = NULL;
    ListaTransaccion* actual = NULL;
    while (fread(&length, sizeof(int), 1, file) == 1) {
        ListaTransaccion* nuevo = malloc(sizeof(ListaTransaccion));
        nuevo->siguiente = NULL;

        nuevo->transaccion.fecha = malloc(sizeof(char) * length);
        fread(nuevo->transaccion.fecha, sizeof(char), length, file);

        fread(&length, sizeof(int), 1, file);
        nuevo->transaccion.hora = malloc(sizeof(char) * length);
        fread(nuevo->transaccion.hora, sizeof(char), length, file);

        fread(&length, sizeof(int), 1, file);
        nuevo->transaccion.tipo = malloc(sizeof(char) * length);
        fread(nuevo->transaccion.tipo, sizeof(char), length, file);

        fread(&length, sizeof(int), 1, file);
        nuevo->transaccion.usuario = malloc(sizeof(char) * length);
        fread(nuevo->transaccion.usuario, sizeof(char), length, file);

        fread(&(nuevo->transaccion.cambioDeSaldo), sizeof(float), 1, file);

        if(usuario->transacciones == NULL){
            usuario->transacciones = nuevo;
            actual = nuevo;
        }else{
            actual->siguiente = nuevo;
            actual = nuevo;
        }
        // Verificar si hay más datos en el archivo antes de asignar memoria para un nuevo nodo
        //if (fread(&length, sizeof(int), 1, file) == 1) {
        //    actual->siguiente = malloc(sizeof(ListaTransaccion));
        //    actual = actual->siguiente;
        //} else {
        //    actual->siguiente = NULL;
        //}
    }
    fclose(file);
    return usuario;
    }

// Funcion para guardar archivo 
void GuardarUsuario(Usuario* usuario){
    FILE* file = fopen("Usuarios.bin", "wb");
    if (file == NULL) {
        printf("No se pudo abrir el archivo %s\n", "Usuarios.bin");
        return;
    }

    // Guardar los campos del usuario
    int length = strlen(usuario->usuario) + 1;
    fwrite(&length, sizeof(int), 1, file);
    fwrite(usuario->usuario, sizeof(char), length, file);

    length = strlen(usuario->pass) + 1;
    fwrite(&length, sizeof(int), 1, file);
    fwrite(usuario->pass, sizeof(char), length, file);

    fwrite(&(usuario->saldo), sizeof(float), 1, file);

    length = strlen(usuario->meta) + 1;
    fwrite(&length, sizeof(int), 1, file);
    fwrite(usuario->meta, sizeof(char), length, file);

     // Guardar las transacciones
    ListaTransaccion* actual = usuario->transacciones;
    while (actual != NULL) {
        length = strlen(actual->transaccion.fecha) + 1;
        fwrite(&length, sizeof(int), 1, file);
        fwrite(actual->transaccion.fecha, sizeof(char), length, file);

        length = strlen(actual->transaccion.hora) + 1;
        fwrite(&length, sizeof(int), 1, file);
        fwrite(actual->transaccion.hora, sizeof(char), length, file);

        length = strlen(actual->transaccion.tipo) + 1;
        fwrite(&length, sizeof(int), 1, file);
        fwrite(actual->transaccion.tipo, sizeof(char), length, file);

        length = strlen(actual->transaccion.usuario) + 1;
        fwrite(&length, sizeof(int), 1, file);
        fwrite(actual->transaccion.usuario, sizeof(char), length, file);

        fwrite(&(actual->transaccion.cambioDeSaldo), sizeof(float), 1, file);

        actual = actual->siguiente;
    }
    fclose(file);
}

// Funcion para liberar memoria dinamica 
void LiberarUsuario(Usuario* usuario) {
    if (usuario == NULL) return;
    free(usuario->usuario);
    free(usuario->pass);
    free(usuario->meta);

    ListaTransaccion* actual = usuario->transacciones;
    while (actual != NULL) {
        ListaTransaccion* siguiente = actual->siguiente;
        free(actual->transaccion.fecha);
        free(actual->transaccion.hora);
        free(actual->transaccion.tipo);
        free(actual->transaccion.usuario);   
        free(actual);
        actual = siguiente;
    }
}


int main() {
    Usuario usuario;
    char opc;
    int opciones;
    bool salir = 1;
    char fecha[20];
    char hora[10];
    char name[50];
    char password[50];
    float cambioDeSaldo; 
    // Definimos el espacio para la lista de transacciones 
    //Definimos el usuario 
    //usuario.usuario = "Angel";
    //usuario.pass = "pass";
    //usuario.saldo = 0;
    //usuario.meta = "Silla Gamer: 5000"; 
    
    while(1){
        MenuInicio();
        //Leemos el usuario desde el archivo
        usuario = *LeerUsuario();
        
        scanf(" %c",&opc);
        if(opc == 'a' || opc == 'A'){
            printf("Ingresa el usuario:\n");
            scanf("%s",name);
            printf("Ingresa la contrasena:\n");
            scanf("%s",password);
            if(IniciarSesion(name,password,usuario)){
                do{
                MenuCRUD(usuario);
                scanf("%i",&opciones);
                switch(opciones){
                    case 1:
                        //VerSaldo
                        printf("Tu saldo es de $%f pesos\n",usuario.saldo);
                    break;

                    case 2:
                        //Retirar
                        printf("Ingresa la cantidad en pesos a retirar: \n");
                        scanf("%f",&cambioDeSaldo);
                        if(cambioDeSaldo>usuario.saldo){
                            printf("No tienes dinero suficiente para hacer este retiro\n");
                        }else{
                            usuario.saldo = usuario.saldo-cambioDeSaldo;
                            obtenerFechaHora(fecha,hora);
                            usuario.transacciones = AgregarTransaccion(usuario.transacciones,fecha,hora,"Retiro",usuario.usuario,cambioDeSaldo);
                            printf("Tu nuevo saldo es $%f pesos\n",usuario.saldo);
                        }
                    break;

                    case 3:
                        //Agregar
                        printf("Ingresa la cantidad en pesos a depositar: \n");
                        scanf("%f",&cambioDeSaldo);
                        usuario.saldo = usuario.saldo+cambioDeSaldo;
                        obtenerFechaHora(fecha,hora);
                        usuario.transacciones = AgregarTransaccion(usuario.transacciones,fecha,hora,"Deposito",usuario.usuario,cambioDeSaldo);
                        printf("Tu nuevo saldo es $%f pesos\n",usuario.saldo);                    
                    break;

                    case 4:
                        //Historial
                        ImprimirListaTransaccion(usuario.transacciones);
                    break;

                    case 5:
                        //Meta
                        printf("La objetivo de %s para ahorrar es para: %s\n",usuario.usuario,usuario.meta);
                    break;

                    case 6:
                        printf("Cerrando sesión...\n");
                        salir = 0;
                    break;

                    default:
                        printf("Opcion no valida\n");
                    break;
                    }            
            }while(salir);
            salir = 1;
            GuardarUsuario(&usuario);
            //LiberarUsuario(&usuario);
            }else{
                printf("Combinacion de claves incorrecta\n");
            }
        }else if(opc == 'b' || opc == 'B'){
            printf("Hasta luego!\n");
            break;
        }else{
            printf("Esa opcion no es correcta\n");
        }
    }
    return 0;
}