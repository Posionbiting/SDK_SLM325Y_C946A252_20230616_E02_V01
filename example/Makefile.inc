PROJECT_EXAMPLE_DIR = $(TOP)/example

################################## sys example #################################

### SYS MEM

ifeq ($(strip $(C_APP_PROJECT)),EXAMPLE_SYS_MEM)
CFLAGS += -D__EXAMPLE_SYS_MEM__
EX_INC += $(PROJECT_EXAMPLE_DIR)/sys
EX_SRCS += $(PROJECT_EXAMPLE_DIR)/sys/mg_example_memory.c
endif


### SYS OS

ifeq ($(strip $(C_APP_PROJECT)),EXAMPLE_SYS_OS)
CFLAGS += -D__EXAMPLE_SYS_OS__
EX_INC +=  $(PROJECT_EXAMPLE_DIR)/sys
EX_SRCS += $(PROJECT_EXAMPLE_DIR)/sys/mg_example_os.c
endif


### SYS FS

ifeq ($(strip $(C_APP_PROJECT)),EXAMPLE_SYS_FS)
CFLAGS += -D__EXAMPLE_SYS_FS__
EX_INC += $(PROJECT_EXAMPLE_DIR)/sys
EX_SRCS += $(PROJECT_EXAMPLE_DIR)/sys/mg_example_fs.c
endif


### SYS UTILS

ifeq ($(strip $(C_APP_PROJECT)),EXAMPLE_SYS_UTILS)
CFLAGS += -D__EXAMPLE_SYS_UTILS__
EX_INC += $(PROJECT_EXAMPLE_DIR)/sys
EX_SRCS += $(PROJECT_EXAMPLE_DIR)/sys/mg_example_utils.c
endif


### SYS TIME

ifeq ($(strip $(C_APP_PROJECT)),EXAMPLE_SYS_TIME)
CFLAGS += -D__EXAMPLE_SYS_TIME__
EX_INC += $(PROJECT_EXAMPLE_DIR)/sys
EX_SRCS += $(PROJECT_EXAMPLE_DIR)/sys/mg_example_time.c
endif

### SYS QUEUE

ifeq ($(strip $(C_APP_PROJECT)),EXAMPLE_SYS_QUEUE)
CFLAGS += -D__EXAMPLE_SYS_QUEUE__
EX_INC += $(PROJECT_EXAMPLE_DIR)/sys
EX_SRCS += $(PROJECT_EXAMPLE_DIR)/sys/mg_example_queue.c
endif

### SYS TIMER

ifeq ($(strip $(C_APP_PROJECT)),EXAMPLE_SYS_TIMER)
CFLAGS += -D__EXAMPLE_SYS_TIMER__
EX_INC += $(PROJECT_EXAMPLE_DIR)/sys
EX_SRCS += $(PROJECT_EXAMPLE_DIR)/sys/mg_example_timer.c
endif

### SYS SYS

ifeq ($(strip $(C_APP_PROJECT)),EXAMPLE_SYS_SYS)
CFLAGS += -D__EXAMPLE_SYS_SYS__
EX_INC += $(PROJECT_EXAMPLE_DIR)/sys
EX_SRCS += $(PROJECT_EXAMPLE_DIR)/sys/mg_example_sys.c
endif

################################# net example ##################################

### sim example

ifeq ($(strip $(C_APP_PROJECT)),EXAMPLE_NET_SIM)
CFLAGS += -D__EXAMPLE_NET_SIM__
EX_INC += $(PROJECT_EXAMPLE_DIR)/net
EX_SRCS += $(PROJECT_EXAMPLE_DIR)/net/mg_example_sim.c
endif

### sim hot plug example

ifeq ($(strip $(C_APP_PROJECT)),EXAMPLE_SIM_HOT_PLUG)
CFLAGS += -D__EXAMPLE_NET_SIM__
EX_INC +=  $(PROJECT_EXAMPLE_DIR)/net
EX_SRCS += $(PROJECT_EXAMPLE_DIR)/net/mg_example_sim_hot_plug.c
endif


### network example

ifeq ($(strip $(C_APP_PROJECT)),EXAMPLE_NET_NETWORK)
CFLAGS += -D__EXAMPLE_NET_NETWORK__
EX_INC += $(PROJECT_EXAMPLE_DIR)/net
EX_SRCS += $(PROJECT_EXAMPLE_DIR)/net/mg_example_network.c
endif


################################# apps example #################################

### dns example

ifeq ($(strip $(C_APP_PROJECT)),EXAMPLE_APP_DNS)
CFLAGS += -D__EXAMPLE_APP_DNS__
EX_INC += $(PROJECT_EXAMPLE_DIR)/apps
EX_SRCS += $(PROJECT_EXAMPLE_DIR)/apps/mg_example_dns.c
endif


### tcp example

ifeq ($(strip $(C_APP_PROJECT)),EXAMPLE_APP_TCP_BLOCK)
CFLAGS += -D__EXAMPLE_APP_TCP_BLOCK__
EX_INC += $(PROJECT_EXAMPLE_DIR)/apps
EX_SRCS += $(PROJECT_EXAMPLE_DIR)/apps/mg_example_tcp_block.c
endif

