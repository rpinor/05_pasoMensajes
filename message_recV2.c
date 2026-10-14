#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MSGSZ 128

typedef struct msgbuf {
    long mtype;
    char mtext[MSGSZ];
} message_buf;

int main(void) {
    int msqid;
    key_t key = 1234;
    message_buf rbuf, sbuf;
    size_t buf_length;

    // Lista de respuestas preestablecidas para el Receptor
    char *dialogos_receptor[] = {
        "Loud and clear"
    };

    if ((msqid = msgget(key, IPC_CREAT | 0666)) < 0) {
        perror("msgget (receptor)");
        exit(1);
    }

    printf("Esperando al emisor...\n\n");

    for (int i = 0; i < 1; i++) {
        // 1. Recibir el mensaje del emisor (Tipo 1)
        if (msgrcv(msqid, &rbuf, MSGSZ, 1, 0) < 0) {
            perror("msgrcv");
            exit(1);
        }
        printf("[Receptor] Emisor dijo: \"%s\"\n", rbuf.mtext);

        // 2. Enviar la respuesta preestablecida correspondiente (Tipo 2)
        sbuf.mtype = 2;
        strcpy(sbuf.mtext, dialogos_receptor[i]);
        buf_length = strlen(sbuf.mtext) + 1;

        printf("[Receptor] Respondiendo: \"%s\"\n\n", sbuf.mtext);
        if (msgsnd(msqid, &sbuf, buf_length, 0) < 0) {
            perror("msgsnd");
            exit(1);
        }
    }

    printf("Fin de las respuestas estructuradas.\n");
    return 0;
}

