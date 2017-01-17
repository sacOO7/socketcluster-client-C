#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <pthread.h>
#include <json/json.h>
#include <libwebsockets.h>
#include "hashmap_string.c"
#include "hashmap.c"
#include "scclient.h"
#include "json_parser.c"
#include "parser.c"


#define KGRN "\033[0;32;32m"
#define KCYN "\033[0;36m"
#define KRED "\033[0;32;31m"
#define KYEL "\033[1;33m"
#define KBLU "\033[0;32;34m"
#define KCYN_L "\033[1;36m"
#define KBRN "\033[0;33m"
#define RESET "\033[0m"

typedef void (*callback)(struct ackdata *,json_object *,json_object *);

//Defining structure for extra_extensions

int counter=0;

static const struct lws_extension exts[] = {
    {
        "permessage-deflate",
        lws_extension_callback_pm_deflate,
        "permessage-deflate; client_max_window_bits"
    },
    {
        "deflate-frame",
        lws_extension_callback_pm_deflate,
        "deflate_frame"
    },
    { NULL, NULL, NULL /* terminator */ }
};



//Setting flags
static int destroy_flag = 0;
static int connection_flag = 0;
static int writeable_flag = 0;
map_t acks;
map_t singlecallbacks;
map_t singleackcallbacks;
map_t publishcallbacks;

//Setting everything for connect method
struct lws_context *context;
struct lws_context_creation_info info;
struct lws *wsi;
struct lws_client_connect_info i;
struct lws_protocols protocol;
struct sigaction act;
int ietf_version = -1;

int use_ssl = 0;




static void INT_HANDLER(int signo) {
    destroy_flag = 1;
}

//register the signal SIGINT handler 
static struct recv_message * get_message_object(){
    struct recv_message *recv=(struct recv_message *)malloc(sizeof(struct recv_message));
    recv->rid=-1;
    recv->cid=-1;
    recv->data=NULL;
    recv->event=NULL;
    recv->error=NULL;
    return recv;
}



struct socket
{
    char * id;
    char * address;
    int port;
    char * token;
    char * path;
    char *proxy_address;
    int proxy_port;
    void (* connect)();
    void (* disconnect)();


    //Declaring function pointers to set of callbacks
    void (* connect_callback)(struct socket *);
    void (* disconnect_callback)(struct socket *);
    void (* connect_error_callback)(struct socket *);
    void (* onauth_callback)(struct socket *,bool);
    void (* onauthtoken_callback)(struct socket *,char * token);

    //Declaring functions for sending and receiving events
    void (* emitint)(char *,int);
    void (* emitstring)(char *,char *);
    void (* emitobject)(char *,json_object *);

    //Declaring functions for sending and receiving events along with ack
    void (* emitintack)(char *,int,void (*f)(char *,json_object *,json_object *));   
    void (* emitstringack)(char *,char *,void (*f)(char *,json_object *,json_object *));
    void (* emitobjectack)(char *,json_object *,void (*f)(char *,json_object *,json_object *));

    //Declaring functions for subscribe
    void (*subscribe)(char *);
    void (*subscribe_ack)(char *,void (*f)(char *,json_object *,json_object *));

    //Declaring functions for unsubscribe

    void (*unsubscribe)(char *);
    void (*unsubscribe_ack)(char *,void (*f)(char *,json_object *,json_object *));

    //Declaring functions for publish
    void (* publishint)(char *,int);
    void (* publishsring)(char *,char *);
    void (* publishobject)(char *,json_object *);

    //Declaring functions for publish events along with ack
    void (* publishintack)(char *,int,void (*f)(char *,json_object *,json_object *));   
    void (* publishstringack)(char *,char *,void (*f)(char *,json_object *,json_object *));
    void (* publishobjectack)(char *,json_object *,void (*f)(char *,json_object *,json_object *));

