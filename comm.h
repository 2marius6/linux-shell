void help(){//help command
	printf("\n\nAll commands avalible for this shell:\n\n");
	printf("- help\n");
	printf("- exit\n");
	printf("- version\n");
	printf("- clear\n");
	printf("- wc with parameters: -c, -w, -l, -L\n");
	printf("  Example: wc file wc -param file\n");
	printf("  IMPORTANT: while in wc loop, when using it without parameters, files or redirection, when typing 'xt', the loop will stop and you will get back\n");
	printf("- tee with parameter: -a\n");
	printf("  Example: tee file or tee -a file\n");
	printf("  IMPORTANT: while in tee loop, when typing 'xt', the loop will stop and you will get back\n");
	printf("- cd\n");
	printf("  Example: cd path\n");
	printf("  IMPORTANT: this shell supports a maximum of 5 files (except for wc command with parameter which supports just one) and 4 parameters at once for every command\n\n\n");
}

void version(){//version command
	printf("\n\nThis is a shell done for the Operating Systems lab by Marius Nichiteanu\n\n");
	printf("CS (english language) year 2\n");
	printf("Winter semester 2021-2022\n\n\n");
}

char* wc(char* comm, char* param[], char* files[], int isRedirect, char* redirectFile){//wc command
	if(strcmp(param[0],"")&&strcmp(param[0],"-c")&&strcmp(param[0],"-w")&&strcmp(param[0],"-l")&&strcmp(param[0],"-L")){
		return "Error: invalid parameter specified for 'wc', accepted parameters are: '-c', '-w', '-l', '-L'";
	}
	char* errNum=(char*)malloc(100);
	if(isRedirect == 1){//wc with redirrect
		FILE* input;
		input=fopen(redirectFile,"r");
		if(!input){
			sprintf(errNum,"%d",errno);
	    	char* errorTxt=strerror(errno);
	    	char* error=(char*)malloc(1024);
	    	strcpy(error,"Error number ");
	    	strcat(error,errNum);
	    	strcat(error,": ");
	    	strcat(error,errorTxt);
	    	return error;
		}else{
			if(!strcmp(param[0],"")){//no param
				int nOfChar=0, nOfWord=0, nOfLines=0, w=0;
		    	char c;
		    	while((c=fgetc(input))!=EOF){
		    		nOfChar++;
		    		if(c==' '||c=='\t'||c=='\0'||c=='\n'){//searching for word delimiters
						if(w){
							nOfWord++;
							w=0;
						}
					}else{
						w=1;
					}
					if(c=='\n'){//searching for newline
						nOfLines++;
					}
		    	}
				fclose(input);
				char* output=(char*)malloc(1024);
				char* lineNo=(char*)malloc(100);
				char* wordNo=(char*)malloc(100);
				char* charNo=(char*)malloc(100);
				sprintf(lineNo,"%d",nOfLines);
				sprintf(wordNo,"%d",nOfWord);
				sprintf(charNo,"%d",nOfChar);
				strcpy(output,lineNo);
				strcat(output," ");
				strcat(output,wordNo);
				strcat(output," ");
				strcat(output,charNo);
				return output;
			}else{//w param
				int isParam[4]={0,0,0,0};
				int nOfChar=0, nOfWord=0, nOfLines=0, w=0, lineLen=0, lineLenLongest=0,j=0;
				char c;
				for(int i=0;i<5;i++){
					if(!strcmp(param[i],"-c")){
						isParam[0]++;
						rewind(input);
						while(fgetc(input) != EOF){
							nOfChar++;
						}
					}else if(!strcmp(param[i],"-w")){
						isParam[1]++;
						rewind(input);
						while((c=fgetc(input)) != EOF){
							if(c==' '||c=='\t'||c=='\0'||c=='\n'){//searching for word delimiters
								if(w){
									nOfWord++;
									w=0;
								}
							}else{
								w=1;
							}		
						}
					}else if(!strcmp(param[i],"-l")){
						isParam[2]++;
						rewind(input);
						while((c=fgetc(input)) != EOF){
							if(c=='\n'){//searching for newline
								nOfLines++;
							}		
						}
					}else if(!strcmp(param[i],"-L")){
						isParam[3]++;
						rewind(input);
						while((c=fgetc(input))!=EOF){
					    	if(c=='\n'){
					    		if(lineLen>lineLenLongest)//comparing the former longest length to the current length
					    			lineLenLongest=lineLen;
					    		lineLen=0;
					    	}else{
					    		lineLen++;
					    	}
					    }

					}
				}
				fclose(input);
				char* output=(char*)malloc(1024);
				strcpy(output,"");
				if(isParam[0]!=0){
					char* charNo=(char*)malloc(100);
					sprintf(charNo,"%d",nOfChar);
					strcat(output,charNo);
					strcat(output," ");
				}
				if(isParam[1]!=0){
					char* wordNo=(char*)malloc(100);
					sprintf(wordNo,"%d",nOfWord);
					strcat(output,wordNo);
					strcat(output," ");
				}
				if(isParam[2]!=0){
					char* lineNo=(char*)malloc(100);
					sprintf(lineNo,"%d",nOfLines);
					strcat(output,lineNo);
					strcat(output," ");
				}
				if(isParam[3]!=0){
					char* lineLongest=(char*)malloc(100);
					sprintf(lineLongest,"%d",lineLenLongest);
					strcat(output,lineLongest);
				}
				return output;
			}
		}
	}else if(isRedirect!=1&&!strcmp(files[0],"")&&!strcmp(param[0],"")){//wc no param no files
		FILE* temp;
		temp=fopen("temp","w");
		if(!temp){
			sprintf(errNum,"%d",errno);
	    	char* errorTxt=strerror(errno);
	    	char* error=(char*)malloc(1024);
	    	strcpy(error,"Error number ");
	    	strcat(error,errNum);
	    	strcat(error,": ");
	    	strcat(error,errorTxt);
	    	return error;
		}else{
			char* input;
			do{
				input=readline("");
				if(strcmp(input,"xt")){
					fprintf(temp,"%s\n",input);
				}
			}while(strcmp(input,"xt"));
			fclose(temp);
			temp=fopen("temp","r");
			if(!temp){
				sprintf(errNum,"%d",errno);
		    	char* errorTxt=strerror(errno);
		    	char* error=(char*)malloc(1024);
		    	strcpy(error,"Error number ");
		    	strcat(error,errNum);
		    	strcat(error,": ");
		    	strcat(error,errorTxt);
		    	return error;
			}else{
				int nOfChar=0, nOfWord=0, nOfLines=0, w=0;
		    	char c;
		    	while((c=fgetc(temp))!=EOF){
		    		nOfChar++;
		    		if(c==' '||c=='\t'||c=='\0'||c=='\n'){//searching for word delimiters
						if(w){
							nOfWord++;
							w=0;
						}
					}else{
						w=1;
					}
					if(c=='\n'){//searching for newline
						nOfLines++;
					}
		    	}
				fclose(temp);
				remove("temp");
				char* output=(char*)malloc(1024);
				char* lineNo=(char*)malloc(100);
				char* wordNo=(char*)malloc(100);
				char* charNo=(char*)malloc(100);
				sprintf(lineNo,"%d",nOfLines);
				sprintf(wordNo,"%d",nOfWord);
				sprintf(charNo,"%d",nOfChar);
				strcpy(output,lineNo);
				strcat(output," ");
				strcat(output,wordNo);
				strcat(output," ");
				strcat(output,charNo);
				return output;
			}
		}
	}else if(strcmp(param[0],"")){//wc w parameters and file
		if(strcmp(param[0],"")&&!strcmp(files[0],"")){//just param
			FILE* temp;
			temp=fopen("temp","w");
			if(!temp){
				sprintf(errNum,"%d",errno);
		    	char* errorTxt=strerror(errno);
		    	char* error=(char*)malloc(1024);
		    	strcpy(error,"Error number ");
		    	strcat(error,errNum);
		    	strcat(error,": ");
		    	strcat(error,errorTxt);
		    	return error;
			}else{
				char* input;
				do{
					input=readline("");
					if(strcmp(input,"xt")){
						fprintf(temp,"%s\n",input);
					}
				}while(strcmp(input,"xt"));
				fclose(temp);
				temp=fopen("temp","r");
				if(!temp){
					sprintf(errNum,"%d",errno);
			    	char* errorTxt=strerror(errno);
			    	char* error=(char*)malloc(1024);
			    	strcpy(error,"Error number ");
			    	strcat(error,errNum);
			    	strcat(error,": ");
			    	strcat(error,errorTxt);
			    	return error;
				}else{
					int isParam[4]={0,0,0,0};
					int nOfChar=0, nOfWord=0, nOfLines=0, w=0, lineLen=0, lineLenLongest=0,j=0;
					char c;
					for(int i=0;i<5;i++){
						if(!strcmp(param[i],"-c")){
							isParam[0]++;
							rewind(temp);
							while(fgetc(temp) != EOF){
								nOfChar++;
							}
						}else if(!strcmp(param[i],"-w")){
							isParam[1]++;
							rewind(temp);
							while((c=fgetc(temp)) != EOF){
								if(c==' '||c=='\t'||c=='\0'||c=='\n'){//searching for word delimiters
									if(w){
										nOfWord++;
										w=0;
									}
								}else{
									w=1;
								}		
							}
						}else if(!strcmp(param[i],"-l")){
							isParam[2]++;
							rewind(temp);
							while((c=fgetc(temp)) != EOF){
								if(c=='\n'){//searching for newline
									nOfLines++;
								}		
							}
						}else if(!strcmp(param[i],"-L")){
							isParam[3]++;
							rewind(temp);
							while((c=fgetc(temp))!=EOF){
						    	if(c=='\n'){
						    		if(lineLen>lineLenLongest)//comparing the former longest length to the current length
						    			lineLenLongest=lineLen;
						    		lineLen=0;
						    	}else{
						    		lineLen++;
						    	}
						    }
						}
					}
					fclose(temp);
					remove("temp");
					char* output=(char*)malloc(1024);
					strcpy(output,"");
					if(isParam[0]!=0){
						char* charNo=(char*)malloc(100);
						sprintf(charNo,"%d",nOfChar);
						strcat(output,charNo);
						strcat(output," ");
					}
					if(isParam[1]!=0){
						char* wordNo=(char*)malloc(100);
						sprintf(wordNo,"%d",nOfWord);
						strcat(output,wordNo);
						strcat(output," ");
					}
					if(isParam[2]!=0){
						char* lineNo=(char*)malloc(100);
						sprintf(lineNo,"%d",nOfLines);
						strcat(output,lineNo);
						strcat(output," ");
					}
					if(isParam[3]!=0){
						char* lineLongest=(char*)malloc(100);
						sprintf(lineLongest,"%d",lineLenLongest);
						strcat(output,lineLongest);
					}
					return output;
				}
			}
		}else{//param + file
			FILE* temp;
			temp=fopen(files[0],"r");
			if(!temp){
				sprintf(errNum,"%d",errno);
		    	char* errorTxt=strerror(errno);
		    	char* error=(char*)malloc(1024);
		    	strcpy(error,"Error number ");
		    	strcat(error,errNum);
		    	strcat(error,": ");
		    	strcat(error,errorTxt);
		    	return error;
			}else{
				char* input;
				int isParam[4]={0,0,0,0};
				int nOfChar=0, nOfWord=0, nOfLines=0, w=0, lineLen=0, lineLenLongest=0,j=0;
				char c;
				for(int i=0;i<5;i++){
					if(!strcmp(param[i],"-c")){
						isParam[0]++;
						rewind(temp);
						while(fgetc(temp) != EOF){
							nOfChar++;
						}
					}else if(!strcmp(param[i],"-w")){
						isParam[1]++;
						rewind(temp);
						while((c=fgetc(temp)) != EOF){
							if(c==' '||c=='\t'||c=='\0'||c=='\n'){//searching for word delimiters
								if(w){
									nOfWord++;
									w=0;
								}
							}else{
								w=1;
							}		
						}
					}else if(!strcmp(param[i],"-l")){
						isParam[2]++;
						rewind(temp);
						while((c=fgetc(temp)) != EOF){
							if(c=='\n'){//searching for newline
								nOfLines++;
							}		
						}
					}else if(!strcmp(param[i],"-L")){
						isParam[3]++;
						rewind(temp);
						while((c=fgetc(temp))!=EOF){
					    	if(c=='\n'){
					    		if(lineLen>lineLenLongest)//comparing the former longest length to the current length
					    			lineLenLongest=lineLen;
					    		lineLen=0;
					    	}else{
					    		lineLen++;
					    	}
					    }
					}
				}
				fclose(temp);
					char* output=(char*)malloc(1024);
					strcpy(output,"");
					if(isParam[2]!=0){
						char* lineNo=(char*)malloc(100);
						sprintf(lineNo,"%d",nOfLines);
						strcat(output,lineNo);
						strcat(output," ");
					}
					if(isParam[1]!=0){
						char* wordNo=(char*)malloc(100);
						sprintf(wordNo,"%d",nOfWord);
						strcat(output,wordNo);
						strcat(output," ");
					}
					if(isParam[0]!=0){
						char* charNo=(char*)malloc(100);
						sprintf(charNo,"%d",nOfChar);
						strcat(output,charNo);
						strcat(output," ");
					}
					if(isParam[3]!=0){
						char* lineLongest=(char*)malloc(100);
						sprintf(lineLongest,"%d",lineLenLongest);
						strcat(output,lineLongest);
					}
					strcat(output," ");
					strcat(output,files[0]);
					return output;
			}
		}

	}else if(!strcmp(param[0],"")&&strcmp(files[0],"")){//wc just file/files
		FILE* input[5];
		int j=0, nOfCharT=0,nOfWordT=0,nOfLinesT=0,multF;
		char* output=(char*)malloc(1024);
		for (int i=0;i<5;i++){
			if(strcmp(files[i],"")){
				input[i]=fopen(files[i],"r");
				j++;
			}
			if(!input[i]&&strcmp(files[i],"")){
				printf("%d\n",i);
				sprintf(errNum,"%d",errno);
		    	char* errorTxt=strerror(errno);
		    	char* error=(char*)malloc(1024);
		    	strcpy(error,"Error number ");
		    	strcat(error,errNum);
		    	strcat(error,": ");
		    	strcat(error,errorTxt);
		    	return error;
			}
		}
		for(int i=0;i<j;i++){
	    	char c;
	    	char* lineNo=(char*)malloc(100);
			char* wordNo=(char*)malloc(100);
			char* charNo=(char*)malloc(100);
    		int nOfChar=0, nOfWord=0, nOfLines=0, w=0;
    		multF=i;
    		while((c=fgetc(input[i]))!=EOF){
	    		nOfChar++;
	    		if(c==' '||c=='\t'||c=='\0'||c=='\n'){//searching for word delimiters
					if(w){
						nOfWord++;
						w=0;
					}
				}else{
					w=1;
				}
				if(c=='\n'){//searching for newline
					nOfLines++;
				}
	    	}
	    	fclose(input[i]);
	    	strcpy(lineNo,"");
	    	strcpy(wordNo,"");
	    	strcpy(charNo,"");
			sprintf(lineNo,"%d",nOfLines);
			sprintf(wordNo,"%d",nOfWord);
			sprintf(charNo,"%d",nOfChar);
			if(i==0){
				strcpy(output,lineNo);
			}else{
				strcat(output,lineNo);
			}
			strcat(output," ");
			strcat(output,wordNo);
			strcat(output," ");
			strcat(output,charNo);
			strcat(output," ");
			strcat(output,files[i]);
			strcat(output,"\n");
	    	nOfCharT+=nOfChar;
	    	nOfWordT+=nOfWord;
	    	nOfLinesT+=nOfLines;
		}
		if(multF){
    		char* lineNoT=(char*)malloc(100);
			char* wordNoT=(char*)malloc(100);
			char* charNoT=(char*)malloc(100);
    		sprintf(lineNoT,"%d",nOfLinesT);
			sprintf(wordNoT,"%d",nOfWordT);
			sprintf(charNoT,"%d",nOfCharT);
			strcat(output,lineNoT);
			strcat(output," ");
			strcat(output,wordNoT);
			strcat(output," ");
			strcat(output,charNoT);
			strcat(output," ");
			strcat(output,"total");
    	}
    	return output;
	}
}

