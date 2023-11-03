#!/bin/bash

operator="$OPERATOR"
num1="$NUM1"
num2="$NUM2"

if [ -n "$operator" ]; then
	if [ "$operator" = "add" ]; then
		result=$(echo "scale=2; $num1 + $num2" | bc)
		printf "%.2f\n" "$result"
	elif [ "$operator" = "subtract" ]; then
		result=$(echo "scale=2; $num1 - $num2" | bc)
		printf "%.2f\n" "$result"
	elif [ "$operator" = "multiply" ]; then
		result=$(echo "scale=2; $num1 * $num2" | bc)
		printf "%.2f\n" "$result"
	elif [ "$operator" = "divide" ]; then
		if [ "$num2" != "0" ]; then
			result=$(echo "scale=2; $num1 / $num2" | bc)
			printf "%.2f\n" "$result"
		else
			echo "inf"
		fi
	else
		echo "operator not found"
	fi
else
	echo "no operator set"
fi
