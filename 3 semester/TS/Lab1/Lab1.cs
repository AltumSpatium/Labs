using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

class Student
{
    private string name;
    private string surname;
    private string group;

    public string Name
    {
        get { return name; }
        set { name = value; }
    }

    public string Surname
    {
        get { return surname; }
        set { surname = value; }
    }

    public string Group
    {
        get { return group; }
        set { group = value; }
    }

    public Student(string name, string surname)
    {
        Name = name;
        Surname = surname;
    }
}

class Group : IEnumerable<Student>
{
    private string number;
    public List<Student> students;

    public string Number
    {
        get { return number; }
        set { number = value; }
    }

    public Group(string number)
    {
        Number = number;
        students = new List<Student>();
    }

    public int Count
    {
        get { return students.Count; }
    }

    public void AddStudent(Student student)
    {
        students.Add(student);
        student.Group = number;
    }

    public bool ExpellStudent(Student student)
    {
        return students.Remove(student);
    }

    public IEnumerator<Student> GetEnumerator()
    {
        foreach (Student g in students)
        {
            yield return g;
        }
    }

    IEnumerator IEnumerable.GetEnumerator()
    {
        return GetEnumerator();
    }
}


class Faculty : IEnumerable<Group>
{
    private string name;
    public List<Group> groups;

    public string Name
    {
        get { return name; }
        set { name = value; }
    }

    public Faculty(string name)
    {
        Name = name;
        groups = new List<Group>();
    }

    public int Count
    {
        get { return groups.Count; }
    }

    public void AddGroup(Group group)
    {
        groups.Add(group);
    }

    public bool RemoveGroup(Group group)
    {
        return groups.Remove(group);
    }

    public void MergeGroups()
    {
        Console.Clear();
        Console.WriteLine("Enter first group number: ");
        string fgr = Console.ReadLine();
        Console.WriteLine("Enter second group number: ");
        string sgr = Console.ReadLine();
        Group newgr = new Group(fgr);
        bool ffound = false;
        bool sfound = false;
        foreach (Group gr in this)
        {
            if (gr.Number == fgr)
            {
                foreach (Student st in gr)
                {
                    newgr.AddStudent(st);
                }
                ffound = true;
            }
            if (gr.Number == sgr)
            {
                foreach (Student st in gr)
                {
                    newgr.AddStudent(st);
                }
                sfound = true;
            }
        }

        if (!ffound)
        {
            Console.Clear();
            Console.WriteLine("Group '" + fgr + "' was not found!");
        }
        else if (!sfound)
        {
            Console.Clear();
            Console.WriteLine("Group '" + sgr + "' was not found!");
        }
        else
        {
            foreach (Group gr in this)
            {
                if (gr.Number == fgr)
                {
                    this.RemoveGroup(gr);
                    break;
                }
            }
            foreach (Group gr in this)
            {
                if (gr.Number == sgr)
                {
                    this.RemoveGroup(gr);
                    break;
                }
            }
            this.AddGroup(newgr);
            Console.Clear();
            Console.WriteLine("Groups were successfully merged!");
        }
    }

    public void ShowFaculty()
    {
        Console.Clear();
        Console.WriteLine(this.Name + ": \t(" + this.Count + " group(s))");

        foreach (Group gr in this)
        {
            Console.WriteLine("  " + gr.Number + ": \t(" + gr.Count + " student(s))");
            foreach (Student st in gr)
                Console.WriteLine("    " + st.Name + " " + st.Surname);
        }
    }

    public void SearchStudent()
    {
        Console.Clear();
        Console.WriteLine("Enter student's name: ");
        string name = Console.ReadLine();
        Console.WriteLine("Enter student's surname: ");
        string surname = Console.ReadLine();
        bool found = false;
        foreach (Group gr in this)
        {
            foreach (Student st in gr)
            {
                if (st.Name == name && st.Surname == surname)
                {
                    
                    Console.Clear();
                    Console.WriteLine("Search results:");
                    Console.WriteLine("\n\t" + st.Name + " " + st.Surname + " " + st.Group);
                    found = true;
                    break;
                }
            }
        }
        if (!found)
        {
            Console.Clear();
            Console.WriteLine("Student '" + name + " " + surname + "' was not found!");
        }
    }

    public void TransferStudent()
    {
        Console.Clear();
        Console.WriteLine("Enter student's name: ");
        string name = Console.ReadLine();
        Console.WriteLine("Enter student's surname: ");
        string surname = Console.ReadLine();
        Console.WriteLine("Enter group number where to transfer:");
        string number = Console.ReadLine();
        bool foundst = false;
        bool foundgr = false;
        Student transferedStudent = new Student("", "");
        foreach (Group gr in this)
        {
            foreach (Student st in gr)
            {
                if (st.Name == name && st.Surname == surname)
                {
                    transferedStudent.Name = st.Name;
                    transferedStudent.Surname = st.Surname;
                    transferedStudent.Group = st.Group;
                    gr.ExpellStudent(st);
                    foundst = true;
                    break;
                }
            }
        }
        if (!foundst)
        {
            Console.Clear();
            Console.WriteLine("Student '" + name + " " + surname + "' was not found!");
            goto exit;
        }
        foreach (Group gr in this)
        {
            if (gr.Number == number)
            {
                foundgr = true;
                gr.AddStudent(transferedStudent);
                Console.Clear();
                Console.WriteLine("Student was successfully transfered!");
                break;
            }
        }
        if (!foundgr)
        {
            Console.Clear();
            Console.WriteLine("Group '" + number + "' was not found!");
        }
    exit: ;
    }

