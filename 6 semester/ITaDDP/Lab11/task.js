function addInput() {
	var form = document.getElementById('form');
	var input = document.createElement('input');
	input.type = "text";
	input.name = "inp";
	input.className = "text-input";
	form.appendChild(input);
}

function delInput() {
	var form = document.getElementById('form');
	var lastChild = form.lastChild;
	if (lastChild.tagName != "P")
		lastChild.remove();
}