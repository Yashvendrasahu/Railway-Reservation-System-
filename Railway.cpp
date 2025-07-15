#include <iostream>
#include <vector>
#include <queue>
#include <iomanip>
using namespace std;

int pnr_counter = 1000;
const int max_waiting = 5;

class Passenger {
public:
    int pnr;
    string name;
    int age;
    int seat_no;
    string status;
    int train_no;

    Passenger(int p, string n, int a, int s, string st, int t)
        : pnr(p), name(n), age(a), seat_no(s), status(st), train_no(t) {}
};

class Train {
public:
    int train_no;
    string train_name;
    int total_seats;
    int booked_seats;
    vector<string> routeStations;
    vector<int> fares;
    vector<string> arrivalTimes;
    Train* next;

    Train(int no, string name, int seats, vector<string> route, vector<int> fareList, vector<string> timeList) {
        train_no = no;
        train_name = name;
        total_seats = seats;
        booked_seats = 0;
        routeStations = route;
        fares = fareList;
        arrivalTimes = timeList;
        next = nullptr;
    }
};

class RailwaySystem {
private:
    Train* head = nullptr;
    vector<Passenger> confirmedPassengers;
    queue<Passenger> waitingList;

public:
    void addTrain(int no, string name, int seats, vector<string> route, vector<int> fareList, vector<string> timeList) {
        Train* newTrain = new Train(no, name, seats, route, fareList, timeList);
        newTrain->next = head;
        head = newTrain;
    }

    Train* findTrain(int train_no) {
        Train* temp = head;
        while (temp) {
            if (temp->train_no == train_no)
                return temp;
            temp = temp->next;
        }
        return nullptr;
    }

    void showAllTrains() {
        Train* temp = head;
        cout << "\n=== Available Trains ===\n";
        while (temp) {
            cout << "Train No: " << temp->train_no
                 << ", Name: " << temp->train_name
                 << ", Total Seats: " << temp->total_seats
                 << ", Booked: " << temp->booked_seats << "\n";
            temp = temp->next;
        }
    }

    void bookTicket() {
        int train_no;
        cout << "Enter Train Number: ";
        cin >> train_no;
        Train* train = findTrain(train_no);
        if (!train) {
            cout << "Train not found!\n";
            return;
        }

        string name;
        int age;
        cout << "Enter passenger name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter age: ";
        cin >> age;

        string source, destination;
        cout << "Enter Source Station (From): ";
        cin.ignore();
        getline(cin, source);
        cout << "Enter Destination Station (To): ";
        getline(cin, destination);

        pnr_counter++;

        if (train->booked_seats < train->total_seats) {
            train->booked_seats++;
            int seat_no = train->booked_seats;
            Passenger p(pnr_counter, name, age, seat_no, "Confirmed", train_no);
            confirmedPassengers.push_back(p);

            cout << "\n===== Ticket Receipt =====\n";
            cout << "PNR: " << p.pnr << "\nName: " << p.name << "\nAge: " << p.age
                 << "\nFrom: " << source << "  To: " << destination
                 << "\nTrain No: " << train->train_no << " (" << train->train_name << ")"
                 << "\nSeat No: " << p.seat_no << "\nStatus: " << p.status << "\n";
            cout << "===========================\n";

        } else {
            if ((int)waitingList.size() < max_waiting) {
                Passenger p(pnr_counter, name, age, 0, "Waiting", train_no);
                waitingList.push(p);

                cout << "\n===== Waiting List Receipt =====\n";
                cout << "PNR: " << p.pnr << "\nName: " << p.name << "\nAge: " << p.age
                     << "\nFrom: " << source << "  To: " << destination
                     << "\nTrain No: " << train->train_no << " (" << train->train_name << ")"
                     << "\nSeat No: Not Allocated\nStatus: " << p.status
                     << "\nWaiting Position: " << waitingList.size() << "\n";
                cout << "===============================\n";
            } else {
                cout << "Waiting list full. Cannot book ticket now.\n";
            }
        }
    }

