/***************************************************************************
 *  GenModelGem.cpp
 *  GenModel gem library
 *
 *  February 28 14:20 2014
 *  Copyright  2014  Mathieu Bouchard
 *  mathbouchard@gmail.com
 ****************************************************************************/

#include "GenModelGem.h"
#include <dlfcn.h>
#include <string>

char* error = NULL;
char tmp[4096];
char func_name[4096];
void* genmodel_lib = NULL;

#define START_FUNC try{ if(genmodel_lib == NULL) {initall();}
#define END_FUNC } catch (string e) { throw e; } return 1;

void call_error(const char* func)
{
    snprintf(tmp, 4096, "Cannot load function %s : %s\n", func, error);
    fprintf(stderr, "%s\n", tmp);
    throw string(tmp);
}

//using namespace std;

double (*_FindConstraintMaxLhs)(long row, long token) = NULL;
double (*_FindConstraintMinLhs)(long row, long token) = NULL;
long (*_MakeConstraintFeasible)(long row, long token) = NULL;
long (*_WriteProblemToLpFile)(char* filename, long token) = NULL;
long (*_WriteSolutionToFile)(char* filename, long token) = NULL;
long (*_AddConst)(char* cname, double rhs, char sense, long token) = NULL;
bool (*_AddConstBulk)(char* cname, double* rhs, long length, char sense, long token) = NULL;
long (*_AddVar)(char* nn, double o, double l, double u, char t, long token) = NULL;
bool (*_AddVarBulk)(char* nn, double* o, long length, double l, double u, char t, long token) = NULL;
long (*_AddNz)(long row, long col, double val, long token) = NULL;
long (*_AddNzToLast)(long col, double val, long token) = NULL;
long (*_AddNzBulk)(long* rows, long* cols, double* values, long valuesLength, long rowCount, long colCount, long iterations, long token) = NULL;
long (*_SetQpCoef)(long i, long j, double val, long token) = NULL;
long (*_SetNumbers)(long token) = NULL;
long (*_SetLongParam)(char* param, long val, long token) = NULL;
long (*_SetDblParam)(char* param, double val, long token) = NULL;
long (*_SetBoolParam)(char* param, bool val, long token) = NULL;
long (*_SetStrParam)(char* param, char* val, long token) = NULL;
long (*_CreateNewModel)(char type, char* name) = NULL;
bool (*_IsSolverAvailable)(char type) = NULL;
long (*_CopyOrder)(long token, int count, int* ind, int* weight) = NULL;
long (*_DeleteModel)(long token) = NULL;
long (*_CreateModel)(long token) = NULL;
long (*_SolveModel)(long token) = NULL;
bool (*_GetSolVars)(double* values, long length, long token) = NULL;
bool (*_HasSolution)(long token) = NULL;
bool (*_GetDualPrices)(double* values, long length, long token) = NULL;
bool (*_GetSlacks)(double* values, long length, long token) = NULL;
bool (*_GetReducedCosts)(double* values, long length, long token) = NULL;
bool (*_GetRowValues)(double* values, long length, long rowIndex, long token) = NULL;
bool (*_GetObjCoef)(double* values, long length, long token) = NULL;
bool (*_GetBounds)(double* lb, double* ub, long length, long token) = NULL;
double (*_GetLowerBound)(long col, long token) = NULL;
double (*_GetUpperBound)(long col, long token) = NULL;
bool (*_SetLowerBound)(long col, double val, long token) = NULL;
bool (*_SetUpperBound)(long col, double val, long token) = NULL;
double (*_GetRHS)(long row, long token) = NULL;
bool (*_SetRHS)(long row, double val, long token) = NULL;
char (*_GetSense)(long row, long token) = NULL;
bool (*_SetSense)(long row, char sense, long token) = NULL;
double (*_GetObjVal)(long token) = NULL;
long (*_ChangeBulkBounds)(int count, int* indices, char* types, double* values, long token) = NULL;
long (*_ChangeBulkObjectives)(int count, int* indices, double* values, long token) = NULL;
long (*_DeleteMipStarts)(long token) = NULL;
double (*_GetMIPRelativeGap)(long token) = NULL;

