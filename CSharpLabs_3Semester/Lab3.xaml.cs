using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.Specialized;
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

namespace Lab3
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        Faculty fac = new Faculty();
        Group first;
        bool buttonChooseClicked = false;

        public MainWindow()
        {
            InitializeComponent();
            listbox1.ItemsSource = fac;
            textblock1.Visibility = Visibility.Hidden;
            textblock2.Visibility = Visibility.Hidden;
            textblock3.Visibility = Visibility.Hidden;
            textbox1.Visibility = Visibility.Hidden;
            textbox2.Visibility = Visibility.Hidden;
            textbox3.Visibility = Visibility.Hidden;
            buttonCancel.Visibility = Visibility.Hidden;
            buttonChoose.Visibility = Visibility.Hidden;
            Group gr1 = new Group("453501");
            Group gr2 = new Group("453502");
            Group gr3 = new Group("453503");
            Student st1 = new Student("Alex", "Martynov");
            Student st2 = new Student("Ivan", "Petrov");
            Student st3 = new Student("Harry", "Potter");
            Student st4 = new Student("Jesse", "Pinkman");
            Student st5 = new Student("Lev", "Tolstoy");
            Student st6 = new Student("Sample", "Text");
            listbox2.ItemsSource = gr1;
            gr1.AddStudent(st1);
            gr1.AddStudent(st2);
            listbox2.ItemsSource = gr2;
            gr2.AddStudent(st3);
            gr2.AddStudent(st4);
            listbox2.ItemsSource = gr3;
            gr3.AddStudent(st5);
            gr3.AddStudent(st6);
            listbox2.ItemsSource = null;
            fac.AddGroup(gr1);
            fac.AddGroup(gr2);
            fac.AddGroup(gr3);
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
                textbox1.Visibility = Visibility.Visible;
                textbox2.Visibility = Visibility.Visible;
                button2.Content = "Add";
            }
            else
            {
                if (textbox1.Text != "" && textbox2.Text != "" && listbox1.SelectedItem != null)
                {
                    Group selectedGroup = (Group)listbox1.SelectedItem;
                    selectedGroup.AddStudent(new Student(textbox1.Text, textbox2.Text));
                    listbox1.UnselectAll();
                    textblock1.Visibility = Visibility.Hidden;
                    textblock2.Visibility = Visibility.Hidden;
                    textbox1.Visibility = Visibility.Hidden;
                    textbox2.Visibility = Visibility.Hidden;
                    buttonCancel.Visibility = Visibility.Hidden;
                    textbox1.Text = "";
                    textbox2.Text = "";
                    button2.Content = "Add student";
                }
                else
                {
                    MessageBox.Show("All fields must be filled and group must be choosed!");
                }
            }
        }

        private void button4_Click(object sender, RoutedEventArgs e)
        {
            if (listbox1.SelectedItem != null)
            {
                Group selectedGroup = (Group)listbox1.SelectedItem;
                listbox2.ItemsSource = null;
                fac.RemoveGroup(selectedGroup);
            }
        }

        private void button3_Click(object sender, RoutedEventArgs e)
        {
            if (listbox1.SelectedItem != null && listbox2.SelectedItem != null)
            {
                Group selectedGroup = (Group)listbox1.SelectedItem;
                Student selectedStudent = (Student)listbox2.SelectedItem;
                selectedGroup.ExpellStudent(selectedStudent);
                listbox2.UnselectAll();
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
            textbox1.Visibility = Visibility.Hidden;
            textbox2.Visibility = Visibility.Hidden;
            textbox3.Visibility = Visibility.Hidden;
            buttonChoose.Visibility = Visibility.Hidden;
            buttonCancel.Visibility = Visibility.Hidden;
            listbox1.UnselectAll();
            listbox2.UnselectAll();
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
                textbox3.Text = "";
                buttonCancel.Visibility = Visibility.Visible;
                textblock3.Visibility = Visibility.Visible;
                textbox3.Visibility = Visibility.Visible;
                button6.Content = "Transfer";
            }
            else
            {
                if (listbox1.SelectedItem != null && listbox2.SelectedItem != null && textbox3.Text != "")
                {
                    Student selectedStudent = (Student)listbox2.SelectedItem;
                    fac.TransferStudent(selectedStudent, textbox3.Text);
                    textblock3.Visibility = Visibility.Hidden;
                    textbox3.Visibility = Visibility.Hidden;
                    buttonCancel.Visibility = Visibility.Hidden;
                    textbox3.Text = "";
                    button6.Content = "Transfer student";
                    listbox1.UnselectAll();
                    listbox2.UnselectAll();
                }
                else
                {
                    MessageBox.Show("All fields must be filled and student must be choosed!");
                }
            }
        }

        private void button7_Click(object sender, RoutedEventArgs e)
        {
            if ((string)button7.Content != "Merge")
            {
                buttonCancel.Visibility = Visibility.Visible;
                buttonChoose.Visibility = Visibility.Visible;
                button7.Content = "Merge";
            }
            else
            {
                if (listbox1.SelectedItem != null && buttonChooseClicked)
                {
                    Group second = (Group)listbox1.SelectedItem;
                    if (first.Number != second.Number)
                    {
                        fac.MergeGroups(first, second);
                    }
                    buttonCancel.Visibility = Visibility.Hidden;
                    buttonChoose.Visibility = Visibility.Hidden;
                    listbox1.UnselectAll();
                    button7.Content = "Merge groups";
                }
                else
                {
                    MessageBox.Show("Groups must be choosed!");
                }
            }
        }

        private void listbox1_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (listbox1.SelectedItem != null)
            {
                Group selectedGroup = (Group)listbox1.SelectedItem;
                listbox2.ItemsSource = selectedGroup;
            }
        }

        private void buttonChoose_Click(object sender, RoutedEventArgs e)
        {
            if (listbox1.SelectedItem != null)
            {
                first = (Group)listbox1.SelectedItem;
                buttonChoose.Visibility = Visibility.Hidden;
                buttonChooseClicked = true;
                listbox1.UnselectAll();
            }
        }
    }

    public class Student
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

    public class Group : IEnumerable<Student>, INotifyCollectionChanged
    {
        public List<Student> students;
        public event NotifyCollectionChangedEventHandler CollectionChanged;

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
            CollectionChanged(this, new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Reset));
        }

        public void ExpellStudent(Student student)
        {
            students.Remove(student);
            CollectionChanged(this, new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Reset));
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

    public class Faculty : IEnumerable<Group>, INotifyCollectionChanged
    {
        public List<Group> groups = new List<Group>();
        public event NotifyCollectionChangedEventHandler CollectionChanged;

        public int Count
        {
            get { return groups.Count; }
        }

        public void AddGroup(Group group)
        {
            bool found = false;
            foreach (Group gr in this)
            {
                if (gr.Number == group.Number)
                {
                    MessageBox.Show("Such group already exists!");
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                groups.Add(group);
                CollectionChanged(this, new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Reset));
            }
        }

        public void RemoveGroup(Group group)
        {
            groups.Remove(group);
            CollectionChanged(this, new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Reset));
        }

        public void MergeGroups(Group first, Group second)
        {
            for (int i = 0; i < second.Count; i++)
            {
                first.AddStudent(second.students[i]);
            }
            this.RemoveGroup(second);
            CollectionChanged(this, new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Reset));
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

        public void TransferStudent(Student student, string number)
        {
            Student transferedStudent = new Student("", "");
            bool found = false;
            foreach (Group gr in this)
            {
                if (gr.Number == number)
                {
                    found = true;
                    break;
                }
            }
            if (found)
            {
                foreach (Group gr in this)
                {
                    foreach (Student st in gr)
                    {
                        if (st.Name == student.Name && st.Surname == student.Surname)
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
                CollectionChanged(this, new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Reset));
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