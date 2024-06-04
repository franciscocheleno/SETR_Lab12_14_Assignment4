# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(Lab12_14_IO_Module_Assignment4)

target_sources(app PRIVATE src/main.c)

zephyr_include_directories(adc) #Add this line
target_include_directories(app PRIVATE src/adc) #Add this line
target_sources(app PRIVATE src/adc/adc.c) # Add module c source

zephyr_include_directories(IO) #Add this line
target_include_directories(app PRIVATE src/IO) #Add this line
target_sources(app PRIVATE src/IO/IO.c) # Add module c source

zephyr_include_directories(threads) #Add this line
target_include_directories(app PRIVATE src/threads) #Add this line
target_sources(app PRIVATE src/threads/threads.c) # Add module c source

zephyr_include_directories(uart) #Add this line
target_include_directories(app PRIVATE src/uart) #Add this line
target_sources(app PRIVATE src/uart/uart.c) # Add module c source
