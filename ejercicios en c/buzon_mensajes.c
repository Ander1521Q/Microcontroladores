#include <stdio.h>

#define MAX_SMS_TEXT 140
#define MAX_MMS_RUTA 200
#define MAX_MENSAJES 100

// Estructura para un mensaje SMS
struct SMS {
    char remitente[20];
    int fechaHora;
    char texto[MAX_SMS_TEXT + 1];
    char tipo[10];  // Puede ser "SMC" o "normal"
};

// Estructura para un mensaje MMS
struct MMS {
    char remitente[20];
    int fechaHora;
    char texto[MAX_SMS_TEXT + 1];
    char rutaImagen[MAX_MMS_RUTA + 1];
};

// Estructura general para un mensaje del buzón
struct Mensaje {
    int esMMS; // 0 = SMS, 1 = MMS
    union {
        struct SMS sms;
        struct MMS mms;
    } contenido;
};

// Estructura del buzón con 100 mensajes
struct Buzon {
    struct Mensaje mensajes[MAX_MENSAJES];
};

int main() {
    struct Buzon buzon;  // Creamos el buzón de entrada
    printf("Estructura de buzon creada con %d mensajes.\n", MAX_MENSAJES);
    return 0;
}
