/*
* The Artificial Intelligence Using Randomness (AIUR) is an AI for StarCraft: Broodwar, 
* aiming to be unpredictable thanks to some stochastic behaviors. 
* Please visit http://code.google.com/p/aiurproject/ for further information.
* 
* Copyright (C) 2011 - 2014 Florian Richoux
*
* This file is part of AIUR.
* AIUR is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.

* AIUR is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.

* You should have received a copy of the GNU General Public License
* along with AIUR.  If not, see http://www.gnu.org/licenses/.
*/

#include <Random.h>

Random::Random()
{
	maxInt	= 0;
	lastInt	= 0;
	randGen.seed( static_cast<unsigned int>(std::time(NULL) + _getpid() ) );
}

Random::Random( int maxInt )
{
	std::vector<double>	proba;
	for( int i = 0; i < maxInt; ++i)
		proba.push_back(1.0 / maxInt);

	this->maxInt	= maxInt;
	lastInt			= 0;
	boost::random::discrete_distribution<>::param_type pt( proba );
	random.param( pt );
	randGen.seed( static_cast<unsigned int>(std::time(NULL) + _getpid() ) );
}

Random::Random( int *data, int numberMoods )
{
	maxInt	= numberMoods;
	lastInt	= 0;
	randGen.seed( static_cast<unsigned int>(std::time(NULL) + _getpid() ) );
	this->setDistribution( data, numberMoods );
}

Random::~Random(){}

std::vector<double> Random::getDistribution()
{
	return random.probabilities();
}

void Random::setDistribution( int *data, int numberMoods )
{
	int numberGames = data[0];
	std::vector<double> distrib;
	double temp;

	for( int i = 0; i < numberMoods; ++i )
	{
		temp = 3*data[i*2+1] - 2*data[i*2+2];
		temp /= numberGames;
		temp += ( 1.0 / numberMoods );
		if( temp < 0 )
			temp = 0;
		distrib.push_back( temp );
	}

	// boost::random::discrete_distribution auto-normalizes proba
	boost::random::discrete_distribution<>::param_type pt( distrib );
	random.param( pt );
}

void Random::setMaxInt( int maxInt )
{
	this->maxInt = maxInt;
	std::vector<double>	proba;

	for( int i = 0; i < maxInt; ++i)
		proba.push_back(1.0 / maxInt);

	boost::random::discrete_distribution<>::param_type pt( proba );
	random.param( pt );
}

/**
* generate a random integer in [0, maxInt[
*/
int Random::nextInt()
{
	lastInt = random( randGen );
	return lastInt;
}

/**
* generate a random integer in [0, maxInt[
* different from lastInt
*/
int Random::nextAnotherInt()
{
	int random;
	int last = lastInt;
	int limit = 0;

	do
	{
		random = nextInt();
		++limit;
	}
	while ( random == last && limit < 100 );

	return random;
}