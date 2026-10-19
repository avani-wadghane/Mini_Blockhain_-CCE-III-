// AVL Event Scheduler - timestamps (YYYYMMDDHHMM style ints) are the keys
// Supports: insert, remove, k-th smallest event, floor, ceiling, inorder print

#include <iostream>
#include <string>
using namespace std;

struct Event {
    long long timestamp;   // key, e.g. 202610161430 = 16 Oct 2026, 14:30
    string title;
};

struct Node {
    Event ev;
    int height;
    Node *left, *right;
    Node(Event e) : ev(e), height(1), left(nullptr), right(nullptr) {}
};

int height(Node* n) { return n ? n->height : 0; }

void updateHeight(Node* n) {
    if (n) n->height = 1 + max(height(n->left), height(n->right));
}

int balanceFactor(Node* n) {
    return n ? height(n->left) - height(n->right) : 0;
}

/*
right rotation:
      y            x
     / \          / \
    x   C  -->   A   y
   / \              / \
  A   B            B   C
*/
Node* rotateRight(Node* y) {
    Node* x = y->left;
    Node* B = x->right;
    x->right = y;
    y->left = B;
    updateHeight(y);
    updateHeight(x);
    return x;
}

// left rotation (mirror image of rotateRight)

Node* rotateLeft(Node* x) {
    Node* y = x->right;
    Node* B = y->left;
    y->left = x;
    x->right = B;
    updateHeight(x);
    updateHeight(y);
    return y;
}

// fix balance at node n after insert/remove
Node* rebalance(Node* n) {
    updateHeight(n);
    int bf = balanceFactor(n);
    if (bf > 1) {                       // left-heavy
        if (balanceFactor(n->left) < 0) // Left-Right case
            n->left = rotateLeft(n->left);
        return rotateRight(n);          // Left-Left case
    }
    if (bf < -1) {                      // right-heavy
        if (balanceFactor(n->right) > 0) // Right-Left case
            n->right = rotateRight(n->right);
        return rotateLeft(n);            // Right-Right case
    }
    return n;
}

// ---------------- INSERT ----------------
Node* insert(Node* root, Event e) {
    if (!root) return new Node(e);
    if (e.timestamp < root->ev.timestamp)
        root->left = insert(root->left, e);
    else if (e.timestamp > root->ev.timestamp)
        root->right = insert(root->right, e);
    else
        root->ev.title = e.title;   // same timestamp: update title
    return rebalance(root);
}

// ---------------- REMOVE ----------------
Node* minNode(Node* n) {
    while (n->left) n = n->left;
    return n;
}

Node* remove(Node* root, long long ts) {
    if (!root) return nullptr;
    if (ts < root->ev.timestamp)
        root->left = remove(root->left, ts);
    else if (ts > root->ev.timestamp)
        root->right = remove(root->right, ts);
    else {   // found the node to delete
        if (!root->left || !root->right) {
            Node* tmp = root->left ? root->left : root->right;
            delete root;
            root = tmp;
        } else {
            Node* succ = minNode(root->right);   // in-order successor
            root->ev = succ->ev;
            root->right = remove(root->right, succ->ev.timestamp);
        }
    }
    if (root) root = rebalance(root);
    return root;
}

// ---------------- SEARCH ----------------
Node* search(Node* root, long long ts) {
    if (!root || ts == root->ev.timestamp) return root;
    if (ts < root->ev.timestamp) return search(root->left, ts);
    return search(root->right, ts);
}

// ---------------- K-th SMALLEST (in-order: left, node, right) ----------------
bool kthSmallest(Node* root, int& k, Event& ans) {
    if (!root) return false;
    if (kthSmallest(root->left, k, ans)) return true;   // k lies in left subtree
    if (--k == 0) { ans = root->ev; return true; }      // this node is the k-th
    return kthSmallest(root->right, k, ans);            // k lies in right subtree
}

// ---------------- FLOOR: largest timestamp <= ts ----------------
Event* floorOf(Node* root, long long ts) {
    if (!root) return nullptr;
    if (root->ev.timestamp == ts) return &root->ev;
    if (ts < root->ev.timestamp) return floorOf(root->left, ts);
    Event* cand = floorOf(root->right, ts);   // root itself is a candidate
    return cand ? cand : &root->ev;
}

// ---------------- CEILING: smallest timestamp >= ts ----------------
Event* ceilingOf(Node* root, long long ts) {
    if (!root) return nullptr;
    if (root->ev.timestamp == ts) return &root->ev;
    if (ts > root->ev.timestamp) return ceilingOf(root->right, ts);
    Event* cand = ceilingOf(root->left, ts);  // root itself is a candidate
    return cand ? cand : &root->ev;
}

// ---------------- IN-ORDER PRINT ----------------
void inorder(Node* root) {
    if (!root) return;
    inorder(root->left);
    cout << "  " << root->ev.timestamp << "  " << root->ev.title << "\n";
    inorder(root->right);
}

void printEvent(const string& label, Event* e) {
    cout << label;
    if (e) cout << e->timestamp << " (" << e->title << ")\n";
    else   cout << "none\n";
}

int main() {
    Node* root = nullptr;

    long long data[] = {
        202610161000LL, 202610161430LL, 202610150900LL,
        202610171100LL, 202610160800LL, 202610181600LL,
        202610162000LL
    };
    string titles[] = {
        "DBMS Lecture", "Team Standup", "Doctor Appointment",
        "Project Demo", "Morning Yoga", "Flight Check-in", "Gym Session"
    };
    for (int i = 0; i < 7; i++)
        root = insert(root, {data[i], titles[i]});

    cout << "Events in order (sorted by timestamp):\n";
    inorder(root);

    cout << "\n--- K-th smallest ---\n";
    for (int k = 1; k <= 7; k++) {
        Event ans;
        int kk = k;
        if (kthSmallest(root, kk, ans))
            cout << k << "-th smallest: " << ans.timestamp << " (" << ans.title << ")\n";
    }

    cout << "\n--- Floor and Ceiling ---\n";
    long long queries[] = {202610161200, 202610160800, 202610190000, 202610150000};
    for (long long q : queries) {
        printEvent("Query " + to_string(q) + " -> floor:    ", floorOf(root, q));
        printEvent("Query " + to_string(q) + " -> ceiling: ", ceilingOf(root, q));
    }

    cout << "\n--- Remove an event ---\n";
    root = remove(root, 202610161430);
    cout << "After removing 202610161430:\n";
    inorder(root);

    Node* found = search(root, 202610161430);
    cout << "\nSearch 202610161430: " << (found ? "found" : "not found") << "\n";
    return 0;
}
