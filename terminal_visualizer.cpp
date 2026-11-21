/*
 ============================================================================
  Project: Terminal-Based ASCII Sorting Visualizer (DSA-1 Major Project)
  File   : terminal_visualizer.cpp (Separate visualizer file)
  
  Features:
  - Real-time animated ASCII horizontal bar charts
  - ANSI Escape Codes for screen refresh and colors:
      * Yellow = Active Comparison
      * Red    = Active Swap / Shift
      * Green  = Confirmed Sorted Position
      * Cyan   = Normal / Unsorted state
  - Live metric counters: Passes, Comparisons, Swaps
  - Pure Standard C++ (No external graphics libraries required)
 ============================================================================
*/

#include <iostream>
#include <iomanip>
#include <string>
#include <thread>
#include <chrono>

using namespace std;

const int MAX_PRODUCTS = 100;
const int DEFAULT_DELAY_MS = 220; // Animation delay per step in milliseconds

// ANSI Color Codes for terminal visualizer
const string RESET       = "\033[0m";
const string BOLD        = "\033[1m";
const string RED         = "\033[1;31m";
const string GREEN       = "\033[1;32m";
const string YELLOW      = "\033[1;33m";
const string CYAN        = "\033[1;36m";
const string BLUE        = "\033[1;34m";
const string MAGENTA     = "\033[1;35m";
const string CLEAR_HOME  = "\033[H\033[J"; // Move cursor to top-left and clear screen

// Structure to store product details
struct Product {
    int id;
    string name;
    double price;
};

// Pointer-based swap function
void swapProducts(Product* a, Product* b) {
    Product temp = *a;
    *a = *b;
    *b = temp;
}

// Pointer-based copy function
void copyProducts(Product* src, Product* dest, int n) {
    for (int i = 0; i < n; i++) {
        *(dest + i) = *(src + i);
    }
}

// Default 12 products
void loadDefaultProducts(Product* arr, int* n) {
    *n = 12;
    arr[0]  = {101, "Wireless Mouse", 29.99};
    arr[1]  = {102, "Mech Keyboard", 89.50};
    arr[2]  = {103, "Gaming Headset", 149.00};
    arr[3]  = {104, "USB-C Hub", 19.99};
    arr[4]  = {105, "4K Monitor 27\"", 299.99};
    arr[5]  = {106, "1080p Webcam", 45.00};
    arr[6]  = {107, "Laptop Stand", 32.50};
    arr[7]  = {108, "BT Speaker", 59.95};
    arr[8]  = {109, "1TB NVMe SSD", 115.00};
    arr[9]  = {110, "Phone Tripod", 14.99};
    arr[10] = {111, "Desk Mat / Pad", 12.50};
    arr[11] = {112, "Charging Dock", 39.99};
}

