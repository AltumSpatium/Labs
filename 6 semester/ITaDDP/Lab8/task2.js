function quicksort(arr, comparator) {
    if (arr.length == 0) return [];
    comparator = comparator ||
    	function(a, b) { return (a == b) ? 0 : ((a > b) ? 1 : -1); };

    var qs = function(l, r) {
    	var i = l, j = r, m = arr[l + r >> 1];

    	while (i <= j) {
    		while (comparator(arr[i], m) == -1) { i++; }
			while (comparator(arr[j], m) == 1) { j--; }
			if (i <= j) {
				var c = arr[i];
				arr[i] = arr[j];
				arr[j] = c;
				i++, j--;
			}
    	}
    	if (l < j) { qs(l, j); }
    	if (i < r) { qs(i, r); }
    }

    qs(0, arr.length - 1);
}

function task2() {
	var descendingComparator = function(a, b) {
		return (a == b) ? 0 : ((a > b) ? -1 : 1);
	}
	var arr = prompt("Enter array elements", "").split(' ');
	var useDescending = prompt("Descending? (y/n)", 'n') == 'y';

	if (useDescending) {
		quicksort(arr, descendingComparator);
	} else {
		quicksort(arr);
	}

	alert(arr);
}
