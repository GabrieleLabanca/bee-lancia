#include <fstream> 
#include <string>
#include <iostream>

class Datafile
{  
  public:
    // constructor
    Datafile(string,float,int);


    void fill_data(float);


    // mean
    int n_mean; // how much point over which make the mean?
    void fill_mean(int);

    // derivative
    void fill_derivative();

    // ntuple 
    TNtuple* nt_data = new TNtuple("nt_data",
        "data from Arduino",
        "Time:Weight:Temperature:Humidity");
    TNtuple* nt_mean = new TNtuple("nt_mean",
        "mean of data",
        "Time:Weight:Temperature:Humidity");
    TNtuple* nt_der  = new TNtuple("nt_der",
        "derivative of data",
        "Time:Weight:Temperature:Humidity");
    void fill_ntuple();
    void fill_ntuple_mean();
    void fill_ntuple_der();

    float unit; // to rescale i.e. the time
    /* EXAMPLE: acquisition every minute 
     * unit=1./60 gives the time on the axis in units of 1 hour
     */
    bool ACQUISITION_DONE = 0; // set to 1 the first time file is scanned 
    bool MEAN_DONE = 0; // set to 1 the first time mean is calculated 
    bool DERIVATIVE_DONE = 0; // set to 1 the first time derivative is calculated

    // actual data
    float * x;
    float * y;
    float * T;
    float * hum;

    // derivative
    float * dy; 
    float * dT;
    float * dhum;

    // data after elaboration
    int n_elab;
    float * x_elab;
    float * y_elab;
    float * T_elab;
    float * hum_elab;


    const char * filename;// name of the file to read from
    int headlines, lines; // to be set by count_lines()
    void count_lines();   // counts actual lines vs. header lines ("# ...") 

};


// CONSTRUCTOR
// sets: filename, unit
// fills x, y 
Datafile::Datafile(string filestring, float myunit = 1, int n_mean = 10):
  filename(filestring.c_str()),unit(myunit), n_mean(n_mean)
{
  count_lines();
  fill_ntuple();
  fill_ntuple_der();
  fill_ntuple_mean();
}

// performs a count of headlines and effective lines
// sets: headlines, lines
void Datafile::count_lines()
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


// extracts data from the file and creates a corresponding TGraph
// sets: gr_raw, gr_weight_raw, gr_temper_raw, ACQUISITION_DONE
// REQUIRES: headlines, lines
void Datafile::fill_data(float unit)
{
  x   = new float[lines];
  y   = new float[lines];
  T   = new float[lines];
  hum = new float[lines];
  using namespace std;
  ifstream myfile(filename);
  string temp;

  for (int i=0; i< headlines; i++){
    getline(myfile, temp);
  }
  for (int i =0; i<lines; i++){
    x[i] = i*unit; // rescale x
    myfile >> y[i] >> T[i] >> hum[i];
  }
  /* //OLD
  gr_raw = new TGraph (lines, x, y);
  gr_weight_raw = new TGraph (lines, x, y);
  gr_temper_raw = new TGraph (lines, x, T);
  */

  ACQUISITION_DONE = 1;
}

// extracts data from the file and fills the NTntuple nt_data
// sets: nt_data
// REQUIRES: headlines, lines
void Datafile::fill_ntuple()
{
  // if acquisition has been already done, skips the scan of document
  if(!ACQUISITION_DONE){ fill_data(unit); }
  for (int i =0; i<lines; i++){
    nt_data->Fill(x[i],y[i],T[i],hum[i]);
  }
}

void Datafile::fill_ntuple_mean()
{
  // if acquisition has been already done, skips the scan of document
  if(!MEAN_DONE){ fill_mean(n_mean); }
  for (int i =0; i<n_elab; i++){  // different index from data!
    nt_mean->Fill(x_elab[i],y_elab[i],T_elab[i],hum_elab[i]);
  }
}

void Datafile::fill_ntuple_der()
{
  // if acquisition has been already done, skips the scan of document
  if(!DERIVATIVE_DONE){ fill_derivative(); }
  for (int i =0; i<lines; i++){
    nt_der->Fill(x[i],dy[i],dT[i],dhum[i]);
  }
}



// performs a simple mean on the data and fills a TGraph
// REQUIRES: fill_data()
// N: every N points, a mean-point is produced
void Datafile::fill_mean(int N)
{
  if(!ACQUISITION_DONE) fill_data(unit);
  using namespace std;
  int npoints = float(lines/N);
  n_elab = npoints;
  x_elab   = new float[npoints];
  y_elab   = new float[npoints];
  T_elab   = new float[npoints];
  hum_elab = new float[npoints];
  float temp_sum[3] = {0,0,0};
  int ireal = 0; //counts from 0 to lines
  for(int index=0; index<npoints; index++){ //counts the points in _elab
    for(int i=0; i<N; i++){ //counts the points over which mean is taken
      temp_sum[0] += y[ireal];
      temp_sum[1] += T[ireal];
      temp_sum[2] += hum[ireal];
      ++ireal;
    }
    x_elab[index] = N*(index+1./2);
    y_elab[index] = temp_sum[0]/float(N);
    T_elab[index] = temp_sum[1]/float(N);
    hum_elab[index] = temp_sum[2]/float(N);
    temp_sum[0]=temp_sum[1]=temp_sum[2]=0;
  }
}

void Datafile::fill_derivative()
{
  //creates the arrays, [0]=0
  dy = new float[lines];
  dy[0] = 0;
  dT = new float[lines];
  dT[0] = 0;
  dhum = new float[lines];
  dhum[0] = 0;
  for(int i=0; i<lines; ++i){
    //if(i==0) dT[i]=0;
    dy[i] = (y[i] - y[i-1]);
    dT[i] = (T[i] - T[i-1]);//unit;
    dhum[i] = (hum[i] - hum[i-1]);
  }
  DERIVATIVE_DONE = 1;
}


