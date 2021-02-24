#include "mbed.h"
#include "NetworkInterface.h"
#include "debug_log.h"

#define FUZZ_TARGET 1 // 1: MQTT 2: CoAP 3: mbed-client-cli
#if FUZZ_TARGET == 1

#elif FUZZ_TARGET == 2
    #define FUZZ_PART 1 // 1: parser of CoAP 2: builder of CoAP
#elif FUZZ_TARGET == 3

#else

#endif

#if FUZZ_TARGET==1 // MQTT section start
    #include "MQTTNetwork.h"
    #include "MQTTmbed.h"
    #include "MQTTClient.h"

    void messageArrived(MQTT::MessageData& md)
    {
        debug_log("messageArrived",sizeof("messageArrived"));
    }
    // NetworkInterface* easy_connect(){
    //     NetworkInterface* network_interface = 0;
    //     int connect_success = -1;
    //     connect_success = eth.connect();
    //     network_interface = &eth;
    //     return network_interface;
    // }

    void Fuzz() {
        float version = 0.6;
        char* topic = "mbed-sample";
        debug_log("main start",sizeof("main start"));
        NetworkInterface network;
        debug_log("main 1",sizeof("main 1"));
        MQTTNetwork mqttNetwork(&network);
        debug_log("main 2",sizeof("main 2"));
        MQTT::Client<MQTTNetwork, Countdown> client(mqttNetwork);
        const char* hostname = "mqtt.eclipse.org";
        int port = 1883;
        int rc = mqttNetwork.connect(hostname, port);
        assert(rc==0);
        MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
        data.MQTTVersion = 3;
        data.clientID.cstring = "mbed-sample";
        data.username.cstring = "testuser";
        data.password.cstring = "testpassword";
        rc = client.connect(data);
        assert(rc==0);
        rc = client.subscribe(topic, MQTT::QOS2, messageArrived);
        assert(rc==0);
        Countdown cd;
    #ifdef __AFL_HAVE_MANUAL_CONTROL
        while (__AFL_LOOP(1000)) {
    #endif
        client.cycle(cd);
        }
    }
