#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

struct msg {
    long type;
    char text[100];
};

int main() {
    int msgid;
    struct msg message;

    msgid = msgget(IPC_PRIVATE, 0666 | IPC_CREAT);

    if (fork() == 0) {
        // Child Process
        msgrcv(msgid, &message, sizeof(message.text), 1, 0);

        int len = strlen(message.text);
        char reversed[100];

        for (int i = 0; i < len; i++) {
            reversed[i] = message.text[len - i - 1];
        }
        reversed[len] = '\0';

        strcpy(message.text, reversed);
        message.type = 2;

        msgsnd(msgid, &message, sizeof(message.text), 0);
    }
    else {
        // Parent Process
        char original[100];

        printf("Enter a string: ");
        scanf("%s", original);

        message.type = 1;
        strcpy(message.text, original);

        msgsnd(msgid, &message, sizeof(message.text), 0);

        msgrcv(msgid, &message, sizeof(message.text), 2, 0);

        printf("Reversed string: %s\n", message.text);

        if (strcmp(original, message.text) == 0)
            printf("String is Palindrome\n");
        else
            printf("String is NOT Palindrome\n");

        msgctl(msgid, IPC_RMID, NULL);
    }

    return 0;
}
