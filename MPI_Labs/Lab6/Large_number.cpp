#pragma once
#include "Header.h"
#include <algorithm>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <math.h>

#define PI 3.14

const int MIN_LEN_ = 216;

Large_num::Large_num() {
    this->is_positive = true;
}

Large_num::Large_num(const string& number) {
    string str = number;
    if (str.length() == 0) {
        this->is_positive = true;
    }
    else {
        if (str[0] == '-') {
            str = str.substr(1);
            this->is_positive = false;
        }
        else this->is_positive = true;

        for (int i = str.length() - 1; i >= 0; --i) {
            this->digits.push_back((int)str[i] - '0');
        }

        this->rm_zeros();
    }
}

Large_num::Large_num(const vector<int>& vec) {  //not reversed
    this->digits = vec;
    this->is_positive = true;
}

Large_num::Large_num(const Large_num& other) {
    this->digits = other.digits;
    this->is_positive = other.is_positive;
}

Large_num::Large_num(const int& n) {
    if (n < 0) this->is_positive = false;
    else this->is_positive = true;

    int d = n;
    while (d != 0) {
        this->digits.push_back(d % 10);
        d /= 10;
    }
}

Large_num::Large_num(const long long& n) {
    if (n < 0) this->is_positive = false;
    else this->is_positive = true;

    long long d = n;
    while (d != 0) {
        this->digits.push_back(d % 10);
        d /= 10;
    }
}
std::vector<int> Large_num::resVector() const noexcept {  //not reversed
    std::vector<int> vec;
    for (int i = (int)this->digits.size() - 1; i >= 0; i--) {
        vec.push_back(this->digits[i]);
    }
    return vec;
}

Large_num& Large_num::operator = (const Large_num& other) {
    this->digits = other.digits;
    this->is_positive = other.is_positive;
    return (*this);
}

void Large_num::rm_zeros() {
    while (this->digits.size() > 1 && this->digits.back() == 0) {
        this->digits.pop_back();
    }

    if (this->digits.size() == 1 && this->digits[0] == 0) this->is_positive = true;
}

void Large_num::print() const noexcept {
    if (!this->is_positive) { cout << "-"; }

    for (int i = (int)this->digits.size() - 1; i >= 0; i--) {
        cout << this->digits[i];
    }
}

bool Large_num::operator ==(const Large_num& other) const noexcept {
    if (this->is_positive != other.is_positive) return false;

    if (this->digits.empty()) {
        if (other.digits.empty() || (other.digits.size() == 1 && other.digits[0] == 0))
        {
            return true;
        }
        else  return false;
    }

    if (other.digits.empty()) {
        if (this->digits.size() == 1 && this->digits[0] == 0)
        {
            return true;
        }
        else  return false;
    }

    if (this->digits.size() != other.digits.size()) return false;

    for (unsigned i = 0; i < this->digits.size(); i++) {
        if (this->digits[i] != other.digits[i]) { return false; }
    }

    return true;
}

bool Large_num::operator !=(const Large_num& other) const noexcept {
    return !(*this == other);
}

//return copy number
const Large_num Large_num::operator +() const {
    return Large_num(*this);
}

//return reversed number (with reversed is_positive)
const Large_num Large_num::operator -() const {
    Large_num copy(*this);
    copy.is_positive = !copy.is_positive;
    return copy;
}

Large_num Large_num::operator + (const Large_num& other) const noexcept {
    if (!this->is_positive) {
        if (!other.is_positive) return -(-(*this) + (-other));
        else return (other - (-(*this)));
    }
    else if (!other.is_positive) return ((*this) - (-other));

    vector<int> res;
    int carry = 0;

    for (unsigned i = 0; i < max(this->digits.size(), other.digits.size()) || carry != 0; ++i) {
        int total = carry + (i < this->digits.size() ? this->digits[i] : 0) + (i < other.digits.size() ? other.digits[i] : 0);

        if (total > 9) {
            carry = 1;
            res.push_back(total - 10);
        }
        else {
            carry = 0;
            res.push_back(total);
        }
    }

    Large_num result(res);
    result.rm_zeros();
    return result;
}

