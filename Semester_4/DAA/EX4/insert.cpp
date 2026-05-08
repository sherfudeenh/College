#include <iostream>
using namespace std;

void insertionSort(int A[], int n) {
    for (int i = 1; i < n; i++) {
        int v = A[i];
        int j = i - 1;

        while (j >= 0 && A[j] > v) {
            A[j + 1] = A[j];
            j = j - 1;
        }
        A[j + 1] = v;
    }
}

int main() {
    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    int A[n];

    cout << "Enter elements:\n";
    for (int i = 0; i < n; i++) {
        cin >> A[i];
    }

    insertionSort(A, n);

    cout << "Sorted array:\n";
    for (int i = 0; i < n; i++) {
        cout << A[i] << " ";
    }

    return 0;
}
