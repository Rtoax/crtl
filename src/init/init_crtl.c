#include <stdlib.h>

#include "crtl/init.h"
#include "crypto/init.h"


/* ��ʼ����־ */
static volatile crtl_boolean crtl_init_flag = crtl_false;


/**
 *  crtl library initializer function
 *  @param argc: argument count
 *  @param argv: argument values
 *              int main(int argc, char *argv[]);
 *  @return error number
 */
int crtl_init(int argc, char **argv)
{
    /* ���� argv���������ö�Ӧ���� */
    crtl_init_parse_config(argc, argv);

    /* ��ʼ�� */
    
    //TODO: ��ʼ��һЩȫ�ֱ���

    /* ��ʼ����־ */
    crtl_init_flag = crtl_true;

    return CRTL_SUCCESS;
}



/**
 *  crtl library exit function
 *  @param status: CRTL_SUCCESS, CRTL_ERROR
 *  @return error number
 */
void crtl_exit(int status)
{
    //TODO
    
    /* ��ʼ����־ */
    crtl_init_flag = crtl_false;
    exit(status);
    
    return;
}

