function addInput() {
	$('#form').append('<input type="text" class="text-input">');
}

function delInput() {
	$('#form').children()
		.filter(function() {return $(this).prop('tagName') !== 'P'})
		.last().remove();
}

function sendAjax() {
	$('#content').load('/part.html');
}