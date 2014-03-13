/* 
 * File:   main.cpp
 * Author: carles
 *
 * Created on 17 / agost / 2013, 22:04
 */

#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <pthread.h>

using namespace std;

#ifdef WIN32
#include <Windows.h>
#else
#include <sys/time.h>
#include <ctime>
#endif

/* Returns the amount of milliseconds elapsed since the UNIX epoch. Works on both
 * windows and linux. */
/*
int64 GetTimeMs64()
{
#ifdef WIN32
 /* Windows * /
 FILETIME ft;
 LARGE_INTEGER li;

 /* Get the amount of 100 nano seconds intervals elapsed since January 1, 1601 (UTC) and copy it
  * to a LARGE_INTEGER structure. * /
 GetSystemTimeAsFileTime(&ft);
 li.LowPart = ft.dwLowDateTime;
 li.HighPart = ft.dwHighDateTime;

 uint64 ret = li.QuadPart;
 ret -= 116444736000000000LL; /* Convert from file time to UNIX epoch time. * /
 ret /= 10000; /* From 100 nano seconds (10^-7) to 1 millisecond (10^-3) intervals * /

 return ret;
#else
 /* Linux * /
 struct timeval tv;

 gettimeofday(&tv, NULL);

 uint64 ret = tv.tv_usec;
 /* Convert from micro seconds (10^-6) to milliseconds (10^-3) * /
 ret /= 1000;

 /* Adds the seconds (10^0) after converting them to milliseconds (10^-3) * /
 ret += (tv.tv_sec * 1000);

 return ret;
#endif
}

*/

typedef unsigned long long timestamp_t;

int i_max_threads = 100;
int i_finished_threads = 0;

int i_loop1 = 0;
int i_loop_max = 32000;
int i_loop2 = 0;
int i_loop2_max = 32000;
int i_loop3 = 0;
int i_loop3_max = 2;
int i_counter = 0;

void *t_calculations(void *param)
{
    //char *thread_id = param;

    // current date/time based on current system
    time_t now = time(0);
   
    // convert now to string form
    char* dt_now = ctime(&now);
    
    printf("Starting thread "); // %s at ", thread_id);
    cout << dt_now << "\n";
    for (i_loop1 = 0; i_loop1<i_loop_max; i_loop1++)
    {
        for (i_loop2 = 0; i_loop2<i_loop2_max; i_loop2++) 
        {
            for (i_loop3 = 0; i_loop3<i_loop3_max; i_loop3++) {
                i_counter++;
                if (i_counter>32000) {
                    i_counter = 0;
                }                
            }
        }   
    }
    
    time_t now_end = time(0);
    
    // convert now to string form
    char* dt_now_end = ctime(&now_end);
    
    printf("End thread at "); // %s at ", thread_id);
    cout << dt_now_end << "\n";
    
    i_finished_threads++;
    
    return NULL;
}

static timestamp_t
get_timestamp ()
{
  struct timeval now;
  gettimeofday (&now, NULL);
  return  now.tv_usec + (timestamp_t)now.tv_sec * 1000000;
}

int main()
{
    
    timestamp_t t0 = get_timestamp();
    
    // current date/time based on current system
    time_t now = time(0);
   
    // convert now to string form
    char* dt_now = ctime(&now);
    
    printf("CMIPS V1.0 by Carles Mateo - www.carlesmateo.com\n");
    printf("\n");
    printf("Starting time: ");
    cout << dt_now;
    printf("\n");
    printf("Starting calculations\n");
    
    pthread_t p1, p2, p3, p4, p5, p6, p7, p8, p9, p10;

    pthread_create(&p1, NULL, t_calculations, NULL);
    pthread_create(&p2, NULL, t_calculations, NULL);
    pthread_create(&p3, NULL, t_calculations, NULL);
    pthread_create(&p4, NULL, t_calculations, NULL);
    pthread_create(&p5, NULL, t_calculations, NULL);
    pthread_create(&p6, NULL, t_calculations, NULL);
    pthread_create(&p7, NULL, t_calculations, NULL);
    pthread_create(&p8, NULL, t_calculations, NULL);
    pthread_create(&p9, NULL, t_calculations, NULL);
    pthread_create(&p10, NULL, t_calculations, NULL);
    
    // Wait for the threads to finish
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    pthread_join(p3, NULL);
    pthread_join(p4, NULL);
    pthread_join(p5, NULL);
    pthread_join(p6, NULL);
    pthread_join(p7, NULL);
    pthread_join(p8, NULL);
    pthread_join(p9, NULL);
    pthread_join(p10, NULL);
    
    // Process
    timestamp_t t1 = get_timestamp();

    double secs = (t1 - t0) / 1000000.0L;

    time_t now_end = time(0);
    
    // convert now to string form
    char* dt_now_end = ctime(&now_end);
    printf("End time: ");
    cout << dt_now_end << "\n";
    
    printf("Execution time:");
    //printf(secs);
    cout << secs << "\n";
    
    return 0;
    
}