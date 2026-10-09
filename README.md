# Mini Blockchain Simulator
# Problem Statement

Design and implement a simple blockchain simulator in C++ that stores blocks in a singly linked list. The system should allow users to:

- create a genesis block,
- add new blocks containing transaction-like data,
- display the full chain,
- validate whether the chain has been tampered with,
- and simulate data modification in an existing block.

The main challenge is to keep each block linked to the previous one using a hash-based chain so that any modification of block data can be detected during validation.

## Algorithm

1. Start by creating a genesis block.
2. Each block stores:
   - transaction data,
   - previous hash,
   - current hash,
   - pointer to the next block.
3. When a new block is added:
   - the previous hash is set to the current hash of the last block,
   - the current hash is computed from the block data and the previous hash.
4. To validate the blockchain:
   - recompute the hash for each block using its stored data and previous hash,
   - compare it with the stored current hash,
   - ensure each block's previous hash matches the previous block's current hash.
5. If a block's data is changed without updating its hash, validation fails and tampering is detected.

## Explanation

This project demonstrates the basic idea behind blockchain technology: every block is connected to the previous one through a hash reference. A block is considered valid only if its stored hash matches the hash computed from its current contents and the previous block's hash.

Using a singly linked list is a simple and effective way to model block chaining because each block points to the next block in the sequence. The `head` pointer marks the first block, while the `tail` pointer marks the latest block added.

The program uses a basic hash function (`std::hash<std::string>`) to generate deterministic hash values for block content. Although this is not a secure cryptographic hash, it is sufficient for demonstrating the integrity-checking concept in a small educational project.

### Why tampering is detected

If someone changes a block's transaction data after the block has been created, the evaluated hash no longer matches the stored hash. During validation, the program recalculates the hash and immediately notices the mismatch.

## Features

- Automatic creation of a Genesis Block
- Addition of new blocks to the chain
- Display of the entire blockchain
- Validation of chain integrity
- Tampering simulation by modifying block data without updating hash

## Requirements

- C++11 or later
- Any C++ compiler (GCC/MinGW recommended)

## How to Compile and Run

### Windows (GCC/MinGW)
```bash
g++ -o blockchain.exe main.cpp
blockchain.exe
```


## Usage

1. Run the program.
2. Choose one of the menu options:
   - `1. Add New Block` – Add a new transaction
   - `2. Display Entire Chain` – View all blocks
   - `3. Validate Chain Integrity` – Check whether the chain is valid
   - `4. Simulate Tampering` – Change a block's data without updating its hash
   - `5. Exit`

Tip: Add a few blocks, validate the chain, modify one block, and validate again to see tampering detection in action.
