// MovieRatings.cpp : recursive negative scores

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <string>
using namespace std;

typedef vector<int> MyVec;
typedef MyVec::iterator MyIter;

int max_ratings(MyVec);
bool is_negative(int);
int neg_score(MyVec);

#define MAX(a,b) (a > b ? a : b)
#define ERROR(X) process_error(X, __LINE__)
#define PRINT(X) std::cout << X << std::endl

int main()
{
	int num_ratings, count;
	MyVec ratings;

	cin >> num_ratings;
	count = 0;

	while (count < num_ratings)
	{
		int rnext;
		cin >> rnext;
		ratings.push_back(rnext);
		count++;
	}

	PRINT(max_ratings(ratings));

	return 0;
}

int max_ratings(MyVec myratings)
{
	bool current_sign;
	int result = 0;
	MyIter start, finish;

	start = myratings.begin();
	finish = start + 1;
	current_sign = is_negative(*start);

	if (finish == myratings.end())
	{
		if (current_sign)
			return 0;
		else
			return *start;
	}

	MyVec nextbatch;
	nextbatch.push_back(*start);
	while (finish != myratings.end())
	{
		// find next different sign
		if (current_sign != is_negative(*finish))
		{
			if (is_negative(nextbatch[0]))
				result += neg_score(nextbatch);
			else
				result += std::accumulate(nextbatch.begin(), nextbatch.end(), 0);

			nextbatch.clear();
			start = finish; finish += 1;
			current_sign = is_negative(*start);
			nextbatch.push_back(*start);
		}
		else
		{
			nextbatch.push_back(*finish);
			++finish;
		}
	}

	if (nextbatch.size() > 0)
		if (is_negative(nextbatch[0]))
			result += neg_score(nextbatch);
		else
			result += std::accumulate(nextbatch.begin(), nextbatch.end(), 0);

	return result;
}

bool is_negative(int rating)
{
	if (rating < 0)
		return true;
	else
		return false;
}

// process_error
void process_error(const string msg, const int line)
{
	cerr << __FILE__ << ", line " << line << endl;
	cerr << "Error: " << msg << endl;
	cerr << msg << endl;
	exit(1);
}

// all combinations through recursion
int neg_score(MyVec negatives)
{
	if (negatives.size() == 0)
		ERROR("neg_score 0 args");

	if (negatives.size() == 1) return 0;

	if (negatives.size() == 2)
		return MAX(negatives[0], negatives[1]);

	// size => 3 proceceed w/ recursion
	MyVec allresults;

	// skip the current element, use recursion on the left, on the right
	// skip the current element, and find out the rest w/ recursion
	for (MyIter iter = negatives.begin(); iter != negatives.end(); ++iter)
	{
		int next_result = 0;

		// left part recursion
		if (iter > negatives.begin())
		{
			next_result += *(iter - 1);
			if (iter - 1 > negatives.begin())
			{
				MyVec on_the_left;
				copy(negatives.begin(), iter - 1, back_inserter(on_the_left));
				next_result += neg_score(on_the_left);
			}
		}

		// right part recursion
		if (iter + 1 < negatives.end())
		{
			next_result += *(iter + 1);

			if (iter + 2 < negatives.end())
			{
				MyVec on_the_right;
				copy(iter + 2, negatives.end(), back_inserter(on_the_right));
				next_result += neg_score(on_the_right);
			}
		}

		allresults.push_back(next_result);
	}

	return *(std::max_element(allresults.begin(), allresults.end()));

}

