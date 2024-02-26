/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 13:28:27 by lbrusa            #+#    #+#             */
/*   Updated: 2024/02/25 17:23:07 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
need to implement a struct for a token with the following attributes:
type: the type of the token from the enum e_token_type
lexeme a string containing the actual token
maybe the line number and column number of the token in the input string

The core of the scanner is a loop. 
Starting at the first character of the source code, it figures out what lexeme 
it belongs to, and consumes it and any following characters that are part of 
that lexeme. 
When it reaches the end of that lexeme, it emits a token.

need to implement a function that takes a string and returns a list of 
tokens.

also error handling for invalid tokens like
error(line, "Unexpected character.");
but keep scanning the input to find more errors
What about ! ? It’s a single character, right? Sometimes, yes, but 
if the very next character is an equals sign, then we should instead 
create a != lexeme.
It’s like a conditional advance() . We only consume the current character 
if it’s what we’re looking for.
/ for division. But what if it’s the start of a comment?

peek() 		- look at the current character without consuming it

skip over those other characters: newlines and whitespace.
When encountering whitespace, 
we simply go back to the beginning of the scan loop
 (( )){} // grouping stuff
  !*+-/=<> <= == // operators

Strings always begin with a specific character, " and check for 
ending with " as well
Trim the surrounding quotes.

To recognize the beginning of a number lexeme, we look for any digit. 

identifiers and their close cousins, the reserved words

an important principle called maximal munch. 
When two lexical grammar rules can both match a chunk of code that the scanner is looking at, whichever one matches the most characters wins.

private boolean isAlpha(char c) {
      return (c >= 'a' && c <= 'z') ||
             (c >= 'A' && c <= 'Z') ||
              c == '_';
}
isAlphaNumeric(char c) {
	  return isAlpha(c) || isDigit(c);
}

We define the set of reserved words in a map:


*/