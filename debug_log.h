#ifndef DEBUG_LOG_H
#define DEBUG_LOG_H
    // 0: printf
    // 1: message passing
    #define LOG_METHOD -1

    #if LOG_METHOD == 0
        void debug_log(char* msg, int len){
            printf("%s\n",msg);
        }
    #elif LOG_METHOD == 1
        #include <stdio.h>
        #include <sys/ipc.h>
        #include <sys/msg.h>
        #include <string.h>
        struct msg_buffer{
            long msg_type;
            char msg[1000];
        }message;
        void debug_log(char* msg, int len){
            key_t key;
            int msgid;
            key =ftok("mqtt",999);
            msgid = msgget(key, 0666 | IPC_CREAT);
            if(msgid != -1){
                message.msg_type = 1;
                memcpy(message.msg, msg, len);
                msgsnd(msgid, &message, sizeof(message), 0);
                // msgctl(msgid, IPC_RMID, NULL);
            }
            printf("%.*s\n",len,msg);
        }
    #else
        #define debug_log(a,b)
    #endif

#endif