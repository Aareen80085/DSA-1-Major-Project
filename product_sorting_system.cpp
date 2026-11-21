/*
 ============================================================================
  Case Study 3: Online Shopping Product Price Sorting System
  Course : Data Structures and Algorithms 1 (DSA-1) Major Project
  Language: C++ (Structure-based with Pointers, No Inbuilt Functions)

  Concepts Applied:
  - Structure (struct Product): Encapsulates ID, Name, and Price cleanly
  - Arrays & Pointers: Product catalog[MAX_PRODUCTS], Product* arr navigation
  - Custom Swap via Pointers: swapProducts(Product* a, Product* b)
  - Custom Copy via Pointers: copyProducts(Product* src, Product* dest, int n)
  - Bubble Sort, Selection Sort, and Insertion Sort
  - Ascending & Descending Order Sorting
  - Step-by-Step Price Comparison and Exchange Tracing
  - Metrics tracking (Passes, Comparisons, Swaps/Shifts) via pointers
 ============================================================================
*/

#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

const int MAX_PRODUCTS = 100;

// Structure to encapsulate product details as a single record
struct Product {
    int id;
    string name;
    double price;
};

// ==========================================================================
// Custom Swap Function using Pointers (No Inbuilt swap)
// ==========================================================================
void swapProducts(Product* a, Product* b) {
    Product temp = *a;
    *a = *b;
    *b = temp;
}

// ==========================================================================
// Custom Copy Function using Pointers (No Inbuilt copy)
// ==========================================================================
void copyProducts(Product* src, Product* dest, int n) {
    for (int i = 0; i < n; i++) {
        *(dest + i) = *(src + i);
    }
}

// ==========================================================================
// Display Function using Pointers
// ==========================================================================
void displayProducts(Product* arr, int n, const string& title) {
    cout << "\n====================================================================\n";
    cout << "  " << title << " (Total: " << n << ")\n";
    cout << "====================================================================\n";
    cout << left << setw(8)  << "ID"
         << setw(36) << "Product Name"
         << right << setw(16) << "Price ($)\n";
    cout << "--------------------------------------------------------------------\n";

    for (int i = 0; i < n; i++) {
        Product* p = arr + i;
        cout << left << setw(8)  << p->id
             << setw(36) << p->name
             << right << setw(16) << fixed << setprecision(2) << p->price << "\n";
    }
    cout << "--------------------------------------------------------------------\n";
}

// ==========================================================================
// Default Dataset: Loads 12 products using pointers
// ==========================================================================
void loadDefaultProducts(Product* arr, int* n) {
    *n = 12;
    arr[0]  = {101, "Wireless Gaming Mouse", 29.99};
    arr[1]  = {102, "Mechanical RGB Keyboard", 89.50};
    arr[2]  = {103, "Pro Noise-Cancel Headset", 149.00};
    arr[3]  = {104, "4-Port USB-C Multi-Hub", 19.99};
    arr[4]  = {105, "Ultra HD 4K Monitor 27\"", 299.99};
    arr[5]  = {106, "Full HD 1080p Webcam", 45.00};
    arr[6]  = {107, "Ergonomic Laptop Stand", 32.50};
    arr[7]  = {108, "Portable Bluetooth Speaker", 59.95};
    arr[8]  = {109, "External 1TB NVMe SSD", 115.00};
    arr[9]  = {110, "Compact Smartphone Tripod", 14.99};
    arr[10] = {111, "Anti-Fray Desk Mat / Pad", 12.50};
    arr[11] = {112, "Fast Wireless Charging Dock", 39.99};
}

