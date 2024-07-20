/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 11:17:39 by lbrusa            #+#    #+#             */
/*   Updated: 2024/01/17 20:01:02 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

/*
 getops_nr returns which op to call from the array of pointer 
 to functions g_ops. ex: I have %5.5s so I look up the 
 position of s in the array of conversion and I have 2. 
 Therefore I call the second function in the array which 
 happens to be pr_s
 As arguments I pass ii which is the pointer to the counter to update it
 and update also the format string which will be
 updated in the function. 
 p is the pointer to the main string (format) starting from the next char
 where I had a '%' char. format_str needs to be freed when finished.  
 */
int	getops_nr(char *s, char **conv, int *ii)
{
	int		i;
	char	*r;

	i = 0;
	r = 0;
	while (s[i])
	{
		r = ft_strchr(CONVS, (char)(s[i]));
		if (r != NULL)
		{
			*conv = ft_substr(s, 0, i);
			*ii += i + 1;
			return ((int)(r - CONVS));
		}
		if (ft_isalpha(s[i]))
			return (CONV_ERROR);
		i++;
	}
	return (CONV_ERROR);
}

/*
I will look for the correct conversion function in the array and 
launch it getting back the number of chars written
ops[CONV_ERROR] just prints the original sonversion string.
ex if I have %4.4r which is not found as a valid conv then I just 
print %4.4r and not an error message(like the orig printf)
 */
ssize_t	get_conv(char *format, int *ii, va_list *ap)
{
	ssize_t	n;
	int		opnr;
	char	*conv;
	t_ffunc	ops[MAX_OPS];

	n = 0;
	++(*ii);
	init_ops(ops);
	conv = NULL;
	opnr = getops_nr(format + (*ii), &conv, ii);
	if (!conv)
		return (ops[CONV_ERROR](ap, conv));
	if (opnr == CONV_ERROR)
		return (ops[CONV_ERROR](ap, conv));
	n = ops[opnr](ap, conv);
	free(conv);
	return (n);
}

int	ft_printf(const char *format, ...)
{
	ssize_t	n;
	int		i;
	va_list	ap;

	i = 0;
	n = 0;
	if (!format)
		return (-1);
	va_start(ap, format);
	while (format[i])
	{
		if (format[i] == '%')
			n += get_conv((char *)format, &i, &ap);
		else
			n += write(1, format + i++, 1);
	}
	va_end(ap);
	return (n);
}

/*
-Wall -Werror -Wextra 
cc ft_printf.c  pr_.c pr_c.c pr_s.c pr_i.c err.c \
pr_p.c base16_utils.c pr_x.c pr_xx.c pr_u.c base16_utils2.c 
utils.c -L./libft -lft utilsflags2.c utilsflags.c 
allowed are |%cspdiuxXU|

I will be using the return value of write
#include <unistd.h>

ssize_t write(int fd, const void *buf, size_t count);
*/

// #include <stdio.h>
// int main(void)
// {
// 	int n1 = 0;
// 	int n2 = 0;
// 	int num = 42;
// //	 |%22.x| gives |              ffffffff|
// //	 |%.22x| gives |00000000000000ffffffff|
// //	n = printf("|%*p|",-23, &num);//|-01|
// //	printf("n = %d\n", n);
// //	n2 = ft_printf("|%*p|",-23, &num);
// //	printf("n = %d\n", n2);
// 	// n1 = printf("|%#6.2x|", 42);//|-01|
// 	// printf("n = %d\n", n1);
// 	// n2 = ft_printf("|%#6.2X|", 42);
// 	// printf("n = %d\n", n2);
// 	n1 = printf("|%-3.7s|\n", "t");
// 	printf("n1 = %d\n", n1);
// 	n2 = ft_printf("|%-3.7s|\n", "t");
// 	printf("my printf n2 = %d\n", n2);
// 	// n1 = printf("|%p|\n", NULL);
// 	// printf("n1 = %d\n", n1);
// 	// n2 = ft_printf("|%p|\n", NULL);
// 	// printf("my printf n2 = %d\n", n2);
// 	return (1);
// }

