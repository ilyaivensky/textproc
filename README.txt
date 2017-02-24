README.txt
 Created on: Feb 23, 2017
     Author: ilyaivensky

     Decomposition challenge:

A token sequence S is a decomposition of the token T if the concatenation of the tokens in S results in T. For example, "followup" can be decomposed as "follow" + "up". We will consider that T is a valid decomposition of T.

A "unigram score" file (each line of the form: <score> <token>) is attached to this email. 
The score of S is the sum of the unigram scores for all of the tokens in the decomposition.  For example, if the scores of "follow" and "up" are respectively 7 and 20, the score of the decomposition "follow" + "up" is 7 + 20 = 27.  
The tokens in S must all exist in the unigram score file. There is no such restriction for T.

Given the unigram score file and a list of tokens (one per line), the code should output lines of the form <token T> <smallest score> <smallest-score decomposition>.