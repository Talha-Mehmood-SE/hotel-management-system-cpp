#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <limits>
#include <sstream>

using namespace std;
class Room;
class HotelManagement;
class User {
protected:
    string username;
    string password;
public:
    User() {}
    User(const string& uname, const string& pwd) : username(uname), password(pwd) {}
    virtual ~User() {}
    virtual bool login() = 0; // pure virtual
    string getUsername() const { return username; }
    string getPassword() const { return password; }
};
class Admin : public User {
public:
    Admin() : User() {}
    Admin(const string& uname, const string& pwd) : User(uname, pwd) {}
    ~Admin() {}
    bool login() override {
        string enteredUsername, enteredPassword;
        cout << "Admin Login\n";
        cout << "Enter username: ";
        getline(cin, enteredUsername);
        cout << "Enter password: ";
        getline(cin, enteredPassword);
        if (enteredUsername == username && enteredPassword == password) {
            cout << "Login successful.\n";
            return true;
        }
        else {
            cout << "Invalid username or password.\n";
            return false;
        }
    }
};
class Customer : public User {
public:
    Customer() : User() {}
    Customer(const string& uname, const string& pwd) : User(uname, pwd) {}
    ~Customer() {}
    bool login() override {
        string enteredUsername, enteredPassword;
        cout << "Customer Login\n";
        cout << "Enter username: ";
        getline(cin, enteredUsername);
        cout << "Enter password: ";
        getline(cin, enteredPassword);
        if (enteredUsername == username && enteredPassword == password) {
            cout << "Login successful.\n";
            return true;
        }
        else {
            cout << "Invalid username or password.\n";
            return false;
        }
    }
};
class Room {
private:
    int roomNumber;
    string roomType;
    bool isAvailable;
    double price;
public:
    static int roomCount;
    Room() : roomNumber(0), roomType(""), isAvailable(true), price(0.0) {
        roomCount++;
    }
    Room(int rn, const string& rt, bool avail, double pr) : roomNumber(rn), roomType(rt), isAvailable(avail), price(pr) {
        roomCount++;
    }
    ~Room() {
        roomCount--;
    }
 void setRoomNumber(int rn) { roomNumber = rn; }
    int getRoomNumber() const { return roomNumber; }
    void setRoomType(const string& rt) { roomType = rt; }
    string getRoomType() const { return roomType; }
    void setAvailability(bool avail) { isAvailable = avail; }
    bool getAvailability() const { return isAvailable; }
    void setPrice(double pr) { price = pr; }
    double getPrice() const { return price; }
    bool matches(int rn) const { return roomNumber == rn; }
    bool matches(const string& rt) const { return roomType == rt; }
    friend void displayRoom(const Room& r);
};
int Room::roomCount = 0;
void displayRoom(const Room& r) {
    cout << "Room Number: " << r.roomNumber << "\n";
    cout << "Room Type: " << r.roomType << "\n";
    cout << "Available: " << (r.isAvailable ? "Yes" : "No") << "\n";
    cout << "Price per night: $" << r.price << "\n";
}
class IManagementOperations {
public:
    virtual void addRoom() = 0;
    virtual void updateRoom() = 0;
    virtual void deleteRoom() = 0;
    virtual void viewRooms() = 0;
    virtual void searchRoom() = 0;
    virtual ~IManagementOperations() {}
};
class Booking {
public:
    string customerUsername;
    int roomNumber;
    Booking(const string& custUser, int rn) : customerUsername(custUser), roomNumber(rn) {}
};
class HotelManagement : public IManagementOperations {
private:
    vector<Room> rooms;
    vector<Admin> admins;
    vector<Customer> customers;
    vector<Booking> bookings;
    const string roomFile = "rooms.txt";
    const string customerFile = "customers.txt";
    void clearInput() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    int inputRoomNumber() {
        int num;
        while (true) {
            cout << "Enter room number (positive integer): ";
            if (cin >> num && num > 0) {
                clearInput();
                return num;
            }
            cout << "Invalid input. Please enter a positive integer.\n";
            clearInput();
        }
    }
    string inputRoomType() {
        string type;
        while (true) {
            cout << "Enter room type (Single/Double/Suite): ";
            getline(cin, type);
            if (type == "Single" || type == "Double" || type == "Suite") {
                return type;
            }
            cout << "Invalid room type. Please enter Single, Double, or Suite.\n";
        }
    }
    double inputPrice() {
        double price;
        while (true) {
            cout << "Enter price per night (positive number): ";
            if (cin >> price && price > 0) {
                clearInput();
                return price;
            }
            cout << "Invalid price. Please enter a positive number.\n";
            clearInput();
        }
    }
    bool inputAvailability() {
        string input;
        while (true) {
            cout << "Is room available? (y/n): ";
            getline(cin, input);
            if (input == "y" || input == "Y") return true;
            if (input == "n" || input == "N") return false;
            cout << "Invalid input. Enter 'y' or 'n'.\n";
        }
    }
    Room* findRoomByNumber(int rn) {
        for (size_t i = 0; i < rooms.size(); ++i) {
            if (rooms[i].getRoomNumber() == rn)
                return &rooms[i];
        }
        return NULL;
    }
    void saveRoomsToFile() {
        ofstream ofs(roomFile.c_str());
        if (!ofs) {
            cout << "Failed to open room file for writing.\n";
            return;
        }
        for (size_t i = 0; i < rooms.size(); ++i) {
            ofs << rooms[i].getRoomNumber() << "," 
                << rooms[i].getRoomType() << "," 
                << (rooms[i].getAvailability() ? "1" : "0") << "," 
                << rooms[i].getPrice() << "\n";
        }
        ofs.close();
    }
    void loadRoomsFromFile() {
        ifstream ifs(roomFile.c_str());
        if (!ifs) return; // file might not exist initially
        rooms.clear();
        string line;
        while (getline(ifs, line)) {
            if (line.empty()) continue;
            int rn = 0;
            string rt = "";
            bool avail = false;
            double pr = 0.0;
            size_t pos1 = line.find(',');
            if (pos1 == string::npos) continue;
            string rnStr = line.substr(0, pos1);     
            stringstream ss1(rnStr);
            ss1 >> rn;
            if (ss1.fail()) continue;
            size_t pos2 = line.find(',', pos1 + 1);
            if (pos2 == string::npos) continue;
            rt = line.substr(pos1 + 1, pos2 - pos1 - 1);
            size_t pos3 = line.find(',', pos2 + 1);
            if (pos3 == string::npos) continue;
            string availStr = line.substr(pos2 + 1, pos3 - pos2 - 1);
            avail = (availStr == "1");
            string priceStr = line.substr(pos3 + 1);
            stringstream ss2(priceStr);
            ss2 >> pr;
            if (ss2.fail()) continue;
            rooms.push_back(Room(rn, rt, avail, pr));
        }
        ifs.close();
    }
    void saveCustomersToFile() {
        ofstream ofs(customerFile.c_str());
        if (!ofs) {
            cout << "Failed to open customer file for writing.\n";
            return;
        }
        for (size_t i = 0; i < customers.size(); ++i) {
            ofs << customers[i].getUsername() << "," << customers[i].getPassword() << "\n";
        }
        ofs.close();
    }
    void loadCustomersFromFile() {
        ifstream ifs(customerFile.c_str());
        if (!ifs) return;
        customers.clear();
        string line;
        while (getline(ifs, line)) {
            if (line.empty()) continue;
            size_t pos = line.find(',');
            if (pos == string::npos) continue;
            string uname = line.substr(0, pos);
            string pwd = line.substr(pos + 1);
            customers.push_back(Customer(uname, pwd));
        }
        ifs.close();
    }
public:
    HotelManagement() {
        // Initialize admin accounts here
        admins.push_back(Admin("admin1", "admin123"));
        admins.push_back(Admin("admin2", "admin456"));
        loadRoomsFromFile();
        loadCustomersFromFile();
    }
    ~HotelManagement() {
        saveRoomsToFile();
        saveCustomersToFile();
    }
    void addRoom() override {
        int rn = inputRoomNumber();
        if (findRoomByNumber(rn) != NULL) {
            cout << "Room number already exists. Cannot add.\n";
            return;
        }
        string rt = inputRoomType();
        bool avail = inputAvailability();
        double pr = inputPrice();
        Room newRoom(rn, rt, avail, pr);
        rooms.push_back(newRoom);
        cout << "Room added successfully.\n";
        saveRoomsToFile();
    }
    void updateRoom() override {
        int rn = inputRoomNumber();
        Room* room = findRoomByNumber(rn);
        if (room == NULL) {
            cout << "Room not found.\n";
            return;
        }
        cout << "Current details:\n";
        displayRoom(*room);
        cout << "\nEnter new details:\n";
        string rt = inputRoomType();
        bool avail = inputAvailability();
        double pr = inputPrice();
        room->setRoomType(rt);
        room->setAvailability(avail);
        room->setPrice(pr);
        cout << "Room updated successfully.\n";
        saveRoomsToFile();
    }
    void deleteRoom() override {
        int rn = inputRoomNumber();
        for (size_t i = 0; i < rooms.size(); ++i) {
            if (rooms[i].getRoomNumber() == rn) {
                rooms.erase(rooms.begin() + i);
                cout << "Room deleted successfully.\n";
                saveRoomsToFile();
                return;
            }
        }
        cout << "Room not found.\n";
    }
    void viewRooms() override {
        if (rooms.empty()) {
            cout << "No rooms available.\n";
            return;
        }
        for (size_t i = 0; i < rooms.size(); ++i) {
            cout << "------------------------\n";
            displayRoom(rooms[i]);
        }
        cout << "------------------------\n";
    }
    void searchRoom() override {
        cout << "Search by (1) Room Number or (2) Room Type: ";
        int choice;
        if (!(cin >> choice)) {
            cout << "Invalid input.\n";
            clearInput();
            return;
        }
        clearInput();

        if (choice == 1) {
            int rn = inputRoomNumber();
            Room* room = findRoomByNumber(rn);
            if (room != NULL) {
                displayRoom(*room);
            }
            else {
                cout << "Room not found.\n";
            }
        }
        else if (choice == 2) {
            string rt = inputRoomType();
            bool found = false;
            for (size_t i = 0; i < rooms.size(); ++i) {
                if (rooms[i].getRoomType() == rt) {
                    displayRoom(rooms[i]);
                    found = true;
                }
            }
            if (!found) cout << "No rooms of type " << rt << " found.\n";
        }
        else {
            cout << "Invalid choice.\n";
        }
    }
    bool adminLogin() {
        string uname, pwd;
        cout << "Enter Admin username: ";
        getline(cin, uname);
        cout << "Enter Admin password: ";
        getline(cin, pwd);
        for (size_t i = 0; i < admins.size(); ++i) {
            if (admins[i].getUsername() == uname && admins[i].getPassword() == pwd) {
                cout << "Admin login successful.\n";
                return true;
            }
        }
        cout << "Invalid Admin credentials.\n";
        return false;
    }
    void customerSignup() {
        string uname, pwd, pwdConfirm;
        cout << "Customer Signup\n";
        while (true) {
            cout << "Enter username: ";
            getline(cin, uname);
            if (uname.empty()) {
                cout << "Username cannot be empty.\n";
                continue;
            }
            bool exists = false;
            for (size_t i = 0; i < customers.size(); ++i) {
                if (customers[i].getUsername() == uname) {
                    exists = true;
                    break;
                }
            }
            if (exists) {
                cout << "Username already exists. Try another.\n";
                continue;
            }
            break;
        }
        while (true) {
            cout << "Enter password: ";
            getline(cin, pwd);
            cout << "Confirm password: ";
            getline(cin, pwdConfirm);
            if (pwd == pwdConfirm && !pwd.empty()) {
                break;
            }
            cout << "Passwords do not match or empty. Try again.\n";
        }
        customers.push_back(Customer(uname, pwd));
        saveCustomersToFile();
        cout << "Signup successful. You can now login.\n";
    }
    Customer* customerLogin() {
        string uname, pwd;
        cout << "Customer Login\n";
        cout << "Enter username: ";
        getline(cin, uname);
        cout << "Enter password: ";
        getline(cin, pwd);
        for (size_t i = 0; i < customers.size(); ++i) {
            if (customers[i].getUsername() == uname && customers[i].getPassword() == pwd) {
                cout << "Login successful.\n";
                return &customers[i];
            }
        }
        cout << "Invalid credentials.\n";
        return NULL;
    }
    void bookRoom(const string& customerUsername) {
        cout << "Available Rooms:\n";
        bool anyAvailable = false;
        for (size_t i = 0; i < rooms.size(); ++i) {
            if (rooms[i].getAvailability()) {
                displayRoom(rooms[i]);
                cout << "-------------------\n";
                anyAvailable = true;
            }
        }
        if (!anyAvailable) {
            cout << "No rooms available for booking.\n";
            return;
        }
        int rn = inputRoomNumber();
        Room* room = findRoomByNumber(rn);
        if (room == NULL) {
            cout << "Room not found.\n";
            return;
        }
        if (!room->getAvailability()) {
            cout << "Room is not available.\n";
            return;
        }
        room->setAvailability(false);
        bookings.push_back(Booking(customerUsername, rn));
        cout << "Room booked successfully.\n";
        saveRoomsToFile();
    }
    void cancelBooking(const string& customerUsername) {
        cout << "Your Bookings:\n";
        bool found = false;
        for (size_t i = 0; i < bookings.size(); ++i) {
            if (bookings[i].customerUsername == customerUsername) {
                cout << "Room Number: " << bookings[i].roomNumber << "\n";
                found = true;
            }
        }
        if (!found) {
            cout << "You have no bookings.\n";
            return;
        }
        int rn = inputRoomNumber();
        for (size_t i = 0; i < bookings.size(); ++i) {
            if (bookings[i].customerUsername == customerUsername && bookings[i].roomNumber == rn) {
                bookings.erase(bookings.begin() + i);
                Room* room = findRoomByNumber(rn);
                if (room != NULL) {
                    room->setAvailability(true);
                }
                cout << "Booking cancelled successfully.\n";
                saveRoomsToFile();
                return;
            }
        }
        cout << "Booking not found.\n";
    }
    void customerMenu(Customer* cust) {
        if (cust == NULL) return;
        while (true) {
            cout << "\nCustomer Menu:\n";
            cout << "1. View Rooms\n";
            cout << "2. Search Room\n";
            cout << "3. Book Room\n";
            cout << "4. Cancel Booking\n";
            cout << "5. Logout\n";
            cout << "Enter choice: ";
            int choice;
            if (!(cin >> choice)) {
                cout << "Invalid input.\n";
                clearInput();
                continue;
            }
            clearInput();

            switch (choice) {
                case 1:
                    viewRooms();
                    break;
                case 2:
                    searchRoom();
                    break;
                case 3:
                    bookRoom(cust->getUsername());
                    break;
                case 4:
                    cancelBooking(cust->getUsername());
                    break;
                case 5:
                    cout << "Logging out...\n";
                    return;
                default:
                    cout << "Invalid choice.\n";
            }
        }
    }
    void adminMenu() {
        while (true) {
            cout << "\nAdmin Menu:\n";
            cout << "1. Add Room\n";
            cout << "2. Update Room\n";
            cout << "3. Delete Room\n";
            cout << "4. View Rooms\n";
            cout << "5. Search Room\n";
            cout << "6. Logout\n";
            cout << "Enter choice: ";
            int choice;
            if (!(cin >> choice)) {
                cout << "Invalid input.\n";
                clearInput();
                continue;
            }
            clearInput();

            switch (choice) {
                case 1:
                    addRoom();
                    break;
                case 2:
                    updateRoom();
                    break;
                case 3:
                    deleteRoom();
                    break;
                case 4:
                    viewRooms();
                    break;
                case 5:
                    searchRoom();
                    break;
                case 6:
                    cout << "Logging out...\n";
                    return;
                default:
                    cout << "Invalid choice.\n";
            }
        }
    }
    void mainMenu() {
    	 cout << "\nHotel Management System\n";
        while (true) {
            cout << "\n1. Admin Login\n";
            cout << "2. Customer Login\n";
            cout << "3. Customer Signup\n";
            cout << "4. Exit\n";
            cout << "Enter choice: ";
            int choice;
            if (!(cin >> choice)) {
                cout << "Invalid input.\n";
                clearInput();
                continue;
            }
            clearInput();

            switch (choice) {
                case 1:
                    if (adminLogin()) {
                        adminMenu();
                    }
                    break;
                case 2: {
                    Customer* cust = customerLogin();
                    if (cust != NULL) {
                        customerMenu(cust);
                    }
                    break;
                }
                case 3:
                    customerSignup();
                    break;
                case 4:
                    cout << "Exiting system...\n";
                    return;
                default:
                    cout << "Invalid choice.\n";
            }
        }
    }
};
int main() {
    HotelManagement hm;
    hm.mainMenu();
    return 0;
}

