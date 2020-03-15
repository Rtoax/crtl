#include "crtl/crtl_file.h"
#include "crtl/crtl_log.h"

void demo_mkdir_rmdir()
{
    crtl_print_info("RT_DIR_MODE = %d\n", CRTL_DIR_MODE);
    
    crtl_mkdir("1/2/3/4/5/6/7/", CRTL_DIR_MODE);

    
    crtl_rmdir("1/");

    crtl_print_info("1/2 -> %d\n", crtl_is_directory("1/2"));
    crtl_print_info("toa -> %d\n", crtl_is_regular_file("toa"));
}

void demo_tempfile()
{
    char tempfile[256] = {0};
    crtl_print_info("tmpname: %s\n", crtl_mktempfile(tempfile, ".", "core."));
}

int main()
{
    demo_mkdir_rmdir();
    demo_tempfile();
    
    return 0;
}

