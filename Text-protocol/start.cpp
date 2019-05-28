#include "server.h"
#include "client.h"
#include <iostream>
#include <string>
#include <conio.h>
#pragma comment(lib, "Ws2_32.lib")
using namespace std;

void main() {

	while (1) {
		system("cls");
		cout << "1 - KLIENT\n2 - SERWER\nESC - WYJSCIE";

		switch (_getch()) {
		case '1': {
			system("cls");
			CLIENT c;
			c.link();
			break;
		}
		case '2': {
			system("cls");
			SERVER s;
			s.link();
			break;
		}
		case 27: {
			exit(0);
		}
		}
	}

}

/*
status - h, r, e, f
op = +, -, ^, l, h1, h2
*/