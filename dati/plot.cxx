#include <fstream> 
#include <string>
#include <iostream>


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



float** plot(string filestring){

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
		x[i] = i;
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
