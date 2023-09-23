#pragma once
#include <iostream>
#include <deque>
#include <cstring>
#include <string>

const static int MAX_INT_STORED = 999'999'999 + 1;
const static int N = 9;

class BigInteger {
private:
    std::deque<int> digits;
    bool positive = true;
public:
    BigInteger(): digits(std::deque<int>{}), positive(true) {}
    BigInteger(const char* digits) {
        size_t length = std::strlen(digits);
        if (digits[0] == '-') {
            ++digits;
            length -= 1;
            positive = false;
        }
        int rem = length % N;
        if (rem > 0) {
            this -> digits.push_front(std::stoi(std::string(digits, digits + rem)));
            digits += rem;
            length -= rem;
        }
        for (; length >= N; length -= N, digits += N) {
            this -> digits.push_front(std::stoi(std::string(digits, digits + N)));
        }
    }
    BigInteger(std::string digits) {
        size_t length = digits.length();
        if (digits[0] == '-') {
            digits = digits.substr(1, length);
            positive = false;
        }
        int rem = length % N;
        if (rem > 0) {
            this -> digits.push_front(std::stoi(digits.substr(0, rem)));
        }
        for (size_t i = 0; i < length - rem; i += N) {
            this -> digits.push_front(std::stoi(digits.substr(i, i + N)));
        }
    }
    template <typename Numeric>
    BigInteger(const Numeric& number): BigInteger(std::to_string(number)) { }
    BigInteger(const BigInteger& bi): digits(bi.digits), positive(bi.positive) {}
    BigInteger operator=(const BigInteger& other) {
        if (this == &other) { return *this; }
        positive = other.positive;
        digits = other.digits;
        return *this;
    }

    bool greater_modulo(const BigInteger&) const;
    bool operator==(const BigInteger&) const;
    bool operator!=(const BigInteger& other) const { return !(*this == other); }
    friend bool operator>(const BigInteger&, const BigInteger&);
    bool operator>=(const BigInteger& other) const { return (*this > other || *this == other); }
    bool operator<(const BigInteger& other) const { return !(*this >= other); }
    bool operator<=(const BigInteger& other) const { return !(*this > other); }

    BigInteger& operator+=(const BigInteger&);
    BigInteger operator-() const { BigInteger copy = *this; copy.positive = !positive; return copy; }
    BigInteger& operator-=(const BigInteger& other) { *this += (-other); return *this; }
    BigInteger& operator*=(int);
    BigInteger& operator*=(const BigInteger&);

    friend BigInteger operator*(BigInteger a, int digit) {
        a *= digit;
        return a;
    }

    friend BigInteger operator*(BigInteger a, const BigInteger& b) {
        a *= b;
        return a;
    }

    friend BigInteger operator+(BigInteger a, const BigInteger& b) {
        a += b;
        return a;
    }

    friend BigInteger operator-(BigInteger a, const BigInteger& b) {
        a -= b;
        return a;
    }

    friend std::ostream& operator<<(std::ostream&, const BigInteger&);
};

bool BigInteger::operator==(const BigInteger& other) const {
    if (digits.size() != other.digits.size()) { return false; }
    if (digits.front() == 0 && other.digits.front() == 0) { return true; }
    if (positive != other.positive) { return false; }
    for (size_t i = 0; i < digits.size(); ++i) {
        if (digits[i] != other.digits[i]) {
            return false;
        }
    }
    return true;
}

bool BigInteger::greater_modulo(const BigInteger& other) const {
    size_t size = digits.size(), size_ = other.digits.size();
    if (size == size_) {
        return digits.back() > other.digits.back();
    }
    return (size > size_);
}

bool greater_modulo(const BigInteger& a, const BigInteger& b) {
    return a.greater_modulo(b);
}

bool operator>(const BigInteger& a, const BigInteger& b) {
    if (a.positive == b.positive) {
        if (a.positive) {
            return greater_modulo(a, b);
        }
        return !greater_modulo(a, b);
    }
    if (a.positive) { return true; }
    return false;
}

BigInteger& BigInteger::operator+=(const BigInteger& other) {
    if (positive == other.positive) {
        int add = 0, sum;
        auto it = digits.begin();
        auto it_ = other.digits.begin();
        for (; it != digits.end() && it_ != other.digits.end(); ++it, ++it_) {
            sum = *it + *it_ + add;
            *it = sum % MAX_INT_STORED;
            add = sum / MAX_INT_STORED;
        }
        for (; it != digits.end(); ++it) {
            sum = *it + add;
            *it = sum % MAX_INT_STORED;
            add = sum / MAX_INT_STORED;
        }
        for (; it_ != other.digits.end(); ++it_) {
            sum = *it_ + add;
            digits.push_back(sum % MAX_INT_STORED);
            add = sum / MAX_INT_STORED;
        }
        if (add) {
            digits.push_back(add);
        }
        return *this;
    }
    if ((*this).greater_modulo(other)) {
        int add = 0;
        auto it = digits.begin();
        auto it_ = other.digits.begin();
        for (; it != digits.end() && it_ != other.digits.end(); ++it, ++it_) {
            if ((*it - add) < *it_) {
                *it = MAX_INT_STORED + ((*it - add) - *it_);
                add = 1;
            } else {
                *it -= *it_;
                add = 0;
            }
        }
        for (; it != digits.end(); ++it) {
            if (*it > 0) {
                *it -= add;
                add = 0;
            } else {
                *it = MAX_INT_STORED - 1;
                add = 1;
            }
        }
        return *this;
    }
    positive = other.positive;
    int add = 0;
    auto it = digits.begin();
    auto it_ = other.digits.begin();
    for (; it != digits.end() && it_ != other.digits.end(); ++it, ++it_) {
        if ((*it_ - add) < *it) {
            *it = MAX_INT_STORED + ((*it_ - add) - *it);
            add = 1;
        } else {
            *it = *it_ - *it;
            add = 0;
        }
    }
    for (; it_ != other.digits.end(); ++it_) {
        if (*it_ > 0) {
            digits.push_back(*it_ - add);
            add = 0;
        } else {
            digits.push_back(MAX_INT_STORED - 1);
            add = 1;
        }
    }
    return *this;
}

BigInteger& BigInteger::operator*=(int digit) {
    positive = !(positive ^ (digit > 0));
    long long int add = 0, prod;
    for (auto it = digits.begin(); it != digits.end(); ++it) {
        prod = static_cast<long long int>(*it) * static_cast<long long int>(digit) + add;
        *it = prod % MAX_INT_STORED;
        add = prod / MAX_INT_STORED;
    }
    if (add) { digits.push_back(static_cast<int>(add)); }
    return *this;
}

BigInteger& BigInteger::operator*=(const BigInteger& other) {
    positive = !(positive ^ other.positive);
    BigInteger copy = *this;
    BigInteger prod;
    digits.clear();
    digits.push_back(0);
    for (size_t i = 0; i < other.digits.size(); ++i) {
        prod = copy * other.digits[i];
        for (size_t c = 0; c < i; ++c) {
            prod.digits.push_back(0);
        }
        *this += prod;
    }
    return *this;
}

std::ostream& operator<<(std::ostream& out, const BigInteger& bi) {
    if (!bi.positive) {
        std::cout << "-";
    }
    auto it = bi.digits.rbegin();
    std::cout << *it;
    ++it;
    for (; it != bi.digits.rend(); ++it) {
        std::string value = std::to_string(*it);
        for (int i = 0; i < N - static_cast<int>(value.length()); ++i) { std::cout << "0"; }
        out << *it;
    }
    return out;
}