// --------------------------------------------------------------------------
// Renders the entire animated frame to the terminal
// --------------------------------------------------------------------------
void renderFrame(Product* arr, int n, const string& algorithmName, int pass,
                 int comparisons, int swaps, int activeIdx1, int activeIdx2,
                 const string& actionType, int sortedFromIndex, bool ascending) {
    // Redraw screen from top-left (no flickering)
    cout << CLEAR_HOME;

    cout << BOLD << CYAN << "==============================================================================\n";
    cout << "             ONLINE SHOPPING PRODUCT PRICE SORTING - LIVE ASCII VISUALIZER     \n";
    cout << "==============================================================================\n" << RESET;

    cout << BOLD << "Algorithm: " << MAGENTA << algorithmName << RESET
         << " | Order: " << (ascending ? (GREEN + "Ascending (Low->High)") : (BLUE + "Descending (High->Low)")) << RESET
         << " | Pass: " << BOLD << pass << RESET << "\n";

    cout << "Comparisons: " << YELLOW << comparisons << RESET
         << " | Swaps/Moves: " << RED << swaps << RESET
         << " | Status: " << BOLD << actionType << RESET << "\n";

    cout << "------------------------------------------------------------------------------\n";
    cout << left << setw(4) << "#"
         << setw(7) << "ID"
         << setw(18) << "Product Name"
         << right << setw(10) << "Price ($)"
         << left << "   Visual Price Bar\n";
    cout << "------------------------------------------------------------------------------\n";

    double maxPrice = 300.0; // Scale bars based on maximum catalog price

    for (int i = 0; i < n; i++) {
        Product* p = arr + i;
        int barLength = static_cast<int>((p->price / maxPrice) * 36);
        if (barLength < 1) barLength = 1;

        string barStr = "";
        for (int b = 0; b < barLength; b++) barStr += "█";

        string itemColor = RESET;
        string tag = "     ";

        // Determine highlighting based on current sorting step
        if (i == activeIdx1 || i == activeIdx2) {
            if (actionType.find("SWAP") != string::npos || actionType.find("SHIFT") != string::npos) {
                itemColor = RED;
                tag = " ◄SWAP";
            } else {
                itemColor = YELLOW;
                tag = " ◄COMP";
            }
        } else if (sortedFromIndex >= 0) {
            // For Bubble Sort (sorted tail) or Selection Sort (sorted head)
            if ((ascending && sortedFromIndex < n && i >= sortedFromIndex) ||
                (!ascending && sortedFromIndex < n && i >= sortedFromIndex) ||
                (algorithmName == "Selection Sort" && i < sortedFromIndex)) {
                itemColor = GREEN;
                tag = " [OK]";
            }
        }

        cout << itemColor << left << setw(4) << (i + 1)
             << setw(7) << p->id
             << setw(18) << p->name
             << right << setw(10) << fixed << setprecision(2) << p->price << "   "
             << barStr << tag << RESET << "\n";
    }

    cout << "------------------------------------------------------------------------------\n";
    cout << "Legend: "
         << YELLOW << "█ Yellow = Comparing" << RESET << " | "
         << RED << "█ Red = Swapping/Moving" << RESET << " | "
         << GREEN << "█ Green = Sorted Position" << RESET << "\n";
    cout << "==============================================================================\n";

    // Pause briefly so the animation is visible
    this_thread::sleep_for(chrono::milliseconds(DEFAULT_DELAY_MS));
}

// --------------------------------------------------------------------------
// Animated Bubble Sort
// --------------------------------------------------------------------------
void visualBubbleSort(Product* arr, int n, bool ascending) {
    int comparisons = 0;
    int swaps = 0;
    int pass = 0;

    // i starts at (n - 1) and shrinks; j simply runs up to i
    for (int i = n - 1; i > 0; i--) {
        pass++;
        bool swapped = false;

        for (int j = 0; j < i; j++) {
            comparisons++;
            Product* p1 = arr + j;
            Product* p2 = arr + (j + 1);

            // Step 1: Comparing phase
            string compMsg = "Comparing [" + to_string((int)p1->price) + "] with [" + to_string((int)p2->price) + "]";
            renderFrame(arr, n, "Bubble Sort", pass, comparisons, swaps, j, j + 1, compMsg, i + 1, ascending);

            bool condition = ascending ? (p1->price > p2->price) : (p1->price < p2->price);

            if (condition) {
                swaps++;
                swapProducts(p1, p2);
                swapped = true;

                // Step 2: Swap phase
                string swapMsg = "SWAPPING [" + p1->name + "] <--> [" + p2->name + "]";
                renderFrame(arr, n, "Bubble Sort", pass, comparisons, swaps, j, j + 1, swapMsg, i + 1, ascending);
            }
        }

        if (!swapped) break;
    }

    // Final sorted frame
    renderFrame(arr, n, "Bubble Sort", pass, comparisons, swaps, -1, -1, "SORTING COMPLETED!", 0, ascending);
}

// --------------------------------------------------------------------------
// Animated Selection Sort (Method 1: Clean Min-Index Selection)
// --------------------------------------------------------------------------
void visualSelectionSort(Product* arr, int n, bool ascending) {
    int comparisons = 0;
    int swaps = 0;

    for (int i = 0; i < n - 1; i++) {
        int minIdx = i; // Assume current position holds the target element

        for (int j = i + 1; j < n; j++) {
            comparisons++;
            string scanMsg = "Scanning: compare [" + to_string((int)(arr + j)->price) + 
                             "] with current target [" + to_string((int)(arr + minIdx)->price) + "]";
            renderFrame(arr, n, "Selection Sort", i + 1, comparisons, swaps, minIdx, j, scanMsg, i, ascending);

            bool condition = ascending ? ((arr + j)->price < (arr + minIdx)->price)
                                       : ((arr + j)->price > (arr + minIdx)->price);
            if (condition) {
                minIdx = j;
            }
        }

        if (minIdx != i) {
            swaps++;
            swapProducts(arr + i, arr + minIdx);
            string swapMsg = "SWAPPING target into position " + to_string(i + 1);
            renderFrame(arr, n, "Selection Sort", i + 1, comparisons, swaps, i, minIdx, swapMsg, i + 1, ascending);
        }
    }

    renderFrame(arr, n, "Selection Sort", n - 1, comparisons, swaps, -1, -1, "SORTING COMPLETED!", n, ascending);
}

