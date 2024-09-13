#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
using namespace std;


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


int main() {
	System system;
	system.run();
}