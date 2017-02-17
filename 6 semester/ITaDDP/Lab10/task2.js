class Node {
	constructor (value) {
		this.value = value ? value : null;
		this.frequency = 0;
		this.left = null;
		this.right = null;
		this.parent = null;
		this.height = 0;
		this.bits = null;
		this.bitLength = 0;
	};
	setDictionary (dictionary, bitLength) {
		if (dictionary[dictionary.nextIndex + 1] == bitLength + 1) {
			this.left = new Node(dictionary[dictionary.nextIndex]);
			this.left.parent = this;
			dictionary.nextIndex += 2;
		} else {
			this.left = new Node();
			this.left.parent = this;
			this.left.setDictionary(dictionary, bitLength + 1);
		}

		if (dictionary[dictionary.nextIndex + 1] == bitLength + 1) {
			this.right = new Node(dictionary[dictionary.nextIndex]);
			this.right.parent = this;
			dictionary.nextIndex += 2;
		} else {
			this.right = new Node();
			this.right.parent = this;
			this.right.setDictionary(dictionary, bitLength + 1);
		}
	};
	getDictionary (dictionary, bitLength) {
		if (this.value == null) {
			this.left.getDictionary(dictionary, bitLength + 1);
			this.right.getDictionary(dictionary, bitLength + 1);
		} else {
			dictionary.push(this.value, bitLength);
		}
	};
	toString () {
		let str = "";
		if (this.left)
			str += "[" + this.value + "," + this.frequency + "," + this.height + "]"
					+ "(" + this.left.toString() + "," + this.right.toString() + ")";
		else
			str += "[" + this.value + "," + this.frequency + "," + this.height + "]";
		return str;
	};
}

