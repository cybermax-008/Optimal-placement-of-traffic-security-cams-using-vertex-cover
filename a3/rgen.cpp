#include <iostream>
#include <fstream>
#include <unistd.h>
#include <vector>
#include <cmath>
using namespace std;

int s_min=2, s_max=10;
int n_min=1, n_max=5;
int l_min=5, l_max=5;
int c_min=-20, c_max=20;    
//Function that parses cmd line arguments to 
//retrive -s,-n,-l,-c values 

int randomGen(int min, int max)
{
    std::ifstream urandom("/dev/urandom");

    if (urandom.fail()) {
        std::cerr << "Error: cannot open /dev/urandom\n";
        return 1;
    }
    int N = (max - min)+1 ;
    int num=0;     
    while(1)
    {
        
        urandom.read((char*)&num,1);
        //cout<<num<<endl;
        num= num % N + min;
        //cout<<"RandGen: "<<num<<endl;
        if(num <= max && num >= min)
        {
            urandom.close();
            break;
        }
    }

    return num;
}


class Point
{
private:
        int xval, yval;

public:

    Point(int x = 0, int y = 0)
    {
        xval=x;
        yval=y;
    }
    int getX(){ return xval;} //get x value
    int getY(){ return yval;} //get y value

    //dist to another point
    int dist(Point other) 
    {
        int xd = xval - other.xval;
        int yd = yval - other.yval;
        return sqrt(xd*xd + yd*yd);
    }



};

bool equality(Point a,Point b)
{
    int a_x=a.getX();
    int b_x=b.getX();
    int a_y=a.getY();
    int b_y=b.getY();
    if ( a_x == b_x && a_y == b_y)
    {
        return true;
    }
    else
    {
        return false;
    }
} 

bool onSegment(Point p, Point q, Point r) 
{   
    if (q.getX() <= max(p.getX(), r.getX()) && q.getX() >= min(p.getX(), r.getX()) && 
        q.getY() <= max(p.getY(), r.getY()) && q.getY() >= min(p.getY(), r.getY())) 
       return true; 
  
    return false; 
}

bool isIntersecting(Point& p1, Point& p2, Point& q1, Point& q2) {
    return (((q1.getX()-p1.getX())*(p2.getY()-p1.getY()) - (q1.getY()-p1.getY())*(p2.getX()-p1.getX())) 
            * ((q2.getX()-p1.getX())*(p2.getY()-p1.getY()) - (q2.getY()-p1.getY())*(p2.getX()-p1.getX())) < 0)
            &&
           (((p1.getX()-q1.getX())*(q2.getY()-q1.getY()) - (p1.getY()-q1.getY())*(q2.getX()-q1.getX()))
            * ((p2.getX()-q1.getX())*(q2.getY()-q1.getY()) - (p2.getY()-q1.getY())*(q2.getX()-q1.getX())) < 0);
}  


class Line
{
public:
    Line();
    Line(Point src, Point dst);
    Point getScr(){ return p1;}
    Point getDst(){ return p2;}
private:
    Point p1, p2;
};

Line::Line(Point src, Point dst)
{
    p1= src;
    p2= dst;

}



class street
{
private:
    int st_max;
    int st_min;
    int stCount;
    vector<string> st_names;
    int numOfLines;
    int numOfcoordinates;
    int coordinatesRange;
    string coordinates_str;
    vector<vector<Point>> cod_vector;
    vector<vector<Line>> line_vector;
    vector<string> coordinates;
public:
    street(int min,int max);
    vector<string> streetNameGen();
    vector<string> coordinatesGen();
    vector<vector<Line>> lineGen();
    int getStreetCount(){ return stCount;}

};

street::street(int min,int max)
{
    st_min=min;
    st_max=max;
    stCount=randomGen(st_min,st_max);
}

vector<string> street::streetNameGen()
{
    string st_name;
    int count=stCount;
    for(char i='A';i<='Z';i++)
    {
        st_name="\"Street ";
        string s(1,i);
        st_name.append(s);
        st_name += "\" ";
        st_names.push_back(st_name);
        count--;
        if(count<= 0)
        {
            break;
        }
    }
    return st_names;
}

