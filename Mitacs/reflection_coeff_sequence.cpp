#define _USE_MATH_DEFINES // Marco for Pi

#include <iostream>
#include <fstream>
#include <chrono>
#include<iomanip>

#include "Ray_Tracing_Rev3.h"
#pragma once

using namespace std;

//Initliaze ray counter
long int ray::ray_count = 0;
int source::source_count = 0;

int main()
{    
    //time to calculate the run time
    time_t start_rt, end_rt;
    time(&start_rt);
        
    //Generate room
    box room;
    room.width = 0.7;
    room.length = 0.6;
    room.height = 0.5;

    // Generates array of planes objects according to room dimension
    room.generate_box_room();
    int no_of_planes = 6;
    int receiver_detected = 0;
    
    complex_no R;
    R.real = 0.9;
    R.imaginary = 0;
    
    //Room surface reflection coefficients real = 0.9 and imaginary = 0
    room.box_plane[0].R_coeff.real = R.real;//Bottom
    room.box_plane[0].R_coeff.imaginary = R.imaginary;
    room.box_plane[1].R_coeff.real = R.real;//left
    room.box_plane[1].R_coeff.imaginary = R.imaginary;
    room.box_plane[2].R_coeff.real = R.real;//front
    room.box_plane[2].R_coeff.imaginary = R.imaginary;
    room.box_plane[3].R_coeff.real = R.real; //right
    room.box_plane[3].R_coeff.imaginary = R.imaginary;
    room.box_plane[4].R_coeff.real = R.real;//back
    room.box_plane[4].R_coeff.imaginary = R.imaginary;
    room.box_plane[5].R_coeff.real = R.real;//top
    room.box_plane[5].R_coeff.imaginary = R.imaginary;

    int reflecting_plane_no;
    
    double R_eff_magnitude = 0.0;
    
    //Generate source
    source source1;
    source1.number_of_rays = 5*1.0e6;
    source1.position.x = room.length / 10.0;
    source1.position.y = room.width / 10.0;
    source1.position.z = room.height / 10.0;

    source1.power_db = 100;
    
        //Termination criteria
        int reflection_order = 20; //initial was 0
        double threshold=0.0;

        source1.generate_source();

        //Generate reciever
        receiver receiver1;
        receiver1.position.x = 3*(room.length / 10.0);
        receiver1.position.y = 4*(room.width / 10.0);
        receiver1.position.z = 5*(room.height / 10.0);
        receiver1.radius = .025; 
      
        cout << "Position of source \n";
        cout << source1.position.x << "\t" << source1.position.y << "\t" << source1.position.z << "\n";
        cout << "Source power (Watts) \t" << source1.power << "\n";
        cout << "Source frequency (Hz) \t" << source1.frequency << "\n";
        cout << "\n************************** \nRunning ray tracing algorithm\n";

        source1.generate_source();
                
        //Reset Source
        source1.reset_rays();

        //Reset Receiver
        receiver1.reset();
        
        //Timer
        auto start = std::chrono::steady_clock::now();
        
        for (unsigned int ray_no = 0; ray_no < source1.number_of_rays; ray_no++)
            {
            	//variable for reflection order loop
                int r;
                
                //temporary vectors
                vector<vec3> temp_src_pos, temp_img_pos, temp_rp, temp_reflected;
                vector<int>temp_reflecting_plane_no;
                vector<double>temp_angle_of_reflection;
                vector<complex_no>temp_Rsurface;
            
                R_eff_magnitude = 0.0;
                receiver_detected = 0;
                source1.rays[ray_no].isource_position = source1.position;
                
                if (source1.rays[ray_no].R_eff.real == 0 && source1.rays[ray_no].R_eff.imaginary == 0)
                {
                    continue; // Move to next ray
                }
                for (r = 1; (receiver_detected == 0) && (r <= reflection_order); r++) 
                    {
                        if (source1.rays[ray_no].R_eff.real == 0 && source1.rays[ray_no].R_eff.imaginary == 0)
                        {
                            break; // Terminate Ray
                        }
                        else if (detect_receiver(source1, receiver1, ray_no))
                        {                            
                            R_eff_magnitude = sqrt(source1.rays[ray_no].R_eff.real*source1.rays[ray_no].R_eff.real + source1.rays[ray_no].R_eff.imaginary*source1.rays[ray_no].R_eff.imaginary);

                            if (R_eff_magnitude <= threshold) {
                                break; //Terminate Ray
                            }
                            
                            receiver_detected = 1;
                        }

                        // Ray Reflection

                        // Find Reflection Plane
                        reflecting_plane_no = find_reflecting_box_plane(room, source1, ray_no);
                                                
                        // Get source image
                        vec3 image = image_source_position(source1.rays[ray_no].isource_position, room.box_plane[reflecting_plane_no]);

                        // Get reflection point
                        vec3 rp = reflection_point(source1.rays[ray_no].isource_position, room.box_plane[reflecting_plane_no], source1.rays[ray_no]);

                        // Get new direction
                        vec3 reflected = reflected_ray(room.box_plane[reflecting_plane_no], source1.rays[ray_no]);

                        // Get Angle
                        double angle_of_reflection = reflection_angle(room.box_plane[reflecting_plane_no], source1.rays[ray_no]);

                        //Calculate Reflection Coeff for the surface
                        complex_no Rsurface = calculate_R(room.box_plane[reflecting_plane_no], angle_of_reflection);

                        if(receiver_detected == 0)
                        {
                            temp_src_pos.push_back(source1.rays[ray_no].isource_position);
                            temp_img_pos.push_back(image);
    
                            // Get new ray after reflection (update ray)
                            source1.rays[ray_no].isource_position = image;
                            source1.rays[ray_no].direction = reflected;
                            source1.rays[ray_no].R_eff = source1.rays[ray_no].R_eff*Rsurface;
                        }
                        else
                        {
                            temp_src_pos.push_back(source1.rays[ray_no].isource_position);
                            temp_img_pos.push_back(receiver1.position);
                        }
                    
                        temp_reflecting_plane_no.push_back(reflecting_plane_no);
                        temp_rp.push_back(rp);
                        temp_reflected.push_back(reflected);
                        temp_angle_of_reflection.push_back(angle_of_reflection);
                        temp_Rsurface.push_back(Rsurface);
                    
                     }
                
                //receiver detected
                if(r != (reflection_order+1))
                {                    
                    //function to add values of reflection coefficient in file
                    add_reflection_coeff_sequence(source1.number_of_rays, receiver1.radius,temp_src_pos, temp_img_pos, temp_reflecting_plane_no, temp_rp, temp_reflected, temp_angle_of_reflection, temp_Rsurface); 
				}
            }
        
        //Program Timer
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::milli> elapsed_seconds = end - start;
        double total_time_milli = elapsed_seconds.count();
        double total_time = elapsed_seconds.count() / (1000.0);
        int hrs = (int)(total_time / (60.0 * 60.0));
        int minutes = (int)((total_time - hrs * 60.0 * 60.0) / 60.0);
        int seconds = (int)((total_time - hrs * 60.0 * 60.0 - minutes * 60.0));
        int millis = (int)((total_time_milli - hrs * 60.0 * 60.0*1000.0 - minutes * 60.0*1000.0 - seconds * 1000.0));
        std::cout << "elapsed time: " << hrs << "\t hrs \t" << minutes << "\t min \t" << seconds << "\t seconds \t" << millis << " milliseconds \t" << "\n";

        source1.delete_source();
    
    // Recording end time.
    time(&end_rt);
    
    // Calculating total time taken by the program.
    double time_taken = double(end_rt - start_rt);
    cout << "\nTime taken by program is : " << fixed<< time_taken << setprecision(5);
    cout << " sec " << endl;
    
    cout << "Program Complete \n";
    cout << "Press Enter key to close program";

    getchar();

    return 0;
}


