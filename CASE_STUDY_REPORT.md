# MAJOR PROJECT CASE STUDY REPORT
## Case Study 3: Online Shopping Product Price Sorting System

**Course:** Data Structures and Algorithms 1 (DSA-1)  
**Programming Language:** C++ (Standard C++11 / C++17)  
**Development Environment:** macOS / POSIX (g++ / clang++)  
**Author / Student:** Engineering Student  
**Workspace Files:**  
- `product_sorting_system.cpp` (Core Academic System)  
- `terminal_visualizer.cpp` (Live Animated Visualizer Companion)  
- `CASE_STUDY_REPORT.md` (Project Documentation)  

---

## 1. Abstract
In modern e-commerce platforms (such as Amazon, Flipkart, and eBay), catalog browsing requires fast and intuitive data reordering. Customers routinely sort listings by price—either **Low to High** to discover budget-friendly deals or **High to Low** to browse premium selections. 

This project designs and implements an in-memory **Online Shopping Product Price Sorting System** in C++. Product records containing Product ID, Product Name, and Price are stored using an array of structures (`struct Product`). The system implements three classical comparison-based sorting algorithms: **Bubble Sort**, **Selection Sort**, and **Insertion Sort**. Memory operations and record exchanges are executed using **pointers** without relying on built-in library sorting routines. The project empirically measures and compares the exact number of **passes**, **comparisons**, and **swaps/data moves**, provides a step-by-step trace of each sorting pass, and presents an engineering evaluation demonstrating why **Insertion Sort** is the most optimal choice for small product collections.

---

## 2. Problem Statement & Case Study Context

### 2.1 The Scenario
An online shopping platform hosts hundreds of retail products. Initially, product records arrive in an arbitrary, unsorted sequence based on supplier upload time. Users demand the ability to sort these products by price in either ascending or descending order.

### 2.2 Functional Requirements
1. **Array-Based Storage:** Store records of at least 10 products using fixed-size arrays.
2. **Data Attributes:** Store Product ID (`int`), Product Name (`string`), and Price (`double`).
3. **Dynamic Input:** Allow dynamic entry of new products while checking for array overflow.
4. **Display Capabilities:** Present products in their original unsorted state and formatted sorted state.
5. **Multiple Sorting Algorithms:**
   - Implement **Bubble Sort** with early termination optimization.
   - Implement **Selection Sort** using clean minimum-index selection.
   - Implement **Insertion Sort** using the intuitive swap-down method.
6. **Dual Direction Support:** Enable sorting from Lowest to Highest and Highest to Lowest.
7. **Empirical Metric Tracking:** Count and compare the total number of passes, key comparisons, and record swaps/moves.
8. **Step-by-Step Educational Traces:** Output intermediate array states and comparison logs.

---

## 3. Data Structure Design & Memory Architecture

### 3.1 Record Encapsulation: `struct Product`
Rather than maintaining disconnected parallel arrays, each product is modeled as an atomic record:

```cpp
struct Product {
    int id;          // Unique product identifier (4 bytes)
    string name;     // Product title (24/32 bytes standard string)
    double price;    // Product price in USD (8 bytes)
};
```

#### Why `struct` with Pointers was Chosen:
- **Data Cohesion:** The ID, title, and price reside contiguously in memory.
- **Cleaner Function Signatures:** Sorting routines accept a single pointer parameter `Product* arr` rather than three independent array pointers (`int* ids, string* names, double* prices`).
- **Atomic Pointer Swapping:** One pointer swap swaps the entire product record at once without risks of attribute desynchronization:
  ```cpp
  void swapProducts(Product* a, Product* b) {
      Product temp = *a;
      *a = *b;
      *b = temp;
  }
  ```

