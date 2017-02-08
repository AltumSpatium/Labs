function task3() {
	function fib(n) {
		if (n <= 1) return n;
		return fib(n - 1) + fib(n - 2);
	}

	var i = prompt("Enter i", "");
	if (isNaN(i) || i < 0) {
		alert("Incorrect input!");
		return;
	}

	alert("Fibonacci number: " + fib(i));
}
