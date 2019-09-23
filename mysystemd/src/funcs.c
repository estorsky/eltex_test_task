#include <stdio.h>
#include "cJSON.h"
#include <stdlib.h> /*  fopen, fseek, ... */
#include <errno.h>
#include <string.h>
#include <limits.h> // wat

/* int supports_full_hd(const char * const monitor)
{
    const cJSON *resolution = NULL;
    const cJSON *resolutions = NULL;
    const cJSON *name = NULL;
    int status = 0;
    cJSON *monitor_json = cJSON_Parse(monitor);
    if (monitor_json == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        status = 0;
        goto end;
    }

    name = cJSON_GetObjectItemCaseSensitive(monitor_json, "name");
    if (cJSON_IsString(name) && (name->valuestring != NULL))
    {
        printf("Checking monitor \"%s\"\n", name->valuestring);
    }

    resolutions = cJSON_GetObjectItemCaseSensitive(monitor_json, "resolutions");
    cJSON_ArrayForEach(resolution, resolutions)
    {
        cJSON *width = cJSON_GetObjectItemCaseSensitive(resolution, "width");
        cJSON *height = cJSON_GetObjectItemCaseSensitive(resolution, "height");

        if (!cJSON_IsNumber(width) || !cJSON_IsNumber(height))
        {
            status = 0;
            goto end;
        }

        if ((width->valuedouble == 1920) && (height->valuedouble == 1080))
        {
            status = 1;
            goto end;
        }
    }

end:
    cJSON_Delete(monitor_json);
    return status;
} */

int get_pointer_config (char **buffer)
{
    size_t size = 0;
    *buffer = NULL;
    FILE *fp = NULL;
    int ret = 0;
    errno = 0;

    /*  Open your_file in read-only mode */
    fp = fopen("monitors.json", "r");
    if (NULL == fp)
    {
        printf("Error! Can't open file config(%s)\n", strerror(errno));
        ret = -1;
        goto exit;
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
        goto exit;
    }

    /*  Read the file into the buffer */
    fread(*buffer, size, 1, fp); /*  Read 1 chunk of size bytes from fp into buffer */

    /*  NULL-terminate the buffer */
    buffer[size] = '\0';

    /*  Print it ! */
    printf("%s\n", *buffer);
    ret = 0;


exit:
    fclose(fp);

    return ret;
}