    void showPassengers() {
        int train_no;
        cout << "Enter Train Number to view passengers: ";
        cin >> train_no;

        Train* train = findTrain(train_no);
        if (!train) {
            cout << "Train not found!\n";
            return;
        }

        cout << "\nPassengers for Train " << train->train_no << " (" << train->train_name << "):\n";
        for (auto& p : confirmedPassengers) {
            if (p.train_no == train_no) {
                cout << "PNR: " << p.pnr << ", Name: " << p.name << ", Age: " << p.age
                     << ", Seat No: " << p.seat_no << ", Status: " << p.status << "\n";
            }
        }

        cout << "\nWaiting List:\n";
        queue<Passenger> temp = waitingList;
        int pos = 1;
        while (!temp.empty()) {
            Passenger p = temp.front();
            temp.pop();
            if (p.train_no == train_no) {
                cout << pos << ". PNR: " << p.pnr << ", Name: " << p.name << ", Age: " << p.age
                     << ", Status: " << p.status << "\n";
                pos++;
            }
        }
    }

    void showTrainRouteAndFare() {
        int train_no;
        cout << "Enter Train Number to view route and fare: ";
        cin >> train_no;

        Train* train = findTrain(train_no);
        if (!train) {
            cout << "Train not found!\n";
            return;
        }

        cout << "\nRoute for Train No: " << train->train_no << " (" << train->train_name << "):\n";
        for (size_t i = 0; i < train->routeStations.size(); ++i) {
            cout << train->routeStations[i] << " (" << train->arrivalTimes[i] << ")";
            if (i < train->fares.size())
                cout << " --[" << train->fares[i] << "Rs]--> ";
        }
        cout << "\n";
    }

    void cancelTicket() {
        int pnr;
        cout << "Enter PNR to cancel ticket: ";
        cin >> pnr;

        bool found = false;

        // Step 1: Confirmed List से हटाओ
        for (size_t i = 0; i < confirmedPassengers.size(); ++i) {
            if (confirmedPassengers[i].pnr == pnr) {
                int train_no = confirmedPassengers[i].train_no;
                Train* train = findTrain(train_no);
                if (train) train->booked_seats--;

                cout << "Ticket with PNR " << pnr << " cancelled (Confirmed Passenger).\n";
                confirmedPassengers.erase(confirmedPassengers.begin() + i);
                found = true;

                // Waiting List से promote करें
                if (!waitingList.empty()) {
                    Passenger wp = waitingList.front();
                    waitingList.pop();

                    wp.seat_no = train->booked_seats + 1;
                    wp.status = "Confirmed";
                    confirmedPassengers.push_back(wp);
                    train->booked_seats++;
                    cout << "Waiting passenger " << wp.name << " promoted to Confirmed. PNR: " << wp.pnr << "\n";
                }

                break;
            }
        }

        // Step 2: Waiting List से हटाओ अगर नहीं मिला Confirmed में
        if (!found) {
            queue<Passenger> temp;
            while (!waitingList.empty()) {
                Passenger p = waitingList.front();
                waitingList.pop();
                if (p.pnr == pnr) {
                    cout << "Ticket with PNR " << pnr << " cancelled (Waiting Passenger).\n";
                    found = true;
                    continue;
                }
                temp.push(p);
            }
            waitingList = temp;
        }

        if (!found) {
            cout << "PNR not found.\n";
        }
    }
};

int main() {
    RailwaySystem system;

    // Adding Trains
    system.addTrain(101, "Mumbai Express", 5,
        {"Jabalpur", "Itarsi", "Nasik", "Mumbai"},
        {200, 250, 300},
        {"06:00", "09:30", "13:00", "17:30"});

    system.addTrain(102, "Delhi Rajdhani", 4,
        {"Jabalpur", "Bhopal", "Jhansi", "Agra", "Delhi"},
        {180, 200, 220, 250},
        {"07:00", "09:00", "12:00", "15:00", "18:00"});

    system.addTrain(1005, "Rewa Jabalpur Express", 6,
        {"Jabalpur", "Katni", "Maihar", "Unchehra", "Satna", "Rewa"},
        {70, 60, 30, 40, 50},
        {"05:30", "06:45", "07:30", "08:15", "09:00", "09:45"});

    int choice;
    do {
        cout << "\n===== Railway Reservation System =====\n";
        cout << "1. Show All Available Trains\n";
        cout << "2. Book Ticket\n";
        cout << "3. Show Passengers of a Train\n";
        cout << "4. Show Train Route & Fare\n";
        cout << "5. Cancel Ticket\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: system.showAllTrains(); break;
            case 2: system.bookTicket(); break;
            case 3: system.showPassengers(); break;
            case 4: system.showTrainRouteAndFare(); break;
            case 5: system.cancelTicket(); break;
            case 6: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice! Try again.\n";
        }
    } while (choice != 6);

    return 0;
}