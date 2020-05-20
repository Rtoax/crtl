

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////     ////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////// ///  ///////////////////////////////////////////////////////////////////////////////////////////////////////////
/////// ///  ///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////     ///// //////////////////////////////////////////////////////////////////////////////////////////////////////
///////  //////     ////////////////////////////////////////////////////////////////////////////////////////////////////
/////// /  ////// //////////////////////////////////////////////////////////////////////////////////////////////////////
/////// //  /////  /////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _REGEX_H

#ifdef _REGEX_LARGE_OFFSETS
/* POSIX 1003.1-2008 requires that regoff_t be at least as wide as
   ptrdiff_t and ssize_t.  We don't know of any hosts where ptrdiff_t
   is wider than ssize_t, so ssize_t is safe.  ptrdiff_t is not
   visible here, so use ssize_t.  */
typedef ssize_t regoff_t;
#else
/* The traditional GNU regex implementation mishandles strings longer
   than INT_MAX.  */
typedef int regoff_t;
#endif

typedef struct {
   regoff_t rm_so;
   regoff_t rm_eo;
} regmatch_t;

struct re_pattern_buffer
{
  /* Space that holds the compiled pattern.  The type
     'struct re_dfa_t' is private and is not declared here.  */
  struct re_dfa_t *__REPB_PREFIX(buffer);

  /* Number of bytes to which 'buffer' points.  */
  __re_long_size_t __REPB_PREFIX(allocated);

  /* Number of bytes actually used in 'buffer'.  */
  __re_long_size_t __REPB_PREFIX(used);

  /* Syntax setting with which the pattern was compiled.  */
  reg_syntax_t __REPB_PREFIX(syntax);

  /* Pointer to a fastmap, if any, otherwise zero.  re_search uses the
     fastmap, if there is one, to skip over impossible starting points
     for matches.  */
  char *__REPB_PREFIX(fastmap);

  /* Either a translate table to apply to all characters before
     comparing them, or zero for no translation.  The translation is
     applied to a pattern when it is compiled and to a string when it
     is matched.  */
  __RE_TRANSLATE_TYPE __REPB_PREFIX(translate);

  /* Number of subexpressions found by the compiler.  */
  size_t re_nsub;

  /* Zero if this pattern cannot match the empty string, one else.
     Well, in truth it's used only in 're_search_2', to see whether or
     not we should use the fastmap, so we don't set this absolutely
     perfectly; see 're_compile_fastmap' (the "duplicate" case).  */
  unsigned __REPB_PREFIX(can_be_null) : 1;

  /* If REGS_UNALLOCATED, allocate space in the 'regs' structure
     for 'max (RE_NREGS, re_nsub + 1)' groups.
     If REGS_REALLOCATE, reallocate space if necessary.
     If REGS_FIXED, use what's there.  */
#ifdef __USE_GNU
# define REGS_UNALLOCATED 0
# define REGS_REALLOCATE 1
# define REGS_FIXED 2
#endif
  unsigned __REPB_PREFIX(regs_allocated) : 2;

  /* Set to zero when 're_compile_pattern' compiles a pattern; set to
     one by 're_compile_fastmap' if it updates the fastmap.  */
  unsigned __REPB_PREFIX(fastmap_accurate) : 1;

  /* If set, 're_match_2' does not return information about
     subexpressions.  */
  unsigned __REPB_PREFIX(no_sub) : 1;

  /* If set, a beginning-of-line anchor doesn't match at the beginning
     of the string.  */
  unsigned __REPB_PREFIX(not_bol) : 1;

  /* Similarly for an end-of-line anchor.  */
  unsigned __REPB_PREFIX(not_eol) : 1;

  /* If true, an anchor at a newline matches.  */
  unsigned __REPB_PREFIX(newline_anchor) : 1;
};

typedef struct re_pattern_buffer regex_t;

int regcomp(regex_t *preg, const char *regex, int cflags);

int regexec(const regex_t *preg, const char *string, size_t nmatch, regmatch_t pmatch[], int eflags);

size_t regerror(int errcode, const regex_t *preg, char *errbuf, size_t errbuf_size);

void regfree(regex_t *preg);

