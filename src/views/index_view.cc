#include <ctime>

#include "../shiro.hh"
#include "index_page.hh"
#include "index_view.hh"

std::string shiro::views::index::get_view() {
    std::string view = std::string(reinterpret_cast<char*>(shiro_index_html), shiro_index_html_len);

    double difference = std::difftime(std::time(nullptr), start_time);
    std::time_t seconds((time_t) difference);
    std::tm *p = std::gmtime(&seconds);

    char buffer[64];
    std::snprintf(buffer, sizeof(buffer), "%i days %i hours %i minutes %i seconds", p->tm_yday, p->tm_hour, p->tm_min, p->tm_sec);

    size_t index = view.find("{{uptime}}");
    if (index == std::string::npos)
        return std::string();

    view.replace(index, 10, buffer);
    return view;
}
