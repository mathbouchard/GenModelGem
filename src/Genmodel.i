/* ScheduleMathTools : main header file for the GenModel gem */

%include exception.i

%exception {
	try { $function }
    catch(string str) { SWIG_exception(SWIG_RuntimeError,str.c_str()); }
    catch(...) { SWIG_exception(SWIG_RuntimeError,"Unknown exception"); }
}

%module Genmodel
%{
    #include "InterfaceObject.h"
    #include "GenModel.h"
    #include "GenModelCplex.h"
    #include "GenModelOsi.h"
%}

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

%template(IntVector) InterfaceVector<int>;
%template(LongVector) InterfaceVector<long>;
%template(DoubleVector) InterfaceVector<double>;

class GenModel
{
public:
	GenModel();
	virtual ~GenModel()  { ClearStructure(); };
	long AddIndexToCoef(string coef, string index);
	long AddCoef(string coef);
	long CoefIndex(string coef, int nargs, ...);
	long AddConst(string cname);
	long AddConst(string cname, double rhs, char sense);
	long AddVar(string nn, double o, double l, double u, char t);
	long AddVars(string nn, long size, double o, double l, double u, char t);
	long AddModelCol(vector<int>& ind, vector<double>& val, double obj, double lb, double ub, string name, char type = 'C');
	long AddModelRow(vector<int>& ind, vector<double>& val, double rhs, char sense, string name);
	long SetQpCoef(long i, long j, double val);
	long AddNz(long row, long col, double val);
	long AddNzToLast(long col, double val);
	long SetNumbers();
	long ClearStructure();
	long PrintSol();
    long PrintModel();
	long PrintSol(string v);
	long PrintSolNz();
	long PrintSolNz(string v);
	long PrintObjVal();
	long SetLongParam(string param, long val);
	long SetDblParam(string param, double val);
	long SetBoolParam(string param, bool val);
	long SetStrParam(string param, string val);
    long ThrowError(string error);
	virtual long Init(string name) = 0;
	virtual long CreateModel() = 0;
	virtual long Solve() = 0;
	virtual long SetSol() = 0;
    virtual long ChangeBulkBounds(int count, int * ind, char * type, double * vals);
	virtual long WriteProblemToLpFile(string filename);
    virtual long WriteSolutionToFile(string filename);
	virtual long ChangeBulkObjectives(int count, int * ind, double * vals);
	virtual long DeleteMipStarts();
	virtual double GetMIPRelativeGap();
    virtual double FindConstraintMaxLhs(long row);
    virtual double FindConstraintMinLhs(long row);
    virtual long MakeConstraintFeasible(long row);
    bool binit;
    bool bcreated;
    string name;
	
	vector<ModConsts> consts;
	map<string,long> ci;
	unsigned long nc;
	unsigned long nr;
	unsigned long nz;
	ModVars vars;
    string version;
    
	double objval;
	int solstat;
	bool feasible;
	bool dualfeasible;
	bool hassolution;
	void* solverdata;
	map<string, long> longParam;
	map<string, double> dblParam;
	map<string, bool> boolParam;
	map<string, string> strParam;
};

class GenModelCplex : public GenModel
{
public:
    GenModelCplex();
    ~GenModelCplex();
	long Init(string name);
    long CreateModel(string filename, int type=0, string dn="");
	long CreateModel();
	long AddSolverCol(vector<int>& ind, vector<double>& val, double obj, double lb, double ub, string name, char type = 'C');
	long AddSolverRow(vector<int>& ind, vector<double>& val, double rhs, char sense, string name);
	long AddCol(int* newi, double* newcol, int nz, double obj, double lb, double ub, const char* name, char type = 'C');
	long AddCut(int* cols, double* vals, int nz, double rhs, char sense, const char* name);
	long ChangeBulkBounds(int count, int * ind, char * type, double * vals);
	long ChangeBulkObjectives(int count, int * ind, double * vals);
    long ChangeBulkNz(int count, int* rind, int* cind, double* vals);
    long WriteProblemToLpFile(string filename);
    long WriteSolutionToFile(string filename);
    long SwitchToMip();
    long SwitchToLp();
	long DeleteMipStarts();
	long Solve();
	long SetSol();
	long Clean();
	double GetMIPRelativeGap();
    long SetDirectParam(int whichparam, genmodel_param value, string type, string message);
    long SetParam(string param, int whichparam, string type, string message, bool implemented = true);
};

class GenModelOsi : public GenModel
{
public:
	~GenModelOsi() {if (solverdata != NULL) delete static_cast<OsiData*>(solverdata);}
	long Init(string name);//, int type=0);
	long CreateModel();
    long CreateModel(string filename, int type=0, string dn="");
	long AddSolverRow(vector<int>& ind, vector<double>& val, double rhs, char sense, string name);
	long AddSolverCol(vector<int>& ind, vector<double>& val, double obj, double lb, double ub, string name, char type = 'C');
	long AddCut(int* cols, double* vals, int nz, double rhs, char sense, const char* name);
	long AddCol(int* newi, double* newcol, int nz, double obj, double lb, double ub, const char* name, char type = 'C');
    long WriteProblemToLpFile(string filename);
    long WriteSolutionToFile(string filename);
	long Solve();
	long SetSol();
	long Clean();
    long SetDirectParam(int whichparam, genmodel_param value, string type, string message);
    long SetParam(string param, int whichparam, string type, string message, bool implemented = true);
};
