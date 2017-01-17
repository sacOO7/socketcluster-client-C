
#ifndef __SCCLIENT_H__
#define __SCCLIENT_H__


enum parseresult {
	ISAUTHENTICATED,
	PUBLISH,
	REMOVETOKEN,
	SETTOKEN,
	EVENT,
	ACKRECEIVE
};

void socket_connect();
void socket_disconnect();
void _emit_int(char * event,int data);
void _emit_string(char * event,char * data);
void _emit_object(char *event,json_object * data);

void _emit_int_ack(char * event,int data,void (*f)(char * event,json_object *error,json_object *data));
void _emit_string_ack(char * event,char * data,void (*f)(char * event,json_object *error,json_object *data));
void _emit_object_ack(char *event,json_object * data,void (*f)(char * event,json_object *error,json_object *data));

void _subscribe(char *);
void _subscribe_ack(char *,void (*f)(char * event,json_object *error,json_object *data));

void _unsubscribe(char *);
void _unsubscribe_ack(char *,void (*f)(char * event,json_object *error,json_object *data));

void _publishint(char *,int);
void _publishstring(char *,char *);
void _publishobject(char *,json_object *);


void _publishint_ack(char *,int,void (*f)(char * event,json_object *error,json_object *data));
void _publishstring_ack(char *,char *,void (*f)(char * event,json_object *error,json_object *data));
void _publishobject_ack(char *,json_object *,void (*f)(char * event,json_object *error,json_object *data));


struct ackdata
{
    char *name;
    int id;
    /* data */
};


void _on(char *event,void (*f)(char * event,json_object *data));
void _onack(char *event,void (*f)(struct ackdata * name,json_object *data,void (*f)(struct ackdata * name,json_object *error,json_object *data)));
void _onpublish(char *event,void (*f)(char * event,json_object *data));


void handle_emit(char * event,json_object *object);
void handle_emit_ack(struct ackdata *ack,json_object *object,void (*f)(struct ackdata *,json_object *error,json_object *data));
void handle_publish(char *event,json_object *object);

void _Ack(struct ackdata *ack,json_object *error,json_object *data);

struct recv_message
{
    int rid;
    int cid;
    json_object *data;
    char * event;
    json_object *error;
};


struct ackobject
{
    /* data */
    char *event;
    void (* listener)(char * event,json_object *error,json_object *data);
};

#endif