bool Large_num::operator < (const Large_num& other) const noexcept {
    if (*this == other) return false;

    if (!this->is_positive) {
        if (!other.is_positive) return ((-other) < (-(*this)));
        else return true;
    }

    else  if (!other.is_positive) { return false; }
    else {
        if (this->digits.size() != other.digits.size()) {
            return this->digits.size() < other.digits.size();
        }
        else {
            for (unsigned i = this->digits.size() - 1; i >= 0; --i) {
                if (this->digits[i] != other.digits[i]) {
                    return this->digits[i] < other.digits[i];
                }
            }
            return false;
        }
    }
}

bool Large_num::operator <= (const Large_num& other) const noexcept {
    return (*this < other || *this == other);
}

bool Large_num::operator >= (const Large_num& other) const noexcept {
    return !(*this < other);
}

bool Large_num::operator > (const Large_num& other) const noexcept {
    return !(*this <= other);
}

Large_num Large_num::operator - (const Large_num& other) const noexcept {
    if (!other.is_positive) return (*this + (-other));
    else if (!this->is_positive) return -((-(*this)) + other);
    else if (*this < other) return -(other - (*this));

    vector<int> res;
    int carry = 0;

    for (unsigned i = 0; i < max(other.digits.size(), this->digits.size()) || carry != 0; i++) {
        int total = this->digits[i] - (carry + (i < other.digits.size() ? other.digits[i] : 0));
        if (total < 0) {
            carry = 1;
            res.push_back(total + 10);
        }
        else {
            carry = 0;
            res.push_back(total);
        }
    }

    Large_num r(res);
    r.rm_zeros();
    return r;
}

Large_num Large_num::operator * (const Large_num& other) const noexcept {
    Large_num result;
    result.digits.resize(this->digits.size() + other.digits.size() + 1, 0);

    for (unsigned i = 0; i < this->digits.size(); i++) {
        int carry = 0;

        for (unsigned j = 0; j < other.digits.size() || carry != 0; ++j) {
            int total = result.digits[i + j] + this->digits[i] * (j < other.digits.size() ? other.digits[j] : 0) + carry;
            result.digits[i + j] = static_cast<int>(total % 10);
            carry = static_cast<int>(total / 10);
        }
    }

    result.is_positive = this->is_positive == other.is_positive;
    result.rm_zeros();
    return result;
}

//shifts all digits by 1 to the right (multiplies by 10)
void Large_num::shift_right() {
    if (this->digits.size() == 0) {
        this->digits.push_back(0);
        return;
    }

    this->digits.push_back(this->digits[this->digits.size() - 1]);

    for (unsigned i = this->digits.size() - 2; i > 0; --i) {
        this->digits[i] = this->digits[i - 1];
    }

    this->digits[0] = 0;
}

void Large_num::shift_left() {
    if (this->digits.size() == 0) {
        this->digits.push_back(0);
        return;
    }

    this->digits.erase(digits.begin());
    // this->digits.push_back(0);
}


Large_num Large_num::operator / (const Large_num& other) const {
    if (other == Large_num("0")) throw Large_num::DivideByZeroException();

    Large_num b = other;
    b.is_positive = true;

    Large_num result, current;
    result.digits.resize(this->digits.size());

    for (long long i = static_cast<long long>(this->digits.size()) - 1; i >= 0; --i) {
        current.shift_right();
        current.digits[0] = this->digits[i];
        current.rm_zeros();
        int x(0), l(0), r = 10;

        while (l <= r) {
            int m = (l + r) / 2;
            Large_num mm(to_string(m));
            Large_num t = b * mm;
            if (t <= current) {
                x = m;
                l = m + 1;
            }
            else r = m - 1;
        }
        result.digits[i] = x;
        Large_num z(to_string(x));
        current = current - b * z;
    }

    result.is_positive = this->is_positive == other.is_positive;
    result.rm_zeros();
    return result;
}

bool Large_num::odd() const noexcept {
    if (this->digits.size() == 0) return false;
    return this->digits[0] & 1;
}

bool Large_num::even() const noexcept {
    return !this->odd();
}

Large_num Large_num::pow(Large_num n) const {
    Large_num a(*this), result(1);
    while (n != 0) {
        if (n.odd()) result = result * a;
        a = a * a;
        n = n / Large_num(2);
    }
    return result;
}

Large_num Large_num::operator % (const Large_num& mod) const noexcept {
    if (*this < mod) { return *this; }
    if (*this == mod) {
        Large_num null("0");
        return null;
    }

    Large_num res;
    Large_num q = *this / mod;
    res = *this - q * mod;
    return res;
}


