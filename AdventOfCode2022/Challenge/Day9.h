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

namespace Day9
{
    enum class Direction
    {
        Up,
        Down,
        Left,
        Right
    };

    struct Command
    {
        Direction direction;
        int64_t distance;
    };

    struct Vec2
    {
        int64_t x = 0;
        int64_t y = 0;

        auto operator<=>( const Vec2& other ) const = default;
        Vec2 operator-( const Vec2& other ) const {
            return { x - other.x, y - other.y };
        }
        Vec2 operator+( const Vec2& other ) const {
            return { x + other.x, y + other.y };
        }
    };

    Vec2 clamp( const Vec2& vec, int64_t min, int64_t max ) {
        return { std::clamp( vec.x, min, max ), std::clamp( vec.y, min,max ) };
    }

    Direction toDirection( char c ) {
        switch( c ) {
            case 'U':
                return Direction::Up;
            case 'D':
                return Direction::Down;
            case 'L':
                return Direction::Left;
            case 'R':
                return Direction::Right;
        }
        throw std::runtime_error( "invalid direction" );
    }

    std::vector<Command> parseInput( std::ifstream& stream ) {
        std::vector<Command> commands;
        for( std::string line; std::getline( stream, line ); )
            commands.push_back( { toDirection( line[ 0 ] ), std::stoi( line.substr( 2 ) ) } );

        return commands;
    }

    Vec2 getPositionAfterStep( Direction direction, Vec2 pos ) {
        switch( direction ) {
            case Day9::Direction::Up:
                pos.y += 1;
                break;
            case Day9::Direction::Down:
                pos.y -= 1;
                break;
            case Day9::Direction::Left:
                pos.x -= 1;
                break;
            case Day9::Direction::Right:
                pos.x += 1;
                break;
        }
        return pos;
    }

    bool updateRopeElement( Vec2& ropeElementPos, Vec2 newConnectingPos ) {
        Vec2 diff = newConnectingPos - ropeElementPos;

        if( std::abs( diff.x ) > 1 || std::abs( diff.y ) > 1 ) {
            ropeElementPos = ropeElementPos + clamp( diff, -1ll, 1ll );
            return true;
        }
        return false;
    }

    void updateRope( std::vector<Vec2>& rope, Direction direction, std::set<Vec2>& tailVisits ) {
        rope.front() = getPositionAfterStep( direction, rope.front() );

        for( int64_t i = 1; i < std::ssize( rope ); i++ )
            if( !updateRopeElement( rope[ i ], rope[ i - 1 ] ) )
                return;

        tailVisits.insert( rope.back() );
    }

    void executeRopeStep( std::vector<Vec2>& rope, Command command, std::set<Vec2>& tailVisits ) {
        for( int64_t steps = 0; steps < command.distance; steps++ )
            updateRope( rope, command.direction, tailVisits );
    }

    int64_t getNumTailVisits( const std::vector<Command>& commands, int64_t ropeLength ) {
        std::set<Vec2> tailPositions{ {0,0} };
        std::vector<Vec2> rope( ropeLength, Vec2() );

        for( auto& command : commands )
            executeRopeStep( rope, command, tailPositions );

        return std::ssize( tailPositions );
    }

    void execute() {
        std::ifstream file( "input/Day9.txt" );
        auto commands = parseInput( file );

        fmt::print( "Day9: number of tail visit: {}\n", getNumTailVisits( commands, 2 ) );
        fmt::print( "Day9: number of tail visit for long rope: {}\n", getNumTailVisits( commands, 10 ) );
    }
}
