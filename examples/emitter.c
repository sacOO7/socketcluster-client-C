#include <stdio.h>
#include "../lib/scclient.c"

void _callback(char * event,json_object *error,json_object *data){
    printf("Callback got called\n");
    printf("Data is :: %s",json_object_to_json_string(data));
    printf("Error is :: %s",json_object_to_json_string(error));
}

void _connect(struct socket * s)
{
    printf("successfully connected");
    int t;
    scanf ("%d",&t);
    s->emitint((char *)"chat",3);
    // s->emitstring((char *)"chat","Hi there, How are you brother");
    // s->emitobject((char *)"chat",json_object_new_string("Hey there bro"));
    // s->emitintack((char *)"chat",1,&_callback);
    // s->emitstringack((char *)"chat","This is a sample string",&_callback);
    // s->emitobjectack((char *)"chat",json_object_new_int(9),&_callback);

}

void disconnect(struct socket * s)
{
    printf("successfully dis-connected");
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
     
    s=Socket((char *)"http",(char *)"localhost",8000,(char *)"/socketcluster/",NULL,-1);
    s->connect_callback=&_connect;
    s->disconnect_callback=&disconnect;
    s->connect_error_callback=&connect_error;    
    s->onauth_callback=&on_auth;
    s->onauthtoken_callback=&on_set_auth_token;
    // s->allowselfsigned();
    s->connect();
    // s->disconnect();
}


/**
gcc emitter.c -o emitter -lwebsockets -l json -lpthread
*/
