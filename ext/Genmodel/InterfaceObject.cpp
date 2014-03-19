//
//  InterfaceObject.cpp
//  
//
//  Created by Mathieu Bouchard on 2014-03-19.
//
//

#include <stdio.h>
//#include <stdlib.h>
#include "InterfaceObject.h"

template<class T> InterfaceVector<T>::InterfaceVector()
{
    val = NULL;
    size = 0;
    //printf("\tC++: Adress at creation is %p size = %d\n", val, size);
}

template<class T> InterfaceVector<T>::InterfaceVector(int _size)
{
    size = _size;
    val = new T[size];
    //printf("\tC++: Adress at creation is %p size = %d\n", val, size);
    //memset(val, 0, sizeof(T));
}

template<class T> InterfaceVector<T>::~InterfaceVector()
{
    Delete();
}

template<class T> void InterfaceVector<T>::SetSize(int _size)
{
    Delete();
    size = _size;
    val = new T[size];
}
template<class T> void InterfaceVector<T>::Delete()
{
    size = 0;
    if(val != NULL)
        delete[] val;
}

template<class T> void InterfaceVector<T>::Set(int index, T inval)
{
    if(index < 0 || index >= size)
        printf("Index out of bound (%d not in 0..%d)\n", index, size);
    else
        val[index] = inval;
}

template<class T> T InterfaceVector<T>::Get(int index)
{
    if(index < 0 || index >= size)
        printf("Index out of bound (%d not in 0..%d)\n", index, size);
    else
        return val[index];
    return 0.0;
}

template<class T> T* InterfaceVector<T>::Ptr()
{
    return val;
}

template class InterfaceVector<int>;
template class InterfaceVector<long>;
template class InterfaceVector<double>;