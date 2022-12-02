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

namespace Day2
{
    enum class Shape
    {
        Rock,
        Paper,
        Scissor
    };

    enum class Outcome
    {
        Lose,
        Draw,
        Win
    };

    struct ShapeRound
    {
        Shape oponent;
        Shape player;
    };

    struct StrategyRound
    {
        Shape oponent;
        Outcome strategy;

        auto operator<=>( const StrategyRound& ) const = default;
    };

    using ParsedData = std::vector<std::tuple<char, char>>;
    ParsedData parseInput( std::istream& stream )
    {
        ParsedData data;
        for( std::string line; std::getline( stream, line ); )
        {
            if( line.size() > 2 )
                data.emplace_back( line[ 0 ], line[ 2 ] );
        }
        return data;
    }

    int64_t calculateShapeScore( Shape shape ) {
        using enum Shape;

        switch( shape )
        {
        case Rock:
            return 1;
        case Paper:
            return 2;
        case Scissor:
            return 3;
        }
        throw std::runtime_error( "invalid shape" );
    }

    Shape toShape(char value) {
        using enum Shape;
        switch( value )
        {
        case 'A':
        case 'X':
            return Rock;
        case 'B':
        case 'Y':
            return Paper;
        case 'C':
        case 'Z':
            return Scissor;
        }
        throw std::runtime_error( "invalid shape" );
    }

    ShapeRound toShapeRound( const std::tuple<char, char>& input ) {
        return ShapeRound{ toShape( std::get<0>( input ) ), toShape( std::get<1>( input ) ) };
    }

    int64_t getOutcomeScore( Outcome outcome ) {
        switch( outcome )
        {
        case Outcome::Lose:
            return 0;
        case Outcome::Draw:
            return 3;
        case Outcome::Win:
            return 6;
        }
        throw std::runtime_error( "invalid outcome" );
    }

    Outcome getRoundOutcome( const ShapeRound& round ) {
        using enum Shape;

        if( round.oponent == round.player )
            return Outcome::Draw;

        if( round.oponent == Rock && round.player == Paper ||
            round.oponent == Paper && round.player == Scissor ||
            round.oponent == Scissor && round.player == Rock )
            return Outcome::Win;

        return Outcome::Lose;
    }

    int64_t calculateRoundScore( const ShapeRound& round ) {
        return calculateShapeScore( round.player ) + getOutcomeScore( getRoundOutcome( round ) );
    }

    int64_t calculateScorePart1( const ParsedData& data ) {
        using ranges::views::transform;

        return ranges::accumulate( data | transform( toShapeRound ) | transform( calculateRoundScore ), 0ll);
    }

    Shape getStrategyShape( const StrategyRound& round ) {
        static const std::map<StrategyRound, Shape> strategyMapping = 
        {
            {{Shape::Paper, Outcome::Lose}, Shape::Rock},
            {{Shape::Paper, Outcome::Draw}, Shape::Paper},
            {{Shape::Paper, Outcome::Win}, Shape::Scissor},

            {{Shape::Rock, Outcome::Lose}, Shape::Scissor},
            {{Shape::Rock, Outcome::Draw}, Shape::Rock},
            {{Shape::Rock, Outcome::Win}, Shape::Paper},

            {{Shape::Scissor, Outcome::Lose}, Shape::Paper},
            {{Shape::Scissor, Outcome::Draw}, Shape::Scissor},
            {{Shape::Scissor, Outcome::Win}, Shape::Rock}
        };

        return strategyMapping.at( round );
    }

    Outcome toOutcome( char strategy ) {
        switch( strategy )
        {
        case 'X':
            return Outcome::Lose;
        case 'Y':
            return Outcome::Draw;
        case 'Z':
            return Outcome::Win;
        }
        throw std::runtime_error( "invalid strategy" );
    }

    StrategyRound toStrategyRound( const std::tuple<char, char>& input ) {
        return StrategyRound{ toShape( std::get<0>( input ) ), toOutcome( std::get<1>( input ) ) };
    }

    int64_t calculateRoundScorePart2( const StrategyRound& round ) {
        return getOutcomeScore( round.strategy ) + calculateShapeScore( getStrategyShape( round ) );
    }

    int64_t calculateScorePart2( const ParsedData& data ) {
        using ranges::views::transform;

        return ranges::accumulate( data | transform( toStrategyRound ) | transform( calculateRoundScorePart2 ), 0ll);
    }

    void execute() {
        std::ifstream input( "Input/Day2.txt" );
        auto data = parseInput( input );

        std::cout << "Day2: Final score: " << calculateScorePart1( data ) << "\n";
        std::cout << "Day2: Final score(2): " << calculateScorePart2( data ) << "\n";
    }
}
