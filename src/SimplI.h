#pragma once

#include <WString.h>
#include <map>

namespace simpli
{
    typedef void (*callback_t)(void);

#ifndef NO_SIMPLI_VALUE
    int64_t & get_value(String name);
    bool add_value(String name, int64_t current, int64_t min, int64_t max, bool read_only = false, bool write_only = false, callback_t callback = nullptr);
#endif

#ifndef NO_SIMPLI_FLOAT
    double & get_float(String name);
    bool add_float(String name, double current, double min, double max, bool read_only, callback_t callback = nullptr);
#endif

#ifndef NO_SIMPLI_STRING
    String & get_string(String name);
    bool add_string(String name, String current, bool read_only = false, bool write_only = false, int max_size = -1, callback_t callback = nullptr);
#endif

#ifndef NO_SIMPLI_BOOL
    bool & get_bool(String name);
    bool add_bool(String name, bool current, bool read_only, callback_t callback = nullptr);
#endif

#ifndef NO_SIMPLI_ACTION
    bool add_action(String name, callback_t callback);
#endif

    String process(String cmd);
}