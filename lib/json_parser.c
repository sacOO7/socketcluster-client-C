
/*printing the value corresponding to boolean, double, integer and strings*/

void print_json_value(json_object *jobj){
  enum json_type type;
  // type = json_object_get_type(jobj); /*Getting the type of the json object*/
  // printf("type: ",type);
  switch (type) {
    case json_type_boolean: printf("json_type_booleann");
                         printf("value: %sn", json_object_get_boolean(jobj)? "true": "false");
                         break;
    case json_type_double: printf("json_type_doublen");
                        printf("          value: %lfn", json_object_get_double(jobj));
                         break;
    case json_type_int: printf("json_type_intn");
                        printf("          value: %dn", json_object_get_int(jobj));
                         break;
    case json_type_string: printf("json_type_stringn");
                         printf("          value: %sn", json_object_get_string(jobj));
                         break;
  }

}

void json_parse_array( json_object *jobj, char *key,struct recv_message * recv) {
  void json_parse(json_object * jobj,struct recv_message * recv); /*Forward Declaration*/
  enum json_type type;

  json_object *jarray = jobj; /*Simply get the array*/
  if(key) {
    jarray = json_object_object_get(jobj, key); /*Getting the array if it is a key value pair*/
  }

  int arraylen = json_object_array_length(jarray); /*Getting the length of the array*/
  printf("Array Length: %dn",arraylen);
  int i;
  json_object * jvalue;

  for (i=0; i< arraylen; i++){
    jvalue = json_object_array_get_idx(jarray, i); /*Getting the array element at position i*/
    type = json_object_get_type(jvalue);
    if (type == json_type_array) {
      json_parse_array(jvalue, NULL,recv);
    }
    else if (type != json_type_object) {
      printf("value[%d]: ",i);
      print_json_value(jvalue);
    }
    else {
      json_parse(jvalue,recv);
    }
  }
}

/*Parsing the json object*/
void json_parse(json_object * jobj,struct recv_message * recv) {
  enum json_type type;
  json_object_object_foreach(jobj, key, val) { /*Passing through every array element*/
    // type = json_object_get_type(val);
    // printf("type: ",type);
    if (strcmp("rid",key)==0){
      recv->rid=json_object_get_int(val);
      // printf("rid is %d",recv->rid);
    }
    if (strcmp("cid",key)==0){
      recv->cid=json_object_get_int(val);
      // printf("cid is %d", recv->cid);
    }
    if (strcmp("data",key)==0){
      recv->data=val;
      // printf("data is %s",json_object_to_json_string(recv->data));
    }
    if (strcmp("event",key)==0){
      recv->event=(char *)json_object_get_string(val);
      // printf("event is %s", recv->event);
    }
    if (strcmp("error",key)==0){
      recv->error=val;
      // printf("error is %s", recv->error);
    }
    // switch (type) {
    //   case json_type_boolean: 
    //   case json_type_double: 
    //   case json_type_int: 
    //   case json_type_string: print_json_value(val);
    //                        break; 
    //   case json_type_object: printf("json_type_objectn");
    //                        jobj = json_object_object_get(jobj, key);
    //                        json_parse(jobj,recv); 
    //                        break;
    //   case json_type_array: printf("type: json_type_array, ");
    //                       json_parse_array(jobj, key,recv);
    //                       break;
    // }
  }
} 