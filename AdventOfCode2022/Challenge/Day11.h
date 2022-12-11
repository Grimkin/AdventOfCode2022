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

namespace Day11
{
    struct Monkey
    {
        std::vector<int64_t> items;
        std::function<int64_t( int64_t )> getNewItemValue;
        std::function<int64_t( int64_t )> getMonkey;
        int64_t testValue;
    };

    std::vector<int64_t> parseItems( std::istream& stream ) {
        std::string itemString;
        std::getline( stream, itemString );

        auto items = itemString
            | ranges::views::drop( 18 )
            | ranges::views::split( ',' )
            | ranges::views::transform( [] ( auto&& rng ) -> int64_t {
                return std::stoi( std::string( &*rng.begin(), ranges::distance( rng ) ) );
                } )
            | ranges::to_vector;
        return items;
    }

    std::function<int64_t( int64_t )> getSquareOperation() {
        return [] ( int64_t value ) { return value * value; };
    }

    std::function<int64_t( int64_t )> getAddConstantOperation( int64_t constant ) {
        return [ constant ] ( int64_t value ) { return value + constant; };
    }

    std::function<int64_t( int64_t )> getMultiplyConstantOperation( int64_t constant ) {
        return [ constant ] ( int64_t value ) { return value * constant; };
    }

    std::function<int64_t( int64_t )> parseOperation( std::istream& stream ) {
        std::string operationString;
        std::getline( stream, operationString );

        auto splitOperations = operationString
            | ranges::views::drop( 19 )
            | ranges::views::split( ' ' )
            | ranges::views::transform( [] ( auto&& rng ) {
                    return std::string( &*rng.begin(), ranges::distance( rng ) );
                } )
            | ranges::to_vector;

        if( splitOperations[2] == "old" )
            return getSquareOperation();

        if( splitOperations[1][0] == '+' )
            return getAddConstantOperation( std::stoll( splitOperations[ 2 ] ) );

        return getMultiplyConstantOperation( std::stoll( splitOperations[ 2 ] ) );
    }

    std::pair<std::function<int64_t( int64_t )>, int64_t> parseTest( std::istream& stream ) {
        std::string testString;
        std::getline( stream, testString );
        int64_t testValue = std::stoll( testString.substr( 21 ) );
        std::getline( stream, testString );
        int64_t trueMonkey = std::stoll( testString.substr( 29 ) );
        std::getline( stream, testString );
        int64_t falseMonkey = std::stoll( testString.substr( 30 ) );

        return { [ = ]( int64_t value ) {
            if( value % testValue == 0 )
                return trueMonkey;

            return falseMonkey;
        }, testValue };
    }

    Monkey parseMonkey( std::istream& stream ) {
        Monkey monkey;

        monkey.items = parseItems( stream );
        monkey.getNewItemValue = parseOperation( stream );
        auto [getMonkey, testValue] = parseTest( stream );
        monkey.getMonkey = std::move( getMonkey );
        monkey.testValue = testValue;

        return monkey;
    }

    std::vector<Monkey> parseInput( std::istream& stream ) {
        std::vector<Monkey> monkeys;
        for( std::string line; std::getline( stream, line ); ) {
            if( line.empty() )
                continue;
            monkeys.push_back( parseMonkey( stream ) );
        }
        return monkeys;
    }

    int64_t getNewItemLevel( int64_t oldItemValue, std::function<int64_t( int64_t )>& getNewValue, int64_t commonDenominator, bool divideByThree ) {
        auto newItemLevel = getNewValue( oldItemValue );
        if( divideByThree )
            newItemLevel /= 3;
        return newItemLevel % commonDenominator;
    }

    void throwItems( Monkey& monkey, std::vector<Monkey>& otherMonkeys, int64_t commonDenominator, bool divideByThree ) {
        for( auto& item : monkey.items ) {
            auto newItemLevel = getNewItemLevel( item, monkey.getNewItemValue, commonDenominator, divideByThree );
            auto otherMonkeyId = monkey.getMonkey( newItemLevel );
            otherMonkeys[ otherMonkeyId ].items.push_back( newItemLevel );
        }
        monkey.items.clear();
    }

    int64_t getTopMonkeysScore( std::vector<Monkey> monkeys, int64_t rounds, int64_t reduceValue, bool divideByThree ) {
        std::vector<int64_t> monkeyScores( monkeys.size() );

        for( int64_t i = 0; i < rounds; i++ ) {
            int64_t monkeyId = 0;
            for( auto& monkey : monkeys ) {
                monkeyScores[ monkeyId++ ] += std::ssize( monkey.items );
                throwItems( monkey, monkeys, reduceValue, divideByThree );
            }
        }

        ranges::nth_element( monkeyScores, monkeyScores.begin() + 2, ranges::greater());
        return monkeyScores[ 0 ] * monkeyScores[ 1 ];
    }

    int64_t getCommonDenominator( const std::vector<Monkey>& monkeys ) {
        return ranges::accumulate( monkeys, 1ll, std::multiplies(), &Monkey::testValue );
    }

    void execute() {
        std::ifstream file( "input/Day11.txt" );
        const auto monkeys = parseInput( file );

        auto commonDenominator = getCommonDenominator( monkeys );

        fmt::print( "Day 11: Score of top monkeys: {}\n", getTopMonkeysScore( monkeys, 20, commonDenominator, true ) );
        fmt::print( "Day 11: Score of top monkeys with much worrying: {}\n", getTopMonkeysScore( monkeys, 10'000, commonDenominator, false ) );
    }
}