    //Declaring listeners for receiving
    void (* on)(char *,void (*f)(char *,json_object *));
    void (* onack)(char *,void (*f)(struct ackdata *,json_object *data,void (*f)(struct ackdata *,json_object *error,json_object *data)));
    void (* onpublish)(char *,void (*f)(char *,json_object *));
    /* data */
}* s;

// struct socket * s;


struct socket * _socket(char *protocol,char *address,int port,char *path,char *proxy_address,int proxy_port){
    s=(struct socket *)malloc(sizeof(struct socket));
    s->id=NULL;
    s->address=address;
    s->port=port;
    s->path=path;
    s->token=NULL;
    s->proxy_address=proxy_address;
    s->proxy_port=proxy_port;

    if(strcmp(protocol,"ws")!=0){
        use_ssl=1;
    }

    s->connect=&socket_connect;
    s->disconnect=&socket_disconnect;

    //Emitters and receivers goes here
    s->emitint=&_emit_int;
    s->emitstring=&_emit_string;
    s->emitobject=&_emit_object;

    s->emitintack=&_emit_int_ack;
    s->emitstringack=&_emit_string_ack;
    s->emitobjectack=&_emit_object_ack;

    s->subscribe=&_subscribe;
    s->subscribe_ack=&_subscribe_ack;

    s->unsubscribe=&_unsubscribe;
    s->unsubscribe_ack=&_unsubscribe_ack;

    s->publishint=&_publishint;
    s->publishsring=&_publishstring;
    s->publishobject=&_publishobject;

    s->publishintack=&_publishint_ack;
    s->publishstringack=&_publishstring_ack;
    s->publishobjectack=&_publishobject_ack;

    s->on=&_on;
    s->onack=&_onack;
    s->onpublish=&_onpublish;


    s->connect_callback=NULL;
    s->disconnect_callback=NULL;
    s->connect_error_callback=NULL;
    s->onauth_callback=NULL;
    s->onauthtoken_callback=NULL;

    acks=hashmap_new();
    singlecallbacks=_hashmap_new();
    singleackcallbacks=_hashmap_new();
    publishcallbacks=_hashmap_new();
    return s;
}

struct session_data {
    int fd;
};

struct pthread_routine_tool {
    struct lws_context *context;
    struct lws *wsi;
};

static int websocket_write_back(struct lws *wsi_in, char *str, int str_size_in) 
{
    if (str == NULL || wsi_in == NULL)
        return -1;

    int n;
    int len;
    unsigned char *out = NULL;

    if (str_size_in < 1) 
        len = strlen(str);
    else
        len = str_size_in;

    out = (unsigned char *)malloc(sizeof(unsigned char )*(LWS_SEND_BUFFER_PRE_PADDING + len + LWS_SEND_BUFFER_POST_PADDING));
    //* setup the buffer*/
    memcpy (out + LWS_SEND_BUFFER_PRE_PADDING, str, len );
    //* write out*/
    n = lws_write(wsi_in, out + LWS_SEND_BUFFER_PRE_PADDING, len, LWS_WRITE_TEXT);

    printf(KBLU"[websocket_write_back] %s\n"RESET, str);
    //* free the buffer*/
    free(out);

    return n;
}

struct ackdata *getackobject(char * name,int rid){
    struct ackdata * data=(struct ackdata *)malloc(sizeof(struct ackdata));
    data->name=name;
    data->id=rid;
    return data;
}

