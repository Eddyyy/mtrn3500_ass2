
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>

#define MSG_END_ZEROS 5

using namespace std;

int main(int argc, char ** argv) {
    string line;
    std::ifstream ifs ("Laserdata.txt");
    if (ifs.is_open()) {
        getline(ifs, line);
    }
    //std::cout << line << std::endl;
    std::vector<std::string> result;

    boost::split(result, line, boost::is_any_of(" "));

    bool message_started = false;
    bool message_ended = false;
    int end_zeros = 0;

    std::vector<std::string> extractedMsg;
    std::string prev = "";

    //--Extract One Message--
    std::cout << "Parsing String: I think" << std::endl;
    for (std::string curr : result) {
        if (!message_started) {
            if (curr == "LMDscandata") {
                message_started = true;
                std::cout << prev << "\t" << curr << std::endl;
                extractedMsg.push_back(curr);
            }
        } else if (!message_ended) {
            extractedMsg.push_back(curr);
            std::cout << prev << "\t" << curr << "\t" << end_zeros << std::endl;
            if (curr == "0") {
                end_zeros += 1;
                if (end_zeros >= MSG_END_ZEROS) {
                    message_ended = true;
                } 
            }
            if (end_zeros > 0 and prev == "0" and curr != "0") {
                end_zeros = 0;
            }
        }
        prev = curr;
    }
    std::cout << "Parsed String: Therefore I am" << std::endl;

    //--Extract Values--
    std::cout << "TestVal " << extractedMsg[24] << std::endl;
    std::cout << "TestVal " << std::stoi(extractedMsg[24], 0, 16) << std::endl;
    std::cout << "TestVal " << extractedMsg[24+360] << std::endl;
    std::cout << "TestVal " << extractedMsg[24+361] << std::endl;
    std::cout << "TestVal " << extractedMsg[24+362] << std::endl;
    for (std::string bit: extractedMsg) {
        std::cout << bit << "\t";
    }
    std::cout << std::endl;
    return 0;
}
