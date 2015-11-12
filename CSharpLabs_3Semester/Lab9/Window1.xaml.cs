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

namespace Lab9
{
    /// <summary>
    /// Interaction logic for Window1.xaml
    /// </summary>

    public partial class Window1 : Window
    {
        Group tempGroup = new Group();
        Faculty fac;

        public Window1(Faculty _fac)
        {
            InitializeComponent();
            fac = _fac;
            foreach (var gr in fac)
            {
                foreach (var st in gr)
                {
                    tempGroup.AddStudent(st);
                }
            }
            foreach (var st in tempGroup)
            {
                listbox1.Items.Add(st.Name + " " + st.Surname);
            }
            combobox1.Visibility = Visibility.Hidden;
            buttonCancel.Visibility = Visibility.Hidden;
        }

        private void buttonSelect_Click(object sender, RoutedEventArgs e)
        {
            if ((string)buttonSelect.Content == "Operator Select")
            {
                combobox1.Visibility = Visibility.Visible;
                buttonCancel.Visibility = Visibility.Visible;
                combobox1.Items.Add("Name");
                combobox1.Items.Add("Surname");
                buttonSelect.Content = "Select";
            }
            else
            {
                if (combobox1.SelectedItem != null)
                {
                    IEnumerable<string> result = null;
                    listbox1.Items.Clear();
                    if ((string)combobox1.SelectedItem == "Name")
                    {
                        result = tempGroup.Select(student => student.Name);
                    }
                    else if ((string)combobox1.SelectedItem == "Surname")
                    {
                        result = tempGroup.Select(student => student.Surname);
                    }
                    else result = null;

                    if (result != null)
                    {
                        foreach (var str in result)
                        {
                            listbox1.Items.Add(str);
                        }
                    }

                    combobox1.Visibility = Visibility.Hidden;
                    buttonCancel.Visibility = Visibility.Hidden;
                    combobox1.Items.Clear();
                    buttonSelect.Content = "Operator Select";
                }
                else
                {
                    MessageBox.Show("You should choose a property to select!");
                }
            }
        } // Select students' name/surname only

        private void buttonOrderBy_Click(object sender, RoutedEventArgs e)
        {
            if ((string)buttonOrderBy.Content == "Operator OrderBy")
            {
                combobox1.Visibility = Visibility.Visible;
                buttonCancel.Visibility = Visibility.Visible;
                combobox1.Items.Add("Age");
                buttonOrderBy.Content = "Order";
            }
            else
            {
                if (combobox1.SelectedItem != null)
                {
                    IEnumerable<Student> result = null;
                    listbox1.Items.Clear();
                    if ((string)combobox1.SelectedItem == "Age")
                    {
                        result = tempGroup.OrderBy(student => student.Age);
                    }
                    else result = null;

                    if (result != null)
                    {
                        foreach (var st in result)
                        {
                            listbox1.Items.Add(st.Name + " " + st.Surname + " " + st.Age);
                        }
                    }

                    combobox1.Visibility = Visibility.Hidden;
                    buttonCancel.Visibility = Visibility.Hidden;
                    combobox1.Items.Clear();
                    buttonOrderBy.Content = "Operator OrderBy";
                }
                else
                {
                    MessageBox.Show("You should choose a property to order by!");
                }
            }
        } // Order students by age

        private void buttonGroupBy_Click(object sender, RoutedEventArgs e)
        {
            if ((string)buttonGroupBy.Content == "Operator GroupBy")
            {
                combobox1.Visibility = Visibility.Visible;
                buttonCancel.Visibility = Visibility.Visible;
                combobox1.Items.Add("Name");
                combobox1.Items.Add("Surname");
                buttonGroupBy.Content = "Group";
            }
            else
            {
                if (combobox1.SelectedItem != null)
                {
                    IEnumerable<IGrouping<string, Student>> result = null;
                    listbox1.Items.Clear();
                    if ((string)combobox1.SelectedItem == "Name")
                    {
                        result = tempGroup.GroupBy(student => student.Name);
                    }
                    else if ((string)combobox1.SelectedItem == "Surname")
                    {
                        result = tempGroup.GroupBy(student => student.Surname);
                    }
                    else result = null;

                    if (result != null)
                    {
                        foreach (IGrouping<string, Student> group in result)
                        {
                            foreach (var st in group)
                                listbox1.Items.Add(st.Name + " " + st.Surname + " " + st.Age);
                        }
                    }

                    combobox1.Visibility = Visibility.Hidden;
                    buttonCancel.Visibility = Visibility.Hidden;
                    combobox1.Items.Clear();
                    buttonGroupBy.Content = "Operator GroupBy";
                }
                else
                {
                    MessageBox.Show("You should choose a property to group by!");
                }
            }
        } // Group students by name/surname

        public void buttonCancel_Click(object sender, RoutedEventArgs e) // Cancel
        {
            buttonSelect.Content = "Operator Select";
            buttonOrderBy.Content = "Operator OrderBy";
            buttonGroupBy.Content = "Operator GroupBy";
            buttonAverage.Content = "Operator Average";
            combobox1.Items.Clear();
            combobox1.Visibility = Visibility.Hidden;
            buttonCancel.Visibility = Visibility.Hidden;
        }

        private void buttonAverage_Click(object sender, RoutedEventArgs e) // Calculate the average age
        {
            if ((string)buttonAverage.Content == "Operator Average")
            {
                combobox1.Visibility = Visibility.Visible;
                buttonCancel.Visibility = Visibility.Visible;
                combobox1.Items.Add("Age");
                buttonAverage.Content = "Calculate";
            }
            else
            {
                if (combobox1.SelectedItem != null)
                {
                    double average = 0;
                    listbox1.Items.Clear();
                    if ((string)combobox1.SelectedItem == "Age")
                    {
                        average = tempGroup.Average(student => student.Age);
                    }

                    listbox1.Items.Add(average);

                    combobox1.Visibility = Visibility.Hidden;
                    buttonCancel.Visibility = Visibility.Hidden;
                    combobox1.Items.Clear();
                    buttonAverage.Content = "Operator Average";
                }
                else
                {
                    MessageBox.Show("You should choose a property to aggregate!");
                }
            }
        }
    }
}
