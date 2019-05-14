#ifndef FLAG_INDEX_HPP
#define FLAG_INDEX_HPP

#include "flag.hpp"

#include "../../../func/filefunctions.hpp"

#define DEFAULT_INDEX "index.html"

class FlagIndex : public Flag {
public:
    FlagIndex() {
        name = "index";
        call_names = {"index", "i"};
        description = "Site home page, defaults to index.html";
        requires_content = true;
    }

    std::string parse(std::string flag, std::string flag_argument, struct SERVER_PARAMS* server_params) {
        if(flag_argument == "") {
            return "Index flag set but no argument given."; 
        }

        server_params->index = flag_argument;
        
        return std::string("");
    }

    bool check_set(struct SERVER_PARAMS* server_params) {
        return server_params->index != "";
    }

    void set_default(struct SERVER_PARAMS* server_params) {
        server_params->index = DEFAULT_INDEX;
        server_params->uses_default_index = true;
    }
};

#endif // !FLAG_INDEX_HPP