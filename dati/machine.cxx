#include <fstream> 
#include <string>
#include <iostream>


class datafile
{
  public:
    datafile(string,float);

    void fill_mean(int);

    int get_n_lines() { return lines; } 
    int get_n_elab() { return n_elab; }


    void plot_data();
    void plot_elab();

  private:
    const char * filename;
    int headlines, lines;
    // actual data
    float * x;
    float * y;
    // elaboration
    int n_elab;
    float * x_elab;
    float * y_elab;

    void count_lines();
    void fill_data(float);

};


datafile::datafile(string filestring, float unit):
  filename(filestring.c_str())
{
  count_lines();
  x = new float[lines];
  y = new float[lines];
  fill_data(unit); 
}

void datafile::count_lines()
{
  using namespace std;
  headlines = 0;
  lines = -1;
  string line;
  ifstream myfile(filename);

  while (getline(myfile, line)){
    if(line[0]!='#')
      ++lines;
    else if(line[0]=='#')
      ++headlines;
  }
}

void datafile::fill_data(float unit)
{
  using namespace std;
  ifstream myfile(filename);
  string temp;

  for (int i=0; i< headlines; i++){
    getline(myfile, temp);
  }
  for (int i =0; i<lines; i++){
    x[i] = i*unit; // rescale x
    myfile >> y[i];
    //std::cerr << x[i] << "   " << y[i] << endl;
  }
}

void datafile::plot_data()
{
  TGraph * plot = new TGraph (lines, x, y);	
  plot -> Draw();
}

void datafile::plot_elab()
{
  TGraph * plot = new TGraph (n_elab, x_elab, y_elab);	
  plot -> Draw();
}


void datafile::fill_mean(int N)
{
  using namespace std;
  int npoints = float(lines/N);
  n_elab = npoints;
//  cerr << "npoints " << npoints << endl;
  x_elab = new float[npoints];
  y_elab = new float[npoints];
  float tsum = 0;
  int ireal = 0; //counts from 0 to lines
  for(int index=0; index<npoints; index++){ //counts the points in _elab
    for(int i=0; i<N; i++){ //counts the points over which mean is taken
    //  cerr << "y[i] " << y[ireal] << endl;
      tsum += y[ireal];
      ++ireal;
    }
   // cerr << "TSUM " << tsum << endl;
    x_elab[index] = N*(index+1./2);
    y_elab[index] = tsum/int(N);
    //cerr << x_elab[index] << " " << y_elab[index] << endl;
    tsum = 0;
  }
}








void plot_file(string NAME, float UNIT=1)
{
  datafile mydata(NAME,UNIT);
  mydata.plot_data();
}

void plot_mean(string NAME, float UNIT=1, int NMEAN=5)
{
  datafile mydata(NAME,UNIT);
  mydata.fill_mean(NMEAN);
  mydata.plot_elab();
}













//__________________________//

//OLD plot.cxx
/*
   int * n_lines(const char * name) { 
   int number_of_header = 0;
   int number_of_lines = -1;
   std::string line;
   std::ifstream myfile(name);

   while (std::getline(myfile, line))
   if(line[0]!='#')
   ++number_of_lines;
   else if(line[0]=='#')
   ++number_of_header;
   int * numbers = new int[2];
   numbers[0] = number_of_header;
   numbers[1] = number_of_lines;
   return numbers;
   }

   float** plot(string filestring, float unit=1){

   const char * filename = filestring.c_str();
   int header = n_lines(filename)[0];
   int lines  = n_lines(filename)[1];
   std::cerr << header << "  " << lines << endl;

   std::fstream file (filename, std::fstream::in);

   std::string temp;
   float * x = new float [lines];
   float * y = new float [lines];

   for (int i=0; i< header; i++){
   std::getline(file, temp);
//std::cerr << temp << endl;
}
for (int i =0; i<lines; i++){
x[i] = i*unit; // rescale x
file >> y[i];
std::cerr << x[i] << "   " << y[i] << endl;
}

TGraph * plot = new TGraph (lines, x, y);	
plot -> Draw();

float** data_vect = new float*[2];
data_vect[0] = x;
data_vect[1] = y;

return data_vect;

}

*/
