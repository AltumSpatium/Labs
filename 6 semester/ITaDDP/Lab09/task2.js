function Task(dateStart, dateEnd, descr) {
	this.dateStart = dateStart;
	this.dateEnd = dateEnd;
	this.descr = descr;
	this.subTasks = [];
}

Task.prototype.addTask = function(task) {
	this.subTasks.push(task);
}

Task.prototype.toString = function() {
	return this.descr;
}

function ExecutingTask(dateStart, dateEnd, descr) {
	Task.apply(this, arguments);
	this.percent = 0;
	this.isReady = false;
}

ExecutingTask.prototype = Object.create(Task.prototype);
ExecutingTask.prototype.constructor = ExecutingTask;

function task2() {
	var task = new Task("02.01.2017", "02.02.2017", "Task 1");
	alert(task.subTasks);
	task.addTask(new Task("02.01.2017", "03.01.2017", "Subtask 1"));
	task.addTask(new ExecutingTask("03.01.2017", "08.01.2017", "Subtask 2 ex"));
	alert(task.subTasks);
}
