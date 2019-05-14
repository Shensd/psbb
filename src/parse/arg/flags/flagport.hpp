#ifndef FLAG_PORT_HPP
#define FLAG_PORT_HPP

#include "flag.hpp"

#define DEFAULT_PORT 80

class FlagPort : public Flag {
public:
    FlagPort() {
        name = "port";
        call_names = {"port", "p"};
        description = "Port to listen for server requests on, default is 80";
        requires_content = true;
    }

    std::string parse(std::string flag, std::string flag_argument, struct SERVER_PARAMS* server_params) {
        if(flag_argument == "") {
            return "Port flag set but no argument given.";
        }

        try {
            server_params->port = std::stoi(flag_argument, nullptr); 
        } catch(const std::invalid_argument &ia) {
            return "Port given not a number";
        } catch(const std::out_of_range &oor) {
            return "Port given too large";
        }
        return std::string("");
    }

    bool check_set(struct SERVER_PARAMS* server_params) {
        return server_params->port;
    }

    void set_default(struct SERVER_PARAMS* server_params) {
        server_params->port = DEFAULT_PORT;
        server_params->uses_default_port = true;
    }
};

#endif // !FLAG_PORT_HPP