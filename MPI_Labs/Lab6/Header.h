#pragma once
#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include <exception>

using namespace std;

class Large_num
{
public:
    Large_num();
    Large_num(const string&);
    Large_num(const vector<int>&);
    Large_num(const int&);
    Large_num(const long long int&);

    vector<int> resVector() const noexcept;

    Large_num(const Large_num&);
    Large_num& operator = (const Large_num&);

    const Large_num operator +() const;
    const Large_num operator -() const;

    bool operator ==(const Large_num&) const noexcept;
    bool operator != (const Large_num&) const noexcept;
    bool operator <(const Large_num&) const noexcept;
    bool operator <= (const Large_num&) const noexcept;
    bool operator >(const Large_num&) const noexcept;
    bool operator >= (const Large_num&) const noexcept;

    Large_num operator +(const Large_num&)const noexcept;
    Large_num operator -(const Large_num&) const noexcept;
    Large_num operator *(const Large_num&)const noexcept;
    Large_num operator /(const Large_num&) const;

    Large_num operator%(const Large_num&)const noexcept;

    Large_num gcd(const Large_num&);
    Large_num random();
    vector<Large_num> factorize();

    ///Toom Cook method Multiplication
    Large_num toom_cook(const Large_num& other) const;
    ///Katsuba mathod
    Large_num karatsuba(const Large_num& x);
    ///Schonhage Strassen Multiplication
    Large_num schonhage_strasen(const Large_num&) noexcept;
    ///Module Multiplication Method (Schonhage)
    Large_num shonhage(const Large_num&);
    ///Inverse Number
    Large_num inverse_number() const noexcept;
    Large_num integer_divizion_by(const Large_num&);
    ///Solovay-Strassen test
    bool solovay_strassen_primality_test();
    ///Test Miller-Rabin
    bool miller_rabin_primality_test();
    bool leman_primality_test();

    void print()const noexcept;

private:
    class DivideByZeroException : public exception {};
    vector<int> digits;
    bool is_positive;

    void rm_zeros();
    void shift_right();
    void shift_left();

    bool odd()const noexcept;///checks if the current number is even
    bool even()const noexcept;///checks if the current number is odd

    ///karatsuba method's methodes
    void extend_vector(vector<int>& a, int n);
    vector<int> vec_karatsuba(const vector<int>& a, const vector<int>& b);
    void fft(vector<int> & a, bool invert);
    ///for checken simplicity number
    Large_num legendre_symbol(const Large_num& a, const Large_num& b);
    Large_num jacobi_symbol(Large_num, Large_num, const vector<Large_num>&);

    Large_num pow(Large_num)const;
    Large_num sqrt_bi();
    Large_num to_binary() const noexcept;
    Large_num log2_ceil() const;

    vector<int> mul(const vector<int>& a, const vector<int>& b);
    vector<int> sum(const vector<int>& a, const vector<int>& b);
    vector<int> sub(const vector<int>& a, const vector<int>& b);
    void vect_rm_zeros(vector<int>& a);
    vector<int> to_vec(const string& a);
    vector<int> pow(const vector<int>& a, long long n);
    vector<int> random(const vector<int>& num);

    Large_num schonhage_strassen(const Large_num&) noexcept;
    bool lemman_primality_test();
};