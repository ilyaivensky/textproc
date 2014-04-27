/*
 * Decomposer.h
 *
 *  Created on: Apr 26, 2014
 *      Author: ilyaivensky
 */

#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>

#ifndef DECOMPOSER_H_
#define DECOMPOSER_H_

// Data structure to represent decomposition of token
struct Decomposition
{
	Decomposition(const std::string & tok = std::string()) :
		token(tok), score(INVALID) {}

	static const unsigned INVALID = -1;

	bool isValid() const { return score != INVALID; }

	std::vector<std::string>::const_iterator begin() const {
		return subtokens.begin();
	}

	std::vector<std::string>::const_iterator end() const {
		return subtokens.end();
	}

	// Original token
	std::string token;

	// Scoring for decomposition
	unsigned score;

	// Decompositions
	std::vector<std::string> subtokens;
};

// Dump Decomposition to the stream
std::ostream & operator<<(std::ostream & os, const Decomposition & d);

// Decomposes given token into sub tokens
class Decomposer
{
public:

	// Constructs parser and initializes dictionary with unigram scores from the file
	Decomposer(const std::string & unigram_file_path);

	Decomposition decompose(const std::string & token) const;

private:

	Decomposition _decompose(const std::string & token,
		std::unordered_map<std::string, Decomposition> & processed_suffixes) const;

	// Dictionary to store scores
	std::unordered_map<std::string, unsigned> unigram_scores;
};

#endif /* DECOMPOSER_H_ */
