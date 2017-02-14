function checkNumber(value) {
	return typeof(value) == "number";
}

function typeCheck(f) {
	return (v => {
		if (!checkNumber(arguments[0])) {
			alert("Invalid argument type!");
			return;
		}
		return f.apply(this, arguments);
	})
}

function typeCheckArbitrary(f) {
	return (v => {
		for (let i = 0; i < arguments.length; i++) {
			if (!checkNumber(arguments[i])) {
				alert("Invalid type of argument " + i + "!");
				return;
			}
		}
		return f.apply(this, arguments);
	})
}

var inc = (v => n + 1);

function listOfArgs() {
	let args = "";

	for (let i = 0; i < arguments.length - 1; i++) {
		args += arguments[i] + ", ";
	}
	args += arguments[arguments.length - 1] + ".";

	return args;
}

function task172() {
	inc = typeCheck(inc);
	listOfArgs = typeCheckArbitrary(listOfArgs);

	alert("inc(1) = " + inc(1));
	alert("inc(true) = " + inc(true));

	alert("listOfArgs(1, 2, 3) = " + listOfArgs(1, 2, 3));
	alert("listOfArgs(1, 2, true) = " + listOfArgs(1, 2, true));
}
