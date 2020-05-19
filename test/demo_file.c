#include "crtl/file.h"
#include "crtl/log.h"

void demo_mkdir_rmdir()
{
    crtl_print_info("RT_DIR_MODE = %d\n", CRTL_DIR_MODE);
    
    crtl_mkdir("1/2/3/4/5/6/7/", CRTL_DIR_MODE);

    
    crtl_rmdir("1/");

    crtl_print_info("1/2 -> %d\n", crtl_is_directory("1/2"));
    crtl_print_info("toa -> %d\n", crtl_is_regular_file("toa"));
    crtl_print_info("/home/rongtao -> %d\n", crtl_is_directory("/home/rongtao"));
}

void demo_tempfile()
{
    char tempfile[256] = {0};
    crtl_print_info("tmpname: %s\n", crtl_mktemp_string(tempfile, ".", "core."));
    crtl_print_info("tmpname: %s\n", crtl_mktemp_string(tempfile, "", "core."));
    crtl_print_info("tmpname: %s\n", crtl_mktemp_string(tempfile, NULL, "core."));
}
void demo_file_fd()
{
//    if(CRTL_FD_Available(1)) {
//        crtl_print_info("1 is Available %d\n", CRTL_FD_Available(1));
//    }
//    crtl_print_info("1 is Available %d\n", CRTL_FD_Available(1));
}


void demo_scandir_test()
{
    struct dirent **dirs;
    int ndirs = crtl_scandir("../../", &dirs, crtl_scandir_filter_default, crtl_scandir_sort_default);

    int i;
    for(i=0;i<ndirs;i++) {
        printf("%s\n", dirs[i]->d_name);
    }
    
}

int main()
{
    demo_mkdir_rmdir();
    demo_tempfile();
    demo_file_fd();

    demo_scandir_test();
    
    return 0;
}

