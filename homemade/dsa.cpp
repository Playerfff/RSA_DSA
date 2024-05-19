#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <string>
#include <compare>

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <compare>

class BigInt {
public:
    std::string value;

    BigInt(std::string val = "0") : value(val) {
        if (value[0] == '-' && value.size() == 1) {
            value = "0";
        }
    }

    BigInt(long long val) : value(std::to_string(val)) {}

    BigInt operator+(const BigInt& other) const {
        if (value[0] == '-' && other.value[0] != '-') {
            return other - BigInt(value.substr(1));
        } else if (value[0] != '-' && other.value[0] == '-') {
            return *this - BigInt(other.value.substr(1));
        } else if (value[0] == '-' && other.value[0] == '-') {
            return BigInt('-' + add(value.substr(1), other.value.substr(1)));
        } else {
            return BigInt(add(value, other.value));
        }
    }

    BigInt operator-(const BigInt& other) const {
        if (value[0] == '-' && other.value[0] != '-') {
            return BigInt('-' + add(value.substr(1), other.value));
        } else if (value[0] != '-' && other.value[0] == '-') {
            return *this + BigInt(other.value.substr(1));
        } else if (value[0] == '-' && other.value[0] == '-') {
            return BigInt(subtract(other.value.substr(1), value.substr(1)));
        } else {
            return BigInt(subtract(value, other.value));
        }
    }

    BigInt operator*(const BigInt& other) const {
        bool negative = (value[0] == '-' && other.value[0] != '-') ||
                        (value[0] != '-' && other.value[0] == '-');
        std::string result = multiply(positiveValue(), other.positiveValue());
        if (negative && result != "0") {
            result = '-' + result;
        }
        return BigInt(result);
    }

    BigInt operator/(const BigInt& other) const {
        bool negative = (value[0] == '-' && other.value[0] != '-') ||
                        (value[0] != '-' && other.value[0] == '-');
        std::string result = divide(positiveValue(), other.positiveValue());
        if (negative && result != "0") {
            result = '-' + result;
        }
        return BigInt(result);
    }

    BigInt operator%(const BigInt& other) const {
        return *this - (*this / other) * other;
    }

    BigInt operator<<(int shift) const {
        if (shift < 0) return *this >> (-shift);
        std::string shiftedValue = value;
        shiftedValue.append(shift, '0');
        return BigInt(shiftedValue);
    }

    BigInt operator>>(int shift) const {
        if (shift < 0) return *this << (-shift);
        return BigInt(value.substr(0, value.size() - shift));
    }

    std::strong_ordering operator<=>(const BigInt& other) const {
        if (value[0] == '-' && other.value[0] != '-') return std::strong_ordering::less;
        if (value[0] != '-' && other.value[0] == '-') return std::strong_ordering::greater;

        std::string thisVal = positiveValue();
        std::string otherVal = other.positiveValue();

        if (thisVal.size() != otherVal.size()) {
            if (value[0] == '-') {
                return thisVal.size() < otherVal.size() ? std::strong_ordering::greater : std::strong_ordering::less;
            } else {
                return thisVal.size() < otherVal.size() ? std::strong_ordering::less : std::strong_ordering::greater;
            }
        }

        for (size_t i = 0; i < thisVal.size(); ++i) {
            if (thisVal[i] != otherVal[i]) {
                if (value[0] == '-') {
                    return thisVal[i] < otherVal[i] ? std::strong_ordering::greater : std::strong_ordering::less;
                } else {
                    return thisVal[i] < otherVal[i] ? std::strong_ordering::less : std::strong_ordering::greater;
                }
            }
        }

        return std::strong_ordering::equal;
    }

    bool operator==(const BigInt& other) const = default;

private:
    std::string positiveValue() const {
        return value[0] == '-' ? value.substr(1) : value;
    }

    static std::string add(const std::string& a, const std::string& b) {
        std::string result;
        int carry = 0, sum = 0;
        int i = a.size() - 1, j = b.size() - 1;

        while (i >= 0 || j >= 0 || carry) {
            sum = carry;
            if (i >= 0) sum += a[i--] - '0';
            if (j >= 0) sum += b[j--] - '0';
            carry = sum / 10;
            result.push_back(sum % 10 + '0');
        }

        std::reverse(result.begin(), result.end());
        return result;
    }

    static std::string subtract(const std::string& a, const std::string& b) {
        if (a == b) return "0";
        bool negative = a < b;
        std::string bigger = negative ? b : a;
        std::string smaller = negative ? a : b;

        std::string result;
        int borrow = 0, diff = 0;
        int i = bigger.size() - 1, j = smaller.size() - 1;

        while (i >= 0) {
            diff = (bigger[i--] - '0') - (j >= 0 ? (smaller[j--] - '0') : 0) - borrow;
            borrow = diff < 0 ? 1 : 0;
            if (diff < 0) diff += 10;
            result.push_back(diff + '0');
        }

        while (result.size() > 1 && result.back() == '0') result.pop_back();
        if (negative) result.push_back('-');
        std::reverse(result.begin(), result.end());
        return result;
    }

    static std::string multiply(const std::string& a, const std::string& b) {
        std::vector<int> result(a.size() + b        .size(), 0);
        for (int i = a.size() - 1; i >= 0; --i) {
            for (int j = b.size() - 1; j >= 0; --j) {
                result[i + j + 1] += (a[i] - '0') * (b[j] - '0');
                result[i + j] += result[i + j + 1] / 10;
                result[i + j + 1] %= 10;
            }
        }

        std::string product;
        for (int num : result) {
            if (!(product.empty() && num == 0)) product.push_back(num + '0');
        }

        return product.empty() ? "0" : product;
    }

