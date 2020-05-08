
#include "crtl/bits/crtl_core.h"
#include "crtl/crtl_log.h"

static void demo_core_test1()
{
    crtl_utsname_t buffer;
    crtl_uname(&buffer);

    crtl_print_info("machine = %s\n", buffer.machine);
    crtl_print_info("release = %s\n", buffer.release);
    crtl_print_info("sysname = %s\n", buffer.sysname);
    crtl_print_info("version = %s\n", buffer.version);

    int priority = -1;
    crtl_getpriority(crtl_getpid(), &priority);
    crtl_print_info("priority = %d\n", priority);

    char cwd[1024];
    size_t size = 1024;

    crtl_cwd(cwd, &size);
    
    crtl_print_info("crtl_cwd = %s, size = %d\n", cwd, size);
    
    crtl_chdir("../");
    
    crtl_cwd(cwd, &size);
    crtl_print_info("crtl_cwd = %s, size = %d\n", cwd, size);

    
    char tmpdir[1024];
    size_t tmpdirsize = 1024;

    crtl_tmpdir(tmpdir, &tmpdirsize);
    crtl_print_info("tmpdir = %s, size = %d\n", tmpdir, tmpdirsize);

    
}

static void demo_core_test2()
{
    crtl_rusage_t rusage;
    crtl_getrusage(&rusage);
    
    crtl_print_info("ru_maxrss = %ld\n", rusage.ru_maxrss);
    crtl_print_info("ru_ixrss = %ld\n", rusage.ru_ixrss);
    crtl_print_info("ru_idrss = %ld\n", rusage.ru_idrss);
    crtl_print_info("ru_isrss = %ld\n", rusage.ru_isrss);
    crtl_print_info("ru_minflt = %ld\n", rusage.ru_minflt);
    crtl_print_info("ru_majflt = %ld\n", rusage.ru_majflt);
    crtl_print_info("ru_nswap = %ld\n", rusage.ru_nswap);
    crtl_print_info("ru_inblock = %ld\n", rusage.ru_inblock);
    crtl_print_info("ru_oublock = %ld\n", rusage.ru_oublock);
    crtl_print_info("ru_msgsnd = %ld\n", rusage.ru_msgsnd);
    crtl_print_info("ru_msgrcv = %ld\n", rusage.ru_msgrcv);
    crtl_print_info("ru_nsignals = %ld\n", rusage.ru_nsignals);
    crtl_print_info("ru_nvcsw = %ld\n", rusage.ru_nvcsw);
    crtl_print_info("ru_nivcsw = %ld\n", rusage.ru_nivcsw);
    //uint64_t ;    /* maximum resident set size */
    //uint64_t ;     /* integral shared memory size */
    //uint64_t ;     /* integral unshared data size */
    //uint64_t ;     /* integral unshared stack size */
    //uint64_t ;    /* page reclaims (soft page faults) */
    //uint64_t ;    /* page faults (hard page faults) */
    //uint64_t ;     /* swaps */
    //uint64_t ;   /* block input operations */
    //uint64_t ;   /* block output operations */
    //uint64_t ;    /* IPC messages sent */
    //uint64_t ;    /* IPC messages received */
    //uint64_t ;  /* signals received */
    //uint64_t ;     /* voluntary context switches */
    //uint64_t ;    /* involuntary context switches */
}


int main()
{
    demo_core_test1();
    demo_core_test2();

    
    return 0;
}

