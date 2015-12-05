// ITP 365 Fall 2015
// HW7 - IMDB Search
// Name: Johnathan Mai
// Email: johnathm@usc.edu
// Platform: Mac
//
// IDMBData.cpp - Implements IMDBData interface

#include "IMDBData.h"
#include <fstream>
#include <iostream>
#include <queue>

// Do for Part 1
// Function: Default Constructor
// Purpose: Loads in the actor/movie file specified,
// and populates the data into the appropriate hash maps.
// Also constructs the graph with actor relationships.
// Input: Name of file to load (by reference)
// Returns: Nothing
IMDBData::IMDBData(const std::string& fileName)
{
	// TODO for Part 1
    std::ifstream fin(fileName);
    
    if (fin.is_open())
    {
        std::string temp = "";
        std::string currentActor = "";
        while(std::getline(fin, temp))
        {
            if (temp[0] != '|')
            {
                mActorsToMoviesMap.emplace(temp, sEmptyVector); // Creates entry in map with the current actor and an empty vector. vector will be filled soon
                currentActor = temp; // Remembers current actor so we can add the movies to it in the following getlines
            }
            else if (temp[0] == '|')
            {
                std::string currentMovie = temp.substr(1); // Takes out the |
                mActorsToMoviesMap[currentActor].push_back(currentMovie); // Inserts movie into vector.
            }
        }
    }
    
    std::unordered_map<std::string, std::vector<std::string>>::iterator j = mActorsToMoviesMap.begin(); // Iterator to traverse through map in no specific order
    for (j; j != mActorsToMoviesMap.end(); ++j)
    {
        reverseMap((*j).first, (*j).second); // first is the actorName, second is the vector of movies
    }
    
}

// Do for Part 1
// Function: reverseMap
// Purpose: This is a helper function that is used to generate
// the reverse map (movies to actors)
// Input: Name of actor (by reference), vector of their movies (by reference)
// Returns: Nothing
void IMDBData::reverseMap(const std::string& actorName, const std::vector<std::string>& movies)
{
    for (int i = 0; i < movies.size(); i++)
    {
        if (mMoviesToActorsMap.emplace(movies[i], sEmptyVector).second) // If movie didn't exist before in map, create a new key with this movie and go in this loop
        {
            mMoviesToActorsMap[movies[i]].push_back(actorName);
        }
        else // else, it's already created but push_back anyways
        {
            mMoviesToActorsMap[movies[i]].push_back(actorName);
        }
    }
}

// Do for Part 1
// Function: getMoviesFromActor
// Purpose: Given an actor's name, returns a vector containing their movies
// Input: Name of actor (by reference)
// Returns: Vector of movies (by reference)
std::vector<std::string>& IMDBData::getMoviesFromActor(const std::string& actorName)
{
    if (mActorsToMoviesMap.find(actorName) != mActorsToMoviesMap.end())
    {
        return mActorsToMoviesMap[actorName];
    }
    else
    {
        return sEmptyVector;
    }
}

// Do for Part 1
// Function: getActorsFromMovie
// Purpose: Given a movie's name, returns the name of its actors
// Input: Name of movie (by reference)
// Returns: Vector of actors (by reference)
std::vector<std::string>& IMDBData::getActorsFromMovie(const std::string& movieName)
{
    if (mMoviesToActorsMap.find(movieName) != mMoviesToActorsMap.end())
    {
        return mMoviesToActorsMap[movieName];
    }
    else
    {
        return sEmptyVector;
    }
}

// Do for Part 2
// Function: findRelationship
// Purpose: Tries to find a relationship between two actors, using a BFS
// and outputs (to cout) the result of the search.
// Input: Name of two actors to check for a relationship
// Returns: Nothing
void IMDBData::findRelationship(const std::string& fromActor, const std::string& toActor)
{
    if (!mGraph.containsActor(fromActor) || !mGraph.containsActor(toActor)) // Check to see if actors exist in graph
    {
        std::cout << "One of the actors don't exist" << std::endl;
        return;
    }
    
    std::queue<ActorNode*> queue; // BFS queue
    queue.push(mGraph.getActorNode(fromActor)); // pushes first node
    
    
    while (queue.size() > 0)
    {
        ActorNode* currentNode = queue.front(); // BFS queue
        queue.pop();
        
        if (currentNode->mName == toActor) // Solution case
        {
            std::cout << "Found a match!! (" << currentNode->mPath.size() << " hops)" << std::endl << currentNode->mPath.front().getActorName() << " was in..." << std::endl << currentNode->mPath.front().getMovieName();
            // Starts outputting from beginning of mPath list
            
            currentNode->mPath.pop_front();
            
            while (currentNode->mPath.size() > 0) // Reads mPath list one by one from the front and pops them until finished
            {
                std::cout << " with " << currentNode->mPath.front().getActorName() << " who was in..." << std::endl << currentNode->mPath.front().getMovieName();
                currentNode->mPath.pop_front();
            }
            
            std::cout << " with " << toActor << std::endl;
            
            mGraph.clearVisited(); // Clear bools
            return;
        }
        else
        {
            if (currentNode->mIsVisited == true) // Skips node if already visited
            {
            }
            else
            {
                currentNode->mIsVisited = true;
                for (auto e : currentNode->mEdges) // iterate through edges
                {
                    if (e->mOtherActor->mIsVisited == true)
                    {
                    }
                    else
                    {
                        queue.push(e->mOtherActor); // Push adjacent actor
                        if (e->mOtherActor->mPath.size() < 1) // copies mPath to adjacent actor (if mPath size is zero)
                        {
                            e->mOtherActor->mPath = currentNode->mPath;
                            PathPair temp(e->mMovieName, currentNode->mName);
                            e->mOtherActor->mPath.push_back(temp);
                        }
                    }
                }
            }
        }
    }
    
    // If reaches this point, then we went through the whole queue (meaning we went through all connections of first node) and didn't find a match
    
    std::cout << "Didn't find a path.." << std::endl;

	// LEAVE THIS AT THE END OF THE FUNCTION
	mGraph.clearVisited();
}

// For Part 2
// Function: createGraph
// Purpose: This helper function constructs the IMDBGraph from the movie to actors map
// Input: None
// Returns: Nothing
void IMDBData::createGraph()
{
	// DO NOT EDIT THIS FUNCTION
	// For every movie in the actors map...
	for (auto& p : mMoviesToActorsMap)
	{
		const std::string& movieName = p.first;
		// Get the actors for this movie
		const std::vector<std::string>& actors = mMoviesToActorsMap[movieName];

		// Make n^2 edges between these actors
		for (size_t i = 0; i < actors.size(); i++)
		{
			ActorNode* firstActor = mGraph.getActorNode(actors[i]);
			for (size_t j = i + 1; j < actors.size(); j++)
			{
				ActorNode* secondActor = mGraph.getActorNode(actors[j]);
				mGraph.createActorEdge(firstActor, secondActor, movieName);
			}
		}
	}
}

// Leave here! Do not edit!!! Use in part 1
// DO NOT REMOVE THIS LINE
std::vector<std::string> IMDBData::sEmptyVector;
