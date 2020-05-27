#include<vector>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include<iostream>
using namespace std;

int pipeRgentoA1_status,pipeA1toA2_status;
int pipeRgentoA1[2];
int pipeA1toA2[2];

pid_t child_rgen,child_rgen_status;
int pid_rgen_status,pid_a1_status;

pid_t child_a1,child_a1_status;

pid_t child_a2,child_a2_status;

std::vector<pid_t> kids;

void spawn_rgen(char** argv1)
{
	//cout<<"child: Executing rgen!\n"; //Debug_Point

	// output to pipeRgentoA1
	dup2(pipeRgentoA1[1],STDOUT_FILENO);
	close(pipeRgentoA1[0]);
	close(pipeRgentoA1[1]);
	execvp("./rgen",argv1);
	cerr<<"Error:Exec of rgen!\n";
	exit(1);
}

void spawn_a1()
{
	
	//cout<<"child : Executing a1!\n"; //Debug_Point

	// input from pipeRgentoA1
	dup2(pipeRgentoA1[0],STDIN_FILENO);

	// output to pipeA1toA2
	dup2(pipeA1toA2[1],STDOUT_FILENO);

	// close fds
	close(pipeRgentoA1[0]);
	close(pipeRgentoA1[1]);
	close(pipeA1toA2[0]);
	close(pipeA1toA2[1]);

	//exec
	char* pyArg[3];
	pyArg[0]=(char*)"/usr/bin/python2";
	pyArg[1]=(char*)"ece650-a1.py";
	pyArg[2]= nullptr;
	execv("/usr/bin/python2",pyArg);
	cerr<<"Error:Exec of a1!\n";
	exit(1);
}

void spawn_a2()
{
	//cout<<"child : Executing a2!\n"; //Debug_Point

	//input from pipeA1toA2
	dup2(pipeA1toA2[0],STDIN_FILENO);

	//output to stdout

	//close fds
	close(pipeA1toA2[0]);
	close(pipeA1toA2[1]);

	//exec
	execvp("./ece650-a2",nullptr);

	cerr<<"Error: Exec of a2!\n";
	exit(1);
}


int main(int argc, char** argv){

// Create pipeRgentoA1
	pipeRgentoA1_status=pipe(pipeRgentoA1);

	if (pipeRgentoA1_status == -1)
	{
	  cerr<<"Error:creating the pipeRgentoA1"<<endl;
	  exit(1);
	}

//=================PIPE Rgen to A1 CREATED ========================================

// Forking (child_rgen)
	//cout<<"Parent: giving birth to child_rgen!\n"; //Debug_Point	
  	if ((child_rgen= fork()) == -1)
  	{
    cerr<<"Error: bad child_rgen forking!"<<endl;
    exit(1); 		
  	}
  	else if(child_rgen ==0)
  	{
  		//Inside Child Rgen
  		// rgen --> pipeRgentoA1
  		spawn_rgen(argv);
    }

// =================== pipeRgentoA1 CONTAINS rgen OUTPUT ===========================

// parent
	kids.push_back(child_rgen);
  	//cout<<"Parent: Back to driver with successful spawn_rgen!\n"; //DEBUG_POINT

// create pipeA1toA2
  	pipeA1toA2_status=pipe(pipeA1toA2);

  	if(pipeA1toA2_status == -1)
  	{
	  cerr<<"Error:creating the pipeA1toA2"<<endl;
	  exit(1);  		
  	}

//=================PIPE A1 to A2 CREATED ========================================


// forking (child_a1)
  	if((child_a1=fork())== -1)
  	{
	  cerr<<"Error: bad child_a1 forking!"<<endl;
	  exit(1);		
  	}
  	else if(child_a1 ==0)
  	{
  		// pipeRgentoA1 --> a1 --> pipeA1toA2
  		spawn_a1();
  	}

// =================== pipeA1toA2 CONTAINS A1 OUTPUT ===========================

// parent
	kids.push_back(child_a1);

// close unused fds
	close(pipeRgentoA1[0]);
	close(pipeRgentoA1[1]);
  	//cout<<"Back to driver with successful spawn_a1!\n";

// forking (child_a2)
  	if((child_a2=fork())== -1)
  	{
	  cerr<<"Error: bad child_a2 forking!"<<endl;
	  exit(1);		
  	}
  	else if(child_a2 ==0)
  	{
  		// pipeA1toA2 --> a2 --> stdout
  		spawn_a2();
  	}

//

//  console to pipeA1toA2 to A2
  	dup2(pipeA1toA2[1],STDOUT_FILENO);
  	close(pipeA1toA2[0]);
  	close(pipeA1toA2[1]);

    while (!std::cin.eof()) {
        // read a line of input until EOL and store in a string
        std::string line;
        std::getline(std::cin, line);
        if (line.size () > 0)
            std::cout << line << std::endl;
    }

    kids.push_back(child_a2);
  	//parent 

    // send kill signal to all children
    for (pid_t k : kids) {
        int status;
        kill (k, SIGTERM);
        waitpid(k, &status, 0);
    }

    return 0;
}