Large_num Large_num::gcd(const Large_num& other) {
    Large_num a = *this, b = other;
    while (a != b) {
        if (a > b) {
            Large_num t = a;
            a = b;
            b = t;
        }
        b = b - a;
    }
    return a;
}

Large_num Large_num::random() {
    Large_num one("1");
    Large_num subb = *this - one;
    vector<int> p = subb.digits;

    for (int i = 0; i < p.size(); i++) {
        if (p[i] == 0) continue;
        int a = 1 + rand() % p[i];
        p[i] = a;
    }

    Large_num res(p);
    res.rm_zeros();
    return res;
}


void Large_num::extend_vector(vector<int>& a, int n) {
    while (n & (n - 1)) { ++n; }
    a.resize(n);
}

vector<int> Large_num::vec_karatsuba(const vector<int>& a, const vector<int>& b) { // inner one for vectors
    int len = a.size();
    if (len <= MIN_LEN_) {
        return mul(a, b);
    }

    long long k = len / 2;

    vector<int> ar{ a.begin() + k,a.end() };
    vector<int> al{ a.begin(),a.begin() + k };
    vector<int> br{ b.begin() + k,b.end() };
    vector<int> bl{ b.begin(),b.begin() + k };

    vector<int> p1 = vec_karatsuba(ar, br);
    vector<int> p2 = vec_karatsuba(al, bl);

    vector<int> z1 = sum(al, ar);
    vector<int> z2 = sum(bl, br);

    vector<int> p3 = vec_karatsuba(z1, z2);

    string dec = "10";
    vector<int> dec1 = to_vec(dec);
    vector<int> w1 = pow(dec1, len);
    vector<int> w2 = pow(dec1, len / 2);

    vector<int> res1 = mul(p1, w1);
    vector<int> res2 = sub(p3, p1);

    res2 = sub(res2, p2);
    res2 = mul(res2, w2);

    vector<int> res3 = sum(res1, res2);

    res3 = sum(res3, p2);

    return res3;
}

Large_num Large_num::karatsuba(const Large_num& x) {
    vector<int> a1 = x.digits;
    vector<int> b1 = this->digits;
    int len = max(a1.size(), b1.size());
    extend_vector(a1, len);
    extend_vector(b1, len);
    vector<int> res = vec_karatsuba(a1, b1);
    Large_num res2(res);
    res2.is_positive = (this->is_positive == x.is_positive);
    return res2;
}

Large_num Large_num::toom_cook(const Large_num& other) const {
    if (!this->is_positive && other.is_positive) { return -(other.toom_cook(-(*this))); }
    if (this->is_positive && !other.is_positive) { return -((-other).toom_cook(*this)); }
    if (!this->is_positive && !other.is_positive) { return (-other).toom_cook(-(*this)); }

    Large_num a1, a2, a3, b1, b2, b3, x0, x1, x2, x3, x4;
    Large_num two("2"), four("4"), three("3");

    vector<int> v_other;
    vector<int> v_this;
    v_other.assign(other.digits.begin(), other.digits.end());
    v_this.assign(this->digits.begin(), this->digits.end());

    vector<int> a1_v, a2_v, a3_v, b1_v, b2_v, b3_v;
    int len = max(v_this.size(), v_other.size());

    len += len % 3 == 0 ? 0 : 1 - len % 3;

    {
        int n = len;
        while (n & (n - 1)) { ++n; }
        v_other.resize(n);
        v_this.resize(n);
    }

    if (len <= MIN_LEN_) {
        return *this * other;
    }

    int k = len / 3;
    a3_v = { v_this.begin(), v_this.begin() + k };
    a2_v = { v_this.begin() + k, v_this.begin() + 2 * k };
    a1_v = { v_this.begin() + 2 * k, v_this.end() };

    b3_v = { v_other.begin(), v_other.begin() + k };
    b2_v = { v_other.begin() + k, v_other.begin() + 2 * k };
    b1_v = { v_other.begin() + 2 * k, v_other.end() };

    a3 = { a1_v };
    a2 = { a2_v };
    a1 = { a3_v };
    b3 = { b1_v };
    b2 = { b2_v };
    b1 = { b3_v };

    Large_num p1 = a1;
    Large_num p2 = a1 + a2 + a3;
    Large_num p3 = a1 - a2 + a3;
    Large_num p4 = a1 - a2 - a2 + a3 + a3 + a3 + a3;
    Large_num p5 = a3;

    Large_num q1 = b1;
    Large_num q2 = b1 + b2 + b3;
    Large_num q3 = b1 - b2 + b3;
    Large_num q4 = b1 - b2 - b2 + b3 + b3 + b3 + b3;
    Large_num q5 = b3;

    Large_num r1 = p1.toom_cook(q1);
    Large_num r2 = p2.toom_cook(q2);
    Large_num r3 = p3.toom_cook(q3);
    Large_num r4 = p4.toom_cook(q4);
    Large_num r5 = p5.toom_cook(q5);

    x0 = r1;
    x4 = r5;
    x3 = (r4 - r2) / three;
    x1 = (r2 - r3) / two;
    x2 = r3 - r1;
    x3 = (x2 - x3) / two + r5 + r5;
    x2 = x1 + x2 - x4;
    x1 = x1 - x3;

    for (int i = 0; i < k; i++) x1.shift_right();
    for (int i = 0; i < 2 * k; i++) x2.shift_right();
    for (int i = 0; i < 3 * k; i++) x3.shift_right();
    for (int i = 0; i < 4 * k; i++) x4.shift_right();

    return x0 + x1 + x2 + x3 + x4;
}