static int ws_service_callback(
                         struct lws *wsi,
                         enum lws_callback_reasons reason, void *user,
                         void *in, size_t len)
{

    switch (reason) {

        case LWS_CALLBACK_CLIENT_ESTABLISHED:{
            printf(KYEL"[Main Service] Connect with server success.\n"RESET);
            json_object * jobj = json_object_new_object();
            json_object *event = json_object_new_string("#handshake");
            json_object * authobject = json_object_new_object();
            json_object *cid = json_object_new_int(++counter);
            if (s->token!=NULL){
            json_object *authtoken = json_object_new_string(s->token);
            json_object_object_add(authobject,"authToken",authtoken);
            json_object_object_add(jobj,"event",event);
            json_object_object_add(jobj,"data",authobject);
            json_object_object_add(jobj,"cid",cid);
            }else{
            json_object_object_add(jobj,"event",event);
            json_object_object_add(jobj,"data",NULL);
            json_object_object_add(jobj,"cid",cid);
            }
            
            char * data = (char *)json_object_to_json_string(jobj);
            printf ("The json object created: %sn",json_object_to_json_string(jobj));
            // "{\"event\": \"#handshake\",\"data\": {\"authToken\":null},\"cid\":1}"
            websocket_write_back(wsi, data, -1);
            if (s->connect_callback!=NULL) {
                s->connect_callback(s);
            }
            
            connection_flag = 1;
        }
            break;

        case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:{
            if (s->connect_error_callback!=NULL) s->connect_error_callback(s);
            printf(KRED"[Main Service] Connect with server error.\n"RESET);
            destroy_flag = 1;
            connection_flag = 0;
        }
            break;

        case LWS_CALLBACK_CLOSED:{
            if (s->disconnect_callback!=NULL) s->disconnect_callback(s);
            printf(KYEL"[Main Service] LWS_CALLBACK_CLOSED\n"RESET);
            destroy_flag = 1;
            connection_flag = 0;
        }    
            break;

        case LWS_CALLBACK_CLIENT_RECEIVE:{
            if (strcmp((char *)in,"#1")==0){
                websocket_write_back(wsi, (char *) "#2", -1);    
            }else{
                printf(KCYN_L"[Main Service] Client recvived:%s\n"RESET, (char *)in);
                // printf("UNDER MESSAGE GOT CALLED");
                char * channel;
                json_object * data;
                bool isAuthenticated;
                struct recv_message * _recv=get_message_object();
                json_object * jobj = json_tokener_parse((char *)in);
                json_parse(jobj,_recv);
                enum parseresult result=parse(_recv);
                if (json_object_get_type(_recv->data)==json_type_object){
                    json_object_object_foreach(_recv->data, key, val) {
                            if (strcmp("channel",key)==0){
                                channel=(char *)json_object_get_string(val);
                            }if (strcmp("data",key)==0){
                                data=val;
                            }if (strcmp("token",key)==0){
                                s->token=(char *)json_object_get_string(val);
                            }if (strcmp("isAuthenticated",key)==0){
                                isAuthenticated=json_object_get_boolean(val);
                            }if (strcmp("id",key)==0){
                                s->id=(char *)json_object_get_string(val);
                            }
                    }
                }
                
                switch(result){
                    case ISAUTHENTICATED:
                    if (s->onauth_callback!=NULL) s->onauth_callback(s,isAuthenticated); 
                    // printf("id is %s",s->id );
                    break;
                    case PUBLISH:
                    printf("Publish got called");
                    handle_publish(channel,data);

                    break;
                    case REMOVETOKEN:
                    printf("Removetoken got called");
                    s->token=NULL;
                    break;
                    case SETTOKEN:
                    printf("Set token got called");
                    if (s->onauthtoken_callback!=NULL) s->onauthtoken_callback(s,s->token);
                    break;
                    case EVENT:
                    printf("Event got called");
                    handle_emit(_recv->event,_recv->data);
                    handle_emit_ack(getackobject(_recv->event,_recv->cid),_recv->data,&_Ack);
                    break;
                    case ACKRECEIVE:
                    printf("Ack receive got called");

                    struct ackobject *ackobj;
                    int error = hashmap_get(acks, _recv->rid, (void**)(&ackobj));
                    if (error==MAP_OK){
                        hashmap_remove(acks,_recv->rid);
                        ackobj->listener(ackobj->event,_recv->error,_recv->data);
                    }
                    break;

                }
                // if (_recv->rid!=-1) 
                // printf("rid is %d",_recv->rid);
                // if (_recv->cid!=-1)
                // printf("cid is %d", _recv->cid);
                // if (_recv->data!=NULL)
                // printf("data is %s",json_object_to_json_string(_recv->data));
                // if (_recv->event!=NULL)
                // printf("event is %s", _recv->event);
                // if (_recv->error!=NULL)
                // printf("error is %s", _recv->error);
                
            }
            if (writeable_flag)
                destroy_flag = 1;
        }
            break;
        case LWS_CALLBACK_CLIENT_WRITEABLE :{
            printf(KYEL"[Main Service] On writeable is called. send byebye message\n"RESET);
            websocket_write_back(wsi, (char *)"Byebye! See you later", -1);
            writeable_flag = 1;
        }    
            break;

        default:
            break;
    }

    return 0;
}

