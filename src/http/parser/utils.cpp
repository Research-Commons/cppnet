#include "parser/utils.h"
#include <iomanip>
#include <sstream>

namespace http
{

    std::string url_decode(const std::string &str)
    {
        std::string result;
        result.reserve(str.size());
        for (size_t i = 0; i < str.size(); ++i)
        {
            if (str[i] == '%')
            {
                if (i + 2 < str.size())
                {
                    std::istringstream iss(str.substr(i + 1, 2));
                    int hex = 0;
                    if (iss >> std::hex >> hex)
                    {
                        result += static_cast<char>(hex);
                        i += 2;
                    }
                    else
                    {
                        result += '%';
                    }
                }
                else
                {
                    result += '%';
                }
            }
            else if (str[i] == '+')
            {
                result += ' ';
            }
            else
            {
                result += str[i];
            }
        }
        return result;
    }

    QueryParams parse_query_string(const std::string &query)
    {
        QueryParams params;
        size_t start = 0;
        while (start < query.size())
        {
            size_t end = query.find('&', start);
            if (end == std::string::npos)
            {
                end = query.size();
            }
            size_t eq_pos = query.find('=', start);
            if (eq_pos != std::string::npos && eq_pos < end)
            {
                std::string key = url_decode(query.substr(start, eq_pos - start));
                std::string value = url_decode(query.substr(eq_pos + 1, end - eq_pos - 1));
                params[key] = value;
            }
            else
            {
                std::string key = url_decode(query.substr(start, end - start));
                params[key] = "";
            }
            start = end + 1;
        }
        return params;
    }

    std::string normalize_header_field(const std::string &field)
    {
        std::string result = field;
        std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c)
                       { return std::tolower(c); });
        return result;
    }

    std::string trim(const std::string &s)
    {
        size_t start = 0;
        while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start])))
        {
            ++start;
        }
        if (start == s.size())
        {
            return "";
        }
        size_t end = s.size() - 1;
        while (end > start && std::isspace(static_cast<unsigned char>(s[end])))
        {
            --end;
        }
        return s.substr(start, end - start + 1);
    }

} // namespace http
