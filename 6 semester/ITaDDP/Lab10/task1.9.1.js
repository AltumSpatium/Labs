class Vector {
	constructor (x, y, z) {
		this.x = x;
		this.y = y;
		this.z = z;
	};
	plus (other) {
		if (other instanceof Vector) {
			this.x += other.x;
			this.y += other.y;
			this.z += other.z;
		} else {
			alert("Instance must be vector!");
		}
	};
	scalar (other) {
		if (other instanceof Vector) {
			return this.x * other.x + this.y * other.y + this.z * other.z;
		} else {
			alert("Instance must be vector!");
		}
	};
	toString () { return "Vector(" + this.x + ", " + this.y + ", " + this.z + ")" };
	valueOf () { return "[vector Vector]" };
	get length () { return Math.sqrt(this.x*this.x + this.y*this.y + this.z*this.z) };
}

function task191() {
	var vector1 = new Vector(1, 2, 2);
	var vector2 = new Vector(0, 3, 4);
	
	alert("Vector 1: " + vector1.toString() + ", length: " + vector1.length);
	alert("Vector 2: " + vector2.toString() + ", length: " + vector2.length);
	alert("Vector 1 * Vector 2: " + vector1.scalar(vector2));
	vector1.plus(vector2);
	alert("Vector 1 + Vector 2: " + vector1.toString());
}