### 3.2 Default Catalog Dataset (12 Products)
| ID | Product Name | Original Unsorted Price ($) |
|:--:|---|:--:|
| 101 | Wireless Gaming Mouse | $29.99 |
| 102 | Mechanical RGB Keyboard | $89.50 |
| 103 | Pro Noise-Cancel Headset | $149.00 |
| 104 | 4-Port USB-C Multi-Hub | $19.99 |
| 105 | Ultra HD 4K Monitor 27" | $299.99 |
| 106 | Full HD 1080p Webcam | $45.00 |
| 107 | Ergonomic Laptop Stand | $32.50 |
| 108 | Portable Bluetooth Speaker | $59.95 |
| 109 | External 1TB NVMe SSD | $115.00 |
| 110 | Compact Smartphone Tripod | $14.99 |
| 111 | Anti-Fray Desk Mat / Pad | $12.50 |
| 112 | Fast Wireless Charging Dock | $39.99 |

---

## 4. Sorting Algorithms: Logic & Implementation

All sorting routines share an identical parameter interface using pointers:
```cpp
void sortFunction(Product* arr, int n, bool ascending, bool verbose,
                  int* comparisons, int* swaps, int* passes);
```
- `Product* arr`: Pointer to the first element of the Product array.
- `int n`: Total number of products in the catalog.
- `bool ascending`: Direction controller (`true` = Low $\rightarrow$ High; `false` = High $\rightarrow$ Low).
- `bool verbose`: Trace switch (`true` prints console logs; `false` runs silently for benchmarking).
- `int* comparisons`: Output pointer returning total comparisons performed.
- `int* swaps`: Output pointer returning total record swaps/moves performed.
- `int* passes`: Output pointer returning total outer loop passes executed.

---

### 4.1 Bubble Sort (Countdown Boundary Method)
**Algorithm Mechanics:**  
Compares adjacent pairs `*(arr + j)` and `*(arr + j + 1)`. If they violate the desired order, they swap. The outer loop index `i` begins at `n - 1` and decrements to `1`. As a result, the inner loop simply runs `for (int j = 0; j < i; j++)`, eliminating complex arithmetic.

```cpp
void bubbleSort(Product* arr, int n, bool ascending, bool verbose,
                int* comparisons, int* swaps, int* passes) {
    *comparisons = 0;
    *swaps = 0;
    *passes = 0;

    for (int i = n - 1; i > 0; i--) {
        (*passes)++;
        bool swapped = false;

        for (int j = 0; j < i; j++) {
            (*comparisons)++;
            Product* p1 = arr + j;
            Product* p2 = arr + (j + 1);

            bool shouldSwap = ascending ? (p1->price > p2->price) : (p1->price < p2->price);

            if (shouldSwap) {
                swapProducts(p1, p2);
                (*swaps)++;
                swapped = true;
            }
        }
        // Early Exit: if no swaps occurred, the array is already sorted
        if (!swapped) break;
    }
}
```

---

### 4.2 Selection Sort (Clean Minimum-Index Selection)
**Algorithm Mechanics:**  
Divides the array into a sorted sublist (left) and an unsorted sublist (right). In pass `i`, it assumes `minIdx = i` and scans the remaining elements (`j = i + 1` to `n - 1`). The absolute minimum price is identified, and **exactly one swap** is executed at the end of the pass.

```cpp
void selectionSort(Product* arr, int n, bool ascending, bool verbose,
                   int* comparisons, int* swaps, int* passes) {
    *comparisons = 0;
    *swaps = 0;
    *passes = 0;

    for (int i = 0; i < n - 1; i++) {
        (*passes)++;
        int minIdx = i;

        for (int j = i + 1; j < n; j++) {
            (*comparisons)++;
            bool condition = ascending ? ((arr + j)->price < (arr + minIdx)->price)
                                       : ((arr + j)->price > (arr + minIdx)->price);
            if (condition) {
                minIdx = j;
            }
        }

        if (minIdx != i) {
            swapProducts(arr + i, arr + minIdx);
            (*swaps)++;
        }
    }
}
```

---

### 4.3 Insertion Sort (Swap-Down Method)
**Algorithm Mechanics:**  
Considers the first item sorted (`i = 0`). For each item from `i = 1` to `n - 1`, the item "walks backwards" by repeatedly swapping with its left neighbor until its left neighbor is smaller. The loop halts immediately when the item is in its correct place.

