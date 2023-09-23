#include "bigint.h"
#include "test.h"

int main() {
    std::cout << "\n=======================\n\n";
    test_constructors_and_output();
    std::cout << "\n=======================\n\n";
    if (test_operations("test.txt")) {
        std::cout << "operations worked\n";
    }
    std::cout << "\n=======================\n\n";
    compute_and_print_100_fib_numbers();
    return 0;
}