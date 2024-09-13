#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
using namespace std;

bool isNumeric(const std::string& str) {
	if (str.empty()) return false;

	for (char c : str) {
		if (!isdigit(c)) {
			return false;
		}
	}
	return true;
}

class Seat {
public:
	string name;
	string price;
	string passenger;
	bool isBooked;
	int ID;

	Seat() : name(""), price(""), passenger(""), isBooked(false), ID(0) {}
	Seat(string n, string p) : name(n), price(p), passenger(""), isBooked(false), ID(0) {} // by default it creates a seat, which is not booked

	void bookSeat(string& passengerName, int seatID) {
		passenger = passengerName;
		isBooked = true;
		ID = seatID;
	}
};

class Flight {
public:
	string flightDate;
	string flightNumber;
	vector<Seat> seats;

	Flight(string d, string fN, vector<Seat> s) : flightDate(d), flightNumber(fN), seats(s) {}
};

class System {
public:
	vector<Flight> flights;
	void run();
private:
	string input;
	string command;
	string forView;
	string systemFlightNumber;
	string systemFlightDate;
	string ID;
	string seat;
	string username;
	int currentTicketID = 1;
	int numbOfOutputs;
	void readFile();
	void printCommands();
	void clearData();
	void check();
	void book();
	void returnTicket();
	void viewUsername();
	void viewFlight();
	void viewID();
};

void System::printCommands() {
	cout << "Choose a command:\n"
		<< "check - to check available places for the flight\n"
		<< "book - to buy a ticket for the flight\n"
		<< "return - return ticket with refund\n"
		<< "view - view the booking confirmation info by entering ID\n"
		<< "view username - view all booked tickets for a particular user\n"
		<< "view flight - view all booked tickets for the particular flight\n"
		<< "exit - to leave\n\n";
}

void System::readFile() {
	int seatsPerRow;
	int numberOfRecords;
	string line;
	vector<Seat> seats;
	map<string, string> rowsPriceMap;

	ifstream inputFile("Text.txt");

	inputFile >> numberOfRecords;
	getline(inputFile, line); // to skip first line


	for (int i = 0; i < numberOfRecords; i++)
	{
		// here I split each line by space
		getline(inputFile, line);
		stringstream ss(line);
		ss >> systemFlightDate >> systemFlightNumber >> seatsPerRow;

		string rowRange, rowPrice;
		while (ss >> rowRange >> rowPrice) {
			rowsPriceMap[rowRange] = rowPrice;
		}

		//here I separate two numbers by hyphen between them and convert them to int
		string range;
		string price;

		for (const auto& entry : rowsPriceMap) {
			range = entry.first;
			price = entry.second;
			size_t hyphenPos = range.find('-');

			string firstNumberStr = range.substr(0, hyphenPos);
			string secondNumberStr = range.substr(hyphenPos + 1);

			int firstNumber = stoi(firstNumberStr);
			int secondNumber = stoi(secondNumberStr);

			//here I create seat objects with corresponding prices
			char maxCol = 'A' + seatsPerRow - 1;
			for (int curRow = firstNumber; curRow <= secondNumber; curRow++) {
				for (char col = 'A'; col <= maxCol; col++)
				{
					string seatName = to_string(curRow) + col;
					Seat seat(seatName, price);
					seats.push_back(seat);
				}
			}
		}
		// here I create a flight object and add it to the list of flights
		Flight flight(systemFlightDate, systemFlightNumber, seats);
		flights.push_back(flight);
	}
}

void System::run() {
	readFile();
	while (true)
	{
		clearData();
		printCommands();
		getline(cin, input);
		stringstream sss(input);
		sss >> command;
		if (command == "check")
		{
			sss >> systemFlightDate >> systemFlightNumber;
			check();
		}
		else if (command == "book")
		{
			sss >> systemFlightDate >> systemFlightNumber >> seat >> username;
			book();
		}
		else if (command == "return")
		{
			sss >> ID;
			returnTicket();
		}
		else if (command == "view")
		{
			sss >> forView;
			if (forView == "username")
			{
				sss >> username;
				viewUsername();
			}
			else if (forView == "flight")
			{
				numbOfOutputs = 0;
				sss >> systemFlightDate >> systemFlightNumber;
				viewFlight();
			}
			else
			{
				viewID();
			}
		}
		else if (command == "exit") {
			return;
		}
		else {
			cout << "\nThe input is incorrect!\n\n";
		}
	}
}

