
void printChar(char c);
void printString(char* chars);
char* readString(char* line);

int main(){

		
	char line[80];
	printString("Enter a line: ");
	readString(line);
	printString(line);

	while(1);/*boucle infini*/

}

void printChar(char c){
	interrupt(0x10,0xe*256+c,0,0,0);
}

void printString(char* chars){  


	while(*chars != 0x0){
		interrupt(0x10,0xe*256+*chars,0,0,0);
		chars++;
	}
 

}

char* readString(char line[]){
	char new;
	do{

		new = interrupt(0x16,0,0,0,0);
		
		if(new == 0x08 && *line != 0){//---------------------
			interrupt(0x10,0xe*256+0x08,0,0,0);//print bkspc
			line--;
			interrupt(0x10,0xe*256+' ',0,0,0);//print space over bkspc 
			interrupt(0x10,0xe*256+0x08,0,0,0);//second backspace??
				
		}
		else{
		*line = new;
		interrupt(0x10,0xe*256+*line,0,0,0);
		line++;
		}

	}while(new != 0xd);


	*line = 0xa; //line return
	line++;
	*line = 0x10;
	line++;
	*line = 0x0; //append NULL

	return line;
}

	

