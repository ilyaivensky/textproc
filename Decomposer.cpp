/*
 * Decomposer.cpp
 *
 *  Created on: Apr 26, 2014
 *      Author: ilyaivensky
 */

#include "Decomposer.h"

#include <fstream>
#include <sstream>
#include <algorithm>
#include <exception>

using namespace std;

// Dump Decomposition to the stream
std::ostream & operator<<(std::ostream & os, const Decomposition & d)
{
	os << d.token << " " << d.score << " [ ";
	for (auto w : d.subtokens)
		os << w << " ";
	os << "]";

	return os;
}

Decomposer::Decomposer(const string & unigram_file_path) : max_len(0)
{
	ifstream ifs;

	ifs.open(unigram_file_path, std::ios::in);

	if (!ifs.is_open())
		throw runtime_error("File cannot be open");

	while (!ifs.eof())
	{
		std::string line;
		std::getline(ifs, line);

		stringstream ss(line);
		string unigram;
		unsigned score;
		ss >> score;
		ss >> unigram;

		unigram_scores[unigram] = score;
		max_len = max(unigram.length(), max_len);
	}

	ifs.close();
}

namespace {

// Matches prefix with decomposition of suffix
// If the match is successful, it will be stored in token_decomposition
// The match is successful iff:
// 	a) suffix decomposition is valid and
// 	b) it has lower score with combination of prefix score
//     is lower than any previously known decomposition
void match_suffix_decomposition(
		Decomposition & token_decomposition,
		const Decomposition & suffix_decomposition,
		const string & prefix,
		unsigned prefix_score)
{
	if (!suffix_decomposition.isValid())
		return;

	unsigned total_score = suffix_decomposition.score + prefix_score;

	if (!token_decomposition.isValid() || // this is the first valid decomposition we have seen so far
			total_score < token_decomposition.score || // this decomposition has the lowest score
			(total_score == token_decomposition.score && // this decomposition has the same score, but less subtokens
					suffix_decomposition.size() + 1 < token_decomposition.size()))
	{
		// It is the best known decomposition so far

		// Drop anything we have found before
		token_decomposition.subtokens.clear();

		// add the prefix
		token_decomposition.subtokens.push_back(prefix);
		// add decompositions of suffix
		token_decomposition.subtokens.insert(token_decomposition.end(),
				suffix_decomposition.begin(), suffix_decomposition.end());

		// update the score
		token_decomposition.score = total_score;
	}

}
} // namespace

// Decomposes string using dynamic programming algorithm
Decomposition Decomposer::decompose(const std::string & token) const
{
	// Store processed suffixes to avoid re-processing
	unordered_map<string, Decomposition> processed_suffixes;

	return _decompose(token, processed_suffixes);
}

Decomposition Decomposer::_decompose(
		const std::string & token,
		unordered_map<string, Decomposition> & processed) const
{
	// Keep here the best known decomposition
	Decomposition token_decomposition(token);

	if (token.empty())
		return token_decomposition;

	// Look for valid decompositions in the form <prefix><suffix>
	for (auto itPrefixEnd = next(token.begin()),
			itTokenEnd = (token.end() - token.begin() > max_len ? token.begin() + max_len : token.end());
			itPrefixEnd != itTokenEnd; ++itPrefixEnd)
	{
		// Find the score for the prefix
		auto itFound = unigram_scores.find(string(token.begin(), itPrefixEnd));

		// If the entry is not found, then this prefix is not a part of valid decomposition
		if (itFound == unigram_scores.end())
			continue;

		// Create a suffix and try to decompose it
		string suffix(itPrefixEnd, itTokenEnd);

		// Check whether we already have processed this suffix
		auto itProcessedSuffix = processed.find(suffix);
		if (itProcessedSuffix == processed.end())
		{
			// ...not yet processed. Doing that now.
			Decomposition suffix_decomposition = _decompose(suffix, processed);
			// ...and save result for potential reuse
			itProcessedSuffix = processed.insert(make_pair(suffix, suffix_decomposition)).first;
		}

		// Match suffix and prefix.
		// If the match is successful, it will be stored in token_decomposition
		match_suffix_decomposition(token_decomposition, itProcessedSuffix->second,
			string(token.begin(), itPrefixEnd), itFound->second);
	}

	auto itFound = unigram_scores.find(token);
	if (itFound != unigram_scores.end() && itFound->second <= token_decomposition.score)
	{
		// We have reached the end of the token and have found a valid score at the end of iteration.
		// That means the token without any decomposition is a valid token
		// More of this, the score without decomposition is better than the score for any decomposition,
		// or it has the same score, but it has the minimal numbers of subtokens (which is 1 because there is no decomposition)

		token_decomposition.score = itFound->second;
		token_decomposition.subtokens.clear();
		token_decomposition.subtokens.push_back(token);
	}

	return token_decomposition;
}

