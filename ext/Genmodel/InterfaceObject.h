//
//  InterfaceObject.h
//  
//
//  Created by Mathieu Bouchard on 2014-03-19.
//
//

#ifndef _InterfaceObject_h
#define _InterfaceObject_h

template<class T> class InterfaceVector {
public:
    InterfaceVector();
    InterfaceVector(int size);
    ~InterfaceVector();
    void SetSize(int size);
    void Delete();
    void Set(int index, T val);
    T Get(int index);
    T* Ptr();
    
private:
    int size;
    T * val;
};

#endif
