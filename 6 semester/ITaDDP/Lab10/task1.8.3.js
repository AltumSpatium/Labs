function task183() {
	let matrix1 = createMatrix(3, 3);
	let matrix2 = createMatrix(3, 3);
	let sum = sumMatrix(matrix1, matrix2);

	alert(matrixToString(matrix1) + "\n+\n\n" +
		matrixToString(matrix2) + "\n=\n\n" +
		matrixToString(sum));
}

function createMatrix(n, m) {
	let matrix = [];
	for (let i = 0; i < n; i++) {
		matrix.push([]);
		for (let j = 0; j < m; j++) {
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

	let n = matrix1.length, m = matrix1[0].length;
	let sum = [];
	for (let i = 0; i < n; i++) {
		sum.push([]);
		for (let j = 0; j < m; j++) {
			sum[i].push(matrix1[i][j] + matrix2[i][j]);
		}
	}

	return sum;
}

var getRandomInt = (v => Math.floor(Math.random() * 100));

function matrixToString(matrix) {
	let n = matrix.length, m = matrix[0].length;
	let str = "";
	for (let i = 0; i < n; i++) {
		str += "|";
		for (let j = 0; j < m; j++) {
			a_ij = matrix[i][j];
			str += (a_ij < 10 ? ("  " + a_ij) : (a_ij)) + " ";
		}
		str += "\b|\n";
	}

	return str;
}