/*
 ============================================================================
  1. BUBBLE SORT
  
  Parameters Explained:
  1. Product* arr     : Pointer to the first element of the Product array.
                        Allows in-place comparison and swapping of products.
  2. int n            : Total number of products currently in the list.
  3. bool ascending   : Sort order flag (true = Lowest to Highest price;
                        false = Highest to Lowest price).
  4. bool verbose     : Trace flag (true = print step-by-step trace;
                        false = silent execution for benchmark/comparison).
  5. int* comparisons : Output pointer returning total price comparisons made.
  6. int* swaps       : Output pointer returning total element swaps performed.
  7. int* passes      : Output pointer returning total passes completed.
 ============================================================================
*/
void bubbleSort(Product* arr, int n, bool ascending, bool verbose,
                int* comparisons, int* swaps, int* passes) {
    *comparisons = 0;
    *swaps = 0;
    *passes = 0;

    // i starts at (n - 1) and shrinks after each pass; j simply checks up to i
    for (int i = n - 1; i > 0; i--) {
        (*passes)++;
        bool swapped = false;

        if (verbose) {
            cout << "\n--- Pass " << *passes << " (Boundary: up to index " << i << ") ---\n";
        }

        for (int j = 0; j < i; j++) {
            (*comparisons)++;

            Product* p1 = arr + j;
            Product* p2 = arr + (j + 1);

            bool shouldSwap = ascending ? (p1->price > p2->price) : (p1->price < p2->price);

            if (shouldSwap) {
                if (verbose) {
                    cout << "  [Swap] $" << p1->price << " (" << p1->name << ") <--> $"
                         << p2->price << " (" << p2->name << ")\n";
                }
                swapProducts(p1, p2);
                (*swaps)++;
                swapped = true;
            }
        }

        // Early termination optimization if array is already sorted
        if (!swapped) {
            if (verbose) {
                cout << "  (Zero swaps in this pass: array is already fully sorted)\n";
            }
            break;
        }
    }
}

/*
 ============================================================================
  2. SELECTION SORT

  Parameters Explained:
  1. Product* arr     : Pointer to the first element of the Product array.
  2. int n            : Total number of products in the list.
  3. bool ascending   : Sort order flag (true = find minimum; false = find maximum).
  4. bool verbose     : Trace flag (true = print trace; false = silent).
  5. int* comparisons : Output pointer returning total price comparisons made.
  6. int* swaps       : Output pointer returning total element swaps performed.
  7. int* passes      : Output pointer returning total passes completed.
 ============================================================================
*/
void selectionSort(Product* arr, int n, bool ascending, bool verbose,
                   int* comparisons, int* swaps, int* passes) {
    *comparisons = 0;
    *swaps = 0;
    *passes = 0;

    for (int i = 0; i < n - 1; i++) {
        (*passes)++;
        int minIdx = i; // Assume current position has the minimum (or maximum)

        if (verbose) {
            cout << "\n--- Pass " << (i + 1) << " (Finding " 
                 << (ascending ? "Minimum" : "Maximum") << " Price from position " << i << ") ---\n";
        }

        // Scan remaining elements to find the true min/max index
        for (int j = i + 1; j < n; j++) {
            (*comparisons)++;

            bool condition = ascending ? ((arr + j)->price < (arr + minIdx)->price)
                                       : ((arr + j)->price > (arr + minIdx)->price);
            if (condition) {
                minIdx = j;
            }
        }

        // Swap the target element into position i
        if (minIdx != i) {
            if (verbose) {
                cout << "  [Swap] Position " << i << " ($" << (arr + i)->price << ") <--> "
                     << "Target Position " << minIdx << " ($" << (arr + minIdx)->price << " - " << (arr + minIdx)->name << ")\n";
            }
            swapProducts(arr + i, arr + minIdx);
            (*swaps)++;
        } else if (verbose) {
            cout << "  Position " << i << " ($" << (arr + i)->price << ") is already correct. (0 swaps)\n";
        }
    }
}

