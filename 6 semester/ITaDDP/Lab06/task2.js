function task2() {
	function input(message) {
		var inp = prompt(message, "");
		if (isNaN(inp) || inp <= 0) {
			alert("Incorrect input!");
			return NaN;
		}

		return inp;
	}

	var numOfStages = input("Enter number of stages");
	if (isNaN(numOfStages)) return;

	var numOfPorches = input("Enter number of porches");
	if (isNaN(numOfPorches)) return;

	var floorFlatsCount = input("Enter an amount of flats on the floor");
	if (isNaN(floorFlatsCount)) return;

	var flatNumber = input("Enter flat number");
	if (isNaN(flatNumber)) return;

	if (flatNumber > (numOfStages * floorFlatsCount * numOfPorches)) {
		alert("Flat number is too big with such house parameters!");
		return;
	}
	
	var porchNum = Math.round(flatNumber / (numOfStages * floorFlatsCount));
	alert("Flat " + flatNumber + " is in porch #" + porchNum);
}
