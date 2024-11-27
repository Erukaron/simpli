#include <SimplI.h>

namespace
{
#ifndef NO_SIMPLI_VALUE
    typedef struct value_t
    {
        int64_t current;
        int64_t min;
        int64_t max;
        bool read_only;
        bool write_only; // for passwords
        SimplI::callback_t callback;
    } value_t;

    std::map<String, value_t> values;
#endif

#ifndef NO_SIMPLI_FLOAT
    typedef struct float_tt
    {
        double current;
        double min;
        double max;
        bool read_only;
        SimplI::callback_t callback;
    } float_tt;

    std::map<String, float_tt> floats;
#endif

#ifndef NO_SIMPLI_STRING
    typedef struct string_t
    {
        String current;
        bool read_only;
        bool write_only; // for passwords
        int max_size;
        SimplI::callback_t callback;
    } string_t;

    std::map<String, string_t> strings;
#endif

#ifndef NO_SIMPLI_BOOL
    typedef struct bool_t
    {
        bool current;
        bool read_only;
        SimplI::callback_t callback;
    } bool_t;

    std::map<String, bool_t> bools;
#endif

#ifndef NO_SIMPLI_ACTION
    typedef struct action_t
    {
        SimplI::callback_t callback;
    } action_t;

    std::map<String, action_t> actions;
#endif
}

namespace SimplI
{
    bool check_if_name_exists(String name)
    {
#ifndef NO_SIMPLI_VALUE
        if (values.find(name) != values.end())
        {
            return true;
        }
#endif        

#ifndef NO_SIMPLI_FLOAT
        if (floats.find(name) != floats.end())
        {
            return true;
        }
#endif

#ifndef NO_SIMPLI_STRING
        if (strings.find(name) != strings.end())
        {
            return true;
        }
#endif

#ifndef NO_SIMPLI_BOOL
        if (bools.find(name) != bools.end())
        {
            return true;
        }
#endif

#ifndef NO_SIMPLI_ACTION
        if (actions.find(name) != actions.end())
        {
            return true;
        }
#endif

        return false;
    }

#ifndef NO_SIMPLI_VALUE
    int64_t get_value(String name)
    {
        return values[name].current;
    }

    bool add_value(String name, int64_t current, int64_t min, int64_t max, bool read_only, bool write_only, callback_t callback)
    {
        if (check_if_name_exists(name))
        {
            return false;
        }

        value_t item;
        item.current = current;
        item.min = min;
        item.max = max;
        item.read_only = read_only;
        item.write_only = write_only;
        item.callback = callback;

        auto const result = values.try_emplace(name, item);
        return result.second;
    }
#endif

#ifndef NO_SIMPLI_FLOAT
    double get_float(String name)
    {
        return floats[name].current;
    }

    bool add_float(String name, double current, double min, double max, bool read_only, callback_t callback)
    {
        if (check_if_name_exists(name))
        {
            return false;
        }

        float_tt item;
        item.current = current;
        item.min = min;
        item.max = max;
        item.read_only = read_only;
        item.callback = callback;

        auto const result = floats.try_emplace(name, item);
        return result.second;
    }
#endif

#ifndef NO_SIMPLI_STRING
    String get_string(String name)
    {
        return strings[name].current;
    }

    bool add_string(String name, String current, bool read_only, bool write_only, int max_size, callback_t callback)
    {
        if (check_if_name_exists(name))
        {
            return false;
        }

        string_t item;
        item.current = current;
        item.read_only = read_only;
        item.write_only = write_only;
        item.max_size = max_size;
        item.callback = callback;

        auto const result = strings.try_emplace(name, item);
        return result.second;
    }
#endif

#ifndef NO_SIMPLI_BOOL
    bool get_bool(String name)
    {
        return bools[name].current;
    }

    bool add_bool(String name, bool current, bool read_only, callback_t callback)
    {
        if (check_if_name_exists(name))
        {
            return false;
        }

        bool_t item;
        item.current = current;
        item.read_only = read_only;
        item.callback = callback;

        auto const result = bools.try_emplace(name, item);
        return result.second;
    }
#endif

#ifndef NO_SIMPLI_ACTION
    bool add_action(String name, callback_t callback)
    {
        if (check_if_name_exists(name))
        {
            return false;
        }

        action_t item;
        item.callback = callback;

        auto const result = actions.try_emplace(name, item);
        return result.second;
    }
#endif

