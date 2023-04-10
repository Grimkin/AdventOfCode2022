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

namespace Day12
{
    struct IVec2
    {
        int64_t x = 0;
        int64_t y = 0;
    };

    struct Node
    {
        int height = 0;
        std::vector<Node*> neighbors;
        size_t distance = std::numeric_limits<size_t>::max();
    };

    struct Map
    {
        std::vector<std::unique_ptr<Node>> nodes;
        Node* start = nullptr;
        Node* end = nullptr;
    };

    int getHeight( char value )
    {
        if( value == 'S' )
            return 0;
        if( value == 'E' )
            return 25;
        return value - 'a';
    }

    void createConnection( Node& node1, Node& node2 )
    {
        if( node1.height + 1 >= node2.height )
            node1.neighbors.push_back( &node2 );
        if( node2.height + 1 >= node1.height )
            node2.neighbors.push_back( &node1 );
    };

    void updateHorizontalConnections( std::vector<std::unique_ptr<Node>>& nodes, size_t lineSize )
    {
        const auto offset = nodes.size() - lineSize;
        for( auto neighbors : nodes | std::views::drop( offset ) | std::views::slide( 2 ) )
            createConnection( *neighbors.front(), *neighbors.back() );
    }

    void updateVerticalConnections( std::vector<std::unique_ptr<Node>>& nodes, size_t lineSize )
    {
        const auto offset = nodes.size() - 2 * lineSize;
        for( auto neighbors : nodes | std::views::drop( offset ) | std::views::slide( lineSize + 1 ) )
            createConnection( *neighbors.front(), *neighbors.back() );
    }

    void updateConnections( Map& map, size_t lineSize )
    {
        updateHorizontalConnections( map.nodes, lineSize );

        if( map.nodes.size() >= 2 * lineSize )
            updateVerticalConnections( map.nodes, lineSize );
    }

    void parseLine( Map& map, const std::string& line )
    {
        for( auto& c : line )
        {
            auto& newNode = map.nodes.emplace_back( std::make_unique<Node>( Node{ getHeight( c ) } ) );
            if( c == 'S' )
                map.start = newNode.get();
            else if( c == 'E' )
                map.end = newNode.get();
        }
    }

    Map parseInput( std::istream& stream )
    {
        Map map;
        for( std::string line; std::getline( stream, line ); )
        {
            parseLine( map, line );
            updateConnections( map, line.size() );
        }

        return map;
    }

    void updateNeighbors( Day12::Node* currentNode )
    {
        for( auto neighbor : currentNode->neighbors )
            neighbor->distance = std::min( currentNode->distance + 1, neighbor->distance );
    }

    Node* getCurrentNode( std::vector<Node*>& nodes )
    {
        std::ranges::sort( nodes, [] ( auto& rhs, auto& lhs ) { return rhs->distance > lhs->distance; } );
        auto currentNode = nodes.back();
        nodes.pop_back();
        return currentNode;
    }

    void updateDistances( Map& map )
    {
        std::vector<Node*> workingNodes = map.nodes
            | ranges::views::transform( [ & ] ( auto& node ) { return node.get(); } )
            | ranges::to_vector;

        while( !workingNodes.empty() )
        {
            auto currentNode = getCurrentNode( workingNodes );
            if( currentNode == map.end )
                return;

            updateNeighbors( currentNode );
        }
    }

    void setAnyGroundStart( Map& map )
    {
        for( auto& node : map.nodes | std::views::filter( [] ( auto& node ) { return node->height == 0; } ) )
            node->distance = 0;
    }

    void execute()
    {
        std::ifstream file( "input/Day12.txt" );
        auto map = parseInput( file );

        map.start->distance = 0;
        updateDistances( map );
        fmt::print( "Distance traveled from S: {}\n", map.end->distance );
        setAnyGroundStart( map );
        updateDistances( map );
        fmt::print( "Distance traveled from ground: {}\n", map.end->distance );
    }
}
