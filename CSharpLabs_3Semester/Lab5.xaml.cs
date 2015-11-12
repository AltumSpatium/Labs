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
using System.IO;
using System.IO.Compression;
using Microsoft.Win32;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Formatters.Binary;
using System.Xml.Serialization;
using System.Reflection;

namespace Lab5
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

        private void button1_Click(object sender, RoutedEventArgs e) // Add a group to faculty
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

        private void button2_Click(object sender, RoutedEventArgs e) // Add a student to group
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

        private void button4_Click(object sender, RoutedEventArgs e) // Remove group from faculty
        {
            if (listbox1.SelectedItem != null)
            {
                Group selectedGroup = (Group)listbox1.SelectedItem;
                listbox2.ItemsSource = null;
                fac.RemoveGroup(selectedGroup);
            }
        }

        private void button3_Click(object sender, RoutedEventArgs e) // Expell student from group
        {
            if (listbox1.SelectedItem != null && listbox2.SelectedItem != null)
            {
                Group selectedGroup = (Group)listbox1.SelectedItem;
                Student selectedStudent = (Student)listbox2.SelectedItem;
                selectedGroup.ExpellStudent(selectedStudent);
                listbox2.UnselectAll();
            }
        }

        private void buttonCancel_Click(object sender, RoutedEventArgs e) // Cancel
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


        private void button5_Click(object sender, RoutedEventArgs e) // Search for a student
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

        private void button6_Click(object sender, RoutedEventArgs e) // Transfer student from one group to another
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

        private void button7_Click(object sender, RoutedEventArgs e) // Merge two groups
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

        private void buttonChoose_Click(object sender, RoutedEventArgs e) // Choose first group in "Merge groups" method
        {
            if (listbox1.SelectedItem != null)
            {
                first = (Group)listbox1.SelectedItem;
                buttonChoose.Visibility = Visibility.Hidden;
                buttonChooseClicked = true;
                listbox1.UnselectAll();
            }
        }

        private void buttonClearFaculty_Click(object sender, RoutedEventArgs e) // Delete all students and all groups
        {
            foreach (Group gr in fac)
            {
                gr.ClearGroup();
            }
            fac.ClearFaculty();
        }

        private void buttonSaveToFile_Click(object sender, RoutedEventArgs e) // Save collection to file .txt/.data
        {
            if ((bool)radiobutton1.IsChecked)
            {
                SaveFileDialog fileDialog = new SaveFileDialog();
                string filename;

                fileDialog.DefaultExt = ".txt";
                fileDialog.Filter = "Text files (txt)|*.txt";
                if (fileDialog.ShowDialog() == true)
                    filename = fileDialog.FileName;
                else
                    return;
                FileStream fs;
                if (!File.Exists(filename))
                    fs = new FileStream(filename, FileMode.CreateNew);
                else
                    fs = new FileStream(filename, FileMode.Create);
                StreamWriter sw = new StreamWriter(fs);
                sw.WriteLine(fac.Count);
                foreach (Group gr in fac)
                {
                    sw.WriteLine(gr.Number);
                    sw.WriteLine(gr.Count);
                    foreach (Student st in gr)
                    {
                        sw.WriteLine(st.Name);
                        sw.WriteLine(st.Surname);
                    }
                }
                sw.Close();
                fs.Close();
            }
            if ((bool)radiobutton2.IsChecked)
            {
                SaveFileDialog fileDialog = new SaveFileDialog();
                string filename;

                fileDialog.DefaultExt = ".data";
                fileDialog.Filter = "Binary files (data)|*.data";
                if (fileDialog.ShowDialog() == true)
                    filename = fileDialog.FileName;
                else
                    return;
                FileStream fs = new FileStream(filename, FileMode.CreateNew);
                DeflateStream ds = new DeflateStream(fs, CompressionMode.Compress);
                BinaryWriter bw = new BinaryWriter(ds);
                bw.Write(fac.Count);
                foreach (Group gr in fac)
                {
                    bw.Write(gr.Number);
                    bw.Write(gr.Count);
                    foreach (Student st in gr)
                    {
                        bw.Write(st.Name);
                        bw.Write(st.Surname);
                    }
                }
                bw.Close();
                ds.Close();
                fs.Close();
            }
        }

        private void buttonReadFromFile_Click(object sender, RoutedEventArgs e) // Read collection from file .txt/.data
        {
            if ((bool)radiobutton1.IsChecked)
            {
                OpenFileDialog fileDialog = new OpenFileDialog();
                string filename;

                fileDialog.DefaultExt = ".txt";
                fileDialog.Filter = "Text documents (txt)|*.txt";
                if (fileDialog.ShowDialog() == true)
                    filename = fileDialog.FileName;
                else
                    return;
                fac.ClearFaculty();
                FileStream fs = new FileStream(filename, FileMode.Open);
                StreamReader sr = new StreamReader(fs);
                int groupCount = Convert.ToInt32(sr.ReadLine());
                for (int i = 0; i < groupCount; i++)
                {
                    string groupNumber = sr.ReadLine();
                    Group tempGroup = new Group(groupNumber);
                    fac.AddGroup(tempGroup);
                    int studentCount = Convert.ToInt32(sr.ReadLine());
                    for (int j = 0; j < studentCount; j++)
                    {
                        string studentName = sr.ReadLine();
                        string studentSurname = sr.ReadLine();
                        tempGroup.AddStudent(new Student(studentName, studentSurname));
                    }
                }
                sr.Close();
                fs.Close();
            }
            if ((bool)radiobutton2.IsChecked)
            {
                OpenFileDialog fileDialog = new OpenFileDialog();
                string filename;

                fileDialog.DefaultExt = ".data";
                fileDialog.Filter = "Text documents (data)|*.data";
                if (fileDialog.ShowDialog() == true)
                    filename = fileDialog.FileName;
                else
                    return;
                fac.ClearFaculty();
                FileStream fs = new FileStream(filename, FileMode.Open);
                DeflateStream ds = new DeflateStream(fs, CompressionMode.Decompress);
                BinaryReader br = new BinaryReader(ds);
                int groupCount = br.ReadInt32();
                for (int i = 0; i < groupCount; i++)
                {
                    string groupNumber = br.ReadString();
                    Group tempGroup = new Group(groupNumber);
                    fac.AddGroup(tempGroup);
                    int studentCount = br.ReadInt32();
                    for (int j = 0; j < studentCount; j++)
                    {
                        string studentName = br.ReadString();
                        string studentSurname = br.ReadString();
                        tempGroup.AddStudent(new Student(studentName, studentSurname));
                    }
                }
                br.Close();
                ds.Close();
                fs.Close();
            }
        }

        private void buttonSerialize_Click(object sender, RoutedEventArgs e)
        {
            SaveFileDialog fileDialog = new SaveFileDialog();
            string filename;

            fileDialog.DefaultExt = ".txt";
            fileDialog.Filter = "Text files (txt)|*.txt";
            if (fileDialog.ShowDialog() == true)
                filename = fileDialog.FileName;
            else
                return;
            Serializer sl = new Serializer(filename, true);
            sl.WriteObject(fac);
            sl.Close();
        }

        private void buttonDeserialize_Click(object sender, RoutedEventArgs e)
        {
            OpenFileDialog fileDialog = new OpenFileDialog();
            string filename;

            fileDialog.DefaultExt = ".txt";
            fileDialog.Filter = "Text documents (txt)|*.txt";
            if (fileDialog.ShowDialog() == true)
                filename = fileDialog.FileName;
            else
                return;
            fac.ClearFaculty();
            Serializer sl = new Serializer(filename, false);
            fac = (Faculty)sl.ReadObject();
            sl.Close();
            listbox1.ItemsSource = fac;
        }
    }

    public class Student
    {
        public string Name { get; set; }

        public string Surname { get; set; }

        public string Group { get; set; }

        public Student()
        {
            Name = "";
            Surname = "";
        }

        public Student(string name, string surname)
        {
            Name = name;
            Surname = surname;
        }
    }

    public class Group : IEnumerable<Student>, INotifyCollectionChanged
    {
        public List<Student> students = new List<Student>();
        public event NotifyCollectionChangedEventHandler CollectionChanged;

        public string Number { get; set; }

        public Group()
        {
            Number = "";
        }

        public Group(string number)
        {
            Number = number;            
        }

        public int Count
        {
            get { return students.Count; }
        }

        public void AddStudent(Student student)
        {
            students.Add(student);
            student.Group = Number;
            if (CollectionChanged != null)
                CollectionChanged(this, new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Reset));
        }

        public void ExpellStudent(Student student)
        {
            students.Remove(student);
            if (CollectionChanged != null)
                CollectionChanged(this, new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Reset));
        }

        public void ClearGroup()
        {
            students.Clear();
            if (CollectionChanged != null)
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
        public string name = "FCSaN";
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
                if (CollectionChanged != null)
                    CollectionChanged(this, new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Reset));
            }
        }

        public void RemoveGroup(Group group)
        {
            groups.Remove(group);
            if (CollectionChanged != null)
                CollectionChanged(this, new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Reset));
        }

        public void ClearFaculty()
        {
            groups.Clear();
            if (CollectionChanged != null)
                CollectionChanged(this, new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Reset));
        }

        public void MergeGroups(Group first, Group second)
        {
            for (int i = 0; i < second.Count; i++)
            {
                first.AddStudent(second.students[i]);
            }
            this.RemoveGroup(second);
            if (CollectionChanged != null)
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
                if (CollectionChanged != null)
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

    class Serializer
    {
        public FileStream stream = null;
        protected StreamWriter writer = null;
        protected StreamReader reader = null;
        protected int spaces = 0;

        public Serializer(string _filename, bool _storemode)
        {
            if (_storemode)
            {
                stream = new FileStream(_filename, FileMode.Create);
                writer = new StreamWriter(stream);
            }
            else
            {
                stream = new FileStream(_filename, FileMode.Open);
                reader = new StreamReader(stream);
            }
        }

        public void Close()
        {
            if (writer != null)
            {
                writer.Close();
                writer = null;
            }

            if (reader != null)
            {
                reader.Close();
                reader = null;
            }

            if (stream != null)
            {
                stream.Close();
                stream = null;
            }
        }

        protected void WriteLine(string _key, string _line, bool _useconvert)
        {
            if (spaces > 0)
                writer.Write(new string(' ', spaces << 1));

            if (_useconvert)
            {
                _line = _line.Replace("&", "&amp;");
                _line = _line.Replace("\n", "&ln;");
                _line = _line.Replace("\r", "&lf;");
                _line = _line.Replace("{", "&bl;");
                _line = _line.Replace("}", "&br;");
                _line = _line.Replace("[", "&al;");
                _line = _line.Replace("]", "&ar;");
                _line = _line.Replace("(", "&ul;");
                _line = _line.Replace(")", "&ur;");
                _line = _line.Replace("=", "&eq;");
            }

            if (_key != null)
                writer.Write(_key + "=");

            writer.WriteLine(_line);
        }

        protected string ReadLine(out string _key, bool _useconvert)
        {
            string line;

            while ((line = reader.ReadLine()) != null)
            {
                if (line.Trim().Length > 0)
                    break;
            }

            if (line != null)
            {
                int eqpos = (line = line.TrimStart(null)).IndexOf('=');

                if (eqpos > 0)
                {
                    _key = line.Substring(0, eqpos);
                    line = line.Substring(eqpos + 1);
                }
                else _key = null;

                if (_useconvert)
                {
                    line = line.Replace("&ln;", "\n");
                    line = line.Replace("&lf;", "\r");
                    line = line.Replace("&bl;", "{");
                    line = line.Replace("&br;", "}");
                    line = line.Replace("&al;", "[");
                    line = line.Replace("&ar;", "]");
                    line = line.Replace("&ul;", "(");
                    line = line.Replace("&ur;", ")");
                    line = line.Replace("&eq;", "=");
                    line = line.Replace("&amp;", "&");
                }
            }
            else _key = null;

            return line;
        }

        public void WriteObject(object _object)
        {
            List<object> slist = new List<object>();
            WriteObject(_object, slist);
        }

        protected void WriteObject(object _object, List<object> _slist)
        {
            int refindex;

            if (_object == null)
            {
                WriteLine(null, typeof(object).FullName, true);
                WriteLine(null, "{", false);
            }
            else if ((refindex = _slist.IndexOf(_object)) >= 0)
            {
                Type objtype = _object.GetType();

                WriteLine(null, objtype.FullName, true);
                WriteLine(null, "{", false);
                spaces++;
                WriteLine("reference", refindex.ToString(), false);
                spaces--;
            }
            else
            {
                _slist.Add(_object);

                Type objtype = _object.GetType();

                WriteLine(null, objtype.FullName, true);
                WriteLine(null, "{", false);
                spaces++;

                if (objtype.IsPrimitive || objtype.Equals(typeof(string)))
                {
                    WriteLine("value", _object.ToString(), true);
                }
                else if (objtype.IsArray)
                {
                    WriteLine(null, "[", false);
                    spaces++;

                    WriteLine("count", ((System.Array)_object).Length.ToString(), false);
                    WriteLine(null, "", false);

                    foreach (object item in ((System.Array)_object))
                    {
                        WriteObject(item, _slist);
                    }

                    spaces--;
                    WriteLine(null, "]", false);
                }
                else
                {
                    WriteLine(null, "[", false);
                    spaces++;

                    foreach (FieldInfo finfo in objtype.GetFields(BindingFlags.GetField | BindingFlags.SetField | BindingFlags.NonPublic | BindingFlags.Public | BindingFlags.Instance))
                    {
                        if (!finfo.IsLiteral && !finfo.IsInitOnly && !finfo.IsNotSerialized && finfo.FieldType.BaseType != null && !finfo.FieldType.BaseType.Equals(typeof(System.MulticastDelegate)))
                        {
                            WriteLine(null, finfo.Name, true);
                            WriteLine(null, "(", false);
                            spaces++;

                            WriteObject(finfo.GetValue(_object), _slist);

                            spaces--;
                            WriteLine(null, ")", false);
                        }
                    }

                    spaces--;
                    WriteLine(null, "]", false);
                }

                spaces--;
            }

            WriteLine(null, "}", false);
        }

        protected object LoadObject(string _classname, List<object> _slist)
        {
            object resobject = null;
            string key = null;
            string line;

            if (_classname == null)
                _classname = ReadLine(out key, true);

            if (_classname == null)
                return null;

            if (key != null || ReadLine(out key, false) != "{" || key != null)
                throw new Exception("Bad header object signature");

            if ((line = ReadLine(out key, true)) != "}")
            {
                Type objtype = Type.GetType(_classname);

                if (key != null && key == "reference")
                {
                    int refindex = Convert.ToInt32(line);

                    if (refindex < 0 || refindex >= _slist.Count)
                        throw new Exception("Bad reference on previous object");

                    resobject = _slist[refindex];
                }
                else if (objtype.IsPrimitive)
                {
                    if (key != "value")
                        throw new Exception("Bad value for primitive object");

                    resobject = (object)objtype.InvokeMember("Parse",
                      BindingFlags.InvokeMethod | BindingFlags.Static | BindingFlags.Public,
                      null, null, new object[] { line });

                    _slist.Add(resobject);
                }
                else if (objtype.Equals(typeof(string)))
                {
                    if (key != "value")
                        throw new Exception("Bad value for primitive object");

                    resobject = line;
                    _slist.Add(resobject);
                }
                else if (line == "[" && key == null)
                {
                    if (objtype.IsArray)
                    {
                        if ((line = ReadLine(out key, false)) == null || key != "count")
                            throw new Exception("Bad count for array object");

                        int count = Convert.ToInt32(line), index = 0;

                        resobject = Activator.CreateInstance(objtype, count);
                        _slist.Add(resobject);

                        while ((line = ReadLine(out key, true)) != "]" || key != null)
                            ((System.Array)resobject).SetValue(LoadObject(line, _slist), index++);
                    }
                    else
                    {
                        resobject = Activator.CreateInstance(objtype);
                        _slist.Add(resobject);

                        while ((line = ReadLine(out key, true)) != "]" || key != null)
                        {
                            if (line == null || key != null)
                                throw new Exception("Bad header field signature");

                            string fieldname = line;

                            if ((line = ReadLine(out key, true)) != "(" || key != null)
                                throw new Exception("Bad header object signature");

                            object fieldobj = LoadObject(null, _slist);
                            FieldInfo fiendinf = objtype.GetField(fieldname, BindingFlags.GetField | BindingFlags.SetField | BindingFlags.NonPublic | BindingFlags.Public | BindingFlags.Instance);

                            if (fiendinf != null)
                                fiendinf.SetValue(resobject, fieldobj);

                            if ((line = ReadLine(out key, true)) != ")" || key != null)
                                throw new Exception("Bad tail object signature");
                        }
                    }
                }
                else throw new Exception("Bad header content signature");

                if ((line = ReadLine(out key, true)) != "}" || key != null)
                    throw new Exception("Bad tail object signature");
            }
            else if (key != null)
                throw new Exception("Bad tail object signature");

            return resobject;
        }

        public object ReadObject()
        {
            List<object> slist = new List<object>();
            return LoadObject(null, slist);
        }
    }
}