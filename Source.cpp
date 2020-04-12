#include<iostream>
#include<fstream>
#include<string>
using namespace std;
class CReteaException {
	string mesaj;
public:
	CReteaException(const string mesaj) : mesaj(mesaj) {}
	~CReteaException() { mesaj = ""; }
	string getmsj() { return mesaj; }
};
class Retea;
class Calculator {
protected:
	string* ms;
	string nume;
	string IP;
	int nrs;
public:
	virtual string type() { return "Calculator obisnuit"; }
	Calculator(string addnume = "", string addIP = "");
	friend ostream& operator<<(ostream& out, const Calculator& c);
	void listServers(Retea& R);
	string getIP() { return IP; }
	string getname() { return nume; }
	void makeServer(Calculator& c);
	bool isServer(Calculator& c, string s);
	~Calculator();
};
Calculator::Calculator(string addnume, string addIP)
{
	ms = new string[1001];
	nrs = 0;
	IP = addIP;
	nume = addnume;
}
ostream& operator<<(ostream& out, const Calculator& c)
{
	out << "IP:" << " ";
	out << c.IP;
	out << " " << "Nume:" << " ";
	out << c.nume;
	return out;
}
void Calculator::makeServer(Calculator& c)
{
	ms[nrs++] = c.getIP();
}
bool Calculator::isServer(Calculator& c, string s)
{
	for (int i = 0; i < this->nrs; i++)
	{
		if (this->ms[i] == s)
			return true;
	}
	return false;
}
Calculator::~Calculator()
{
	IP.clear();
	nume.clear();
}
class Server : public Calculator {
	int nracces;
	Calculator* accesserv;
public:
	Server(string addnume = "", string addIP = "");
	void permitStation(Calculator& c);
	void listStation();
	string type() override { return "Server"; }
	~Server();
};
Server::Server(string addnume, string addIP) :Calculator(addnume, addIP)
{
	nracces = 0;
	accesserv = new Calculator[1001];
	if (accesserv == NULL)
		throw(CReteaException("Memory not allocated\n"));
}
void Server::permitStation(Calculator& c)
{
	accesserv[nracces++] = c;
	c.makeServer(*this);
}
void Server::listStation()
{
	for (int i = 0; i < nracces; i++)
		cout << accesserv[i] << '\n';
}
Server::~Server()
{
	nracces = 0;
	delete[] accesserv;
}
class Retea : public Calculator {
	Calculator* calc;
	string* tip;
	int nrcalc;
	int nrserv;
public:
	Retea(string addname = "");
	void addStation(Calculator& c);
	void listStations();
	void removeStation(Calculator& c);
	Retea& operator +(Retea& r);
	Retea& operator =(Retea& r);
	~Retea();
	friend class Calculator;
};
Retea::Retea(string addname) : Calculator(addname)
{
	nrcalc = 0;

	calc = new Calculator[2001];
	tip = new string[2001];
	if (calc == NULL || tip == NULL)
		throw(CReteaException("Memory not allocated\n"));
	calc = new Calculator[2001];
}
void Retea::addStation(Calculator& c)
{
	calc[nrcalc++] = c;
	tip[nrcalc - 1] = c.type();
}
void Retea::listStations()
{
	string fisierout;
	cin >> fisierout;
	ofstream g(fisierout);
	for (int i = 0; i < nrcalc; i++)
	{
		g << calc[i];
		g << " Tip:";
		g << tip[i] << '\n';
	}
}void Retea::removeStation(Calculator& c)
{
	int poz = -1;
	for (int i = 0; i < nrcalc; i++)
	{
		if (calc[i].getIP() == c.getIP() && calc[i].getname() == c.getname())
			poz = i;
	}
	if (poz == -1)
		throw(CReteaException("Station was not found\n"));
	for (int i = poz; i < nrcalc - 1; i++)
	{
		calc[i] = calc[i + 1];
		tip[i] = tip[i + 1];
	}
	nrcalc--;
}
Retea& Retea::operator+(Retea& R)
{
	Retea* rsuma = new Retea;
	rsuma->nrcalc = nrcalc + R.nrcalc;
	for (int i = 0; i < nrcalc; i++)
	{
		rsuma->calc[i] = calc[i];
		rsuma->tip[i] = tip[i];
	}
	int j = nrcalc;
	for (int i = 0; i < R.nrcalc; i++)
	{
		rsuma->calc[j + i] = R.calc[i];
		rsuma->tip[j + i] = R.tip[i];
	}
	return *rsuma;
}
Retea& Retea::operator=(Retea& R)
{
	delete[] this->calc;
	delete[] this->tip;
	this->calc = new Calculator[2001];
	this->tip = new string[2001];
	if (this->calc == NULL || this->tip == NULL)
		throw(CReteaException("Memory not allocated\n"));
	this->nrcalc = R.nrcalc;
	for (int i = 0; i < R.nrcalc; i++)
		this->calc[i] = R.calc[i], this->tip[i] = R.tip[i];
	return *this;
}
void Calculator::listServers(Retea& R)
{
	for (int i = 0; i < R.nrcalc; i++)
		if (R.tip[i] == "Server")
		{
			if (this->isServer(R.calc[i],R.calc[i].getIP()))
				cout << R.calc[i] << '\n';
		}
}
Retea::~Retea()
{
	delete[] calc;
	delete[] tip;
}
int main()
{
    /* try {
		Retea  R1("Reteaua 1"); // constructorul primeste numele retelei
		Calculator C1("calculatorul 1", "172.16.100.101");
		Calculator C2("calculatorul 2 ", "172.16.100.102");
		Calculator C3("calculatorul 3 ", "172.16.100.103");
		Calculator C4("calculatorul 4 ", "172.16.100.104");
		Server  S1("serverul 1", "172.16.100.1");
		Server  S2("serverul 2", "172.16.100.2");
		Server  S3("serverul 3", "172.16.100.3");
		R1.addStation(C1); // adauga calculatorul C1 la reteaua R1
		R1.addStation(S1); // adauga calculatorul S1 la reteaua R1
		R1.addStation(C2);
		R1.addStation(C4);
		R1.addStation(S2);

		S1.permitStation(C1);
		S1.permitStation(C1);
		S1.permitStation(C3);

		S2.permitStation(C1);

		R1.listStations(); // afiseaza lista calculatoarelor din retea
		cout << endl;
		C1.listServers(R1); // afiseaza informatiile despre serverele S1 si S2
		cout << endl;
		R1.removeStation(S2);
		C1.listServers(R1); // afiseaza informatiile doar despre serverul S1


	}
	catch (CReteaException & e) {
		e.getmsj();
	}*/
	
	//exemplu (+)
	/*try{
	Calculator c1( "alfa","1"), c2("beta","2"), c3("dorel","3"), c4("gigel","4"), c5("john","5");
	Server s1("sfa","6"), s2("2131as","7"), s3("af3qa","8"), s4("Asdsd","9");
	Retea R;
	//retea1
	R.addStation(c1);
	R.addStation(c2);
	R.addStation(c3);
	R.addStation(s1);
	R.addStation(s2);
	R.listStations();
	//dupa stergere
	R.removeStation(c3);
	R.removeStation(s2);
	R.listStations();
	Retea R2;
	R2.addStation(c4);
	R2.addStation(c5);
	R2.addStation(s3);
	R2.addStation(s4);
	//Retea2
	R2.listStations();
	Retea R3;
	R3 = R + R2;
	//Retea3
	R3.listStations();}
	catch (CReteaException & e) {
		e.getmsj();
	}*/

	return 0;
}
