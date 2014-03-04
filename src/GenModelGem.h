/***************************************************************************
 *  GenModelGem.h
 *  GenModel gem library
 *
 *  February 28 14:18 2014
 *  Copyright  2014  Mathieu Bouchard
 *  mathbouchard@gmail.com
 ****************************************************************************/

#ifndef GenModelGem_H
#define GenModelGem_H

#if defined WIN64 || defined WIN32
	#ifndef snprintf
		#define snprintf sprintf_s
	#endif
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;

double FindConstraintMaxLhs(long row, long token);
double FindConstraintMinLhs(long row, long token);
long MakeConstraintFeasible(long row, long token);
long WriteProblemToLpFile(char* filename, long token);
long WriteSolutionToFile(char* filename, long token);
long AddConst(char* cname, double rhs, char sense, long token);
bool AddConstBulk(char* cname, double* rhs, long length, char sense, long token);
long AddVar(char* nn, double o, double l, double u, char t, long token);
bool AddVarBulk(char* nn, double* o, long length, double l, double u, char t, long token);
long AddNz(long row, long col, double val, long token);
long AddNzToLast(long col, double val, long token);
long AddNzBulk(long* rows, long* cols, double* values, long valuesLength, long rowCount, long colCount, long iterations, long token);
long SetQpCoef(long i, long j, double val, long token);
long SetNumbers(long token);
long SetLongParam(char* param, long val, long token);
long SetDblParam(char* param, double val, long token);
long SetBoolParam(char* param, bool val, long token);
long SetStrParam(char* param, char* val, long token);
long CreateNewModel(char type, char* name);
bool IsSolverAvailable(char type);
long CopyOrder(long token, int count, int* ind, int* weight);
long DeleteModel(long token);
long CreateModel(long token);
long SolveModel(long token);
bool GetSolVars(double* values, long length, long token);
bool HasSolution(long token);
bool GetDualPrices(double* values, long length, long token);
bool GetSlacks(double* values, long length, long token);
bool GetReducedCosts(double* values, long length, long token);
bool GetRowValues(double* values, long length, long rowIndex, long token);
bool GetObjCoef(double* values, long length, long token);
bool GetBounds(double* lb, double* ub, long length, long token);
double GetLowerBound(long col, long token);
double GetUpperBound(long col, long token);
bool SetLowerBound(long col, double val, long token);
bool SetUpperBound(long col, double val, long token);
double GetRHS(long row, long token);
bool SetRHS(long row, double val, long token);
char GetSense(long row, long token);
bool SetSense(long row, char sense, long token);
double GetObjVal(long token);
long ChangeBulkBounds(int count, int* indices, char* types, double* values, long token);
long ChangeBulkObjectives(int count, int* indices, double* values, long token);
long DeleteMipStarts(long token);
double GetMIPRelativeGap(long token);

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

#endif // GenModelGem_H
