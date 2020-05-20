#include "crtl/log.h"
#include "crtl/bits/version.h"

int main()
{
    int version = crtl_version();

    crtl_print_info("info: %x\n", version);
    crtl_print_info("      %s\n", crtl_version_string());
}
