//
//  BitVector.cpp
//  
//
//  Created by Mathieu Bouchard on 2014-03-03.
//
//

#include "BitVector.h"
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <stdlib.h>
#include <time.h>

/****************************************** BitMatrix ******************************************/

BitMatrix::BitMatrix()
{
    n = 0;
    m = 0;
    set_size(0,0);
}
BitMatrix::BitMatrix(size_t _m, size_t _n)
{
    n = 0;
    m = 0;
    set_size(_m,_n);
}

BitMatrix::~BitMatrix()
{
    reset();
}

void BitMatrix::set_size(size_t _m, size_t _n)
{
    reset();
    n = _n;
    m = _m;
    if(_m > 0 && n > 0)
    {
        rows = new BitVector[m];
        cols = new BitVector[n];
        a_c = new double*[n];
        for(size_t i = 0; i < n; i++)
        {
            a_c[i] = new double[m];
            memset(a_c[i], 0, m*sizeof(double));
        }
        a_r = new double*[m];
        for(size_t i = 0; i < m; i++)
        {
            a_r[i] = new double[n];
            memset(a_r[i], 0, n*sizeof(double));
        }
    }
    else
    {
        rows = NULL;
        cols = NULL;
        a_c = NULL;
        a_r = NULL;
    }
}

void BitMatrix::reset()
{
    if(m > 0 && n > 0)
    {
        delete[] rows;
        delete[] cols;
        
        for(size_t i = 0; i < n; i++)
            delete[] a_c[i];
        delete[] a_c;
        for(size_t i = 0; i < m; i++)
            delete[] a_r[i];
        delete[] a_r;
    }
}

void BitMatrix::seta(size_t index_i, size_t index_j, double val)
{
    if(index_i >= m || index_j >=n)
        throw string("BitMatrix : index out of bound\n");
    a_r[index_i][index_j] = val;
    a_c[index_j][index_i] = val;
}

double BitMatrix::geta(size_t index_i, size_t index_j) const
{
    if(index_i >= m || index_j >=n)
        throw string("BitMatrix : index out of bound\n");
    return a_r[index_i][index_j];
}

double BitMatrix::sum(const BitVector& col, const BitVector& row) const
{
    double ret = 0.0;
    BitIndex ind_j = BitIndex(col);
    BitIndex ind_i = BitIndex(row);
    for(size_t i = 0; i < ind_i.n; i++)
        for(size_t j = 0; j < ind_j.n; j++)
            ret += a_r[ind_i[i]][ind_j[j]];
    return ret;
}

double BitMatrix::sum_row(size_t index) const
{
    double ret = 0.0;
    for(size_t i = 0; i < n; i++)
        ret += a_r[index][i];
    return ret;
}

double BitMatrix::sum_col(size_t index) const
{
    double ret = 0.0;
    for(size_t i = 0; i < m; i++)
        ret += a_c[index][i];
    return ret;
}


void BitMatrix::setrow(size_t index, const string& bit_str)
{
    if(index >= m)
        throw string("BitMatrix : index out of bound\n");
    size_t str_size = bit_str.size() - count(bit_str.begin(), bit_str.end(), '-');
    if(str_size != n)
        throw string("BitMatrix : The bit vector string has not the right size (string size %ld != nb_columns %ld)\n", str_size, n);
    rows[index].set(bit_str);
}

void BitMatrix::setcol(size_t index, const string& bit_str)
{
    if(index >= n)
        throw string("BitMatrix : index out of bound\n");
    size_t str_size = bit_str.size() - count(bit_str.begin(), bit_str.end(), '-');
    if(str_size != m)
        throw string("BitMatrix : The bit vector string has not the right size (string size %ld != nb rows %ld)\n", str_size, m);
    cols[index].set(bit_str);
}
BitMatrix BitMatrix::operator*=(double m) const
{
    BitMatrix ret = BitMatrix(m,n);
    for(size_t i = 0; i < n; i++)
        for(size_t j = 0; j < m; j++)
            ret.a_c[i][j] = m*a_c[i][j];
    
    for(size_t i = 0; i < m; i++)
        for(size_t j = 0; j < n; j++)
            ret.a_r[i][j] = m*a_r[i][j];
    return ret;
}
BitMatrix& BitMatrix::operator*(double m)
{
    for(size_t i = 0; i < n; i++)
        for(size_t j = 0; j < m; j++)
            a_c[i][j] *= m;
    
    for(size_t i = 0; i < m; i++)
        for(size_t j = 0; j < n; j++)
            a_r[i][j] *= m;
    return *this;
}

