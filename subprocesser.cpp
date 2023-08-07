#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>
std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> result;
    bool within_quotes = false;
    std::string current;

    for (char c : s) {
        if (c == '"') {
            within_quotes = !within_quotes;
        }
        else if (c == delim && !within_quotes) {
            if (!current.empty()) {
                result.push_back(current);
                current.clear();
            }
        }
        else {
            current += c;
        }
    }
    if (!current.empty()) {
        result.push_back(current);
    }
    return result;
}


std::string modifyCommand(std::string input) {
    // Find the position of the first non-space character
    size_t first_non_space = input.find_first_not_of(' ');
    if (first_non_space == std::string::npos) {
        return input + "\n";
    }
    // Extract the indentation from the original string
    std::string indentation = input.substr(0, first_non_space);

    // Remove the indentation from the original string
    std::string command = input.substr(first_non_space);

    std::string new_command;
    size_t pos;
    int offset;

    // Check for commands and set the new command and offset accordingly
    if ((pos = command.find("!curl")) != std::string::npos) {
        new_command = "subprocess.call([\"curl\"";
        offset = 5;
    } else if ((pos = command.find("!git clone")) != std::string::npos) {
        new_command = "subprocess.call([\"git\", \"clone\"";
        offset = 10;
    } else if ((pos = command.find("!unzip")) != std::string::npos) {
        new_command = "subprocess.call([\"unzip\"";
        offset = 6;
    } else if ((pos = command.find("!rm")) != std::string::npos) {
        new_command = "subprocess.call([\"rm\"";
        offset = 3;
    } else if ((pos = command.find("!mkdir")) != std::string::npos) {
        new_command = "subprocess.call([\"mkdir\"";
        offset = 6;
    } else if ((pos = command.find("%cd")) != std::string::npos) {
        new_command = "os.chdir(";
        offset = 3;
    } else {
        // If no command found, return the original line
        return input + "\n";
    }

    auto args_str = split(command.substr(pos + offset), ' ');

    if (new_command != "os.chdir(") {
        for (const auto& s : args_str) {
            // Remove the surrounding quotes before appending to new_command
            auto arg = s[0] == '"' ? s.substr(1, s.length() - 2) : s;
            new_command += ", \"" + arg + "\"";
        }
        new_command += "], stdout=subprocess.DEVNULL, stderr=subprocess.STDOUT)";
    } else {
        auto arg = args_str[0][0] == '"' ? args_str[0].substr(1, args_str[0].length() - 2) : args_str[0];
        new_command += "\"" + arg + "\")";  // only one argument for os.chdir
    }

    // Add the indentation back and return the new command
    return indentation + new_command + "\n";
}





int main() {

    using namespace std;

    ifstream inFile("input.txt");
    ofstream outFile("output.txt");

    while(inFile.good()){
        string line;
        getline(inFile,line);
        string output = modifyCommand(line);  // Store the result of 'modifyCommand(line)' in a variable
        if (!output.empty()) {  // Only write the output to the file if it's not an empty string
            outFile << output;
        }
    }
    inFile.close();
    outFile.close();
    return 0;
}
