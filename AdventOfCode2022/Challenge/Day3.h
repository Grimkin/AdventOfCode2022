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

namespace Day3
{
    std::vector<std::string> parseInput( std::istream& stream ) {
        std::vector<std::string> data;
        for( std::string line; std::getline( stream, line ); )
            data.emplace_back( std::move( line ) );
        return data;
    }

    char getWronglyPackedItem( const std::string& items ) {
        std::string firstCompartment{ items.begin(), items.begin() + items.size() / 2 };
        std::string secondCompartment{ items.begin() + items.size() / 2, items.end() };

        ranges::sort( firstCompartment );
        ranges::sort( secondCompartment );

        auto commonItems = ranges::views::set_intersection( firstCompartment, secondCompartment );

        return *commonItems.begin();
    }

    int64_t getPriorityValue( char item ) {
        if( item >= 'a' && item <= 'z' )
            return item - 'a' + 1;

        return item - 'A' + 27;
    }

    int64_t calculateSumOfItems( const std::vector<std::string>& data ) {
        using ranges::views::transform;

        return ranges::accumulate( data | transform( getWronglyPackedItem ) | transform( getPriorityValue ), 0ll );
    }

    char getBadgeItem( ranges::input_range auto&& group ) {
        using ranges::views::transform;
        using ranges::views::set_intersection;

        std::vector<std::string> backpacks;
        backpacks.reserve( group.size() );
        ranges::copy( group, std::back_inserter( backpacks ) );

        ranges::for_each( backpacks, ranges::sort );

        auto commonItems = set_intersection( set_intersection( backpacks[0], backpacks[1] ), backpacks[2]);

        return *commonItems.begin();
    }

    int64_t calculateSumOfBadges( const std::vector<std::string>& data ) {
        using ranges::views::transform;
        using ranges::views::chunk;

        return ranges::accumulate(
               data
             | chunk( 3 )
             | transform( [] ( auto group ) { return getBadgeItem( group );})
             | transform(getPriorityValue),
            0ll);
    }

    void execute() {
        std::ifstream file( "input/Day3.txt" );
        auto data = parseInput( file );

        fmt::print( "Day3: Sum of wrongly packed item priorities: {}\n", calculateSumOfItems( data ) );
        fmt::print( "Day3: Sum of badge priorities: {}\n", calculateSumOfBadges( data ) );
    }
}