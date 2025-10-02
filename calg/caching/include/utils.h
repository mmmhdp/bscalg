#pragma once

#include <stdio.h>

#define TRUE 1
#define FALSE 0

#define ERROR(fmt, ...)                                                       \
  fprintf (stderr, "%s:%d:%s error: " fmt "\n", __FILE__, __LINE__, __func__, \
           ##__VA_ARGS__)

#define TEST_PASSED(...)                                                      \
  fprintf (                                                                   \
      stdout,                                                                 \
      "\nPassed: %s "                                                         \
      "\n************************************************************\n",     \
      __func__)

#define TEST_BEGIN(...)                                                       \
  fprintf (stdout,                                                            \
           "\n------------------------------------------------------------"   \
           "\nBegin: %s \n",                                                  \
           __func__)

#define TEST_FAILED(fmt, ...)                                                 \
  fprintf (stderr, "Failed %s:%s:%d with error: " fmt "\n", __func__,         \
           __FILE__, __LINE__, ##__VA_ARGS__)
