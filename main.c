#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <errno.h>
#include "comm.h"//commands header

#define MAX_BUFF 1024

extern int errno;

void shellExec(char* comm, char* param[], char* files[], int isRedirect, char* redirectFile){//running or calling the implemented commands
	char* CM[3]={"cd","wc","tee"};
	char* output;
	int noCM=-1;
	for(int i=0;i<3;i++) {
        if (strcmp(comm,CM[i]) == 0){
        	noCM=i;
            break;
        }
    }
    switch(noCM){
    	case 0:
    		output=cd(comm,param,files, isRedirect, redirectFile);
    		break;
    	case 1:
    		output=wc(comm,param,files, isRedirect, redirectFile);
    		break;
    	case 2:
    		output=tee(comm,param,files, isRedirect, redirectFile);
    		break;
    	default:
    		break;
    }
    if(isRedirect==0){
    	FILE* outRedir;
    	int errNum;
    	outRedir=fopen(redirectFile,"w");
    	if(!output){
	    	errNum=errno;
	    	fprintf(stderr,"Error code %d: %s\n",errno,strerror(errNum));
	    }else{
			fprintf(outRedir,"%s",output);
			fclose(outRedir);
	    }
    }else if(isRedirect==2){
		FILE* outRedir;
    	int errNum;
    	outRedir=fopen(redirectFile,"a");
    	if(!output){
	    	errNum=errno;
	    	fprintf(stderr,"Error code %d: %s\n",errno,strerror(errNum));
	    }else{
			fprintf(outRedir,"%s",output);
			fclose(outRedir);
	    }
    }else{
    	printf("%s\n",output);
    }
}

void shellParse(char* s){//input preprocessing
	char* comm;
	char* param[4]={"","","",""};
	char* files[5]={"","","","",""};
	int isRedirect=-1;
	char* redirectFile="";
	char* token=strtok(s," ");
	char* acCm[7]={"exit","clear","version","help","cd","wc","tee"};
	int noCM=-1;
	comm=token;
	for(int i=0;i<7;i++) {
        if (strcmp(comm,acCm[i]) == 0){
        	token=strtok(NULL," ");
        	noCM=i;
            break;
        }
    }
    if(noCM==-1){
    	printf("Error: '%s' is not an accepted command, type 'help' to see the accepted commands\n",comm);
    }else if (noCM>=0&&noCM<=3){
    	switch(noCM){
    		case 0:
				exit(0);
	    	case 1:
	    		system("clear");
	    		break;
	    	case 2:
	    		version();
	    		break;
	    	case 3:
	    		help();
	    		break;
	    	default:
	    		break;
    	}
    }else{
    	int i=0,j=0,foundParam=0;
	    while(i<4&&token){
	    	if(!strncmp("-",token,1)){
	    		if(strncmp("--",token,2)){
	    			param[j]=token;
	    			j++;
	    			token=strtok(NULL," ");
	    		}else{
	    			printf("Error: '%s' is not a valid parameter format, valid format is '-param'\n",token);
	    			break;
	    		}
	    	}
	    	else{
	    		break;
	    	}
	    	i++;
	    }
	    i=0;
	    while(i<5&&token){
	    	if(!strcmp(token,">")){
	    		isRedirect=0;
	    		token=strtok(NULL," ");
	    		break;
	    	}else if(!strcmp(token,"<")){
	    		isRedirect=1;
	    		token=strtok(NULL," ");
	    		break;
	    	}else if(!strcmp(token,">>")){
	    		isRedirect=2;
	    		token=strtok(NULL," ");
	    		break;
	    	}else{
	    		files[i]=token;
		    	i++;
		    	token=strtok(NULL," ");
	    	}
	    }
	    if(isRedirect!=-1){
	    	redirectFile=token;
	    }
	    shellExec(comm,param,files,isRedirect,redirectFile);
    }
    
}

void shellLoop(){//input loop
	char path[MAX_BUFF];
	char* lineRead;
	while(1){
		char *s;
		getcwd(path, MAX_BUFF);
		lineRead=strcat(path,">");
		s=readline(lineRead);
		if(strlen(s)>0){
			add_history(s);
			shellParse(s);
		}
	}
}

int main(){
	shellLoop();
}
