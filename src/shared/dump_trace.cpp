// prints a backtrace
// original author: jeff cobb 
#include "backtrace.h"


// #include <iostream>
// #include <execinfo.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <cxxabi.h>
// #include <string.h>
// #define BT_SIZE 100


// void dump_backtrace(const char *tag)
// {
//     printf("\n******** %s **********\n", tag);

//     const size_t max_depth = 100;
//     const char nul = '\0';
//     size_t stack_depth;
//     void * stack_addrs[max_depth];
//     char ** stack_strings;

//     stack_depth = backtrace(stack_addrs, max_depth);
//     stack_strings = backtrace_symbols(stack_addrs, stack_depth);
 
//     for (size_t i = 1; i < stack_depth; i++) {

//         size_t sz = 200; // just a guess, template names will go much wider
//         char *function = static_cast<char*>(malloc(sz));
//         char *begin = 0, *end = 0;

//         // find the parentheses and address offset surrounding the mangled name
//         for (char *j = stack_strings[i]; *j; ++j) {
//             if (*j == '(') {
//                 begin = j;
//             } 
//             else if (*j == '+') {
//                 end = j;
//             }
//         }

//         std::cout << stack_strings[i] << std::endl;

//         if (begin && end) {
//             *begin++ = nul;
//             *end = nul;
//             // found our mangled name, now in [begin, end)

//             int status;
//             char * ret = abi::__cxa_demangle(begin, function, &sz, &status);
//             if (ret) {
//                 // return value may be a realloc() of the input
//                 function = ret;
//             } 
//             else {
//                 // demangling failed, just pretend it's a C function with no args
//                 strncpy(function, begin, sz);
//                 strncat(function, "()", sz);
//                 function[sz-1] = nul;
//             }
//             printf("    %s:%s\n", stack_strings[i], function);
//         } 
//         else {
//             // didn't find the mangled name, just print the whole line
//             printf("    %s\n", stack_strings[i]);
//         }
//         free(function);
//     }
// }



#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void dump_trace() {
    char pid_buf[30];
    sprintf(pid_buf, "%d", getpid());
    char name_buf[512];
    name_buf[readlink("/proc/self/exe", name_buf, 511)]=0;
    int child_pid = fork();
    if (!child_pid) {           
        dup2(2,1); // redirect output to stderr
        fprintf(stdout,"stack trace for %s pid=%s\n",name_buf,pid_buf);
        execlp("gdb", "gdb", "--batch", "-n", "-ex", "thread", "-ex", "bt", name_buf, pid_buf, NULL);
        abort(); /* If gdb failed to start */
    } else {
        waitpid(child_pid,NULL,0);
    }
}
