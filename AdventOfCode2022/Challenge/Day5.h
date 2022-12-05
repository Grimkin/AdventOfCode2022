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
#include <deque>

namespace Day5
{
    using CargoStacks = std::vector<std::deque<char>>;

    struct Move
    {
        int64_t count = 0;
        int64_t from = 0;
        int64_t to = 0;
    };

    struct CargoSetup
    {
        CargoStacks cargo;
        std::vector<Move> moves;
    };

    CargoStacks parseCargo( std::istream& stream ) {
        CargoStacks cargo;

        bool isInitialized = false;
        for( std::string line; std::getline( stream, line ); ) {
            if( !isInitialized ) {
                const auto numStacks = ( line.size() - 3 ) / 4 + 1;
                cargo.resize( numStacks );
                isInitialized = true;
            }

            if( line.empty() )
                break;

            for( int64_t i = 0; i < cargo.size(); i++ ) {
                const auto idx = ( 4 * i ) + 1;

                const auto value = line[ idx ];
                if( value >= 'A' && value <= 'Z' )
                    cargo[ i ].push_front( line[ idx ] );
            }
        }

        return cargo;
    }

    std::vector<Move> parseMoves( std::istream& stream ) {
        static const std::regex regex( R"(move (\d+) from (\d+) to (\d+))" );
        std::smatch match;

        std::vector<Move> moves;
        for( std::string line; std::getline( stream, line ); ) {
            std::regex_match( line, match, regex );

            moves.push_back( { std::stoi( match[ 1 ] ),std::stoi( match[ 2 ] ) - 1,std::stoi( match[ 3 ] ) - 1 } );
        }

        return moves;
    }

    CargoSetup parseInput( std::istream& stream ) {
        return { parseCargo( stream ), parseMoves( stream ) };
    }

    void executeMove( CargoStacks& cargo, const Move& move ) {
        for( int64_t i = 0; i < move.count; i++ )
        {
            cargo[ move.to ].push_back( cargo[ move.from ].back() );
            cargo[ move.from ].pop_back();
        }
    }

    void executeMoves( CargoStacks& cargo, const std::vector<Move>& moves ) {
        for( auto& move : moves )
            executeMove( cargo, move );
    }

    std::string getTopCargo( const CargoStacks& stack ) {
        std::string topCargo;
        for( auto& cargo : stack )
            topCargo += cargo.back();

        return topCargo;
    }

    std::string getTopCargo( const CargoSetup& cargoSetup ) {
        auto workCargo = cargoSetup.cargo;

        executeMoves( workCargo, cargoSetup.moves );

        return getTopCargo( workCargo );
    }

    void executeMoveAdvanced( CargoStacks& cargo, const Move& move ) {
        for( int64_t i = 0; i < move.count; i++ )
            cargo[ move.to ].push_back( *( cargo[ move.from ].end() - ( move.count - i ) ) );
        for( int64_t i = 0; i < move.count; i++ )
            cargo[ move.from ].pop_back();
    }

    void executeMovesAdvanced( CargoStacks& cargo, const std::vector<Move>& moves ) {
        for( auto& move : moves )
            executeMoveAdvanced( cargo, move );
    }

    std::string getTopCargoAdvanced( const CargoSetup& cargoSetup ) {
        auto workCargo = cargoSetup.cargo;

        executeMovesAdvanced( workCargo, cargoSetup.moves );

        return getTopCargo( workCargo );
    }

    void execute() {
        std::ifstream file( "input/Day5.txt" );
        auto cargoSetup = parseInput( file );

        fmt::print( "Day5: Top cargo items: {}\n", getTopCargo( cargoSetup ) );
        fmt::print( "Day5: Top cargo items advanced: {}\n", getTopCargoAdvanced( cargoSetup ) );
    }
}
