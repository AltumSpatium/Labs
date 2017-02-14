function quicksort(arr, comparator) {
    if (arr.length == 0) return [];
    comparator = comparator ||
    	function(a, b) { return (a == b) ? 0 : ((a > b) ? 1 : -1); };

    var qs = function(l, r) {
    	let i = l, j = r, m = arr[l + r >> 1];

    	while (i <= j) {
    		while (comparator(arr[i], m) == -1) { i++; }
			while (comparator(arr[j], m) == 1) { j--; }
			if (i <= j) {
				[arr[i], arr[j]] = [arr[j], arr[i]];
				i++, j--;
			}
    	}
    	if (l < j) { qs(l, j); }
    	if (i < r) { qs(i, r); }
    }

    qs(0, arr.length - 1);
}

function task182() {
	let descendingComparator = function(a, b) {
		return (a == b) ? 0 : ((a > b) ? -1 : 1);
	};
	let arr = prompt("Enter array elements", "").split(' ');
	let useDescending = prompt("Descending? (y/n)", 'n') == 'y';

	if (useDescending) {
		quicksort(arr, descendingComparator);
	} else {
		quicksort(arr);
	}

	alert(arr);
}
