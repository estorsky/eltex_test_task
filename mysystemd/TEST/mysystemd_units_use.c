#include <string.h>
#include <errno.h>

#include "../../unit_test/tap.h"
#include "../../unit_test/fff.h"
DEFINE_FFF_GLOBALS
#define TESTING

#include "../src/units.c"

struct Unit *head = NULL;
char const PATH_DEAMONS[] = "./deamons/";
char NAME[NAME_MAX] = { 0 };

FAKE_VALUE_FUNC(
    int,
    kill,
    int,
    int)

FAKE_VALUE_FUNC(
    bool,
    valid_name_and_args,
    char const *,
    char const *,
    char const *,
    char const *)

void mysystemd_units_test_kill(void)
{
    {
        kill_fake.return_val = 0;

        head->pid = -1;
        head->status = STARTED;
        head->non_respons = 3;

        cmp_ok(0, "==", unit_update_non_response(),
            "update non_response success, "
            "kill return 0");
    }

    {
        kill_fake.return_val = -1;

        head->pid = -1;
        head->status = STARTED;
        head->non_respons = 3;

        cmp_ok(0, "==", unit_update_non_response(),
            "update non_response success, "
            "kill return -1");
    }
}

void mysystemd_units_test_valid_name(void)
{
    int pid = 123;
    {
        valid_name_and_args_fake.return_val = true;

        head->pid = 0;
        head->status = STARTED;
        head->non_respons = 3;

        cmp_ok(0, "==", unit_init_pid(NAME, "", pid), "unit_init_pid success");
        ok((pid == head->pid), "pid change");
    }

    {
        valid_name_and_args_fake.return_val = false;

        head->pid = 0;
        head->status = STARTED;
        head->non_respons = 3;

        cmp_ok(0, "==", unit_init_pid(NAME, "", pid), "unit_init_pid success");
        ok((pid != head->pid), "pid not change");
    }
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
    
    plan(7);

    sprintf(NAME, "%s", "test");
    cmp_ok(0, "==", unit_new_element(NAME, "-t"), "create new unit success");
    mysystemd_units_test_kill();
    mysystemd_units_test_valid_name();

    done_testing();

finally:
    zlog_fini();
    return ret;
}

