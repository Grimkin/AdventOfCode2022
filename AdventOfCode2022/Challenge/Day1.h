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

namespace Day1
{
    struct Elf
    {
        std::vector<int64_t> caloriesCarried;
    };

    std::vector<Elf> parseInput( std::istream& stream )
    {
        std::vector<Elf> elves;
        elves.emplace_back();
        for( std::string line; std::getline( stream, line ); )
        {
            if( line.empty() )
                elves.emplace_back();
            else
                elves.back().caloriesCarried.push_back( std::stoi( line ) );
        }
        return elves;
    }
    
    int64_t getSumCalories( const Elf& elf ) {
        return std::accumulate( elf.caloriesCarried.begin(), elf.caloriesCarried.end(), 0ll );
    }

    int64_t getMaxCaloriesCarried( const std::vector<Elf>& elves ) {
        return ranges::max( elves | ranges::views::transform( getSumCalories ) );
    }

    int64_t getTop3SumCalories( const std::vector<Elf>& elves ) {
        std::vector<int64_t> summedValues = elves | ranges::views::transform( getSumCalories ) | ranges::to_vector;

        ranges::nth_element( summedValues, summedValues.begin() + 2, ranges::greater() );
        return ranges::accumulate( summedValues | ranges::views::take( 3 ), 0ll );
    }

    void execute() {
        std::ifstream input( "Input/Day1.txt" );
        auto elves = parseInput( input );

        std::cout << "Day1: max calories elf: " << getMaxCaloriesCarried( elves ) << "\n";
        std::cout << "Day1: sum top 3 calories elf: " << getTop3SumCalories( elves ) << "\n";
    }
}