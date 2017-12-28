//Brandon Roy
#include <fstream>
#include <iostream>
#include <iomanip>
#include <queue>
#include <vector>
#include <cmath>
using namespace std;

struct Event {
    char type;
    int time;
    int transaction;
    Event(char type, int time, int transaction) {
        this->type = type;
        this->time = time;
        this->transaction = transaction;
    }
};
bool operator<(const Event& s1, const Event& s2) {
        return s1.time > s2.time;
}
struct EventList {
    priority_queue<Event> queueOfEvents;
    bool fill(string fileName);
    void simulate();
};


bool EventList::fill(string fileName) {
    ifstream file;

    file.open(fileName);

    if (!file) {
        cerr << "Unable to open file: " << fileName << endl << endl;
        file.close();
        return false;
    }

    int x, y;
    while (file >> x >> y) {
        if((x < 0 || y < 0) || (x < 0 && y < 0)){
            cerr << "Negative arrival or transaction time detected." << endl << endl;
            file.close();
            return false;
        } else {
            queueOfEvents.push(Event('A', x, y));
            //queueOfEvents.push(Event('D', x + y, 0));
        }
    }

    file.close();
}
void EventList::simulate() {
    priority_queue<Event> copy = queueOfEvents;

    int open_time = 0, total_wait = 0;
    while(!queueOfEvents.empty()) {
        Event current = queueOfEvents.top();
        if(current.type == 'A') {
            open_time = max(current.time, open_time);
            total_wait += open_time - current.time;
            open_time += current.transaction;
            queueOfEvents.push(Event('D', open_time, 0));
            cout << "Processing An Arrival Event At Time: "
                 << setw(5) << current.time << endl;
        } else if(current.type == 'D') {
            cout << "Processing A Departure Event At Time: "
                 << setw(4) << current.time << endl;
        }
        queueOfEvents.pop();
    }

    int totalProcessed = 0;
    while(!copy.empty()) {
        Event current = copy.top();
        if(current.type == 'A')
            totalProcessed++;
        copy.pop();
    }

    cout << endl << "Final Stats" << endl
         << "==================================" << endl
         << "Total Number Of People Processed: " << totalProcessed << endl
         << "Average Amount Of Time Spent Waiting: "
         << abs((double)total_wait/(double)totalProcessed) << endl << endl;

}

int main() {
    string input = "";

    while(true) {
        EventList eventList;

        cout << "Input file name to read: ";
        getline(cin, input);

        if(input == "^C") break;

        if(eventList.fill(input)) eventList.simulate();
    }
}


