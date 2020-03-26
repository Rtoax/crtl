#include "crtl/crtl_config.h"
#include "crtl/crtl_log.h"


#define CONFIGREADFILE "demo_config.conf"
#define CONFIGSAVEFILE "demo_config_gen.conf"


void demo_config_test1()
{
    crtl_config_t *cfg = NULL;


	if (crtl_config_readfile(CONFIGREADFILE, &cfg) != CRTL_SUCCESS) {
		crtl_print_err("ConfigOpenFile failed for %s", CONFIGREADFILE);
		return;
	}

	crtl_config_printsettings(cfg, stdout);
	crtl_config_print(cfg, stdout);

	crtl_config_free(cfg);
}
void demo_config_test2()
{
    crtl_config_t *cfg = NULL;

	/* set settings */
	cfg = crtl_config_new();
	crtl_config_set_boolstring(cfg, "yes", "no");

	/* we can give initialized handle (rules has been set) */
	if (crtl_config_readfile(CONFIGREADFILE, &cfg) != CRTL_SUCCESS) {
		crtl_print_err("ConfigOpenFile failed for %s", CONFIGREADFILE);
		return;
	}

	crtl_config_remove_key(cfg, "SECT1", "a");
	crtl_config_remove_key(cfg, "SECT2", "aa");
	crtl_config_remove_key(cfg, "owner", "title");
	crtl_config_remove_key(cfg, "database", "file");

	crtl_config_add_bool  (cfg, "SECT1", "isModified", true);
	crtl_config_add_string(cfg, "owner", "country", "Turkey");

	crtl_config_printsettings(cfg, stdout);
	crtl_config_print(cfg, stdout);
	crtl_config_printtofile(cfg, CONFIGSAVEFILE);

	crtl_config_free(cfg);
}
    
void demo_config_test3()
{

    crtl_config_t *cfg = NULL;


	cfg = crtl_config_new();

	crtl_config_set_boolstring(cfg, "true", "false");

	crtl_config_add_string(cfg, "SECTION1", "Istanbul", "34");
	crtl_config_add_integer   (cfg, "SECTION1", "Malatya", 44);

	crtl_config_add_bool  (cfg, "SECTION2", "enable", true);
	crtl_config_add_double(cfg, "SECTION2", "Lira", 100);

	crtl_config_printsettings(cfg, stdout);
	crtl_config_print(cfg, stdout);

	crtl_config_free(cfg);
}


/*
 * Create Config without any section
 */
static void demo_config_test4()
{
	crtl_config_t *cfg = NULL;
	char s[1024];
	bool b;
	float f;

	cfg = crtl_config_new();

	crtl_config_add_string(cfg, NULL, "Mehmet Akif ERSOY", "Safahat");
	crtl_config_add_string(cfg, NULL, "Necip Fazil KISAKUREK", "Cile");
	crtl_config_add_bool  (cfg, NULL, "isset", true);
	crtl_config_add_float (cfg, NULL, "degree", 35.0);

	crtl_config_print(cfg, stdout);

	///////////////////////////////////////////////////////////////////////////////////////////////

	crtl_config_read_string(cfg, NULL, "Mehmet Akif Ersoy", s, sizeof(s), "Poet");
	crtl_print_debug("Mehmet Akif Ersoy = %s\n", s);

	crtl_config_read_string(cfg, NULL, "Mehmet Akif ERSOY", s, sizeof(s), "Poet");
	crtl_print_debug("Mehmet Akif ERSOY = %s\n", s);

	crtl_config_read_bool(cfg, NULL, "isset", &b, false);
	crtl_print_debug("isset = %s\n", b ? "true" : "false");

	crtl_config_read_float(cfg, NULL, "degree", &f, 1.5);
	crtl_print_debug("degree = %f\n", f);

	crtl_config_printtofile(cfg, CONFIGSAVEFILE);

	///////////////////////////////////////////////////////////////////////////////////////////////

	crtl_config_free(cfg);
}


int main()
{
    demo_config_test1();
    demo_config_test2();
    demo_config_test3();
    demo_config_test4();

    
    return 0;
}


