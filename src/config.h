#ifndef DC_APPLICATION_CONFIG_H
#define DC_APPLICATION_CONFIG_H


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