// --------------------------------------------------------------------------
// Animated Insertion Sort (Swap-Down Method)
// --------------------------------------------------------------------------
void visualInsertionSort(Product* arr, int n, bool ascending) {
    int comparisons = 0;
    int swaps = 0;

    for (int i = 1; i < n; i++) {
        int j = i;

        string startMsg = "Inserting [" + (arr + i)->name + " - $" + to_string((int)(arr + i)->price) + "]";
        renderFrame(arr, n, "Insertion Sort (Swap-Down)", i, comparisons, swaps, i, -1, startMsg, -1, ascending);

        while (j > 0) {
            comparisons++;
            Product* prev = arr + (j - 1);
            Product* curr = arr + j;

            bool shouldSwap = ascending ? (prev->price > curr->price)
                                        : (prev->price < curr->price);

            if (shouldSwap) {
                swaps++;
                swapProducts(prev, curr);
                string swapMsg = "SWAPPING [" + curr->name + "] left into position " + to_string(j);
                renderFrame(arr, n, "Insertion Sort (Swap-Down)", i, comparisons, swaps, j - 1, j, swapMsg, -1, ascending);
                j--;
            } else {
                break;
            }
        }
    }

    renderFrame(arr, n, "Insertion Sort (Swap-Down)", n - 1, comparisons, swaps, -1, -1, "SORTING COMPLETED!", 0, ascending);
}

int main() {
    Product originalCatalog[MAX_PRODUCTS];
    int count = 0;
    loadDefaultProducts(originalCatalog, &count);

    int choice = 0;

    do {
        cout << CLEAR_HOME;
        cout << BOLD << CYAN << "==============================================================================\n";
        cout << "       ONLINE SHOPPING PRODUCT PRICE SORTING - TERMINAL VISUALIZER            \n";
        cout << "==============================================================================\n" << RESET;
        cout << "  1. Watch Bubble Sort Animation (Price: Low to High)\n";
        cout << "  2. Watch Selection Sort Animation (Price: Low to High)\n";
        cout << "  3. Watch Insertion Sort Animation (Price: Low to High)\n";
        cout << "  4. Watch Sorting (Price: High to Low / Descending)\n";
        cout << "  5. Exit Visualizer\n";
        cout << "------------------------------------------------------------------------------\n";
        cout << "Enter your choice (1-5): ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        if (choice >= 1 && choice <= 3) {
            Product workingCopy[MAX_PRODUCTS];
            copyProducts(originalCatalog, workingCopy, count);

            if (choice == 1) visualBubbleSort(workingCopy, count, true);
            else if (choice == 2) visualSelectionSort(workingCopy, count, true);
            else if (choice == 3) visualInsertionSort(workingCopy, count, true);

            cout << "\nAnimation finished! Press ENTER to return to menu...";
            cin.ignore();
            cin.get();
        } else if (choice == 4) {
            int subChoice;
            cout << "\nChoose algorithm for Descending sort (1=Bubble, 2=Selection, 3=Insertion): ";
            cin >> subChoice;
            Product workingCopy[MAX_PRODUCTS];
            copyProducts(originalCatalog, workingCopy, count);

            if (subChoice == 1) visualBubbleSort(workingCopy, count, false);
            else if (subChoice == 2) visualSelectionSort(workingCopy, count, false);
            else if (subChoice == 3) visualInsertionSort(workingCopy, count, false);

            cout << "\nAnimation finished! Press ENTER to return to menu...";
            cin.ignore();
            cin.get();
        }
    } while (choice != 5);

    cout << "\nVisualizer closed. Original 'product_sorting_system.cpp' remains untouched!\n";
    return 0;
}