static void *pthread_routine(void *data)
{
    
    // struct pthread_routine_tool *tool = (struct pthread_routine_tool *)tool_in;

    // printf(KBRN"[pthread_routine] Good day. This is pthread_routine.\n"RESET);

    // //* waiting for connection with server done.*/
    // while(!connection_flag)
    //     usleep(1000*20);

    printf("Thread got called");
    sleep(1);
    websocket_write_back(wsi, (char *)data, -1);
    // websocket_write_back(wsi, data, -1);
    // websocket_write_back(wsi, data, -1);
    // websocket_write_back(wsi, data, -1);  

    // //*Send greeting to server*/
    // printf(KBRN"[pthread_routine] Server is ready. send a greeting message to server.\n"RESET); 
    // websocket_write_back(tool->wsi,(char *) "Good day", -1);

    // printf(KBRN"[pthread_routine] sleep 2 seconds then call onWritable\n"RESET);
    // sleep(1);
    // printf(KBRN"------------------------------------------------------\n"RESET);
    // sleep(1);
    // //printf(KBRN"[pthread_routine] sleep 2 seconds then call onWritable\n"RESET);

    // //*involked wriable*/
    // printf(KBRN"[pthread_routine] call on writable.\n"RESET);   
    // lws_callback_on_writable(tool->wsi);

}

void _emit_int(char * event,int data){
    json_object * jobj = json_object_new_object();
    json_object *eventobject = json_object_new_string(event);
    json_object *dataobject = json_object_new_int(data);
    json_object_object_add(jobj,"event",eventobject);
    json_object_object_add(jobj,"data",dataobject);
    // websocket_write_back(wsi, (char *)json_object_to_json_string(jobj), -1);     
    pthread_t pid;
    pthread_create(&pid, NULL, pthread_routine, (char *)json_object_to_json_string(jobj));
    pthread_detach(pid); 
    free(jobj);  
}

void _emit_string(char * event,char * data){
    json_object * jobj = json_object_new_object();
    json_object *eventobject = json_object_new_string(event);
    json_object *dataobject = json_object_new_string(data);
    json_object_object_add(jobj,"event",eventobject);
    json_object_object_add(jobj,"data",dataobject);
    // websocket_write_back(wsi, (char *)json_object_to_json_string(jobj), -1);     
    pthread_t pid;
    pthread_create(&pid, NULL, pthread_routine, (char *)json_object_to_json_string(jobj));
    pthread_detach(pid);   
    free(jobj); 

}
void _emit_object(char *event,json_object * data){
    json_object * jobj = json_object_new_object();
    json_object *eventobject = json_object_new_string(event);
    json_object_object_add(jobj,"event",eventobject);
    json_object_object_add(jobj,"data",data);
    // websocket_write_back(wsi, (char *)json_object_to_json_string(jobj), -1);     
    pthread_t pid;
    pthread_create(&pid, NULL, pthread_routine, (char *)json_object_to_json_string(jobj));
    pthread_detach(pid);
    free(jobj);
}

struct ackobject* getack(char *event,void (*f)(char * event,json_object *error,json_object *data)){
    struct ackobject* object=(struct ackobject*)malloc(sizeof(struct ackobject));
    object->event=event;
    object->listener=f;
    return object;
}

