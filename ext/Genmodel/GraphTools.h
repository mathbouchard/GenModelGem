//
//  GraphTools.h
//  
//
//  Created by Mathieu Bouchard on 2014-03-21.
//
//

#ifndef _GraphTools_h
#define _GraphTools_h

#include <lemon/concepts/graph.h>
#include <lemon/list_graph.h>
#include "BitVector.h"

using namespace lemon;
using namespace std;

class GmGraph
{
public:
    GmGraph() {}
    ~GmGraph() {}
    size_t AddNode();
    size_t AddEdge(size_t i, size_t j);
    void EraseNode(size_t i);
    void EraseEdge(size_t i);
    void ChangeU(size_t i, size_t j);
    void ChangeV(size_t i, size_t j);
    size_t u(size_t i);
    size_t v(size_t i);
    void Contract(size_t i, size_t j, bool r = true);
    void Clear();
    void ReserveNode(size_t n);
    void ReserveEdge(size_t m);
    size_t n();
    size_t e();
    size_t di(size_t i);
    vector<size_t> ie(size_t i);
    vector<size_t> in(size_t i);
    BitVector ie2bv(size_t i);
    BitVector in2bv(size_t i);
    
    vector<vector<size_t> > GetCliqueCover();
    
    ListGraph g;
};

#endif
