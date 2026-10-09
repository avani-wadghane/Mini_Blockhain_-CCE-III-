# Mini Blockchain Simulator

A simple menu-driven C++ blockchain simulation using a **Singly Linked List**.

## Features

- Create a Genesis Block automatically
- Add new blocks (each block stores transaction data, previous hash, and current hash)
- Display the entire blockchain
- Validate chain integrity
- Detect tampering by modifying a block's data

## Requirements

- C++11 or later
- Any C++ compiler (g++ recommended)

## How to Compile & Run

### Windows (GCC/MinGW)
```bash
g++ -o blockchain.exe main.cpp
blockchain.exe
```

## Usage

1. Run the program
2. Choose from the menu:
   - **1. Add New Block** – Add a new transaction
   - **2. Display Entire Chain** – View all blocks
   - **3. Validate Chain Integrity** – Check if the chain is valid
   - **4. Simulate Tampering** – Change data of any block (without recomputing hash)
   - **5. Exit**

**Tip:** Try adding a few blocks → Validate (should show valid) → Tamper a block → Validate again (will detect tampering).