char* tee(char* comm, char* param[], char* files[], int isRedirect, char* redirectFile){//tee implementation
	if(strcmp(param[0],"")&&strcmp(param[0],"-a")){
		return "Error: invalid parameter specified for 'tee', accepted parameters are: '-a'";
	}else{
		char* errNum=(char*)malloc(100);
		if(!strcmp(param[0],"-a")){
			if(!strcmp(files[0],"")){
				char* input;
				do{
					input=readline("");
					if(strcmp(input,"xt")){
						printf("%s\n",input);
					}
				}while(strcmp(input,"xt"));
				return "Success";
			}else{
				FILE* output[5];
				int j=0;
				for(int i=0;i<5;i++){
					if(strcmp(files[i],"")){
						output[i]=fopen(files[i],"a");
						j++;
					}
					if(!output[i]&&strcmp(files[i],"")){
						sprintf(errNum,"%d",errno);
				    	char* errorTxt=strerror(errno);
				    	char* error=(char*)malloc(1024);
				    	strcpy(error,"Error number ");
				    	strcat(error,errNum);
				    	strcat(error,": ");
				    	strcat(error,errorTxt);
				    	return error;
					}
				}
				char* input;
				do{
					input=readline("");
					if(strcmp(input,"xt")){
						printf("%s\n",input);
						for(int i=0;i<j;i++){
							fprintf(output[i],"%s\n",input);
						}
					}
				}while(strcmp(input,"xt"));
				for(int i=0;i<j;i++){
					fclose(output[i]);
				}
				return "Success";
			}	
		}else{
			if(!strcmp(files[0],"")){
				char* input;
				do{
					input=readline("");
					if(strcmp(input,"xt")){
						printf("%s\n",input);
					}
				}while(strcmp(input,"xt"));
				return "Success";
			}else{
				FILE* output[5];
				int j=0;
				for(int i=0;i<5;i++){
					if(strcmp(files[i],"")){
						output[i]=fopen(files[i],"w");
						j++;
					}
					if(!output[i]&&strcmp(files[i],"")){
						sprintf(errNum,"%d",errno);
				    	char* errorTxt=strerror(errno);
				    	char* error=(char*)malloc(1024);
				    	strcpy(error,"Error number ");
				    	strcat(error,errNum);
				    	strcat(error,": ");
				    	strcat(error,errorTxt);
				    	return error;
					}
				}
				char* input;
				do{
					input=readline("");
					if(strcmp(input,"xt")){
						printf("%s\n",input);
						for(int i=0;i<j;i++){
							fprintf(output[i],"%s\n",input);
						}
					}
				}while(strcmp(input,"xt"));
				for(int i=0;i<j;i++){
					fclose(output[i]);
				}
				return "Success";
			}	
		}
		
	}
}