void _emit_int_ack(char * event,int data,void (*f)(char * event,json_object *error,json_object *data)){
    json_object * jobj = json_object_new_object();
    json_object *eventobject = json_object_new_string(event);
    json_object *dataobject = json_object_new_int(data);
    json_object *cnt = json_object_new_int(++counter);
    hashmap_put(acks, counter, getack(event,f));
    json_object_object_add(jobj,"event",eventobject);
    json_object_object_add(jobj,"data",dataobject);
    json_object_object_add(jobj,"cid",cnt);
    // websocket_write_back(wsi, (char *)json_object_to_json_string(jobj), -1);     
    pthread_t pid;
    pthread_create(&pid, NULL, pthread_routine, (char *)json_object_to_json_string(jobj));
    pthread_detach(pid);
    free(jobj); 
}

void _emit_string_ack(char * event,char * data,void (*f)(char * event,json_object *error,json_object *data)){
    json_object * jobj = json_object_new_object();
    json_object *eventobject = json_object_new_string(event);
    json_object *dataobject = json_object_new_string(data);
    json_object *cnt = json_object_new_int(++counter);
    hashmap_put(acks, counter, getack(event,f));
    json_object_object_add(jobj,"event",eventobject);
    json_object_object_add(jobj,"data",dataobject);
    json_object_object_add(jobj,"cid",cnt);
    // websocket_write_back(wsi, (char *)json_object_to_json_string(jobj), -1);     
    pthread_t pid;
    pthread_create(&pid, NULL, pthread_routine, (char *)json_object_to_json_string(jobj));
    pthread_detach(pid);
    free(jobj); 

}

void _emit_object_ack(char *event,json_object * data,void (*f)(char * event,json_object *error,json_object *data)){
    json_object * jobj = json_object_new_object();
    json_object *eventobject = json_object_new_string(event);
    json_object *cnt = json_object_new_int(++counter);
    hashmap_put(acks, counter,getack(event,f));
    json_object_object_add(jobj,"event",eventobject);
    json_object_object_add(jobj,"data",data);
    json_object_object_add(jobj,"cid",cnt);
    // websocket_write_back(wsi, (char *)json_object_to_json_string(jobj), -1);     
    pthread_t pid;
    pthread_create(&pid, NULL, pthread_routine, (char *)json_object_to_json_string(jobj));
    pthread_detach(pid);
    free(jobj); 
}

void _subscribe(char *channelname){
    json_object * jobj = json_object_new_object();
    json_object *eventobject = json_object_new_string("#subscribe");
    json_object * jobj1 = json_object_new_object();
    json_object *channelobject = json_object_new_string(channelname);
    json_object_object_add(jobj,"event",eventobject);
    json_object_object_add(jobj1,"channel",channelobject);
    json_object_object_add(jobj,"data",jobj1);    
    json_object *cnt = json_object_new_int(++counter);
    json_object_object_add(jobj,"cid",cnt);

    pthread_t pid;
    pthread_create(&pid, NULL, pthread_routine, (char *)json_object_to_json_string(jobj));
    pthread_detach(pid);
    free(jobj); 
}

void _subscribe_ack(char *channelname,void (*f)(char *event,json_object *error,json_object *data)){
    json_object * jobj = json_object_new_object();
    json_object *eventobject = json_object_new_string("#subscribe");
    json_object * jobj1 = json_object_new_object();
    json_object *channelobject = json_object_new_string(channelname);
     json_object *cnt = json_object_new_int(++counter);
    hashmap_put(acks, counter,getack(channelname,f));
    json_object_object_add(jobj,"event",eventobject);
    json_object_object_add(jobj1,"channel",channelobject);
    json_object_object_add(jobj,"data",jobj1);    
    json_object_object_add(jobj,"cid",cnt);

    pthread_t pid;
    pthread_create(&pid, NULL, pthread_routine, (char *)json_object_to_json_string(jobj));
    pthread_detach(pid);
    free(jobj); 
}

