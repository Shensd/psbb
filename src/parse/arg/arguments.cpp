#include "arguments.hpp"

namespace arg {


    namespace {

        /**
         * Converts raw command line arguments to a vector
         * 
         * @param argc length of arguments
         * @param argv arguments themselves
         * @return a vector of strings
         */
        std::vector<std::string> get_arg_vector(int argc, char* argv[]) {
            std::vector<std::string> args(argc);

            for(int i = 0; i < argc; i++) {
                args.push_back(argv[i]);
            }

            return args;
        }

        /**
         * Test if a given flag is a tack flag
         * 
         * @param flag flag to test
         * @return true if tack flag
         */
        bool is_tack_flag(std::string flag) {
            return flag[0] == '-';
        }

        /**
         * Gets the content from a tack flag, essentailly strips tacks from
         * the beginning of a string
         * 
         * @param flag flag to strip tacks from
         * @return string with no leading tacks
         */
        std::string get_tack_flag_content(std::string flag) {

            std::vector<std::string> tokens = stringutils::split(flag, '-');

            // tokens that are just tacks are empty, move through tokens
            // until a token is found with content and mark as beginning
            int i = -1;
            while(!tokens.at(++i).compare("")) {}
            
            tokens = stringutils::subarr(tokens, i, tokens.size());

            return stringutils::join(tokens, "-");;
        }

        /**
         * Convert an array of flags into a map with the call names of each flag
         * corresponding to its flag pointer
         * 
         * @param flags vector of flags to convert to map
         * @return map of flags with call names as keys
         */
        std::map<std::string, Flag*> get_flag_map(std::vector<Flag*> flags) {

            std::map<std::string, Flag*> flag_map;

            for(Flag* flag : flags) {

                for(std::string call_name : flag->get_call_names()) {
                    flag_map.insert(
                        std::pair<std::string, Flag*>(
                            call_name,
                            flag
                        )
                    );
                }

            }

            return flag_map;

        }

    }

    /**
     * Processes given command line arguments into a SERVER_PARAMS struct
     * 
     * @param argc command line number of arguments
     * @param argv command line arguments
     * @param server data structure to read server settings into
     * @param flags array of flag pointers for parsing flags
     * @return empty string if no error, populated string if there was
     */
    std::string process_args(int argc, char* argv[], struct SERVER_PARAMS* server, std::vector<Flag*> flags) {
        // convert raw command line arguments to vector
        std::vector<std::string> args = get_arg_vector(argc, argv);

        // create a map from the given arguments for faster reference
        std::map<std::string, Flag*> flag_map = get_flag_map(flags);

        // first arg is exe name, not used here
        for(int i = 1; i < args.size(); i++) {
            // if it is a tack flag then parse, otherwise ignore
            if(is_tack_flag(args[i])) {
                
                std::string name = get_tack_flag_content(args[i]);

                auto flag_parser = flag_map.find(name);

                // only parse flag if it exists, otherwise ignore
                if(flag_parser != flag_map.end()) {

                    if(flag_parser->second->needs_content() && i >= (args.size() - 1)) {
                        return "Flag " + name + " set but no argument given.";
                    }

                    // pass empty string for content if at end of args
                    std::string content = (i < args.size() - 1) ? args[i+1] : "";

                    // parser will return a populated string if there was an error,
                    // otherwise an empty string is returned
                    std::string error = flag_parser->second->parse(name, content, server);

                    // immedately exit if an error is encountered
                    if(error != "") return error;

                    // eat next token place if the flag needed content (so it isn't)
                    // needlessly parsed
                    if(flag_parser->second->needs_content()) i++;

                } 
            }
        }

        // set defaults for non-set flags
        for(Flag* f : flags) {
            if(!f->check_set(server)) f->set_default(server);
        }

        // return empty string to signify no error
        return std::string("");
    }

}