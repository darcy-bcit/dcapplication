#ifndef DC_APPLICATION_CONFIG_H
#define DC_APPLICATION_CONFIG_H


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


#include <netinet/in.h>
#include <stdbool.h>


struct dc_config
{
    bool verbose;
};


void dc_config_setup(struct dc_config *config,
                     int               argc,
                     const char       *argv[],
                     const char       *env[],
                     const char       *config_file_path);
void dc_config_destroy(struct dc_config **config);


#endif // DC_APPLICATION_CONFIG_H