class Huffman {
	constructor (dictionary) {
		if (dictionary) this.setDictionary(dictionary);
		this.root = null;
		this.leaves = {};
	};
	setDictionary (dictionary) {
		if (!dictionary)
			throw "No dictionary provided.";
		this.root = new Node();
		if (typeof dictionary == "string")
			dictionary = dictionary.split("");
		dictionary.nextIndex = 0;
		this.root.setDictionary(dictionary, 0);
		let size = dictionary.nextIndex;
		delete dictionary.nextIndex;
		return size;
	};
	getDictionary (asArray) {
		let dictionary = [];
		if (!this.root)
			throw "Impossible to extract from non-existing tree.";
		this.root.getDictionary(dictionary, 0);
		if (!asArray) return dictionary.join("");
		else return dictionary;
	};
	buildTree (data) {
		let roots = {};

		if (typeof data == "string")
			data = data.split("");

		for (let key of data) {
			if (!roots[key]) {
				roots[key] = new Node(key);
				this.leaves[key] = roots[key];
			}
			roots[key].frequency++;
		}

		let arr = [];
		for (var key in roots)
			arr.push(roots[key]);
		roots = arr;

		if (arr.length === 1) {
			key = key.length === 1 ? String.fromCharCode(255 - key.charCodeAt(0)) :
				key + "+";
			let artificial = new Node(key);
			roots[key] = artificial;
			this.leaves[key] = artificial;
			arr.push(artificial);
		}

		while (roots.length > 1) {
			let leastOften, secondLeastOften;
			if (roots[0].frequency < roots[1].frequency) {
				leastOften = 0, secondLeastOften = 1;
			} else {
				leastOften = 1, secondLeastOften = 0;
			}

			for (let i = 2; i < roots.length; i++) {
				if (roots[i].frequency < roots[leastOften].frequency) {
					secondLeastOften = leastOften, leastOften = i;
				} else if (roots[i].frequency < roots[secondLeastOften].frequency)
					secondLeastOften = i;
			}

			let node = new Node(), leastLeft = true;
			if (roots[leastOften].height > roots[secondLeastOften].height)
				leastLeft = false;
			else if (roots[leastOften].height == roots[secondLeastOften].height &&
				roots[leastOften].value > roots[secondLeastOften].value)
				leastLeft = false;
			if (leastLeft) {
				node.left = roots[leastOften];
				node.right = roots[secondLeastOften];
			} else {
				node.left = roots[secondLeastOften];
				node.right = roots[leastOften];
			}
			node.frequency = node.left.frequency + node.right.frequency;
			node.height = 1 + Math.max(node.left.height, node.right.height);
			node.left.parent = node;
			node.right.parent = node;
			roots[leastOften] = node;
			roots.splice(secondLeastOften, 1);
		}

		this.root = roots[0];
	};
	intToString (value) {
		return String.fromCharCode(
			(value >> 24) & 0xFF,(value >> 16) & 0xFF,
			(value >> 8) & 0xFF, value & 0xFF);
	};
	stringToInt (str) {
		return (str.charCodeAt(0) << 24) | (str.charCodeAt(1) << 16) |
			(str.charCodeAt(2) << 8) | str.charCodeAt(3);
	};
	compressData (data, asArray) {
		let dword = 0, bitsLeft = 32, compressed;

		if (!asArray) compressed = this.intToString(data.length);
		else {
			compressed = [];
			compressed.push(data.length);
		}

		if (typeof data == "string")
			data = data.split("");

		for (let key of data) {
			let node = this.leaves[key];
			if (!node)
				throw "Huffman tree does not match input data.";

			if (node.bitLength == 0) {
				node.bits = 0;
				let current = node;
				while (current.parent) {
					if (current.parent.right == current)
						node.bits |= (1 << node.bitLength);
					node.bitLength++;
					current = current.parent;
				}
			}

			if (bitsLeft >= node.bitLength) {
				dword = (dword << node.bitLength) | node.bits;
				bitsLeft -= node.bitLength;
			} else {
				dword = (dword << bitsLeft) | (node.bits >>> (node.bitLength - bitsLeft));
				let value = dword & 0xffffffff;
				if (!asArray) compressed += this.intToString(value);
				else compressed.push(value);
				dword = node.bits;
				bitsLeft = 32 - (node.bitLength - bitsLeft);
			}
		}
		let value = (dword << bitsLeft) & 0xffffffff;
		if (!asArray) compressed += this.intToString(value);
		else compressed.push(value);
		return compressed;
	};
	compress (data) {
		this.buildTree(data);
		let asArray = !(typeof data == "string");
		return this.getDictionary(asArray).concat(this.compressData(data, asArray));
	};
	decompressData (compressed, asArray, startIndex) {
		let index = 0, bitIndex = 32, data = [], compressedIndex, count;
		let strtype = typeof compressed == "string";
		compressedIndex = !startIndex ? 0 : startIndex;
		if (strtype) {
			count = this.stringToInt(compressed.substr(compressedIndex, 4));
			compressedIndex += 4;
		} else {
			count = compressed[compressedIndex];
			compressedIndex++;
		}

		while (index < count) {
			let node = this.root;
			while (!node.value) {
				let value = !strtype ? compressed[compressedIndex] :
					this.stringToInt(compressed.substr(compressedIndex, 4));
				let bit = (value >> (bitIndex - 1)) & 1;
				bitIndex--;
				if (bitIndex == 0) {
					if (strtype) compressedIndex += 4;
					else compressedIndex++;
					bitIndex = 32;
				}

				node = bit ? node.right : node.left;
			}

			data.push(node.value);
			index++;
		}

		if (!asArray) return data.join("");
		else return data;
	};
	decompress (bitStream) {
		let index = this.setDictionary(bitStream);
		let asArray = !(typeof bitStream == "string");
		return this.decompressData(bitStream, asArray, index);
	};
	toString () {
		if (!this.root) return "no tree";
		else return root.toString();
	};
}

function input() {
	let asArray = prompt("Compress an array? (y/n): ", 'n') == 'y';
	if (asArray)
		return prompt("Enter array values: ", "").split(' ');
	else
		return prompt("Enter string: ", "");
}

function task2() {
	let data = input();
	let huffman = new Huffman();
	let compressed = huffman.compress(data);
	alert(compressed);
	alert(huffman.decompress(compressed));
}
