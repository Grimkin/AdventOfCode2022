#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <range/v3/all.hpp>
#include <fmt/core.h>
#include <set>
#include <optional>

namespace Day8
{
    struct Vec2
    {
        int64_t x = 0;
        int64_t y = 0;

        auto operator<=>( const Vec2& other ) const = default;
    };
    class TreeMap
    {
        public:
        void addLine( const std::string& line ) {
            if( !width )
                width = line.size();

            auto toInt = [] ( char val ) { return val - '0'; };
            ranges::transform( line, back_inserter( data ), toInt );
            height++;
        }

        char get( Vec2 pos ) const {
            auto& [x, y] = pos;
            if( x > width || x < 0 || y > height || y < 0 )
                throw std::runtime_error( "invalid index" );

            return data[ x + y * width.value() ];
        }
        int64_t getWidth() const {
            return width.value();
        }
        int64_t getHeight() const {
            return height;
        }
        private:
        std::optional<int64_t> width;
        int64_t height = 0;
        std::vector<char> data;
    };

    TreeMap parseInput( std::ifstream& stream ) {
        TreeMap treeMap;
        for( std::string line; std::getline( stream, line ); )
        {
            treeMap.addLine( line );
        }
        return treeMap;
    }

    bool updateVisibleTree( std::set<Vec2>& visibleTrees, const TreeMap& treeMap, Vec2 pos, char& lastTreeHeight ) {
        auto treeHeight = treeMap.get(pos );
        if( treeHeight <= lastTreeHeight )
            return false;

        lastTreeHeight = treeHeight;
        visibleTrees.insert( pos );

        return true;
    }

    void checkVerticalLine( const TreeMap& treeMap, int64_t x, std::set<Vec2>& visibleTrees ) {
        char lastTreeHeight = -1;
        for( int64_t y = 0; y < treeMap.getHeight(); y++ )
            updateVisibleTree( visibleTrees, treeMap, { x,y }, lastTreeHeight );

        lastTreeHeight = -1;
        for( int64_t y = treeMap.getHeight() - 1; y >= 0; y-- )
            updateVisibleTree( visibleTrees, treeMap, { x,y }, lastTreeHeight );
    }

    void checkHorizontalLine( const TreeMap& treeMap, int64_t y, std::set<Vec2>& visibleTrees ) {
        char lastTreeHeight = -1;
        for( int64_t x = 0; x < treeMap.getWidth(); x++ )
            updateVisibleTree( visibleTrees, treeMap, { x,y }, lastTreeHeight );

        lastTreeHeight = -1;
        for( int64_t x = treeMap.getWidth() - 1; x >= 0; x-- )
            updateVisibleTree( visibleTrees, treeMap, { x,y }, lastTreeHeight );
    }

    int64_t getNumVisibleTrees( const TreeMap& treeMap ) {
        std::set<Vec2> visibleTrees;
        for( int64_t x = 0; x < treeMap.getWidth(); x++ )
            checkVerticalLine( treeMap, x, visibleTrees );
        for( int64_t y = 0; y < treeMap.getHeight(); y++ )
            checkHorizontalLine( treeMap, y, visibleTrees );

        return visibleTrees.size();
    }

    int64_t getBottomScore( const TreeMap& treeMap, Vec2 pos ) {
        int64_t score = 0;
        auto treeSize = treeMap.get( pos );
        for( int64_t y = pos.y + 1; y < treeMap.getHeight(); y++ )
        {
            score++;
            if( treeMap.get( { pos.x, y } ) >= treeSize )
                break;
        }
        return score;
    }
    int64_t getTopScore( const TreeMap& treeMap, Vec2 pos ) {
        int64_t score = 0;
        auto treeSize = treeMap.get( pos );
        for( int y = pos.y - 1; y >= 0; y-- )
        {
            score++;
            if( treeMap.get( { pos.x, y } ) >= treeSize )
                break;
        }
        return score;
    }

    int64_t getRightScore( const TreeMap& treeMap, Vec2 pos ) {
        int64_t score = 0;
        auto treeSize = treeMap.get( pos );
        for( int64_t x = pos.x + 1; x < treeMap.getWidth(); x++ )
        {
            score++;
            if( treeMap.get( { x, pos.y } ) >= treeSize )
                break;
        }
        return score;
    }
    int64_t getLeftScore( const TreeMap& treeMap, Vec2 pos ) {
        int64_t score = 0;
        auto treeSize = treeMap.get( pos );
        for( int x = pos.x - 1; x >= 0; x-- )
        {
            score++;
            if( treeMap.get( { x, pos.y } ) >= treeSize )
                break;
        }
        return score;
    }

    int64_t getTreeScore( const TreeMap& treeMap, Vec2 pos ) {
        return getTopScore( treeMap, pos ) *
            getBottomScore( treeMap, pos ) *
            getLeftScore( treeMap, pos ) *
            getRightScore( treeMap, pos );
    }

    int64_t getMaxTreeScore( const TreeMap& treeMap ) {
        int64_t maxScore = 0;
        for( int64_t y = 1; y < treeMap.getHeight() - 1; y++ )
            for( int64_t x = 1; x < treeMap.getWidth() - 1; x++ )
                maxScore = std::max( maxScore, getTreeScore( treeMap, { x,y } ) );

        return maxScore;
    }

    void execute() {
        std::ifstream file( "input/Day8.txt" );
        auto data = parseInput( file );

        fmt::print( "Day 8: Number of visible trees: {}\n", getNumVisibleTrees( data ) );
        fmt::print( "Day 8: Max tree score: {}\n", getMaxTreeScore( data ) );
    }
}