```cpp
void insertionSort(Product* arr, int n, bool ascending, bool verbose,
                   int* comparisons, int* shifts, int* passes) {
    *comparisons = 0;
    *shifts = 0;
    *passes = 0;

    for (int i = 1; i < n; i++) {
        (*passes)++;
        int j = i;

        while (j > 0) {
            (*comparisons)++;
            Product* prev = arr + (j - 1);
            Product* curr = arr + j;

            bool shouldSwap = ascending ? (prev->price > curr->price)
                                        : (prev->price < curr->price);

            if (shouldSwap) {
                swapProducts(prev, curr); // Swap backwards
                (*shifts)++;
                j--;
            } else {
                break; // Neighbor is smaller; stop walking left
            }
        }
    }
}
```

---

## 5. Step-by-Step Sorting Tracing (Actual Prices)

### 5.1 Bubble Sort Walkthrough (Ascending Order)
- **Pass 1 (Boundary: up to index 11):**
  - `$149.00` (Headset) compared with `$19.99` (Hub) $\rightarrow$ **Swap!**
  - `$299.99` (Monitor) compared with `$45.00` (Webcam) $\rightarrow$ **Swap!**
  - Continues bubbling `$299.99` down to index 11.
  - *End of Pass 1:* The largest item (`$299.99`) is locked at index 11.
- **Pass 2 (Boundary: up to index 10):**
  - `$149.00` bubbles down to index 10.
- **Pass 11 (Boundary: up to index 1):**
  - Array is fully sorted $\rightarrow 0$ swaps $\rightarrow$ Early exit triggers!

### 5.2 Selection Sort Walkthrough (Ascending Order)
- **Pass 1:** Scans indices 0 to 11. Absolute minimum found is `$12.50` (Desk Mat, idx 10).  
  **Swap:** Position 0 (`$29.99`) $\longleftrightarrow$ Position 10 (`$12.50`).
- **Pass 2:** Scans indices 1 to 11. Absolute minimum found is `$14.99` (Tripod, idx 9).  
  **Swap:** Position 1 (`$89.50`) $\longleftrightarrow$ Position 9 (`$14.99`).
- **Pass 8:** Position 7 (`$59.95`) is already the minimum $\rightarrow$ **0 swaps performed!**

### 5.3 Insertion Sort Walkthrough (Ascending Order)
- **Pass 1 (Key: `$89.50`):** Compared with `$29.99` $\rightarrow$ Correct order $\rightarrow$ 0 swaps.
- **Pass 2 (Key: `$149.00`):** Compared with `$89.50` $\rightarrow$ Correct order $\rightarrow$ 0 swaps.
- **Pass 3 (Key: `$19.99` - USB Hub):**
  - Swaps with `$149.00`
  - Swaps with `$89.50`
  - Swaps with `$29.99`
  - Lands at position 0 in just 3 backward swaps!

---

## 6. Empirical Results & Performance Comparison

All three algorithms were executed on exact duplicates of the unsorted 12-product catalog:

| Metric | Bubble Sort | Selection Sort | Insertion Sort |
|---|:---:|:---:|:---:|
| **Outer Passes** | 11 | 11 | 11 |
| **Total Price Comparisons** | 66 | 66 | **48** |
| **Total Data Moves / Swaps** | 40 | **8** | 40 |
| **Best-Case Time Complexity** | $O(n)$ | $O(n^2)$ | $O(n)$ |
| **Average-Case Time Complexity** | $O(n^2)$ | $O(n^2)$ | $O(n^2)$ |
| **Worst-Case Time Complexity** | $O(n^2)$ | $O(n^2)$ | $O(n^2)$ |
| **Space Complexity** | $O(1)$ | $O(1)$ | $O(1)$ |
| **Algorithmic Stability** | **Stable** | Unstable | **Stable** |

