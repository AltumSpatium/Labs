function task3() {
	var matrix1 = createMatrix(3, 3);
	var matrix2 = createMatrix(3, 3);
	var sum = sumMatrix(matrix1, matrix2);

	alert(matrixToString(matrix1) + "\n+\n\n" +
		matrixToString(matrix2) + "\n=\n\n" +
		matrixToString(sum));
}

function createMatrix(n, m) {
	var matrix = [];
	for (var i = 0; i < n; i++) {
		matrix.push([]);
		for (var j = 0; j < m; j++) {
			matrix[i].push(getRandomInt());
		}
	}

	return matrix;
}

function sumMatrix(matrix1, matrix2) {
	if (matrix1.length != matrix2.length ||
		matrix1[0].length != matrix2[0].length) {
		alert('You can only sum matrices of the same dimension!');
		return NaN;
	}

	var n = matrix1.length, m = matrix1[0].length;
	var sum = [];
	for (var i = 0; i < n; i++) {
		sum.push([]);
		for (var j = 0; j < m; j++) {
			sum[i].push(matrix1[i][j] + matrix2[i][j]);
		}
	}

	return sum;
}

function getRandomInt() {
	return Math.floor(Math.random() * 100);
}

function matrixToString(matrix) {
	var n = matrix.length, m = matrix[0].length;
	var str = "";
	for (var i = 0; i < n; i++) {
		str += "|";
		for (var j = 0; j < m; j++) {
			a_ij = matrix[i][j];
			str += (a_ij < 10 ? ("  " + a_ij) : (a_ij)) + " ";
		}
		str += "\b|\n";
	}

	return str;
}
