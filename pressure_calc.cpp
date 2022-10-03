#define _USE_MATH_DEFINES // Marco for Pi

#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include<iomanip>

#include "Ray_Tracing_Rev3.h"

using namespace std;

//Initliaze ray counter
long int ray::ray_count = 0;
int source::source_count = 0;

int main()
{
	static int prev_ray_no = 0;

	//vector to store source and receiver position of each reflection
	vector<source> s1;
	vector<receiver> r1;

	int source_frequency = 0; //in hertz
	
	//read value from reflection_coeff_sequence file
	fstream ray_tracing_plot;
	ray_tracing_plot.open("C:/Mitacs/reflection_coeff_sequence.csv",ios::in);

	//write value in pressure_value file	
	fstream pressure_value;
	pressure_value.open("C:/Mitacs/pressure_value.csv",ios::out|ios::in|ios::app);
	
	//variables to read from file
	vector<string> row_entry;
	string read_line, read_word;

	if(ray_tracing_plot.is_open())
	{
		while(getline(ray_tracing_plot, read_line))
		{
			if(read_line[0] == prev_ray_no +1)
			{
				row_entry.clear();
				
				stringstream str(read_line);
				
				while(getline(str, read_word, ','))
					row_entry.push_back(read_word);
					
				temp_r1.position.x = stoi(row_entry[6]);
				temp_r1.position.y = stoi(row_entry[7]);
				temp_r1.position.z = stoi(row_entry[8]);
				temp_s1.position.x = stoi(row_entry[3]);
				temp_s1.position.y = stoi(row_entry[4]);
				temp_s1.position.z = stoi(row_entry[5]);
				temp_r1.radius = stoi(row_entry[2]);
				temp_s1.number_of_rays = stoi(row_entry[1]);
				r1.push_back(temp_r1);
				s1.push_back(temp_s1);			
			}
			else
			{
				prev_ray_no++;
				complex_no receiver_pressure;
				for(int receiver_val_len = 0 ; receiver_val_len < r1.size(); receiver_val_len++)
				{
					receiver_pressure = receiver_pressure + calculate_ray_pressure(s1[receiver_val_len],r1[receiver_val_len],source_frequency);	
				}

				phase p1 = calculate_arg(receiver_pressure);
				pressure_value << prev_ray_no << "," << source_frequency << "," << receiver_pressure.real << "," << receiver_pressure.imaginary << "," << p1.degree << "\n";
			}
		}
	}
	else
		cout<<"Could not open the file\n";	
		
    getchar();

	return 0;
}