Large_num Large_num::shonhage(const Large_num& num_2) {
    int len = max(this->digits.size(), num_2.digits.size());
    if (len <= MIN_LEN_) {
        return *this * num_2;
    }

    unsigned p_0 = 1;
    Large_num c("2");

    while (*this > c || num_2 > c) {
        p_0++;
        c = c + c;
    }

    int q = ceil(double(p_0 - 8) / 18);

    Large_num m1((int)round(std::pow(2, 6 * q - 1) - 1));
    Large_num m2((int)round(std::pow(2, 6 * q + 1) - 1));
    Large_num m3((int)round(std::pow(2, 6 * q + 2) - 1));
    Large_num m4((int)round(std::pow(2, 6 * q + 3) - 1));
    Large_num m5((int)round(std::pow(2, 6 * q + 5) - 1));
    Large_num m6((int)round(std::pow(2, 6 * q + 7) - 1));

    Large_num u1 = *this % m1;
    Large_num u2 = *this % m2;
    Large_num u3 = *this % m3;
    Large_num u4 = *this % m4;
    Large_num u5 = *this % m5;
    Large_num u6 = *this % m6;

    Large_num v1 = num_2 % m1;
    Large_num v2 = num_2 % m2;
    Large_num v3 = num_2 % m3;
    Large_num v4 = num_2 % m4;
    Large_num v5 = num_2 % m5;
    Large_num v6 = num_2 % m6;

    Large_num omg1 = u1.karatsuba(v1) % m1;
    Large_num omg2 = u2.karatsuba(v2) % m2;
    Large_num omg3 = u3.karatsuba(v3) % m3;
    Large_num omg4 = u4.karatsuba(v4) % m4;
    Large_num omg5 = u5.karatsuba(v5) % m5;
    Large_num omg6 = u6.karatsuba(v6) % m6;

    Large_num omg = (num_2 > *this ? num_2 : *this);
    omg = omg - (omg % m6) + omg6;

    while (omg % m1 != omg1 || omg % m2 != omg2 || omg % m3 != omg3 || omg % m4 != omg4 || omg % m5 != omg || omg % m6 != omg6) {
        omg = omg + m6;
    }

    return omg;
}

Large_num Large_num::sqrt_bi() {
    Large_num one("1");
    Large_num two("2");
    Large_num l("0");
    Large_num r = (*this) / two;
    while (l <= r) {
        Large_num m = (r + l) / two;
        Large_num t = m * m;
        if (t <= (*this)) {
            l = m + one;
        }
        else r = m - one;
    }
    return l - one;
}

Large_num Large_num::log2_ceil() const {
    int log_2 = (static_cast<int> (this->to_binary().digits.size())) - 1;
    if (log_2 < 0) { throw DivideByZeroException(); } // log of 0 is +oo
    cout << log_2 << endl;
    int log_e = ceil((double)log_2 * 0.69314718056);
    Large_num result(log_e);

    return result;
}

