//
//  Created by Alexandra Härnström on 2021-08-05.
//


#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
using namespace std;


class Transaktion {
private:
    string datum;
    string typ;
    string namn;
    double belopp;
    int    antal_kompisar;
    string* kompisar;

public:
    Transaktion();
    ~Transaktion();
    Transaktion& operator=(const Transaktion& t);
    string hamtaNamn() const;
    double hamtaBelopp() const;
    int    hamtaAntalKompisar() const;
    bool   finnsKompis(const string &namnet) const;
    bool   lasIn(istream& is);
    void   skrivUt(ostream& os) const;
    void   skrivUt(string filnamn) const;
    void   skrivTitel(ostream& os) const;
};


class Person {
private:
    string namn;
    double betalat_andra;
    double skyldig;

public:
    Person();
    Person(const string &namn, double betalat_andra, double skyldig);
    string getNamn();
    double hamtaBetalat();
    double hamtaSkyldig();
    void   skrivUt(ostream &os);
};


class PersonLista {
private:
    int    antal_personer;
    Person *personer;

public:
    PersonLista();
    ~PersonLista();
    PersonLista& operator=(const PersonLista& p);
    void   laggTill(Person ny_person);
    void   skrivUtOchFixa(ostream &os);
    double summaSkyldig();
    double summaBetalat();
    bool   finnsPerson(const string &namn);
};


class TransaktionsLista {
private:
    int         antal_transaktioner;
    Transaktion *transaktioner;

public:
    TransaktionsLista();
    ~TransaktionsLista();
    TransaktionsLista& operator=(const TransaktionsLista& tl);
    void   lasIn(istream &is);
    void   skrivUt(ostream &os);
    void   laggTill(Transaktion &transaktion);
    double totalKostnad();
    double liggerUteMed(const string &namnet);
    double arSkyldig(const string &namnet);
    PersonLista FixaPersoner();

 };


int main() {
    
    cout << "Startar med att läsa från en fil." << endl;

    TransaktionsLista transaktioner;

    // Skapa ett objekt av klassen ifstream och namnge en fil
    ifstream is("resa.txt");

    // Läs in alla objekt från fil
    while (!is.eof()) {
        transaktioner.lasIn(is);
    }
    
    int operation = 1;
    
    while (operation != 0) {
        cout << endl;
        cout << "Välj i menyn nedan:" << endl;
        cout << "0. Avsluta. Alla transaktioner sparas på fil." << endl;
        cout << "1. Skriv ut information om alla transaktioner." << endl;
        cout << "2. Läs in en transaktion från tangentbordet." << endl;
        cout << "3. Beräkna totala kostnaden." << endl;
        cout << "4. Hur mycket är en viss person skyldig?" << endl;
        cout << "5. Hur mycket ligger en viss person ute med?" << endl;
        cout << "6. Lista alla personer mm och FIXA" << endl;

        cin >> operation;
        cout << endl;
        
        switch (operation) {
                
            case 1: {
                // Skriv ut info till terminal
                cin.ignore();
                transaktioner.skrivUt(cout);
                break;
            }
                
            case 2: {
                // Läs in ett nytt objekt från tangentbord
                cin.ignore();
                Transaktion transaktion;
                cout << "Ange transaktion i följande format: " << endl;
                transaktion.skrivTitel(cout);
                transaktion.lasIn(cin);
                transaktioner.laggTill(transaktion);
                break;
            }
                
            case 3: {
                // Beräkna total kostnad av alla transaktioner
                cin.ignore();
                cout    << "Den totala kostnaden för resan var: "
                        << transaktioner.totalKostnad() << endl;
                break;
            }
                
            case 4: {
                // Beräknar hur mycket en viss person är skyldig
                cin.ignore();
                string namn;
                cout << "Ange personen:" << endl;
                getline(cin, namn);
                double skyldig = transaktioner.arSkyldig(namn);
                if (skyldig == 0.0) {
                    cout << "Kan inte hitta personen " << namn << endl;
                }
                else {
                    cout << namn << " är skyldig: " << skyldig << endl;
                }
                break;
            }
                
            case 5: {
                // Beräknar hur mycket en viss person ligger ute med
                cin.ignore();
                string namn;
                cout << "Ange personen:" << endl;
                getline(cin, namn);
                double uteMed = transaktioner.liggerUteMed(namn);
                if (uteMed == 0.0) {
                    cout << "Kan inte hitta personen " << namn << endl;
                }
                else {
                    cout << namn << " ligger ute med: " << uteMed << endl;
                }
                break;
            }
                
            case 6: {
                // Skapar en lista av personer och skriver ut vad alla ligger ute med,
                // är skyldiga, och vad de ska ha/få från potten
                cin.ignore();
                cout << "Nu skapar vi en personarray och reder ut det hela!" << endl;
                PersonLista lista = transaktioner.FixaPersoner();
                lista.skrivUtOchFixa(cout);
                break;
            }
        }
    }
    
    // Skapa ett objekt av klassen ofstream och skapa fil 'transaktioner.txt'
    ofstream os("transaktioner.txt");
    // Skriv ut info till fil
    transaktioner.skrivUt(os);
    
    return 0;
}


