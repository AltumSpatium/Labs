using System;

namespace Lab8
{
    public class Student
    {
        static Random rand = new Random();

        public string Name { get; set; }

        public string Surname { get; set; }

        public int Age { get; set; }

        public string Group { get; set; }

        public Student()
        {
            Name = "";
            Surname = "";
            Age = 0;
        }

        public Student(string name, string surname)
        {
            Name = name;
            Surname = surname;
            Age = rand.Next(18, 25);
        }

        public Student(string name, string surname, int age)
        {
            Name = name;
            Surname = surname;
            Age = age;
        }
    }
}
