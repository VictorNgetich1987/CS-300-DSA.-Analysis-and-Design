#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <ctime>
#include <cctype>
#include <cstdlib>
using namespace std;

#define null NULL

struct Bid {
    string bidId;   // Auction ID
    string title;
    string fund;
    double amount;

    Bid() {
        amount = 0.0;
    }
};

struct Node {
    Bid bid;
    Node* next;

    Node() {
        next = null;
    }

    Node(const Bid& aBid) {
        bid = aBid;
        next = null;
    }
};

class HashTable {
private:
    vector<Node*> table;
    unsigned int tableSize;

    unsigned int hash(const string& bidId) const {
        unsigned int key = 0;
        for (size_t i = 0; i < bidId.size(); ++i) {
            key += static_cast<unsigned int>(bidId[i]);
        }
        return key % tableSize;
    }

public:
    HashTable(unsigned int size = 179) {
        tableSize = size;
        table.resize(tableSize, null);
    }

    virtual ~HashTable() {
        for (unsigned int i = 0; i < tableSize; ++i) {
            Node* cur = table[i];
            while (cur != null) {
                Node* tmp = cur;
                cur = cur->next;
                delete tmp;
            }
            table[i] = null;
        }
    }

    void Insert(const Bid& bid) {
        unsigned int key = hash(bid.bidId);
        Node* newNode = new Node(bid);

        if (table[key] == null) {
            table[key] = newNode;
            return;
        }
        Node* cur = table[key];
        while (cur->next != null) {
            cur = cur->next;
        }
        cur->next = newNode;
    }

    void PrintAll() const {
        for (unsigned int i = 0; i < tableSize; ++i) {
            Node* cur = table[i];
            while (cur != null) {
                cout << "Key " << i << ": "
                     << "Auction ID: " << cur->bid.bidId << " | "
                     << cur->bid.title << " | "
                     << cur->bid.amount << " | "
                     << cur->bid.fund << "\n";
                cur = cur->next;
            }
        }
    }

    Bid Search(const string& bidId) const {
        unsigned int key = hash(bidId);
        Node* cur = table[key];
        while (cur != null) {
            if (cur->bid.bidId == bidId) {
                return cur->bid;
            }
            cur = cur->next;
        }
        return Bid();
    }

    void Remove(const string& bidId) {
        unsigned int key = hash(bidId);
        Node* cur = table[key];
        Node* prev = null;

        while (cur != null) {
            if (cur->bid.bidId == bidId) {
                if (prev == null) {
                    table[key] = cur->next;
                } else {
                    prev->next = cur->next;
                }
                delete cur;
                return;
            }
            prev = cur;
            cur = cur->next;
        }
    }
};

static string trim(const string& s) {
    size_t start = 0;
    size_t end = s.size();

    while (start < end && isspace(static_cast<unsigned char>(s[start]))) {
        ++start;
    }
    while (end > start && isspace(static_cast<unsigned char>(s[end - 1]))) {
        --end;
    }
    return s.substr(start, end - start);
}

static double parseAmount(string s) {
    s = trim(s);
    string cleaned;
    for (size_t i = 0; i < s.size(); ++i) {
        char c = s[i];
        if (c == '$' || c == ',') {
            continue;
        }
        cleaned.push_back(c);
    }
    if (cleaned.empty()) {
        return 0.0;
    }
    return atof(cleaned.c_str());
}

vector<Bid> loadBids(const string& filename) {
    vector<Bid> bids;

    ifstream file;
    file.open(filename.c_str());

    if (!file.is_open()) {
        cout << "Error opening file: " << filename << "\n";
        return bids;
    }

    string line;
    if (!getline(file, line)) {
        file.close();
        return bids;
    }

    while (getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        stringstream ss(line);
        Bid bid;
        string amountStr;

        if (!getline(ss, bid.bidId, ',')) {
            continue;
        }
        if (!getline(ss, bid.title, ',')) {
            bid.title.clear();
        }
        if (!getline(ss, bid.fund, ',')) {
            bid.fund.clear();
        }
        if (!getline(ss, amountStr, ',')) {
            amountStr.clear();
        }

        bid.amount = parseAmount(amountStr);
        bids.push_back(bid);
    }

    file.close();
    return bids;
}

int main() {
    HashTable hashTable;
    int choice = 0;

    // Always use the full dataset file
    string filename = "C:\\Users\\pc\\Downloads\\CS 300 Hash Table Assignment Student Files\\eBid_Monthly_Sales.csv";

    while (choice != 9) {
        cout << "\nMenu:\n"
             << "1. Load Bids\n"
             << "2. Display All Bids\n"
             << "3. Find Auction ID\n"
             << "4. Remove Auction ID\n"
             << "9. Exit\n"
             << "Enter choice: ";
        if (!(cin >> choice)) {
            cout << "Invalid input. Exiting.\n";
            return 0;
        }

        if (choice == 1) {
            cout << "Loading CSV file: " << filename << "\n";
            clock_t start = clock();
            vector<Bid> bids = loadBids(filename);
            for (size_t i = 0; i < bids.size(); ++i) {
                hashTable.Insert(bids[i]);
            }
            clock_t end = clock();

            cout << bids.size() << " bids read\n";
            cout << "time: " << (end - start) << " clock ticks\n";
            cout << "time: " << (static_cast<double>(end - start) / CLOCKS_PER_SEC) << " seconds\n";
        }
        else if (choice == 2) {
            hashTable.PrintAll();
        }
        else if (choice == 3) {
            string auctionId;
            cout << "Enter Auction ID to search: ";
            cin >> auctionId;

            clock_t start = clock();
            Bid found = hashTable.Search(auctionId);
            clock_t end = clock();

            if (!found.bidId.empty()) {
                cout << "Auction ID: " << found.bidId << " | "
                     << found.title << " | "
                     << found.amount << " | " << found.fund << "\n";
            } else {
                cout << "Auction ID " << auctionId << " not found.\n";
            }
            cout << "time: " << (end - start) << " clock ticks\n";
            cout << "time: " << (static_cast<double>(end - start) / CLOCKS_PER_SEC) << " seconds\n";
        }
        else if (choice == 4) {
            string auctionId;
            cout << "Enter Auction ID to remove: ";
            cin >> auctionId;
            hashTable.Remove(auctionId);
            cout << "Auction ID " << auctionId << " removed (if it existed).\n";
        }
        else if (choice == 9) {
            cout << "Good bye.\n";
        }
    }

    return 0;
}
