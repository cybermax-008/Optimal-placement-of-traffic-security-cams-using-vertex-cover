#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <numeric>
#include <list>
#include<algorithm>

// defined std::unique_ptr
#include <memory>
// defines Var and Lit
#include "minisat/core/SolverTypes.h"
// defines Solver
#include "minisat/core/Solver.h"

using namespace std;

class graph{

    int vertexRange;
    int sourceVertex=-1;
    int destVertex=-1;
    vector<list<int>> adj;
    vector<vector<int>> adj_matrix;
    vector<int> edgeVector;

public:
    graph(int V);
    void setAdjMatrix(int vrange);
    void add_edge_list(int vrange,int v1, int v2);
    void printAdjList();
    void printAdjMatrix();
    void add_edge_matrix(int v1, int v2);
    vector<int> getEdgeVector(){return edgeVector;};
    void setEdgeVector(vector<int> e);
    int getVertexRange(){return vertexRange;};
    void changeVertexRange(int newVertexRange){ vertexRange=newVertexRange;};
    vector<list<int>> getAdjacentList(){return adj;};
    void clearAdjacentList(){ adj.clear();}
    int getSourceVertex(){return sourceVertex;};
    int getDestVertex(){return destVertex;};
    vector<vector<int>> getAdjacentMatrix(){return adj_matrix;}

};

graph::graph(int V){

    vertexRange=V;
}
void graph::add_edge_list(int vrange,int v1, int v2){
    adj.resize(vrange);
    adj[v1].push_back(v2); 
    adj[v2].push_back(v1); 
}

void graph::printAdjList()
{
    for(std::size_t i = 0; i < adj.size(); i++)
    {
        std::cout << i << ": ";
        for(auto &j : adj[i])//range based for loop to iterate through adjList[i] and use the reference j to refer to each element
        {
            std::cerr << j << ' ';
        }
        std::cerr << '\n';
    }
}

void graph::setAdjMatrix(int vrange)
{
    vector<vector<int>> m(vrange,vector<int> (vrange));
    adj_matrix=m;
}

void graph::add_edge_matrix(int v1, int v2)
{
    adj_matrix[v1][v2]=1;
    adj_matrix[v2][v1]=1;
}

void graph::printAdjMatrix()
{
    for(int i=0;i<adj_matrix.size();i++)
    {
        for(int j=0;j<adj_matrix[i].size();j++)
        {
            cout<<adj_matrix[i][j]<<" ";
        }
        cout<<endl;
    }
}

void graph::setEdgeVector(vector<int>e)
{
	edgeVector=e;
}

int extractvertexRange(string str) 
{ 
    stringstream ss;    
  
    /* Storing the whole string into string stream */
    ss << str; 
    /* Running loop till the end of the stream */
    string temp; 
    int foundVertexSize; 
    while (!ss.eof()) { 

        /* extracting word by word from stream */
        ss >> temp; 
  
        /* To check if a positive integer is captured */
        if ((stringstream(temp) >> foundVertexSize) && (foundVertexSize >= 0))
        {
            //cout << foundVertexSize; /* DEBUG_POINT:: Print the captured number*/
            break;
        }
        /* To save from space at the end of string */
        temp = ""; 
    } 
    return (foundVertexSize);
} 

vector<string> split (string edge_string, string delimiter) {

    //Removing the curly braces
    string start_delim="{";
    string stop_delim="}";
    unsigned first=edge_string.find(start_delim);
    unsigned last=edge_string.find(stop_delim);
    edge_string=edge_string.substr(first+1,last-3);

    //Split string based on a string delimiter
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = edge_string.find (delimiter, pos_start)) != string::npos) {
        token = edge_string.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (edge_string.substr (pos_start));
    return res;
}

void extractvertexSrcDst(string str, int &sourceVertex, int &destVertex) 
{ 
    stringstream ss;    
  
    /* Storing the whole string into string stream */
    ss << str; 
    /* Running loop till the end of the stream */
    string temp; 
    int flag=0; 
    while (!ss.eof()) { 

        /* extracting word by word from stream */
        ss >> temp; 
  
        /* To check if a positive integer is captured */
        if ((flag==0) && (stringstream(temp) >> sourceVertex) && (sourceVertex >=0))
        {
            flag+=1;
            //cout<<sourceVertex<<endl; //DEBUG_POINT
           continue;
        }
        else if((flag!=0) && (stringstream(temp) >> destVertex) && (destVertex >=0)){
            //cout<<destVertex<<endl; //DEBUG_POINT
            break;

        }
        else{
            continue;
        }
        /* To save from space at the end of string */
        temp = ""; 

    } 
}