BitMatrix BitMatrix::add_m(const BitMatrix& bit_matrix, double m1) const
{
    if(m != bit_matrix.m || n != bit_matrix.n)
        throw string("BitMatrix : matrix size are incompatible\n");
    BitMatrix ret = BitMatrix(m,n);
    for(size_t i = 0; i < n; i++)
    {
        ret.rows[i] = (rows[i] | bit_matrix.rows[i]);
        for(size_t j = 0; j < m; j++)
            ret.a_c[i][j] = a_c[i][j] + m1*bit_matrix.a_c[i][j];
    }
    for(size_t i = 0; i < m; i++)
    {
        ret.cols[i] = (cols[i] | bit_matrix.cols[i]);
        for(size_t j = 0; j < n; j++)
            ret.a_r[i][j] = a_r[i][j] + m1*bit_matrix.a_r[i][j];
    }
    return ret;
}

BitMatrix& BitMatrix::s_add_m(const BitMatrix& bit_matrix, double m1)
{
    if(m != bit_matrix.m || n != bit_matrix.n)
    {
        printf("[%ld %ld] [%ld %ld]\n", long(m),long(n),long(bit_matrix.m),long(bit_matrix.n));
        throw string("BitMatrix : matrix size are incompatible\n");
    }
    for(size_t i = 0; i < n; i++)
    {
        cols[i] |= bit_matrix.cols[i];
        for(size_t j = 0; j < m; j++)
            a_c[i][j] += m1*bit_matrix.a_c[i][j];
    }
    for(size_t i = 0; i < m; i++)
    {
        rows[i] |= bit_matrix.rows[i];
        for(size_t j = 0; j < n; j++)
            a_r[i][j] += m1*bit_matrix.a_r[i][j];
    }
    return *this;
}

BitMatrix& BitMatrix::operator+=(const BitMatrix& bit_matrix)
{
    return s_add_m(bit_matrix, 1.0);
}

BitMatrix BitMatrix::operator+(const BitMatrix& bit_matrix) const
{
    return add_m(bit_matrix, 1.0);
}

BitMatrix& BitMatrix::operator-=(const BitMatrix& bit_matrix)
{
    return s_add_m(bit_matrix, -1.0);
}

BitMatrix BitMatrix::operator-(const BitMatrix& bit_matrix) const
{
    return add_m(bit_matrix, -1.0);
}

BitMatrix& BitMatrix::operator=(const BitMatrix& bit_matrix)
{
    set_size(bit_matrix.m, bit_matrix.n);
    
    for(size_t i = 0; i < m; i++)
        rows[i] = bit_matrix.rows[i];
    for(size_t i = 0; i < n; i++)
        cols[i] = bit_matrix.cols[i];
    for(size_t i = 0; i < n; i++)
        memcpy(a_c[i], bit_matrix.a_c[i], m*sizeof(double));
    for(size_t i = 0; i < m; i++)
        memcpy(a_r[i], bit_matrix.a_r[i], n*sizeof(double));
    return *this;
}

BitMatrix BitMatrix::copy() const
{
    BitMatrix ret(m, n);
    for(size_t i = 0; i < m; i++)
        ret.rows[i] = rows[i];
    for(size_t i = 0; i < n; i++)
        ret.cols[i] = cols[i];
    for(size_t i = 0; i < n; i++)
        memcpy(ret.a_c[i], a_c[i], m*sizeof(double));
    for(size_t i = 0; i < m; i++)
        memcpy(ret.a_r[i], a_r[i], n*sizeof(double));
    return ret;
}



/****************************************** BitVector ******************************************/


BitVector::BitVector(const BitVector& bit_vector)
{
    bits = NULL;
    n = 0;
    m = 0;
    extra = 0;
    *this = bit_vector;
}

BitVector::BitVector(size_t _size)
{
    bits = NULL;
    init(_size);
}

BitVector::~BitVector()
{
    if(bits != NULL)
        delete[] bits;
}

