/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 13:28:27 by lbrusa            #+#    #+#             */
/*   Updated: 2024/02/26 13:52:12 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
The scanner is the first step in the compilation process. 
It takes the raw input from the command line. It is super important to
get it right because it is the foundation of the entire process.
Especially in a shell, where the input is so varied and complex.
Doing it properly will make our bonus easier to implement.

The error handling is also very important, because the user can input
garbage and we need to be able to handle it.


In order to evaluate an arithmetic node, you need to know the numeric 
values of its subtrees, so you have to evaluate those first. 
That means working your way from the leaves up to the root—a post-order 
traversal:
A formal grammar’s job is to specify which strings are valid and which aren’t. If we were defining a grammar for English sentences, “eggs are tasty for breakfast”
would be in the grammar, but “tasty breakfast for are eggs” would probably not.
*/