bool BFS_Algo(vector<list<int>> adj, int src, int dest, int v, 
                            int pred[], int dist[]) 
{ 
    // a queue to maintain queue of vertices used in BFS
    list<int> queue; 
  
    // boolean array which stores the information of vertices that are used
    bool visited[v]; 
  
    for (int i = 0; i < v; i++) { 
        visited[i] = false; // initially all vertices are unvisited 
        dist[i] = INT8_MAX; // and as no path is yet constructed dist[i] for all i set to infinity
        pred[i] = -1; 
    } 
  
    
    visited[src] = true; // now source is first to be visited
    dist[src] = 0; // distance from source to itself should be 0 
    queue.push_back(src); 
  
    while (!queue.empty()) { 
        if( src==dest){
            return true;
        }
        int u = queue.front(); 
        queue.pop_front();
        list<int>::iterator it; 
        for (it = adj[u].begin(); it != adj[u].end(); ++it) { 
            if (visited[*it] == false) { 
                visited[*it] = true; 
                dist[*it] = dist[u] + 1; 
                pred[*it]= u; 
                queue.push_back(*it); 
 
                if (*it == dest){  // We stop BFS when we find destination.

                    queue.clear();
                    return true; 
                }
            } 
        } 
    } 
  
    return false; 
} 

void ShortestPath(vector<list<int>> adj, int s,  
                                    int dest, int v) 
{ 

    int pred[v], dist[v]; 

    if (BFS_Algo(adj, s, dest, v, pred, dist) == false) 
    { 
        cerr<< "Error:Given source and destination are not connected.\n"; 
        return;   
    } 
    if (s == dest){
        cout<<s<<"-"<<dest<<"\n";
        return;
    }
    else{ 
        vector<int> path; 
        int crawl = dest; 
        path.push_back(crawl); 
        while (pred[crawl] != -1) { 
            path.push_back(pred[crawl]); 
            crawl = pred[crawl]; 
        } 
      
        // distance from source is in distance array 
        // cout << "Shortest path length is : "
        //     << dist[dest]<<"\n"; 
        // cout<<path.size()<<"\n";
        // printing path from source to destination 
        for (int i = path.size() - 1; i >= 0; i--){ 
            cout << path[i]; 
            if(i>=1){
                cout<<"-";
            }
        }
        cout<<"\n";
        return;
    }
} 

vector<int> vertexCoverSAT(vector<int> edge_vector,vector<vector<int>> adjacentcyMatrix, int v)
{
    

    int n_max=v;
    int n_min=1;
    vector<int> vertexCover;
    vertexCover.clear();
    for(int k=1;k<=n_max;k++)
    {

        unique_ptr<Minisat::Solver> solver(new Minisat::Solver());
		Minisat::vec<Minisat::Lit> c;
		solver.reset (new Minisat::Solver());
        vector<vector<Minisat::Lit>> lit(n_max,vector<Minisat::Lit> (k));
        //cout<<"Started SAT function\n";

        //Creating lit matrix
        for(int i=0;i<n_max;i++)
        {
        	for(int j=0;j<k;j++)
        	{
        		lit[i][j]=Minisat::mkLit(solver->newVar());
        	}
        }

        //First Clause
        for(int i=0;i<k;i++)
        {
        	for(int n=0;n<n_max;n++)
        	{

        		c.push(lit[n][i]);
        	}
        	solver->addClause(c);
        	c.clear();       
        }

        //Second Clause
        for(int m=0;m<n_max;m++)
        {
        	for(int p=0;p<k;p++)
        	{
        		for(int q=p+1;q<k;q++)
        		{
        			solver->addClause(~lit[m][p],~lit[m][q]);
        		}
        	}
        }

        //Third Clause
        for(int m=0;m<k;m++)
        {
        	for(int p=0;p<n_max;p++)
        	{
        		for(int q=p+1;q<n_max;q++)
        		{
        			solver->addClause(~lit[p][m],~lit[q][m]);
        		}
        	}
        }

        //Fourth Clause
        for(int i=0;i<edge_vector.size();i=i+2)
        {
        	for(int j=0;j<k;j++)
        	{
        		c.push(lit[edge_vector[i]][j]);
        		c.push(lit[edge_vector[i+1]][j]);
        	}
        	solver->addClause(c);
        	c.clear();
        }

        //cout<<"Solivng...for k="<<k<<endl;
        Minisat::lbool res = solver->solveLimited();
    	//std::cout << "The result is: " << res << "\n";
    	if(res ==1)
    	{
    		for(int i=0;i<n_max;i++)
    		{
    			for(int j=0;j<k;j++)
    			{
    				if(Minisat::toInt(solver->modelValue(lit[i][j]))==0)
    				{
    					vertexCover.push_back(i);
    				}
    			}
    		}
    		sort(vertexCover.begin(),vertexCover.end());
    		return vertexCover;
    	} 
    }
}

