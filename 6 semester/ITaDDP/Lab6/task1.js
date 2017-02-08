function task1() {
	while (true) {
		var firstNum = prompt("Enter first number", "");
		if (isNaN(firstNum)) {
			alert("First input is not a number");
			return;
		}
		var secondNum = prompt("Enter second number", "");
		if (isNaN(secondNum)) {
			alert("Second input is not a number");
			return;
		}

		if (firstNum == secondNum)
			alert("Numbers are equal");
		else if (firstNum > secondNum)
			alert("Second number is less");
		else
			alert("First number is less");
	}
}
