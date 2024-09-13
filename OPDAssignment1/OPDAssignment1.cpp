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

int main() {
	System system;
	system.run();
}