#pragma once
#include "enum.h"
#include <iostream>
#include <vector>
#include <conio.h>
#include <ctime>
#include <string>
#include <WS2tcpip.h>
#include <WinSock2.h>

using namespace std;

class CLIENT {
	WSADATA wsaData;
	WORD DllVersion = MAKEWORD(2, 2);
	SOCKADDR_IN address;
	SOCKET soc;

	//nagłowek: SE=xx$ID=xx$ST=x$OP=x$TI=xxxxxx$A1=x$A2=x$RE=x$
	vector<char> header;

	//pola
	int session = 0, id = 0;
	status stat;
	operation op;
	string date;
	double A, B, C;
	string hist;

public:
	//funkcje
	void link() {
		WSAStartup(DllVersion, &wsaData);

		inet_pton(AF_INET, "127.0.0.1", &address.sin_addr);
		address.sin_port = htons(37220);
		address.sin_family = AF_INET;

		int addSize = sizeof(sockaddr_in);
		soc = socket(AF_INET, SOCK_STREAM, 0);

		while (1) {
			cout << "Trwa proba nawiazania polaczenia..." << endl;
			if (!connect(soc, (SOCKADDR*)&address, addSize)) {
				system("cls");
				cout << "Oczekiwanie na serwer" << endl;
				//test();
				receive();
				main();
				finish();
				cout << "Zakonczono polaczenie." << endl;
				break;
			}
			else
				cout << "Nie mozna nawiazac polaczenia" << endl;
		}
	}
	void main() {

		while (1) {
			system("cls");
			cout << "Nawiazano polaczenie" << endl;
			cout << "1 - OPERACJE\nESC - WYJDZ\n";
			char i = _getch();
			//opercaje
			if (i == '1') {
				//system("cls");
				while (1) {
					cout << "D - DODAWANIE\n- - ODEJMOWANIE\n^ - POTEGOWANIE\nl - LOGARYTMOWANIE\n * - MNOZENIE\n / - DZIELENIE\n % - MODULO\n v - PIERWIASTEK\n s - SILNIA\nESC - WYJDZ" << endl;
					char j = _getch();
					double a, b;
					if (j == 'D') {
						op = ADD;
						cout << "Dodawanie\nPodaj pierwsza liczbe: ";
						cin >> a;
						cout << "Podaj druga liczbe ";
						cin >> b;
					}
					if (j == '-') {
						op = SUB;
						cout << "Odejmowanie\nPodaj pierwsza liczbe: ";
						cin >> a;
						cout << "Podaj druga liczbe: ";
						cin >> b;
					}
					if (j == '^') {
						op = POW;
						cout << "Potegowanie\nPodaj podstawe: ";
						cin >> a;
						cout << "Podaj potege: ";
						cin >> b;
					}
					if (j == 'l') {
						op = LOG;
						cout << "Logarytmowanie\n";
						while (1) {
							cout << "Podaj podstawe: ";
							cin >> a;
							if (a <= 0 || a == 1) {
								cout << "Podstawa logarytmu musi sie miescic w dziedzinie: D=(0;1)(1;∞)\n";
							}
							else {
								break;
							}
						}
						while (1) {
							cout << "Podaj liczbe logarytmowana: ";
							cin >> b;
							if (b <= 0) {
								cout << "Liczba logarytmowana musi byc dodatnia!\n";
							}
							else {
								break;
							}
						}
					}
					if (j == '/') {
						op = DZIEL;
						cout << "Dzielenie\nPodaj A: ";
						cin >> a;
						cout << "Podaj B: ";
						cin >> b;
					}
					if (j == '*') {
						op = MNOZ;
						cout << "MNOZENIE\nPodaj A: ";
						cin >> a;
						cout << "Podaj B: ";
						cin >> b;
					}
					if (j == '%') {
						op = MODUL;
						cout << "MODULO\nPodaj A: ";
						cin >> a;
						cout << "Podaj B: ";
						cin >> b;
					}
					if (j == 'v') {
						op = PIERW;
						cout << "PIERWIASTKOWANIE\nPodaj A: ";
						cin >> a;
						cout << "Podaj B: ";
						cin >> b;
					}
					if (j == 's') {
						op = SIL;
						cout << "SILNIA\nPodaj A: ";
						cin >> a;
						b = 0;
					}
					if (j == 27) {
						break;
					}
					if (j != 'D' && j != '-' && j != '^' && j != 'l'&& j != '*'&& j != '/' && j != 'v'&& j != '%'&& j != 's'&& j != 27) {
						continue;
					}
					if (range(a) == 0 || range(b) == 0) {
						cout << "Przekroczenie zakresu!" << endl;
					}
					else {
						A = a;
						B = b;
						stat = RES;
						sendf();
						receive();
						if (stat == ERR) {
							cout << "Przekroczenie zakresu (serwer)" << endl;
							cout << "\n" << C << "\n" << A << "\n" << B;
						}
						if (stat == RES) { result(); }
					}

				}
			}
			//historia

			//wyjście
			if (i == 27) {
				break;
			}
		}
	}

