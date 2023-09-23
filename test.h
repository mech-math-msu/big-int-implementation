#include <fstream>
#include "bigint.h"

bool test_operation(const BigInteger& a, const BigInteger& b, const BigInteger& result, char op) {
    std::cout << a << " " << op << " " << b << " = ";
    switch (op) {
    case '+':
        std::cout << (a + b) << "\n";
        return (a + b) == result;
    case '-':
        std::cout << (a - b) << "\n";
        return (a - b) == result;
    case '*':
        std::cout << (a * b) << "\n";
        return (a * b) == result;
    default:
        return true;
    }
}

bool test_operations(const std::string& filename) {
    std::ifstream file;
    file.open(filename);
    std::string a, b, result;
    char op, eq;
    while (true) {
        file >> a >> op >> b >> eq >> result;
        if (file.eof()) { break; }
        if (!test_operation(BigInteger(a), BigInteger(b), BigInteger(result), op)) {
            return false;
        }
    }
    return true;
}

void test_constructors_and_output() {
    BigInteger a("1");
    BigInteger b("13183918391");
    BigInteger c(std::string("13183918"));
    BigInteger d(43764832);
    BigInteger e(b);
    BigInteger j = d;
    std::cout << "small integer: " << a << "\n";
    std::cout << "constructed from const char*: " << b << "\n";
    std::cout << "constructed from std::string: " << c << "\n";
    std::cout << "constructed from int: " << d << "\n";
    std::cout << "copy constructor: " << e << "\n";
    std::cout << "assignment operator: " << j << "\n";
}

void compute_and_print_100_fib_numbers() {
    BigInteger t1(0);
    BigInteger t2(1);
    BigInteger tmp;

    std::cout << "100 fibonacci nums:\n";

    for (int i = 0; i <= 200; ++i) {
        std::cout << t1 << "\n";
        tmp = t2;
        t2 += t1;
        t1 = tmp;
    }
}