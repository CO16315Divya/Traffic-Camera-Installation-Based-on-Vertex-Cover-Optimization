// Compile with c++ ece650-a2cpp -std=c++11 -o ece650-a2
#include <bits/stdc++.h>
#include<math.h>
using namespace std;

vector<int> bellman_ford(vector<vector<int>>& edges, int V, int s, int d) {
    // cout<<"Parsing";
    vector<int> dist(V, INT_MAX); //stores the minimum distance of vertex i from source
    vector<int> parent(V, -1); //stores the parent of vertex i in shortest path from source
    dist[s] = 0; //distance of source from source is 0

    //iterate V-1 times
    for (int i = 0; i < V-1; i++) {
        //iterate over all edges
        for (int j = 0; j < edges.size(); j++) {
            int u = edges[j][0];
            int v = edges[j][1];
            //if shortest path from source to v exists through source->u->v, update
            if (dist[u] != INT_MAX && dist[u] + 1 < dist[v]) {
                dist[v] = dist[u] + 1;
                parent[v] = u;
            }
        }
    }

    //path stores the shortest path from source to destination
    vector<int> path;
    int t = d; 

    //backtrack until we find source (as parent[source]=-1), or if we find an unreachable node from s 
    //(i.e. parent[v] = -1)
    while (t != -1) {
        path.push_back(t);
        t = parent[t];
    }
    //reverse the path
    reverse(path.begin(), path.end());
    
    //if the first node in the found path is source, return path, else return empty path
    if (path[0] == s) return path;
    else return {};
}

int main(int argc, char** argv) {
    //declare an edges vector of vector, where edges[i] gives the edge in the format {u, v}
    vector<vector<int>> edges;
    //input specifies the number of vertices of graph
    int V=-1;
    
    // cout<<"In A2\n";


    // read from stdin until EOF
    while (!cin.eof()) {
        // read a line of input until EOL and store in a string
        string line;
        getline(cin, line);
        bool flag=false;
        // create an input stream based on the line
        // we will use the input stream to parse the line
        
        istringstream input(line);
        // line=line+"\n"+"s 1 2";
        // cout<<line<<endl;
        string type; //specifies the type of line (V, E, or s)

        // while there are characters in the input line
        while (!input.eof()) {
            input >> type; //the type of command
            if (type == "V") {
                input >> V;
                // cout<<"In V"<<V;
                if(V<=1){
                    // cout<<"Flag is true";
                    flag=true;
                }
                cout<<line<<endl;
                // cout<<"V "<<V<<endl;
                //clear and reinitialize the edges vector 
                edges.clear();
            }
            else if (type == "E") {
                // cout<<"IN Edges\n";
                cout<<line<<endl;
                if(flag==true) {

                    std::cerr << "Error: Insufficient edges to generate graph" << std::endl;
                }
                int st = line.find('{');
                int en = line.find('}');
                string edgelist = line.substr(st+1, en-st-1); //contains the list of edges within braces
                

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
                    int u = temp[0] - 1; //subtract -1 as 0 based indexing for edges
                    int v = temp[1] - 1;
                    if (u >= V) {
                        cout << "Error: Vertex "<< u+1 << " does not exist\n";
                        break;
                    }
                    else if (v >= V) {
                        cout << "Error: Vertex "<< v+1 << " does not exist\n";
                        break;
                    }
                    else {
                        edges.push_back({u, v});
                        edges.push_back({v, u});
                    }
                    edgelist = edgelist.substr(r+1); //update edgelist
                }
                // for (auto x : edges) {
                //     if( % 2 !=0){
                //         cout << x[0]+1 << " " << x[1]+1 << endl;
                //     }
                    
                // }
                
            }
            else if (type == "s") {
                // cout<<"IN S\n";
                vector<int> temp;
                int i;
                while (input >> i) {
                    temp.push_back(i);
                }
                int s = temp[0] - 1; //source vertex
                int d = temp[1] - 1; //destination vertex
                if (s >= V) {
                        cout << "Error: Vertex "<< s+1 << " does not exist\n";
                        break;
                    }
                else if (d >= V) {
                    cout << "Error: Vertex "<< d+1 << " does not exist\n";
                    break;
                }
                vector<int> path; //path from source to destination
                path = bellman_ford(edges, V, s, d);
                //if returned path is empty, output path does not exist
                if (path.size() == 0) {
                    cout << "Error: Path does not exist\n";
                }
                else {
                    //output the shortest path
                    for (int i = 0; i < path.size(); i++) {
                        cout << path[i] + 1;
                        if (i != path.size()-1) cout << "-";
                    }
                }

                cout << endl;
            }
            else {
                //cout << "type is " << type << endl;
            }
        }
    }
}