// MQTT section start
#elif FUZZ_TARGET==2 // CoAP section start
    #include "sn_coap_protocol.h"
    #include "sn_coap_header.h"

    #if FUZZ_PART==1
        struct coap_s* coapHandle;
        coap_version_e coapVersion = COAP_VERSION_1;

        void* coap_malloc(uint16_t size){
            return malloc(size);
        }
        void coap_free(void* addr){
            free(addr);
        }
        uint8_t coap_tx_cb(uint8_t *a, uint16_t b, sn_nsdl_addr_s *c, void *d){
            debug_log("coap_tx_cb", sizeof("coap_tx_cb"));
            return 0;
        }
        int8_t coap_rx_cb(sn_coap_hdr_s *a, sn_nsdl_addr_s *b, void *c){
            debug_log("coap_rx_cb", sizeof("coap_rx_cb"));
            return 0;
        }

        void fuzz_parser() {
            coapHandle = sn_coap_protocol_init(&coap_malloc, &coap_free, &coap_tx_cb, &coap_rx_cb);
            nsapi_size_or_error_t ret;
            uint8_t* recv_buffer = (uint8_t*)malloc(1280);
            #ifdef __AFL_HAVE_MANUAL_CONTROL
            while (__AFL_LOOP(1000)) {      
            #endif
            memset(recv_buffer, 0x0, 1280);
            ret = read(0, recv_buffer, 1280);
            sn_coap_hdr_s* parsed = sn_coap_parser(coapHandle, ret, recv_buffer, &coapVersion);
            #ifdef __AFL_HAVE_MANUAL_CONTROL
            }
            #endif
        }
    #endif // FUZZ_PART==1
    #if FUZZ_PART==2
        void fuzz_builder(){
            sn_coap_hdr_s* coap_res_ptr = (sn_coap_hdr_s*)calloc(sizeof(sn_coap_hdr_s), 1); 
            
        #ifdef __AFL_HAVE_MANUAL_CONTROL
            while (__AFL_LOOP(1000)) {      
        #endif
            read(0, &coap_res_ptr->token_len, sizeof(coap_res_ptr->token_len));   
            read(0, &coap_res_ptr->coap_status, sizeof(coap_res_ptr->coap_status));
            read(0, &coap_res_ptr->msg_code, sizeof(coap_res_ptr->msg_code)); // 4 bytes
            read(0, &coap_res_ptr->msg_type, sizeof(coap_res_ptr->msg_type));
            read(0, &coap_res_ptr->content_format, sizeof(coap_res_ptr->content_format)); // 4 bytes
            read(0, &coap_res_ptr->msg_id, sizeof(coap_res_ptr->msg_id)); // 2 bytes
            read(0, &coap_res_ptr->uri_path_len, sizeof(coap_res_ptr->uri_path_len)); // 2 bytes
            read(0, &coap_res_ptr->payload_len, sizeof(coap_res_ptr->payload_len)); // 2 bytes
            coap_res_ptr->token_ptr = (uint8_t*)malloc(coap_res_ptr->token_len);
            read(0, coap_res_ptr->token_ptr, coap_res_ptr->token_len);
            coap_res_ptr->uri_path_ptr = (uint8_t*)malloc(coap_res_ptr->uri_path_len);
            read(0, coap_res_ptr->uri_path_ptr, coap_res_ptr->uri_path_len);
            coap_res_ptr->payload_ptr = (uint8_t*)malloc(coap_res_ptr->payload_len);
            read(0, coap_res_ptr->payload_ptr, coap_res_ptr->payload_len);
            coap_res_ptr->options_list_ptr = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
            read(0, &coap_res_ptr->options_list_ptr->etag_len, sizeof(coap_res_ptr->options_list_ptr->etag_len));
            unsigned int use_size1_1 = 0;
            read(0, &use_size1_1, sizeof(unsigned int));
            coap_res_ptr->options_list_ptr->use_size1 = use_size1_1;
            unsigned int use_size2_1 = 0;
            read(0, &use_size2_1, sizeof(unsigned int));
            coap_res_ptr->options_list_ptr->use_size2 = use_size2_1;
            read(0, &coap_res_ptr->options_list_ptr->proxy_uri_len, sizeof(coap_res_ptr->options_list_ptr->proxy_uri_len));
            read(0, &coap_res_ptr->options_list_ptr->uri_host_len, sizeof(coap_res_ptr->options_list_ptr->uri_host_len));
            read(0, &coap_res_ptr->options_list_ptr->location_path_len, sizeof(coap_res_ptr->options_list_ptr->location_path_len));
            read(0, &coap_res_ptr->options_list_ptr->location_query_len, sizeof(coap_res_ptr->options_list_ptr->location_query_len));
            read(0, &coap_res_ptr->options_list_ptr->uri_query_len, sizeof(coap_res_ptr->options_list_ptr->uri_query_len));
            read(0, &coap_res_ptr->options_list_ptr->accept, sizeof(coap_res_ptr->options_list_ptr->accept));
            read(0, &coap_res_ptr->options_list_ptr->max_age, sizeof(coap_res_ptr->options_list_ptr->max_age));
            read(0, &coap_res_ptr->options_list_ptr->size1, sizeof(coap_res_ptr->options_list_ptr->size1));
            read(0, &coap_res_ptr->options_list_ptr->size2, sizeof(coap_res_ptr->options_list_ptr->size2));
            read(0, &coap_res_ptr->options_list_ptr->uri_port, sizeof(coap_res_ptr->options_list_ptr->uri_port));
            read(0, &coap_res_ptr->options_list_ptr->observe, sizeof(coap_res_ptr->options_list_ptr->observe));
            read(0, &coap_res_ptr->options_list_ptr->block1, sizeof(coap_res_ptr->options_list_ptr->block1));
            read(0, &coap_res_ptr->options_list_ptr->block2, sizeof(coap_res_ptr->options_list_ptr->block2));
            coap_res_ptr->options_list_ptr->proxy_uri_ptr = (uint8_t*)malloc(coap_res_ptr->options_list_ptr->proxy_uri_len);
            read(0, coap_res_ptr->options_list_ptr->proxy_uri_ptr, coap_res_ptr->options_list_ptr->proxy_uri_len);
            coap_res_ptr->options_list_ptr->etag_ptr = (uint8_t*)malloc(coap_res_ptr->options_list_ptr->etag_len);
            read(0, coap_res_ptr->options_list_ptr->etag_ptr, coap_res_ptr->options_list_ptr->etag_len);
            coap_res_ptr->options_list_ptr->uri_host_ptr = (uint8_t*)malloc(coap_res_ptr->options_list_ptr->uri_host_len);
            read(0, coap_res_ptr->options_list_ptr->uri_host_ptr, coap_res_ptr->options_list_ptr->uri_host_len);
            coap_res_ptr->options_list_ptr->location_path_ptr = (uint8_t*)malloc(coap_res_ptr->options_list_ptr->location_path_len);
            read(0, coap_res_ptr->options_list_ptr->location_path_ptr, coap_res_ptr->options_list_ptr->location_path_len);
            coap_res_ptr->options_list_ptr->location_query_ptr = (uint8_t*)malloc(coap_res_ptr->options_list_ptr->location_query_len);
            read(0, coap_res_ptr->options_list_ptr->location_query_ptr, coap_res_ptr->options_list_ptr->location_query_len);
            coap_res_ptr->options_list_ptr->uri_query_ptr = (uint8_t*)malloc(coap_res_ptr->options_list_ptr->uri_query_len);
            read(0, coap_res_ptr->options_list_ptr->uri_query_ptr, coap_res_ptr->options_list_ptr->uri_query_len);
            uint16_t message_len = sn_coap_builder_calc_needed_packet_data_size(coap_res_ptr);
            uint8_t* message_ptr = (uint8_t*)malloc(message_len);
            sn_coap_builder(message_ptr, coap_res_ptr);
            if(coap_res_ptr->options_list_ptr != NULL){
                if(coap_res_ptr->options_list_ptr->uri_query_ptr != NULL)
                free(coap_res_ptr->options_list_ptr->uri_query_ptr);
                if(coap_res_ptr->options_list_ptr->location_query_ptr != NULL)
                    free(coap_res_ptr->options_list_ptr->location_query_ptr);
                if(coap_res_ptr->options_list_ptr->location_path_ptr != NULL)
                    free(coap_res_ptr->options_list_ptr->location_path_ptr);
                if(coap_res_ptr->options_list_ptr->uri_host_ptr != NULL)
                    free(coap_res_ptr->options_list_ptr->uri_host_ptr);
                if(coap_res_ptr->options_list_ptr->etag_ptr != NULL)
                    free(coap_res_ptr->options_list_ptr->etag_ptr);
                if(coap_res_ptr->options_list_ptr->proxy_uri_ptr != NULL)
                    free(coap_res_ptr->options_list_ptr->proxy_uri_ptr);
                if(coap_res_ptr->options_list_ptr != NULL)
                    free(coap_res_ptr->options_list_ptr);
                if(coap_res_ptr->payload_ptr != NULL)
                    free(coap_res_ptr->payload_ptr);
                if(coap_res_ptr->uri_path_ptr != NULL)
                    free(coap_res_ptr->uri_path_ptr);
                if(coap_res_ptr->token_ptr != NULL)
                    free(coap_res_ptr->token_ptr);
            }
            
        #ifdef __AFL_HAVE_MANUAL_CONTROL
            }
        #endif
        }
    #endif // FUZZ_PART==2
    void Fuzz(){
        #if FUZZ_PART == 1
            fuzz_parser();
        #elif FUZZ_PART == 2
            fuzz_builder();
        #else

        #endif
    }
