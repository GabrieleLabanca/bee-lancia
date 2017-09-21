#include <fstream>
#include <iostream>
#include <vector>

using namespace std;


int headlines, lines;

int count_lines(char * file)
{
  headlines = 0;
  lines = -1;
  string line;
  ifstream myfile(file);
  while (getline(myfile, line)){
    if(line[0]!='#')
      ++lines;
    else if(line[0]=='#')
      ++headlines;
  }
}



int main(int argc, char** argv)
{
  char* in_file = argv[1];
  count_lines(in_file);
  
  float * x = new float[lines];
  float * y = new float[lines];

  ifstream myfile(in_file);
  string temp;

  for(int i=0; i<headlines; i++){
    getline(myfile, temp);
  }
  for(int i=0; i<lines; i++){
    x[i] = i; 
    myfile >> y[i];
  }

  char* out_file = argv[2];
  ofstream ofile(out_file);
  for(int i=0; i<lines; ++i){
    ofile << x[i] << " " << y[i] << endl;
  }

  

















  return 0;
}