void _unsubscribe(char *channelname){
    json_object * jobj = json_object_new_object();
    json_object *eventobject = json_object_new_string("#unsubscribe");
    json_object *cnt = json_object_new_int(++counter);
    json_object *channelobject = json_object_new_string(channelname);
    json_object_object_add(jobj,"event",eventobject);
    json_object_object_add(jobj,"data",channelobject);
    json_object_object_add(jobj,"cid",cnt);
    // websocket_write_back(wsi, (char *)json_object_to_json_string(jobj), -1);     
    pthread_t pid;
    pthread_create(&pid, NULL, pthread_routine, (char *)json_object_to_json_string(jobj));
    pthread_detach(pid);
    free(jobj); 
}
void _unsubscribe_ack(char *channelname,void (*f)(char * event,json_object *error,json_object *data)){
    json_object * jobj = json_object_new_object();
    json_object *eventobject = json_object_new_string("#unsubscribe");
    json_object *cnt = json_object_new_int(++counter);
    hashmap_put(acks, counter,getack(channelname,f));
    json_object *channelobject = json_object_new_string(channelname);
    json_object_object_add(jobj,"event",eventobject);
    json_object_object_add(jobj,"data",channelobject);
    json_object_object_add(jobj,"cid",cnt);
    // websocket_write_back(wsi, (char *)json_object_to_json_string(jobj), -1);     
    pthread_t pid;
    pthread_create(&pid, NULL, pthread_routine, (char *)json_object_to_json_string(jobj));
    pthread_detach(pid);
    free(jobj); 

}

void _publishint(char *channelname,int data){
    json_object * jobj = json_object_new_object();
    json_object *eventobject = json_object_new_string("#publish");
    json_object * jobj1 = json_object_new_object();
    json_object *cnt = json_object_new_int(++counter);
    json_object *channelobject = json_object_new_string(channelname);
    json_object *dataobject = json_object_new_int(data);
    json_object_object_add(jobj1,"channel",channelobject);
    json_object_object_add(jobj1,"data",dataobject);
    json_object_object_add(jobj,"event",eventobject);
    json_object_object_add(jobj,"data",jobj1);
    json_object_object_add(jobj,"cid",cnt);
    // websocket_write_back(wsi, (char *)json_object_to_json_string(jobj), -1);     
    pthread_t pid;
    pthread_create(&pid, NULL, pthread_routine, (char *)json_object_to_json_string(jobj));
    pthread_detach(pid); 
    free(jobj);  
}
void _publishstring(char *channelname,char *data){
    json_object * jobj = json_object_new_object();
    json_object *eventobject = json_object_new_string("#publish");
    json_object * jobj1 = json_object_new_object();
    json_object *cnt = json_object_new_int(++counter);
    json_object *channelobject = json_object_new_string(channelname);
    json_object *dataobject = json_object_new_string(data);
    json_object_object_add(jobj1,"channel",channelobject);
    json_object_object_add(jobj1,"data",dataobject);
    json_object_object_add(jobj,"event",eventobject);
    json_object_object_add(jobj,"data",jobj1);
    json_object_object_add(jobj,"cid",cnt);
    // websocket_write_back(wsi, (char *)json_object_to_json_string(jobj), -1);     
    pthread_t pid;
    pthread_create(&pid, NULL, pthread_routine, (char *)json_object_to_json_string(jobj));
    pthread_detach(pid);   
    free(jobj);
}
void _publishobject(char *channelname,json_object *data){   

    json_object * jobj = json_object_new_object();
    json_object *eventobject = json_object_new_string("#publish");
    json_object * jobj1 = json_object_new_object();
    json_object *cnt = json_object_new_int(++counter);
    json_object *channelobject = json_object_new_string(channelname);
    json_object_object_add(jobj1,"channel",channelobject);
    json_object_object_add(jobj1,"data",data);
    json_object_object_add(jobj,"event",eventobject);
    json_object_object_add(jobj,"data",jobj1);
    json_object_object_add(jobj,"cid",cnt);
    // websocket_write_back(wsi, (char *)json_object_to_json_string(jobj), -1);     
    pthread_t pid;
    pthread_create(&pid, NULL, pthread_routine, (char *)json_object_to_json_string(jobj));
    pthread_detach(pid);
    free(jobj);
}



