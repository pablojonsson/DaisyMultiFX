# Project Name
TARGET = EffectPedal

# Sources
CPP_SOURCES = EffectPedal.cpp \
              Effects/Distortion.cpp \
              Effects/Chorus.cpp \
              Effects/Reverb.cpp \
              DSP/MasterDiffuser.cpp \
              DSP/ReverbFeedback.cpp \
              DSP/Diffusers/Diffuser2.cpp \
              DSP/Diffusers/Diffuser4.cpp \
              DSP/Diffusers/Diffuser8.cpp \
              DSP/Diffusers/Diffuser16.cpp

# Optimization
OPT = -O3

# Library Locations
LIBDAISY_DIR = ../../libDaisy/
DAISYSP_DIR = ../../DaisySP/

# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile