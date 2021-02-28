#ifndef DC_APPLICATION_APPLICATION_H
#define DC_APPLICATION_APPLICATION_H


#include "config.h"


typedef struct dc_config *(*create_config_func)(void);
typedef void (*destroy_config_func)(struct dc_config **pconfig);
typedef void (*setup_config_func)(struct dc_config *config,
                                  int            argc,
                                  const char    *argv[],
                                  const char    *env[],
                                  const char    *config_file_path);
typedef int (*run_func)(const struct dc_config *config, void *app_data);
typedef void (*cleanup_func)(struct dc_config *config, void *app_data);
void expand_path(char **expanded_path, const char *path);


struct dc_application
{
    create_config_func create_config;
    destroy_config_func destroy_config;
    setup_config_func setup_config;
    run_func run;
    cleanup_func cleanup;
};


int dc_application_run(struct dc_application *app,
                       int                    argc,
                       const char            *argv[],
                       const char            *env[],
                       const char            *config_file_path,
                       void                  *app_data);


#endif // DC_APPLICATION_APPLICATION_H
