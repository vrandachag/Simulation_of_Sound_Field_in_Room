#define _USE_MATH_DEFINES // Marco for Pi

#include <iostream>
#include <fstream>
#include <chrono>
#include<iomanip>

#include "Ray_Tracing_Rev3.h"
//#pragma once

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
    
    complex R;
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
    
    //Source Frequency Variables in Hertz
    float freq_min = 0.0;
    float freq_step = 0.25;
    float freq_max = 10.0;

    for(float f = freq_min ; f < freq_max ; f = f + freq_step)
    {
        source1.frequency = f;//Hz

        cout<<"\nfrequency"<<source1.frequency; //for dry run
        
        //Termination criteria
        int reflection_order = 40; //initial was 0
        double threshold=0.0;

        source1.generate_source();

        //Generate reciever
        receiver receiver1;
        receiver1.position.x = 3*(room.length / 10.0);
        receiver1.position.y = 4*(room.width / 10.0);
        receiver1.position.z = 5*(room.height / 10.0);
        receiver1.radius = .050; //initial radius was 0.025

        //Plotting variable
        fstream ray_tracing_plot;
        int progress = 10;
        int progress_count = 0;

        ray_tracing_plot.open("C:/Mitacs/replica_reflection_order_40.csv",ios::out|ios::in|ios::app);
        
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
                cout<<"\nray_no:"<<ray_no;
                R_eff_magnitude = 0.0;
                receiver_detected = 0;
                
                if (source1.rays[ray_no].R_eff.real == 0 && source1.rays[ray_no].R_eff.imaginary == 0)
                {
                    cout<<"\n no reflection happened";
                    continue; // Move to next ray
                }
                for (int r = 1; r <= reflection_order; r++) //removed +1 in condition check
                    {
                        cout<<"\nr:"<<r; //for dry run
                        
                        if (source1.rays[ray_no].R_eff.real == 0 && source1.rays[ray_no].R_eff.imaginary == 0)
                        {
                            cout<<"\nRay terminated in if"; //for dry run
                            break; // Terminate Ray
                        }
                        else if ((!receiver_detected) && detect_receiver(source1, receiver1, ray_no))
                        {
                            cout<<"\nreceiver detected"; //for dry run
                            
                            complex ray_pressure = calculate_ray_pressure(source1, receiver1, ray_no);
                            
                            cout<<"\nray pressure real value:"<<ray_pressure.real<<"\tray pressure imag value:"<<ray_pressure.imaginary; //for dry run
                            
                            R_eff_magnitude = sqrt(source1.rays[ray_no].R_eff.real*source1.rays[ray_no].R_eff.real + source1.rays[ray_no].R_eff.imaginary*source1.rays[ray_no].R_eff.imaginary);

                            cout<<"\n R_eff_magnitude:"<<R_eff_magnitude<<"\tthreshold:"<<threshold; //for dry run
                            if (R_eff_magnitude <= threshold) {
                                cout<<"ray terminated because of threshold value";
                                break; //Terminate Ray
                            }

                            else {
                                cout<<"\nprogram in else calculating receiver pressure"; //for dry run
                                
                                receiver1.pressure = receiver1.pressure + ray_pressure;
                            }
                            
                            receiver_detected = 1;
                        }

                        // Ray Reflection

                        // Find Reflection Plane
                        reflecting_plane_no = find_reflecting_box_plane(room, source1, ray_no);
                        
                        cout<<"\nreflecting plane number:"<<reflecting_plane_no;
                        
                        // Get source image
                        vec3 image = image_source_position(source1.rays[ray_no].isource_position, room.box_plane[reflecting_plane_no]);

                        // Get reflection point
                        vec3 rp = reflection_point(source1.rays[ray_no].isource_position, room.box_plane[reflecting_plane_no], source1.rays[ray_no]);

                        // Get new direction
                        vec3 reflected = reflected_ray(room.box_plane[reflecting_plane_no], source1.rays[ray_no]);

                        // Get Angle
                        double angle_of_reflection = reflection_angle(room.box_plane[reflecting_plane_no], source1.rays[ray_no]);

                        //Calculate Reflection Coeff for the surface
                        complex Rsurface = calculate_R(room.box_plane[reflecting_plane_no], angle_of_reflection);

                        // Get new ray after reflection (update ray)
                        source1.rays[ray_no].isource_position = image;
                        source1.rays[ray_no].direction = reflected;
                        source1.rays[ray_no].R_eff = source1.rays[ray_no].R_eff*Rsurface;
                    }

                progress_count++;
                cout<<"\n progress count:"<<progress_count; //for dry run
                
                if (progress_count >= (progress / 100.0) * source1.number_of_rays)
                {
                    cout << "Progress = " << progress << " % \n";
                    progress += 10;
                }
            }
        
        phase receiver_phase = calculate_receiver_phase(receiver1.pressure);

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

        //write the headings
        /*
        if(!fileExists(ray_tracing_plot))
        {
            ray_tracing_plot << "number_of_rays" << "," << "receiver1_radius" << "," << "total_time_milli" << "," << "receiver_detect_count" << "," << "p2_analytical" << "," << "receiver_pressure_real" << "," << "receiver_pressure_imaginary" << "," << "receiver_pressure_db" << "," << "analytical_pressure_db" << "," << "sub_receiver_pressure_db_analytical_pressure_db" << "," << "source_frequency" << "," << "receiver_phase_degree" << "\n";
    
        }
        */
        ray_tracing_plot << source1.number_of_rays << "," << receiver1.radius << "," << total_time_milli << "," << receiver1.detect_count << "," << R_eff_magnitude << "," << receiver1.pressure.real << "," << receiver1.pressure.imaginary << "," << receiver_pressure_db(receiver1) << "," << source1.frequency << "," << receiver_phase.degree << "\n";

        source1.delete_source();

        ray_tracing_plot.close();
    }
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

