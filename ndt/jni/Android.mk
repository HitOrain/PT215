LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

MY_FILES_PATH  :=  $(LOCAL_PATH)/src/
rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))
MY_FILES_SUFFIX := %.c

MY_ALL_FILES := $(foreach src_path,$(MY_FILES_PATH), $(call rwildcard,$(src_path),*.*) )
MY_ALL_FILES := $(MY_ALL_FILES:$(MY_C_PATH)/./%=$(MY_C_PATH)%)
MY_SRC_LIST  := $(filter $(MY_FILES_SUFFIX),$(MY_ALL_FILES)) 
MY_SRC_LIST  := $(MY_SRC_LIST:$(LOCAL_PATH)/%=%)

LOCAL_CFLAGS += -DANDROID
LOCAL_CFLAGS += -std=c99
MY_ALL_DIRS := $(dir $(foreach src_path,$(MY_FILES_PATH), $(call rwildcard,$(src_path),*/) ) )
MY_ALL_DIRS := $(call uniq,$(MY_ALL_DIRS))

LOCAL_SRC_FILES  := $(MY_SRC_LIST)
LOCAL_C_INCLUDES := $(MY_ALL_DIRS)

LOCAL_MODULE := libndt 
include $(BUILD_SHARED_LIBRARY)



# include $(CLEAR_VARS) 
# LOCAL_MODULE := libndt
# LOCAL_SRC_FILES := ./prebuild/libndt.so 
# include $(PREBUILT_SHARED_LIBRARY)



include $(CLEAR_VARS)
LOCAL_SRC_FILES := ./main/main.c ./main/aw8686x_data_process.c ./main/apk_data_io.c
LOCAL_MODULE := ndt_algorithm
LOCAL_CPPFLAGS := -DANDROID 
LOCAL_CFLAGS := -std=c99
LOCAL_LDLIBS += -L$(SYSROOT)/usr/lib -llog
LOCAL_SHARED_LIBRARIES := libndt libc

include $(BUILD_EXECUTABLE)

