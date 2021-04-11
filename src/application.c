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


#include "application.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wordexp.h>
#include <dc_fsm/fsm.h>


struct application_environment
{
    struct dc_fsm_environment common;
    struct dc_application *app;
    int argc;
    const char **argv;
    const char **env_vars;
    const char *config_file_path;
    void *app_data;
    struct dc_config *config;
    int result;
};


static int create_config(struct application_environment *env);
static int setup_config(struct application_environment *env);
static int run(struct application_environment *env);
static int cleanup(struct application_environment *env);
static int error(struct application_environment *env);
static int destroy_config(struct application_environment *env);


typedef enum
{
    CREATE_CONFIG = FSM_APP_STATE_START, // 2
    SETUP_CONFIG,                        // 3
    RUN,                                 // 4
    CLEANUP,                             // 5
    DESTROY_CONFIG,                      // 6
    ERROR,                               // 7
} States;


void dc_application_init(struct dc_application *app, run_func func)
{
    memset(app, 0, sizeof(struct dc_application));
    app->run = func;
}


int dc_application_run(struct dc_application *app,
                       int                    argc,
                       const char            *argv[],
                       const char            *env_vars[],
                       const char            *config_file_path,
                       void                  *app_data,
                       bool                   verbose)
{
    struct application_environment *env;
    int                             result;
    int                             start_state;
    int                             end_state;
    static struct state_transition  transitions[] =
            {
                    { FSM_INIT,       CREATE_CONFIG,  (state_func)create_config  },
                    { CREATE_CONFIG,  SETUP_CONFIG,   (state_func)setup_config   },
                    { CREATE_CONFIG,  RUN,            (state_func)run            },
                    { SETUP_CONFIG,   RUN,            (state_func)run            },
                    { RUN,            CLEANUP,        (state_func)cleanup        },
                    { RUN,            DESTROY_CONFIG, (state_func)destroy_config },
                    { RUN,            FSM_EXIT,       NULL                       },
                    { RUN,            ERROR,          (state_func)error          },
                    { CLEANUP,        DESTROY_CONFIG, (state_func)destroy_config },
                    { CLEANUP,        FSM_EXIT,       NULL                       },
                    { DESTROY_CONFIG, FSM_EXIT,       NULL                       },
                    { CREATE_CONFIG,  ERROR,          (state_func)error          },
                    { SETUP_CONFIG,   ERROR,          (state_func)error          },
                    { CLEANUP,        ERROR,          (state_func)error          },
                    { ERROR,          FSM_EXIT,       NULL                       },
                    { FSM_IGNORE,     FSM_IGNORE,     NULL                       },
            };

    start_state           = FSM_INIT;
    end_state             = CREATE_CONFIG;
    env                   = malloc(sizeof(struct application_environment));
    env->common.name      = "Application";
    env->app              = app;
    env->argc             = argc;
    env->argv             = argv;
    env->env_vars         = env_vars;
    env->config_file_path = config_file_path;
    env->app_data         = app_data;
    result                = dc_fsm_run((struct dc_fsm_environment *)env, &start_state, &end_state, transitions, verbose);

    if(result != 0)
    {
        // how do we signal this back to the user?
    }

    return env->result;
}

static int create_config(struct application_environment *env)
{
    int next_state;

    if(env->app->create_config)
    {
        env->config = env->app->create_config();

        if(env->app->setup_config)
        {
            next_state = SETUP_CONFIG;
        }
        else
        {
            next_state = RUN;
        }
    }
    else
    {
        env->config = NULL;
        next_state  = RUN;
    }

    return next_state;
}

static int setup_config(struct application_environment *env)
{
    env->app->setup_config(env->config,
                           env->argc,
                           env->argv,
                           env->env_vars,
                           env->config_file_path);

    return RUN;
}

static int run(struct application_environment *env)
{
    int next_state;

    env->result = env->app->run(env->config,
                                env->app_data);

    if(env->result != 0)
    {
        next_state = ERROR;
    }
    else if(env->app->cleanup)
    {
        next_state = CLEANUP;
    }
    else if(env->app->destroy_config)
    {
        next_state = DESTROY_CONFIG;
    }
    else
    {
        next_state = FSM_EXIT;
    }

    return next_state;
}

static int cleanup(struct application_environment *env)
{
    int next_state;

    env->app->cleanup(env->config, env->app_data);

    if(env->app->destroy_config)
    {
        next_state = DESTROY_CONFIG;
    }
    else
    {
        next_state = FSM_EXIT;
    }

    return next_state;
}

static int destroy_config(struct application_environment *env)
{
    env->app->destroy_config(&env->config);

    return FSM_EXIT;
}

static int error(struct application_environment *env)
{
    (void)env;

    return FSM_EXIT;
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
