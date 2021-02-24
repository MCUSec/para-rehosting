#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

struct msg_buffer{
    long msg_type;
    char msg[1000];
}message;

int main()
{
    char msg[1000];
    key_t key;
    int msgid;
    key =ftok("mqtt",999);
    msgid = msgget(key, 0666 | IPC_CREAT);
    if(msgid == -1){
        printf("msgget return error\n");
        return -1;
    }
    while(1){
        msgrcv(msgid, &message, sizeof(message), 1, 0);
        printf("%s\n",message.msg);
        memset(&message, 0x0, sizeof(message));
    }
    msgctl(msgid, IPC_RMID, NULL);
    return -1;

}