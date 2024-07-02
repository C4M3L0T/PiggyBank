#include <stdio.h>
#include <string.h>
#include <time.h>

// Datos de usuario almacenados
#define USERNAME "usuario"
#define PASSWORD "pass"
#define TOTAL_BALANCE 1000.0

// Función para obtener la fecha actual
void getCurrentDate(int *day, int *month, int *year) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    *day = tm.tm_mday;
    *month = tm.tm_mon + 1;
    *year = tm.tm_year + 1900;
}

// Función para realizar el desglose de billetes
void desgloseBilletes(int monto) {
    int billetes[] = {500, 200, 100, 50, 20, 10, 5, 1};
    int cantidadBilletes[sizeof(billetes) / sizeof(billetes[0])] = {0};

    for (int i = 0; i < sizeof(billetes) / sizeof(billetes[0]); i++) {
        cantidadBilletes[i] = monto / billetes[i];
        monto %= billetes[i];
    }

    printf("Desglose de billetes:\n");
    for (int i = 0; i < sizeof(billetes) / sizeof(billetes[0]); i++) {
        if (cantidadBilletes[i] > 0) {
            printf("%d billete(s) de %d\n", cantidadBilletes[i], billetes[i]);
        }
    }
}

int main() {
    char usuario[50], contrasena[50];
    int day, month, year;
    float maxRetiro, cantidadRetirar;

    // Mostrar mensaje de bienvenida y presentación de uso
    printf("Bienvenido al cajero automático.\n");
    printf("Por favor, inicie sesión para continuar.\n");

    // Solicitar usuario y contraseña
    printf("Usuario: ");
    scanf("%s", usuario);
    printf("Contraseña: ");
    scanf("%s", contrasena);

    // Comparar datos de inicio de sesión
    if (strcmp(usuario, USERNAME) != 0 || strcmp(contrasena, PASSWORD) != 0) {
        printf("Usuario o contraseña incorrectos.\n");
        return 1;
    }

    // Obtener la fecha actual
    //getCurrentDate(&day,&month,&year);
    day = 4; //Ponemos esta fecha, ya que en el sistema de mi computadora estamos a ultimos de Junio C:


    // Validar si el día es dentro de los primeros 5 días del mes
    if (day > 5) {
        printf("Solo se puede retirar en los primeros 5 días del mes.\n");
        return 1;
    }

    // Calcular el monto máximo a retirar
    maxRetiro = TOTAL_BALANCE * 0.40;
    printf("El monto máximo que puede retirar es: %.2f\n", maxRetiro);

    // Solicitar la cantidad a retirar
    printf("Ingrese la cantidad a retirar: ");
    scanf("%f", &cantidadRetirar);

    // Validar la cantidad a retirar
    if (cantidadRetirar > maxRetiro) {
        printf("La cantidad no puede ser mayor al 40%% del total (%.2f).\n", maxRetiro);
        return 1;
    }

    // Realizar el desglose de billetes
    desgloseBilletes((int)cantidadRetirar);

    return 0;
}