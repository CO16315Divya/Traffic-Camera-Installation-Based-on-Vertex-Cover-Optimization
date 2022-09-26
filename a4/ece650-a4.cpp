#include <bits/stdc++.h>
// defined std::unique_ptr
#include <memory>
// defines Var and Lit
#include "minisat/core/SolverTypes.h"
// defines Solver
#include "minisat/core/Solver.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>
#include <set>
#include <pthread.h>
#include <errno.h>

using namespace std;

vector<vector<int>> edges;
int max_v=-1;
bool flag=true;

pair<bool, string> cnf(vector<vector<int>>& E_, int V, int k) {
    using Minisat::Var;
    using Minisat::Lit;
    using Minisat::mkLit;
    using Minisat::lbool;
    using Minisat::l_True;
    using Minisat::vec;
    Var arr[V][k]= {0};
    Minisat::Solver solver;
    string output = "";
    // Create variables
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < k; j++) {
            arr[i][j] = solver.newVar();
        }
    }
    // Create the clauses
    for (int j = 0; j < k; j++) {
        vec<Lit> v;
        for (int i = 0; i < V; i++) {
            Lit l = mkLit(arr[i][j]);
            const Lit& l_ = l;
            v.push(l_);
        }
        const vec<Lit>& v_ = v;
        solver.addClause(v_);
    }
    for (int i = 0; i < V; i++) {
        for (int p = 0; p < k; p++) {
            for (int q = p + 1; q < k; q++) {
                solver.addClause(~mkLit(arr[i][p]), ~mkLit(arr[i][q]));
            }
        }
    }
    for (int j = 0; j < k; j++) {
        for (int p = 0; p < V; p++) {
            for (int q = p + 1; q < V; q++) {
                solver.addClause(~mkLit(arr[p][j]), ~mkLit(arr[q][j]));
            }
        }
    }
    for (auto it = E_.begin(); it != E_.end(); ++it) {
        int i = (*it).front();
        int j = (*it).back();
        vec<Lit> v;
        for (int x = 0; x < k; x++) {
            Lit l1 = mkLit(arr[i][x]);
            const Lit& l1_ = l1;
            v.push(l1_);
            Lit l2 = mkLit(arr[j][x]);
            const Lit& l2_ = l2;
            v.push(l2_);
        }
        const vec<Lit>& v_ = v;
        solver.addClause(v_);
    }

    // Check for solution and retrieve model if found
    auto sat = solver.solve();
    if (sat) {
        vector<int> vertices;
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < k; j++) {
                if (solver.modelValue(arr[i][j]) == l_True) {
                    vertices.push_back(i);
                }
            }
        }
        //sorting
        sort(vertices.begin(), vertices.end());
        for (auto it = vertices.begin(); it != vertices.end(); ++it) {
            output += to_string(*(it));
            if ((it + 1) != vertices.end()) {
                output += " ";
            }
        }
        output += "\n";
        return make_pair(true, output);
    } else {
        return make_pair(false, output);
        ;
    }
}


void vertexCover(vector<vector<int>>& edges, int max_v,int V){
    int low=1;
    string outputStr="";
    vector<vector<int>>& E_a=edges;
    int high=max_v;
    int mid;
    while (low <= high) {
        mid = (high + low) / 2;
        pair<bool, string> p = cnf(E_a, max_v + 1, mid);
        if (p.first) {
            high = mid - 1;
            outputStr = p.second;
        } else {
            low = mid + 1;
        }
    }
    cout<<outputStr;
    

} 

int main(int argc, char** argv) {
    //declare an edges vector of vector, where edges[i] gives the edge in the format {u, v}
    
    //input specifies the number of vertices of graph
    int V=1;
   
    // read from stdin until EOF
    while (!cin.eof()) {
        // read a line of input until EOL and store in a string
        string line;
        getline(cin, line);
        
        // create an input stream based on the line
        // we will use the input stream to parse the line
        istringstream input(line);

        string type; //specifies the type of line (V, E, or s)

        // while there are characters in the input line
        while (!input.eof()) {
            
            input >> type; //the type of command
            if (type == "V") {
                
                input >> V;
                //clear and reinitialize the edges vector
                // if(V==1) {
                //     flag=false;
                //     cout<<"Error: Vertex must be greater 1\n";
                // }
                edges.clear();
            }
            else if (type == "E") {
                bool flag=true;
                int st = line.find('{');
                int en = line.find('}');
                string edgelist = line.substr(st+1, en-st-1); //contains the list of edges within braces

                if(edgelist=="") flag=false;
                //iterate till we find new edges in the line
                while (edgelist.find('<') != string::npos) {

                    int l = edgelist.find('<');
                    int r = edgelist.find('>');
                    string edge = edgelist.substr(l+1, r-l-1); //gives the edges (u,v)
                    vector<int> temp;
                    stringstream ss(edge); //used to extract the edge vertices from (u,v)
                    int i;
                    while (ss >> i)
                    {
                        temp.push_back(i);
                        if (ss.peek() == ',') //ignore if input is comma
                            ss.ignore();
                    }
                    int u = temp[0]; //subtract -1 as 0 based indexing for edges
                    int v = temp[1];
                    if (u > V) {
                        cout << "Error: Vertex "<< u << " does not exist\n";
                        flag= false;
                        break;
                    }
                    else if (v > V) {
                        cout << "Error: Vertex "<< v << " does not exist\n";
                        flag= false;
                        break;
                    }
                    else {
                        if(u>v){
                            if(u>max_v){
                                max_v=u;
                            }
                        }
                        else{
                            if(v>max_v){
                                max_v=v;
                            }

                        }
                        edges.push_back({u, v});
                        // edges.push_back({v, u});
                    }
                    edgelist = edgelist.substr(r+1); //update edgelist
                }
                // for (auto x : edges) 
                // {
                //     cout << x[0] << " " << x[1] << endl;
                // }
                // cout<<"No of vertices"<<V;
                if(flag){

                vertexCover(edges,max_v,V);
                max_v=-1;  

            } 
            }   
            
                  
        }
    }
}