/*
// s bomus 1.OK 2.KO 3.KO 4.OK 5.OK 6.KO 7.KO 8.KO 9.KO 
///home/lbrusa/francinette/temp/printf/ft_printf_test/result.log

	n1 = printf("|%#05X|", 0);//|-01|
	printf("n = %d\n", n1);
	n2 = ft_printf("|%#05X|", 0);
	printf("n = %d\n", n2);

	n1 = printf("|%-3.0s|\n", "tubular");
	printf("n1 = %d\n", n1);
	
	n2 = ft_printf("|%-3.0s|\n", "tubular");
	printf("my printf n2 = %d\n", n2)#include <stdio.h>;


	n1 = printf("|%-.9s|\n", "tubular");
	printf("n1 = %d\n", n1);
	
	n2 = ft_printf("|%-0.9s|\n", "tubular");
	printf("my printf n2 = %d\n", n2);

	n1 = printf("|%-11c|\n", 't');
	printf("n1 = %d\n", n1);
	
	n2 = ft_printf("|%-11c|\n", 't');
	printf("my printf n2 = %d\n", n2);

	n1 = printf("|%15.4i|\n", -42);
	printf("n1 = %d\n", n1);
	
	n2 = ft_printf("|%15.4i|\n", -42);
	printf("my printf n2 = %d\n", n2);

	n1 = printf("|%15.4i|\n", -42);
	printf("n1 = %d\n", n1);
	
	n2 = ft_printf("|%15.4i|\n", -42);
	printf("my printf n2 = %d\n", n2);

	n1 = printf("|%-5.i|\n", 0);
	printf("n1 = %d\n", n1);%___r|n = 7
|___
	
	n2 = ft_printf("|%-5.i|\n", 0);
	printf("my printf n2 = %d\n", n2);

	n1 = printf("|%8.3i|\n", -8473);
	printf("n1 = %d\n", n1);
	
	n2 = ft_printf("|%8.3i|\n", -8473);
	printf("my printf n2 = %d\n", n2);

	n1 = printf("|%03x|\n", 1);
	printf("n1 = %d\n", n1);	
	n2 = ft_printf("|%03x|\n", 1);
	printf("my printf n2 = %d\n", n2);
	
	
TEST(88, print(" %03x ", 1));
	TEST(89, print(" %04x ", 9));
	TEST(90, print(" %01x ", 10));
	TEST(91, print(" %02x ", 11));
	TEST(92, print(" %03x ", 15));
	TEST(93, print(" %04x ", 16));
	TEST(94, print(" %05x ", 17));
	TEST(95, print(" %01x ", 99));
	TEST(96, print(" %02x ", 100));
	TEST(97, print(" %03x ", 101));
	TEST(98, print(" %01x ", -9));
	TEST(99, print(" %02x ", -10));
	TEST(100, print(" %03x ", -11));
	TEST(101, print(" %04x ", -14));
	TEST(102, print(" %05x ", -15));
	TEST(103, print(" %06x ", -16));
	TEST(104, print(" %01x ", -99));
	TEST(105, print(" %02x ", -100));
	
	TEST(106, print(" %03x ", -101));
	TEST(107, print(" %09x ", INT_MAX));
	TEST(108, print(" %010x ", INT_MIN));
	TEST(109, print(" %011x ", LONG_MAX));
	TEST(110, print(" %012x ", LONG_MIN));
	TEST(111, print(" %013x ", UINT_MAX));
	TEST(112, print(" %014x ", ULONG_MAX));
	TEST(113, print(" %015x ", 9223372036854775807LL));
	TEST(114, print(" %09x %010x %011x %012x %013x %014x %015x", 
	INT_MAX, INT_MIN, LONG_MAX, LONG_MIN, ULONG_MAX, 0, -42));


		n1 = printf("|%-8.3i|\n", -8473);
	printf("n1 = %d\n", n1);
	
	n2 = ft_printf("|%-8.3i|\n", -8473);
	printf("my printf n2 = %d\n", n2);

	
n1 = printf("|%3.2u|\n", 1);
	printf("n1 = %d\n", n1);
	
	n2 = ft_printf("|%3.2u|\n", 1);
	printf("my printf n2 = %d\n", n2);
n1 = printf("|%3.2u|\n", 1);
	printf("n1 = %d\n", n1);
	
	n2 = ft_printf("|%3.2u|\n", 1);
	printf("my printf n2 = %d\n", n2);

	n1 = printf("|%.2d|\n", -1);
	printf("n1 = %d\n", n1);
	
	n2 = ft_printf("|%.2d|\n", -1);
	printf("my printf n2 = %d\n", n2);
		n1 = printf("|%.1d|\n", 0);
	printf("n1 = %d\n", n1);
	
	n2 = ft_printf("|%.1d|\n", 0);
	printf("my printf n2 = %d\n", n2);
	


	// n1 = printf("|%04u|\n", 9);
	// printf("n1 = %d\n", n1);
	
	// n2 = ft_printf("|%04u|\n", 9);
	// printf("my printf n2 = %d\n", n2);
	
	// n1 = printf("|%-1d|\n", -99);
	// printf("n1 = %d\n", n1);
	
	// n2 = ft_printf("|%-1d|\n", -99);
	// printf("my printf n2 = %d\n", n2);

// 	n1 = printf("|%02d|\n", -1);
// 	printf("n1 = %d\n", n1);
	
// 	n2 = ft_printf("|%02d|\n", -1);
// 	printf("my printf n2 = %d\n", n2);
	
// 	// n1 = printf("|%-1d|\n", -99);
// 	// printf("n1 = %d\n", n1);
	
// 	// n2 = ft_printf("|%-1d|\n", -99);
// 	// printf("my printf n2 = %d\n", n2);
	// n1 = printf("|%-s|\n", "-");
	// printf("n1 = %d\n", n1);
	
	// n2 = ft_printf("|%-s|\n", "-");
	// printf("my printf n2 = %d\n", n2);

	// n1 = printf("|%s|\n", (char*)NULL);
	// printf("n1 = %d\n", n1);
	
	// n2 = ft_printf("|%s|\n", (char*)NULL);
	// printf("my printf n2 = %d\n", n2);

	// n1 = printf("|%s|\n", "hello");
	// printf("n1 = %d\n", n1);
	
	// n2 = ft_printf("|%s|\n", "hello");
	// printf("my printf n2 = %d\n", n2);

	// n1 = printf("|%7s%7s|\n", "hello", "world");
	// printf("n1 = %d\n", n1);
	
	// n2 = ft_printf("|%7s%7s|\n", "hello", "world");
	// printf("my printf n2 = %d\n", n2);

	// n1 = printf("|%3.7s%7.3s|\n", "hello", "world");
	// printf("n1 = %d\n", n1);
	
	// n2 = ft_printf("|%3.7s%7.3s|\n", "hello", "world");
	// printf("my printf n2 = %d\n", n2);

	// n1 = printf("|%7.7s%3.3s|\n", "hello", "world");
	// printf("n1 = %d\n", n1);
	
	// n2 = ft_printf("|%7.7s%3.3s|\n", "hello", "world");
	// printf("my printf n2 = %d\n", n2);


	// n1 = printf("|%3.7s%3.3s|\n", "hello", "world");
	// printf("n1 = %d\n", n1);
	
	// n2 = ft_printf("|%3.7s%3.3s|\n", "hello", "world");
	// printf("my printf n2 = %d\n", n2);


	// n1 = printf("|%.3s|\n", (char *)NULL);
	// printf("n1 = %d\n", n1);
	
	// n2 = ft_printf("|%.3s|\n",(char *) NULL);
	// printf("my printf n2 = %d\n", n2);

	// n1 = printf("|%.6s|\n", (char *)NULL);
	// printf("n1 = %d\n", n1);
	
	// n2 = ft_printf("|%.6s|\n",(char *) NULL);
	// printf("my printf n2 = %d\n", n2);

	// n1 = printf("|%3.1s|\n", (char *)NULL);
	// printf("n1 = %d\n", n1);
	
	// n2 = ft_printf("|%3.1s|\n",(char *) NULL);
	// printf("my printf n2 = %d\n", n2);

	// n1 = printf("|%-3.1s|\n", (char *)NULL);
	// printf("n1 = %d\n", n1);
	
	// n2 = ft_printf("|%-3.1s|\n",(char *) NULL);
	// printf("my printf n2 = %d\n", n2);

	// n1 = printf("|%-3.1s|\n", '\0');
	// printf("n1 = %d\n", n1);
	
	// n2 = ft_printf("|%-3.1s|\n",'\0');
	// printf("my printf n2 = %d\n", n2)

	// n1 = printf("|%-3.6s|\n", '\0');
	// printf("n1 = %d\n", n1);
	
	// n2 = ft_printf("|%-3.6s|\n",'\0');
	// printf("my printf n2 = %d\n", n2)




	// n1 = printf("|%7.3s%7.3s|\n", "hello", "world");
	// printf("n1 = %d\n", n1);
	
	// n2 = ft_printf("|%7.3s%7.3s|\n", "hello", "world");
	// printf("my printf n2 = %d\n", n2);


	// n1 = printf("|%7.3s%7.3s|\n", "hello", "world");
	// printf("n1 = %d\n", n1);
	
	// n2 = ft_printf("|%7.3s%7.3s|\n", "hello", "world");
	// printf("my printf n2 = %d\n", n2);

	// n1 = printf("|%3.3s%7.7s|\n", "hello", "world");
	// printf("n1 = %d\n", n1);
	
	// n2 = ft_printf("|%3.3s%7.7s|\n", "hello", "world");
	// printf("my printf n2 = %d\n", n2);

	// n1 = printf("|%7.3s|\n", "hello");
	// printf("n1 = %d\n", n1);
	
	// n2 = ft_printf("|%7.3s|\n", "hello");
	// printf("my printf n2 = %d\n", n2);

	// n1 = printf("|%7.3s%7.7s|\n", "hello", "world");
	// printf("n1 = %d\n", n1);
	
	// n2 = ft_printf("|%7.3s%7.7s|\n", "hello", "world");
	// printf("my printf n2 = %d\n", n2);

	// n1 = printf("|%3.7s%3.7s|\n", "hello", "world");
	// printf("n1 = %d\n", n1);
	
	// n2 = ft_printf("|%3.7s%3.7s|\n", "hello", "world");
	// printf("my printf n2 = %d\n", n2);

	// n1 = printf("|%7.7s%3.7s|\n", "hello", "world");
	// printf("n1 = %d\n", n1);
	
	// n2 = ft_printf("|%7.7s%3.7s|\n", "hello", "world");
	// printf("my printf n2 = %d\n", n2);


	// n1 = printf("|%3.7s%7.7s|\n", "hello", "world");
	// printf("n1 = %d\n", n1);
	
	// n2 = ft_printf("|%3.7s%7.7s|\n", "hello", "world");
	// printf("my printf n2 = %d\n", n2);


	// n1 = printf("|%7.7s%7.7s|\n", "hello", "world");
	// printf("n1 = %d\n", n1);
	
	// n2 = ft_printf("|%7.7s%7.7s|\n", "hello", "world");
	// printf("my printf n2 = %d\n", n2);

	// n1 = printf("|%-7.5s|\n", "hell");
	// printf("n1 = %d\n", n1);
	
	// n2 = ft_printf("|%-7.5s|\n", "hell");
	// printf("my printf n2 = %d\n", n2);

	// n1 = printf("|%7.5s|\n", "hell");
	// printf("n1 = %d\n", n1);
	
	// n2 = ft_printf("|%7.5s|\n", "hell");
	// printf("my printf n2 = %d\n", n2);

	// n1 = printf("|%.7s%.2s|\n", "hello", "world");
	// printf("n1 = %d\n", n1);
	
	// n2 = ft_printf("|%.7s%.2s|\n", "hello", "world");
	// printf("my printf n2 = %d\n", n2);

	// n1 = printf("|%.5s%.7s|\n", "he", "you");
	// printf("n1 = %d\n", n1);
	
	// n2 = ft_printf("|%.5s%.7s|\n", "he", "you");
	// printf("my printf n2 = %d\n", n2);


	// printf("=======================\n");
	// n1 = printf("hey |%c| \n",'0' - 256);
	// printf("n1 = %d\n", n1);
	// n2 = ft_printf("hey |%c| \n",'0' - 256);
	// printf("my printf n2 = %d\n", n2);



	// printf("=======================\n");
	// n1 = printf(" |%c| |%c| |%c| \n", '0', 0, '1');
	// printf("n1 = %d\n", n1);
	// n2 = ft_printf(" |%c| |%c| |%c| \n", '0', 0, '1');
	// printf("my printf n2 = %d\n", n2);


	printf("=======================\n");



	// n1 = printf("hey |%c| \n",'0' + 256);
	// printf("n1 = %d\n", n1);
	// n2 = ft_printf("hey |%c| \n",'0' + 256);
	// printf("my printf n2 = %d\n", n2);


	// n1 = printf("hey %% \n");
	// n1 = printf("hey %% \n"

// - [OUTPUT] ft_printf("%3.7s%3.7s", "hello", "world")
//    expected: "helloworld"
//    actual:   "hello");
	// printf("n1 = %d\n", n1);
	// n2 = ft_printf("hey %% \n");
	// n2 = ft_printf("hey %% \n");
	// printf("my printf n2 = %d\n", n2);


	// n1 = printf("hey %%|%-5s| \n", "you");
	// n1 = printf("hey %%|%5.2s| \n", "you");
	// printf("n1 = %d\n", n1);
	// n2 = ft_printf("hey %%|%-5s| \n", "you");
	// n2 = ft_printf("hey %%|%5.2s| \n", "you");

	// 	n1 = printf("hey %%|%-5d| \n", INT_MIN);
	// n1 = printf("hey %%|%-d| \n", INT_MIN);
	// printf("n1 = %d\n", n1);
	// n2 = ft_printf("hey %%|%-7i| \n", 2);
	// n2 = ft_printf("hey %%|%-d| \n", -2);
	// printf("n2 = %d\n", n2);


//	unsigned int a = -12;
//	n = printf("|%22u|", a);
//	printf("n = %d\n", n);
//	n2 = ft_printf("|%u|",a);
//	printf("n = %d\n", n2);
	// this makes no sense |%#.22X| with zero padding..
	// and adding again 2 extra
//	n = printf("|%#.22X|",-12);
//	printf("n = %d\n", n);
//	n2 = ft_printf("|%#0.22X|",-12);
//	printf("n = %d\n", n2);
//
//	n = printf("|%-#22.X|",-12);
//	printf("n = %d\n", n);
//	n2 = ft_printf("|%#-0.22X|",-12);
//	printf("n = %d\n", n2);
//	n = printf("|%-22.X|",-12);
//	printf("n = %d\n", n);
//	n2 = ft_printf("|%-0.22X|",-12);
//	printf("n = %d\n", n2);
//	n = printf("|%-22.x|",-12);
//	printf("n = %d\n", n);
//	n2 = ft_printf("|%-0.22x|",-12);
//	printf("n = %d\n", n2);
//	n = printf("|%.2x|",-12);//|-01|
//	printf("n = %d\n", n);
//	n2 = ft_printf("|%.x|",-12);
//	printf("n = %d\n", n2);
//	n = printf("|%#.2x|",-12);//|-01|
//	printf("n = %d\n", n);
//	n2 = ft_printf("|%#.x|",-12);
//	printf("n = %d\n", n2);
//	n = printf("|%-20.p|",NULL);//|-01|
//	printf("n = %d\n", n);
//	n2 = ft_printf("|%-20p|",NULL);
//	printf("n = %d\n", n2);
	// ok this is bs .. with the dot giving just 0x!
//	n = printf("|%-20.p|",NULL);//|-01|
//	printf("n = %d\n", n);
//	n2 = ft_printf("|%-20p|",NULL);
//	printf("n = %d\n", n2);
//	n = printf("|%-20p|",&num);//|-01|
//	printf("n = %d\n", n);
//	n2 = ft_printf("|%-20p|",&num);
//	printf("n = %d\n", n2);
//	n = printf("|%*p|",-23, &num);//|-01|
//	printf("n = %d\n", n);
//	n2 = ft_printf("|%*p|",-23, &num);
//	printf("n = %d\n", n2);
//	n = printf("|%*p|",23, &num);//|-01|
//	printf("n = %d\n", n);
//	n2 = ft_printf("|%*p|",23, &num);
//	printf("n = %d\n", n2);
//
//	n = printf("|%*p|",3, &num);//|-01|
//	printf("n = %d\n", n);
//	n2 = ft_printf("|%*p|",3, &num);
//	printf("n = %d\n", n2);
//
//	n = printf("|%+-2.i|", -0x10);//|-01|
//	printf("n = %d\n", n);
//	n2 = ft_printf("|%+-2.i|",-0x10);
//	printf("n = %d\n", n2);
//	n = printf("|%+-4.i|", -0x10);//|-01|
//	printf("n = %d\n", n);
//	n2 = ft_printf("|% -+4.i|",-0x10);
//	printf("n = %d\n", n2);
//	n = printf("|%+-4.i|", 0x10);//|-01|
//	printf("n = %d\n", n);
//	n2 = ft_printf("|% -+4.i|",0x10);
//	printf("n = %d\n", n2);
//	n = printf("%d", -10);//|-01|
//	printf("n = %d\n", n);
////	n = ft_printf("%d", 12);
//	n2 = ft_printf("%d", -10);
//	printf("n = %d\n", n2);
//	
// 	return (0);
// }

 
- [OUTPUT] ft_printf("%3.1s", s_hidden)
   expected: "  h"
   actual:   "   "

- [OUTPUT] ft_printf("%10.1s", s_hidden)
   expected: "         h"
   actual:   "          "

- [OUTPUT] ft_printf("%-3.1s", s_hidden)
   expected: "h  "
   actual:   "   "

- [OUTPUT] ft_printf("%-10.1s", s_hidden)
   expected: "h         "
   actual:   "          "

- [OUTPUT] ft_printf("%3.6s", s_hidden)
   expected: "hi low"
   actual:   "   "

- [OUTPUT] ft_printf("%20.6s", s_hidden)
   expected: "              hi low"
   actual:   "                    "

- [OUTPUT] ft_printf("%3.6s", NULL)
   expected: "(null)"
   actual:   "   "

- [OUTPUT] ft_printf("%20.6s", NULL)
   expected: "              (null)"
   actual:   "                    "

- [OUTPUT] ft_printf("%-3.6s", s_hidden)
   expected: "hi low"
   actual:   "   "

- [OUTPUT] ft_printf("%-20.6s", s_hidden)
   expected: "hi low              "
   actual:   "                    "

- [OUTPUT] ft_printf("%-3.8s", s_hidden)
   expected: "hi low"
   actual:   "   "

- [OUTPUT] ft_printf("%-10.8s", s_hidden)
   expected: "hi low    "
   actual:   "          "

- [OUTPUT] ft_printf("%-3.8s", NULL)
   expected: "(null)"
   actual:   "   "

- [OUTPUT] ft_printf("%-10.8s", NULL)
   expected: "(null)    "
   actual:   "          "

- [OUTPUT] ft_printf("%-.09s", s_hidden)
   expected: "hi low"
   actual:   ""

- [OUTPUT] ft_printf("%.4i", 5263)
   expected: "5263"
   actual:   " 5263"

- [OUTPUT] ft_printf("%.4i", -2372)
   expected: "-2372"
   actual:   "- 2372"

- [OUTPUT] ft_printf("%.3i", 13862)
   expected: "13862"
   actual:   " 13862"

- [OUTPUT] ft_printf("%.3i",-23646)
   expected: "-23646"
   actual:   "- 23646"

- [OUTPUT] ft_printf("%04i", -532)
   expected: "-532"
   actual:   "0532"

- [OUTPUT] ft_printf("%10.5i", -216)
   expected: "    -00216"
   actual:   "-    00216"

- [OUTPUT] ft_printf("%8.3i", -8473)
   expected: "   -8473"
   actual:   "-   8473"

- [OUTPUT] ft_printf("%3.3i", 6983)
   expected: "6983"
   actual:   " 6983"

- [OUTPUT] ft_printf("%3.3i", -8462)
   expected: "-8462"
   actual:   "- 8462"

- [OUTPUT] ft_printf("%-8.5i", 34)
   expected: "00034   "
   actual:   "34000   "

- [OUTPUT] ft_printf("%-3.7i", 3267)
   expected: "0003267"
   actual:   "3267000"

- [OUTPUT] ft_printf("%010.5i", -216)
   expected: "    -00216"
   actual:   "-    00216"

- [OUTPUT] ft_printf("%03.3i", 6983)
   expected: "6983"
   actual:   " 6983"

- [OUTPUT] ft_printf("%03.3i", -8462)
   expected: "-8462"
   actual:   " -8462"

- [OUTPUT] ft_printf("%.0i", 0)
   expected: ""
   actual:   " 0"

- [OUTPUT] ft_printf("%.i", 0)
   expected: ""
   actual:   " 0"

- [OUTPUT] ft_printf("%5.0i", 0)
   expected: "     "
   actual:   "    0"

- [OUTPUT] ft_printf("%5.i", 0)
   expected: "     "
   actual:   "    0"

- [OUTPUT] ft_printf("%-5.0i", 0)
   expected: "     "
   actual:   "0    "

- [OUTPUT] ft_printf("%-5.i", 0)
   expected: "     "
   actual:   "0    "

- [OUTPUT] ft_printf("%.4d", 5263)
   expected: "5263"
   actual:   " 5263"

- [OUTPUT] ft_printf("%.4d", -2372)
   expected: "-2372"
   actual:   "- 2372"

- [OUTPUT] ft_printf("%.3d", 13862)
   expected: "13862"
   actual:   " 13862"

- [OUTPUT] ft_printf("%.3d",-23646)
   expected: "-23646"
   actual:   "- 23646"

- [OUTPUT] ft_printf("%04d", -532)
   expected: "-532"
   actual:   "0532"

- [OUTPUT] ft_printf("%10.5d", -216)
   expected: "    -00216"
   actual:   "-    00216"

- [OUTPUT] ft_printf("%8.3d", -8473)
   expected: "   -8473"
   actual:   "-   8473"

- [OUTPUT] ft_printf("%3.3d", 6983)
   expected: "6983"
   actual:   " 6983"

- [OUTPUT] ft_printf("%3.3d", -8462)
   expected: "-8462"
   actual:   "- 8462"

- [OUTPUT] ft_printf("%-8.5d", 34)
   expected: "00034   "
   actual:   "34000   "

- [OUTPUT] ft_printf("%-3.7d", 3267)
   expected: "0003267"
   actual:   "3267000"

- [OUTPUT] ft_printf("%010.5d", -216)
   expected: "    -00216"
   actual:   "-    00216"

- [OUTPUT] ft_printf("%03.3d", 6983)
   expected: "6983"
   actual:   " 6983"

- [OUTPUT] ft_printf("%03.3d", -8462)
   expected: "-8462"
   actual:   " -8462"

- [OUTPUT] ft_printf("%.0d", 0)
   expected: ""
   actual:   " 0"

- [OUTPUT] ft_printf("%.d", 0)
   expected: ""
   actual:   " 0"

- [OUTPUT] ft_printf("%5.0d", 0)
   expected: "     "
   actual:   "    0"

- [OUTPUT] ft_printf("%5.d", 0)
   expected: "     "
   actual:   "    0"

- [OUTPUT] ft_printf("%-5.0d", 0)
   expected: "     "
   actual:   "0    "

- [OUTPUT] ft_printf("%-5.d", 0)
   expected: "     "
   actual:   "0    "

- [OUTPUT] ft_printf("%.5u", 2)
   expected: "00002"
   actual:   "2"

- [OUTPUT] ft_printf("%.3u", 0)
   expected: "000"
   actual:   "0"

- [OUTPUT] ft_printf("%05u", 43)
   expected: "00043"
   actual:   "   43"

- [OUTPUT] ft_printf("%03u", 0)
   expected: "000"
   actual:   "  0"

- [OUTPUT] ft_printf("%8.5u", 34)
   expected: "   00034"
   actual:   "      34"

- [OUTPUT] ft_printf("%8.5u", 0)
   expected: "   00000"
   actual:   "       0"

- [OUTPUT] ft_printf("%3.7u", 3267)
   expected: "0003267"
   actual:   "3267"

- [OUTPUT] ft_printf("%-8.5u", 34)
   expected: "00034   "
   actual:   "34      "

- [OUTPUT] ft_printf("%-8.5u", 0)
   expected: "00000   "
   actual:   "0       "

- [OUTPUT] ft_printf("%-3.7u", 3267)
   expected: "0003267"
   actual:   "3267"

- [OUTPUT] ft_printf("%08.5u", 34)
   expected: "   00034"
   actual:   "      34"
ssize_t		get_l_pad_i(char *p, t_flags *flags);
ssize_t		get_r_pad_i(char *p, t_flags *flags);
- [OUTPUT] ft_printf("%08.5u", 0)
   expected: "   00000"
   actual:   "       0"

- [OUTPUT] ft_printf("%03.7u", 3267)
   expected: "0003267"
   actual:   "3267"

- [OUTPUT] ft_printf("%.0u", 0)
   expected: ""
   actual:   "0"

- [OUTPUT] ft_printf("%.u", 0)
   expected: ""
   actual:   "0"

- [OUTPUT] ft_printf("%5.0u", 0)
   expected: "     "
   actual:   "    0"

- [OUTPUT] ft_printf("%5.u", 0)
   expected: "     "
   actual:   "    0"

- [OUTPUT] ft_printf("%-5.0u", 0)
   expected: "     "
   actual:   "0    "

- [OUTPUT] ft_printf("%-5.u", 0)
   expected: "     "
   actual:   "0    "

- [OUTPUT] ft_printf("%.5x", 21)
   expected: "00015"
   actual:   "15"

- [OUTPUT] ft_printf("%.3x", 0)
   expected: "000"
   actual:   "0"

- [OUTPUT] ft_printf("%8.5x", 34)
   expected: "   00022"
   actual:   "      22"

- [OUTPUT] ft_printf("%8.5x", 0)
   expected: "   00000"
   actual:   "       0"

- [OUTPUT] ft_printf("%2.7x", 3267)
   expected: "0000cc3"
   actual:   "cc3"

- [OUTPUT] ft_printf("%-8.5x", 34)
   expected: "00022   "
   actual:   "22      "

- [OUTPUT] ft_printf("%-8.5x", 0)
   expected: "00000   "
   actual:   "0       "

- [OUTPUT] ft_printf("%-2.7x", 3267)
   expected: "0000cc3"
   actual:   "cc3"

- [OUTPUT] ft_printf("%08.5x", 34)
   expected: "   00022"
   actual:   "00000022"

- [OUTPUT] ft_printf("%08.5x", 0)
   expected: "   00000"
   actual:   "00000000"

- [OUTPUT] ft_printf("%08.3x", 8375)
   expected: "    20b7"
   actual:   "000020b7"

- [OUTPUT] ft_printf("%02.7x", 3267)
   expected: "0000cc3"
   actual:   "cc3"

- [OUTPUT] ft_printf("%.0x", 0)
   expected: ""
   actual:   "0"

- [OUTPUT] ft_printf("%.x", 0)
   expected: ""
   actual:   "0"

- [OUTPUT] ft_printf("%5.0x", 0)
   expected: "     "
   actual:   "    0"

- [OUTPUT] ft_printf("%5.x", 0)
   expected: "     "
   actual:   "    0"

- [OUTPUT] ft_printf("%-5.0x", 0)
   expected: "     "
   actual:   "0    "

- [OUTPUT] ft_printf("%-5.x", 0)
   expected: "     "
   actual:   "0    "

- [OUTPUT] ft_printf("%.5x", 21)
   expected: "00015"
   actual:   "15"

- [OUTPUT] ft_printf("%.3x", 0)
   expected: "000"
   actual:   "0"

- [OUTPUT] ft_printf("%8.5x", 34)
   expected: "   00022"
   actual:   "      22"

- [OUTPUT] ft_printf("%8.5x", 0)
   expected: "   00000"
   actual:   "       0"

- [OUTPUT] ft_printf("%2.7x", 3267)
   expected: "0000cc3"
   actual:   "cc3"

- [OUTPUT] ft_printf("%-8.5x", 34)
   expected: "00022   "
   actual:   "22      "

- [OUTPUT] ft_printf("%-8.5x", 0)
   expected: "00000   "
   actual:   "0       "

- [OUTPUT] ft_printf("%-2.7x", 3267)
   expected: "0000cc3"
   actual:   "cc3"

- [OUTPUT] ft_printf("%08.5x", 34)
   expected: "   00022"
   actual:   "00000022"

- [OUTPUT] ft_printf("%08.5x", 0)
   expected: "   00000"
   actual:   "00000000"

- [OUTPUT] ft_printf("%08.3x", 8375)
   expected: "    20b7"
   actual:   "000020b7"

- [OUTPUT] ft_printf("%02.7x", 3267)
   expected: "0000cc3"
   actual:   "cc3"

- [OUTPUT] ft_printf("%.0x", 0)
   expected: ""
   actual:   "0"

- [OUTPUT] ft_printf("%.x", 0)
   expected: ""
   actual:   "0"

- [OUTPUT] ft_printf("%5.0x", 0)
   expected: "     "
   actual:   "    0"

- [OUTPUT] ft_printf("%5.x", 0)
   expected: "     "
   actual:   "    0"

- [OUTPUT] ft_printf("%-5.0x", 0)
   expected: "     "
   actual:   "0    "

- [OUTPUT] ft_printf("%-5.x", 0)
   expected: "     "
   actual:   "0    "

- [OUTPUT] ft_printf("%.5X", 21)
   expected: "00015"
   actual:   "15"

- [OUTPUT] ft_printf("%.3X", 0)
   expected: "000"
   actual:   "0"

- [OUTPUT] ft_printf("%8.5X", 34)
   expected: "   00022"
   actual:   "      22"

- [OUTPUT] ft_printf("%8.5X", 0)
   expected: "   00000"
   actual:   "       0"

- [OUTPUT] ft_printf("%2.7X", 3267)
   expected: "0000CC3"
   actual:   "CC3"

- [OUTPUT] ft_printf("%-8.5X", 34)
   expected: "00022   "
   actual:   "22      "

- [OUTPUT] ft_printf("%-8.5X", 0)
   expected: "00000   "
   actual:   "0       "

- [OUTPUT] ft_printf("%-2.7X", 3267)
   expected: "0000CC3"
   actual:   "CC3"

- [OUTPUT] ft_printf("%08.5X", 34)
   expected: "   00022"
   actual:   "00000022"

- [OUTPUT] ft_printf("%08.5X", 0)
   expected: "   00000"
   actual:   "00000000"

- [OUTPUT] ft_printf("%08.3X", 8375)
   expected: "    20B7"
   actual:   "000020B7"ssize_t		get_l_pad_i(char *p, t_flags *flags);
ssize_t		get_r_pad_i(char *p, t_flags *flags);

- [OUTPUT] ft_printf("%02.7X", 3267)
   expected: "0000CC3"
   actual:   "CC3"

- [OUTPUT] ft_printf("%.5X", 21)
   expected: "00015"
   actual:   "15"

- [OUTPUT] ft_printf("%.3X", 0)
   expected: "000"
   actual:   "0"

- [OUTPUT] ft_printf("%8.5X", 34)
   expected: "   00022"
   actual:   "      22"

- [OUTPUT] ft_printf("%8.5X", 0)
   expected: "   00000"
   actual:   "       0"

- [OUTPUT] ft_printf("%2.7X", 3267)
   expected: "0000CC3"
   actual:   "CC3"

- [OUTPUT] ft_printf("%-8.5X", 34)
   expected: "00022   "
   actual:   "22      "

- [OUTPUT] ft_printf("%-8.5X", 0)
   expected: "00000   "
   actual:   "0       "

- [OUTPUT] ft_printf("%-2.7X", 3267)
   expected: "0000CC3"
   actual:   "CC3"

- [OUTPUT] ft_printf("%08.5X", 34)
   expected: "   00022"
   actual:   "00000022"

- [OUTPUT] ft_printf("%08.5X", 0)
   expected: "   00000"
   actual:   "00000000"

- [OUTPUT] ft_printf("%08.3X", 8375)
   expected: "    20B7"
   actual:   "000020B7"

- [OUTPUT] ft_printf("%02.7X", 3267)
   expected: "0000CC3"
   actual:   "CC3"

- [OUTPUT] ft_printf("%.0X", 0)
   expected: ""
   actual:   "0"

- [OUTPUT] ft_printf("%.X", 0)
   expected: ""
   actual:   "0"

- [OUTPUT] ft_printf("%5.0X", 0)
   expected: "     "
   actual:   "    0"

- [OUTPUT] ft_printf("%5.X", 0)
   expected: "     "
   actual:   "    0"

- [OUTPUT] ft_printf("%-5.0X", 0)
   expected: "     "
   actual:   "0    "

- [OUTPUT] ft_printf("%-5.X", 0)
   expected: "     "
   actual:   "0    "


*/