// Hash2018.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <queue>
#include <ostream>
#include <cmath>
#include <array>
#include <set>
#include <map>

using namespace std;

class ride {
	public:
		int startStep;
		int endStep;
		short index;
		short startX;
		short startY;
		short endX;
		short endY;

		ride() {};
		ride(int startStep) { this->startStep = startStep; };

		short getDistance() const {
			short dx = endX - startX;
			short dy = endY - startY;
			if (dx < 0)
				dx = -dx;

			if (dy < 0)
				dy = -dy;

			return dx + dy;
		}

		int getStepDiff() const
		{
			return endStep - startStep;
		}

		short calcDistance(short xPos, short yPos) const
		{
			short dx = xPos - startX;
			short dy = yPos - startY;
			if (dx < 0)
				dx = -dx;

			if (dy < 0)
				dy = -dy;

			return dx + dy;
		}

		bool isDoable(short xPos, short yPos, int currStep) const
		{
			const auto dist = calcDistance(xPos, yPos);
			const auto finishStep = currStep + dist + getDistance();
			return finishStep < endStep;
		}
};

struct rideCompare {
	bool operator() (const ride & lhs, const ride & rhs) const {
		return lhs.startStep < rhs.startStep;
	}
};

class car {
	public:
		vector<short> rides;

};

class rideSolver
{
	public:
		// constants
		const int MAX_NUM_RIDES_TO_COMPARE = 3;
		const double EARLY_FACTOR = 10.0;

		// vlues
		short numStreetY, numStreetX, numTotalVehicles, numTotalRides, earlyPointBonus;
		int totalStepLimit;
		multiset<ride, rideCompare> rides;
		vector<car> cars;

		short centerX;
		short centerY;

		//methods
		rideSolver() {
			centerX = numStreetX / 2;
			centerY = numStreetY / 2;
		}

		void solve();
		car buildACar();
		double getRideValue(const ride & ride, int currStep, int currX, int currY);
		void flushResults(ofstream & file);
		short calcCenterOffset(const ride & ride) const {
			short dx = ride.endX - centerX;
			short dy = ride.endY - centerY;
			if (dx < 0)
				dx = -dx;

			if (dy < 0)
				dy = -dy;

			return dx + dy;
		}
};

void rideSolver::flushResults(ofstream & file)
{
	int counter = 0;
	for (int i = 0; i < cars.size(); i++, counter++)
	{
		const car & car = cars[i];
		file << car.rides.size();
		for (auto index : car.rides)
		{
			file << " " << index;
		}
		file << endl;
	}

	for (; counter < numTotalVehicles; counter++)
	{
		file << 0;
		file << endl;
	}
}

double rideSolver::getRideValue(const ride & ride, int currStep, int currX, int currY)
{
	double score = 0.0;


	score = ride.getDistance();
	auto travelDist = ride.calcDistance(currX, currY);
	auto startingStep = travelDist + currStep;
	if (startingStep < ride.startStep)
		score += earlyPointBonus;

	

	auto centerOffset = (numStreetX + numStreetY) / 2 - calcCenterOffset(ride);
	//score += centerOffset;


	score = score / (ride.getStepDiff() + travelDist);
	return score;
}

void rideSolver::solve()
{
	// iterate over cars
	for (int i = 0; i < numTotalVehicles; i++)
	{
		car currCar = buildACar();
		cars.push_back(currCar);
	}
}

car rideSolver::buildACar()
{
	int currStep = 0;
	short currX = 0, currY = 0;
	car currCar;

	for (; currStep < totalStepLimit;)
	{
		ride comparison = ride(currStep);
		auto iterator = rides.lower_bound(comparison);
		
		multimap<double, multiset<ride, rideCompare>::iterator> values;
		for (int rideCounter = 0; iterator != rides.end() && rideCounter < MAX_NUM_RIDES_TO_COMPARE; )
		{
			const ride & currRide = *iterator;
			if (currRide.isDoable(currX, currY, currStep))
			{
				double score = getRideValue(*iterator, currStep, currX, currY);
				values.insert(pair<double, multiset<ride, rideCompare>::iterator>(score, iterator));
				rideCounter++;
			}
			iterator++;
		}

		if (values.empty())
			break;

		auto bestValue = values.end();
		bestValue--;

		auto rideIterator = (*bestValue).second;
		const ride & ride = *rideIterator;
		auto rideIndex = ride.index;

		auto deltaSteps = ride.calcDistance(currX, currY) + ride.getDistance();


		currX = ride.endX;
		currY = ride.endY;
		currStep += deltaSteps;

		currCar.rides.push_back(rideIndex);

		rides.erase(rideIterator);

	}

	return currCar;
}

void solve(ifstream &fileIn, ofstream &fileOut)
{
	rideSolver solver;
	fileIn >> solver.numStreetY >> solver.numStreetX >> solver.numTotalVehicles >> solver.numTotalRides >> solver.earlyPointBonus >> solver.totalStepLimit;
	
	for (int rideCounter = 0; ; rideCounter++)
	{
		ride ride;
		fileIn >> ride.startX >> ride.startY >> ride.endX >> ride.endY >> ride.startStep >> ride.endStep;
		if (fileIn.fail())
			break;
		ride.index = rideCounter;
		solver.rides.insert(ride);
	}

	solver.solve();
	solver.flushResults(fileOut);
}

int main()
{

	array<string, 5> files = { "a_example", "b_should_be_easy", "c_no_hurry", "d_metropolis", "e_high_bonus" };

	for (auto filename : files)
	{
		ifstream fileIn(filename + ".in");
		ofstream fileOut(filename + ".out");
		solve(fileIn, fileOut);
	}

    return 0;
}

