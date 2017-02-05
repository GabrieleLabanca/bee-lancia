#include <fstream> 
#include <string>

using filename std;

void plot(string filename){
	std::fstream file;
	file (filename.c_str(), std::fstream::in);

	string line = "000";
	float data;
	while(line.c_str()[0] != '#'  ) getline(file, line);
	
	int count_line =0 ;
	while( getline(file,line) ) count_line++;

	float * x = new float [count_line];
	float * y = new float [count_line];
	
	file (filename.c_str(), std::fstream::in);
	while(line.c_str()[0] != '#'  ) getline(file, line);
	for (int i =0; i<count_line; i++){
		x[i] = i;
		file >> y[i];
	}

	TGraph * plot = new TGraph (count_line, x, y);	


}
