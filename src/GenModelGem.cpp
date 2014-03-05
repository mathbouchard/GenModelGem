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

char* _error = NULL;
char tmp[4096];
char func_name[4096];
void* genmodel_lib = NULL;

#define START_FUNC try{ if(genmodel_lib == NULL) {initall();}
#define END_FUNC } catch (string e) { throw e; } return 1;

typedef double (*d_f_ll)(long, long);
typedef long (*l_ll)(long, long);
typedef long (*l_cpl)(char*, long);
typedef long (*l_cpdcl)(char*, double, char, long);
typedef bool (*b_cpdplcl)(char*, double*, long, char, long);
typedef long (*l_cpdddcl)(char*, double, double, double, char, long);
typedef bool (*b_cpdplddcl)(char*, double*, long, double, double, char, long);
typedef long (*l_lldl)(long, long, double, long);
typedef long (*l_ldl)(long, double, long);
typedef long (*l_lplpdplllll)(long*, long*, double*, long, long, long, long, long);
typedef long (*l_l)(long);
typedef long (*l_cpll)(char*, long, long);
typedef long (*l_cpdl)(char*, double, long);
typedef long (*l_cpbl)(char*, bool, long);
typedef long (*l_cpcpl)(char*, char*, long);
typedef long (*l_ccp)(char, char*);
typedef bool (*b_c)(char);
typedef long (*l_liipip)(long, int, int*, int*);
typedef bool (*b_dpll)(double*, long, long);
typedef bool (*b_l)(long);
typedef bool (*b_dplll)(double*, long, long, long);
typedef bool (*b_dpdpll)(double*, double*, long, long);
typedef double (*d_ll)(long, long);
typedef bool (*b_ldl)(long, double, long);
typedef char (*c_ll)(long, long);
typedef bool (*b_lcl)(long, char, long);
typedef double (*d_l)(long);
typedef long (*l_iipcpdpl)(int, int*, char*, double*, long);
typedef long (*l_iipdpl)(int, int*, double*, long);