int main(int argc, char **argv) {

    graph g(0);
    vector<list<int>>adjList=g.getAdjacentList();
    int flag_vc=0;
    //cout<<g.getVertexRange()<<"\n"; //DEBUG_POINT
    while(!cin.eof()){

        // read a line of input until EOL and store in a string
        string line;

        getline(cin,line);

        if( line[0] == 'V'){
            g.clearAdjacentList(); //The adjacentcyList is cleared once new Vertex range is provided
            //cout<< "inside V\n"; // DEBUG_POINT 
            g.changeVertexRange(extractvertexRange(line));
            int ver=g.getVertexRange();
            //vector<vector<int>> adjMatrix(ver,vector<int> (ver));
            g.setAdjMatrix(ver);
            //vector<vector<int>> adjMatrix1=g.getAdjacentMatrix();
            /* Following comments are for Debug Purpose:*/

            //cout <<"The graph vertex range is: " <<g.getVertexRange()<< "\n"; //DEBUG_POINT:: print the vertex caputured.
            continue;
        }

        else if( line[0] == 'E'){ 

            //adjList.resize(g.getVertexRange());
            if (g.getVertexRange() !=0){

                int flag=0; //flag to check if edge values intact

                vector<string> v=split(line,">"); //introduce first delimiter
                string firstFilter;
                for (auto const& s : v) { firstFilter += s; } //convert v into string
                //cout<<"The first filter: "<<result<<"\n"; //DEBUG_POINT

                v=split(firstFilter,"<"); //introduce second delimiter
                string secondFilter;
                for (auto const& s : v) { secondFilter += s; } //convert v into string
                //cout<<"The second filter: "<<result1<<"\n"; //DEBUG_POINT

                v=split(secondFilter,","); //introduce third delimiter

                //convert vector<string> to vector<int>
                vector<int> edgeVertexIndex ;
                for (auto &s : v) {
                    stringstream parser(s);
                    int x = 0;

                    parser >> x;
                    // Check if the vertex values are valid and if not raise the flag
                    if (x>=g.getVertexRange()){ 
                        g.changeVertexRange(0);
                        ++flag;
                        break;
                    }
                edgeVertexIndex.push_back(x);
                }
                g.setEdgeVector(edgeVertexIndex);

                //if the edge vertices are valid
                if (flag !=1){

                    for(unsigned int i=0;i<edgeVertexIndex.size();i=i+2){
                        g.add_edge_list(g.getVertexRange(),edgeVertexIndex[i],edgeVertexIndex[i+1]);
                        g.add_edge_matrix(edgeVertexIndex[i],edgeVertexIndex[i+1]);
                    }
                // for (auto i : edgeVertexIndex) cout << i << endl; //DEBUG_POINT
                //cout<<edgeVertexIndex[3]<<endl;
                //cout<< "inside E\n"; //DEBUG_POINT
                    flag_vc++;
                }

                else{

                    cerr<<"Error:One or more vertex values are invalid\n";
                    continue;
                }
            }
            else{

                cerr<<"Error:No vertex range available!\n";
                continue;
            }
        }

        else if( line[0] == 's'){ //add a check for edgeVertexIndex

            if(g.getVertexRange()!=0 && !adjList.empty()){
                int source=g.getSourceVertex();
                int dest=g.getDestVertex();
                extractvertexSrcDst(line,source,dest);
                //cout<<source<<", "<<dest<<endl; //DEBUG_POINT
                if((source>=0 && dest>=0)&&(source < g.getVertexRange() && dest < g.getVertexRange()))
                {
                    ShortestPath(adjList, source, dest, g.getVertexRange()); 
                    continue;
                }
                else{
                    g.changeVertexRange(0);
                    adjList.clear();
                    cerr<<"Error:Invalid Vertex\n";
                }
                //cout<< "inside s\n"; //DEBUG_POINT
            }
            else{
                if(g.getVertexRange()==0){
                    g.changeVertexRange(0);
                    adjList.clear();
                    cerr<<"Error:No vertex range available!\n";
                }
                else{
                    g.changeVertexRange(0);
                    adjList.clear();
                    cerr<<"Error:No Edges are available!\n";
                }
            }
        }
		adjList=g.getAdjacentList();
		vector<vector<int>>adjMatrix=g.getAdjacentMatrix();
		int vert=g.getVertexRange();
		vector<int> ee=g.getEdgeVector();

        if(flag_vc>0)
        {
        	//Initialize adjacent list and matrix for the given graph G=(V,E)
			//g.printAdjMatrix();
			vector<int> vc=vertexCoverSAT(ee,adjMatrix,vert);

			if(!vc.empty())
			{
				for(int i=0;i<vc.size();i++)
				{
					cout<<vc[i]<<" ";
				}
				cout<<endl;
			}
			flag_vc=0;
        }

    }

 return 0;
}
