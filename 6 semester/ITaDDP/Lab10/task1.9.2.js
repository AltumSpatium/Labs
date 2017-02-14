class Task {
	constructor (dateStart, dateEnd, descr) {
		this.dateStart = dateStart;
		this.dateEnd = dateEnd;
		this.descr = descr;
		this.subTasks = [];
	};
	addTask (task) {
		this.subTasks.push(task);
	};
	toString () { return this.descr };
}

class ExecutingTask extends Task {
	constructor (dateStart, dateEnd, descr, percent, isReady) {
		super(dateStart, dateEnd, descr);
		this.percent = percent;
		this.isReady = isReady;
	};
}

function task192() {
	var task = new Task(new Date("02.01.2017"), new Date("02.02.2017"), "Task 1");
	alert(task.subTasks);
	task.addTask(new Task(new Date("02.01.2017"), new Date("03.01.2017"), "Subtask 1"));
	task.addTask(new ExecutingTask(new Date("03.01.2017"), new Date("08.01.2017"), "Subtask 2 ex"));
	alert(task.subTasks);
}
