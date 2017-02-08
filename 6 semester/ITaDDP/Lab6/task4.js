function task4() {
	var monthsLength = {'1': 31, '2': 28, '3': 31, '4': 30, '5': 31,
		'6': 30, '7': 31, '8': 31, '9': 30, '10': 31, '11': 30, '12': 31};

	var daysOfWeek = {'1': 'Monday', '2': 'Tuesday', '3': 'Wednesday',
		'4': 'Thursday', '5': 'Friday', '6': 'Saturday', '7': 'Sunday'}

	var month = prompt("Enter month number", "");
	if (isNaN(month) || month < 1 || month > 12) {
		alert("Incorrect input!");
		return;
	}

	var day = prompt("Enter day in a month", "");
	if (isNaN(day) || day < 1 || day > 31) {
		alert("Incorrect input!");
		return;
	}

	if (day > monthsLength[month]) {
		alert("There is less days in this month!");
		return;
	}

	var days = 0;

	for (i = 1; i < month; i++) {
		days += parseInt(monthsLength[i]);
	}
	days += parseInt(day);

	var dayOfWeek = daysOfWeek[days % 7 + 3];
	var date = (day < 10 ? "0" : "") + day + "." + 
		(month < 10 ? "0" : "") + month + ".2015";

	alert(date + " is " + dayOfWeek);
}