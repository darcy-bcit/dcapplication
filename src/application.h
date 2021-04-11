#ifndef DC_APPLICATION_APPLICATION_H
#define DC_APPLICATION_APPLICATION_H


/*
 * Copyright 2021 D'Arcy Smith + the BCIT CST Datacommunications Option students.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


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


void dc_application_init(struct dc_application *app, run_func func);
int dc_application_run(struct dc_application *app,
                       int                    argc,
                       const char            *argv[],
                       const char            *env[],
                       const char            *config_file_path,
                       void                  *app_data,
                       bool                   verbose);


#endif // DC_APPLICATION_APPLICATION_H
