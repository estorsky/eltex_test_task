#include "parse.h"

static int parse_config(char const *const config);

static int get_pointer_config(
    char const *path,
    char **buffer);


int read_config(void)
{
    char *config = NULL;
    int ret = 0;

    if (valid_file(PATH_CONFIG) != 1)
    {
        ret = -1;
        dzlog_error("config file not found");
        goto finally;
    }

    if (0 != get_pointer_config(PATH_CONFIG, &config))
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

static int parse_config(
    char const *const config)
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
            // fprintf(stderr, "Error before: %s\n", error_ptr);
            dzlog_error("Error before: %s\n", error_ptr);
        }
        ret = -1;
        goto finally;
    }

    deamons = cJSON_GetObjectItemCaseSensitive(config_json, "deamons");
    if (NULL == deamons)
    {
        dzlog_debug("obj 'deamons' in conf not found");
        goto finally;
    }

    cJSON_ArrayForEach(deamon, deamons)
    {
        name = cJSON_GetObjectItemCaseSensitive(deamon, "name");
        if (cJSON_IsString(name) && (NULL != name->valuestring))
        {
            strcpy(dem_name, name->valuestring);
        }
        else
        {
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

            strcpy(dem_args, arg->valuestring);

            unit_new_element(&head, dem_name, dem_args);

            starts_left--;
        }

        strcpy(dem_args, "");
        while (starts_left > 0)
        {
            unit_new_element(&head, dem_name, dem_args);
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

    /*  Open your_file in read-only mode */
    fp = fopen(path, "r");
    if (NULL == fp)
    {
        printf("Error! Can't open file config(%s)\n", strerror(errno));
        ret = -1;
        goto finally;
    }

    /*  Get the buffer size */
    fseek(fp, 0, SEEK_END); /*  Go to end of file */
    size = ftell(fp); /*  How many bytes did we pass ? */

    /*  Set position of stream to the beginning */
    rewind(fp);

    /*  Allocate the buffer (no need to initialize it with calloc) */
    *buffer = malloc((size + 1) * sizeof(*buffer)); /*  size + 1 byte for the \0 */
    if (NULL == *buffer)
    {
        ret = -2;
        goto finally;
    }

    /*  Read the file into the buffer */
    fread(*buffer, size, 1, fp); /*  Read 1 chunk of size bytes from fp into buffer */

    /*  NULL-terminate the buffer */
    buffer[size] = '\0';

    /*  Print it ! */
    // printf("%s\n", *buffer);

finally:
    fclose(fp);
    return ret;
}

