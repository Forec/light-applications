#include "scanner.h"

int main(int argc, char * argv[]){
	std::string IP = "10.201.14.169";
	Scanner *scanner = new Scanner(IP, 300);
	if (scanner->scan(0, 65535))
		scanner->printOpenPorts();
	else
		std::cout << "ERROR SCANNING" << std::endl;
	delete scanner;
	system("pause");
	return 0;
}