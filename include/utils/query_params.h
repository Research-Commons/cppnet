#pragma once

#include <string>
#include <unordered_map>
#include <stdexcept>
#include <optional>
#include <sstream>

namespace util
{

    // Basic string-to-type converter (for int, float, etc.)
    template <typename T>
    std::optional<T> convert(const std::string &s);

    template <>
    inline std::optional<int> convert<int>(const std::string &s)
    {
        try
        {
            size_t pos;
            int value = std::stoi(s, &pos);
            if (pos == s.size())
                return value;
        }
        catch (...)
        {
        }
        return std::nullopt;
    }

    template <>
    inline std::optional<size_t> convert<size_t>(const std::string &s)
    {
        try
        {
            size_t pos;
            size_t value = std::stoul(s, &pos);
            if (pos == s.size())
                return value;
        }
        catch (...)
        {
        }
        return std::nullopt;
    }

    template <>
    inline std::optional<double> convert<double>(const std::string &s)
    {
        try
        {
            size_t pos;
            double value = std::stod(s, &pos);
            if (pos == s.size())
                return value;
        }
        catch (...)
        {
        }
        return std::nullopt;
    }

    // Extract a query parameter as string (return nullopt if missing)
    inline std::optional<std::string>
    get_param(const std::unordered_map<std::string, std::string> &params,
              const std::string &key)
    {
        auto it = params.find(key);
        if (it != params.end())
            return it->second;
        return std::nullopt;
    }

    // Extract a query parameter as int/double/etc.
    // Returns std::nullopt if missing or failed conversion
    template <typename T>
    std::optional<T>
    get_param(const std::unordered_map<std::string, std::string> &params,
              const std::string &key)
    {
        auto strval = get_param(params, key);
        if (strval)
            return convert<T>(*strval);
        return std::nullopt;
    }

    // Get with default fallback. For simple usage: limit = get_with_default(params, "limit", 20);
    template <typename T>
    T get_with_default(const std::unordered_map<std::string, std::string> &params,
                       const std::string &key, T default_val)
    {
        auto maybe_val = get_param<T>(params, key);
        if (maybe_val)
            return *maybe_val;
        return default_val;
    }

} // namespace util
