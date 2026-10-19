# AVL Event Scheduler

C++ program that stores events in an **AVL tree** keyed by timestamp and
answers three queries: the k-th smallest event, the floor of a timestamp, and
the ceiling of a timestamp.

Source: `avl_event_scheduler.cpp`

---

## 1. Problem Statement

Design an event scheduling application that maintains a set of events, where
each event has:

- a **timestamp** used as the key (format `YYYYMMDDHHMM`, e.g. `202610161430`
  means 16 Oct 2026, 14:30)
- a **title** describing the event

The application must support the following operations efficiently:

1. **Insert** a new event into the schedule.
2. **Remove** a cancelled event by its timestamp.
3. **K-th smallest** — find the event with the k-th smallest timestamp
   (k = 1 gives the earliest event).
4. **Floor** of a given timestamp — the event with the largest timestamp that
   is less than or equal to the given timestamp.
5. **Ceiling** of a given timestamp — the event with the smallest timestamp
   that is greater than or equal to the given timestamp.

All five operations must run in **O(log n)** time (k-th smallest in
O(log n + k)), where n is the number of scheduled events.

### Why a plain BST is not enough

If events are inserted in increasing time order, a normal BST degenerates into
a linked list of height n, and every operation degrades to O(n). An **AVL tree**
fixes this: after every insert or remove it rebalances using rotations so that
the heights of the left and right subtrees of any node differ by at most 1.
This guarantees height = O(log n) and therefore O(log n) operations.

---

## 2. Algorithm

### Node and balance

```
Node { Event ev; int height; Node *left, *right; }

balanceFactor(n) = height(n->left) − height(n->right)
```

After every insert/remove, fix any node whose balance factor is outside
[−1, 1] using rotations:

| Case | Condition | Fix |
|---|---|---|
| LL | bf > 1, bf(left) ≥ 0 | right rotate |
| LR | bf > 1, bf(left) < 0 | left rotate child, then right rotate |
| RR | bf < −1, bf(right) ≤ 0 | left rotate |
| RL | bf < −1, bf(right) > 0 | right rotate child, then left rotate |

### Insert / Remove

Standard BST insert or delete (two children → replace with in-order
successor), then `updateHeight` + `rebalance` on the way back up.
**O(log n)** each.

### K-th smallest

In-order traversal visits events in timestamp order. Recurse left, decrement
k; when k hits 0 the current node is the answer, else recurse right.
**O(log n + k)**.

### Floor of ts (largest ≤ ts)

One root-to-leaf walk: go left if ts is smaller than the node; otherwise the
node is a candidate and the right subtree may do better.
**O(log n)**.

### Ceiling of ts (smallest ≥ ts)

Mirror of floor: go right if ts is larger, keep the node as candidate
otherwise. **O(log n)**.

---

## 3. Explanation of the Program

The whole application is one file, `avl_event_scheduler.cpp`, organized as
follows:

| Lines | Part | What it does |
|---|---|---|
| 1 | `Event` / `Node` | Data stored at each tree node |
| 2 | `height`, `updateHeight`, `balanceFactor` | Height bookkeeping for AVL |
| 3 | `rotateRight`, `rotateLeft` | The two rotations (all four cases reuse them) |
| 4 | `rebalance` | Detects LL / LR / RR / RL and applies the correct rotation |
| 5 | `insert` | BST insert + rebalance on the way back up |
| 6 | `remove` | BST delete (with in-order successor) + rebalance |
| 7 | `search` | Standard BST search by timestamp |
| 8 | `kthSmallest` | Recursive in-order count-down |
| 9 | `floorOf`, `ceilingOf` | One root-to-leaf walk each |
| 10 | `inorder` | Prints all events sorted by timestamp |
| 11 | `main` | Demo: inserts 7 events, then runs all queries |

### Walkthrough of `main`

1. Seven events are inserted, e.g. `202610161000 DBMS Lecture`,
   `202610161430 Team Standup`, `202610150900 Doctor Appointment`.
2. `inorder` prints them already sorted by time — this is the free benefit of
   in-order traversal on a BST.
3. `kthSmallest(root, k)` is run for k = 1 to 7, printing each event in order.
4. Floor/ceiling are queried for four timestamps, covering all cases:
   - a time between two events → floor and ceiling return the surrounding events
   - an exact event time → both return that event
   - a time after all events → floor returns the last event, ceiling = none
   - a time before all events → floor = none, ceiling returns the first event
5. `remove(202610161430)` cancels the Team Standup; the tree rebalances and a
   final `search` confirms the event is gone.

### Complexity summary

| Operation | Time | Space |
|---|---|---|
| Insert | O(log n) | O(log n) recursion stack |
| Remove | O(log n) | O(log n) |
| Search | O(log n) | O(log n) |
| K-th smallest | O(log n + k) | O(log n) |
| Floor | O(log n) | O(log n) |
| Ceiling | O(log n) | O(log n) |
| Print all (in-order) | O(n) | O(log n) |

---

## Build & Run

```
g++ -std=c++17 -Wall avl_event_scheduler.cpp -o scheduler
scheduler
```

## Sample Output

```
Events in order (sorted by timestamp):
  202610150900  Doctor Appointment
  202610160800  Morning Yoga
  202610161000  DBMS Lecture
  202610161430  Team Standup
  202610162000  Gym Session
  202610171100  Project Demo
  202610181600  Flight Check-in

--- K-th smallest ---
1-th smallest: 202610150900 (Doctor Appointment)
2-th smallest: 202610160800 (Morning Yoga)
...

--- Floor and Ceiling ---
Query 202610161200 -> floor:    202610161000 (DBMS Lecture)
Query 202610161200 -> ceiling:  202610161430 (Team Standup)
Query 202610190000 -> floor:    202610181600 (Flight Check-in)
Query 202610190000 -> ceiling:  none
```
