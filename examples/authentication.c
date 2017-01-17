#include <stdio.h>
#include "../lib/scclient.c"

void _connect(struct socket * s)
{
    printf("\nsuccessfully connected");

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
    printf("\non token got called and token is %s\n",token);
}

int main()
{
     
    s=Socket((char *)"ws",(char *)"localhost",8000,(char *)"/socketcluster/",NULL,-1);
    s->connect_callback=&_connect;
    s->disconnect_callback=&disconnect;
    s->connect_error_callback=&connect_error;    
    s->onauth_callback=&on_auth;
    s->onauthtoken_callback=&on_set_auth_token;
    s->connect();
    // s1->disconnect();
}

/**
gcc authentication.c -o authentication.c -lwebsockets -l json -lpthread
*/
