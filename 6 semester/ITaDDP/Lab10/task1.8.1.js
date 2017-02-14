function range(start, end, step = 1) {
	if (step == 0) return start;
	let arr = [];
	for (let i = start; step > 0 ? (i <= end) : (i >= end); i += step) {
		arr.push(i);
	}

	return arr;
}

function task181() {
	let start = prompt('Enter start of range', 1);
	let end = prompt('Enter end of range', 10);
	let step = prompt('Enter step', 1);

	alert(range(parseInt(start), parseInt(end), parseInt(step)));
}