void _publishint_ack(char *channelname,int data,void (*f)(char * event,json_object *error,json_object *data)){
    json_object * jobj = json_object_new_object();
    json_object *eventobject = json_object_new_string("#publish");
    json_object * jobj1 = json_object_new_object();
    json_object *cnt = json_object_new_int(++counter);
    hashmap_put(acks, counter,getack(channelname,f));
    json_object *channelobject = json_object_new_string(channelname);
    json_object *dataobject = json_object_new_int(data);
    json_object_object_add(jobj1,"channel",channelobject);
    json_object_object_add(jobj1,"data",dataobject);
    json_object_object_add(jobj,"event",eventobject);
    json_object_object_add(jobj,"data",jobj1);
    json_object_object_add(jobj,"cid",cnt);
    // websocket_write_back(wsi, (char *)json_object_to_json_string(jobj), -1);     
    pthread_t pid;
    pthread_create(&pid, NULL, pthread_routine, (char *)json_object_to_json_string(jobj));
    pthread_detach(pid);
    free(jobj); 
}
void _publishstring_ack(char *channelname,char *data,void (*f)(char * event,json_object *error,json_object *data)){

    json_object * jobj = json_object_new_object();
    json_object *eventobject = json_object_new_string("#publish");
    json_object * jobj1 = json_object_new_object();
    json_object *cnt = json_object_new_int(++counter);
    hashmap_put(acks, counter,getack(channelname,f));
    json_object *channelobject = json_object_new_string(channelname);
    json_object *dataobject = json_object_new_string(data);
    json_object_object_add(jobj1,"channel",channelobject);
    json_object_object_add(jobj1,"data",dataobject);
    json_object_object_add(jobj,"event",eventobject);
    json_object_object_add(jobj,"data",jobj1);
    json_object_object_add(jobj,"cid",cnt);
    // websocket_write_back(wsi, (char *)json_object_to_json_string(jobj), -1);     
    pthread_t pid;
    pthread_create(&pid, NULL, pthread_routine, (char *)json_object_to_json_string(jobj));
    pthread_detach(pid);
    free(jobj); 

}
void _publishobject_ack(char *channelname,json_object *data,void (*f)(char * event,json_object *error,json_object *data)){
    
    json_object * jobj = json_object_new_object();
    json_object *eventobject = json_object_new_string("#publish");
    json_object * jobj1 = json_object_new_object();
    json_object *cnt = json_object_new_int(++counter);
    hashmap_put(acks, counter,getack(channelname,f));
    json_object *channelobject = json_object_new_string(channelname);
    json_object_object_add(jobj1,"channel",channelobject);
    json_object_object_add(jobj1,"data",data);
    json_object_object_add(jobj,"event",eventobject);
    json_object_object_add(jobj,"data",jobj1);
    json_object_object_add(jobj,"cid",cnt);
    // websocket_write_back(wsi, (char *)json_object_to_json_string(jobj), -1);     
    pthread_t pid;
    pthread_create(&pid, NULL, pthread_routine, (char *)json_object_to_json_string(jobj));
    pthread_detach(pid);
    free(jobj);
}

void _on(char *event,void (*f)(char * event,json_object *data)){
    _hashmap_put(singlecallbacks, event,(void *) f);
}
void _onack(char *event,void (*f)(struct ackdata *,json_object *data,void (*f)(struct ackdata * event,json_object *error,json_object *data))){
    _hashmap_put(singleackcallbacks, event, (void *)f);
}
void _onpublish(char *event,void (*f)(char * event,json_object *data)){
    _hashmap_put(publishcallbacks, event, (void *)f);
}


