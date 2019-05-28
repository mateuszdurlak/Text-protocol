#pragma once
#include "enum.h"
#include <iostream>
#include <vector>
#include <conio.h>
#include <ctime>
#include <string>
#include <WS2tcpip.h>
#include <WinSock2.h>
#include <cmath>
#include <math.h>
using namespace std;

class SERVER {
	WSADATA wsaData;
	WORD DllVersion = MAKEWORD(2, 2);
	SOCKADDR_IN server, client;
	SOCKET socListen, socClient;

	//nag³ówek: SE=xx$ID=xx$ST=x$OP=x$TI=xxxxxx$A1=x$A2=x$RE=x$
	vector<char> header;
	char h[46];

	//pola
	int session = 0, id = 0;
	status stat;
	operation op;
	string date;
	double A, B, C;
	double wynik;
	double jazda;
	string hist;

	//historia
	struct data {
		string i;
		string d;
	};
	vector<vector<data>> history;

public:
	//funkcje
	void link() {
		WSAStartup(DllVersion, &wsaData);
		inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);
		server.sin_port = htons(37220);
		server.sin_family = AF_INET;
		int addSize = sizeof(sockaddr_in);

		socListen = socket(AF_INET, SOCK_STREAM, 0);
		bind(socListen, (SOCKADDR*)&server, addSize);
		listen(socListen, SOMAXCONN);

