#include <iostream>
#include <string>
#include <sstream>
#include <list>
#include <vector>
#include <climits>
#include <algorithm>
#include <pthread.h>
#include <time.h>
#include <cstdlib>
#include <memory>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "minisat/core/SolverTypes.h"
#include "minisat/core/Solver.h"

using namespace std;

std::unique_ptr<Minisat::Solver> solver(new Minisat::Solver());
double vf, vc1f, vc2f; 
int fvertix;
vector <int> input; // collect Edges
vector <int>  vertexcoverresult; // collect the vertexcovers. 
vector <int> approxvc1result; // approx vc1.
vector <int> approxvc2result; // approx vc2.
void* vertexcover1(void* arg);
void* mains(void* arg);
void* Ass4(void* arg);
int getdegree(int node, vector<int> edgex);
vector<int> remove(int node, vector<int> edgex);
void* approxvc1(void* arg);
void* approxvc2(void* arg);


// clockid_t cidvx;
// clockid_t cidvc1;
// clockid_t cidvc2;
string done;

int s; 
// static long double timerx; 
// #define handle_error(msg) \
//                do { perror(msg); exit(EXIT_FAILURE); } while (0)

// #define handle_error_en(en, msg) \
//                do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)
// static long double pclock(clockid_t cid)
//        {
//            struct timespec ts;
//            if (clock_gettime(cid, &ts) == -1)
//                handle_error("clock_gettime");
//         //   printf("%4ld.%03ld\n", ts.tv_sec, ts.tv_nsec );
// 		 //cout << (ts.tv_sec*1000000)+(ts.tv_nsec/1000);
// 		  //  printf("%4ld.%03ld\n",(ts.tv_sec*1000000)+(ts.tv_nsec/1000) );
// 		 //cout << endl;
// 		 return ((ts.tv_sec*1000000)+(ts.tv_nsec/1000));
//        }

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ MAIN +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int main() {
	pthread_t main;
    //clockid_t maintime;
    
	pthread_create(&main, NULL, mains, NULL);
	pthread_join(main, NULL);
	
/*    pclock("Process total CPU time: ", CLOCK_PROCESS_CPUTIME_ID);

   s = pthread_getcpuclockid(pthread_self(), &cid);
    if (s != 0)
        handle_error_en(s, "pthread_getcpuclockid");
    pclock("Main thread CPU time:   ", cid);
	return 0;*/
}
void* mains(void* arg) {
	//--------------------------------------------------------------------------------------- INTIALIZATIONS-----------------------------------------
	string Edges;// define string edges
	//  Define important variables to navigate string  ;
	istringstream ss;
	int row; // number of Edges pairs
	int rows = 0;
	int column = 2;// for a 2 dimentional use for future
	bool compare = false; // SO VERTIX COVER DOES NOT PRINT UNNCESSARILLY 
	vector <int> vertix; // collet highest Vertex
	vector <int> shortpath;// collect shortpath source and destination
	//int fvertix = 0; // the maximum number of nodes//**********************************notice fvertix is made global ********************
	// vector<int> adjacent[fvertix];
	while (!std::cin.eof()) { // to read end of line
		getline(cin, Edges);// collect inputs from user
		int size1, size2; // for sizes of strings
		size1 = Edges.size();
		row = 0;
		if (Edges[0] == 'V')//------------------------------------------------- if V was the comand --------------------
		{
			input.clear();
			shortpath.clear();
			vertix.clear();
			//  row = 0;
			string v1;
			//int fvertix2;
			for (int t = 0; t <= size1; t++) {
				if (isdigit(Edges[t])) {
					v1 += Edges[t];
					if (isdigit(Edges[t + 1])) {
						v1 += Edges[t + 1];
						t++;
					}
					istringstream(v1) >> fvertix;
					//istringstream(v1) >> fvertix2;
					vertix.push_back(fvertix);
				}
				v1.clear();
			}
			size2 = vertix.size();
			for (int i = 0; i < size2; i++) {
				// cout << "highest number of node" << fvertix<< endl;
			}
			if (fvertix == 0) {
				cerr << "Error: Highest number of nodes is a empty gragh \n";
			}
		}
		else if (Edges[0] == 'E') //---------------------------if E was the comand ---------------------------
		{
			input.clear();
			string node;
			// counting number of pairs of Edges for future manioulation
			for (int i = 0; i <= size1; i++)
			{
				if (Edges[i] == '>')
				{
					row++;
				}
			}
			//cout << "number of rows " << row << endl;
			rows = row;
			// itterate to find the nodes in the list
			for (int t = 0; t <= size1; t++)
			{
				//cout << " Edges index" << t << " = " << Edges[t] << endl;
				if (isdigit(Edges[t]))
				{
					node += Edges[t];
					if (isdigit(Edges[t + 1]))
					{
						node += Edges[t + 1];
						t++;
					}
					input.push_back(stoi(node));
				}
				node.clear();
			}
			size2 = input.size();
			for (int i = 0; i < size2; i++)
			{
				//   cout << "vecotr " << input[i] << endl;
			}
			// VALIDATION THAT ALL NODES IN EDGES ARE LEGAL
			for (int i = 0; i < size2; i++) {

				int comparator = input[i];
				if (comparator >= fvertix) {

					//cout << " highest number of node = " << fvertix << endl;
					std::cerr << "Error: Invalid Edges" << endl;
					input.clear();
					compare = true;
					break;
				}
				else
					compare = false;
			}
//-------------------------------------------------------------------------- Creating Processes --------------------------------------------------------------
			if (!compare ) {
				// Ass4(fvertix); // vertix cover 
				pthread_t cnfred;
			    pthread_t approx1;
		     	pthread_t approx2;
               // clockid_t ids;
                //int s; 
				 //pthread_t approx2;
				pthread_create(&cnfred, NULL,vertexcover1, NULL);
				pthread_create(&approx1, NULL, approxvc1, NULL);
				pthread_create(&approx2, NULL, approxvc2, NULL);

				sleep(10);
				//cout<<"interrupt"<<endl;
				solver->interrupt();
				pthread_join(cnfred, NULL);
				//sleep(1);
				
				pthread_join(approx1, NULL);
				//sleep(1);
				
				pthread_join(approx2, NULL);   
			//	sleep(1);
			//	 cout << "vertext time get : " << vf << endl;
	  		//	cout << "vertext 1 time get : " << vc1f << endl;
	   		//	cout << "vertext 2 time get : " << vc2f << endl;
                //--------------------------------------- 
				if (input.empty())
				{
					cout << "CNF-SAT-VC: " << endl;
					cout << "APPROX-VC-1: " << endl;
					cout << "APPROX-VC-2: " << endl;
					
				}
				else 
				{
					if(!vertexcoverresult.empty())
					{
						cout << "CNF-SAT-VC: ";
						for (int i = 0; i <  vertexcoverresult.size(); i++) {
							std::cout << vertexcoverresult[i];
							if(i != vertexcoverresult.size() - 1) {
								std::cout << ",";
        }
					}

					}
					else{
						cout<<"CNF-SAT-VC: Timeout";
					}
				cout << endl;
				cout << "APPROX-VC-1: ";
				for (int i = 0; i < approxvc1result.size(); i++) {
				    std::cout << approxvc1result[i] ;
					if (i != approxvc1result.size() - 1) 
						cout << ",";
				}
    			approxvc1result.clear();
				cout << endl;
				cout << "APPROX-VC-2: ";
				for (int i = 0; i < approxvc2result.size(); i++) {
				    std::cout << approxvc2result[i] ;
					if (i != approxvc2result.size() - 1) 
						cout << ",";
				}
   				 approxvc2result.clear();
				cout << endl;
			}
			}
			else
				continue;
		}
	}
	return 0;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ Assigment 4 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++==

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ VERTEX COVER  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void* vertexcover1(void* arg) {
	//cout <<" yeah I hope to start my vertext cover here " << endl; 
	//cout<<fvertix<<endl;
	struct timespec begin , end; 
	clock_gettime(CLOCK_MONOTONIC, &begin);
	Minisat::lbool checker;
	vertexcoverresult.clear();
	for(int k=1;k<fvertix;k++)
	{
		solver.reset (new Minisat::Solver());
		std::vector<std::vector<Minisat::Lit> > literals(fvertix, vector<Minisat::Lit>(k));
		//cout<<"k="<<k<<endl;
		for (int i = 0; i < fvertix; i++) {
			for (int j = 0; j < k; j++) {
				literals[i][j] = Minisat::mkLit(solver->newVar());
			}
		}
		//cout << " Initiation went well " << endl; 
		//***** clause 1 -----------------------------------------------------------------------clause 1 ----------------------------------------------------------------------
		for (int i = 0; i < k; i++) {
			//   (x1;i V x2;i V...V xn;i)
			Minisat::vec<Minisat::Lit> c1;
			for (int j = 0; j < fvertix; j++) {
				c1.push(literals[j][i]);
			}
			solver->addClause_(c1);
		}
		//cout << " Clause 1 is fine " << endl;
		//***** clause2 ------------------------------------------------------------------------clause 2---------------------------------------
		for (int i = 0; i < fvertix; i++) {
			for (int j = 0; j < k; j++) {
				for (int l = j + 1; l < k; l++) {
					solver->addClause(~literals[i][j], ~literals[i][l]);
				}
			}
		}
		//cout << " Clause 2 is fine " << endl;
		//***** clause3 ------------------------------------------------------------------------clause 3 ---------------------------------------
		for (int i = 0; i < k; i++) {
			for (int j = 0; j < fvertix; j++) {
				for (int l = j + 1; l < fvertix; l++) {
					solver->addClause(~literals[j][i], ~literals[l][i]);
				}
			}
		}
		//cout << " Clause 3 is fine " << endl;
		//Clause 4 ----------------------------------------------------------------------------clause 4 -----------------------------------------------
		for (int j = 0; j < input.size(); j = j + 2) {
			Minisat::vec<Minisat::Lit> c4;
			for (int l = 0; l < k; l++) {
				c4.push(literals[input[j]][l]);
				c4.push(literals[input[j + 1]][l]);
				//cout << " j " << j << " l " << l << endl;
			}
			solver->addClause(c4);
		}
		// cout << " Clause 4 is fine " << endl;
	//----------------------------------------------------------------------------------------final print---------------------------------------------------------
		Minisat::vec<Minisat::Lit> dummy;
		checker = solver->solveLimited(dummy);
		if (checker == Minisat::l_True)
		{
			for (int i = 0; i < fvertix; i++) {
				for (int j = 0; j < k; j++) {
					if (Minisat::toInt(solver->modelValue(literals[i][j])) == 0) {
						vertexcoverresult.push_back(i);
						break;
					}
				}
			}
			//cout<<"l_TRUE"<<endl;
			break;
			//vertexcoverresult = result;

		}
		else if(checker == Minisat::l_Undef){
			//cout<<"l_Undef"<<endl;
			return NULL;
		}
	}
        if (checker == Minisat::l_True) {
            // Sort for output
            sort(vertexcoverresult.begin(), vertexcoverresult.end());
        }
		clock_gettime(CLOCK_MONOTONIC, &end); 
		vf =  end.tv_sec - begin.tv_sec; 
		vf += (end.tv_nsec - begin.tv_nsec)/1000000000;
		return NULL;

}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ Aprox VC1 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void * approxvc1(void * arg){
	struct timespec begin, end; 
	clock_gettime(CLOCK_MONOTONIC, &begin);
	vector <int> aproxholder; 
	vector <int> returner; 
	aproxholder = input; 
	int degree; 
	int node;
	while (!aproxholder.empty())
	{
		degree = 0; 
		for (int i = 0; i < fvertix; i++ )
		{
			int degv = getdegree(i, aproxholder);
			if (degv > degree) {
				degree  = degv; 
				node = i; 
			}
		}
		returner.push_back(node);
		aproxholder = remove(node, aproxholder);
	}
	// Should I sort ? 
	sort(returner.begin(),returner.end(), std::less<int>());
	approxvc1result = returner; 
	clock_gettime(CLOCK_MONOTONIC, &end); 
	vc1f = end.tv_sec -begin.tv_sec; 
	//cout << "vertext time vc1 test 1 get : " << vc1f << endl;
	vc1f += (end.tv_nsec -begin.tv_nsec)/1000000000;
	//cout << "vertext time test 2 vc1 get : " << vc1f << endl;
	/*
	  cout << "vertext time vc1 get : " << vc1f << endl;
//---------------------------------------------------------------------
	 s = pthread_getcpuclockid(pthread_self(), &cidvc1);
           if (s != 0)
               handle_error_en(s, "pthread_getcpuclockid");
         timerx =  pclock(cidvc1);
		    cout <<" Approx VC1 time: " << timerx << endl;
//---------------------------------------------------------------------------*/
	return 0;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ GETTING HIGH DEGREE +++++++++++++++++++++++++++++++
int getdegree(int node, vector<int> edgex){
    int value = 0;
    for(int i = 0; i < edgex.size(); i++){
	//int k = input[i];
        if(edgex[i] == node){
            value++;
        }
    }
    return value;
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ removing degree +++++++++++++++++++++++++++++++++++++++++++
vector<int> remove(int node, vector<int> edgex){
		vector<int>result;
		for(size_t i=0;i<edgex.size();i+=2){
			//int first=edgex[i],second=edgex[i+1];
			if(edgex[i] != node && edgex[i+1] != node){
				result.push_back(edgex[i]);
				result.push_back(edgex[i+1]);
			}
		}
		//cout<<result.size()<<endl;
		return result;
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ Approximation vc 2 ++++++++++++++++++++++++++++++++++++++++++++++++++
void* approxvc2(void* arg){
	struct timespec begin, end; 
	clock_gettime(CLOCK_MONOTONIC, &begin);
	vector<int> result;
    vector<int> edgex = input;
	while (!edgex.empty()){
		result.push_back(edgex[0]);
        result.push_back(edgex[1]);
		int whatevs = edgex[1];
        edgex = remove(edgex[0], edgex);
        edgex = remove(whatevs, edgex);
	}
	sort(result.begin(),result.end(), std::less<int>());
	approxvc2result = result;
	clock_gettime(CLOCK_MONOTONIC, &end);
	vc2f = end.tv_sec - begin.tv_sec; 
	vc2f += (end.tv_nsec - begin.tv_nsec)/1000000000;/*
	  cout << "vertext time vc2 get : " << vc2f << endl;
	//---------------------------------------------------------------------
	 s = pthread_getcpuclockid(pthread_self(), &cidvc2);
           if (s != 0)
               handle_error_en(s, "pthread_getcpuclockid");
    timerx =  pclock(cidvc2);
	  cout << " Approx VC2 time: "  << timerx << endl;
//---------------------------------------------------------------------------*/
    return 0;
}