void initall()
{
    //printf("************************* init all\n");
    try {
        if(genmodel_lib == NULL)
        {
            dlerror();
#ifdef Darwin
            string suffix = string(".dylib");
#else
            string suffix = string(".so");
            genmodel_lib = dlopen("libgenmodel.so", RTLD_LAZY);
#endif
            genmodel_lib = dlopen((string("libgenmodel")+suffix).c_str(), RTLD_LAZY);
            if(genmodel_lib == NULL)
                throw string("Genmodel C++ library is missing or is not in the library path\n");
        }
        
        *(void **)(&_FindConstraintMaxLhs) = dlsym(genmodel_lib, "FindConstraintMaxLhs");
        if ((error = dlerror()) != NULL) {call_error("FindConstraintMaxLhs");}
        *(void **)(&_FindConstraintMinLhs) = dlsym(genmodel_lib, "FindConstraintMinLhs");
        if ((error = dlerror()) != NULL) {call_error("FindConstraintMinLhs");}
        *(void **)(&_MakeConstraintFeasible) = dlsym(genmodel_lib, "MakeConstraintFeasible");
        if ((error = dlerror()) != NULL) {call_error("MakeConstraintFeasible");}
        *(void **)(&_WriteProblemToLpFile) = dlsym(genmodel_lib, "WriteProblemToLpFile");
        if ((error = dlerror()) != NULL) {call_error("WriteProblemToLpFile");}
        *(void **)(&_WriteSolutionToFile) = dlsym(genmodel_lib, "WriteSolutionToFile");
        if ((error = dlerror()) != NULL) {call_error("WriteSolutionToFile");}
        *(void **)(&_AddConst) = dlsym(genmodel_lib, "AddConst");
        if ((error = dlerror()) != NULL) {call_error("AddConst");}
        *(void **)(&_AddConstBulk) = dlsym(genmodel_lib, "AddConstBulk");
        if ((error = dlerror()) != NULL) {call_error("AddConstBulk");}
        *(void **)(&_AddVar) = dlsym(genmodel_lib, "AddVar");
        if ((error = dlerror()) != NULL) {call_error("AddVar");}
        *(void **)(&_AddVarBulk) = dlsym(genmodel_lib, "AddVarBulk");
        if ((error = dlerror()) != NULL) {call_error("AddVarBulk");}
        *(void **)(&_AddNz) = dlsym(genmodel_lib, "AddNz");
        if ((error = dlerror()) != NULL) {call_error("AddNz");}
        *(void **)(&_AddNzToLast) = dlsym(genmodel_lib, "AddNzToLast");
        if ((error = dlerror()) != NULL) {call_error("AddNzToLast");}
        *(void **)(&_AddNzBulk) = dlsym(genmodel_lib, "AddNzBulk");
        if ((error = dlerror()) != NULL) {call_error("AddNzBulk");}
        *(void **)(&_SetQpCoef) = dlsym(genmodel_lib, "SetQpCoef");
        if ((error = dlerror()) != NULL) {call_error("SetQpCoef");}
        *(void **)(&_SetNumbers) = dlsym(genmodel_lib, "SetNumbers");
        if ((error = dlerror()) != NULL) {call_error("SetNumbers");}
        *(void **)(&_SetLongParam) = dlsym(genmodel_lib, "SetLongParam");
        if ((error = dlerror()) != NULL) {call_error("SetLongParam");}
        *(void **)(&_SetDblParam) = dlsym(genmodel_lib, "SetDblParam");
        if ((error = dlerror()) != NULL) {call_error("SetDblParam");}
        *(void **)(&_SetBoolParam) = dlsym(genmodel_lib, "SetBoolParam");
        if ((error = dlerror()) != NULL) {call_error("SetBoolParam");}
        *(void **)(&_SetStrParam) = dlsym(genmodel_lib, "SetStrParam");
        if ((error = dlerror()) != NULL) {call_error("SetStrParam");}
        *(void **)(&_CreateNewModel) = dlsym(genmodel_lib, "CreateNewModel");
        if ((error = dlerror()) != NULL) {call_error("CreateNewModel");}
        *(void **)(&_IsSolverAvailable) = dlsym(genmodel_lib, "IsSolverAvailable");
        if ((error = dlerror()) != NULL) {call_error("IsSolverAvailable");}
        
        *(void **)(&_CopyOrder) = dlsym(genmodel_lib, "CopyOrder");
        if ((error = dlerror()) != NULL) {call_error("CopyOrder");}
        *(void **)(&_DeleteModel) = dlsym(genmodel_lib, "DeleteModel");
        if ((error = dlerror()) != NULL) {call_error("DeleteModel");}
        *(void **)(&_CreateModel) = dlsym(genmodel_lib, "CreateModel");
        if ((error = dlerror()) != NULL) {call_error("CreateModel");}
        *(void **)(&_SolveModel) = dlsym(genmodel_lib, "SolveModel");
        if ((error = dlerror()) != NULL) {call_error("SolveModel");}
        *(void **)(&_GetSolVars) = dlsym(genmodel_lib, "GetSolVars");
        if ((error = dlerror()) != NULL) {call_error("GetSolVars");}
        *(void **)(&_HasSolution) = dlsym(genmodel_lib, "HasSolution");
        if ((error = dlerror()) != NULL) {call_error("HasSolution");}
        *(void **)(&_GetDualPrices) = dlsym(genmodel_lib, "GetDualPrices");
        if ((error = dlerror()) != NULL) {call_error("GetDualPrices");}
        *(void **)(&_GetSlacks) = dlsym(genmodel_lib, "GetSlacks");
        if ((error = dlerror()) != NULL) {call_error("GetSlacks");}
        *(void **)(&_GetReducedCosts) = dlsym(genmodel_lib, "GetReducedCosts");
        if ((error = dlerror()) != NULL) {call_error("GetReducedCosts");}
        *(void **)(&_GetRowValues) = dlsym(genmodel_lib, "GetRowValues");
        if ((error = dlerror()) != NULL) {call_error("GetRowValues");}
        *(void **)(&_GetObjCoef) = dlsym(genmodel_lib, "GetObjCoef");
        if ((error = dlerror()) != NULL) {call_error("GetObjCoef");}
        *(void **)(&_GetBounds) = dlsym(genmodel_lib, "GetBounds");
        if ((error = dlerror()) != NULL) {call_error("GetBounds");}
        *(void **)(&_GetLowerBound) = dlsym(genmodel_lib, "GetLowerBound");
        if ((error = dlerror()) != NULL) {call_error("GetLowerBound");}
        *(void **)(&_GetUpperBound) = dlsym(genmodel_lib, "GetUpperBound");
        if ((error = dlerror()) != NULL) {call_error("GetUpperBound");}
        *(void **)(&_SetLowerBound) = dlsym(genmodel_lib, "SetLowerBound");
        if ((error = dlerror()) != NULL) {call_error("SetLowerBound");}
        *(void **)(&_SetUpperBound) = dlsym(genmodel_lib, "SetUpperBound");
        if ((error = dlerror()) != NULL) {call_error("SetUpperBound");}
        *(void **)(&_GetRHS) = dlsym(genmodel_lib, "GetRHS");
        if ((error = dlerror()) != NULL) {call_error("GetRHS");}
        *(void **)(&_SetRHS) = dlsym(genmodel_lib, "SetRHS");
        if ((error = dlerror()) != NULL) {call_error("SetRHS");}
        *(void **)(&_GetSense) = dlsym(genmodel_lib, "GetSense");
        if ((error = dlerror()) != NULL) {call_error("GetSense");}
        *(void **)(&_SetSense) = dlsym(genmodel_lib, "SetSense");
        if ((error = dlerror()) != NULL) {call_error("SetSense");}
        *(void **)(&_GetObjVal) = dlsym(genmodel_lib, "GetObjVal");
        if ((error = dlerror()) != NULL) {call_error("GetObjVal");}
        *(void **)(&_ChangeBulkBounds) = dlsym(genmodel_lib, "ChangeBulkBounds");
        if ((error = dlerror()) != NULL) {call_error("ChangeBulkBounds");}
        *(void **)(&_ChangeBulkObjectives) = dlsym(genmodel_lib, "ChangeBulkObjectives");
        if ((error = dlerror()) != NULL) {call_error("ChangeBulkObjectives");}
        *(void **)(&_DeleteMipStarts) = dlsym(genmodel_lib, "DeleteMipStarts");
        if ((error = dlerror()) != NULL) {call_error("DeleteMipStarts");}
        *(void **)(&_GetMIPRelativeGap) = dlsym(genmodel_lib, "GetMIPRelativeGap");
        if ((error = dlerror()) != NULL) {call_error("GetMIPRelativeGap");}
    } catch (string e) {
        throw e;
    } catch (...) {
        throw string("Unknown C++ exception");
    }
}

