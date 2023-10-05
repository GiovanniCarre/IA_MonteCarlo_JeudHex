#pragma once

#include <map>
#include <iostream>
#include <cstdint>
#include <string>

class PlayingNode {
public:    
    uint16_t _score;
    std::map<std::pair<int, int>, unsigned int> _children;

    PlayingNode(std::string const & s);
    void print() const;

};