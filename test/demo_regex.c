#include <crtl/bits/crtl_regex.h>

#include <crtl/string.h>
#include <crtl/crtl_mask.h>
#include <crtl/log.h>

void demo_regex_posix()
{
    int status = 0, i = 0;
    int flag = REG_EXTENDED;
    regmatch_t pmatch[1];
    const size_t nmatch = 1;
    regex_t reg;

#if 1 
    const char *pattern = "^\\w+([-+.]\\w+)*@\\w+([-.]\\w+)*\\.\\w+([-.]\\w+)*$";
    char *buf = "12345-126789@sy-lin.com.com";//success
    // char *buf = "###270401@163.com";//error
#elif 0
    
#endif
    regcomp(&reg, pattern, flag);
    status = regexec(&reg, buf, nmatch, pmatch, 0);
    if(status == REG_NOMATCH){
        printf("no match\n");
    }else if(status == 0){
        printf("match success\n");
        for(i = pmatch[0].rm_so; i < pmatch[0].rm_eo; i++){
            putchar(buf[i]);
        }
        putchar('\n');
    }
    regfree(&reg);
}

void demo_regex_scanf()
{
    
    char num[123];
    scanf("%[^0-9]", &num);
    printf("%s\n", num);

//scanf("%ns", str);
//　　　　表示读取长度为n的字符串
//　　　　输入：123456
//　　　　输出str：123 ( 以scanf("%3s", str);为例 )
//
//scanf("%[a-z]", str);
//　　　　表示读取a-z的小写字母，出现非a-z的小写字母，立即停止读取。
//　　　　输入：abcd123
//　　　　输出str：abcd
//
//scanf("%*[a-z]%s", str);
//　　　　%*[ ]表示过滤掉满足括号内条件的字符串   %[ ]表示读取
//　　　　输入：abcd123
//　　　　输出str：123
//
//scanf("%[^0-9]", str);
//　　　　^表示非，^0-9表示非0-9的一切字符，因此是遇到0-9就立即停止读取。
//　　　　输入：abcd123
//　　　　输出str：abcd
//　　　　PS：%[^\n] 表示 读取回车符以前的所有字符，常用于读取含空格的字符串。
//　　　　　　%[^ ] 表示 读取空格符以前的所有字符。
//
//scanf("%*[^\n]%d", &num);
//　　　　表示过滤掉回车以前所有的字符，并将回车符的下一个数字赋给num。
//　　　　输入：abcd\n12
//　　　　输出num：12
//　　　　PS：%*[^ ]表示过滤空格以前的所有字符
}

int main()
{
    demo_regex_posix();

    
    return 0;
}
