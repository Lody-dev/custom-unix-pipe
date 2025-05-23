# Libft - Custom Implementation of the C Standard Library

**Libft** is a project from the **42 School** curriculum that challenges students to reimplement key functions from the C standard library (`libc`). The goal is to build a deep understanding of fundamental concepts in C programming by creating a custom version of commonly used functions. This library will serve as a useful foundation for future 42 projects.

## Project Overview

This project requires the implementation of a set of essential C library functions with a consistent naming convention (`ft_`). These functions cover various tasks such as character checks, string operations, memory manipulation, and conversions. By building these from scratch, students gain a stronger grasp of low-level memory management, algorithmic problem solving, and the inner workings of C.

## Features

The library includes the following custom implementations of standard C functions:

### 1. **Character Checks and Manipulations**
- `ft_isalpha` - Checks if a character is an alphabetic letter.
- `ft_isdigit` - Checks if a character is a digit (0-9).
- `ft_isalnum` - Checks if a character is alphanumeric (letter or digit).
- `ft_isascii` - Checks if a character is part of the ASCII set.
- `ft_isprint` - Checks if a character is printable.
- `ft_toupper` - Converts a lowercase letter to uppercase.
- `ft_tolower` - Converts an uppercase letter to lowercase.

### 2. **String Operations**
- `ft_strlen` - Calculates the length of a string.
- `ft_strlcpy` - Copies a string with size limitation to prevent overflow.
- `ft_strlcat` - Concatenates two strings with size limitation to prevent overflow.
- `ft_strchr` - Locates the first occurrence of a character in a string.
- `ft_strrchr` - Locates the last occurrence of a character in a string.
- `ft_strncmp` - Compares two strings up to a given number of characters.
- `ft_strnstr` - Locates a substring in a string, up to a given number of characters.
- `ft_strdup` - Duplicates a string by allocating memory and copying the content.

### 3. **Memory Operations**
- `ft_memset` - Fills memory with a constant byte.
- `ft_bzero` - Zeros out a block of memory.
- `ft_memcpy` - Copies a block of memory.
- `ft_memmove` - Copies a block of memory, handling overlapping regions safely.
- `ft_memchr` - Locates the first occurrence of a byte in memory.
- `ft_memcmp` - Compares two blocks of memory.
- `ft_calloc` - Allocates memory for an array and initializes it to zero.

### 4. **Conversion Functions**
- `ft_atoi` - Converts a string to an integer.

## Purpose of the Project

This project is part of the 42 School program, designed to solidify students’ understanding of fundamental C programming concepts. By re-creating these basic library functions, students develop their ability to:
- Work with low-level memory operations.
- Understand how common standard functions are implemented.
- Create clean, efficient, and reusable code.
  
The **Libft** project is a cornerstone of the 42 curriculum, acting as the foundation for more complex projects and ensuring that students have the necessary tools to succeed in future challenges.