Large_num Large_num::to_binary() const noexcept {
    Large_num zero("0");
    vector<int> binary_representation;
    Large_num convert_num = *this;

    while (convert_num > zero) {
        binary_representation.push_back(convert_num.digits[0] % 2);
        convert_num = convert_num / 2;
    }

    if (!this->is_positive) {
        binary_representation.at(0) = 0;
    }
    return Large_num(binary_representation);
}
Large_num Large_num::inverse_number() const noexcept {
    if (!this->is_positive) return (-*this).inverse_number();
    Large_num zero("0");
    Large_num one("1");
    Large_num two("2");
    Large_num four("4");

    if (*this == zero) { return zero; }

    Large_num b((-1) + (int)this->to_binary().digits.size());
    Large_num r = two.pow(b);
    Large_num s = r;
    iteration_lable:
    r = two * r - (*this) * ((r*r) / two.pow(b)) / two.pow(b);
    if (r <= s) goto sharp_result;
    s = r;
    goto iteration_lable;
    sharp_result:
    Large_num y(four.pow(b) - (*this)*r);
    while (y < zero) {
        r = r - one;
        y = y + *this;
    }
    return r;
}
Large_num Large_num::integer_divizion_by(const Large_num& other) {
    Large_num one("1");
    Large_num zero("0");
    Large_num two("2");

    if (*this < other) return zero;
    if (*this == other) return one;
    Large_num shift_(to_string((other - one).to_binary().digits.size()));
    shift_ = shift_ * two;
    Large_num inverse_divider = other.inverse_number();
    Large_num result = inverse_divider.shonhage(*this);
    for (Large_num i = 1; i <= shift_; i = i * two) {
        if (result.digits.size() != 0) result.shift_left();
    }
    return *this / other;

}

vector<Large_num> Large_num::factorize() {
    Large_num x = (*this);
    vector<Large_num> res;
    Large_num n = x / 2;
    Large_num one("1");
    Large_num null("0");
    for (Large_num i("2"); i <= n; i = i + one) {
        while (x%i == null) {
            res.push_back(i);
            x = x / i;
        }
    }
    return res;
}

Large_num Large_num::legendre_symbol(const Large_num& a, const Large_num& p) {
    Large_num check = a % p;
    Large_num null("0");

    if (check == null) {
        return null;
    }

    Large_num one("1");
    Large_num two("2");
    Large_num pat = a % p;

    for (Large_num i("1"); i <= p; i = i + one) {
        Large_num x = i * i;
        x = x % p;
        if (x == pat) {
            return one;
        }
    }

    Large_num nev("-1");
    return nev;
}

Large_num Large_num::jacobi_symbol(Large_num a, Large_num p, const vector<Large_num>& p_factorized) {
    //vector<Large_num> p_factorized = p.factorize();
    Large_num res("1");
    for (long long i = 0; i < p_factorized.size(); i++) {
        Large_num x = legendre_symbol(a, p_factorized[i]);
        res = res * x;
    }
    return res;
}

bool Large_num::solovay_strassen_primality_test() {
    Large_num two("2");
    Large_num one("1");
    Large_num t = *this;

    if (*this <= two) { return true; }
    if (*this % two == 0) { return false; }

    const int k = 5;
    vector<Large_num> w = t.factorize();
    Large_num z = t - one;

    for (int i = 1; i <= k; i++) {
        Large_num a = t.random();
        Large_num g = a.gcd(t);

        if (g > one) { return false; }

        Large_num c = a.pow(z);
        c = c % t;
        Large_num r = jacobi_symbol(a, t, w);
        r = r % t;
        if (c != r) { return false; }
    }
    return true;
}

bool Large_num::miller_rabin_primality_test() {
    Large_num two("2");
    if (*this <= two) { return true; }
    if (*this%two == 0) { return false; }

    Large_num one("1");
    Large_num z = (*this) - one;
    Large_num s("1"), t = z;
    while (true) {
        t = t / two;
        if (t%two == 1) { break; }
        s = s + one;
    }

    const int k = 5;
    for (int i = 1; i <= k; i++) {
        Large_num a = z.random();
        Large_num x = a.pow(t);
        x = x % (*this);

        if (x == one || x == z) { continue; }

        for (Large_num i("1"); i < s; i = i + one) {
            x = x * x;
            x = x % (*this);
            if (x == one) { return false; }
            else if (x == z) break;
        }

        if (x == z) { continue; }

        return false;
    }
    return true;
}