ifeq ($(strip $(C_APP_PROJECT)),EXAMPLE_APP_TCP_SELECT)
CFLAGS += -D__EXAMPLE_APP_TCP_SELECT__
EX_INC += $(PROJECT_EXAMPLE_DIR)/apps
EX_SRCS += $(PROJECT_EXAMPLE_DIR)/apps/mg_example_tcp_select.c
endif


### ntp example

ifeq ($(strip $(C_APP_PROJECT)),EXAMPLE_APP_NTP)
CFLAGS += -D__EXAMPLE_APP_NTP__
EX_INC += $(PROJECT_EXAMPLE_DIR)/apps
EX_SRCS += $(PROJECT_EXAMPLE_DIR)/apps/mg_example_ntp.c
endif

### ssl example

ifeq ($(strip $(C_APP_PROJECT)),EXAMPLE_APP_SSL_BLOCK)
CFLAGS += -D__EXAMPLE_APP_SSL_BLOCK__
EX_INC += $(PROJECT_EXAMPLE_DIR)/apps
EX_SRCS += $(PROJECT_EXAMPLE_DIR)/apps/mg_example_ssl_block.c
endif

ifeq ($(strip $(C_APP_PROJECT)),EXAMPLE_APP_SSL_SELECT)
CFLAGS += -D__EXAMPLE_APP_SSL_SELECT__
EX_INC += $(PROJECT_EXAMPLE_DIR)/apps
EX_SRCS += $(PROJECT_EXAMPLE_DIR)/apps/mg_example_ssl_select.c
endif

### crypto example

ifeq ($(strip $(C_APP_PROJECT)),EXAMPLE_APP_CRYPTO)
CFLAGS += -D__EXAMPLE_APP_CRYPTO__
EX_INC += $(PROJECT_EXAMPLE_DIR)/apps
EX_SRCS += $(PROJECT_EXAMPLE_DIR)/apps/mg_example_crypto.c
endif

### http example

ifeq ($(strip $(C_APP_PROJECT)),EXAMPLE_APP_HTTP)
CFLAGS += -D__EXAMPLE_APP_HTTP__
EX_INC += $(PROJECT_EXAMPLE_DIR)/apps
EX_SRCS += $(PROJECT_EXAMPLE_DIR)/apps/mg_example_http.c
endif

### https example

ifeq ($(strip $(C_APP_PROJECT)),EXAMPLE_APP_HTTPS)
CFLAGS += -D__EXAMPLE_APP_HTTPS__
EX_INC += $(PROJECT_EXAMPLE_DIR)/apps
EX_SRCS += $(PROJECT_EXAMPLE_DIR)/apps/mg_example_https.c
endif

### mqtt example

ifeq ($(strip $(C_APP_PROJECT)),EXAMPLE_APP_MQTT)
CFLAGS += -D__EXAMPLE_APP_MQTT__
EX_INC +=  $(PROJECT_EXAMPLE_DIR)/apps
EX_SRCS += $(PROJECT_EXAMPLE_DIR)/apps/mg_example_mqtt.c
endif

ifeq ($(strip $(C_APP_PROJECT)),EXAMPLE_APP_MQTT_TCP)
CFLAGS += -D__EXAMPLE_APP_MQTT_TCP__
EX_INC +=  $(PROJECT_EXAMPLE_DIR)/apps
EX_SRCS += $(PROJECT_EXAMPLE_DIR)/apps/mg_example_mqtt_ali_tcp.c
endif

ifeq ($(strip $(C_APP_PROJECT)),EXAMPLE_APP_MQTT_TLS)
CFLAGS += -D__EXAMPLE_APP_MQTT_TLS__
EX_INC +=  $(PROJECT_EXAMPLE_DIR)/apps
EX_SRCS += $(PROJECT_EXAMPLE_DIR)/apps/mg_example_mqtt_ali_tls.c
endif

ifeq ($(strip $(C_APP_PROJECT)),EXAMPLE_APP_MQTT_X509)
CFLAGS += -D__EXAMPLE_APP_MQTT_X509__
EX_INC +=  $(PROJECT_EXAMPLE_DIR)/apps
EX_SRCS += $(PROJECT_EXAMPLE_DIR)/apps/mg_example_mqtt_ali_x509.c
endif

################################ fota example ################################

### fota http example

ifeq ($(strip $(C_APP_PROJECT)),EXAMPLE_FOTA_HTTP)
CFLAGS += -D__EXAMPLE_FOTA_HTTP__
EX_INC += $(PROJECT_EXAMPLE_DIR)/fota
EX_SRCS += $(PROJECT_EXAMPLE_DIR)/fota/mg_example_fota_http.c
endif

ifeq ($(strip $(C_APP_PROJECT)),EXAMPLE_FOTA_HTTP_STREAM)
CFLAGS += -D__EXAMPLE_FOTA_HTTP_STREAM__
EX_INC += $(PROJECT_EXAMPLE_DIR)/fota
EX_SRCS += $(PROJECT_EXAMPLE_DIR)/fota/mg_example_fota_http_stream.c
endif