char* cd(char* comm, char* param[], char* files[], int isRedirect, char* redirectFile){//cd implementation
	char* errNum=(char*)malloc(100);
	if(isRedirect==1){//checking if redirection exists
		FILE* input;
		input=fopen(redirectFile,"r");
		if(!input){
	    	sprintf(errNum,"%d",errno);
	    	char* errorTxt=strerror(errno);
	    	char* error=(char*)malloc(1024);
	    	strcpy(error,"Error number ");
	    	strcat(error,errNum);
	    	strcat(error,": ");
	    	strcat(error,errorTxt);
	    	return error;
	    }else{
	    	fseek (input,0,SEEK_END);
		    int size = ftell(input);
		    if (size==0){
		        return "Error: The input file is empty";
		    }else{
		    	rewind(input);
		    	char* content=(char*)malloc(1024);
		    	fscanf(input,"%s",content);
		    	if(chdir(content)){//checking if path from file exists
					sprintf(errNum,"%d",errno);
			    	char* errorTxt=strerror(errno);
			    	char* error=(char*)malloc(1024);
			    	strcpy(error,"Error number ");
			    	strcat(error,errNum);
			    	strcat(error,": ");
			    	strcat(error,errorTxt);
			    	return error;
				}
		    }
	    }
	}
	else if(files[0]==""){//implementation of cd without anything
		char* user=getlogin();
		char path[1024];
		strcpy(path,"/home/");
		strcat(path,user);
		chdir(path);
		return "Working directory changed";
	}
	else if(chdir(files[0])){//checking if given path exists
		sprintf(errNum,"%d",errno);
    	char* errorTxt=strerror(errno);
    	char* error=(char*)malloc(1024);
    	strcpy(error,"Error number ");
    	strcat(error,errNum);
    	strcat(error,": ");
    	strcat(error,errorTxt);
    	return error;
	}
	return "Working path changed";
}