// Konstruktorer, destruktorer, operatorer

Transaktion::Transaktion() {
    datum = "";
    typ = "";
    namn = "";
    belopp = 0.0;
    antal_kompisar = 0;
    kompisar = NULL;
}

Transaktion::~Transaktion() {
}

Transaktion& Transaktion::operator=(const Transaktion& t) {

    if (this != &t) {
        delete[] kompisar;
        datum          = t.datum;
        typ            = t.typ;
        namn           = t.namn;
        belopp         = t.belopp;
        antal_kompisar = t.antal_kompisar;
        kompisar       = new string[antal_kompisar];

        for (int i = 0; i < antal_kompisar; i++) {
            kompisar[i] = t.kompisar[i];
        }
    }
    return *this;
}

Person::Person() {
    namn = "";
    betalat_andra = 0.0;
    skyldig = 0.0;
}

Person::Person(const string& namn, double betalat_andra, double skyldig) {
    this->namn = namn;
    this->betalat_andra = betalat_andra;
    this->skyldig = skyldig;
}

PersonLista::PersonLista() {
    antal_personer = 0;
    personer = NULL;
}

PersonLista::~PersonLista() {
}

PersonLista& PersonLista::operator=(const PersonLista& p) {

    if (this != &p) {
        delete[] personer;
        antal_personer = p.antal_personer;
        personer       = new Person[antal_personer];

        for (int i = 0; i < antal_personer; i++) {
            personer[i] = p.personer[i];
        }
    }
    return *this;
}

TransaktionsLista::TransaktionsLista() {
    antal_transaktioner = 0;
    transaktioner = NULL;
}

TransaktionsLista::~TransaktionsLista() {
}

TransaktionsLista& TransaktionsLista::operator=(const TransaktionsLista& tl) {

    if (this != &tl) {
        delete[] transaktioner;
        antal_transaktioner = tl.antal_transaktioner;
        transaktioner       = new Transaktion[antal_transaktioner];

        for (int i = 0; i < antal_transaktioner; i++) {
            transaktioner[i] = tl.transaktioner[i];
        }
    }
    return *this;
}


// Funktioner Transaktion

string Transaktion::hamtaNamn() const{
    return namn;
}

double Transaktion::hamtaBelopp() const{
    return belopp;
}

int Transaktion::hamtaAntalKompisar() const{
    return antal_kompisar;
}

bool Transaktion::finnsKompis(const string& namnet) const{
    bool finns = false;

    // Loopa igenom arrayen 'kompisar' och jämför namnen
    // Finns namnet; ändra bool värde till true
    for (int i = 0; i < antal_kompisar; i++) {
        if (kompisar[i] == namnet) {
            finns = true;
            break;
        }
    }
    return finns;
}

bool Transaktion::lasIn(istream& is) {

    // Läs in attribut till en transaktion
    is >> datum;
    is >> typ;
    is >> namn;
    is >> belopp;
    is >> antal_kompisar;
    
    if (kompisar != NULL) {
        delete[] kompisar;
    }
    
    kompisar = new string[antal_kompisar];

    // Lägg till namn på kompisar i arrayen 'kompisar'
    string kompis;
    for (int i = 0; i < antal_kompisar; i++) {
        is >> kompis;
        kompisar[i] = kompis;
    }

    return 1;
}

