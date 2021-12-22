#include <iostream.h>
#include <stdlib.h>
#include "system.h"
#include "pcb.h"
#include "semaphor.h"

extern int userMain(int argc, char* argv[]);

int main(int argc, char* argv[]){
	System::init();
	int ret = userMain(argc, argv);
	System::restore();
	cout << "main -> exit";
	return ret;
}
