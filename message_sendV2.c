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
    message_buf sbuf, rbuf;
    size_t buf_length;

    char *dialogos_emisor[] = {
        "Are you hearing me?",
        "I can hear you too",
    };

    if ((msqid = msgget(key, IPC_CREAT | 0666)) < 0) {
        perror("msgget (emisor)");
        exit(1);
    }

    for (int i = 0; i < 2; i++) {
        // 1. Enviar el mensaje (Tipo 1)
        sbuf.mtype = 1;
        strcpy(sbuf.mtext, dialogos_emisor[i]);
        buf_length = strlen(sbuf.mtext) + 1;

        printf("[Emisor] Enviando: \"%s\"\n", sbuf.mtext);
        if (msgsnd(msqid, &sbuf, buf_length, 0) < 0) {
            perror("msgsnd");
            exit(1);
        }

        // 2. Esperar la respuesta correspondiente del receptor (Tipo 2)
        if (msgrcv(msqid, &rbuf, MSGSZ, 2, 0) < 0) {
            perror("msgrcv");
            exit(1);
        }
        printf("[Emisor] Receptor respondió: \"%s\"\n\n", rbuf.mtext);
    }

    // Limpieza final de la cola
    msgctl(msqid, IPC_RMID, NULL);
    printf("Conversación finalizada y cola eliminada.\n");
    return 0;
}