	void sendf() {
		pack();
		char h[512];
		for (int i = 0; i < header.size(); i++) {
			h[i] = header[i];
		}
		send(soc, h, header.size(), 0);
	}
	void pack() {
		char s, o;
		if (stat == HEL) {
			s = 'h';
		}if (stat == RES) {
			s = 'r';
		}if (stat == ERR) {
			s = 'e';
		}if (stat == FIN) {
			s = 'f';
		}

		if (op == ADD) {
			o = 'D';
		}if (op == SUB) {
			o = '-';
		}if (op == POW) {
			o = '^';
		}if (op == LOG) {
			o = 'l';
		}if (op == HIS1) {
			o = 'h';
		}if (op == HIS2) {
			o = 'a';
		}
		if (op == DZIEL) {
			o = '/';
		}
		if (op == MNOZ) {
			o = '*';
		}
		if (op == MODUL) {
			o = '%';
		}
		if (op == PIERW) {
			o = 'v';
		}
		if (op == SIL) {
			o = 's';
		}
		header.resize(0);
		getTime();
		string h;
		h = "#1:E=" + to_string(session) + "+#2:I=" + to_string(id) + "+#3:S=" + s + "+#4:O=" + o
			+ "+#5:T=" + date + "+#6:A=" + to_string(A) + "+#7:B=" + to_string(B) + "+#8:C=";


		h += to_string(C);

		h += "+";
		for (int i = 0; i < h.size(); i++) {
			header.push_back(h[i]);
		}
	}
	bool receive() {
		header.resize(0);
		char h[512];
		recv(soc, h, 512, 0);
		int c = 0;
		for (int i = 0; i < 100; i++) {
			if (h[i] == '&') {
				c++;
				if (c == 8) {
					break;
				}
			}
			header.push_back(h[i]);
		}
		parse();
		return 1;
	}
	void parse() {
		//podział
		vector<string> v;
		vector<char>::iterator it;
		string temp;
		for (it = header.begin(); it != header.end(); it++) {
			if (*it == '+') {
				v.push_back(temp);
				temp.erase();
			}
			else
				temp.push_back(*it);
		}
		//korekta
		for (int i = 0; i < v.size(); i++) {
			string temp = v[i];
			string temp2 = "";
			int x = temp.size();
			for (int j = 5; j < x; j++) {
				temp2.push_back(temp[j]);
			}
			v[i] = temp2;
		}
		//przyspianie
		session = stoi(v[0]);
		id = stoi(v[1]);
		char s, o;
		s = v[2][0];
		o = v[3][0];
		if (s == 'h') {
			stat = HEL;
		}
		if (s == 'r') {
			stat = RES;
		}
		if (s == 'e') {
			stat = ERR;
		}
		if (s == 'f') {
			stat = FIN;
		}

		if (o == 'D') {
			op = ADD;
		}if (o == '-') {
			op = SUB;
		}if (o == '^') {
			op = POW;
		}if (o == 'l') {
			op = LOG;
		}if (o == 'h') {
			op = HIS1;
		}if (o == 'a') {
			op = HIS2;
		}
		if (o == '/') {
			op = DZIEL;
		}
		if (o == '*') {
			op = MNOZ;
		}
		if (o == 'v') {
			op = PIERW;
		}
		if (o == '%') {
			op = MODUL;
		}
		if (o == 's') {
			op = SIL;
		}
		date = v[4];
		A = stod(v[5]);
		B = stod(v[6]);
		if (op == HIS1 || op == HIS2) {
			hist = v[7];
		}
		else {
			C = stod(v[7]);
		}
	}
	void result() {

		if (op == ADD || op == SUB || op == POW || op == LOG || op == DZIEL || op == MNOZ || op == MODUL || op == PIERW || op == SIL) {
			cout << "Wynik: " << C << endl;
		}
	}
	bool range(double x) {
		string s = to_string(x);
		string r;
		for (int i = 0; i < s.size(); i++) {
			if (s[i] == '.') {
				r.push_back(s[i]);
				r.resize(0);
			}
			else {
				r.push_back(s[i]);
			}
		}
		if (r.length() > 15) {
			return 0;
		}
		return 1;
	}
	void error() {
		cout << "Przekroczono zakres!\n";
		C = 0;
	}
	void finish() {
		stat = FIN;
		if (shutdown(soc, 2) == 0) {
			char h[512];
			for (int i = 0; i < header.size(); i++) {
				h[i] = header[i];
			}
			send(soc, h, header.size(), 0);
		}
		return;
	}
	void getTime() {
		time_t result = time(NULL);
		char c[26];
		ctime_s(c, sizeof c, &result);
		cout << c;
		date = "";
		for (int i = 11; i < 19; i++) {
			date += c[i];
		}
	}

};