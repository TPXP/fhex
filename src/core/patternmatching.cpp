#include "patternmatching.h"

PatternMatching::PatternMatching(string path)
{
    std::ifstream configFile(path);

    if (!configFile.good()) {
        cerr << "The file " << path << " is not accessible." << endl;
        return;
    }

    try {
        configFile >> this->jconfig;
    } catch (exception &e) {
        cerr << "Exception occurred while parsing json configuration file:" << endl << e.what() << endl;
    }
}

vector<Match*> PatternMatching::hasMatches(string str) {
    vector<Match*> res;
    for (auto& e : this->jconfig["PatternMatching"]) {
        string tmp = str;
        long pos = 0;
        try {
            std::regex re(e["regex"].get<string>());
            std::smatch match;
            while (regex_search(tmp, match, re)) {
                int len = match.str(0).size();
                res.push_back(new Match(e["color"].get<string>(), e["message"].get<string>(), match.position(0) + pos, len));
                tmp = match.suffix();
                pos += match.position(0) + len;
            }
        } catch (std::regex_error& e) {
            cerr << "Regex syntax error:" << endl << e.what() << endl;
        }
    }
    return res;
}

