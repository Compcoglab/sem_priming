// prototype of the dbase read

#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

struct Narrow               // container for user requests
{
    list<string> requests;  // parameters to refine searches by
    list<string>  words;    // words user wants to search for
};

// local functions
struct Narrow initCriteria();		    // Reads in user search words && requests

vector< vector<string> > initDbase();   // Builds original database from CSV file

void printMatrix(vector< vector<string> > m);

// uses info in struct n to fill matrix &m with words from matrix d
void fillMat(struct Narrow n, vector< vector<string> > d, vector< vector<string> > &m);

//=========================================================
//=========================================================
int main()
{
    Narrow criteria;			        // user requests
    vector< vector<string> > dbase;	    // database
    vector< vector<string> > usemat;	// matrix to fill with the user searches

    // initialize the database
    dbase = initDbase();
    
    // initialize user chosen criteria for narrowing searches
    criteria = initCriteria();
    
    // at this point dbase represents whole database
    // criteria is filled with words, and narrowing parameters
    
    // fill usemat with initial search values
    fillMat(criteria, dbase, usemat);
    
    
    for(unsigned i = 0; i < dbase[0].size(); i++)  // print column names
    {
        cout << dbase[0][i] << ", ";
    }
    cout << endl;
  
    printMatrix(usemat);
    
   
    return 0;
}
//==========================================================
//==========================================================


vector< vector<string> > initDbase()
{
    string targetFile = "spp_excel.csv"; // file to read in 
    string temp;
    ifstream infs;
    vector<string> vec;
    vector< vector<string> > mat;
    
    // try to open target for read
    infs.open(targetFile.c_str());
    if(!infs.is_open())
    {
        cout << "ERROR: could not open " << targetFile << endl;
        exit(1);
    }
    
    // read in lines from file one at a time
    while( getline(infs, temp) )
    {
        // push back each string in CSV List
        while(!temp.empty())
        {
            string chunk;
            size_t pos;
            pos = temp.find(',');
            
            if( (pos != string::npos) && (pos < temp.size()) )
            {
                chunk = temp.substr(0, pos);    // before ','
                vec.push_back(chunk); 
                temp = temp.substr(pos+1);     // after ','
                
                continue;        // keep breaking up CSV List
            }
            vec.push_back(temp); // last word
            
            break; // go to get next whole line from file
        }
        
        mat.push_back(vec);     // add row to matrix
        vec.clear();            // clear vec for next read
    }
    infs.close(); // done with read
    
    return mat;
}
//----------------------------------------------------------------

struct Narrow initCriteria()
{
    string targetFile = "user_args.txt"; // file to read in 
    string temp;
    ifstream infs;
    Narrow box;     // queue && list
    
    // try to open target for read
    infs.open(targetFile.c_str());
    if(!infs.is_open())
    {
        cout << "ERROR: could not open " << targetFile << endl;
        exit(1);
    }
    
    // here parse user_args.txt and fill queue && list  
    
    // read in first line from file
    getline(infs, temp);
    while(!temp.empty())  // push back each string in CSV List
    {
        string chunk;
        size_t pos;
        pos = temp.find(',');
        
        if( (pos != string::npos) && (pos < temp.size()) )
        {
            chunk = temp.substr(0, pos);    // before ','
            box.requests.push_back(chunk); 
            temp = temp.substr(pos+1);     // after ','
            
            continue;   // keep breaking up CSV List
        }
        
        box.requests.push_back(temp);
        
        break; // get next whole line from file
    }

    temp.clear(); // reset temp
       
    // read rest of file 
    while(getline(infs,temp))
    {
        box.words.push_back(temp);
    }

    infs.close(); // done with read
    
    return box;
}
//--------------------------------------------------------


void printMatrix(vector< vector<string> > m)
{
    if(m.empty())
    {
	    cout << "EMPTY" << endl; return;
    }
  
    for(unsigned i = 0; i < m.size(); i++)		// iterate down rows
    {
    	for(unsigned j = 0; j < (m[i]).size(); j++) // print row content
    	{
    	    if(m[i][j].empty())
    	    {
    		    cout << "N/A" << ", ";
    	    }
    	    else
    	    {
    		    cout << m[i][j] << ", ";
    	    }  
    	}
	    cout << endl;
    }
}


// binary search version  O(log(n))
void fillMat(struct Narrow n, vector< vector<string> > d, vector< vector<string> > &m)
{
    if( n.requests.empty() || n.words.empty() || d.empty() )
    {
	    cout << "ERROR: fillMat() parameter was empty" << endl;
	    return;
    }
        
    // d[i][0] == Target word collumn
    // n.words == words to search for in d
    while( !n.words.empty() )
    {
    	string temp = n.words.front();  // word to look for
    	
    	unsigned left = 0;
    	unsigned right = d.size()-1;
    	
    	while(left <= right)
    	{
    	    unsigned mid = (left + right)/2;
    	    
    	    if(temp == d[mid][0])  // found the word
    	    {
    		    m.push_back(d[mid]);
    		    n.words.pop_front();
    		    break;
    	    }
    	    else if(temp < d[mid][0]) // search in lower half
    	    {
    		    right = mid - 1;
    	    }
    	    else			        // search in upper half
    	    {
    		    left = mid + 1;
    	    }
    	}
    	
    	if(!n.words.front().empty() && temp == n.words.front())  // did not find word
    	{
    	    cout << "ERROR: word \"" << temp << "\" not found" << endl;
    	    n.words.pop_front(); // remove not found word from list
    	}
    }
}
//-----------------------------------------------------------------------------