void BitVector::init(size_t _size)
{
    if(bits != NULL)
        delete[] bits;
    n = _size;
    m = (ceil(n/double(sizeof(uint64_t))));
    bits = new uint64_t[m];
    memset(bits, 0, m*sizeof(uint64_t));
    set_extra();
}

void BitVector::set_extra()
{
    extra = 0;
    size_t start = n%sizeof(uint64_t);
    if(start == 0)
        extra = ~extra;
    else
    {
        for(size_t i = 0; i < start; i++)
            extra |= (1<<i);
    }
}

bool BitVector::is_zero() const
{
    bool ret = true;
    for(size_t i = 0; i < m && ret; i++)
        ret = ret && bits[i] == 0;
    return ret;
}

bool BitVector::is_one() const
{
    bool ret = true;
    for(size_t i = 0; i < m && ret; i++)
        ret = ret && (~bits[i]) == 0;
    return ret;
}

size_t BitVector::sum() const
{
    size_t ret = 0;
    for(size_t i = 0; i < n; i++)
        ret += (get(i) ? 1 : 0);
    return ret;
}

bool BitVector::get(size_t index) const
{
    if(index >= n)
        throw string("BitVector : index out of bound\n");
    return ((1<<(index%sizeof(uint64_t))) & bits[size_t(floor(index/sizeof(uint64_t)))]) > 0;
}

void BitVector::set(size_t index, bool value)
{
    if(index >= n)
        throw string("BitVector : index out of bound\n");
    if(value)
        bits[size_t(floor(index/sizeof(uint64_t)))] |= 1<<(index%sizeof(uint64_t));
    else
        bits[size_t(floor(index/sizeof(uint64_t)))] &= ~(1<<(index%sizeof(uint64_t)));
    
}

string BitVector::to_s() const
{
    size_t nn = n+ceil(n/8);
    string ret('0', nn);
    for(size_t i = 0; i < n; i++)
    {
        ret[nn-1-(i+i/8)] = (get(n-1-i) ? '1' : '0');
        if(i > 0 && i%8==0)
            ret[nn-1-(i+i/8-1)] = '-';
    }
    return ret;
}

void BitVector::random_fill()
{
    srand (0);//(time(NULL));
    for(size_t i = 0; i < n; i++)
    {
        if(rand()%2==1)
            set(i,true);
        else
            set(i,false);
    }
}

void BitVector::set(string val)
{
    size_t new_size= val.size() - count(val.begin(), val.end(), '-');
    if(n != new_size)
        init(new_size);
    
    int j = 0;
    for(unsigned int i = 0; i < n; ++i)
    {
        if(val[i] == '1')
            bits[size_t(floor((i-j)/sizeof(uint64_t)))] |= 1<<((i-j)%sizeof(uint64_t));
        else if(val[i] == '0')
            bits[size_t(floor((i-j)/sizeof(uint64_t)))] &= ~(1<<((i-j)%sizeof(uint64_t)));
        else if(val[i] == '-')
            ++j;
        else
            throw string("BitVector : Invalid string\n");
    }
}

size_t BitVector::size() const
{
    return n;
}

bool BitVector::operator[](size_t index) const
{
    if(index >= n)
        throw string("BitVector : index out of bound\n");
    return ((1<<(index%sizeof(uint64_t))) & bits[size_t(floor(index/sizeof(uint64_t)))]) > 0;
}

BitVector BitVector::operator&(const BitVector& bit_vector) const
{
    if(n != bit_vector.n)
        throw string("BitVector : Vector size mismatch");
    BitVector ret(n);
    for(size_t i = 0; i < m; i++)
        ret.bits[i] = bits[i] & bit_vector.bits[i];
    return ret;
}

BitVector BitVector::operator|(const BitVector& bit_vector) const
{
    if(n != bit_vector.n)
        throw string("BitVector : The vectors size are not the same");
    BitVector ret(n);
    for(size_t i = 0; i < m; i++)
        ret.bits[i] = bits[i] | bit_vector.bits[i];
    return ret;
}

BitVector BitVector::operator^(const BitVector& bit_vector) const
{
    if(n != bit_vector.n)
        throw string("BitVector : The vectors size are not the same");
    BitVector ret(n);
    for(size_t i = 0; i < m; i++)
        ret.bits[i] = bits[i] ^ bit_vector.bits[i];
    return ret;
}

