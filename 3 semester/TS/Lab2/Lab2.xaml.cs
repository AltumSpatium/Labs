using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Lab2
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
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

        public MainWindow()
        {
            InitializeComponent();

            gr1.AddStudent(st11);
            gr1.AddStudent(st12);
            gr2.AddStudent(st21);
            gr2.AddStudent(st22);
            gr3.AddStudent(st31);
            gr3.AddStudent(st32);

            fac.AddGroup(gr1);
            fac.AddGroup(gr2);
            fac.AddGroup(gr3);
            textblock1.Visibility = Visibility.Hidden;
            textblock2.Visibility = Visibility.Hidden;
            textblock3.Visibility = Visibility.Hidden;
            textblock4.Visibility = Visibility.Hidden;
            textblock5.Visibility = Visibility.Hidden;
            textbox1.Visibility = Visibility.Hidden;
            textbox2.Visibility = Visibility.Hidden;
            textbox3.Visibility = Visibility.Hidden;
            textbox4.Visibility = Visibility.Hidden;
            buttonCancel.Visibility = Visibility.Hidden;
            fac.ShowFaculty(listview);
        }

        private void button1_Click(object sender, RoutedEventArgs e)
        {
            if ((string)button1.Content != "Add")
            {
                textbox3.Text = "";
                buttonCancel.Visibility = Visibility.Visible;
                textblock3.Visibility = Visibility.Visible;
                textbox3.Visibility = Visibility.Visible;
                button1.Content = "Add";
            }
            else
            {
                if (textbox3.Text != "")
                {
                    fac.AddGroup(new Group(textbox3.Text));
                    textblock3.Visibility = Visibility.Hidden;
                    textbox3.Visibility = Visibility.Hidden;
                    buttonCancel.Visibility = Visibility.Hidden;
                    textbox3.Text = "";
                    listview.Items.Clear();
                    fac.ShowFaculty(listview);
                    button1.Content = "Add group";
                }
                else
                {
                    MessageBox.Show("All fields must be filled!");
                }
            }
        }

        private void button2_Click(object sender, RoutedEventArgs e)
        {
            if ((string)button2.Content != "Add")
            {
                textbox1.Text = "";
                textbox2.Text = "";
                textbox3.Text = "";
                buttonCancel.Visibility = Visibility.Visible;
                textblock1.Visibility = Visibility.Visible;
                textblock2.Visibility = Visibility.Visible;
                textblock3.Visibility = Visibility.Visible;
                textbox1.Visibility = Visibility.Visible;
                textbox2.Visibility = Visibility.Visible;
                textbox3.Visibility = Visibility.Visible;
                button2.Content = "Add";
            }
            else
            {
                if (textbox1.Text != "" && textbox2.Text != "" && textbox3.Text != "")
                {
                    bool found = false;
                    foreach (Group gr in fac)
                    {
                        if (gr.Number == textbox3.Text)
                        {
                            gr.AddStudent(new Student(textbox1.Text, textbox2.Text));
                            found = true;
                            break;
                        }
                    }
                    if (!found)
                    {
                        Group newgr = new Group(textbox3.Text);
                        newgr.AddStudent(new Student(textbox1.Text, textbox2.Text));
                        fac.AddGroup(newgr);
                    }
                    textblock1.Visibility = Visibility.Hidden;
                    textblock2.Visibility = Visibility.Hidden;
                    textblock3.Visibility = Visibility.Hidden;
                    textbox1.Visibility = Visibility.Hidden;
                    textbox2.Visibility = Visibility.Hidden;
                    textbox3.Visibility = Visibility.Hidden;
                    buttonCancel.Visibility = Visibility.Hidden;
                    textbox1.Text = "";
                    textbox2.Text = "";
                    textbox3.Text = "";
                    listview.Items.Clear();
                    fac.ShowFaculty(listview);
                    button2.Content = "Add student";
                }
                else
                {
                    MessageBox.Show("All fields must be filled!");
                }
            }
        }

        private void button4_Click(object sender, RoutedEventArgs e)
        {
            if ((string)button4.Content != "Remove")
            {
                textbox3.Text = "";
                buttonCancel.Visibility = Visibility.Visible;
                textblock3.Visibility = Visibility.Visible;
                textbox3.Visibility = Visibility.Visible;
                button4.Content = "Remove";
            }
            else
            {
                if (textbox3.Text != "")
                {
                    bool found = false;
                    foreach (Group gr in fac)
                    {
                        if (gr.Number == textbox3.Text)
                        {
                            fac.RemoveGroup(gr);
                            found = true;
                            break;
                        }
                    }
                    if (!found)
                    {
                        MessageBox.Show("Group " + textbox3.Text + " was not found!");
                    }
                    textblock3.Visibility = Visibility.Hidden;
                    textbox3.Visibility = Visibility.Hidden;
                    buttonCancel.Visibility = Visibility.Hidden;
                    textbox3.Text = "";
                    listview.Items.Clear();
                    fac.ShowFaculty(listview);
                    button4.Content = "Remove group";
                }
                else
                {
                    MessageBox.Show("All fields must be filled!");
                }
            }
        }

        private void button3_Click(object sender, RoutedEventArgs e)
        {
            if ((string)button3.Content != "Expell")
            {
                textbox1.Text = "";
                textbox2.Text = "";
                buttonCancel.Visibility = Visibility.Visible;
                textblock1.Visibility = Visibility.Visible;
                textblock2.Visibility = Visibility.Visible;
                textbox1.Visibility = Visibility.Visible;
                textbox2.Visibility = Visibility.Visible;
                button3.Content = "Expell";
            }
            else
            {
                if (textbox1.Text != "" && textbox2.Text != "")
                {
                    bool found = false;
                    foreach (Group gr in fac)
                    {
                        foreach (Student st in gr)
                        {
                            if (st.Name == textbox1.Text && st.Surname == textbox2.Text)
                            {
                                gr.ExpellStudent(st);
                                found = true;
                                break;
                            }
                        }
                    }
                    if (!found)
                    {
                        MessageBox.Show("Student " + textbox1.Text + " " + textbox2.Text + " was not found!");
                    }
                    textblock1.Visibility = Visibility.Hidden;
                    textblock2.Visibility = Visibility.Hidden;
                    textbox1.Visibility = Visibility.Hidden;
                    textbox2.Visibility = Visibility.Hidden;
                    buttonCancel.Visibility = Visibility.Hidden;
                    textbox1.Text = "";
                    textbox2.Text = "";
                    listview.Items.Clear();
                    fac.ShowFaculty(listview);
                    button3.Content = "Expell student";
                }
                else
                {
                    MessageBox.Show("All fields must be filled!");
                }
            }
            
        }

        private void buttonCancel_Click(object sender, RoutedEventArgs e)
        {
            button1.Content = "Add group";
            button2.Content = "Add student";
            button3.Content = "Expell student";
            button4.Content = "Remove group";
            button5.Content = "Search for student";
            button6.Content = "Transfer student";
            button7.Content = "Merge groups";
            textblock1.Visibility = Visibility.Hidden;
            textblock2.Visibility = Visibility.Hidden;
            textblock3.Visibility = Visibility.Hidden;
            textblock4.Visibility = Visibility.Hidden;
            textblock5.Visibility = Visibility.Hidden;
            textbox1.Visibility = Visibility.Hidden;
            textbox2.Visibility = Visibility.Hidden;
            textbox3.Visibility = Visibility.Hidden;
            textbox4.Visibility = Visibility.Hidden;
            buttonCancel.Visibility = Visibility.Hidden;
            listview.Items.Clear();
            fac.ShowFaculty(listview);
        }


        private void button5_Click(object sender, RoutedEventArgs e)
        {
            if ((string)button5.Content != "Search")
            {
                textbox1.Text = "";
                textbox2.Text = "";
                buttonCancel.Visibility = Visibility.Visible;
                textblock1.Visibility = Visibility.Visible;
                textblock2.Visibility = Visibility.Visible;
                textbox1.Visibility = Visibility.Visible;
                textbox2.Visibility = Visibility.Visible;
                button5.Content = "Search";
            }
            else
            {
                if (textbox1.Text != "" && textbox2.Text != "")
                {
                    Student st = fac.SearchStudent(textbox1.Text, textbox2.Text);
                    if (st != null)
                    {
                        MessageBox.Show(st.Name + " " + st.Surname + " " + st.Group);
                    }
                    else
                    {
                        MessageBox.Show("Student " + textbox1.Text + " " + textbox2.Text + " was not found!");
                    }
                    textbox1.Text = "";
                    textbox2.Text = "";
                    textbox1.Visibility = Visibility.Hidden;
                    textbox2.Visibility = Visibility.Hidden;
                    textblock1.Visibility = Visibility.Hidden;
                    textblock2.Visibility = Visibility.Hidden;
                    buttonCancel.Visibility = Visibility.Hidden;
                    button5.Content = "Search for student";
                }
                else
                {
                    MessageBox.Show("All fields must be filled!");
                }
            }
        }

        private void button6_Click(object sender, RoutedEventArgs e)
        {
            if ((string)button6.Content != "Transfer")
            {
                textbox1.Text = "";
                textbox2.Text = "";
                textbox3.Text = "";
                buttonCancel.Visibility = Visibility.Visible;
                textblock1.Visibility = Visibility.Visible;
                textblock2.Visibility = Visibility.Visible;
                textblock3.Visibility = Visibility.Visible;
                textbox1.Visibility = Visibility.Visible;
                textbox2.Visibility = Visibility.Visible;
                textbox3.Visibility = Visibility.Visible;
                button6.Content = "Transfer";
            }
            else
            {
                if (textbox1.Text != "" && textbox2.Text != "" && textbox3.Text != "")
                {
                    fac.TransferStudent(textbox1.Text, textbox2.Text, textbox3.Text);
                    textblock1.Visibility = Visibility.Hidden;
                    textblock2.Visibility = Visibility.Hidden;
                    textblock3.Visibility = Visibility.Hidden;
                    textbox1.Visibility = Visibility.Hidden;
                    textbox2.Visibility = Visibility.Hidden;
                    textbox3.Visibility = Visibility.Hidden;
                    buttonCancel.Visibility = Visibility.Hidden;
                    textbox1.Text = "";
                    textbox2.Text = "";
                    textbox3.Text = "";
                    listview.Items.Clear();
                    fac.ShowFaculty(listview);
                    button6.Content = "Transfer student";
                }
                else
                {
                    MessageBox.Show("All fields must be filled!");
                }
            }
        }

        private void button7_Click(object sender, RoutedEventArgs e)
        {
            if ((string)button7.Content != "Merge")
            {
                textbox3.Text = "";
                textbox4.Text = "";
                buttonCancel.Visibility = Visibility.Visible;
                textblock4.Visibility = Visibility.Visible;
                textblock5.Visibility = Visibility.Visible;
                textbox3.Visibility = Visibility.Visible;
                textbox4.Visibility = Visibility.Visible;
                button7.Content = "Merge";
            }
            else
            {
                if (textbox3.Text != "" && textbox4.Text != "")
                {
                    fac.MergeGroups(textbox3.Text, textbox4.Text);
                    textblock4.Visibility = Visibility.Hidden;
                    textblock5.Visibility = Visibility.Hidden;
                    textbox3.Visibility = Visibility.Hidden;
                    textbox4.Visibility = Visibility.Hidden;
                    buttonCancel.Visibility = Visibility.Hidden;
                    textbox3.Text = "";
                    textbox4.Text = "";
                    listview.Items.Clear();
                    fac.ShowFaculty(listview);
                    button7.Content = "Merge groups";
                }
                else
                {
                    MessageBox.Show("All fields must be filled!");
                }
            }
        }
    }

    class Student
    {
        public string Name { get; set; }

        public string Surname { get; set; }

        public string Group { get; set; }

        public Student(string name, string surname)
        {
            Name = name;
            Surname = surname;
        }
    }

    class Group : IEnumerable<Student>
    {
        public List<Student> students;

        public string Number { get; set; }

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
            student.Group = Number;
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
        public List<Group> groups;

        public string Name { get; set; }

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

        public void MergeGroups(string fgr, string sgr)
        {
            Group newgr = new Group(fgr);
            foreach (Group gr in this)
            {
                if (gr.Number == fgr)
                {
                    foreach (Student st in gr)
                    {
                        newgr.AddStudent(st);
                    }
                }
                if (gr.Number == sgr)
                {
                    foreach (Student st in gr)
                    {
                        newgr.AddStudent(st);
                    }
                }
            }
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
        }

        public void ShowFaculty(ListView listview)
        {
            listview.Items.Add(this.Name + ":   (" + this.Count + " group(s))");
            foreach (Group gr in this)
            {
                listview.Items.Add("  " + gr.Number + ":   (" + gr.Count + " student(s))");
                foreach (Student st in gr)
                    listview.Items.Add("    " + st.Name + " " + st.Surname);
            }
        }

        public Student SearchStudent(string name, string surname)
        {
            foreach (Group gr in this)
            {
                foreach (Student st in gr)
                {
                    if (st.Name == name && st.Surname == surname)
                    {
                        return st;
                    }
                }
            }
            return null;
        }

        public void TransferStudent(string name, string surname, string number)
        {
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
                        break;
                    }
                }
            }
            foreach (Group gr in this)
            {
                if (gr.Number == number)
                {
                    gr.AddStudent(transferedStudent);
                    break;
                }
            }
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
}