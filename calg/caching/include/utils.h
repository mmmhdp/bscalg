#pragma once

#define ERROR(fmt, ...) \
    fprintf(stderr, "%s:%d: error: " fmt "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)
