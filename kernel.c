
void printChar(char c);
void printString(char* chars);
char* readString(char* line);
void readSector(char* buffer, int sector);



int main(){

		
	char line[80];
	char buffer[512];
	printString("Enter a line: ");
	readString(line);
	printString(line);

	readSector(buffer,30);
	printString(buffer);


	while(1);/*boucle infini*/

}

void printChar(char c){
	interrupt(0x10,0xe*256+c,0,0,0);
}

void printString(char* chars){  

	while(*chars != 0x0){ //while element is NOT NULL, iterate.
		interrupt(0x10,0xe*256+*chars,0,0,0); //interrupt to print char
		chars++;
	}
 
}

char* readString(char* line){
	char new; //define a new character container
	do{

		new = interrupt(0x16,0,0,0,0); // interrupt to get character
		
		if(new == 0x08 && *line !=  0){//---------------------
			interrupt(0x10,0xe*256+0x08,0,0,0);//print bkspc
			line--; //step back in array
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
	*line = 0xd;
	line++;
	*line = 0x0; //append NULL
	interrupt(0x10,0xe*256+0xa,0,0,0);
	interrupt(0x10,0xe*256+0xd,0,0,0);

	return line;
}

	
void readSector(char* buffer, int sector){
	int AH = 2; //read
	int AL = 1; //number of sectors to read
	//pass buffer to BX
	int CH = 0;//trackNum
	int CL = sector + 1; //relative Sector
	int DH = 0; //head number
	int DL = 0x80; //device number

	interrupt(0x13,AH*256+AL,buffer,CH*256+CL,DH*256+DL);

}
