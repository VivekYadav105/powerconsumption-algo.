#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>
using namespace std;

const int MAX_CAPACITY = 100;
const int SAFE_CAPACITY = 92;
const int DEVICE_MAX = 40;

class Device {
private:
    string id;
    int consumption;  
    int maxAllowed; 

public:
    Device(string id, int maxAllowed){
      this->id = id;
      this->consumption = 0;
      this->maxAllowed = maxAllowed;
    }

    string getId() const { return id; }
    int getConsumption() const { return consumption; }
    int getMaxAllowed() const { return maxAllowed; }

    void setConsumption(int value) { consumption = value; }
    void setMaxAllowed(int value) { maxAllowed = value; }
};

int currentUsage = 0;                 
queue<string> deviceQueue;            
unordered_map<string, Device> devices; 

void redistributePower() {
    int availablePower = SAFE_CAPACITY - currentUsage;
    if (availablePower <= 0) return;

    queue<string> tempQueue = deviceQueue;
    while (!tempQueue.empty() && availablePower > 0) {
        string id = tempQueue.front();
        tempQueue.pop();

        Device &device = devices[id];
        int extraNeeded = device.getMaxAllowed() - device.getConsumption();
        int allocated = min(extraNeeded, availablePower);
        device.setConsumption(device.getConsumption() + allocated);
        currentUsage += allocated;
        availablePower -= allocated;
    }
}

void addDevice(string id, int requested) {
    if (devices.find(id) != devices.end()) {
        cout << "Device " << id << " already exists.\n";
        return;
    }

    Device newDevice(id, min(requested, DEVICE_MAX));
    deviceQueue.push(id);
    devices[id] = newDevice;

    int allocated = min(newDevice.getMaxAllowed(), SAFE_CAPACITY - currentUsage);
    devices[id].setConsumption(allocated);
    currentUsage += allocated;

    cout << "Device " << id << " added with " << allocated << " units.\n";
    redistributePower();
}

void removeDevice(string id) {
    if (devices.find(id) == devices.end()) {
        cout << "Device " << id << " does not exist.\n";
        return;
    }

    Device &device = devices[id];
    currentUsage -= device.getConsumption();
    devices.erase(id);

    queue<string> tempQueue;
    while (!deviceQueue.empty()) {
        string current = deviceQueue.front();
        deviceQueue.pop();
        if (current != id) {
            tempQueue.push(current);
        }
    }
    swap(deviceQueue, tempQueue);

    cout << "Device " << id << " removed.\n";
    redistributePower();
}

void updateDevice(string id, int newConsumption) {
    if (devices.find(id) == devices.end()) {
        cout << "Device " << id << " does not exist.\n";
        return;
    }

    Device &device = devices[id];
    int delta = newConsumption - device.getConsumption();
    if (delta > 0 && currentUsage + delta > SAFE_CAPACITY) {
        cout << "Cannot increase consumption for " << id << ". Insufficient power.\n";
        return;
    }

    currentUsage += delta;
    device.setConsumption(newConsumption);

    cout << "Device " << id << " updated to " << newConsumption << " units.\n";
    redistributePower();
}

void displayState() {
    cout << "Current power usage: " << currentUsage << "/" << SAFE_CAPACITY << " units.\n";
    cout << "Active devices:\n";
    for (const auto &entry : devices) {
        const Device &device = entry.second;
        cout << "  Device " << device.getId() << ": " << device.getConsumption()
             << "/" << device.getMaxAllowed() << " units\n";
    }
    cout << "Queue order: ";
    queue<string> tempQueue = deviceQueue;
    while (!tempQueue.empty()) {
        cout << tempQueue.front() << " ";
        tempQueue.pop();
    }
    cout << "\n";
}

int main() {
    addDevice("A", 40);
    addDevice("B", 40);
    addDevice("C", 40);
    displayState();

    updateDevice("A", 20);
    displayState();

    removeDevice("B");
    displayState();

    return 0;
}