    public IEnumerator<Group> GetEnumerator()
    {
        foreach (Group g in groups)
        {
            yield return g;
        }
    }

    IEnumerator IEnumerable.GetEnumerator()
    {
        return GetEnumerator();
    }
}

class Program
{
    static void Main(string[] args)
    {
        Faculty fac = new Faculty("FCSaN");
        Group gr1 = new Group("453501");
        Group gr2 = new Group("453502");
        Group gr3 = new Group("453503");
        Student st11 = new Student("Alex", "Martynov");
        Student st12 = new Student("Geralt", "from Rivia");
        Student st21 = new Student("Harry", "Potter");
        Student st22 = new Student("Katniss", "Everdeen");
        Student st31 = new Student("Jesse", "Pinkman");
        Student st32 = new Student("Leonardo", "DiCaprio");

        
        ConsoleKeyInfo ch;

        gr1.AddStudent(st11);
        gr1.AddStudent(st12);
        gr2.AddStudent(st21);
        gr2.AddStudent(st22);
        gr3.AddStudent(st31);
        gr3.AddStudent(st32);

        fac.AddGroup(gr1);
        fac.AddGroup(gr2);
        fac.AddGroup(gr3);

        while (true)
        {
            Console.Clear();
            Console.WriteLine("Select: ");
            Console.WriteLine();
            Console.WriteLine("1 - Show the list of groups");
            Console.WriteLine("2 - Add a group");
            Console.WriteLine("3 - Add a student");
            Console.WriteLine("4 - Remove a group");
            Console.WriteLine("5 - Expell a student");
            Console.WriteLine("6 - Transfer student");
            Console.WriteLine("7 - Search for student");
            Console.WriteLine("8 - Merge groups");

            Console.WriteLine("0 - Exit");
            ch = Console.ReadKey();

            if (ch.KeyChar == '1')
            {
                fac.ShowFaculty();
            }

            if (ch.KeyChar == '2')
            {
                Console.Clear();
                Console.WriteLine("Enter group number:");
                string number = Console.ReadLine();
                fac.AddGroup(new Group(number));
                Console.WriteLine("Press any key...");
            }

            if (ch.KeyChar == '3')
            {
                Console.Clear();
                Console.WriteLine("Enter student's name: ");
                string name = Console.ReadLine();
                Console.WriteLine("Enter student's surname: ");
                string surname = Console.ReadLine();
                Console.WriteLine("Enter group number: ");
                string number = Console.ReadLine();
                bool found = false;
                foreach (Group gr in fac)
                {
                    if (gr.Number == number)
                    {
                        gr.AddStudent(new Student(name, surname));
                        found = true;
                        break;
                    }
                }
                if (!found)
                {
                    Group newgr = new Group(number);
                    newgr.AddStudent(new Student(name, surname));
                    fac.AddGroup(newgr);
                }
                Console.WriteLine("Press any key...");
            }

            if (ch.KeyChar == '4')
            {
                Console.Clear();
                Console.WriteLine("Enter group number: ");
                string number = Console.ReadLine();
                bool found = false;
                foreach (Group gr in fac)
                {
                    if (gr.Number == number)
                    {
                        fac.RemoveGroup(gr);
                        found = true;
                        Console.Clear();
                        Console.WriteLine("Group was successfully removed!");
                        break;
                    }
                }
                if (!found)
                {
                    Console.Clear();
                    Console.WriteLine("Group '" + number + "' was not found!");
                }
            }

            if (ch.KeyChar == '5')
            {
                Console.Clear();
                Console.WriteLine("Enter student's name: ");
                string name = Console.ReadLine();
                Console.WriteLine("Enter student's surname: ");
                string surname = Console.ReadLine();
                bool found = false;
                foreach (Group gr in fac)
                {
                    foreach (Student st in gr)
                    {
                        if (st.Name == name && st.Surname == surname)
                        {
                            gr.ExpellStudent(st);
                            found = true;
                            Console.Clear();
                            Console.WriteLine("Student was successfully removed!");
                            break;
                        }
                    }
                }
                if (!found)
                {
                    Console.Clear();
                    Console.WriteLine("Student '" + name + " " + surname + "' was not found!");
                    break;
                }
            }

            if (ch.KeyChar == '6')
            {
                fac.TransferStudent();
            }

            if (ch.KeyChar == '7')
            {
                fac.SearchStudent();
            }

            if (ch.KeyChar == '8')
            {
                fac.MergeGroups();
            }

            if (ch.KeyChar == '0')
            {
                break;
            }
            Console.ReadKey();
        }
    }
}