#include <stdio.h>
#include "../lib/scclient.c"

void _connect(struct socket * s)
{
    printf("socket successfully connected\n");
    printf("successfully connected");
    int t;
    scanf ("%d",&t);
    s->emitint((char *)"chat",3);
}


void on_chat(struct ackdata *ack,json_object *data,callback f){
    printf("got event for %s and data is %s",ack->name,json_object_to_json_string(data));
    f(ack,json_object_new_string("I got this error"),json_object_new_string("sending this data to you"));
}

void onchat(char *event,json_object *data){
    printf("got event for %s and data is %s",event,json_object_to_json_string(data));

}
void disconnect(struct socket * s)
{
    printf("socket dis-connected from server\n");
}

void connect_error(struct socket * s)
{
    printf("successful connect error");
}
void on_auth(struct socket * s,bool isauthenticated)
{
    printf("Authentication is %s",isauthenticated ? "true" : "false");
}
void on_set_auth_token(struct socket * s,char * token)
{
    printf("on token got called");
}

int main()
{
     
    s=Socket((char *)"ws",(char *)"localhost",8000,(char *)"/socketcluster/",NULL,-1);
    s->connect_callback=&_connect;
    s->disconnect_callback=&disconnect;
    s->connect_error_callback=&connect_error;    
    s->onauth_callback=&on_auth;
    s->onauthtoken_callback=&on_set_auth_token;

    s->on((char *)"chat",&onchat);

    //Setting listener over here
    // s->onack((char *)"chat",&on_chat);




    s->connect();
    // s1->disconnect();
}


/**
gcc receiver.c -o receiver -lwebsockets -l json -lpthread
*/