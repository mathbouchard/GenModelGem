//
//  GraphTools.cpp
//  
//
//  Created by Mathieu Bouchard on 2014-03-21.
//
//

#include <GraphTools.h>
#include <lemon/list_graph.h>
#include <stdlib.h>
#include <time.h>

# define n_id nodeFromId
# define e_id edgeFromId

vector<vector<size_t> > GmGraph::GetCliqueCover()
{
    size_t n = countNodes(g);
    size_t e = countEdges(g);
    vector<vector<size_t> > out;

    vector<bool> done(e,false);
    size_t curr_e = 0;
    size_t added = 0;
    while(true)
    {
        while(curr_e < e && done[curr_e])
            ++curr_e;
        
        if(curr_e == e)
            break;
        done[curr_e] = true;
        added++;
        //printf("curr_e = %ld (added = %ld)\n", curr_e, added);
        
        vector<bool> node_in(n,false);
        vector<size_t> node_depth(n,0);
        
        vector<size_t> clique;
        size_t curr_n = g.id(g.u(g.e_id(curr_e)));
        size_t next_n = g.id(g.v(g.e_id(curr_e)));
        
        //ve.push_back(e);
        
        clique.push_back(curr_n);
        node_in[curr_n] = true;
        
        vector<size_t> dc = in(curr_n);
        for(size_t i = 0; i < dc.size(); i++)
            ++(node_depth[dc[i]]);
        size_t depth = 1;
        
        while(next_n != -1 && depth < e*e)
        {
            //printf("1-Add %ld (depth %ld)\n", next_n, depth);

            clique.push_back(next_n);
            node_in[next_n] = true;
            vector<size_t> de = ie(next_n);
            depth++;
            curr_n = next_n;
            next_n = -1;
            size_t next_nn = -1;
            for(size_t i = 0; i < de.size(); i++)
            {
                size_t nn = (g.id(g.u(g.e_id(de[i]))) == curr_n ?  g.id(g.v(g.e_id(de[i]))) : g.id(g.u(g.e_id(de[i]))));
                size_t onn = (g.id(g.u(g.e_id(de[i]))) == curr_n ?  g.id(g.u(g.e_id(de[i]))) : g.id(g.v(g.e_id(de[i]))));
                ++(node_depth[nn]);
                if(node_in[nn] && !done[de[i]])
                {
                    done[de[i]] = true;
                    added++;
                    //printf("\t2-From %ld : Add edge %ld (depth %ld) [added = %ld] d=%ld --> %ld (other = %ld)\n", curr_n, de[i], depth, added, de.size(), nn, onn);
                }
                if(node_depth[nn] == depth && next_n == -1 && !done[de[i]] && !node_in[nn])
                {
                    next_n = nn;
                    done[de[i]] = true;
                    added++;
                    //printf("\t3-Add edge %ld (depth %ld) [added = %ld]\n", de[i], depth, added);
                }
                else if(node_depth[nn] == depth && next_n == -1 && !node_in[nn])
                {
                    next_nn = nn;
                }
            }
            if (next_n == -1)
                next_n = next_nn;
        }
        if (depth == e*e)
            throw string("GmGraph::GetCliqueCover : Max depth reached");
        out.push_back(clique);
        if(added >= e)
            break;
    }
    if(added < e)
        throw string("GmGraph::GetCliqueCover : The graph was not covered by clique.");
    //printf("Finish %ld!\n", added);
    
    return out;
}

vector<size_t> GmGraph::ie(size_t i)
{
    vector<size_t> out(di(i));
    size_t ii = 0;
    for (ListGraph::IncEdgeIt _e(g, g.n_id(i)); _e != INVALID; ++_e, ++ii)
        out[ii] = (g.id(_e)-g.id(_e)%2)/2;
    return out;
}

BitVector GmGraph::ie2bv(size_t i)
{
    BitVector out(countEdges(g)*2);
    for (ListGraph::IncEdgeIt _e(g, g.n_id(i)); _e != INVALID; ++_e)
        out.set((g.id(_e)-g.id(_e)%2)/2, true);
    return out;
}

vector<size_t> GmGraph::in(size_t i)
{
    vector<size_t> out(di(i));
    size_t ii = 0;
    for (ListGraph::IncEdgeIt _e(g, g.n_id(i)); _e != INVALID; ++_e, ++ii)
        out[ii] = (g.id(g.u(_e)) == i ?  g.id(g.v(_e)) : g.id(g.u(_e)));
    return out;
}

BitVector GmGraph::in2bv(size_t i)
{
    BitVector out(countNodes(g));
    for (ListGraph::IncEdgeIt _e(g, g.n_id(i)); _e != INVALID; ++_e)
        (g.id(g.u(_e)) == i ?  out.set(g.id(g.v(_e)),true) : out.set(g.id(g.u(_e)),true));
    return out;
}


size_t GmGraph::n()
{
    return countNodes(g);
}

size_t GmGraph::e()
{
    return countEdges(g);
}

size_t GmGraph::di(size_t i)
{
    return countIncEdges(g, g.n_id(i));
}

size_t GmGraph::AddNode()
{
    return g.id(g.addNode());
}

size_t GmGraph::AddEdge(size_t i, size_t j)
{
    return g.id(g.addEdge(g.n_id(i), g.n_id(j)));
}

size_t GmGraph::u(size_t i)
{
    return g.id(g.u(g.e_id(i)));
}
size_t GmGraph::v(size_t i)
{
    return g.id(g.v(g.e_id(i)));
}

void GmGraph::EraseNode(size_t i)
{
    g.erase(g.n_id(i));
}

void GmGraph::EraseEdge(size_t i)
{
    g.erase(g.e_id(i));
}

void GmGraph::ChangeU(size_t i, size_t j)
{
    g.changeU(g.e_id(i),g.n_id(j));
}

void GmGraph::ChangeV(size_t i, size_t j)
{
    g.changeV(g.e_id(i),g.n_id(j));
}

void GmGraph::Contract(size_t i, size_t j, bool r)
{
    g.contract(g.n_id(i), g.n_id(j), r);
}

void GmGraph::Clear()
{
    g.clear();
}

void GmGraph::ReserveNode(size_t n)
{
     g.reserveNode(n);
}

void GmGraph::ReserveEdge(size_t m)
{
    g.reserveEdge(2 * m);
}