#include <fstream>
#include <iostream>
#include <vector>

using namespace std;


int headlines, lines, columns;

void count_lines(char * file)
{
  headlines = 0;
  lines = 0;
  string line;
  ifstream myfile(file);
  while (getline(myfile, line)){
    if(line[0]!='#')
      ++lines;
    else if(line[0]=='#')
      ++headlines;
  }
}

void count_columns(char* file)
{
  columns = 1;
  ifstream myfile(file);
  string line;
  for(int i=0; i<headlines+1; ++i){ // "+1" to get the first line after head
    getline(myfile,line);
  }
  int temp = 0;
  while( (temp=line.find(' ',temp+1)) != string::npos ){
    ++columns;  
  }
}



int main(int argc, char** argv)
{
  char* in_file = argv[1];
  count_lines(in_file);
  cerr << lines << endl;
  count_columns(in_file);
  
  float * x = new float[lines];
  vector< vector<float> > y;
  y.resize(columns);
  for(int i=0; i<columns; ++i){
    y.at(i).resize(lines);
  }

  ifstream myfile(in_file);
  string temp;

  for(int i=0; i<headlines; i++){
    getline(myfile, temp);
  }
  for(int i=0; i<lines; i++){
    x[i] = i; 
    for(int j=0; j<columns; ++j){
      myfile >> y.at(j).at(i);
    }
  }

  char* out_file = argv[2];
  ofstream ofile(out_file);
  for(int i=0; i<lines; ++i){
    cerr << x[i] << " " ;
    for(int j=0; j<columns; ++j){
      cerr << y.at(j).at(i) << " " ;
    } 
    cerr << endl;
  }

  

















  return 0;
}



