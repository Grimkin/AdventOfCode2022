#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <range/v3/all.hpp>
#include <fmt/core.h>
#include <unordered_set>

namespace Day6
{
    std::string parseInput( std::ifstream& stream ) {
        std::string data;
        std::getline( stream, data );
        return data;
    }

    bool hasOnlyUniqueElements( auto& data ) {
        std::unordered_set<char> set( begin( data ), end( data ) );

        return set.size() == data.size();
    }

    int64_t getStartPacketMarker( const std::string& data, int64_t markerSize ) {
        auto view = data | ranges::views::sliding(markerSize);
        auto it = ranges::find_if( view, [](const auto& v) { return hasOnlyUniqueElements(v); } );

        return it == view.end() ? -1 : it - view.begin() + markerSize;
    }

    void execute() {
        std::ifstream file( "input/Day6.txt" );
        auto data = parseInput( file );

        fmt::print( "Day6: Start of packet with marker size  4: {}\n", getStartPacketMarker( data, 4 ) );
        fmt::print( "Day6: Start of packet with marker size 14: {}\n", getStartPacketMarker( data, 14 ) );
    }
}