    static std::string divide(const std::string& dividend, const std::string& divisor) {
        if (divisor == "0") throw std::invalid_argument("Division by zero");

        std::string quotient;
        std::string current = "0";
        for (char digit : dividend) {
            current.push_back(digit);
            int count = 0;
            while (compare(current, divisor) >= 0) {
                current = subtract(current, divisor);
                ++count;
            }
            quotient.push_back(count + '0');
        }

        while (quotient.size() > 1 && quotient[0] == '0') quotient.erase(0, 1);
        return quotient.empty() ? "0" : quotient;
    }

    static int compare(const std::string& a, const std::string& b) {
        if (a.size() != b.size()) return a.size() > b.size() ? 1 : -1;
        return a.compare(b);
    }
};

std::ostream& operator<<(std::ostream& os, const BigInt& num) {
    os << num.value;
    return os;
}

BigInt powerMod(const BigInt& base, const BigInt& exp, const BigInt& mod) {
    BigInt result(1);
    BigInt b = base % mod;
    BigInt e = exp;

    while (e > BigInt(0)) {
        if (e % BigInt(2) == BigInt(1)) {
            result = (result * b) % mod;
        }
        b = (b * b) % mod;
        e = e / BigInt(2);
    }

    return result;
}

BigInt modInverse(const BigInt& a, const BigInt& m) {
    BigInt m0 = m;
    BigInt t, q;
    BigInt x0(0), x1(1);

    BigInt a_copy = a;
    BigInt m_copy = m;

    while (a_copy > BigInt(1)) {
        q = a_copy / m_copy;
        t = m_copy;
        m_copy = a_copy % m_copy;
        a_copy = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }

    if (x1 < BigInt(0)) {
        x1 = x1 + m0;
    }

    return x1;
}

bool isPrime(const BigInt& n) {
    // Implement a prime checking algorithm for BigInt
    // Placeholder implementation, actual prime checking needs more work
    long long val = std::stoll(n.value);
    if (val <= 1) return false;
    if (val <= 3) return true;
    if (val % 2 == 0 || val % 3 == 0) return false;
    for (long long i = 5; i * i <= val; i += 6) {
        if (val % i == 0 || val % (i + 2) == 0) return false;
    }
    return true;
}

BigInt findPrime(const BigInt& start, const BigInt& end) {
    srand(time(0));
    while (true) {
        long long diff = std::stoll(end.value) - std::stoll(start.value) + 1;
        long long randVal = std::stoll(start.value) + rand() % diff;
        BigInt prime(std::to_string(randVal));
        if (isPrime(prime)) {
            return prime;
        }
    }
}

void generateDSAKeys(BigInt& p, BigInt& q, BigInt& g, BigInt& x, BigInt& y) {
    q = findPrime(BigInt(1) << 159, (BigInt(1) << 160) - BigInt(1));
    p = findPrime(BigInt(1) << 511, (BigInt(1) << 512) - BigInt(1));

    while ((p - BigInt(1)) % q != BigInt(0)) {
        p = findPrime(BigInt(1) << 511, (BigInt(1) << 512) - BigInt(1));
    }

    g = powerMod(BigInt(2), (p - BigInt(1)) / q, p);
    x = BigInt(rand()) % q;
    y = powerMod(g, x, p);
}

std::pair<BigInt, BigInt> signMessage(const BigInt& p, const BigInt& q, const BigInt& g, const BigInt& x, const BigInt& message) {
    BigInt k;
    do {
        k = BigInt(rand()) % q;
    } while (k == BigInt(0));
    BigInt r = powerMod(g, k, p) % q;
    BigInt k_inv = modInverse(k, q);
    if (k_inv == BigInt(0)) {
        std::cerr << "Failed to compute k inverse." << std::endl;
        exit(1);
    }
    BigInt s = (k_inv * (message + x * r)) % q;
    return {r, s};
}

bool verifySignature(const BigInt& p, const BigInt& q, const BigInt& g, const BigInt& y, const BigInt& message, const BigInt& r, const BigInt& s) {
    if (r <= BigInt(0) || r >= q || s <= BigInt(0) || s >= q) return false;
    BigInt w = modInverse(s, q);
    if (w == BigInt(0)) {
        std::cerr << "Failed to compute w." << std::endl;
        return false;
    }
    BigInt u1 = (message * w) % q;
    BigInt u2 = (r * w) % q;
    BigInt v = ((powerMod(g, u1, p) * powerMod(y, u2, p)) % p) % q;
    return v == r;
}

int main() {
    BigInt p, q, g, x, y;
    generateDSAKeys(p, q, g, x, y);

    std::cout << "Public Key (p, q, g, y): " << p << ", " << q << ", " << g << ", " << y << std::endl;
    std::cout << "Private Key (x): " << x << std::endl;

    BigInt message = 12345; // Example message
    auto [r, s] = signMessage(p, q, g, x, message);

    std::cout << "Signature (r, s): " << r << ", " << s << std::endl;

    bool valid = verifySignature(p, q, g, y, message, r, s);
    std::cout << "Signature valid: " << std::boolalpha << valid << std::endl;

    return 0;
}
