# Project Name
TARGET = EffectPedal

# Sources
CPP_SOURCES = EffectPedal.cpp \
              Effects/Overdrive.cpp \
			  Effects/Distortion.cpp \
              Effects/Chorus.cpp \
              Effects/Reverb.cpp \
              Effects/Delay.cpp \
              DSP/StateVariableFilter.cpp \
              DSP/MasterDiffuser.cpp \
              DSP/ReverbFeedback.cpp \
              DSP/Diffusers/DiffuserMath.cpp \
              DSP/Diffusers/Diffuser2.cpp \
              DSP/Diffusers/Diffuser4.cpp \
              DSP/Diffusers/Diffuser8.cpp \
              DSP/Diffusers/Diffuser16.cpp

# Optimization
OPT = -Os

# Library Locations
LIBDAISY_DIR = ../../libDaisy/
DAISYSP_DIR = ../../DaisySP/

# Core location
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core

include $(SYSTEM_FILES_DIR)/Makefile

# Add custom flags AFTER Daisy Makefile
# CFLAGS += -flto
# CPPFLAGS += -flto
# LDFLAGS += -flto