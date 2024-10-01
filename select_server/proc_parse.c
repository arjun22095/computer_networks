#include "proc_parse.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>

void create_address(char *result, const char *pid) {
    // Create a file path for the /proc/[pid]/stat file
    snprintf(result, 1024, "/proc/%s/stat", pid);
}

void Init_Process(struct Process *proc) {
    memset(proc->name, 0, sizeof(proc->name)); // safer initialization
    proc->pid = -1;
    proc->utime = 0;
    proc->stime = 0;
    proc->cutime = 0;
    proc->cstime = 0;
    proc->total_time = 0;
}

void Init_TopTwoProcesses(struct TopTwoProcesses *cur) {
    Init_Process(&(cur->process_1));
    Init_Process(&(cur->process_2));
}

int get_file_data(const char *file_dir, char *buffer, size_t size) {
    FILE *fp = fopen(file_dir, "r");
    if (fp == NULL) {
        perror("[ERROR] Error while opening proc file");
        return -1;
    }

    if (fgets(buffer, size, fp) == NULL) {
        perror("[ERROR] fgets failed for");
        fclose(fp);
        return -1;
    }

    fclose(fp);
    return 0;
}

struct Process get_proc_data(const char *pid) {
    struct Process cur;
    Init_Process(&cur);

    char file_dir[1024] = {0};
    create_address(file_dir, pid);

    char file_data[1024];
    if (get_file_data(file_dir, file_data, sizeof(file_data)) == -1) {
        return cur; // return the initialized structure
    }

    // Read and parse the data from the file
    int fields_read = sscanf(file_data, "%d %255s %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %lu %lu %lu %lu",
                             &cur.pid, cur.name, &cur.utime, &cur.stime, &cur.cutime, &cur.cstime);

    // Check if the number of fields read is as expected
    if (fields_read < 6) {
        // printf("[ERROR] Incomplete process data for PID %s\n", pid);
        return cur; // return the initialized structure
    }

    // Calculate total time
    cur.total_time = cur.utime + cur.stime; // ( + cur.cutime + cur.cstime) could add

    return cur;
}

int is_string_number(const char *str) {
    while (*str) {
        if (!isdigit(*str)) {
            return -1;
        }
        str++;
    }
    return 0;
}

struct TopTwoProcesses get_max_two_procs_by_cpu_usage() {
    struct TopTwoProcesses answer;
    Init_TopTwoProcesses(&answer);

    const char *path = "/proc/";
    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("[ERROR] Open directory failed");
        return answer; // return initialized structure
    }

    struct dirent *entry;
    char full_path[1024];

    while ((entry = readdir(dir)) != NULL) {
        // Only consider directories with numeric names (process IDs)
        if (is_string_number(entry->d_name) == -1) {
            continue;
        }

        struct Process proc = get_proc_data(entry->d_name);

        // Skip if the process data is incomplete
        if (proc.pid == -1) {
            continue;
        }

        // Update the top two processes based on total_time
        if (proc.total_time > answer.process_1.total_time) {
            answer.process_2 = answer.process_1;
            answer.process_1 = proc;
        } else if (proc.total_time > answer.process_2.total_time) {
            answer.process_2 = proc;
        }
    }

    closedir(dir);
    return answer;
}
