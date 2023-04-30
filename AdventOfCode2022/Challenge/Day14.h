#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <range/v3/all.hpp>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <set>
#include <optional>
#include <functional>
#include <ranges>
#include <variant>

namespace Day14
{
    struct Vec2
    {
        int x, y;
        auto operator<=>( const Vec2& ) const = default;
    };

    enum class FillType
    {
        Empty,
        Rock,
        Sand
    };

    using Cave = std::map<Vec2, FillType>;

    Vec2 parseVec( const std::string& x, const std::string& y )
    {
        return { std::stoi( x ), std::stoi( y ) };
    }

    void addLine( Cave& cave, const Vec2& start, const Vec2& end )
    {
        if( start.x == end.x )
            for( int y = std::min( start.y, end.y ); y <= std::max( start.y, end.y ); y++ )
                cave[ Vec2{ start.x, y } ] = FillType::Rock;
        else
            for( int x = std::min( start.x, end.x ); x <= std::max( start.x, end.x ); x++ )
                cave[ Vec2{ x, start.y } ] = FillType::Rock;
    }

    void parseLine( Cave& cave, const std::string& line )
    {
        static std::regex regex( R"((\d+),(\d+))" );
        auto begin = std::sregex_iterator( line.begin(), line.end(), regex );
        auto end = std::sregex_iterator();

        std::smatch match = *(begin++);
        Vec2 currentPosition = parseVec( match[ 1 ], match[ 2 ] );

        for( auto i = begin; i != end; ++i )
        {
            match = *i;
            auto endPosition = parseVec( match[ 1 ], match[ 2 ] );

            addLine( cave, currentPosition, endPosition );
            currentPosition = endPosition;
        }
    }

    Cave parseInput( std::istream& stream )
    {
        Cave cave;
        for( std::string line; std::getline( stream, line ); )
            parseLine( cave, line );

        return cave;
    }

    std::optional<Vec2> updateSandPos( Cave& cave, const Vec2& oldPos )
    {
        if( cave[ Vec2{ oldPos.x, oldPos.y + 1 } ] == FillType::Empty )
            return Vec2{ oldPos.x, oldPos.y + 1 };
        else if( cave[ Vec2{ oldPos.x - 1, oldPos.y + 1 } ] == FillType::Empty )
            return Vec2{ oldPos.x - 1, oldPos.y + 1 };
        else if( cave[ Vec2{ oldPos.x + 1, oldPos.y + 1 } ] == FillType::Empty )
            return Vec2{ oldPos.x + 1, oldPos.y + 1 };
        else
            return {};
    }

    bool addSandUntilAbyss( Cave& cave, const Vec2& pos, int maxDepth )
    {
        auto currentPos = pos;
        for( int i = 0; i < maxDepth; i++ )
        {
            auto newPos = updateSandPos( cave, currentPos );
            if( newPos )
                currentPos = *newPos;
            else
            {
                cave[ currentPos ] = FillType::Sand;
                return true;
            }
        }
        return false;
    }

    int getMaxRockDepth( const Cave& cave )
    {
        int maxDepth = 0;
        for( auto& val : cave )
            maxDepth = std::max( val.first.y, maxDepth );
        return maxDepth;
    }

    size_t getNumberOfSandTilOverflow( Cave cave )
    {
        int maxDepth = getMaxRockDepth( cave );
        size_t i = 0;
        for( ; addSandUntilAbyss( cave, { 500,0 }, maxDepth ); i++ );
        return i;
    }

    std::pair<int, int> getWidthBounds( const Cave& cave )
    {
        int min = std::numeric_limits<int>::max();
        int max = 0;

        for( auto& val : cave )
        {
            min = std::min( min, val.first.x );
            max = std::max( min, val.first.x );
        }

        return { min,max };
    }

    bool addSandUntilAtTop( Cave& cave, const Vec2& pos, int floorDepth )
    {
        auto currentPos = pos;
        for( int i = 0; i < floorDepth; i++ )
        {
            auto newPos = updateSandPos( cave, currentPos );
            if( newPos )
                currentPos = *newPos;
            else
            {
                cave[ currentPos ] = FillType::Sand;
                return currentPos.y != 0;
            }
        }
        cave[ currentPos ] = FillType::Sand;

        return true;
    }

    size_t getNumberOfSandTilTop( Cave cave )
    {
        int maxDepth = getMaxRockDepth( cave );
        auto [min, max] = getWidthBounds( cave );

        size_t i = 1;
        for( ; addSandUntilAtTop( cave, { 500,0 }, maxDepth + 1 ); i++ );
        return i;
    }

    void execute()
    {
        std::ifstream file( "input/Day14.txt" );
        auto cave = parseInput( file );

        fmt::print( "Day 14: Number of sand added until overflow: {}\n", getNumberOfSandTilOverflow( cave ) );
        fmt::print( "Day 14: Number of sand added until top: {}\n", getNumberOfSandTilTop( cave ) );
    }
}