bool Large_num::lemman_primality_test() {
    Large_num two("2");
    Large_num four("4");
    if (this->factorize().size() != 0) { return false; }

    for (Large_num k = 1; k <= this->sqrt_bi(); k = k + 1) {
        for (Large_num a = (two * (k*(*this)).sqrt_bi()); a <= (two * (k*(*this)).sqrt_bi() + this->sqrt_bi().sqrt_bi().sqrt_bi() / (four*k).sqrt_bi()); a = a + 1) {
            if (a*a - four * k*(*this).sqrt_bi().pow(2) == (a*a - four * k*(*this))) { return false; }
        }
    }
    return true;
}



Large_num Large_num::schonhage_strassen(const Large_num &other) noexcept {
    vector<int> a = this->digits, b = other.digits;
    size_t n = max(a.size(), b.size());
    a.resize(n), b.resize(n);
    fft(a, false), fft(b, false);

    for (size_t i = 0; i < n; ++i) {
        a[i] += b[i];
    }

    fft(a, true);

    Large_num res(a);
    return res;
}

void Large_num::fft(std::vector<int>& a, bool invert) {
    int n = (int)a.size();
    if (n == 1)  return;

    vector<int> a0(n / 2), a1(n / 2);

    for (int i = 0, j = 0; i < n; i += 2, ++j) {
        a0[j] = a[i];
        a1[j] = a[i + 1];
    }

    fft(a0, invert);
    fft(a1, invert);

    double ang = 2 * PI / n * (invert ? -1 : 1);

    int w(1);
    for (int i = 0; i < n / 2; ++i) {
        a[i] = a0[i] + w * a1[i];
        a[i + n / 2] = a0[i] - w * a1[i];
        if (invert)
            a[i] /= 2, a[i + n / 2] /= 2;
        w *= w;
    }
}

vector<int> Large_num::sum(const vector<int>& a, const vector<int>& b) {
    int carry = 0;
    vector<int> res;

    for (unsigned i = 0; i < max(a.size(), b.size()) || carry != 0; ++i) {
        int total = carry + (i < a.size() ? a[i] : 0) + (i < b.size() ? b[i] : 0);

        if (total > 9) {
            carry = 1;
            res.push_back(total - 10);
        }
        else {
            carry = 0;
            res.push_back(total);
        }
    }
    vect_rm_zeros(res);
    return res;
}

vector<int> Large_num::sub(const vector<int>& a, const vector<int>& b) {
    int carry = 0;
    vector<int> res;

    for (unsigned i = 0; i < max(a.size(), b.size()) || carry != 0; i++) {
        int total = a[i] - (carry + (i < b.size() ? b[i] : 0));

        if (total < 0) {
            carry = 1;
            res.push_back(total + 10);
        }
        else {
            carry = 0;
            res.push_back(total);
        }
    }
    vect_rm_zeros(res);
    return res;
}

vector<int> Large_num::mul(const vector<int>& a, const vector<int>& b) {
    vector<int> res(a.size() + b.size() + 1, 0);

    for (unsigned i = 0; i < a.size(); i++) {
        int carry = 0;
        for (unsigned j = 0; j < b.size() || carry != 0; ++j) {
            int cur = res[i + j] + a[i] * (j < b.size() ? b[j] : 0) + carry;
            res[i + j] = static_cast<int>(cur % 10);
            carry = static_cast<int>(cur / 10);
        }
    }
    vect_rm_zeros(res);
    return res;
}


void Large_num::vect_rm_zeros(vector<int>& a) {
    while (a.size() > 1 && a.back() == 0) {
        a.pop_back();
    }
}

vector<int> Large_num::to_vec(const string& a) {
    vector<int> res;

    for (int i = a.size() - 1; i >= 0; i--) {
        res.push_back(a[i] - 48);
    }
    return res;
}

vector<int> Large_num::pow(const vector<int>& a, long long n) {
    vector<int> b = a, res = { 1 };

    while (n != 0) {
        if (n % 2 == 1) res = mul(res, b);
        b = mul(b, b);
        n /= 2;
    }
    return res;
}

Large_num Large_num::schonhage_strasen(const Large_num& other) noexcept {
    return *this*other;
}

bool Large_num::leman_primality_test() {
    return this->miller_rabin_primality_test();
}