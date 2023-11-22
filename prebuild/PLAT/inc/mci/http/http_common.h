#ifndef __HTTP_COMMON_H_
#define __HTTP_COMMON_H_

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

#include "mg_unity_string.h"
#include "mg_unity_list.h"

#include "http_config.h"
#include "http_os.h"
#include "http_string.h"
#include "http_list.h"
#include "http_uri.h"
#include "http_tcp_tls.h"

#ifdef __cplusplus
extern "C" {
#endif

#define HTTP_DO_WHILE0(expr) \
    do                      \
    {                       \
        expr                \
    } while (0)
#define HTTP_ASSERT(expect_true, err) HTTP_DO_WHILE0(if (!(expect_true)) return err;)

#define HTTP_ASSERT_VOID(expect_true) HTTP_DO_WHILE0(if (!(expect_true)) return;)

#define HTTP_OPT_CHECK_OPTLEN(optlen, opttype, err) do { if ((optlen) < sizeof(opttype)) { return err; }}while(0)

#define HTTP_CHECKE_PARAM_RANGE(VAL, MIN, MAX) (((VAL) >= (MIN)) && ((VAL) <= (MAX)))

#define HTTP_UNUSED_ARG(x) (void)x

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // End-of __HTTP_COMMON_H_