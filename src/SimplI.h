#pragma once

#include <WString>
#include <map>

namespace SimplI
{
    typedef void (*callback_t)(void);

    int64_t get_value(String name);

    double get_float(String name);

    String get_string(String name);

    bool get_bool(String name);

    bool add_value(String name, int64_t current, int64_t min, int64_t max, bool read_only, callback_t callback = nullptr);

    bool add_float(String name, double current, double min, double max, bool read_only, callback_t callback = nullptr);

    bool add_string(String name, String current, bool read_only, int max_size = -1, callback_t callback = nullptr);

    bool add_bool(String name, bool current, bool read_only, callback_t callback = nullptr);

    bool add_action(String name, callback_t callback);

    String process(String cmd);
}