void call_error(const char* func)
{
    snprintf(tmp, 4096, "Cannot load function %s : %s\n", func, _error);
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
    try {
        if(genmodel_lib == NULL)
        {
            dlerror();
#ifdef Darwin
            string suffix = string(".dylib");
#else
            string suffix = string(".so");
#endif
            printf("Loading library %s\n", (string("libgenmodel")+suffix).c_str());
            genmodel_lib = dlopen((string("libgenmodel")+suffix).c_str(), RTLD_LAZY);
            if(genmodel_lib == NULL)
            {
                printf("Falling back to default installation directory : %s\n", (string("/usr/local/lib/libgenmodel")+suffix).c_str());
                //genmodel_lib = dlopen((string("/Users/mbouchard/gitwork/GenModel/lib/libgenmodel")+suffix).c_str(), RTLD_LAZY);
                genmodel_lib = dlopen((string("/usr/local/lib/libgenmodel")+suffix).c_str(), RTLD_LAZY);
            }
            if(genmodel_lib == NULL)
                throw string("Genmodel C++ library is missing or is not in the library path\n");
            printf("Done loading library %s\n", (string("libgenmodel")+suffix).c_str());
        }
        printf("Loading library symbols %p\n", genmodel_lib);
        _FindConstraintMaxLhs = (d_f_ll)dlsym(genmodel_lib, "FindConstraintMaxLhs");
        if ((_error = dlerror()) != NULL) {call_error("FindConstraintMaxLhs");}
        _FindConstraintMinLhs = (d_f_ll)dlsym(genmodel_lib, "FindConstraintMinLhs");
        if ((_error = dlerror()) != NULL) {call_error("FindConstraintMinLhs");}
        _MakeConstraintFeasible = (l_ll)dlsym(genmodel_lib, "MakeConstraintFeasible");
        if ((_error = dlerror()) != NULL) {call_error("MakeConstraintFeasible");}
        _WriteProblemToLpFile = (l_cpl)dlsym(genmodel_lib, "WriteProblemToLpFile");
        if ((_error = dlerror()) != NULL) {call_error("_WriteProblemToLpFile");}
        _WriteSolutionToFile = (l_cpl)dlsym(genmodel_lib, "WriteSolutionToFile");
        if ((_error = dlerror()) != NULL) {call_error("WriteSolutionToFile");}
        _AddConst = (l_cpdcl)dlsym(genmodel_lib, "AddConst");
        if ((_error = dlerror()) != NULL) {call_error("AddConst");}
        _AddConstBulk = (b_cpdplcl)dlsym(genmodel_lib, "AddConstBulk");
        if ((_error = dlerror()) != NULL) {call_error("AddConstBulk");}
        _AddVar = (l_cpdddcl)dlsym(genmodel_lib, "AddVar");
        if ((_error = dlerror()) != NULL) {call_error("AddVar");}
        _AddVarBulk = (b_cpdplddcl)dlsym(genmodel_lib, "AddVarBulk");
        if ((_error = dlerror()) != NULL) {call_error("AddVarBulk");}
        _AddNz = (l_lldl)dlsym(genmodel_lib, "AddNz");
        if ((_error = dlerror()) != NULL) {call_error("AddNz");}
        _AddNzToLast = (l_ldl)dlsym(genmodel_lib, "AddNzToLast");
        if ((_error = dlerror()) != NULL) {call_error("AddNzToLast");}
        _AddNzBulk = (l_lplpdplllll)dlsym(genmodel_lib, "AddNzBulk");
        if ((_error = dlerror()) != NULL) {call_error("AddNzBulk");}
        _SetQpCoef = (l_lldl)dlsym(genmodel_lib, "SetQpCoef");
        if ((_error = dlerror()) != NULL) {call_error("SetQpCoef");}
        _SetNumbers = (l_l)dlsym(genmodel_lib, "SetNumbers");
        if ((_error = dlerror()) != NULL) {call_error("SetNumbers");}
        _SetLongParam = (l_cpll)dlsym(genmodel_lib, "SetLongParam");
        if ((_error = dlerror()) != NULL) {call_error("SetLongParam");}
        _SetDblParam = (l_cpdl)dlsym(genmodel_lib, "SetDblParam");
        if ((_error = dlerror()) != NULL) {call_error("SetDblParam");}
        _SetBoolParam = (l_cpbl)dlsym(genmodel_lib, "SetBoolParam");
        if ((_error = dlerror()) != NULL) {call_error("SetBoolParam");}
        _SetStrParam = (l_cpcpl)dlsym(genmodel_lib, "SetStrParam");
        if ((_error = dlerror()) != NULL) {call_error("SetStrParam");}
        _CreateNewModel = (l_ccp)dlsym(genmodel_lib, "CreateNewModel");
        if ((_error = dlerror()) != NULL) {call_error("CreateNewModel");}
        _IsSolverAvailable = (b_c)dlsym(genmodel_lib, "IsSolverAvailable");
        if ((_error = dlerror()) != NULL) {call_error("IsSolverAvailable");}
        _CopyOrder = (l_liipip)dlsym(genmodel_lib, "CopyOrder");
        if ((_error = dlerror()) != NULL) {call_error("CopyOrder");}
        _DeleteModel = (l_l)dlsym(genmodel_lib, "DeleteModel");
        if ((_error = dlerror()) != NULL) {call_error("DeleteModel");}
        _CreateModel = (l_l)dlsym(genmodel_lib, "CreateModel");
        if ((_error = dlerror()) != NULL) {call_error("CreateModel");}
        _SolveModel = (l_l)dlsym(genmodel_lib, "SolveModel");
        if ((_error = dlerror()) != NULL) {call_error("SolveModel");}
        _GetSolVars = (b_dpll)dlsym(genmodel_lib, "GetSolVars");
        if ((_error = dlerror()) != NULL) {call_error("GetSolVars");}
        _HasSolution = (b_l)dlsym(genmodel_lib, "HasSolution");
        if ((_error = dlerror()) != NULL) {call_error("HasSolution");}
        _GetDualPrices = (b_dpll)dlsym(genmodel_lib, "GetDualPrices");
        if ((_error = dlerror()) != NULL) {call_error("GetDualPrices");}
        _GetSlacks = (b_dpll)dlsym(genmodel_lib, "GetSlacks");
        if ((_error = dlerror()) != NULL) {call_error("GetSlacks");}
        _GetReducedCosts = (b_dpll)dlsym(genmodel_lib, "GetReducedCosts");
        if ((_error = dlerror()) != NULL) {call_error("GetReducedCosts");}
        _GetRowValues = (b_dplll)dlsym(genmodel_lib, "GetRowValues");
        if ((_error = dlerror()) != NULL) {call_error("GetRowValues");}
        _GetObjCoef = (b_dpll)dlsym(genmodel_lib, "GetObjCoef");
        if ((_error = dlerror()) != NULL) {call_error("GetObjCoef");}
        _GetBounds = (b_dpdpll)dlsym(genmodel_lib, "GetBounds");
        if ((_error = dlerror()) != NULL) {call_error("GetBounds");}
        _GetLowerBound = (d_ll)dlsym(genmodel_lib, "GetLowerBound");
        if ((_error = dlerror()) != NULL) {call_error("GetLowerBound");}
        _GetUpperBound = (d_ll)dlsym(genmodel_lib, "GetUpperBound");
        if ((_error = dlerror()) != NULL) {call_error("GetUpperBound");}
        _SetLowerBound = (b_ldl)dlsym(genmodel_lib, "SetLowerBound");
        if ((_error = dlerror()) != NULL) {call_error("SetLowerBound");}
        _SetUpperBound = (b_ldl)dlsym(genmodel_lib, "SetUpperBound");
        if ((_error = dlerror()) != NULL) {call_error("SetUpperBound");}
        _GetRHS = (d_ll)dlsym(genmodel_lib, "GetRHS");
        if ((_error = dlerror()) != NULL) {call_error("GetRHS");}
        _SetRHS = (b_ldl)dlsym(genmodel_lib, "SetRHS");
        if ((_error = dlerror()) != NULL) {call_error("SetRHS");}
        _GetSense = (c_ll)dlsym(genmodel_lib, "GetSense");
        if ((_error = dlerror()) != NULL) {call_error("GetSense");}
        _SetSense = (b_lcl)dlsym(genmodel_lib, "SetSense");
        if ((_error = dlerror()) != NULL) {call_error("SetSense");}
        _GetObjVal = (d_l)dlsym(genmodel_lib, "GetObjVal");
        if ((_error = dlerror()) != NULL) {call_error("GetObjVal");}
        _ChangeBulkBounds = (l_iipcpdpl)dlsym(genmodel_lib, "ChangeBulkBounds");
        if ((_error = dlerror()) != NULL) {call_error("ChangeBulkBounds");}
        _ChangeBulkObjectives = (l_iipdpl)dlsym(genmodel_lib, "ChangeBulkObjectives");
        if ((_error = dlerror()) != NULL) {call_error("ChangeBulkObjectives");}
        _DeleteMipStarts = (l_l)dlsym(genmodel_lib, "DeleteMipStarts");
        if ((_error = dlerror()) != NULL) {call_error("DeleteMipStarts");}
        _GetMIPRelativeGap = (d_l)dlsym(genmodel_lib, "GetMIPRelativeGap");
        if ((_error = dlerror()) != NULL) {call_error("GetMIPRelativeGap");}
        printf("done.\n");
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
    dlclose(genmodel_lib);
    genmodel_lib = NULL;
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