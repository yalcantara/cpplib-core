
#pragma once

#include <cmath>
#include <vector>
#include <ylib/core/lang.h>

namespace ylib{
namespace math{

const UInt32 MATRIX_PRINT_MAX = 1000;



template <typename T = float>
class Matrix{
    vector<T> _arr;
    const UInt32 m;
    const UInt32 n;

    string boundsMsg(const char* varName, UInt32 val, UInt32 max) const {
        stringstream ss;
        ss << "The param ";
        ss << varName;
        ss << " must be lower than ";
        ss << std::to_string(max);
        ss << ". Got: ";
        ss << std::to_string(val);
        ss << ".";
        return ss.str();
    }

    void checkBounds(UInt32 i, UInt32 j)const{
        if(i >= m){
            throw Exception(boundsMsg("i", i, m));
        }

        if(j >= n){
            throw Exception(boundsMsg("j", j, n));
        }
    }

public:
    Matrix(UInt32 rows, UInt32 cols):m{rows}, n{cols}{
        UInt32 s = size();
        _arr.resize(s, 0);
    }

    T get(UInt32 i, UInt32 j) const {
        checkBounds(i, j);
        return _arr[i * n + j];
    }

    void set(UInt32 i, UInt32 j, T val){
        checkBounds(i, j);
        _arr[i * n + j] = val;
    }

    T at(UInt32 pos) const {
        UInt32 s = size();

        if(pos >= s){
            stringstream ss;
            ss << "The param pos must be lower than ";
            ss << std::to_string(size());
            ss << ". Got: ";
            ss << std::to_string(pos);
            ss << ".";
            throw Exception(ss.str());
        }

        return _arr[pos];
    }

    void at(UInt32 pos, T val){
        UInt32 s = size();

        if(pos >= s){
            stringstream ss;
            ss << "The param pos must be lower than ";
            ss << std::to_string(size());
            ss << ". Got: ";
            ss << std::to_string(pos);
            ss << ".";
            throw Exception(ss.str());
        }

        _arr[pos] = val;
    }

    void setRangei(T max, T min, T targetMax, T targetMin){

        UInt32 s = size();

        double range        = ((double)max) - ((double)min);
        double targetRange  = ((double)targetMax) - ((double)targetMin);
        double targetStart  = (double)targetMin;

        for(UInt32 i =0; i < s; i++){
            double x = (double)_arr[i];
            double z = ((x / range) * targetRange) + targetStart;
            _arr[i] = (T)z;
        }

    }

    UInt32 rows() const {
        return m;
    }

    UInt32 cols() const {
        return n;
    }

    UInt32 size() const{
        return m * n;
    }

    T min(){
        UInt32 s = size();

        if(s == 0){
            throw Exception("Since there is no element, there is no minumun.");
        }

        T ans = at(0);
        for(UInt32 i = 1; i < s; i++){
            T crt = at(i);

            if(crt < ans){
                ans = crt;
            }
        }

        return ans;
    }

    T max(){
        UInt32 s = size();

        if(s == 0){
            throw Exception("Since there is no element, there is no maximun.");
        }

        T ans = at(0);
        for(UInt32 i = 1; i < s; i++){
            T crt = at(i);

            if(crt > ans){
                ans = crt;
            }
        }

        return ans;
    }

    template<typename B>
    Matrix<B> toRange( T crtMin, T crtMax, B min, B max){
        UInt32 s = size();
        if(s == 0){
            throw Exception("Empty matrix, thus can not apply range.");
        }

        double dcrtMin = crtMin;
        double crtDiff = ((double)crtMax) - ((double)crtMin);

        double diff = ((double)max) - ((double)min);

        double dmin = min;
        Matrix<B> ans(m, n);
        for(UInt32 i = 0; i < s; i++){
            double val = at(i);

            double newVal = ((val - dcrtMin) / crtDiff) * diff + min;

            ans.at(i, (B)newVal);
        }

        return ans;
    }

    double sum(){
        UInt32 s = size();

        if(s == 0){
            return 0.0;
        }

        //let's start using the first element, and let operator overlading do its job
        double ans = (double)at(0); 
        for(UInt32 i = 1; i < s; i++){
            ans += (double)at(i);
        }

        return ans;
    }

    Matrix hflip(){

        UInt32 m = rows();
        UInt32 n = cols();

        Matrix ans(m, n);



        for(UInt32 i =0; i < m; i++){
            for(UInt32 j = 0; j < n; j++){
                UInt32 destj = n - j - 1;

                T val = get(i, j);
                ans.set(i, destj, val);
            }
        }

        return ans;
    }


    T* ptr(){
        return _arr.data();
    }

    void print() const {
    
        UInt32 rows = std::min(MATRIX_PRINT_MAX, m);
        UInt32 cols = std::min(MATRIX_PRINT_MAX, n);
        
        if (m > MATRIX_PRINT_MAX || cols > MATRIX_PRINT_MAX) {
            printf("Matrix  %dx%d   (truncated)\n", (int) m, (int) n);
        } else {
            printf("Matrix  %dx%d\n", (int) m, (int) n);
        }
        for (UInt32 i = 0; i < rows; i++) {
            for (UInt32 j = 0; j < cols; j++) {
                double val = (double)_arr[i * n + j];
                printf("%12.4f", val);
                if (j + 1 < n) {
                    printf("  ");
                }
            }
            println();
        }
        
        println();
    }

