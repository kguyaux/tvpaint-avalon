/* The tokenize function splits a string, with the option giving of
 * an escape-character. I found a c-version on www but that did
 * not give me the resutls I wanted. The C++ version does, so I use this for now
 * bit weird but the input and output have to be undertood by C, because
 * the main program(plugin) is in C.
*/

#include <iostream>
#include <string>
#include <vector>
#include <debug.h>
#include <string>

// this is cpp-code and will be linked with c-code, so use `extern "C"`
extern "C" int tokenize(char **result, char* inputchar, char seperator, char escape);

int tokenize(char **result, char* inputchar, char seperator, char escape) {
    std::string input(inputchar); //
    std::vector<std::string> output;
    std::string token = "";
    bool inEsc = false;
    for (char ch : input) {
        if (inEsc) {
            inEsc = false;
        }
        else if (ch == escape) {
            inEsc = true;
            continue;
        }
        else if (ch == seperator) {
            output.push_back(token);
            token = "";
            continue;
        }
        token += ch;
    }
    if (inEsc) {
        std::cout << "Error!" << std::endl;
    }
    output.push_back(token);

    // result has to be read from a **char(origin is a c-program), and is passed as a
    // reference.
    // So we have to convert vector with strings(output) to char ** result.
    // (First I thought this would do the trick: `result = &cstrings[0];`)
    std::vector<char*> cstrings;
    cstrings.reserve(output.size());
    for(size_t i = 0; i < output.size(); ++i)
        cstrings.push_back(const_cast<char*>(output[i].c_str()));

    for (size_t i =0; i < output.size(); i++) {
        size_t tokenlength = strlen(cstrings[i])+1;
        char * ctoken = (char*)malloc(tokenlength*sizeof(char));
        strncpy(ctoken, cstrings[i], tokenlength);
        result[i] = ctoken;
    }
    // return number of tokens found
    return static_cast<int>(output.size());
}