/*
 ============================================================================
  3. INSERTION SORT (Swap-Down Method)

  Parameters Explained:
  1. Product* arr     : Pointer to the first element of the Product array.
  2. int n            : Total number of products in the list.
  3. bool ascending   : Sort order flag (true = Low to High; false = High to Low).
  4. bool verbose     : Trace flag (true = print trace; false = silent).
  5. int* comparisons : Output pointer returning total price comparisons made.
  6. int* shifts      : Output pointer returning total swaps/moves performed.
  7. int* passes      : Output pointer returning total passes completed.
 ============================================================================
*/
void insertionSort(Product* arr, int n, bool ascending, bool verbose,
                   int* comparisons, int* shifts, int* passes) {
    *comparisons = 0;
    *shifts = 0;
    *passes = 0;

    for (int i = 1; i < n; i++) {
        (*passes)++;
        int j = i;

        if (verbose) {
            cout << "\n--- Pass " << i << " (Inserting: " << (arr + i)->name << " - $" << (arr + i)->price << ") ---\n";
        }

        int swapsInPass = 0;
        while (j > 0) {
            (*comparisons)++;
            Product* prev = arr + (j - 1);
            Product* curr = arr + j;

            bool shouldSwap = ascending ? (prev->price > curr->price)
                                        : (prev->price < curr->price);

            if (shouldSwap) {
                if (verbose) {
                    cout << "  [Swap-Down] $" << curr->price << " (" << curr->name
                         << ") <--> $" << prev->price << " (" << prev->name << ")\n";
                }
                swapProducts(prev, curr);
                (*shifts)++; // Tracks swaps/moves
                swapsInPass++;
                j--;
            } else {
                break; // Left neighbor is in order; stop immediately!
            }
        }

        if (verbose && swapsInPass == 0) {
            cout << "  Item is already in sorted position. (0 swaps)\n";
        }
    }
}

// ==========================================================================
// Comparative Analysis: Compares all three algorithms on identical copies
// ==========================================================================
void compareAlgorithms(Product* original, int n, bool ascending) {
    Product bArr[MAX_PRODUCTS];
    Product sArr[MAX_PRODUCTS];
    Product iArr[MAX_PRODUCTS];

    copyProducts(original, bArr, n);
    copyProducts(original, sArr, n);
    copyProducts(original, iArr, n);

    int bComp = 0, bSwaps = 0, bPasses = 0;
    int sComp = 0, sSwaps = 0, sPasses = 0;
    int iComp = 0, iShifts = 0, iPasses = 0;

    bubbleSort(bArr, n, ascending, false, &bComp, &bSwaps, &bPasses);
    selectionSort(sArr, n, ascending, false, &sComp, &sSwaps, &sPasses);
    insertionSort(iArr, n, ascending, false, &iComp, &iShifts, &iPasses);

    cout << "\n====================================================================\n";
    cout << "             SORTING ALGORITHMS COMPARATIVE ANALYSIS\n";
    cout << "  Direction: " << (ascending ? "Lowest to Highest (Ascending)" : "Highest to Lowest (Descending)") << "\n";
    cout << "====================================================================\n";
    cout << left << setw(18) << "Algorithm"
         << right << setw(12) << "Passes"
         << setw(16) << "Comparisons"
         << setw(18) << "Swaps / Shifts\n";
    cout << "--------------------------------------------------------------------\n";
    cout << left << setw(18) << "Bubble Sort"
         << right << setw(12) << bPasses
         << setw(16) << bComp
         << setw(18) << bSwaps << "\n";
    cout << left << setw(18) << "Selection Sort"
         << right << setw(12) << sPasses
         << setw(16) << sComp
         << setw(18) << sSwaps << "\n";
    cout << left << setw(18) << "Insertion Sort"
         << right << setw(12) << iPasses
         << setw(16) << iComp
         << setw(18) << iShifts << "\n";
    cout << "--------------------------------------------------------------------\n";

    cout << "\nPractical Insight (DSA-1):\n";
    cout << "1. Insertion Sort had the lowest comparisons (" << iComp << ") because its inner loop\n"
         << "   stops comparing immediately once the key's sorted location is determined.\n";
    cout << "2. Selection Sort minimized memory swaps to " << sSwaps << " (always <= n - 1 swaps).\n";
    cout << "3. Bubble Sort performed " << bSwaps << " adjacent swaps, the highest of the three.\n";
    cout << "4. Verdict: Insertion Sort is the most suitable for small collections and e-commerce,\n"
         << "   as new products can be inserted into an already sorted list in linear O(n) time.\n";
}

// ==========================================================================
// Accepts new custom product using pointers
// ==========================================================================
void addProduct(Product* arr, int* n) {
    if (*n >= MAX_PRODUCTS) {
        cout << "\nCatalog array is full!\n";
        return;
    }

    Product* p = arr + *n;
    cout << "\nEnter Product ID (e.g. 113): ";
    cin >> p->id;
    cin.ignore();
    cout << "Enter Product Name: ";
    getline(cin, p->name);
    cout << "Enter Product Price ($): ";
    cin >> p->price;

    (*n)++;
    cout << "Product \"" << p->name << "\" successfully added!\n";
}

