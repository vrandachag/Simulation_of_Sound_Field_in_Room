# Simulation_of_Sound_Field_in_Room
reflection_coeff_sequence.cpp
Description:
The program intends to compute and store the values of the reflection coefficient of the reflected ray for a set of reflections in “reflection_coeff_sequence.csv”.
The program took 6 hours to execute for 5*1.0e6 rays and reflection order = 20.
Program works for 20 reflection orders, meaning it will check if the receiver is detected or not till 20 reflections of the ray. If it fails to detect the receiver, we terminate and move to the next ray.
Modules:
void add_reflection_coeff_sequence((unsigned long int number_of_rays, double radius, vector<vec3> temp_src_pos, vector<vec3> temp_img_pos, vector<int> temp_reflecting_plane_no, vector<vec3> temp_rp, vector<vec3> temp_reflected, vector<double> temp_angle_of_reflection, vector<complex_no> temp_Rsurface)
Function to add values in reflection_coeff_sequence.csv file
Data Dictionary:
unsigned int ray_no
Variable to run program for every ray 
int r
Variable for reflection order loop
vector<vec3> temp_src_pos
Temporary vector to store source positions for each reflection of a ray
vector<vec3> temp_img_pos 
Temporary vector to store image positions for each reflection of a ray
vector<vec3> temp_rp
Temporary vector to store positions of reflection point for each reflection of a ray
vector<vec3> temp_reflected
Temporary vector to store direction for each reflection of a ray
vector<int>temp_reflecting_plane_no
Temporary variable to store reflecting plane number for each reflection of ray
vector<double>temp_angle_of_reflection
Temporary vector to store angle of reflections for each reflection of a ray
vector<complex_no>temp_Rsurface
Temporary vector to store reflection coefficients for the surface for each reflection of a ray
Pressure_calc.cpp
Description:
The program intends to compute and store the values of the pressure of the ray after successful detection of the ray from the receiver in “pressure_value.csv”.
Modules:
phase calculate_arg(complex_no value)
Function to calculate the angle(phase) of complex number passed in the argument
Data Dictionary:
vector<source> s1
vector to store source position of each reflection
vector<receiver> r1
vector to store receiver position of each reflection
int source_frequency
Variable to store frequency of source in hertz
vector<string> row_entry
Variable to read from file reflection_coeff_sequence.csv
string read_line
Variable to read from file reflection_coeff_sequence.csv
string read_word
Variable to read from file reflection_coeff_sequence.csv










Ray_Tracing_Rev3.h
Modules:
phase calculate_arg(complex_no value)
Ref: used in pressure_calc.cpp
void add_reflection_coeff_sequence((unsigned long int number_of_rays, double radius, vector<vec3> temp_src_pos, vector<vec3> temp_img_pos, vector<int> temp_reflecting_plane_no, vector<vec3> temp_rp, vector<vec3> temp_reflected, vector<double> temp_angle_of_reflection, vector<complex_no> temp_Rsurface)
Ref: used in reflection_coeff_sequence.cpp
Data Dictionary:
double radian
Variable to store value of phase in radian
double degree
	Variable to store value of phase in degree
static unsigned long int ray_no
Variable to store value of ray number
string read_file
Variable to read from file reflection_coeff_sequence.csv
string temp_ray_no
Variable to read the last ray number from file reflection_coeff_sequence.csv












reflection_coeff_sequence.csv
Description:
File stores the value of reflection coefficients of the rays for multiple reflections.
Example:
Columns in the table are:
ray_no
number_of_rays
radius
temp_src_pos[i].x
temp_src_pos[i].y
temp_src_pos[i].z
temp_img_pos[i].x
temp_img_pos[i].y
temp_img_pos[i].z
temp_reflecting_plane_no[i]
temp_rp[i].x
temp_rp[i].y 
temp_rp[i].z
temp_reflected[i].x
temp_reflected[i].y
temp_reflected[i].z
temp_angle_of_reflection[i] 
temp_Rsurface[i].real
temp_Rsurface[i].imaginary
The first 13 rows in the file are for the first ray and the receiver was detected in the 13th reflection. The intermediate data will be used in pressure calculation.
To record data while executing C++ program multiple times, the ray number needs to be updated manually in the program(increment the value of last entry of 1st column)
