################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/webhook/statewebhookcredentials.cpp \
../src/webhook/webhookbasiccredentials.cpp 

OBJS += \
./src/webhook/statewebhookcredentials.o \
./src/webhook/webhookbasiccredentials.o 

CPP_DEPS += \
./src/webhook/statewebhookcredentials.d \
./src/webhook/webhookbasiccredentials.d 


# Each subdirectory must supply rules for building sources it contributes
src/webhook/%.o: ../src/webhook/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	$(CXX) -D__DEBUG=1 -DSERVER_VERSION_23 -DUSE_DEPRECATED_EMEV_V1 -D__OPENSSL_TOOLS=1 -D__SSOCKET_WRITE_TO_FILE=$(SSOCKET_WRITE_TO_FILE) -D__BCRYPT=1 -I$(INCMYSQL) -I../src/device -I../src/user -I../src -I$(SSLDIR)/include -I../src/client -O2 -g3 -Wall -fsigned-char -c -fmessage-length=0 -fstack-protector-all -D_FORTIFY_SOURCE=2 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