long WriteProblemToLpFile(char* filename, long token)
{
    START_FUNC
    return (_WriteProblemToLpFile)(filename, token);
    END_FUNC
}
 
 
long WriteSolutionToFile(char* filename, long token)
{
    START_FUNC
    return (_WriteSolutionToFile)(filename, token);
    END_FUNC
}
 
double FindConstraintMaxLhs(long row, long token)
{
    START_FUNC
    return (_FindConstraintMaxLhs)(row, token);
    END_FUNC
}

double FindConstraintMinLhs(long row, long token)
{
    START_FUNC
    return (_FindConstraintMinLhs)(row, token);
    END_FUNC
}

long MakeConstraintFeasible(long row, long token)
{
    START_FUNC
    return (_MakeConstraintFeasible)(row, token);
    END_FUNC
}

long AddConst(char* cname, double rhs, char sense, long token)
{
    START_FUNC
    return (_AddConst)(cname, rhs, sense, token);
    END_FUNC
}
 
bool AddConstBulk(char* cname, double* rhs, long length, char sense, long token)
{
    START_FUNC
    return (_AddConstBulk)(cname, rhs, length, sense, token);
    END_FUNC
}

long AddVar(char* nn, double o, double l, double u, char t, long token)
{
    START_FUNC
    return (_AddVar)(nn, o, l, u, t, token);
    END_FUNC
}

