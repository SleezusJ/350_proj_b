/* kernel.c -program to serve as OS kernel for comp350
 *-----------------------------------------------------
 *Authors: Kyle Snow, Tom Foley, Shane Driskell
 *
 *last modified: 10/26/19 @ 11:32
 *
 */

void printChar(char c);
void printString(char* chars);
char* readString(char* line);
void readSector(char* buffer, int sector);
void handleInterrupt21(int ax, int bx, int cx, int dx);


int main(){

		
/*	//Test readString & printString()
 	char line[80];
	char buffer[512];
	printString("Enter a line: ");
	readString(line);  
	printString(line);

	//Test readSector()
	readSector(buffer,30);
	printString(buffer);
*/

	//Test handleInterrupt21()
	char line[80];
	printString("Gimme a string:");
	makeInterrupt21();
	interrupt(0x21,1,line,0,0);
	interrupt(0x21,0,line,0,0);


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


void handleInterrupt21(int ax, int bx, int cx, int dx){
	//printString("Hello World! I am comming at you live from the interrupt!");
	if(ax == 0){ //print string
		printString(bx);
	}else if(ax == 1){ //read string
		readString(bx);
	}else if(ax == 2){ //read sector
		readSector(bx,cx);
	}else{
		printString("ERROR");
	}
}






