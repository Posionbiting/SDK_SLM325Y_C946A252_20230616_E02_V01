#ifndef _APP_ERR_H
#define _APP_ERR_H

#include "mg_error.h"

/*------------------------ error code ------------------------*/
typedef enum APP_ERROR_E
{
    APP_SUCCESS = 0,
    APP_MQTT_CONNECT_FAIL,
} app_error_e;

#endif

