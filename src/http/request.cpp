#include "http/request.h"
#include "parser/utils.h"

namespace http
{

    std::string Request::get_header(const std::string &key) const
    {
        auto it = headers.find(normalize_header_field(key));
        if (it != headers.end())
        {
            return it->second;
        }
        return "";
    }

    std::string Request::get_query_param(const std::string &key) const
    {
        auto it = query_params.find(key);
        if (it != query_params.end())
        {
            return it->second;
        }
        return "";
    }

} // namespace http
