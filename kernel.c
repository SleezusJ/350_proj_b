
void printChar(char c);
void printString(char* chars);

int main(){

		
	printString("Hello, World!");
	while(1);/*boucle infini*/

}

void printChar(char c){
	interrupt(0x10,0xe*256+c,0,0,0);
}

void printString(char* chars){

	//append NULL terminator to char array to make it a string
	int len = sizeof(chars) / sizeof(char);
	chars[len+1] = '\0';

	while(*chars != 0x0){
		printChar(*chars);
		chars++;
	}
 

}