void Transaktion::skrivUt(ostream& os) const{

    // För snyggare utskrift
    os << fixed << setprecision(2) << setiosflags(ios::left);
    
    // Skriv ut attributen av en transaktion med mellanrum
    os << setw(10) << datum << setw(10) << typ << setw(10) << namn
    << setw(10) << belopp << setw(5) << antal_kompisar;
    
    // Loopa igenom arrayen 'kompisar' och skriv ut dessa med ett komma emellan
    for (int i = 0; i < antal_kompisar; i++) {
        
        if (i + 1 == antal_kompisar) {
            os << kompisar[i];
        }
        else {
            os << kompisar[i] << ", ";
        }
    }
    os << endl;
}

void Transaktion::skrivTitel(ostream& os) const{

    os << fixed << setiosflags(ios::left);
    
    // Skriv ut titlar
    os  << setw(10) << "Datum" << setw(10) << "Typ" << setw(10) << "Namn" << setw(10)
        << "Belopp" << setw(10) << "Antal och lista av kompisar" << endl;
}


// Funktioner Person

string Person::getNamn() {
    return namn;
}

double Person::hamtaBetalat() {
    return betalat_andra;
}

double Person::hamtaSkyldig() {
    return skyldig;
}

void Person::skrivUt(ostream& os) {
    double total = betalat_andra - skyldig;
    
    // Arrangera utskrift av double till två decimaler
    os << fixed << setprecision(2);
    
    // Skriv ut om personen ska ha eller lägga till till potten
    if (total >= 0) {
        os  << namn << " ligger ute med " << betalat_andra << " och är skyldig " << skyldig
        << ". Skall ha " << total << " från potten!" << endl;
    }
    else {
        os  << namn << " ligger ute med " << betalat_andra << " och är skyldig " << skyldig
        << ". Ska lägga till " << -total << " till potten!" << endl;
    }
}


// Funktioner PersonLista

void PersonLista::laggTill(Person ny_person) {

    // Allokerar en ny dynamisk array vid varje anrop av denna metod
    Person *p = new Person[antal_personer + 1];

    // För varje gång denna metod anropas kopieras personer från den gamla arrayen
    // över till den nya arrayen
    for (int i = 0; i < antal_personer; i++) {
        p[i] = personer[i];
    }

    // Lägg till ny person på sista (lediga) index i arrayen 'personer'
    p[antal_personer] = ny_person;
    
    // För varje gång denna metod anropas inkrementeras arrayen 'personer' med +1 index
    antal_personer++;
    
    // Den gamla dynamiska pekaren tas bort inför nytt anrop
    if (personer != NULL) {
        delete[] personer;
    }
    
    // Sätt 'personer' att innehålla nuvarande 'p'
    personer = p;
}

void PersonLista::skrivUtOchFixa(ostream& os) {
    double betalt, skuld;

    // Loopa igenom alla personer i PersonListan 'personer' och hämta deras info
    for (int i = 0; i < antal_personer; i++) {
        personer[i].skrivUt(os);
    }

    betalt = summaBetalat();
    skuld = summaSkyldig();

    if (betalt == skuld) {
        cout << "Det går jämnt ut." << endl;
    }
    else {
        cout << "Det går ej jämnt ut." << endl;
        // cout << "Summa betalt: " << betalt << " Summa skuld: " << skuld << endl;
    }
}

double PersonLista::summaBetalat() {
    double summaBetalat = 0;
    
    // Loopa igenom alla personer och summera vad de betalat
    for (int i = 0; i < antal_personer; i++) {
        summaBetalat += personer[i].hamtaBetalat();
    }
    return summaBetalat;
}

double PersonLista::summaSkyldig() {
    double summaSkyldig = 0;
    
    // Loopa igenom alla personer och summera vad de är skyldiga
    for (int i = 0; i < antal_personer; i++) {
        summaSkyldig += personer[i].hamtaSkyldig();
    }
    return summaSkyldig;
}

bool PersonLista::finnsPerson(const string& namn) {
    bool finns = false;

    // Loopa igenom alla personer och byt bool värde om personen finns
    for (int i = 0; i < antal_personer; i++) {
        if (namn == personer[i].getNamn()) {
            finns = true;
            break;
        }
    }
    return finns;
}