BitVector BitVector::operator~() const
{
    BitVector ret(n);
    ret.extra = extra;
    for(size_t i = 0; i < m; i++)
        ret.bits[i] = ~bits[i];
    ret.bits[m-1] &= extra;
    ret.extra = extra;
    return ret;
}

BitVector&  BitVector::operator=(const BitVector& bit_vector)
{
    init(bit_vector.n);
    memcpy(bits, bit_vector.bits, m*sizeof(uint64_t));
    extra = bit_vector.extra;
    return *this;
}

BitVector& BitVector::operator&=(const BitVector& bit_vector)
{
    if(n != bit_vector.n)
        throw string("BitVector : Vector size mismatch");
    for(size_t i = 0; i < m; i++)
        bits[i] &= bit_vector.bits[i];
    return *this;
}

BitVector& BitVector::operator|=(const BitVector& bit_vector)
{
    if(n != bit_vector.n)
        throw string("BitVector : The vectors size are not the same");
    for(size_t i = 0; i < m; i++)
        bits[i] |= bit_vector.bits[i];
    return *this;
}

BitVector& BitVector::operator^=(const BitVector& bit_vector)
{
    if(n != bit_vector.n)
        throw string("BitVector : The vectors size are not the same");
    for(size_t i = 0; i < m; i++)
        bits[i] ^= bit_vector.bits[i];
    return *this;
}

BitVector BitVector::_and(const BitVector& bit_vector) const
{
    return (*this & bit_vector);
}

BitVector BitVector::_or(const BitVector& bit_vector) const
{
    return (*this | bit_vector);
}

BitVector BitVector::_xor(const BitVector& bit_vector) const
{
    return (*this ^ bit_vector);
}

BitVector BitVector::_not() const
{
    return (~(*this));
}


BitVector& BitVector::s_and(const BitVector& bit_vector)
{
    return (*this &= bit_vector);
}

BitVector& BitVector::s_or(const BitVector& bit_vector)
{
    return (*this |= bit_vector);
}

BitVector& BitVector::s_xor(const BitVector& bit_vector)
{
    return (*this ^= bit_vector);
}

BitVector BitVector::kronecker(const BitVector& bit_vector) const
{
    BitVector ret(n*bit_vector.n);
    for(size_t i = 0; i < n; i++)
    {
        if(get(i))
        {
            for(size_t j = 0; j < bit_vector.n; j++)
                ret.set(i*bit_vector.n+j, bit_vector.get(j));
        }
    }
    return ret;
}

BitVector& BitVector::s_not()
{
    for(size_t i = 0; i < m; i++)
        bits[i] = ~bits[i];
    bits[m-1] &= extra;
    return *this;
}

void* BitVector::Ptr()
{
    printf("From Ptr %p (bits = %p)\n", this, bits);
    return this;
}


/*
BitVector BitVector::operator<<(size_t s)
{
    if(s==0)
        return *this;
    
    size_t step = ceil(s/sizeof(uint64_t));
    size_t mod = s%sizeof(uint64_t);
    size_t mod_comp = sizeof(uint64_t)-s%sizeof(uint64_t);
    for(size_t i = ceil(n/sizeof(uint64_t))-1; i >= 0; i--)
    {
        uint64_t temp = bits[i-step];
        bits[i] = (bits[i] << mod) & temp;
    }
    
    return ret;
}

BitVector BitVector::operator>>(size_t s)
{
    
}*/


/****************************************** BitIndex ******************************************/
           
BitIndex::BitIndex(const BitVector& bit_vector)
{
    n = bit_vector.sum();
    if(n != 0)
        index = new size_t[n];
    size_t j = 0;
    for(size_t i = 0; i < bit_vector.size(); i++)
    {
        if(bit_vector.get(i))
        {
            index[j] = i;
            j++;
        }
    }
}
           
BitIndex::~BitIndex()
{
    if(n != 0)
        delete[] index;
}

bool BitIndex::operator[](size_t _index) const
{
    if(_index >= n)
        throw string("BitIndex : index out of bound\n");
    return index[_index];
}

/****************************************** BitHash ******************************************/

