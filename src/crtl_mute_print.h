#undef  crtl_print_emerg
#undef  crtl_print_alert
#undef  crtl_print_crit
#undef  crtl_print_err
#undef  crtl_print_warning
#undef  crtl_print_notice
#undef  crtl_print_info
#undef  crtl_print_debug

#define crtl_print_emerg(fmt...)    
#define crtl_print_alert(fmt...)    
#define crtl_print_crit(fmt...)    
#define crtl_print_err(fmt...)     
#define crtl_print_warning(fmt...) 
#define crtl_print_notice(fmt...)  
#define crtl_print_info(fmt...)  
#define crtl_print_debug(fmt...) 

