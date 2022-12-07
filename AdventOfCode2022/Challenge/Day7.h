#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <range/v3/all.hpp>
#include <fmt/core.h>
#include <variant>
#include <optional>
#include <ranges>

namespace Day7
{
    struct ChangeDirectory
    {
        std::string path;
    };

    struct DirectoryInfo
    {
        std::string name;
    };

    struct FileInfo
    {
        std::string name;
        int64_t size = 0;
    };

    using ListResult = std::variant<DirectoryInfo, FileInfo>;

    struct ListDirectory
    {
        std::vector<ListResult> results;
    };

    using Command = std::variant<ChangeDirectory, ListDirectory>;

    struct Directory {
        Directory* parent = nullptr;
        std::map<std::string, Directory> subDirectories;
        std::vector<FileInfo> files;
        std::optional<int64_t> size;
    };

    ListResult parseListResult( const std::string& line ) {
        if( line[ 0 ] == 'd' )
            return DirectoryInfo{ line.substr( 4 ) };

        auto splitLine = line
            | ranges::views::split( ' ' )
            | ranges::views::transform( [] ( auto&& rng ) {
                    return std::string( &*rng.begin(), ranges::distance( rng ) );
                } )
            | ranges::to_vector;

        return FileInfo{ splitLine[ 1 ], std::stoi( splitLine[ 0 ] ) };
    }

    std::vector<Command> parseInput( std::ifstream& stream ) {
        std::vector<Command> commands;
        ListDirectory* currentLS = nullptr;
        for( std::string line; std::getline( stream, line ); ) {
            if( line[ 0 ] != '$' )
                currentLS->results.push_back( parseListResult( line ) );
            else if( line[ 2 ] == 'c' )
                commands.emplace_back( ChangeDirectory{ line.substr( 5 ) } );
            else
                currentLS = &std::get<ListDirectory>( commands.emplace_back( ListDirectory() ) );
        }

        return commands;
    }

    Directory& addOrGetDirectory( Directory& parent, const std::string& name ) {
        auto directory = parent.subDirectories.find( name );
        if( directory != parent.subDirectories.end() )
            return directory->second;

        return parent.subDirectories.emplace( name, Directory{ &parent } ).first->second;
    }

    void parseCommand( const ChangeDirectory& command, Directory*& currentDirectory, Directory& root ) {
        if( command.path == "/" )
            currentDirectory = &root;
        else if( command.path == ".." )
            currentDirectory = currentDirectory->parent;
        else
            currentDirectory = &addOrGetDirectory( *currentDirectory, command.path );
    }

    void parseCommand( const ListDirectory& command, Directory*& currentDirectory, Directory& ) {
        if( !currentDirectory->files.empty() )
            return;

        for( auto& entry : command.results )
        {
            if( auto fileInfo = std::get_if<FileInfo>( &entry ) )
                currentDirectory->files.push_back( *fileInfo );
            else
                addOrGetDirectory( *currentDirectory, std::get<DirectoryInfo>( entry ).name );
        }
    }

    Directory buildTree( const std::vector<Command>&commands) {
        Directory root;
        Directory* currentDirectory = &root;

        for( auto& command : commands )
        {
            std::visit( [&currentDirectory, &root] ( auto& command ) {
                parseCommand( command, currentDirectory, root );
            }, command);
        }

        return root;
    }

    int64_t calculateDirectorySize( Directory& directory ) {
        using ranges::views::transform;
        using ranges::views::values;
        
        if( directory.size )
            return directory.size.value();

        auto fileSizeSum = ranges::accumulate( directory.files, 0ll, ranges::plus(), &FileInfo::size );
        auto directorySizeSum = ranges::accumulate( directory.subDirectories | values | transform( calculateDirectorySize ), 0ll );

        directory.size = fileSizeSum + directorySizeSum;
        return *directory.size;
    }

    int64_t calculateSumOfDirectories( const Directory& directory, int64_t maxSize ) {
        using ranges::views::transform;
        using ranges::views::values;

        int64_t sum = 0;
        if( *directory.size <= maxSize )
            sum += *directory.size;

        sum += ranges::accumulate( directory.subDirectories
            | values
            | transform( [&] ( auto& dir ) { return calculateSumOfDirectories( dir, maxSize ); } )
        , 0ll );

        return sum;
    }

    std::optional<int64_t> getSizeOfDeletedDirectory( const Directory& directory, int64_t sizeNeeded ) {
        using ranges::views::transform;
        using ranges::views::values;
        using ranges::views::filter;

        std::optional<int64_t> directorySize = *directory.size >= sizeNeeded ? directory.size : std::nullopt;

        if( directory.subDirectories.empty() )
            return directorySize;

        auto sizeOfSubdiretories = directory.subDirectories
            | values
            | transform( [&] ( auto& dir ) { return getSizeOfDeletedDirectory( dir, sizeNeeded ); } )
            | filter( [] ( auto size ) { return size.has_value(); } )
            | transform([] ( auto size) { return size.value(); });

        if( sizeOfSubdiretories.begin() == sizeOfSubdiretories.end() )
            return directorySize;

        auto minVal = ranges::min( sizeOfSubdiretories );
        if( !directorySize )
            return minVal;

        return  std::min( *directorySize, minVal );
    }

    void execute() {
        std::ifstream file( "input/Day7.txt" );
        auto commands = parseInput( file );

        Directory tree = buildTree( commands );
        calculateDirectorySize( tree );

        fmt::print( "Day 6: Sum of directories with max size 100'000: {}\n", calculateSumOfDirectories( tree, 100'000 ) );
        
        int64_t bytesToFree = 30'000'000 - ( 70'000'000 - tree.size.value() );
        fmt::print( "Day 6: Size of deleted directory: {}\n", getSizeOfDeletedDirectory( tree, bytesToFree ).value() );
    }
}