void System::check() {
	numbOfOutputs = 0;
	cout << endl;
	for (int i = 0; i < flights.size(); i++) {
		if (flights[i].flightDate == systemFlightDate && flights[i].flightNumber == systemFlightNumber) {
			for (int j = 0; j < flights[i].seats.size(); j++) {
				if (!flights[i].seats[j].isBooked) {
					cout << flights[i].seats[j].name << " " << flights[i].seats[j].price << ", ";
					numbOfOutputs++;
				}
			}
			if (numbOfOutputs == 0) {
				cout << "All the seats are booked\n\n";
			}
			cout << "\n\n";
			break;
		}
		else {
			cout << "There is no such flight or the input is incorrect!\n\n";
			break;
		}
		systemFlightDate.clear(); // do I need this?
		systemFlightNumber.clear();
	}
}

void System::book() {
	for (int i = 0; i < flights.size(); i++) {
		if (flights[i].flightDate == systemFlightDate && flights[i].flightNumber == systemFlightNumber) {
			for (int j = 0; j < flights[i].seats.size(); j++) {
				if (flights[i].seats[j].name == seat && !flights[i].seats[j].isBooked) {
					flights[i].seats[j].bookSeat(username, currentTicketID);
					cout << "\nConfirmed with ID " << currentTicketID << "\n\n";
					currentTicketID++;
					systemFlightDate.clear(); // do I need this?
					systemFlightNumber.clear();
					i = flights.size();
					break;
				}
			}
		}
		else {
			cout << "\nThere is no such flight or the input is incorrect!\n\n";
			break;
		}
	}
}

void System::returnTicket() {
	int intID = -1;

	if (isNumeric(ID)) {
		intID = stoi(ID);
	}

	for (int i = 0; i < flights.size(); i++) {
		for (int j = 0; j < flights[i].seats.size(); j++) {
			if (flights[i].seats[j].ID == intID) {
				cout << "\nConfirmed " << flights[i].seats[j].price << " refund for " << flights[i].seats[j].passenger << "\n\n";
				flights[i].seats[j].ID = 0;
				flights[i].seats[j].passenger = "";
				flights[i].seats[j].isBooked = false;
				numbOfOutputs++;
				break;
			}
		}
	}
	if (numbOfOutputs == 0) {
		cout << "\nThere is no such ID!\n\n";
	}
}

void System::viewUsername() {
	int rowNumb = 1;

	for (int i = 0; i < flights.size(); i++) {
		for (int j = 0; j < flights[i].seats.size(); j++) {
			if (flights[i].seats[j].passenger == username && !username.empty()) {
				cout << "\n" << rowNumb << ". Flight " << flights[i].flightNumber << ", " << flights[i].flightDate << ", seat " << flights[i].seats[j].name <<
					", price " << flights[i].seats[j].price << "\n\n";
				rowNumb++;
			}
		}
	}
	if (rowNumb == 1) {
		cout << "\nThere is no such passenger or the passenger name is incorrect!\n\n";
	}
}

void System::viewFlight() {
	for (int i = 0; i < flights.size(); i++) {
		if (flights[i].flightDate == systemFlightDate && flights[i].flightNumber == systemFlightNumber) {
			for (int j = 0; j < flights[i].seats.size(); j++) {
				if (flights[i].seats[j].isBooked) {
					cout << "\n" << flights[i].seats[j].name << " " << flights[i].seats[j].passenger << " " << flights[i].seats[j].price << "\n\n";
					numbOfOutputs++;
				}
			}
			if (numbOfOutputs == 0) {
				cout << "\nThere are no booked seats on this flight!\n\n";
			}
			break;
		}
		else { if (i == flights.size() - 1) { cout << "\nThere is no such flight or the input is incorrect!\n\n"; } }
	}
}

void System::viewID() {
	numbOfOutputs = 0;
	ID = forView;

	if (!isNumeric(ID)) {
		cout << "\nThe input is incorrect!\n\n";
	}
	else {
		int intID = stoi(ID);

		for (int i = 0; i < flights.size(); i++) {
			for (int j = 0; j < flights[i].seats.size(); j++) {
				if (flights[i].seats[j].ID == intID) {
					cout << "\nFlight " << flights[i].flightNumber << ", " << flights[i].flightDate << ", seat " << flights[i].seats[j].name <<
						", price " << flights[i].seats[j].price << ", " << flights[i].seats[j].passenger << "\n\n";
					numbOfOutputs++;
					break;
				}
			}
		}
		if (numbOfOutputs == 0) {
			cout << "\nThere is no such ticket ID or the input is incorrect!\n\n";
		}
	}
}


int main() {
	System system;
	system.run();
}