vector<string> street::coordinatesGen()
{
    vector<Point> cod;
    vector<Line> line_seg;
    int maxlimitloop;
    //cout<<stCount<<endl;
    int flag_intersec=0;
    for (int i=0; i<stCount; i++)     
    {
        cod.clear();
        maxlimitloop=0;
        coordinates_str="";
        numOfLines=randomGen(n_min,n_max);
        numOfcoordinates=numOfLines + 1;
        for(int j=0;j<numOfcoordinates;j++)
        {
            int c_x=randomGen(c_min,c_max);
            int c_y=randomGen(c_min,c_max);
            //cout<<c_x<<","<<c_y<<endl;
            Point p(c_x,c_y);
            if(cod.size()<1)
            {
                cod.push_back(p);
                continue;
            }
            else
            {
                int flag_self=0;
                for( int ii=0;ii<cod.size();ii++)
                {
                    if (onSegment(cod[ii],cod[ii+1],p) || isIntersecting(cod[ii],cod[ii+1],cod[cod.size()-1],p))
                    {
                        flag_self++;
                        maxlimitloop++;
                        //cout<<maxlimitloop<<endl;
                        break;
                    }
                }
                int flag_overlap=0;
                for (int d=0;d<line_vector.size();d++)
                {
                    vector<Line> temp_line=line_vector[d];
                    for(int dd=0;dd<temp_line.size();dd++)
                    {
                        //cout<<"inside overlap\n";
                        Point new_point=p;
                        Point old_point=cod[cod.size()-1];
                        Point l_p1=temp_line[dd].getScr();
                        Point l_p2=temp_line[dd].getDst();
                        //cout<<"l_p1:("<<l_p1.getX()<<","<<l_p1.getY()<<") ; l_p2:("<<l_p2.getX()<<","<<l_p2.getY()<<") || "<<"old_point:("<<old_point.getX()<<","<<old_point.getY()<<") ; new_point:("<<new_point.getX()<<","<<new_point.getY()<<")";
                        bool flag1=equality(old_point,l_p1);
                        bool flag2=equality(new_point,l_p2);
                        //cout<<flag1<<" "<<dd<<" " <<flag2<<endl;
                        if (flag1 == true && flag2 == true )
                        {
                            //cerr<<"l_p1:("<<l_p1.getX()<<","<<l_p1.getY()<<") ; l_p2:("<<l_p2.getX()<<","<<l_p2.getY()<<") || "<<"old_point:("<<old_point.getX()<<","<<old_point.getY()<<") ; new_point:("<<new_point.getX()<<","<<new_point.getY()<<")\n";
                            flag_overlap++;

                        }
                        if(isIntersecting(l_p1,l_p2,old_point,new_point))
                        {
                            flag_intersec++;
                        }
                    }
                }
                if (flag_self !=0 || flag_overlap !=0)
                {
                    j--;
                    if(maxlimitloop==40)
                {
                    i--;
                    cerr<<"Error: failed to generate valid input for "<<to_string(maxlimitloop)<<" simultaneous attempts\n";
                    exit(EXIT_FAILURE);
                }
                    continue;
                }
                else
                {
                    cod.push_back(p);
                    continue;
                }
            }

        }

        if(cod.size()==numOfcoordinates )
        {
            //cout<<"st:"<<to_string(i+1)<<", "<<"n:"<<numOfcoordinates<<"= ";
            for (int ii=0;ii<cod.size();ii++)
            {
                coordinates_str+="("+to_string(cod[ii].getX())+","+to_string(cod[ii].getY())+") ";
                //cout<<"("<<cod[ii].getX()<<","<<cod[ii].getY()<<"),";
            }
            //cout<<endl;
            //cout<<"====================================================================\n";
            coordinates.push_back(coordinates_str);
            cod_vector.push_back(cod);         
        }
        vector<Line> st_line;
        st_line.clear();
        for(int k=0;k<cod.size();k++)
        {
            Line lseg(cod[k],cod[k+1]);
            st_line.push_back(lseg);
        }
        line_vector.push_back(st_line);    
    }
    if(flag_intersec > 1)
    {   
        //cerr<<flag_intersec<<endl;
        return coordinates;
    }
    else
    {
        //cerr<<"here\n";
        coordinates.clear();
        return coordinates;
    }
}

 

int parseArg(int argc, char **argv, int* s_max, int* n_max, int* l_max, int* c_max,int* c_min)
{
    int opt;
    int sflag=0;
    int nflag=0;
    int lflag=0;
    int cflag=0;

    char* svalue;
    char* nvalue;
    char* lvalue;
    char* cvalue;

    opterr = 0;

    while ((opt=getopt(argc,argv,"s:n:l:c:")) != -1)
    {
        switch(opt)
    
    {
        case 's':
            sflag =1;
            svalue=optarg;
            *s_max=atoi(svalue);
            break;
        case 'n':
            nflag =1;
            nvalue=optarg;
            *n_max=atoi(nvalue);
            break;
        case 'l':
            lflag =1;
            lvalue=optarg;
            *l_max=atoi(lvalue);
            break;
        case 'c':
            cflag =1;
            cvalue=optarg;
            *c_min=-atoi(cvalue);
            *c_max=atoi(cvalue);
            break;
        case ':':
            break;
        case '?':
            cerr<<"Error:Unknown option provided\n";
            return 1;
        default:
            abort();
    }
}
    //cout<<s_max<<", "<<n_max<<", "<<l_max<<", "<<c_max<<endl; //DEBUG_POINT
}



int main(int argc, char **argv)
{
    int s,l;
    parseArg(argc,argv,&s_max,&n_max,&l_max,&c_max,&c_min);
    vector<string> street_names;
    vector<string> coo;

   while(1)
   {
    street st(s_min,s_max); // start street instance with s count
    // remove the streets in the street db of python process
    if(street_names.size()>0)
    {
        for (int j=0; j<street_names.size(); j++)
        {
           cout<<"r "<<street_names[j]<< "\n";  
        }
    }
    // empty thr street names vector 
    street_names.clear();
    street_names=st.streetNameGen();
    l=randomGen(l_min,l_max);
    //Debug_Point :: Check the values of s,n,l,c generated.
    //cout<<"s="<<s<<"; n="<<n<<"; l="<<l<<endl;

    //Debug_Point :: check the street_names generated.
    coo=st.coordinatesGen();
    // for (int i=0; i<street_names.size(); i++)     
    // {
    //     cout << street_names[i] << "\n";        
    // }
    //coo.push_back("g");

    //Debug_Point :: check the street_names generated.
    if(!coo.empty())
    {
	    for (int i=0; i<coo.size(); i++)     
	    {
	        cout <<"a "<<street_names[i]<<coo[i] << "\n";        
	    }
	    cout<<"g"<<endl;
	}
	else
	{
		cerr<<"Error:No valid graph generated- Zero Vertices produced\n";
		continue;
	}

    sleep(l);
   } 


    return 0;
}