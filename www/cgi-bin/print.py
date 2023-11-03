#!/usr/bin/python3

import os

operator = os.environ.get("OPERATOR")
num1 = float(os.environ.get("NUM1"))
num2 = float(os.environ.get("NUM2"))

print("HTTP/1.1 200 OK\r\n\r\n")

if operator:
	if operator == "add":
		print("{:.2f}".format(num1 + num2))
	elif operator == "subtract":
		print("{:.2f}".format(num1 - num2))
	elif operator == "multiply":
		print("{:.2f}".format(num1 * num2))
	elif operator == "divide":
		if num2 != 0:
			print("{:.2f}".format(num1 / num2))
		else:
			print("inf")
	else:
		print("operator not found")
else:
	print("no operator set")
