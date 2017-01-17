#include <stdio.h>
#include "../lib/scclient.c"

void _sub_callback(char * channelname,json_object *error,json_object *data){
    if (error==NULL){
        printf("\nsubscribed successfully to channel %s",channelname);
    }
}

void _pub_callback(char * channelname,json_object *error,json_object *data){
    if (error==NULL){
        printf("\nPublished successfully to channel %s",channelname);
    }   
}

void _connect(struct socket * s)
{
    printf("successfully connected");
    s->subscribe_ack("yell",&_sub_callback);
    int a;
    scanf("%d",&a);
    s->publishintack("yell",4,&_pub_callback);
    // s->publishstring("yell","Is anyone here?");


}


void onyell(char *event,json_object *data){
    printf("\ngot event for %s and data is %s",event,json_object_to_json_string(data));
}

void disconnect(struct socket * s)
{
    printf("\nsuccessfully dis-connected");
}

void connect_error(struct socket * s)
{
    printf("\nsuccessful connect error");
}
void on_auth(struct socket * s,bool isauthenticated)
{
    printf("\nAuthentication is %s",isauthenticated ? "true" : "false");
}
void on_set_auth_token(struct socket * s,char * token)
{
    printf("\non token got called");
}

int main()
{
     
    s=Socket((char *)"ws",(char *)"localhost",8000,(char *)"/socketcluster/",NULL,-1);
    s->connect_callback=&_connect;
    s->disconnect_callback=&disconnect;
    s->connect_error_callback=&connect_error;    
    s->onauth_callback=&on_auth;
    s->onauthtoken_callback=&on_set_auth_token;
    s->onpublish("yell",&onyell);
    s->connect();
    // s1->disconnect();
}

/**
gcc channel.c -o channel -lwebsockets -l json -lpthread
*/