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

#define NOT_LOCKED	0
#define RIGHT_LOCK	1	// fix the rightmost score
#define LEFT_LOCK   -1	// fix the leftmost score

int max_ratings(MyVec);
bool is_negative(int);
int neg_score(const int, MyVec);

template<class T> T get_max(T a, T b)
{
	if (a > b)
		return a;
	else
		return b;
}

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

	if (num_ratings != ratings.size())
		cout << "Must have " << num_ratings << " ratings";
	else
		cout << max_ratings(ratings);

	cout << endl;

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

	while (finish != myratings.end())
	{
		MyVec next_batch;

		next_batch.push_back(*start);
		for (finish = start + 1; finish != myratings.end(); ++finish)
		{
			if (current_sign == is_negative(*finish))
				next_batch.push_back(*finish);
			else
			{
				start = finish; finish += 1;
				current_sign = is_negative(*start);
				break;
			}
		}

		if (is_negative(next_batch[0]))
			result += neg_score(NOT_LOCKED, next_batch);
		else
		    result += std::accumulate(next_batch.begin(), next_batch.end(), 0);
	}

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
void process_error(const string msg)
{
	cout << msg << endl;
	exit(1);
}

// all combinations through recursion
int neg_score(const int lock, MyVec negatives)
{
	if (negatives.size() == 0)
		process_error("neg_score 0 args");

	if (negatives.size() == 1)
	{
		if (lock == NOT_LOCKED) return 0;
		if (lock == LEFT_LOCK || lock == RIGHT_LOCK)
			return negatives[0];
	}

	if (negatives.size() == 2)
	{
		if (lock == NOT_LOCKED)
			return get_max(negatives[0], negatives[1]);
		else if (lock == LEFT_LOCK)
			return negatives[0];
		else if (lock == RIGHT_LOCK)
			return negatives[1];
		else
			process_error("internal neg_score error #2");
	}

	// recursion w/ removing the lock, size => 3
	if (lock == LEFT_LOCK)
	{
		int locked_score = *(negatives.begin());
		MyVec on_the_right;
		std::copy(negatives.begin() + 1, negatives.end(), std::back_inserter(on_the_right));
		return locked_score + neg_score(NOT_LOCKED, on_the_right);
	}
	else if (lock == RIGHT_LOCK)
	{
		int locked_score = *(negatives.end() - 1);
		MyVec on_the_left;
		std::copy(negatives.begin(), negatives.end() - 1, std::back_inserter(on_the_left));
		return locked_score + neg_score(NOT_LOCKED, on_the_left);
	}

	MyVec allresults;

	// skip the current element, use recursion on the left, on the right
	for (MyIter iter = negatives.begin(); iter != negatives.end() ; ++iter)
	{
		int next_result = 0;	// skip current score
		
		if (iter != negatives.begin())
		{
			MyVec on_the_left;
			std::copy(negatives.begin(), iter, std::back_inserter(on_the_left));
			next_result += neg_score(RIGHT_LOCK, on_the_left);
		}

		if (iter + 1 != negatives.end())
		{
			MyVec on_the_right;
			std::copy(iter + 1, negatives.end(), std::back_inserter(on_the_right));
			next_result += neg_score(LEFT_LOCK, on_the_right); // must be right_score
		}

		allresults.push_back(next_result);
	}

	return *(std::max_element(allresults.begin(), allresults.end()));

}