bool AddVarBulk(char* nn, double* o, long length, double l, double u, char t, long token)
{
    START_FUNC
    return (_AddVarBulk)(nn, o, length, l, u, t, token);
    END_FUNC
}

long SetQpCoef(long i, long j, double val, long token)
{
    START_FUNC
    return (_SetQpCoef)(i, j, val, token);
    END_FUNC
}

long AddNz(long row, long col, double val, long token)
{
    START_FUNC
    return (_AddNz)(row, col, val, token);
    END_FUNC
}

long AddNzToLast(long col, double val, long token)
{
    START_FUNC
    return (_AddNzToLast)(col, val, token);
    END_FUNC
}

long AddNzBulk(long* rows, long* cols, double* values, long valuesLength, long rowCount, long colCount, long iterations, long token)
{
    START_FUNC
    return (_AddNzBulk)(rows, cols, values, valuesLength, rowCount, colCount, iterations, token);
    END_FUNC
}

long SetNumbers(long token)
{
    START_FUNC
    return (_SetNumbers)(token);
    END_FUNC
}

long SetLongParam(char* param, long val, long token)
{
    START_FUNC
    return (_SetLongParam)(param, val, token);
    END_FUNC
}

long SetDblParam(char* param, double val, long token)
{
    START_FUNC
    return (_SetDblParam)(param, val, token);
    END_FUNC
}

long SetBoolParam(char* param, bool val, long token)
{
    START_FUNC
    return (_SetBoolParam)(param, val, token);
    END_FUNC
}