		while (1) {
			system("cls");
			cout << "1 - SZUKAJ POLACZENIA\nESC - WYJDZ";
			char i = _getch();
			//obs³uga klienta
			if (i == '1') {
				while (1) {
					system("cls");
					cout << "Trwa proba nawiazania polaczenia...";
					if (socClient = accept(socListen, (SOCKADDR*)&server, &addSize)) {
						//test();
						system("cls");
						cout << "Nawiazano polaczenie" << endl;
						session++;
						id = 0;
						vector<data> v;
						history.push_back(v);
						stat = HEL;
						sendf();
						main();
						receive();
						if (stat == FIN) {
							//id = 0;
							shutdown(socClient, 2);
						}
					}
					break;
				}
			}
			//historia



			//wyjœcie
			if (i == 27) {
				break;
			}
		}



	}
	void main() {
		while (1) {
			while (receive()) {
				if (stat == RES || stat == FIN) {
					if (stat == FIN) {
						cout << "Koniec zapytan klienta" << endl;
					}
					if (op == ADD) {
						id++;
						if (add()) { stat = RES; }
						else error();
						string i = to_string(session) + to_string(id);
						string d = "ID: " + i + " Operacja: " + to_string(A) + " D " + to_string(B) + " = ";
						if (stat == ERR) {
							d += "Blad (Przekroczenie zakresu)";
						}
						else {
							d += to_string(C);
						}
						data dat;
						dat.i = i;
						dat.d = d;
						history[session - 1].push_back(dat);
					}
					if (op == SUB) {
						id++;
						if (subtract()) { stat = RES; }
						else error();
						string i = to_string(session) + to_string(id);
						string d = "ID: " + i + " Operacja: " + to_string(A) + " - " + to_string(B) + " = ";
						if (stat == ERR) {
							d += "Blad (Przekroczenie zakresu)";
						}
						else {
							d += to_string(C);
						}
						data dat;
						dat.i = i;
						dat.d = d;
						history[session - 1].push_back(dat);
					}
					if (op == POW) {
						id++;
						if (power()) { stat = RES; }
						else error();
						string i = to_string(session) + to_string(id);
						string d = "ID: " + i + " Operacja: " + to_string(A) + " ^ " + to_string(B) + " = ";
						if (stat == ERR) {
							d += "Blad (Przekroczenie zakresu)";
						}
						else {
							d += to_string(C);
						}
						data dat;
						dat.i = i;
						dat.d = d;
						history[session - 1].push_back(dat);
					}
					if (op == LOG) {
						id++;
						if (logarithm()) { stat = RES; }
						else error();
						string i = to_string(session) + to_string(id);
						string d = "ID: " + i + " Operacja: " + to_string(B) + " log " + to_string(A) + " = ";
						if (stat == ERR) {
							d += "Blad (Przekroczenie zakresu)";
						}
						else {
							d += to_string(C);
						}
						data dat;
						dat.i = i;
						dat.d = d;
						history[session - 1].push_back(dat);
					}
					if (op == DZIEL) {
						id++;
						if (dziel()) { stat = RES; }
						else error();
						string i = to_string(session) + to_string(id);
						string d = "ID: " + i + " Operacja: " + to_string(B) + "/" + to_string(A) + " = ";
						if (stat == ERR) {
							d += "Blad (Przekroczenie zakresu)";
						}
						else {
							d += to_string(C);
						}
						data dat;
						dat.i = i;
						dat.d = d;
						history[session - 1].push_back(dat);
					}
					if (op == MNOZ) {
						id++;
						if (mnoz()) { stat = RES; }
						else error();
						string i = to_string(session) + to_string(id);
						string d = "ID: " + i + " Operacja: " + to_string(B) + "*" + to_string(A) + " = ";
						if (stat == ERR) {
							d += "Blad (Przekroczenie zakresu)";
						}
						else {
							d += to_string(C);
						}
						data dat;
						dat.i = i;
						dat.d = d;
						history[session - 1].push_back(dat);
					}
					if (op == PIERW) {
						id++;
						if (pierw()) { stat = RES; }
						else error();
						string i = to_string(session) + to_string(id);
						string d = "ID: " + i + " Operacja: " + to_string(B) + " pierw " + to_string(A) + " = ";
						if (stat == ERR) {
							d += "Blad (Przekroczenie zakresu)";
						}
						else {
							d += to_string(C);
						}
						data dat;
						dat.i = i;
						dat.d = d;
						history[session - 1].push_back(dat);
					}
					if (op == MODUL) {
						id++;
						if (modulo()) { stat = RES; }
						else error();
						string i = to_string(session) + to_string(id);
						string d = "ID: " + i + " Operacja: " + to_string(B) + " mod " + to_string(A) + " = ";
						if (stat == ERR) {
							d += "Blad (Przekroczenie zakresu)";
						}
						else {
							d += to_string(C);
						}
						data dat;
						dat.i = i;
						dat.d = d;
						history[session - 1].push_back(dat);
					}
					if (op == SIL) {
						id++;
						if (silnia()) { stat = RES; }
						else error();
						string i = to_string(session) + to_string(id);
						string d = "ID: " + i + " Operacja: " + to_string(B) + " sil " + to_string(A) + " = ";
						if (stat == ERR) {
							d += "Blad (Przekroczenie zakresu)";

						}
						else {
							d += to_string(C);
						}
						data dat;
						dat.i = i;
						dat.d = d;
						history[session - 1].push_back(dat);
					}

					sendf();
				}
			}
			break;
		}
	}

	void sendf() {
		pack();
		char h[512];
		for (int i = 0; i < header.size(); i++) {
			h[i] = header[i];
		}
		send(socClient, h, header.size(), 0);
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
		if (recv(socClient, h, 512, 0)) {
			int c = 0;
			for (int i = 0; i < 512; i++) {
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
		cout << "Klient zakonczyl polaczenie" << endl;
		return 0;
	}
	void parse() {
		//podzia³
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
		stat = ERR;
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

	//dzia³ania
	bool add() {
		C = A + B;
		if (range(C) == 0) {
			return 0;
		}
		return 1;
	}
	bool subtract() {
		C = A - B;
		if (range(C) == 0) {
			return 0;
		}
		return 1;
	}
	bool power() {
		C = pow(A, B);
		if (range(C) == 0) {
			return 0;
		}
		return 1;
	}
	bool logarithm() {
		C = log(B) / log(A);
		if (range(C) == 0) {
			return 0;
		}
		return 1;
	}
	bool dziel() {
		C = A / B;
		if (range(C) == 0) {
			return 0;
		}
		return 1;
	}

	bool mnoz() {
		C = A * B;
		if (range(C) == 0) {
			return 0;
		}
		return 1;
	}

	bool modulo() {
		C = (int)A % (int)B;
		if (range(C) == 0) {
			return 0;
		}
		return 1;
	}
	bool pierw() {
		C = sqrt(A);
		if (range(C) == 0) {
			return 0;
		}
		return 1;
	}

	bool silnia() {
		C = 1;
		for (double i = 1; i <= A; i++) {

			C = C * i;
		}

		if (range(C) == 0) {
			return 0;
		}
		return 1;
	}

};