#include <cstddef>
#include <functional>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>

// Function to calculate a simple hash
std::string calculateHash(const std::string& input) {
    std::hash<std::string> hasher;
    std::size_t hashValue = hasher(input);

    std::stringstream ss;
    ss << std::hex << hashValue;
    return ss.str();
}

// Block structure
struct Block {
    std::string transactionData;
    std::string previousHash;
    std::string currentHash;
    Block* next;
};

// Blockchain class
class Blockchain {
private:
    Block* head;
    Block* tail;
    int count;

public:
    Blockchain() {
        head = nullptr;
        tail = nullptr;
        count = 0;
        createGenesisBlock();
    }

    ~Blockchain() {
        Block* current = head;
        while (current != nullptr) {
            Block* temp = current;
            current = current->next;
            delete temp;
        }
    }

    void createGenesisBlock() {
        Block* genesis = new Block();
        genesis->transactionData = "Genesis Block";
        genesis->previousHash = "0";
        genesis->currentHash = calculateHash(genesis->transactionData + genesis->previousHash);
        genesis->next = nullptr;

        head = genesis;
        tail = genesis;
        count = 1;
    }

    void addBlock(const std::string& data) {
        if (tail == nullptr) {
            createGenesisBlock();
        }

        Block* newBlock = new Block();
        newBlock->transactionData = data;
        newBlock->previousHash = tail->currentHash;
        newBlock->currentHash = calculateHash(newBlock->transactionData + newBlock->previousHash);
        newBlock->next = nullptr;

        tail->next = newBlock;
        tail = newBlock;
        count++;

        std::cout << "\nBlock added successfully!\n";
        std::cout << "Current Hash: " << newBlock->currentHash << "\n";
    }

    void displayChain() {
        std::cout << "\n===== BLOCKCHAIN =====\n";
        Block* current = head;
        int index = 1;

        while (current != nullptr) {
            std::cout << "\nBlock #" << index << "\n";
            std::cout << "Data:         " << current->transactionData << "\n";
            std::cout << "Previous Hash:" << current->previousHash << "\n";
            std::cout << "Current Hash: " << current->currentHash << "\n";
            current = current->next;
            index++;
        }
        std::cout << "\nTotal Blocks: " << count << "\n";
        std::cout << "======================\n";
    }

    bool validateChain() {
        if (head == nullptr) {
            std::cout << "\nNo blockchain exists.\n";
            return true;
        }

        Block* current = head;
        while (current != nullptr) {
            std::string recalculated = calculateHash(current->transactionData + current->previousHash);
            if (recalculated != current->currentHash) {
                std::cout << "\nTampering detected at this block!\n";
                return false;
            }
            current = current->next;
        }

        Block* prev = head;
        Block* curr = head->next;

        while (curr != nullptr) {
            if (curr->previousHash != prev->currentHash) {
                std::cout << "\nChain is broken between blocks!\n";
                return false;
            }
            prev = curr;
            curr = curr->next;
        }

        std::cout << "\nChain is VALID! No tampering found.\n";
        return true;
    }

    void tamperBlock(int blockNum, const std::string& newData) {
        if (blockNum < 1 || blockNum > count) {
            std::cout << "\nInvalid block number!\n";
            return;
        }

        Block* current = head;
        int index = 1;

        while (current != nullptr && index < blockNum) {
            current = current->next;
            index++;
        }

        if (current == nullptr) {
            std::cout << "\nBlock not found!\n";
            return;
        }

        current->transactionData = newData;
        std::cout << "\nBlock #" << blockNum << " data changed (Hash NOT updated).\n";
        std::cout << "Run validation to detect tampering.\n";
    }

    int getCount() const {
        return count;
    }
};

void displayMenu() {
    std::cout << "\n===== MINI BLOCKCHAIN =====\n";
    std::cout << "1. Add New Block\n";
    std::cout << "2. Display Entire Chain\n";
    std::cout << "3. Validate Chain Integrity\n";
    std::cout << "4. Simulate Tampering\n";
    std::cout << "5. Exit\n";
    std::cout << "==========================\n";
    std::cout << "Enter choice: ";
}

int main() {
    Blockchain chain;
    int choice = 0;
    std::string data;
    int blockNum = 0;

    std::cout << "Simple Mini Blockchain Simulator\n";

    do {
        displayMenu();
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
        case 1:
            std::cout << "\nEnter transaction data: ";
            std::getline(std::cin, data);
            chain.addBlock(data);
            break;

        case 2:
            chain.displayChain();
            break;

        case 3:
            chain.validateChain();
            break;

        case 4:
            std::cout << "\nEnter block number (1 - " << chain.getCount() << "): ";
            std::cin >> blockNum;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Enter new data: ";
            std::getline(std::cin, data);
            chain.tamperBlock(blockNum, data);
            break;

        case 5:
            std::cout << "\nExiting...\n";
            break;

        default:
            std::cout << "\nInvalid choice! Try again.\n";
            break;
        }
    } while (choice != 5);

    return 0;
}