// CoAP section end
#elif FUZZ_TARGET==3 // mbed-client-cli section start
    #include "ns_cmdline.h"
    //simple print function
    void myprint(const char* fmt, va_list ap){  }
    // simple ready cb, which call next command to be execute
    void cmd_ready_cb(int retcode) { cmd_next( retcode ); }
    // dummy command with some option
    int cmd_dummy(int argc, char *argv[]){
        // if( cmd_has_option(argc, argv, "o") ) {
        //     cmd_printf("This is o option");
        // } else {
        //     return CMDLINE_RETCODE_INVALID_PARAMETERS;
        // }
        return CMDLINE_RETCODE_SUCCESS;
    }
    // timer cb ( pseudo-timer-code )
    void timer_ready_cb(void) {
        cmd_ready(CMDLINE_RETCODE_SUCCESS);
    }
    // long command, which need e.g. some events to finalize command execution
    int cmd_long(int argc, char *argv[] ) {
        // timer_start( 5000, timer_ready_cb );
        return CMDLINE_RETCODE_EXCUTING_CONTINUE;
    }
    void Fuzz() {
        cmd_init( &myprint );              // initialize cmdline with print function
        cmd_set_ready_cb( cmd_ready_cb );  // configure ready cb
        cmd_add("dummy", cmd_dummy, 0, 0); // add one dummy command
        cmd_add("long", cmd_long, 0, 0);   // add one dummy command
        //execute dummy and long commands
        // uint16_t cmd_len = 0;
        // char* p = NULL;
        char p[1024] = {0x0};
    #ifdef __AFL_HAVE_MANUAL_CONTROL
        while (__AFL_LOOP(1000)) {      
    #endif
        memset(p, 0x0, 1024);
        // read(0, &cmd_len, sizeof(cmd_len));
        // char *p = (char*)malloc(cmd_len);
        // read(0, p, cmd_len);
        read(0, p, 1024);
        cmd_exe(p);

    #ifdef __AFL_HAVE_MANUAL_CONTROL
        }
    #endif

    }
// mbed-client-cli section end
#else
    void Fuzz() {}
#endif

int main(int argc, char* argv[])
{
    Fuzz();
    _Exit(0);
    return 0;
}