#include "DataReceiver.h"

int main() {
	DataReceiver * rc = new DataReceiver("text/constellation.IN");
	rc->extractData();
	getchar();
	return 0;
}

