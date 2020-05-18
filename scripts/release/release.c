/*
    release.c   This file is to release CRTL srouce code file
    Copyright (C) 2020  Rong Tao <2386499836@qq.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.



	release.c  Copyright (C) 2020  Rong Tao

    This program comes with ABSOLUTELY NO WARRANTY; for details type `show w'.
    This is free software, and you are welcome to redistribute it
    under certain conditions; type `show c' for details.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

const char GPL_2_0[] = {
"/**    \n"\
" *   %s %s\n"\
" *   Copyright (C) %s  %s\n"\
" *    \n"\
" *   This program is free software: you can redistribute it and/or modify\n"\
" *   it under the terms of the GNU General Public License as published by\n"\
" *   the Free Software Foundation, either version 3 of the License, or\n"\
" *   (at your option) any later version.\n"\
" *    "\
" *   This program is distributed in the hope that it will be useful,\n"\
" *   but WITHOUT ANY WARRANTY; without even the implied warranty of\n"\
" *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"\
" *   GNU General Public License for more details.\n"\
" *    \n"\
" *   You should have received a copy of the GNU General Public License\n"\
" *   along with this program.  If not, see <http://www.gnu.org/licenses/>.\n"\
" *    \n"\
" *    \n"\
" *   %s  Copyright (C) %s  %s\n"\
" *    \n"\
" *   This program comes with ABSOLUTELY NO WARRANTY; for details type `show w'.\n"\
" *   This is free software, and you are welcome to redistribute it\n"\
" *   under certain conditions; type `show c' for details.\n"\
" */ \n\n"\
};


const char GPL_3_0[] = {
"/**    \n"\
" *   %s %s\n"\
" *   Copyright (C) %s  %s\n"\
" *    \n"\
" *   This program is free software: you can redistribute it and/or modify\n"\
" *   it under the terms of the GNU General Public License as published by\n"\
" *   the Free Software Foundation, either version 3 of the License, or\n"\
" *   (at your option) any later version.\n"\
" *    "\
" *   This program is distributed in the hope that it will be useful,\n"\
" *   but WITHOUT ANY WARRANTY; without even the implied warranty of\n"\
" *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"\
" *   GNU General Public License for more details.\n"\
" *    \n"\
" *   You should have received a copy of the GNU General Public License\n"\
" *   along with this program.  If not, see <http://www.gnu.org/licenses/>.\n"\
" *    \n"\
" *    \n"\
" *   %s  Copyright (C) %s  %s\n"\
" *    \n"\
" *   This program comes with ABSOLUTELY NO WARRANTY; for details type `show w'.\n"\
" *   This is free software, and you are welcome to redistribute it\n"\
" *   under certain conditions; type `show c' for details.\n"\
" */ \n\n"\
};


void show_help(char *proname)
{
    printf("\n");
    printf("\033[1m%s\033[m release LibCRTL source code.\n",proname);
    printf("\n");
    printf("Usage:  -f, --filename <filename>       File name of source code.\n");
    printf("        -d, --description <description> Description of source code.\n");
    printf("        -y, --year <year>               Copyright (C) year of source code.\n");
    printf("        -a, --author <author>           Copyright (C) author of source code.\n");
    printf("        -g, --GPL-2.0                   Copyright (C) licenses of source code.\n");
    printf("        -G, --GPL-3.0                   Copyright (C) licenses of source code.\n");
    printf("        -h, --help                      This help text.\n");
    printf("\n");
    exit(0);
}

typedef enum {
    _GPL_2_0 = 1,
    _GPL_3_0,
}licenses_type;

struct {
    char *filename;
    char *introduction;
    char *year;
    char *author;
    licenses_type licenses;
}init_args = {
    NULL, NULL, NULL, NULL, 0
};

#if LOG_DBG_ON
#define debug(fmt...) printf(fmt)
#else
#define debug(fmt...)
#endif

void init_argument(int argc, char *argv[])
{
    
    if(argc < 9) {
        show_help(argv[0]);
        exit(0);
    }

    int c;

    static struct option options[] = {
        {"filename",        required_argument,  0, 'f'},
        {"description",     required_argument,  0, 'd'},
        {"year",            required_argument,  0, 'y'},
        {"author",          required_argument,  0, 'a'},
        {"GPL-2.0",         no_argument,        0, 'g'},
        {"GPL-3.0",         no_argument,        0, 'G'},
        {"help",            no_argument,        0, 'h'},
        {0, 0, 0, 0}
    };
        
    while (1) {
        /* getopt_long stores the option index here. */
        int option_index = 0;
        
        c = getopt_long (argc, argv, "f:d:y:a:gGh", options, &option_index);
        
        /* Detect the end of the options. */
        if (c == -1)
            break;
        switch (c)
        {
            case 'f':
                debug("filename = %s\n", optarg);
                init_args.filename = optarg;
                break;
            case 'd':
                debug("introduction = %s\n", optarg);
                init_args.introduction = optarg;
                break;
            case 'y':
                debug("year = %s\n", optarg);
                init_args.year = optarg;
                break;
            case 'a':
                debug("author = %s\n", optarg);
                init_args.author = optarg;
                break;
            case 'g':
                debug("licenses = GPL-2.0\n");
                init_args.licenses = _GPL_2_0;
                break;
            case 'G':
                debug("licenses = GPL-3.0\n");
                init_args.licenses = _GPL_3_0;
                break;
            case 'h':
                debug("help\n");
                show_help(argv[0]);
                break;
        }
    }

    if(!init_args.filename || !init_args.introduction || !init_args.year || !init_args.author) {
        show_help(argv[0]);
        exit(0);
    }
    if(init_args.licenses != _GPL_2_0 && init_args.licenses != _GPL_3_0) {
        init_args.licenses = _GPL_3_0;
    }

    debug("%s\n", init_args.filename);
    debug("%s\n", init_args.introduction);
    debug("%s\n", init_args.year);
    debug("%s\n", init_args.author);
    debug("%s\n", init_args.licenses==_GPL_2_0?"GPL-2.0":"GPL-3.0");
}

void write_license()
{
#define TMP_FILE "/tmp/crtl_release_tmpfile_12312"
    
    char cmd[256] ={0};
    
    sprintf(cmd, "cat %s > "TMP_FILE, init_args.filename);
    system(cmd);
    
    FILE *fp = fopen(init_args.filename, "w");
    if(init_args.licenses==_GPL_2_0) {
        fprintf(fp, GPL_2_0, init_args.filename, init_args.introduction, init_args.year, init_args.author, 
                             init_args.filename, init_args.year, init_args.author);
    
}else {
        fprintf(fp, GPL_3_0, init_args.filename, init_args.introduction, init_args.year, init_args.author, 
                             init_args.filename, init_args.year, init_args.author);
    }
    fclose(fp);

    sprintf(cmd, "cat "TMP_FILE" >> %s", init_args.filename);
    system(cmd);
    
    sprintf(cmd, "rm -f "TMP_FILE);
    system(cmd);
}

int main(int argc, char *argv[])
{
    init_argument(argc, argv);
    
    write_license();
    
    return 0;
}
