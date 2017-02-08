function checkNumber(value) {
	return typeof(value) == "number";
}

function typeCheck(f) {
	return function() {
		if (!checkNumber(arguments[0])) {
			alert("Invalid argument type!");
			return;
		}
		return f.apply(this, arguments);
	}
}

function typeCheckArbitrary(f) {
	return function() {
		for (var i = 0; i < arguments.length; i++) {
			if (!checkNumber(arguments[i])) {
				alert("Invalid type of argument " + i + "!");
				return;
			}
		}
		return f.apply(this, arguments);
	}
}

function inc(n) {
	return n + 1;
}

function listOfArgs() {
	var args = "";

	for (var i = 0; i < arguments.length - 1; i++) {
		args += arguments[i] + ", ";
	}
	args += arguments[arguments.length - 1] + ".";

	return args;
}

function task2() {
	inc = typeCheck(inc);
	listOfArgs = typeCheckArbitrary(listOfArgs);

	alert("inc(1) = " + inc(1));
	alert("inc(true) = " + inc(true));

	alert("listOfArgs(1, 2, 3) = " + listOfArgs(1, 2, 3));
	alert("listOfArgs(1, 2, true) = " + listOfArgs(1, 2, true));
}
