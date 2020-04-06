#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const char GPL_3_0[] = {
"/*    \n"\
"   %s %s\n"\
"   Copyright (C) %s  %s\n"\
"    \n"\
"   This program is free software: you can redistribute it and/or modify\n"\
"   it under the terms of the GNU General Public License as published by\n"\
"   the Free Software Foundation, either version 3 of the License, or\n"\
"   (at your option) any later version.\n"\
"    "\
"   This program is distributed in the hope that it will be useful,\n"\
"   but WITHOUT ANY WARRANTY; without even the implied warranty of\n"\
"   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"\
"   GNU General Public License for more details.\n"\
"    \n"\
"   You should have received a copy of the GNU General Public License\n"\
"   along with this program.  If not, see <http://www.gnu.org/licenses/>.\n"\
"    \n"\
"    \n"\
"    \n"\
"   %s  Copyright (C) %s  %s\n"\
"    \n"\
"   This program comes with ABSOLUTELY NO WARRANTY; for details type `show w'.\n"\
"   This is free software, and you are welcome to redistribute it\n"\
"   under certain conditions; type `show c' for details.\n"\
"*/ \n"\
};

int main(int argc, char *argv[])
{
    if(argc < 5) {
        printf("Usage: %s <filename> <introduction> <year> <author>\n", argv[0]);
        return -1;
    }

    char *filename = argv[1];
    char *introduction = argv[2];
    char *year = argv[3];
    char *author = argv[4];

#if 0
    char line[256] = {0};
    char ch;

    FILE *fp = fopen(filename, "r");
    FILE *fptmp = fopen("/tmp/crtl_tempfile_12312", "w");

    if(!feof(fp)) {
        fscanf(fp, "%c", &ch);
        printf("%c\n", ch);
        fprintf(fptmp, "%c", ch);
    }
    fclose(fp);
    fclose(fptmp);
    
    fp = fopen(filename, "w");
    fptmp = fopen("/tmp/crtl_tempfile_12312", "r");

    fprintf(fp, GPL_3_0, filename, introduction, year, author, filename, year, author);

    if(!feof(fptmp)) {
        fscanf(fptmp, "%c", &ch);
        printf("%c", ch);
        fprintf(fp, "%c", ch);
    }
    
    fclose(fp);
    fclose(fptmp);
#else

#define TMP_FILE "/tmp/crtl_release_tmpfile_12312"
    
    char cmd[256] ={0};
    
    sprintf(cmd, "cat %s > "TMP_FILE, filename);
    system(cmd);
    
    FILE *fp = fopen(filename, "w");
    fprintf(fp, GPL_3_0, filename, introduction, year, author, filename, year, author);
    fclose(fp);

    sprintf(cmd, "cat "TMP_FILE" >> %s", filename);
    system(cmd);
    

#endif

    return 0;
}
