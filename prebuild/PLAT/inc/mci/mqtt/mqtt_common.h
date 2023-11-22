#ifndef __MQTT_COMMON_H_
#define __MQTT_COMMON_H_

#include "mg_common.h"

#include "mg_trace.h"
#include "mg_error.h"
#include "mg_os.h"
#include "mg_mem.h"
#include "mg_sys.h"
#include "mg_utils.h"
#include "mg_sim.h"
#include "mg_opt_socket.h"
#include "mg_socket.h"
#include "mg_ssl.h"

#include "mg_unity_list.h"

#include "mqtt_config.h"
#include "mqtt_os.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MQTT_DO_WHILE0(expr) \
    do                      \
    {                       \
        expr                \
    } while (0)
#define MQTT_ASSERT(expect_true, err) MQTT_DO_WHILE0(if (!(expect_true)) return err;)

#define MQTT_ASSERT_VOID(expect_true) MQTT_DO_WHILE0(if (!(expect_true)) return;)

#define MQTT_OPT_CHECK_OPTLEN(optlen, opttype, err) do { if ((optlen) < sizeof(opttype)) { return err; }}while(0)

#define MQTT_CHECKE_PARAM_RANGE(VAL, MIN, MAX) (((VAL) >= (MIN)) && ((VAL) <= (MAX)))

#define MQTT_UNUSED_ARG(x) (void)x

#define MQTT_RETURN_ERROR(x) { return x; }

#define MQTT_FUNC_ENTRY

#define MQTT_FUNC_EXIT_RC(x)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // End-of __MQTT_COMMON_H_