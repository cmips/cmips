/* 
 * File:   main.cpp
 * Author: carles
 *
 * Created on 17 / August / 2013, 22:04
 * Last Modified on 27 / August / 2014 17:50
 */

// Note: link with -lpthread
// -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/cmips -lpthread
// 
// To check if binary is 32 or 64 with:
// ldd -v cmips | grep 64
// To know your CPU:
// grep -i --color "model name" /proc/cpuinfo

// TODO: Next version calculate the CMIPS for one single thread

#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fstream>
#include <sstream>
#include <cstring>

using namespace std;

#ifdef WIN32
#include <Windows.h>
#else
#include <sys/time.h>
#include <ctime>
#endif

typedef unsigned long long timestamp_t;

char s_cmips[50] = "CMIPS V.1.0.5 by Carles Mateo www.carlesmateo.com";

// Config for multiple threads, modern Servers.
int i_max_threads = 200;

int i_finished_threads = 0;

// Config parameters. Same with previous CMIPS versions
int i_loop_max = 32000;
int i_loop2_max = 32000;
int i_loop3_max = 10;

void *t_calculations(void *param)
{
    
    int *i_thread_id = (int *) param;

    char l_s_tmp_copy[1];
    int l_i_loop1 = 0;
    int l_i_loop2 = 0;
    int l_i_loop3 = 0;
    
    // current date/time based on current system
    time_t l_now = time(0);
    int l_i_counter = 0;
    int l_i_counter_char = 0;
   
    // convert now to string form
    char* l_dt_now = ctime(&l_now);
    
    printf("Starting thread %i at %s", *i_thread_id, l_dt_now);
    
    for (l_i_loop1 = 0; l_i_loop1 < i_loop_max; l_i_loop1++)
    {
        for (l_i_loop2 = 0; l_i_loop2 < i_loop2_max; l_i_loop2++) 
        {
            for (l_i_loop3 = 0; l_i_loop3 < i_loop3_max; l_i_loop3++) {
                // Increment test
                l_i_counter++;
                // Test "if" and assignment
                if (l_i_counter > 32000) {
                    l_i_counter = 0;
                }
                // Test char, accessing memory
                l_s_tmp_copy[0] = s_cmips[l_i_counter_char];
                
                l_i_counter_char++;
                if (l_i_counter_char > 49) {
                    l_i_counter_char = 0;
                }
                
            }
        }   
    }
    
    time_t l_now_end = time(0);
    
    // convert now to string form
    char* l_dt_now_end = ctime(&l_now_end);
    
    // Note: s_tmp_copy[0] is printed to prevent optimization of some C++ compilers (if the variable is not used)
    printf("End thread %i (%i) at %s", *i_thread_id, l_s_tmp_copy[0], l_dt_now_end);
    //cout << dt_now_end << "\n";
    
    i_finished_threads++;
    
    return NULL;
}

static timestamp_t get_timestamp ()
{
  struct timeval now;
  gettimeofday (&now, NULL);
  return  now.tv_usec + (timestamp_t)now.tv_sec * 1000000;
}

void write_log(char *s_text_to_log) {
    
    ofstream o_myfile;
    
    // Output and append to the end
    o_myfile.open ("cmips.log", ios::out | ios::app);
    
    if (o_myfile.is_open()) {
        o_myfile << s_text_to_log << "\n";
    }
    
    o_myfile.close();
    
}

void write_log(double d_value_to_log) {
    
    std::ostringstream os_text_to_log;
    std::string s_text_to_log;
    
    os_text_to_log << d_value_to_log;
    s_text_to_log = os_text_to_log.str();
 
    char *c_text_to_log=new char[s_text_to_log.size()+1];
    c_text_to_log[s_text_to_log.size()]=0;
    memcpy(c_text_to_log,s_text_to_log.c_str(),s_text_to_log.size());
    
    write_log(c_text_to_log);
    
}

bool fileExists (const std::string& s_name) {
    ifstream f(s_name.c_str());
    if (f.good()) {
        f.close();
        return true;
    } else {
        f.close();
        return false;
    }  
}

char *readFile(char *s_filename) {
    
    char* s_output = new char[2000000];
    
    if (fileExists == false) {
        return s_output;
    }
    
    ifstream o_ReadFile;
    o_ReadFile.open(s_filename);    
    
    if (o_ReadFile.is_open()) {
        while (!o_ReadFile.eof()) {
           //o_ReadFile >> s_output;
           o_ReadFile.read(s_output, 2000000);
        }
    }
    o_ReadFile.close();
   
    return s_output;    
    
}

int main()
{
    // Get info from the System
    char* s_system_maxthreads = new char[2000000];
    char* s_system_cpuinfo    = new char[2000000];

    s_system_maxthreads = readFile("/proc/sys/kernel/threads-max");
    s_system_cpuinfo = readFile("/proc/cpuinfo");
    
    timestamp_t t0 = get_timestamp();    
    
    // current date/time based on current system
    time_t now = time(0);
      
    // convert now to string form
    char* dt_now = ctime(&now);

    // File
    ofstream o_file;
    
    int i_counter = 0;    
    
    printf("CMIPS V1.0.5 by Carles Mateo - www.carlesmateo.com\n");
    write_log("CMIPS V1.0.5 by Carles Mateo - www.carlesmateo.com");
    
    printf("Max threads in the system: %s (from /proc/sys/kernel/threads-max)\n", s_system_maxthreads);
    write_log(s_system_maxthreads);
    printf("/proc/cpuinfo\n");
    cout << s_system_cpuinfo;
    write_log(s_system_cpuinfo);
    printf("\n");
    printf("Starting time: %s", dt_now);
    write_log("Starting time: ");    
    write_log(dt_now);
    
    printf("\n");
    printf("Starting calculations with %i Threads\n", i_max_threads);
    
    // http://stackoverflow.com/questions/344203/maximum-number-of-threads-per-process-in-linux
    // Linux doesn't have a separate threads per process limit, just a limit on the total number of processes on the system
    // To know the limit of processes on your Linux
    // cat /proc/sys/kernel/threads-max
    // The default is the number of memory pages/4. You can increase this like
    // echo 100000 > /proc/sys/kernel/threads-max
       
    pthread_t pt_calcs[i_max_threads];
    int i_pt_counter[i_max_threads];
    
    for (i_counter=1; i_counter<(i_max_threads +1); i_counter++) {
        i_pt_counter[i_counter] = i_counter;
        pthread_create(&pt_calcs[i_counter], NULL, t_calculations, &i_pt_counter[i_counter]);
    }
    
    for (i_counter=1; i_counter<(i_max_threads +1); i_counter++) {
        pthread_join(pt_calcs[i_counter], NULL);
    }
      
    // Process
    timestamp_t t1 = get_timestamp();

    double secs = (t1 - t0) / 1000000.0L;
    
    // * 2 to maintain compatibility with metrics from older CMIPS, as we doubled the number of threads
    int cmips = ((1 / secs) * 1000000) * 2;

    time_t now_end = time(0);
       
    // convert now to string form
    char* dt_now_end = ctime(&now_end);
    printf("End time: %s\n", dt_now_end);
    //cout << dt_now_end << "\n";
    write_log("End time: ");
    write_log(dt_now_end);
    
    printf("Execution time: %f\n", secs);
    write_log("Execution time:");
    write_log(secs);
    
    printf("CMIPS: %i\n", cmips);
    write_log("CMIPS: ");
    //cout << cmips << "\n";
    write_log(cmips);
            
    o_file.close();
    
    return 0;
    
}