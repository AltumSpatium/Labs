function range(start, end, step) {
	if ((step = step || 1) == 0) return start;
	var arr = [];
	for (var i = start; step > 0 ? (i <= end) : (i >= end); i += step) {
		arr.push(i);
	}

	return arr;
}

function task1() {
	var start = prompt('Enter start of range', 1);
	var end = prompt('Enter end of range', 10);
	var step = prompt('Enter step', 1);

	alert(range(parseInt(start), parseInt(end), parseInt(step)));
}
