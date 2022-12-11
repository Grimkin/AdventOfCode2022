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
#include <variant>

namespace Day10
{
    struct NOOP {};
    struct AddOperation
    {
        int64_t value;
    };

    using Operation = std::variant<NOOP, AddOperation>;

    std::vector<Operation> parseInput( std::istream& stream ) {
        std::vector<Operation> operations;
        for( std::string line; std::getline( stream, line ); ) {
            if( line[ 0 ] == 'n' )
                operations.push_back( NOOP{} );
            else
                operations.push_back( AddOperation{ std::stoi( line.substr( 5 ) ) } );
        }
        return operations;
    }

    int64_t executeCommand( const NOOP&, int64_t& ) {
        return 1;
    }

    int64_t executeCommand( const AddOperation& add, int64_t& X ) {
        X += add.value;
        return 2;
    }

    int64_t getSignalStrength( int64_t cycle, int64_t X ) {
        return cycle * X;
    }

    int64_t getSumOfSignalStrengths( const std::vector<Operation>& operations ) {
        constexpr int64_t checkDelta = 40;
        int64_t X = 1;
        int64_t signalCheckTime = 20;
        int64_t signalStrengthSum = 0;
        int64_t cycle = 1;
        for( auto& operation : operations ) {
            auto previousX = X;
            cycle += std::visit( [ &X ] ( auto& operation ) {
                return executeCommand( operation, X );
                }, operation );

            if( cycle > signalCheckTime ) {
                signalStrengthSum += getSignalStrength( signalCheckTime, previousX );
                signalCheckTime += checkDelta;
            }
        }

        return signalStrengthSum;
    }

    constexpr int64_t screenWidth = 40;

    bool isDrawn( int64_t cycle, int64_t X ) {
        const auto pixelPos = ( cycle - 1 ) % screenWidth;
        return std::abs( pixelPos - X ) <= 1;
    }

    char getScreenValue( int64_t cycle, int64_t X ) {
        if( isDrawn( cycle, X ) )
            return char( 219 );

        return ' ';
    }

    void executeCycles( std::string& screen, int64_t& cycle, int64_t cyclesToExecute, int64_t X ) {
        for( int64_t i = 0; i < cyclesToExecute; i++, cycle++ ) {
            if( ( cycle - 1 ) % screenWidth == 0 )
                screen += '\n';
            screen += getScreenValue( cycle, X );
        }
    }

    std::string getScreenAfterOperations( const std::vector<Operation>& operations ) {
        std::string screen;
        int64_t X = 1;
        int64_t cycle = 1;
        for( auto& operation : operations ) {
            const auto previousX = X;
            const auto cyclesExecuted = std::visit( [ &X ] ( auto& operation ) {
                return executeCommand( operation, X );
                }, operation );

            executeCycles( screen, cycle, cyclesExecuted, previousX );
        }
        return screen;
    }

    void execute() {
        std::ifstream file( "input/Day10.txt" );
        auto operations = parseInput( file );

        fmt::print( "Day 10: Sum of signal strengths: {}\n", getSumOfSignalStrengths( operations ) );
        fmt::print( "Day 10: Screen after execution:\n{}\n", getScreenAfterOperations( operations ) );
    }
}