    String process(String cmd)
    {
        String answer = "";

        // Ignore cr&lf
        cmd.replace("\r", "");
        cmd.replace("\n", "");

        if (cmd == "")
        {
            return answer;
        }

        int idx = cmd.indexOf('=');
        bool read;
        String name;
        String value_string = "";
        if (idx < 0)
        {
            read = true; // no equals sign
            name = cmd;
        }
        else
        {
            read = false; // write operation -> equals sign
            name = cmd.substring(0, idx);
            value_string = cmd.substring(idx + 1);
        }

        if (name == "help")
        {
            // Display a list of all items and a usage guide -> <name> to read or <name>=<value> to write
            answer.concat("To read a property, write: <propertyname>\r\n");
            answer.concat("To write a property, write: <propertyname>=<value>\r\n");
            answer.concat("\r\n");
            answer.concat("Available properties:\r\n");

#ifndef NO_SIMPLI_VALUE
            for (auto const & property : values)
            {
                answer.concat(property.first);

                if (property.second.read_only)
                {
                    answer.concat(" [readonly]");
                }

                if (property.second.write_only)
                {
                    answer.concat(" [writeonly]");
                }

                answer.concat(": value\r\n");
            }
#endif
#ifndef NO_SIMPLI_FLOAT
            for (auto const & property : floats)
            {
                answer.concat(property.first);

                if (property.second.read_only)
                {
                    answer.concat(" [readonly]");
                }

                answer.concat(": float\r\n");
            }
#endif
#ifndef NO_SIMPLI_STRING
            for (auto const & property : strings)
            {
                answer.concat(property.first);

                if (property.second.read_only)
                {
                    answer.concat(" [readonly]");
                }

                if (property.second.write_only)
                {
                    answer.concat(" [writeonly]");
                }

                answer.concat(": string\r\n");
            }
#endif
#ifndef NO_SIMPLI_BOOL
            for (auto const & property : bools)
            {
                answer.concat(property.first);

                if (property.second.read_only)
                {
                    answer.concat(" [readonly]");
                }

                answer.concat(": bool\r\n");
            }
#endif
#ifndef NO_SIMPLI_ACTION
            for (auto const & property : actions)
            {
                answer.concat(property.first);
                answer.concat(": action\r\n");
            }
#endif

            return answer;
        }

#ifndef NO_SIMPLI_VALUE
        if (values.find(name) != values.end())
        {
            auto & item = values[name];

            if (read)
            {
                if (!item.write_only)
                {
                    answer.concat(item.current);
                }
                else
                {
                    answer.concat("Property is write only.");    
                }
            }
            else if (!read && item.read_only)
            {
                answer.concat("Property is read only.");
            }
            else
            {
                int64_t value = atol(value_string.c_str());
                
                if ((value <= item.max) && (value >= item.min))
                {
                    item.current = value;

                    if (item.callback != nullptr)
                    {
                        item.callback();
                    }
                }
                else
                {
                    answer.concat("Value ");
                    answer.concat(value);
                    answer.concat(" not in range ");
                    answer.concat(item.min);
                    answer.concat(" to ");
                    answer.concat(item.max);
                    answer.concat(".");
                }
            }

            return answer;
        }
#endif

#ifndef NO_SIMPLI_FLOAT
        if (floats.find(name) != floats.end())
        {
            auto & item = floats[name];

            if (read)
            {
                answer.concat(item.current);
            }
            else if (!read && item.read_only)
            {
                answer.concat("Property is read only.");
            }
            else
            {
                double value = atof(value_string.c_str());
                
                if ((value <= item.max) && (value >= item.min))
                {
                    item.current = value;

                    if (item.callback != nullptr)
                    {
                        item.callback();
                    }
                }
                else
                {
                    answer.concat("Value ");
                    answer.concat(value);
                    answer.concat(" not in range ");
                    answer.concat(item.min);
                    answer.concat(" to ");
                    answer.concat(item.max);
                    answer.concat(".");
                }
            }

            return answer;
        }
#endif

#ifndef NO_SIMPLI_STRING
        if (strings.find(name) != strings.end())
        {
            auto & item = strings[name];

            if (read)
            {
                if (!item.write_only)
                {
                    answer.concat(item.current);
                }
                else
                {
                    answer.concat("Property is write only.");    
                }
            }
            else if (!read && item.read_only)
            {
                answer.concat("Property is read only.");
            }
            else
            {
                auto & item = strings[name];

                if ((item.max_size == -1) || (item.max_size >= value_string.length()))
                {
                    item.current = value_string;

                    if (item.callback != nullptr)
                    {
                        item.callback();
                    }
                }
                else
                {
                    answer.concat("String ");
                    answer.concat(value_string);
                    answer.concat(" longer than ");
                    answer.concat(item.max_size);
                    answer.concat(" characters.");
                }
            }

            return answer;
        }
#endif

#ifndef NO_SIMPLI_BOOL
        if (bools.find(name) != bools.end())
        {
            auto & item = bools[name];

            if (read)
            {
                answer.concat(item.current);
            }
            else if (!read && item.read_only)
            {
                answer.concat("Property is read only.");
            }
            else
            {
                bool value;
                if (value_string.equalsIgnoreCase("on") || (value_string == "1") || value_string.equalsIgnoreCase("true") || value_string.equalsIgnoreCase("yes"))
                {
                    value = true;
                }
                else if (value_string.equalsIgnoreCase("off") || (value_string == "0") || value_string.equalsIgnoreCase("false") || value_string.equalsIgnoreCase("no"))
                {
                    value = false;
                }
                else
                {
                    answer.concat(value_string);
                    answer.concat(" is invalid. Valid options: on|off|1|0|true|false|yes|no.");
                    return answer;
                }
                
                item.current = value;

                if (item.callback != nullptr)
                {
                    item.callback();
                }
            }

            return answer;
        }
#endif

#ifndef NO_SIMPLI_ACTION
        if (actions.find(name) != actions.end())
        {
            auto & item = actions[name];

            if (read)
            {
                item.callback();
            }
            else
            {
                answer.concat("Value of actions can not be changed.");
            }

            return answer;
        }
#endif

        answer.concat("Name ");
        answer.concat(name);
        answer.concat(" unknown. Use help for help.");
        return answer;
    }
}