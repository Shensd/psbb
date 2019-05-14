#ifndef FLAG_HOME_HPP
#define FLAG_HOME_HPP

#include "flag.hpp"

#include "../../../func/filefunctions.hpp"

#define DEFAULT_HOME_DIR "./www/"

class FlagHome : public Flag {
public:
    FlagHome() {
        name = "home";
        call_names = {"home", "h"};
        description = "Home directory for serving page content, defaults to ./www";
        requires_content = true;
    }

    std::string parse(std::string flag, std::string flag_argument, struct SERVER_PARAMS* server_params) {

        if(flag_argument == "") {
            return "Home directory flag set but no argument given.";
        }

        if(fileutils::get_file_exists(flag_argument)) {
            if(server_params->home_dir.at(server_params->home_dir.length()-1) != '/') {
                server_params->home_dir = (std::string)flag_argument + "/";
            } else {
                server_params->home_dir = flag_argument;
            }
        } else {
            return "Given home directory '" + flag_argument + "' does not exist.";
        }
        return std::string("");
    }

    bool check_set(struct SERVER_PARAMS* server_params) {
        return server_params->home_dir != "";
    }

    void set_default(struct SERVER_PARAMS* server_params) {
        server_params->home_dir = DEFAULT_HOME_DIR;
        server_params->uses_default_home_dir = true;
    }
};

#endif // !FLAG_HOME_HPP