// Funktioner Transaktionslista

void TransaktionsLista::lasIn(istream& is) {
    Transaktion transaktion;

    // Läs in transaktion
    transaktion.lasIn(is);
    
    // Lägg till transaktion i TransaktionsListan
    laggTill(transaktion);
}

void TransaktionsLista::skrivUt(ostream& os) {
    Transaktion transaktion;
    
    // Skriv ut antal transaktioner
    os << "Antal transaktioner: " << antal_transaktioner << endl;
    // Skriv ut titlar
    transaktion.skrivTitel(os);
    
    // Loopa igenom alla transaktioner och skriv ut dem
    for (int i = 0; i < antal_transaktioner; i++) {
        transaktioner[i].skrivUt(os);
    }
}

void TransaktionsLista::laggTill(Transaktion& transaktion) {

    // Allokerar en ny dynamisk array vid varje anrop av denna metod
    Transaktion *tl = new Transaktion[antal_transaktioner + 1];

    // För varje gång denna metod anropas kopieras personer från den gamla arrayen
    // över till den nya arrayen
    for (int i = 0; i < antal_transaktioner; i++) {
        tl[i] = transaktioner[i];
    }

    // Lägg till ny person på sista (lediga) index i arrayen 'personer'
    tl[antal_transaktioner] = transaktion;
    
    // För varje gång denna metod anropas inkrementeras arrayen 'personer' med +1 index
    antal_transaktioner++;
    
    // Den gamla dynamiska pekaren tas bort inför nytt anrop
    if (transaktioner != NULL) {
        delete[] transaktioner;
    }
    
    // Sätt 'personer' att innehålla 'p' via overloaded operator
    transaktioner = tl;
}

double TransaktionsLista::totalKostnad() {
    double kostnad = 0;
    
    // Loopa igenom alla transaktioner och summera beloppen
    for (int i = 0; i < antal_transaktioner; i++) {
        kostnad += transaktioner[i].hamtaBelopp();
    }
    return kostnad;
}

double TransaktionsLista::liggerUteMed(const string &namnet) {
    double uteMed = 0.0;
    
    // Loopa igenom alla transaktioner och beräkna beloppen minus den egna andelen
    for (int i = 0; i < antal_transaktioner; i++) {
        
        if (transaktioner[i].hamtaNamn() == namnet) {
            uteMed += transaktioner[i].hamtaBelopp()
                    - (transaktioner[i].hamtaBelopp() / (transaktioner[i].hamtaAntalKompisar() + 1));
        }
    }
    return uteMed;
}

double TransaktionsLista::arSkyldig(const string &namnet) {
    double skyldig = 0.0;

    // Loopa igenom alla transaktioner och sök efter ett namn i arrayen 'kompisar'
    // Finns namnet; summera andelarna av de belopp som personen är skyldig
    for (int i = 0; i < antal_transaktioner; i++) {

        if (transaktioner[i].finnsKompis(namnet)) {
            skyldig += transaktioner[i].hamtaBelopp() / (transaktioner[i].hamtaAntalKompisar() + 1);
        }
    }
    return skyldig;
}

PersonLista TransaktionsLista::FixaPersoner() {
    PersonLista personLista;
    Person nyPerson;
    double uteMed, skyldig;
    string namn;
    
    // Loopa igenom alla transaktioner
    for (int i = 0; i < antal_transaktioner; i++) {
        
        // Hämta namn på personen som betalat för transaktionen
        namn = transaktioner[i].hamtaNamn();
  
        // Loopa igenom alla index i personLista och se om namnet redan finns
        // Om namnet redan finns, sätt bool värdet till true
        bool finns = personLista.finnsPerson(namn);
        
        // Om personen inte redan finns i personLista, skapa en 'nyPerson'
        // och lägg till i 'personLista'
        if (!finns)
        {
            // Beräknar hur mycket personen ligger ute med
            uteMed = liggerUteMed(namn);

            // Beräknar hur mycket personen är skyldig
            skyldig = arSkyldig(namn);
            
            // Initiera ett nytt objekt 'nyPerson'
            nyPerson = {namn, uteMed, skyldig};

            // Lägg till personen i arrayen 'personLista'
            personLista.laggTill(nyPerson);
        }
    }
    return personLista;
}