    ~Matrix(){
        //do nothing
    }
};

template <typename T = float>
class Tensor3{
private:
    vector<T>    _arr;
    const UInt32 m;
    const UInt32 n;
    const UInt32 p;

    string boundsMsg(const char* varName, UInt32 val, UInt32 max){
        stringstream ss;
        ss << "The param ";
        ss << varName;
        ss << " must be lower than ";
        ss << std::to_string(max);
        ss << ". Got: ";
        ss << std::to_string(val);
        ss << ".";
        return ss.str();
    }

    void checkBounds(UInt32 i, UInt32 j, UInt32 k){
        if(i >= m){
            throw Exception(boundsMsg("i", i, m));
        }

        if(j >= n){
            throw Exception(boundsMsg("j", j, n));
        }

        if(k >= p){
            throw Exception(boundsMsg("k", k, p));
        }
    }

public:
    Tensor3(UInt32 rows, UInt32 cols, UInt32 depth):m{rows}, n{cols}, p{depth}{
        UInt32 s = size();
        _arr.resize(s, 0);
    }

    T get(UInt32 i, UInt32 j, UInt32 k) const {
        checkBounds(i, j, k);
        UInt32 pos = (i * m) + j + (k * m * n);
        return _arr[pos];
    }

    void set(UInt32 i, UInt32 j, UInt32 k, T val){
        checkBounds(i, j, k);
        UInt32 pos = (i * m) + j + (k * m * n);
        _arr[pos] = val;
    }

    T at(UInt32 pos) const {
        UInt32 s = size();

        if(pos >= s){
            stringstream ss;
            ss << "The param pos must be lower than ";
            ss << std::to_string(size());
            ss << ". Got: ";
            ss << std::to_string(pos);
            ss << ".";
            throw Exception(ss.str());
        }

        return _arr[pos];
    }

    template<typename B>
    Tensor3<B> toRange( T crtMin, T crtMax, B min, B max) const {
        UInt32 s = size();
        if(s == 0){
            throw Exception("Empty tensor, thus can not apply range.");
        }

        double dcrtMin = crtMin;
        double crtDiff = ((double)crtMax) - ((double)crtMin);

        double diff = ((double)max) - ((double)min);

        double dmin = min;
        Tensor3<B> ans(m, n);
        for(UInt32 i = 0; i < s; i++){
            double val = at(i);

            double newVal = ((val - dcrtMin) / crtDiff) * diff + min;

            ans.at(i, (B)newVal);
        }

        return ans;
    }

    UInt32 rows() const {
        return m;
    }

    UInt32 cols() const {
        return n;
    }

    UInt32 depth() const {
        return p;
    }

    UInt32 size() const {
        return m * n * p;
    }

    ~Tensor3(){
        //do nothing
    }
};


template <typename T>
void checkSameDim(const Tensor3<T>& a, const Tensor3<T>& b){
    Int32 rows = a.rows();
    if(b.rows() != rows){
        stringstream ss;
        ss << "The number of rows does not match: ";
        ss << std::to_string(rows);
        ss << " != ";
        ss << std::to_string(b.rows());
        ss << ".";
        throw Exception(ss.str());
    }

    Int32 cols = a.cols();
    if(b.cols() != cols){
        stringstream ss;
        ss << "The number of columns does not match: ";
        ss << std::to_string(cols);
        ss << " != ";
        ss << std::to_string(b.cols());
        ss << ".";
        throw Exception(ss.str());
    }

    Int32 depth = a.depth();
    if(b.depth() != depth){
        stringstream ss;
        ss << "The depth does not match: ";
        ss << std::to_string(depth);
        ss << " != ";
        ss << std::to_string(b.depth());
        throw Exception(ss.str());
    }
}


template <typename T>
void checkSameDim(const Matrix<T>& a, const Matrix<T>& b){
    Int32 rows = a.rows();
    if(b.rows() != rows){
        stringstream ss;
        ss << "The number of rows does not match: ";
        ss << std::to_string(rows);
        ss << " != ";
        ss << std::to_string(b.rows());
        ss << ".";
        throw Exception(ss.str());
    }

    Int32 cols = a.cols();
    if(b.cols() != cols){
        stringstream ss;
        ss << "The number of columns does not match: ";
        ss << std::to_string(cols);
        ss << " != ";
        ss << std::to_string(b.cols());
        ss << ".";
        throw Exception(ss.str());
    }
}


template <typename T>
double squareErr(const Matrix<T>& a, const Matrix<T>& b){
    checkSameDim(a, b);
    
    UInt32 size = a.size();

    if(size == 0){
        return 0.0;
    }

    double sum  = 0.0;
    for(UInt32 i = 0; i < size; i++){

        double diff = ((double)a.at(i)) - ((double)b.at(i));
        double err = pow(diff, 2);
        sum += err;
    }

    double ans = sum / ((double)size);
    return ans;
}

template <typename T>
double squareErr(const Tensor3<T>& a, const Tensor3<T>& b){
    checkSameDim(a, b);
    
    UInt32 size = a.size();

    if(size == 0){
        return 0.0;
    }

    double sum  = 0.0;
    for(UInt32 i = 0; i < size; i++){

        double diff = ((double)a.at(i)) - ((double)b.at(i));
        double err = pow(diff, 2);
        sum += err;
    }

    double ans = sum / ((double)size);
    return ans;
}





}
}