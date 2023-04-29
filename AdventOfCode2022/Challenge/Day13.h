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

namespace Day13
{
    struct Data : std::variant<std::vector<Data>, int>
    {
    private:
        using base = std::variant<std::vector<Data>, int>;
    public:
        using base::base;
        Data( std::initializer_list<Data> v ) : base( v ) {}
    };

    using Packet = std::vector<Data>;

    std::strong_ordering operator<=>( const Data& data1, const Data& data2 )
    {
        return std::visit(
            [ &data2 ] ( auto&& val1 ) {
                return std::visit(
                    [ &val1 ] ( auto&& val2 ) {
                        return val1 <=> val2;
                    }, data2 );
            }, data1 );
    }

    std::strong_ordering operator<=>( int val1, const std::vector<Data>& val2 )
    {
        return std::vector<Data>{ val1 } <=> val2;
    }

    std::strong_ordering operator<=>( const std::vector<Data>& val1, int val2 )
    {
        return val1 <=> std::vector<Data>{ val2 };
    }

    std::strong_ordering operator<=>( const Packet& packet1, const Packet& packet2 )
    {
        const auto minSize = std::min( packet1.size(), packet2.size() );
        for( size_t i = 0; i < minSize; i++ )
        {
            auto order = packet1[ i ] <=> packet2[ i ];
            if( order != std::strong_ordering::equal )
                return order;
        }

        return packet1.size() <=> packet2.size();
    }

    int parseInt( std::string_view data, size_t& offset )
    {
        auto end = data.find_first_of( "],", offset );
        if( end == std::string_view::npos )
            throw std::runtime_error( "invalid number" );

        auto value = std::stoi( std::string( data.substr( offset, end ) ) );
        offset = end - 1;

        return value;
    }

    std::vector<Data> parseVector( std::string_view data, size_t& offset )
    {
        std::vector<Data> vector;

        for( ; offset < data.size(); offset++ )
        {
            auto val = data[ offset ];
            if( val == '[' )
                vector.push_back( parseVector( data, ++offset ) );
            else if( val == ']' )
                break;
            else if( val == ',' )
                continue;
            else if( val >= '0' && val <= '9' )
                vector.push_back( parseInt( data, offset ) );
            else
                throw std::runtime_error( "invalid character" );
        }

        return vector;
    }

    Packet parsePacket( std::string_view line )
    {
        size_t offset = 1;
        return parseVector( line, offset );
    }

    std::vector<std::pair<Packet, Packet>> parseInput( std::istream& stream )
    {
        std::vector<std::pair<Packet, Packet>> packets;
        for( std::string line; std::getline( stream, line ); )
        {
            if( line.empty() )
                continue;

            auto packet = parsePacket( line );
            std::getline( stream, line );
            auto packet2 = parsePacket( line );

            packets.push_back( { std::move( packet ), std::move( packet2 ) } );
        }

        return packets;
    }

    size_t getSumOfCorrectPackets( const std::vector<std::pair<Packet, Packet>>& packets )
    {
        size_t sum = 0;

        for( size_t i = 0; i < packets.size(); i++ )
        {
            if( packets[ i ].first <= packets[ i ].second )
                sum += i + 1;
        }

        return sum;
    }

    size_t getDecoderKey( const std::vector<std::pair<Packet, Packet>>& data )
    {
        std::vector<Packet> packets;
        packets.reserve( data.size() * 2 + 2 );
        for( auto& [packet1, packet2] : data )
        {
            packets.push_back( packet1 );
            packets.push_back( packet2 );
        }
        Packet startPacket;
        startPacket.push_back( std::vector<Data>{2} );
        Packet endPacket;
        endPacket.push_back( std::vector<Data>{6} );

        packets.push_back( startPacket );
        packets.push_back( endPacket );

        std::ranges::sort( packets );

        auto startPacketIt = std::ranges::find( packets, startPacket );
        auto endPacketIt = std::ranges::find( packets, endPacket );

        return ( std::distance( packets.begin(), endPacketIt ) + 1 ) * ( std::distance( packets.begin(), startPacketIt ) + 1 );
    }

    void execute()
    {
        std::ifstream file( "input/Day13.txt" );
        auto data = parseInput( file );
        fmt::print( "Day 13: sum correct order packets: {}\n", getSumOfCorrectPackets( data ) );
        fmt::print( "Day 13: decoder key: {}\n", getDecoderKey( data ) );
    }
}
