#include <fstream> 
#include <string>
#include <iostream>
#include <cmath>

class datafile
{
  public:
    datafile(string,float);

    // get parameters
    int get_n_lines() { return lines; } 
    int get_n_elab() { return n_elab; }
    float get_x(int i) { return x[i]; }
    float get_y(int i) { return y[i]; }
    float get_xel(int i) { return x_elab[i]; }
    float get_yel(int i) { return y_elab[i]; }
    float get_u() { return unit; }

    // mean
    void fill_mean(int);
    void clean_quad();

    // plot 
    void plot_data();
    void plot_elab();
    // graphs
    TGraph * gr_raw;
    TGraph * gr_elab;


    //private:
    float unit;
    // actual data
    float * x;
    float * y;
    // elaboration
    int n_elab;
    float * x_elab;
    float * y_elab;

    const char * filename;
    int headlines, lines;
    void count_lines();
    void fill_data(float);

    // iterators: start, border, explorer
    int is, ib, ie;
    void explore(TF1*, float, int);

    //float a, b, c;
    TF1 * quad = new TF1("quad","[2]*x*x+[1]*x+[0]",0,10);
    //  void quadratic_fit(float*, float*, int, int, float&, float&, float&);
};


datafile::datafile(string filestring, float myunit):
  filename(filestring.c_str()),unit(myunit)
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

void datafile::explore(TF1 * f, float sigma, int howfar=5)
{
  // goes on until for 'howfar' times sigma is exceeded
  int count = 0;
  float error = 0;
  while( count<howfar ){
    ++ie;
    //cerr << "f = " <<  (*f)(x[ie]) << " y[ie] " << y[ie] << endl;
    error = abs( (*f)(x[ie]) - y[ie] );
    //cerr << "cumulative " << cumulative << " sigma " << sigma << endl;
    if( error  > sigma ) ++count;
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
  }
  gr_raw = new TGraph (lines, x, y);	
}

void datafile::plot_data()
{
  gr_raw -> Draw();
}

void datafile::plot_elab()
{
  gr_elab -> Draw();
}


void datafile::fill_mean(int N)
{
  using namespace std;
  int npoints = float(lines/N);
  n_elab = npoints;
  x_elab = new float[npoints];
  y_elab = new float[npoints];
  float tsum = 0;
  int ireal = 0; //counts from 0 to lines
  for(int index=0; index<npoints; index++){ //counts the points in _elab
    for(int i=0; i<N; i++){ //counts the points over which mean is taken
      tsum += y[ireal];
      ++ireal;
    }
    x_elab[index] = N*(index+1./2);
    y_elab[index] = tsum/int(N);
    tsum = 0;
  }
  gr_elab = new TGraph (n_elab, x_elab, y_elab);	
}

void datafile::clean_quad()
{
  //assumes the first point as the real one
  //removes systematic noise fitting with quadratic curves
  is =0; 
  ie = 100;
  TFitResultPtr r = gr_raw->Fit(quad,"S","",is*unit,ie*unit);
  while(ie < lines){
    /*float a = quad->GetParameter(2);
      float b = quad->GetParameter(1);
      float c = quad->GetParameter(0);
      */
    explore(quad,0.01,5);
    //once parameters are known, removes systematic
    //int n_in_interval = ie - is;
    for(int i=is; i<ie; i++){
      y[i] -= (*quad)(x[i]);
    }
    is = ie; 
    if( ie+100 < lines ) ie += 100; 
    else ie = lines;
    cout << ie << " corresponding to t = " << ie*100./3600 << endl;
    r = gr_raw->Fit(quad,"S","",is*unit,ie*unit);
  }
  gr_raw = new TGraph(lines,x,y);
}







/////////////////////////////////////////////////////
//UI FOR datafile


void plot_file(string NAME, float UNIT=1)
{
  datafile mydata(NAME,UNIT);
  mydata.clean_quad();
  mydata.plot_data();
}

void plot_mean(string NAME, float UNIT=1, int NMEAN=5)
{
  datafile mydata(NAME,UNIT);
  mydata.fill_mean(NMEAN);
  mydata.plot_elab();
}


//////////////////////














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
