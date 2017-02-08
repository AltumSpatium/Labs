function createPoints() {
	 var points = [];
	 for (i = 0; i < 4; i++) {
	 	var point = prompt("Enter coordinates of a point of the rectangle (x y): ", "").split(' ');
	 	points.push(new Array(parseFloat(point[0]), parseFloat(point[1])));
	 }

	 return points
}

function checkRectangle(points) {
	function distance(point1, point2) {
		var dx = Math.abs(point2[0] - point1[0]);
		var dy = Math.abs(point2[1] - point1[1]);
		return Math.sqrt(Math.pow(dx, 2) + Math.pow(dy,2));
	}

	if (distance(points[0], points[2]) == distance(points[1], points[3]))
		return true;
	else return false;
}

function checkPoint(points) {
	var point = prompt("Enter coordinates of a point (x y): ", "").split(' ');
	point = [parseFloat(point[0]), parseFloat(point[1])];

	var xx = points.map(function(point) {
		return point[0];
	});

	var yy = points.map(function(point) {
		return point[1];
	});

	if (point[0] > Math.max.apply(null, xx) || point[0] < Math.min.apply(null, xx) ||
		point[1] > Math.max.apply(null, yy) || point[1] < Math.min.apply(null, yy)) {
		return false;
	}

	return true;
}

function task1() {
	var points = createPoints();
	if (!checkRectangle(points)) {
		alert("This is not a rectangle!");
		return;
	}

	if (checkPoint(points)) {
		alert("Point is inside of the rectangle");
	} else {
		alert("Point is outside of the rectangle");
	}
}
