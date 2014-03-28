//
//  BitVector.h
//  
//
//  Created by Mathieu Bouchard on 2014-03-03.
//
//

#ifndef _BitVector_h
#define _BitVector_h

#include <stdint.h>
#include <math.h>

#include <string>
#include <vector>
#include <map>


using namespace std;

class BitVector
{
public:
    BitVector(const BitVector& bit_vector);
    BitVector() : bits(NULL), n(0), m(0), extra(0) { }
    BitVector(size_t _size);
    ~BitVector();
    
    bool operator[](size_t index) const;
    BitVector operator&(const BitVector& bit_vector) const;
    BitVector operator|(const BitVector& bit_vector) const;
    BitVector operator^(const BitVector& bit_vector) const;
    BitVector operator~() const;
    BitVector& operator&=(const BitVector& bit_vector);
    BitVector& operator|=(const BitVector& bit_vector);
    BitVector& operator^=(const BitVector& bit_vector);
    BitVector _and(const BitVector& bit_vector) const;
    BitVector _or(const BitVector& bit_vector) const;
    BitVector _xor(const BitVector& bit_vector) const;
    BitVector _not() const;
    BitVector& s_and(const BitVector& bit_vector) ;
    BitVector& s_or(const BitVector& bit_vector);
    BitVector& s_xor(const BitVector& bit_vector);
    BitVector& s_not();
    BitVector& operator=(const BitVector& bit_vector);
    BitVector kronecker(const BitVector& bit_vector) const;
    
    bool is_zero() const;
    bool is_one() const;
    size_t sum() const;
    //BitVector operator<<();
    //BitVector operator>>();
    
    string to_s() const;
    bool get(size_t index) const;
    void set(size_t index, bool value);
    void set(string val);
    void random_fill();
    void init(size_t _size);
    size_t size() const;
    void* Ptr();
    
private:
    inline void set_extra();
    uint64_t* bits;
    size_t n;
    size_t m;
    uint64_t extra;
};

class BitIndex
{
public:
    BitIndex(const BitVector& bit_vector);
    ~BitIndex();
    bool operator[](size_t _index) const;
//private:
    size_t* index;
    size_t n;
};

class BitMatrix
{
public:
    BitMatrix();
    BitMatrix(size_t _m, size_t _n);
    ~BitMatrix();
    
    void set_size(size_t _m, size_t _n);
    void reset();
    void seta(size_t index_i, size_t index_j, double val);
    double geta(size_t index_i, size_t index_j) const;
    void setrow(size_t index, const string& bit_str);
    void setcol(size_t index, const string& bit_str);
    double sum(const BitVector& col, const BitVector& row) const;
    double sum_row(size_t i) const;
    double sum_col(size_t i) const;
    BitMatrix& operator+=(const BitMatrix& bit_matrix);
    BitMatrix operator+(const BitMatrix& bit_matrix) const;
    BitMatrix& operator-=(const BitMatrix& bit_matrix);
    BitMatrix operator-(const BitMatrix& bit_matrix) const;
    BitMatrix& operator=(const BitMatrix& bit_matrix);
    BitMatrix copy() const;
    BitMatrix operator*=(double m) const;
    BitMatrix& operator*(double m);
    BitMatrix add_m(const BitMatrix& bit_matrix, double m1) const; // C = A+m2*B
    BitMatrix& s_add_m(const BitMatrix& bit_matrix, double m1); // A = A+m2*B
    
    void ComputeMaximalCliqueCoverage();
    
    size_t n; //nb_col
    size_t m; //nb_row
    BitVector* rows;
    BitVector* cols;
    
private:
    double** a_c;
    double** a_r;
};

class BitHash
{
public:
    BitHash() {name = "BitHash";}
    BitHash(char* _name) {name = string(_name);}
    ~BitHash() { }
    
    size_t id_for_oid(size_t _oid);
    size_t oid_for_id(size_t _id);
    
    string name;
    map<size_t,size_t> oid2id;
    vector<size_t> id2oid;
};

class BitSet
{
public:
    BitSet(	) {name = "BitSet";}
    BitSet(char* _name) {name = string(_name);}
    ~BitSet() { }
    
    void set(char* feature, char* item, bool value);
    bool get(char* feature, char* item);
    void set(size_t feature_id, size_t item_id, bool value);
    bool get(size_t feature_id, size_t item_id);
    void add_feature(char* _name, size_t _size);
    
    string name;
    map<string, size_t> id_for_feature;
    vector<map<string,size_t> > id_for_feature_item;
    vector<BitVector> features;
    vector<string> feature_names;
};

class BitModel
{
public:
    BitModel() {name = "BitModel";}
    BitModel(char* _name) {name = string(_name);}
    
    void add_set(char* _name);
    BitSet& get_set(char* _name);
    BitSet& get_set(size_t set_id);
    
    string name;
    vector<BitSet> sets;
    map<string, size_t> set_for_set_name;
};

#endif