### Key Empirical Findings:
1. **Comparisons:** Insertion Sort made **27.3% fewer comparisons** (48 vs. 66) than Bubble Sort and Selection Sort because its inner loop terminates immediately once the item reaches its relative sorted position.
2. **Memory Swaps:** Selection Sort achieved the absolute lowest write operations (only 8 swaps, strictly $\le n - 1$).
3. **Data Access Pattern:** Bubble Sort exhibited excessive adjacent swaps (40 swaps), making it the least efficient for practical execution.

---

## 7. Practical Evaluation: Which Sorting Method is Best for Small Collections?

### **Engineering Verdict: Insertion Sort is the Most Suitable.**

### Technical Justifications:
1. **Lower Comparison Overhead:**
   Selection Sort has no early break mechanism; it **must** perform $\frac{n(n-1)}{2} = 66$ comparisons regardless of whether the array is partially sorted. Insertion Sort aborts backward scanning as soon as a smaller element is detected, making it substantially faster on typical product catalogs.
2. **Online / Adaptive Property (Critical for E-Commerce):**
   In real-world e-commerce stores, product catalogs are dynamic: new products arrive continually into an already sorted list.
   - **Insertion Sort** inserts a new item into an existing sorted list in **$O(n)$ linear time** (a single pass).
   - **Selection Sort** and **Bubble Sort** would require a full $O(n^2)$ re-computation.
3. **Stability:**
   Insertion Sort is **stable**. If two products share the exact same price (e.g., two accessories priced at `$19.99`), their original relative ordering (by product ID or alphanumeric title) is strictly preserved. Selection Sort is inherently unstable and can scramble records with identical prices.
4. **Low Constant Factors:**
   For small arrays ($n \le 50$), the constant factors of algorithm overhead dominate. Insertion sort has minimal setup and register usage, which is why standard library implementations (such as GNU `std::sort` / Introsort and Python's Timsort) switch internally to Insertion Sort when sub-array partitions fall below 16–32 elements.

---

## 8. User Interface & Auxiliary Tools

### 8.1 Menu-Driven Architecture
The system features an interactive console menu:
```text
====================================================================
       ONLINE SHOPPING PRODUCT PRICE SORTING SYSTEM (DSA-1)
====================================================================
  1. Display Products in Original (Unsorted) Order
  2. Sort by Price using Bubble Sort (Step-by-Step Trace)
  3. Sort by Price using Selection Sort (Step-by-Step Trace)
  4. Sort by Price using Insertion Sort (Step-by-Step Trace)
  5. Compare Sorting Methods (Passes, Comparisons, Swaps)
  6. Add New Product (Accept ID, Name, Price)
  7. Reset to Default 12 Products
  8. Exit System
--------------------------------------------------------------------
```

### 8.2 Companion Tool: Live Terminal ASCII Visualizer
A dedicated companion program, `terminal_visualizer.cpp`, provides an interactive visual demonstration using ANSI terminal escapes (`\033[H\033[J`):
- Dynamic horizontal ASCII price bars (`████`) scaled to product cost.
- Color highlights:
  - 🟡 **Yellow:** Products actively being compared.
  - 🔴 **Red:** Products actively being swapped.
  - 🟢 **Green:** Products in confirmed sorted positions.
- Live ticking counters for Passes, Comparisons, and Swaps.

---

## 9. Compilation and Execution Guide

### 9.1 Compiling the Programs
Compile both standard C++ programs using `g++` or `clang++`:

```bash
# Compile main system
g++ -Wall -Wextra product_sorting_system.cpp -o product_sorting_system

# Compile visualizer
g++ -Wall -Wextra terminal_visualizer.cpp -o terminal_visualizer
```

### 9.2 Running the Systems
```bash
# Run main interactive system
./product_sorting_system

# Run live terminal visualizer
./terminal_visualizer
```

---

## 10. Conclusion
This case study successfully designed and validated an in-memory **Online Shopping Product Price Sorting System** in C++. Through pointer-based manipulation of structured product records, the project established clear empirical and theoretical comparisons among Bubble Sort, Selection Sort, and Insertion Sort. 

The investigation confirmed that while Selection Sort minimizes raw write operations, **Insertion Sort provides the best overall performance, stability, and adaptability for small-to-moderate e-commerce product catalogs**.

