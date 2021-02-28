#include "application.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wordexp.h>


int dc_application_run(struct dc_application *app,
                       int                    argc,
                       const char            *argv[],
                       const char            *env[],
                       const char            *config_file_path,
                       void                  *app_data)
{
    struct dc_config *config;
    int               retVal;

    if(app->create_config)
    {
        config = app->create_config();

        if(app->setup_config)
        {
            app->setup_config(config, argc, argv, env, config_file_path);
        }
    }
    else
    {
        config = NULL;
    }

    retVal = app->run(config, app_data);

    if(app->cleanup)
    {
        app->cleanup(config, app_data);
    }

    if(app->destroy_config)
    {
        app->destroy_config(&config);
    }

    return retVal;
}


void expand_path(char **expanded_path, const char *path)
{
    wordexp_t  expanded;
    char      *root;
    size_t     root_length;

    wordexp(path, &expanded, 0);
    root           = *expanded.we_wordv;
    root_length    = strlen(root);
    *expanded_path = malloc((root_length + 1) * sizeof(char));
    sprintf(*expanded_path, "%s", root);
}