//标准的C和C++都不支持正则表达式，但有一些函数库可以辅助C/C++程序员完成这一功能，
//    其中最著名的当数Philip Hazel的Perl-Compatible Regular Expression库，许多Linux发行版本都带有这个函数库。
// 
//C语言处理正则表达式常用的函数有regcomp()、regexec()、regfree()和regerror()，一般分为三个步骤，如下所示：
// 
//C语言中使用正则表达式一般分为三步：
//    编译正则表达式 regcomp()
//    匹配正则表达式 regexec()
//    释放正则表达式 regfree()
// 
//下边是对三个函数的详细解释
//1、int regcomp (regex_t *compiled, const char *pattern, int cflags)
//    这个函数把指定的正则表达式pattern编译成一种特定的数据格式compiled，这样可以使匹配更有效。
//    函数regexec 会使用这个数据在目标文本串中进行模式匹配。执行成功返回０。 　
//     
//    参数说明：
//    ①regex_t 是一个结构体数据类型，用来存放编译后的正则表达式，它的成员re_nsub 用来存储正则表达式中的子正则表达式的个数，
//    子正则表达式就是用圆括号包起来的部分表达式。
//    ②pattern 是指向我们写好的正则表达式的指针。
//    ③cflags 有如下4个值或者是它们或运算(|)后的值：
//    REG_EXTENDED 以功能更加强大的扩展正则表达式的方式进行匹配。
//    REG_ICASE 匹配字母时忽略大小写。
//    REG_NOSUB 不用存储匹配后的结果。
//    REG_NEWLINE 识别换行符，这样'$'就可以从行尾开始匹配，'^'就可以从行的开头开始匹配。
// 
//2. int regexec (regex_t *compiled, char *string, size_t nmatch, regmatch_t matchptr [], int eflags)
//    当我们编译好正则表达式后，就可以用regexec 匹配我们的目标文本串了，
//    如果在编译正则表达式的时候没有指定cflags的参数为REG_NEWLINE，则默认情况下是忽略换行符的，
//    也就是把整个文本串当作一个字符串处理。执行成功返回０。
//    regmatch_t 是一个结构体数据类型，在regex.h中定义：             
//    typedef struct
//    {
//       regoff_t rm_so;
//       regoff_t rm_eo;
//    } regmatch_t;
//    成员rm_so 存放匹配文本串在目标串中的开始位置，rm_eo 存放结束位置。
//        通常我们以数组的形式定义一组这样的结构。因为往往我们的正则表达式中还包含子正则表达式。
//        数组0单元存放主正则表达式位置，后边的单元依次存放子正则表达式位置。
//     
//    参数说明：
//    ①compiled 是已经用regcomp函数编译好的正则表达式。
//    ②string 是目标文本串。
//    ③nmatch 是regmatch_t结构体数组的长度。
//    ④matchptr regmatch_t类型的结构体数组，存放匹配文本串的位置信息。
//    ⑤eflags 有两个值
//    REG_NOTBOL 按我的理解是如果指定了这个值，那么'^'就不会从我们的目标串开始匹配。
//    总之我到现在还不是很明白这个参数的意义；
//    REG_NOTEOL 和上边那个作用差不多，不过这个指定结束end of line。
// 
//3. void regfree (regex_t *compiled)
//    当我们使用完编译好的正则表达式后，或者要重新编译其他正则表达式的时候，
//    我们可以用这个函数清空compiled指向的regex_t结构体的内容，请记住，如果是重新编译的话，一定要先清空regex_t结构体。
//     
//4. size_t regerror (int errcode, regex_t *compiled, char *buffer, size_t length)
//    当执行regcomp 或者regexec 产生错误的时候，就可以调用这个函数而返回一个包含错误信息的字符串。
//     
//    参数说明：
//    ①errcode 是由regcomp 和 regexec 函数返回的错误代号。
//    ②compiled 是已经用regcomp函数编译好的正则表达式，这个值可以为NULL。
//    ③buffer 指向用来存放错误信息的字符串的内存空间。
//    ④length 指明buffer的长度，如果这个错误信息的长度大于这个值，则regerror 函数会自动截断超出的字符串，
//    但他仍然会返回完整的字符串的长度。所以我们可以用如下的方法先得到错误字符串的长度。
//     
//    size_t length = regerror (errcode, compiled, NULL, 0);
//     
//    下边是一个匹配Email例子，按照上面的三步就可以。
#include <stdio.h>
#include <sys/types.h>
#include <regex.h>

int main(void)
{
    int status = 0, i = 0;
    int flag = REG_EXTENDED;
    regmatch_t pmatch[1];
    const size_t nmatch = 1;
    regex_t reg;
    const char *pattern = "^\\w+([-+.]\\w+)*@\\w+([-.]\\w+)*\\.\\w+([-.]\\w+)*$";
    char *buf = "123456789@qq.com";//success
    // char *buf = "###270401@163.com";//error

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

    return 0;
}
#endif //_REGEX_H

