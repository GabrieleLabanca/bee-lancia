#include <fstream> 
#include <string>
#include <iostream>
#include <cmath>
#include <vector>
#include <map>

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








    /*

    // UI functions 
    void plot_data(char);
    void plot_elab(); 

    void clean_quad();

    // relative maximums
    map<float,float> find_max_weight(); // returns vector of time of maximums
    void fill_graph_weight_max();

    // graphs
    TGraph * gr_raw;  // actual data
    TGraph * gr_weight_raw; // actual data, weight
    TGraph * gr_weight_max; // max of actual data, weight
    TGraph * gr_temper_raw; // actual data, temperature
    TGraph * gr_weight_der;  // "derivative" of weight
    TGraph * gr_temper_der;  // "derivative" of temperature
    TH1F   * h_der;   // histogram of "derivatives"
    TGraph * gr_elab; // data after elaboration


    //protected:

    // get parameters
    int get_n_lines() { return lines; } 
    int get_n_elab() { return n_elab; }
    float get_x(int i) { return x[i]; }
    float get_y(int i) { return y[i]; }
    float get_xel(int i) { return x_elab[i]; }
    float get_yel(int i) { return y_elab[i]; }
    float get_u() { return unit; }

    // derivative
    double get_weight_derivative();
    double get_temperature_derivative();


    //private:
    
    // iterators: start, border, explorer
    int is, ib, ie;
    void explore(TF1*, float, int);

    //float a, b, c;
    TF1 * quad = new TF1("quad","[2]*x*x+[1]*x+[0]",0,10);
    //  void quadratic_fit(float*, float*, int, int, float&, float&, float&);
      */
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


////////////////////////////////////
// OLD METHODS
/*

// finds the first point exceeding an arbitrary deviation from a given function
// f: the desired f(x) fitting the data
// sigma: the maximum deviation allowed
// howfar: number of times sigma can be exceeded 
//         before considering it a consistent behaviour//
void Datafile::explore(TF1 * f, float sigma, int howfar=5)
{
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


// fills gr_der with derivative
double Datafile::get_weight_derivative()
{
  double d_mean = 0; 
  dy = new float[lines];
  dy[0] = 0;
  h_der = new TH1F("h_der","derivative distribution",1000,-0.01,0.01);
  for(int i=0; i<lines; ++i){
    if(i==0) dy[i] = 0;
    dy[i] = (y[i] - y[i-1]);//unit;
    d_mean += dy[i];
    h_der->Fill(dy[i]);
  }
  gr_weight_der = new TGraph(lines,x,dy);
  return d_mean/lines;
}

double Datafile::get_temperature_derivative()
{
  double d_mean = 0; 
  dT = new float[lines];
  dT[0] = 0;
  for(int i=0; i<lines; ++i){
    if(i==0) dT[i]=0;
    dT[i] = (T[i] - T[i-1]);//unit;
    d_mean += dT[i];
  }
  gr_temper_der = new TGraph(lines,x,dT);
  return d_mean/lines;
}




void Datafile::fill_graph_weight_max()
{
  using namespace std;
  map<float,float> max_map = find_max_weight();
  int n_max = max_map.size();
  float * t = new float[n_max];
  float * w = new float[n_max];
  // scans the map, filling the two arrays
  for(map<float,float>::iterator it=max_map.begin(); it!=max_map.end(); ++it){
    int i = std::distance(max_map.begin(),it);
    t[i] = it->first;
    w[i] = it->second;
  }
  gr_weight_max = new TGraph(n_max,t,w);
}


//assumes the first point as the real one
//removes systematic noise fitting with quadratic curves
void Datafile::clean_quad()
{
  //assumes the first point as the real one
  //removes systematic noise fitting with quadratic curves
  is =0; 
  ie = 100;
  TFitResultPtr r = gr_raw->Fit(quad,"S","",is*unit,ie*unit);
  while(ie < lines){
     explore(quad,0.02,1);   // function, sigma, howfar
    //once parameters are known, removes systematic
    //int n_in_interval = ie - is;
    for(int i=is; i<ie; i++){
      y[i] -= quad->GetParameter(2)*x[i]*x[i] + quad->GetParameter(1)*x[i]; //(*quad)(x[i]);
    }
    is = ie; 
    if( ie+50 < lines ) ie += 50; 
    else ie = lines;
    cout << ie << " corresponding to t = " << ie*100./3600 << endl;
    r = gr_raw->Fit(quad,"S","",is*unit,ie*unit);
  }
  gr_raw = new TGraph(lines,x,y);
}

map<float,float> Datafile::find_max_weight()
{
  if(!ACQUISITION_DONE) fill_data(unit);
  using namespace std;
  fill_mean(20);
  map<float,float> max_map;
  float der1 = 0;
  float der2;
  int counter = 0;
  // ranges over mean points
  for(int i=0; i<n_elab; ++i){
    der2 = y_elab[i+1] - y_elab[i];
    if(der1*der2<0) ++counter;
    if(counter>10){
      max_map.insert(pair<float,float>(x_elab[i],y_elab[i]));
      counter = 0;
    }
    der1 = der2;
  }
  return max_map;
}

// plots gr_raw (actual data)
// mode: '0'  normal plot
//       's'  option "same" passed to TGraph->Draw
void Datafile::plot_data(char mode = '0')
{
  switch(mode){
    case '0': 
      gr_raw -> Draw();
      break;
    case 's':
      gr_raw -> Draw("same");
      break;
  }
}


// plots gr_elab (data after elaboration)
void Datafile::plot_elab()
{
  gr_elab -> Draw();
}

*/



/////////////////////////////////////////////////////
//UI FOR Datafile OLD
/*

void plot_file(string NAME, float UNIT=1)
{
  Datafile mydata(NAME,UNIT);
  mydata.plot_data();
  mydata.clean_quad();
  mydata.plot_data('s');
}

void plot_mean(string NAME, float UNIT=1, int NMEAN=5)
{
  Datafile mydata(NAME,UNIT);
  mydata.fill_mean(NMEAN);
  mydata.plot_elab();
}

*/

//////////////////////

