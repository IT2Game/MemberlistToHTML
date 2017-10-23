#include<vector>
#include<string>
#include<iostream>
#include<fstream>
#include<sstream>
#include<iterator>

#define CP_NAME				0b0001
#define CP_ABTEILUNG		0b0010
#define CP_UNTERKATEGORIE	0b0011

template<typename Out>
void split(const std::string &s, char delim, Out result) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		*(result++) = item;
	}
}

std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, std::back_inserter(elems));
	return elems;
}

int main(int argc, char*args[]) {
	std::vector<std::vector<std::string>> data;
	std::ifstream filein;
	std::string line;

	filein.open("members.csv", std::fstream::app);
	if (filein.is_open()) {
		int i{ 0 };
		while (std::getline(filein, line)) {
			std::vector<std::string> x{ split(line, ',') };
			data.push_back(x);
		}
		filein.close();
	}
	else {
		std::wcerr << L"Fehler beim öffnen der Datei!" << std::endl;
	}
	std::vector<std::string> abteilung;
	for (int y = 1; y < data.size(); y++) {
		std::string line = data[y][CP_ABTEILUNG];
		bool exists = false;
		for (int a = 0; a < abteilung.size(); a++) {
			if (abteilung[a] == line)
				exists = true;
		}
		if (!exists)
			abteilung.push_back(line);
	}
	struct unkat {
		std::string abteilung;
		std::string unterkategorie;
	};

	std::vector<unkat> unterkategorie;
	for (int y = 1; y < data.size(); y++) {
		for (int a = 0; a < abteilung.size(); a++) {
			if (abteilung[a] == data[y][CP_ABTEILUNG]) {
				unkat UnterAbteilung;
				UnterAbteilung.abteilung = abteilung[a];
				UnterAbteilung.unterkategorie = data[y][CP_UNTERKATEGORIE];
				bool exsists = false;
				for (int b = 0; b < unterkategorie.size(); b++) {
					if (unterkategorie[b].unterkategorie == data[y][CP_UNTERKATEGORIE] &&
						unterkategorie[b].abteilung == abteilung[a])
						exsists = true;
				}
				if (!exsists) {
					unterkategorie.push_back(UnterAbteilung);
				}
			}
		}
	}

	struct Users {
		int id_abteilung;
		int id_unterkategorie;
		std::string abteilung;
		std::string unterkategorie;
		std::string name;
	};
	std::vector<Users> members;
	for (int y = 1; y < data.size(); y++) {
		for (int a = 0; a < abteilung.size(); a++) {
			if (abteilung[a] == data[y][CP_ABTEILUNG]) {
				for (int x = 0; x < unterkategorie.size(); x++) {
					if (unterkategorie[x].unterkategorie == data[y][CP_UNTERKATEGORIE] && unterkategorie[x].abteilung == abteilung[a]) {
						Users user;
						user.id_abteilung = a;
						user.id_unterkategorie = x;
						user.abteilung = abteilung[a];
						user.unterkategorie = unterkategorie[x].unterkategorie;
						user.name = data[y][CP_NAME];

						bool exsists = false;
						for (int n = 0; n < members.size(); n++) {
							if (unterkategorie[x].unterkategorie == members[n].unterkategorie &&
								abteilung[a] == members[n].abteilung &&
								members[n].name == data[y][CP_NAME]) {
								exsists = true;
							}
						}
						if (!exsists) {
							members.push_back(user);
						}
					}
				}
			}
		}
	}

	

	bool uchanging = true;
	while (uchanging) {
		uchanging = false;
		for (int i = 0; i < members.size() - 1; i++) {
			if (members[i].id_unterkategorie < members[i + 1].id_unterkategorie){
				Users tmp = members[i];
				members[i] = members[i + 1];
				members[i + 1] = tmp;
				uchanging = true;
			}
		}
	}
	uchanging = true;
	while (uchanging) {
		uchanging = false;
		for (int i = 0; i < members.size() - 1; i++) {
			if (members[i].id_abteilung < members[i + 1].id_abteilung) {
				Users tmp = members[i];
				members[i] = members[i + 1];
				members[i + 1] = tmp;
				uchanging = true;
			}
		}
	}

	
	std::string lastAbteilung = "";
	std::string lastUnterkategorie = "";
	std::ofstream ofile("members.html", std::fstream::ate);
	if (ofile.is_open()) {
		ofile
			<< "<!DOCTYPE html>\n"
			<< "<html>\n"
			<< "<head>\n"
			<< "<style> body{"
			<< " background-color: black; color: white;}"
			<< "</style>"
			<< "</head>\n";
		ofile << "<body>\n";
		ofile << "<center>\n";
		for (int i = members.size(); i > 0; i--) {
			if (lastAbteilung != members[i].abteilung) {
				lastAbteilung = members[i].abteilung;
				lastUnterkategorie = members[i].unterkategorie;
				std::cout << members[i].abteilung << std::endl;
				ofile << "\t<h1>" << lastAbteilung << "</h1>" << std::endl;
				std::cout << "\t" << lastUnterkategorie << std::endl;
				ofile << "\t<h3>" << lastUnterkategorie << "</h3>" << std::endl;
			}
			else if (lastUnterkategorie != members[i].unterkategorie) {
				lastUnterkategorie = members[i].unterkategorie;
				std::cout << "\t" << lastUnterkategorie << std::endl;
				ofile << "\t<h3>" << lastUnterkategorie << "</h3>" << std::endl;
			}
			std::cout << "\t\t" << members[i].name << std::endl;
			ofile << "\t<p>" << members[i].name << "</p>" << std::endl;
		}
		ofile << "</center>\n";
		ofile << "</body>\n";
		ofile << "</html>\n";
		ofile.close();
	}
	else
		std::cerr << "can't open file" << std::endl;
	std::cout << std::endl;
	

	int end{ std::cin.get() };
	return 0;
}