#include <string>
#include <memory>
#include <sstream>
#include <vector>
#include "strings.h"
#include <iostream>


std::vector<std::string> split_string(std::string word, char delimiter)
{
    std::vector<std::string> result;
    std::string token;
    std::istringstream tokens(word);

    while (getline(tokens, token, delimiter))
    {
        result.push_back(token);
    }

    return result;
}

uint16_t string_to_int16(std::string word)
{
    int number = std::stoi(word);  // Converter para int

    // Garantir que o número está dentro do intervalo de uint16_t
    if (number < 0 || number > 65535) {
        std::cerr << "Out of range to uint16_t." << std::endl;
        return -1;
    }

    uint16_t result = static_cast<uint16_t>(number);  // Converter para uint16_t
    return result;
}