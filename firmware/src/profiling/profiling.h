// adapted from William Dudley's profiling code
// https://www.dudley.nu/arduino_profiling/

#ifndef PROFILING_H
#define PROFILING_H

// this is just in place of a command line switch
//#define PROFILING

#ifdef PROFILING

#ifndef MAXPROF
#define MAXPROF 30
#endif

#ifndef PROFDUMP
#define PROFDUMP 10000
#endif

#define PF(n) do{prof_bin=n;}while(0)

#ifdef PROFILING_MAIN
    #define EXTERN
#else
    #define EXTERN extern
#endif

EXTERN volatile unsigned long prof_array[MAXPROF];
EXTERN volatile unsigned char prof_bin;
EXTERN volatile unsigned long prof_count;
EXTERN volatile unsigned long prof_overload;
EXTERN volatile unsigned long prof_underload;

EXTERN volatile unsigned long prof_report_sum;
EXTERN volatile unsigned long prof_report_count;

#define PROF_TARGET 25000

void clear_profiling_data();
void dump_profiling_data();
void profiling_loop();

#else
#define PF(n)
#endif


#endif