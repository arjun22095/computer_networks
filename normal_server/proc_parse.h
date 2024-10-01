#ifndef PROC_PARSE_H
#define PROC_PARSE_H

#include <stddef.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <malloc.h>
#include <arpa/inet.h>
#include <signal.h>  // For signal handling
#include <stdlib.h>

// Struct definition for holding process information
struct Process {
    char name[100];   // Process name
    int pid;          // Process ID
    long utime;       // [14] CPU time spent in user code (clock ticks)
    long stime;       // [15] CPU time spent in kernel code (clock ticks)
    long cutime;      // [16] CPU time spent by waited-for children's user code (clock ticks)
    long cstime;      // [17] CPU time spent by waited-for children's kernel code (clock ticks)
    long total_time;  // Total CPU time used by the process
};

// Struct definition for holding the two processes with highest CPU usage
struct TopTwoProcesses {
    struct Process process_1;  // Process with highest CPU usage
    struct Process process_2;  // Process with second-highest CPU usage
};

// Function to initialize a Process structure
void Init_Process(struct Process *proc);

// Function to initialize a TopTwoProcesses structure
void Init_TopTwoProcesses(struct TopTwoProcesses *cur);

// Function to create the full file path for the /proc/[pid]/stat file
void create_address(char *result, const char *pid);

// Function to read file data from a given file path into a buffer
// Returns 0 on success, -1 on error
int get_file_data(const char *file_dir, char *buffer, size_t size);

// Function to parse process data from the /proc/[pid]/stat file
// Returns a Process struct populated with data
struct Process get_proc_data(const char *pid);

// Function to check if a string represents a valid numeric PID
// Returns 0 if the string is numeric, -1 otherwise
int is_string_number(const char *str);

// Function to find and return the top two processes by CPU usage
// Scans the /proc/ directory and returns a TopTwoProcesses struct
struct TopTwoProcesses get_max_two_procs_by_cpu_usage();

#endif // PROC_PARSE_H
