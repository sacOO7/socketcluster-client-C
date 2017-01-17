

enum parseresult parse(struct recv_message * recv){
	
	if (recv->event!=NULL){
		if (strcmp(recv->event,"#publish")==0){
			return PUBLISH;
		}else if (strcmp(recv->event,"#removeAuthToken")==0){
			return REMOVETOKEN;
		}else if (strcmp(recv->event,"#setAuthToken")==0){
			return SETTOKEN;
		}else{
			return EVENT;
		}
	}else if (recv->rid==1){
		return ISAUTHENTICATED;
	}else{
		return ACKRECEIVE;
	}
}