size_t BitHash::id_for_oid(size_t _oid)
{
    pair<map<size_t,size_t>::iterator, bool> it;
    it = oid2id.insert(pair<size_t,size_t>(_oid, id2oid.size()));
    if(it.second)
        id2oid.push_back(_oid);
    return it.first->second;
}
size_t BitHash::oid_for_id(size_t _id)
{
    if(_id >= id2oid.size())
        throw string("BitSet : index out of bound\n");
    return id2oid[_id];
}

/****************************************** BitSet ******************************************/

void BitSet::set(char* feature, char* item, bool value)
{
    string _feature = string(feature);
    string _item = string(item);
    if(id_for_feature.count(_feature) == 0)
        throw string("No feature exist with name ")+_feature;
    size_t feature_id = id_for_feature[_feature];
    
    if(id_for_feature_item[feature_id].count(_item) == 0)
    {
        size_t curr_size=id_for_feature_item[feature_id].size();
        if(curr_size >= features[feature_id].size() )
            throw string("No item exist with name ")+_item+string(" and feature ")+_feature+string(" item list is full");
        id_for_feature_item[feature_id][_item] = curr_size;
        //throw string("No item exist with name ")+_item+string(" for feature ")+_feature;
    }
    size_t item_id = id_for_feature_item[feature_id][_item];
    
    set(feature_id, item_id, value);
}

void BitSet::set(size_t feature_id, size_t item_id, bool value)
{
    if(feature_id >= features.size() || feature_id >= id_for_feature_item.size())
        throw string("BitSet : The internal feature index is too big (this is a bug there is not much you can do)");
    if(item_id >= features[feature_id].size())
        throw string("BitSet : The internal item index is too big (this is a bug there is not much you can do)");
    features[feature_id].set(item_id, value);
}

bool BitSet::get(char* feature, char* item)
{
    string _feature = string(feature);
    string _item = string(item);
    if(id_for_feature.count(_feature) == 0)
        throw string("No feature exist with name ")+_feature;
    size_t feature_id = id_for_feature[_feature];
    
    if(id_for_feature_item[feature_id].count(_item) == 0)
    {
        size_t curr_size=id_for_feature_item[feature_id].size();
        if(curr_size >= features[feature_id].size() )
            throw string("No item exist with name ")+_item+string(" and feature ")+_feature+string(" item list is full");
        id_for_feature_item[feature_id][_item] = curr_size;
        //throw string("No item exist with name ")+_item+string(" for feature ")+_feature;
    }
    size_t item_id = id_for_feature_item[feature_id][_item];
    
    return get(feature_id, item_id);
}

bool BitSet::get(size_t feature_id, size_t item_id)
{
    if(feature_id >= features.size() || feature_id >= id_for_feature_item.size())
        throw string("BitSet : The internal feature index is too big (this is a bug there is not much you can do)");
    if(item_id >= features[feature_id].size())
        throw string("BitSet : The internal item index is too big (this is a bug there is not much you can do)");
    return features[feature_id][item_id];
}

void BitSet::add_feature(char* _name, size_t _size)
{
    string __name = string(_name);
    if(id_for_feature.count(__name) != 0)
        throw string("BitSet : A feature already exist for name ")+__name;
    id_for_feature[__name] = features.size();
    
    //features.push_back(BitVector(_size));
    features.push_back(BitVector());
    features.back().init(_size);
    id_for_feature_item.resize(_size);
    feature_names.push_back(__name);
}


/****************************************** BitModel ******************************************/


void BitModel::add_set(char* _name)
{
    string __name = string(_name);
    if(set_for_set_name.count(__name) != 0)
        throw string("BitModel : A set already exist for name ")+__name;
    set_for_set_name[__name] = sets.size();
    sets.push_back(BitSet(_name));
}

BitSet& BitModel::get_set(char* _name)
{
    string __name = string(_name);
    if(set_for_set_name.count(__name) == 0)
        throw string("No set exist with name ")+__name;
    size_t set_id = set_for_set_name[__name];
    return get_set(set_id);
}

BitSet& BitModel::get_set(size_t set_id)
{
    if(set_id >= sets.size())
        throw string("BitModel : The internal set index is too big (this is a bug there is not much you can do)");
    return sets[set_id];
}