#ifndef FLAG_HPP
#define FLAG_HPP

#include <vector>
#include <string>
#include <stdexcept>

#include "../../../net/handler/nethandler.hpp"

class Flag {
protected:
    std::string name;

    // tack names that can be used on the command line, eg. for --help, "help"
    // would be given
    std::vector<std::string> call_names;

    std::string description;

    bool requires_content = false;
public:
    Flag() {}

    virtual std::string parse(std::string flag, std::string flag_argument, struct SERVER_PARAMS* server_params) = 0;

    virtual bool check_set(struct SERVER_PARAMS* server_params) = 0;

    virtual void set_default(struct SERVER_PARAMS* server_params) = 0;

    std::string get_name() { return name; }
    std::string get_description() { return description; }
    std::vector<std::string> get_call_names() { return call_names; }
    bool needs_content() { return requires_content; }
};

#endif /// !FLAG_HPP