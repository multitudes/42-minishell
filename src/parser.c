/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 13:28:27 by lbrusa            #+#    #+#             */
/*   Updated: 2024/02/25 15:23:19 by lbrusa           ###   ########.fr       */
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
*/