long SetStrParam(char* param, char* val, long token)
{
    START_FUNC
    return (_SetStrParam)(param, val, token);
    END_FUNC
}
 
long CreateNewModel(char type, char* name = NULL)
{
    START_FUNC
    return (_CreateNewModel)(type, name);
    END_FUNC
}

bool IsSolverAvailable(char type)
{
    START_FUNC
    return (_IsSolverAvailable)(type);
    END_FUNC
}


long CopyOrder(long token, int count, int* ind, int* weight)
{
    START_FUNC
    return (_CopyOrder)(token, count, ind, weight);
    END_FUNC
}

long DeleteModel(long token)
{
    START_FUNC
    return (_DeleteModel)(token);
    END_FUNC
}

long CreateModel(long token)
{
    START_FUNC
    return (_CreateModel)(token);
    END_FUNC
}

long SolveModel(long token)
{
    START_FUNC
    return (_SolveModel)(token);
    END_FUNC
}

bool HasSolution(long token)
{
    START_FUNC
    return (_HasSolution)(token);
    END_FUNC
}

bool GetSolVars(double* values, long length, long token)
{
    START_FUNC
    return (_GetSolVars)(values, length, token);
    END_FUNC
}

bool GetDualPrices(double* values, long length, long token)
{
    START_FUNC
    return (_GetDualPrices)(values, length, token);
    END_FUNC
}

bool GetSlacks(double* values, long length, long token)
{
    START_FUNC
    return (_GetSlacks)(values, length, token);
    END_FUNC
}

bool GetReducedCosts(double* values, long length, long token)
{
    START_FUNC
    return (_GetReducedCosts)(values, length, token);
    END_FUNC
}

bool GetRowValues(double* values, long length, long rowIndex, long token)
{
    START_FUNC
    return (_GetRowValues)(values, length, rowIndex, token);
    END_FUNC
}

bool GetObjCoef(double* values, long length, long token)
{
    START_FUNC
    return (_GetObjCoef)(values, length, token);
    END_FUNC
}

bool GetBounds(double* lb, double* ub, long length, long token)
{
    START_FUNC
    return (_GetBounds)(lb, ub, length, token);
    END_FUNC
}

double GetLowerBound(long col, long token)
{
    START_FUNC
    return (_GetLowerBound)(col, token);
    END_FUNC
}

double GetUpperBound(long col, long token)
{
    START_FUNC
    return (_GetUpperBound)(col, token);
    END_FUNC
}

bool SetLowerBound(long col, double val, long token)
{
    START_FUNC
    return (_SetLowerBound)(col, val, token);
    END_FUNC
}

bool SetUpperBound(long col, double val, long token)
{
    START_FUNC
    return (_SetUpperBound)(col, val, token);
    END_FUNC
}

double GetRHS(long row, long token)
{
    START_FUNC
    return (_GetRHS)(row, token);
    END_FUNC
}

bool SetRHS(long row, double val, long token)
{
    START_FUNC
    return (_SetRHS)(row, val, token);
    END_FUNC
}

char GetSense(long row, long token)
{
    START_FUNC
    return (_GetSense)(row, token);
    END_FUNC
}

bool SetSense(long row, char sense, long token)
{
    START_FUNC
    return (_SetSense)(row, sense, token);
    END_FUNC
}

double GetObjVal(long token)
{
    START_FUNC
    return (_GetObjVal)(token);
    END_FUNC
}

long ChangeBulkBounds(int count, int* indices, char* types, double* values, long token)
{
    START_FUNC
    return (_ChangeBulkBounds)(count, indices, types, values, token);
    END_FUNC
}

long ChangeBulkObjectives(int count, int* indices, double* values, long token)
{
    START_FUNC
    return (_ChangeBulkObjectives)(count, indices, values, token);
    END_FUNC
}

long DeleteMipStarts(long token)
{
    START_FUNC
    return (_DeleteMipStarts)(token);
    END_FUNC
}

double GetMIPRelativeGap(long token)
{
    START_FUNC
    return (_GetMIPRelativeGap)(token);
    END_FUNC
}

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