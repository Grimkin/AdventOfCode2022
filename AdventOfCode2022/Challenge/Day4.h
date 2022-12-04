#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <span>
#include <numeric>
#include <tuple>
#include <algorithm>
#include <range/v3/all.hpp>
#include <tuple>
#include <map>
#include <fmt/core.h>
#include <regex>

namespace Day4
{
    struct Sections
    {
        int64_t min = 0;
        int64_t max = 0;
    };

    struct CleanPair
    {
        Sections firstSections;
        Sections secondSections;
    };

    std::vector<CleanPair> parseInput( std::istream& stream ) {
        std::vector<CleanPair> data;

        std::regex regex( R"((\d+)-(\d+),(\d+)-(\d+))" );
        std::smatch match;

        for( std::string line; std::getline( stream, line ); ) {
            std::regex_match( line, match, regex );

            data.push_back( { {std::stoi( match[ 1 ] ),std::stoi( match[ 2 ] )},{std::stoi( match[ 3 ] ),std::stoi( match[ 4 ] )} } );
        }

        return data;
    }

    bool isCompleteOverlap( const CleanPair& sectionPair ) {
        return sectionPair.firstSections.min <= sectionPair.secondSections.min &&
            sectionPair.firstSections.max >= sectionPair.secondSections.max ||
            sectionPair.secondSections.min <= sectionPair.firstSections.min &&
            sectionPair.secondSections.max >= sectionPair.firstSections.max;
    }

    int64_t getNumberOfCompleteOverlapping( const std::vector<CleanPair>& sectionPairs ) {
        using ranges::views::transform;

        return ranges::accumulate( sectionPairs | transform( isCompleteOverlap ), 0ll );
    }

    bool isPartlyOverlap( const CleanPair& sectionPair ) {
        return sectionPair.firstSections.max >= sectionPair.secondSections.min &&
            sectionPair.firstSections.min <= sectionPair.secondSections.max || 
            sectionPair.firstSections.min <= sectionPair.secondSections.max &&
            sectionPair.firstSections.max >= sectionPair.secondSections.min;
    }

    int64_t getNumberOfPartlyOverlapping( const std::vector<CleanPair>& sectionPairs ) {
        using ranges::views::transform;

        return ranges::accumulate( sectionPairs | transform( isPartlyOverlap ), 0ll );
    }

    void execute() {
        std::ifstream file( "input/Day4.txt" );
        auto data = parseInput( file );

        fmt::print( "Day4: Number of complete overlaps: {}\n", getNumberOfCompleteOverlapping( data ) );
        fmt::print( "Day4: Number of partly overlaps: {}\n", getNumberOfPartlyOverlapping( data ) );
    }
}