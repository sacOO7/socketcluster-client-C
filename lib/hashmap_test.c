#include <stdio.h>
#include <stdlib.h>
#include "hashmap.c"
int main(){

map_t mymap;
mymap = hashmap_new();
int x=2;
int error = hashmap_put(mymap, "sample", &x);	
if (error==MAP_OK){
	printf("\nNo error while putting");
}
int result;
error =hashmap_get(mymap, "sample", (void**)(&result));
if (error==MAP_OK){
	printf("\nNo error while getting");
}
printf("result is %d",result);
}