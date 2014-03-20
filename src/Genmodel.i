/* ScheduleMathTools : main header file for the GenModel gem */



%exception {
	try { $function }
    catch(string str) { SWIG_exception(SWIG_RuntimeError,str.c_str()); }
    catch(...) { SWIG_exception(SWIG_RuntimeError,"Unknown exception"); }
}

%module Genmodel

%include exception.i
%include std_vector.i
%include std_string.i

%{
    #include "GenModel.h"
    #include "GenModelCplex.h"
    #include "GenModelOsi.h"
%}

%inline%{
    std::vector<double> test;
    vector<double> test1;
%}

%template(IntVector) std::vector<int>;
%template(LongVector) std::vector<long>;
%template(DoubleVector) std::vector<double>;
%template(BoolVector) std::vector<bool>;
%template(StringVector) std::vector<std::string>;

class ModVars
{
public:
	ModVars() {n=0; defub=DBL_MAX; deflb=-DBL_MAX;}
	long AddVar(string nn, double o, double l, double u, char t);
	long AddVars(string nn, long size, double o, double l, double u, char t);
    
    std::vector<double> GetSolution();
    double GetSolutionFromIndex(unsigned long index);
    
	long SetQpCoef(long i, long j, double val);
	long Print();
	vector<string> name;
	vector<double> obj;
	vector<char> type;
	map<string, long> offset;
	vector<double> ub;
	vector<double> lb;
    std::vector<double> sol;
	vector<double> rc;
	vector<double> qobj;
	vector<long> qi;
	vector<long> qj;
	double defub;
	double deflb;
    
	unsigned long n;
};

class ModConsts
{
public:
	ModConsts() {nz=0; lrhs=0.0; sense = 'E'; urhs=0.0;}
	long AddNz(long c, double v);
	string name;
    std::vector<long> cols;
    std::vector<double> coefs;
	double dual;
	double slack;
	double lrhs;
	char sense;
	double urhs;
	long id;
	unsigned long nz;
};

class GenModel
{
public:
	GenModel();
	virtual ~GenModel()  { ClearStructure(); };
	long AddIndexToCoef(string coef, string index);
	long AddCoef(string coef);
	long CoefIndex(string coef, int nargs, ...);
	long AddConst(string cname);
	long AddConst(std::string cname, double rhs, char sense);
	long AddVar(std::string nn, double o, double l, double u, char t);
	long AddVars(std::string nn, long size, double o, double l, double u, char t);
	long AddModelCol(std::vector<int>& ind, std::vector<double>& val, double obj, double lb, double ub, std::string name, char type = 'C');
	long AddModelRow(std::vector<int>& ind, std::vector<double>& val, double rhs, char sense, std::string name);
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
	long SetLongParam(std::string param, long val);
	long SetDblParam(std::string param, double val);
	long SetBoolParam(std::string param, bool val);
	long SetStrParam(std::string param, std::string val);
    long ThrowError(string error);
	virtual long Init(std::string name) = 0;
	virtual long CreateModel() = 0;
	virtual long Solve() = 0;
	virtual long SetSol() = 0;
    virtual long ChangeBulkBounds(int count, int * ind, char * type, double * vals);
	virtual long WriteProblemToLpFile(std::string filename);
    virtual long WriteSolutionToFile(std::string filename);
	virtual long ChangeBulkObjectives(int count, int * ind, double * vals);
	virtual long DeleteMipStarts();
	virtual double GetMIPRelativeGap();
    virtual double FindConstraintMaxLhs(long row);
    virtual double FindConstraintMinLhs(long row);
    virtual long MakeConstraintFeasible(long row);
    bool binit;
    bool bcreated;
    string name;
	
    std::vector<ModConsts> consts;
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
	long Init(std::string name);
    long CreateModel(std::string filename, int type=0, string dn="");
	long CreateModel();
	long AddSolverCol(std::vector<int>& ind, std::vector<double>& val, double obj, double lb, double ub, std::string name, char type = 'C');
	long AddSolverRow(std::vector<int>& ind, std::vector<double>& val, double rhs, char sense, std::string name);
	long AddCol(int* newi, double* newcol, int nz, double obj, double lb, double ub, const char* name, char type = 'C');
	long AddCut(int* cols, double* vals, int nz, double rhs, char sense, const char* name);
	long ChangeBulkBounds(int count, int * ind, char * type, double * vals);
	long ChangeBulkObjectives(int count, int * ind, double * vals);
    long ChangeBulkNz(int count, int* rind, int* cind, double* vals);
    long WriteProblemToLpFile(std::string filename);
    long WriteSolutionToFile(std::string filename);
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
	long Init(std::string name);//, int type=0);
	long CreateModel();
    long CreateModel(std::string filename, int type=0, string dn="");
	long AddSolverRow(std::vector<int>& ind, std::vector<double>& val, double rhs, char sense, std::string name);
	long AddSolverCol(std::vector<int>& ind, std::vector<double>& val, double obj, double lb, double ub, std::string name, char type = 'C');
	long AddCut(int* cols, double* vals, int nz, double rhs, char sense, const char* name);
	long AddCol(int* newi, double* newcol, int nz, double obj, double lb, double ub, const char* name, char type = 'C');
    long WriteProblemToLpFile(std::string filename);
    long WriteSolutionToFile(std::string filename);
	long Solve();
	long SetSol();
	long Clean();
    long SetDirectParam(int whichparam, genmodel_param value, string type, string message);
    long SetParam(string param, int whichparam, string type, string message, bool implemented = true);
};