################################ pm example ###################################

### pm example

ifeq ($(strip $(C_APP_PROJECT)),EXAMPLE_PM_PM)
CFLAGS += -D__EXAMPLE_PM_PM__
EX_INC += $(PROJECT_EXAMPLE_DIR)/pm
EX_SRCS += $(PROJECT_EXAMPLE_DIR)/pm/mg_example_pm.c
endif

ifeq ($(strip $(C_APP_PROJECT)),EXAMPLE_PM_TIMER)
CFLAGS += -D__EXAMPLE_PM_TIMER__
EX_INC += $(PROJECT_EXAMPLE_DIR)/pm
EX_SRCS += $(PROJECT_EXAMPLE_DIR)/pm/mg_example_pm_timer.c
endif

ifeq ($(strip $(C_APP_PROJECT)),EXAMPLE_PM_PWR_KEY)
CFLAGS += -D__EXAMPLE_PM_PWR_KEY__
EX_INC += $(PROJECT_EXAMPLE_DIR)/pm
EX_SRCS += $(PROJECT_EXAMPLE_DIR)/pm/mg_example_pm_powerKey.c
endif

ifeq ($(strip $(C_APP_PROJECT)),EXAMPLE_PM_WAKEUP_IN)
CFLAGS += -D__EXAMPLE_PM_WAKEUP_IN__
EX_INC += $(PROJECT_EXAMPLE_DIR)/pm
EX_SRCS += $(PROJECT_EXAMPLE_DIR)/pm/mg_example_pm_wakeupIn.c
endif

ifeq ($(strip $(C_APP_PROJECT)),EXAMPLE_PM_PSM)
CFLAGS += -D__EXAMPLE_PM_PSM__
EX_INC += $(PROJECT_EXAMPLE_DIR)/pm
EX_SRCS += $(PROJECT_EXAMPLE_DIR)/pm/mg_example_pm_psm.c
endif

################################ driver example ################################
ifeq ($(strip $(C_APP_PROJECT)),EXAMPLE_DRV_ADC)
CFLAGS += -D__EXAMPLE_DRV_ADC__
EX_INC +=  $(PROJECT_EXAMPLE_DIR)/driver
EX_SRCS += $(PROJECT_EXAMPLE_DIR)/driver/mg_example_adc.c
endif

ifeq ($(strip $(C_APP_PROJECT)),EXAMPLE_DRV_GPIO)
CFLAGS += -D__EXAMPLE_DRV_GPIO__
EX_INC +=  $(PROJECT_EXAMPLE_DIR)/driver
EX_SRCS += $(PROJECT_EXAMPLE_DIR)/driver/mg_example_gpio.c
endif

ifeq ($(strip $(C_APP_PROJECT)),EXAMPLE_DRV_SPI)
CFLAGS += -D__EXAMPLE_DRV_SPI__
EX_INC +=  $(PROJECT_EXAMPLE_DIR)/driver
EX_SRCS += $(PROJECT_EXAMPLE_DIR)/driver/mg_example_spi.c
endif

ifeq ($(strip $(C_APP_PROJECT)),EXAMPLE_DRV_UART)
CFLAGS += -D__EXAMPLE_DRV_UART__
EX_INC +=  $(PROJECT_EXAMPLE_DIR)/driver
EX_SRCS += $(PROJECT_EXAMPLE_DIR)/driver/mg_example_uart.c
endif

ifeq ($(strip $(C_APP_PROJECT)),EXAMPLE_DRV_I2C)
CFLAGS += -D__EXAMPLE_DRV_I2C__
EX_INC +=  $(PROJECT_EXAMPLE_DIR)/driver
EX_SRCS += $(PROJECT_EXAMPLE_DIR)/driver/mg_example_i2c.c
endif

ifeq ($(strip $(C_APP_PROJECT)),EXAMPLE_DRV_PWM)
CFLAGS += -D__EXAMPLE_DRV_PWM__
EX_INC +=  $(PROJECT_EXAMPLE_DIR)/driver
EX_SRCS += $(PROJECT_EXAMPLE_DIR)/driver/mg_example_pwm.c
endif

ifeq ($(strip $(C_APP_PROJECT)),EXAMPLE_DRV_USRL)
CFLAGS += -D__EXAMPLE_DRV_USRL__
EX_INC +=  $(PROJECT_EXAMPLE_DIR)/driver
EX_SRCS += $(PROJECT_EXAMPLE_DIR)/driver/mg_example_usrl.c
endif

ifeq ($(strip $(C_APP_PROJECT)),EXAMPLE_DRV_GNSS)
CFLAGS += -D__EXAMPLE_DRV_GNSS__
EX_INC +=  $(PROJECT_EXAMPLE_DIR)/driver
EX_SRCS += $(PROJECT_EXAMPLE_DIR)/driver/mg_example_gnss.c
endif