int main() {
    Product catalog[MAX_PRODUCTS];
    int count = 0;

    // Load initial 12 products
    loadDefaultProducts(catalog, &count);

    int choice = 0;

    do {
        cout << "\n====================================================================\n";
        cout << "       ONLINE SHOPPING PRODUCT PRICE SORTING SYSTEM (DSA-1)\n";
        cout << "====================================================================\n";
        cout << "  1. Display Products in Original (Unsorted) Order\n";
        cout << "  2. Sort by Price using Bubble Sort (Step-by-Step Trace)\n";
        cout << "  3. Sort by Price using Selection Sort (Step-by-Step Trace)\n";
        cout << "  4. Sort by Price using Insertion Sort (Step-by-Step Trace)\n";
        cout << "  5. Compare Sorting Methods (Passes, Comparisons, Swaps)\n";
        cout << "  6. Add New Product (Accept ID, Name, Price)\n";
        cout << "  7. Reset to Default 12 Products\n";
        cout << "  8. Exit System\n";
        cout << "--------------------------------------------------------------------\n";
        cout << "Enter choice (1-8): ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Please enter a number from 1 to 8.\n";
            continue;
        }

        if (choice >= 2 && choice <= 4) {
            int orderChoice;
            cout << "\nChoose sorting order:\n";
            cout << "  1. Ascending  (Lowest to Highest Price)\n";
            cout << "  2. Descending (Highest to Lowest Price)\n";
            cout << "Enter order (1 or 2): ";
            cin >> orderChoice;
            bool ascending = (orderChoice != 2);

            Product temp[MAX_PRODUCTS];
            copyProducts(catalog, temp, count);

            int comp = 0, moves = 0, passes = 0;

            if (choice == 2) {
                cout << "\n>>> Running Bubble Sort Trace <<<\n";
                bubbleSort(temp, count, ascending, true, &comp, &moves, &passes);
                displayProducts(temp, count, "SORTED CATALOG - BUBBLE SORT");
                cout << "Result: Passes = " << passes << " | Comparisons = " << comp << " | Swaps = " << moves << "\n";
            } else if (choice == 3) {
                cout << "\n>>> Running Selection Sort Trace <<<\n";
                selectionSort(temp, count, ascending, true, &comp, &moves, &passes);
                displayProducts(temp, count, "SORTED CATALOG - SELECTION SORT");
                cout << "Result: Passes = " << passes << " | Comparisons = " << comp << " | Swaps = " << moves << "\n";
            } else if (choice == 4) {
                cout << "\n>>> Running Insertion Sort Trace <<<\n";
                insertionSort(temp, count, ascending, true, &comp, &moves, &passes);
                displayProducts(temp, count, "SORTED CATALOG - INSERTION SORT");
                cout << "Result: Passes = " << passes << " | Comparisons = " << comp << " | Shifts = " << moves << "\n";
            }
        } else if (choice == 1) {
            displayProducts(catalog, count, "ORIGINAL PRODUCT CATALOG (UNSORTED)");
        } else if (choice == 5) {
            int orderChoice;
            cout << "\nChoose sorting order for comparison:\n";
            cout << "  1. Ascending  (Lowest to Highest Price)\n";
            cout << "  2. Descending (Highest to Lowest Price)\n";
            cout << "Enter order (1 or 2): ";
            cin >> orderChoice;
            bool ascending = (orderChoice != 2);
            compareAlgorithms(catalog, count, ascending);
        } else if (choice == 6) {
            addProduct(catalog, &count);
        } else if (choice == 7) {
            loadDefaultProducts(catalog, &count);
            cout << "Catalog reset to default 12 items.\n";
        } else if (choice == 8) {
            cout << "Exiting system. Thank you!\n";
        } else {
            cout << "Invalid choice! Enter 1-8.\n";
        }
    } while (choice != 8);

    return 0;
}