void handle_emit(char * event,json_object *object){
    void (*f)(char * ,json_object* );
    int error=_hashmap_get(singlecallbacks, event, (void**)(&f));
     if (error==MAP_OK){
    f(event,object);
    }
}

void handle_emit_ack(struct ackdata *ack,json_object *object,void (*f)(struct ackdata *ack,json_object *error,json_object *data)){
    void (*f1)(struct ackdata * ,json_object *,void (*f)(struct ackdata *,json_object *,json_object *));
    int error=_hashmap_get(singleackcallbacks, ack->name, (void**)(&f1));
    if (error==MAP_OK){
    f1(ack,object,f);
    }
}

void handle_publish(char *event,json_object *object){
    void (*f)(char * ,json_object* );
    int error=_hashmap_get(publishcallbacks, event, (void**)(&f));
    if (error==MAP_OK){
    f(event,object);
    }
}

void _Ack(struct ackdata *ack,json_object *error,json_object *data){
    json_object * jobj = json_object_new_object();
    json_object *ridobject = json_object_new_int(ack->id);
    json_object_object_add(jobj,"error",error);
    json_object_object_add(jobj,"data",data);
    json_object_object_add(jobj,"rid",ridobject);

    pthread_t pid;
    pthread_create(&pid, NULL, pthread_routine, (char *)json_object_to_json_string(jobj));
    pthread_detach(pid);
    free(jobj);
}

void socket_disconnect(){
     pthread_exit(NULL);
     lws_context_destroy(context);
     free(s);
}

void socket_connect()
{
    context = NULL;
    wsi = NULL;

    act.sa_handler = INT_HANDLER;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    sigaction( SIGINT, &act, 0);


    memset(&info, 0, sizeof info);
    info.port = CONTEXT_PORT_NO_LISTEN;
    info.iface = NULL;
    info.protocols = &protocol;
    info.ssl_cert_filepath = NULL;
    info.ssl_private_key_filepath = NULL;
    info.http_proxy_address=s->proxy_address;
    info.http_proxy_port=(unsigned int )s->proxy_port;
    // info.extensions = lws_get_internal_extensions();
    info.extensions=NULL;
    info.gid = -1;
    info.uid = -1;
    info.options = 0;

    protocol.name  = "websocket";
    protocol.callback = &ws_service_callback;
    protocol.per_session_data_size = sizeof(struct session_data);
    protocol.rx_buffer_size = 0;
    protocol.id = 0;
    protocol.user = NULL;


    context = lws_create_context(&info);

    memset(&i, 0, sizeof(i));

    i.port = s->port;
    i.path = s->path;
    i.address=s->address;
    i.context = context;
    i.ssl_connection = use_ssl;
    i.host = s->address;
    i.origin = s->address;
    i.protocol="websocket";
    i.ietf_version_or_minus_one = ietf_version;
    i.client_exts = exts;

    printf(KRED"[Main] context created.\n"RESET);

    if (context == NULL) {
        printf(KRED"[Main] context is NULL.\n"RESET);
        return;
    }


    // wsi = lws_client_connect(context, "localhost", 8000, 0,
            // "/socketcluster/", "localhost:8000", NULL,
             // protocol.name, -1);

    wsi=lws_client_connect_via_info(&i);
    
    if (wsi == NULL) {
        printf(KRED"[Main] wsi create error.\n"RESET);
        return;
    }

    printf(KGRN"[Main] wsi create success.\n"RESET);

    // struct pthread_routine_tool tool;
    // tool.wsi = wsi;
    // tool.context = context;

    // pthread_t pid;
    // pthread_create(&pid, NULL, pthread_routine, (char *)"hello");
    // pthread_detach(pid);

    while(!destroy_flag)
    {
        lws_service(context, 50);
    }

    lws_context_destroy(context);

    return;
}


/**
Dependancies : libssl-dev,openssl

gcc client.c -o client -lpthread -lwebsockets -l json

**/

