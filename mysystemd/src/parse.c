#include "parse.h"

static int parse_config(char const *const config);

static int get_pointer_config(
    char const *path,
    char **buffer);


int read_config(void)
{
    char *config = NULL;
    int ret = 0;

    if (0 != valid_file(CONFIG_FILE))
    {
        ret = -1;
        dzlog_error("config file not found");
        goto finally;
    }

    if (0 != get_pointer_config(CONFIG_FILE, &config))
    {
        ret = -2;
        dzlog_error("faild get pointer config");
        goto finally;
    }

    if (0 != parse_config(config))
    {
        ret = -3;
        dzlog_error("faild parse_config");
        goto finally;
    }

finally:
    free(config);
    return ret;
}

static int parse_config(char const *const config)
{
    cJSON *config_json = cJSON_Parse(config);
    cJSON *deamons = NULL;
    cJSON *deamon = NULL;
    cJSON *name = NULL;
    cJSON *num_starts = NULL;
    cJSON *args = NULL;
    cJSON *arg = NULL;

    char dem_name[NAME_MAX] = { 0 };
    char dem_args[NAME_MAX] = { 0 };
    int starts_left = 0;
    int ret = 0;

    if (NULL == config_json)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (NULL != error_ptr)
        {
            dzlog_error("Error before: %s\n", error_ptr);
        }
        ret = -1;
        goto finally;
    }

    deamons = cJSON_GetObjectItemCaseSensitive(config_json, "deamons");
    if (NULL == deamons)
    {
        dzlog_error("obj 'deamons' in conf not found");
        goto finally;
    }

    cJSON_ArrayForEach(deamon, deamons)
    {
        name = cJSON_GetObjectItemCaseSensitive(deamon, "name");
        if (cJSON_IsString(name) && (NULL != name->valuestring))
        {
            snprintf(dem_name, NAME_MAX, "%s", name->valuestring);
        }
        else
        {
            break;
        }

        if (0 != valid_dem_unsafe(dem_name))
        {
            dzlog_error("deamon '%s' not valid", dem_name);
            break;
        }

        num_starts = cJSON_GetObjectItemCaseSensitive(deamon, "num starts");
        starts_left = (int) num_starts->valuedouble;

        args = cJSON_GetObjectItemCaseSensitive(deamon, "args");
        cJSON_ArrayForEach(arg, args)
        {
            if (starts_left <= 0)
            {
                break;
            }

            snprintf(dem_args, NAME_MAX, "%s", arg->valuestring);

            if (0 != unit_new_element(dem_name, dem_args))
            {
                dzlog_error("create new unit faild");
            }

            starts_left--;
        }

        dem_args[0] = '\0';
        while (starts_left > 0)
        {
            if (0 != unit_new_element(dem_name, dem_args))
            {
                dzlog_error("create new unit faild");
            }
            starts_left--;
        }
    }

finally:
    cJSON_Delete(config_json);
    return ret;
}

static int get_pointer_config(
    char const *path,
    char **buffer)
{
    size_t size = 0;
    *buffer = NULL;
    FILE *fp = NULL;
    int ret = 0;
    errno = 0;

    if (NULL == path)
    {
        ret = -1;
        goto finally;
    }

    fp = fopen(path, "r");
    if (NULL == fp)
    {
        ret = -2;
        goto finally;
    }

    fseek(fp, 0, SEEK_END);
    size = ftell(fp);

    rewind(fp);

    *buffer = calloc(1, (size + 1) * sizeof(*buffer));
    if (NULL == *buffer)
    {
        ret = -3;
        goto finally;
    }

    if (0 == fread(*buffer, size, 1, fp))
    {
        ret = -4;
        goto finally;
    }

finally:
    fclose(fp);
    return ret;
}

