#include <string.h>
#include <errno.h>

#include "../../unit_test/tap.h"
#include "../../unit_test/fff.h"
DEFINE_FFF_GLOBALS

#include "../src/units.c"

struct Unit *head = NULL;
char const PATH_DEAMONS[] = "./path/";

void mysystemd_units_test_null_ptr(void);

void mysystemd_units_test_null_ptr(void)
{
    ok((-1 == unit_init_pid(NULL, NULL, 123)), "unit_init_pid: first arg NULL, second arg NULL, ret -1");
    ok((-1 == unit_init_pid(NULL, "test", 123)), "unit_init_pid: first arg NULL ret -1");
    ok((-1 == unit_init_pid("test", NULL, 123)), "unit_init_pid: second arg NULL, ret -1");

    ok((-1 == unit_change_answer(NULL, NULL, 123)), "unit_change_answer: first arg NULL, second arg NULL, ret -1");
    ok((-1 == unit_change_answer(NULL, "test", 123)), "unit_change_answer: first arg NULL ret -1");
    ok((-1 == unit_change_answer("test", NULL, 123)), "unit_change_answer: second arg NULL, ret -1");

    ok((-1 == unit_new_element(NULL, NULL)), "unit_new_element: first arg NULL, second arg NULL, ret -1");
    ok((-1 == unit_new_element(NULL, "test")), "unit_new_element: first arg NULL ret -1");
    ok((-1 == unit_new_element("test", NULL)), "unit_init_pid: second arg NULL, ret -1");
}

int main(
    int   argc,
    char *argv[])
{
    (void)argc;
    int ret = 0;

    if (-1 == dzlog_init("../../unit_test/unit_test.conf", argv[0]))
    {
        ret = -1;
        goto finally;
    }
    
    plan(9);
    mysystemd_units_test_null_ptr();
    done_testing();

finally:
    zlog_fini();
    return ret;
}

