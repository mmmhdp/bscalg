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

#ifdef VERBOSE
#define TEST_BEGIN(...)                                                       \
  fprintf (stdout,                                                            \
           "\n------------------------------------------------------------"   \
           "\nBegin: %s \n",                                                  \
           __func__)

#define TEST_FAILED(fmt, ...)                                                 \
  fprintf (stdout, "Failed %s:%s:%d with error: " fmt "\n", __func__,         \
           __FILE__, __LINE__, ##__VA_ARGS__)
#else

#define TEST_BEGIN(...)
#define TEST_FAILED(fmt, ...)

#endif

static int TESTS_CNT __attribute__ ((unused));
static int SUCC_TESTS_CNT __attribute__ ((unused));

#define TEST_PASSED_EXT(...)                                                  \
  do                                                                          \
    {                                                                         \
      SUCC_TESTS_CNT++;                                                       \
      TEST_PASSED (__VA_ARGS__);                                              \
    }                                                                         \
  while (0)

#define TEST_BEGIN_EXT(...)                                                   \
  do                                                                          \
    {                                                                         \
      TESTS_CNT++;                                                            \
      TEST_BEGIN (__VA_ARGS__);                                               \
    }                                                                         \
  while (0)
