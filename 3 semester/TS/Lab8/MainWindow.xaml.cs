using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Linq;
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
using System.Globalization;

namespace Lab8
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        Faculty fac = new Faculty();
        Window1 w1;
        Group first;
        string filename;

        public MainWindow()
        {
            InitializeComponent();
            listbox1.ItemsSource = fac;
            textblock1.Visibility = Visibility.Hidden;
            textblock2.Visibility = Visibility.Hidden;
            textblock3.Visibility = Visibility.Hidden;
            textblock4.Visibility = Visibility.Hidden;
            textbox1.Visibility = Visibility.Hidden;
            textbox2.Visibility = Visibility.Hidden;
            textbox3.Visibility = Visibility.Hidden;
            textbox4.Visibility = Visibility.Hidden;
            button1.Visibility = Visibility.Hidden;
            button2.Visibility = Visibility.Hidden;
            button3.Visibility = Visibility.Hidden;

            App.LanguageChanged += LanguageChanged;
            CultureInfo currLang = App.Language;

            menuItemLanguage.Items.Clear();
            foreach (var lang in App.Languages)
            {
                MenuItem menuLang = new MenuItem();
                menuLang.Header = lang.DisplayName;
                menuLang.Tag = lang;
                menuLang.IsChecked = lang.Equals(currLang);
                menuLang.Click += ChangeLanguageClick;
                menuItemLanguage.Items.Add(menuLang);
            }
        }

        private void LanguageChanged(Object sender, EventArgs e)
        {
            CultureInfo currLang = App.Language;

            foreach (MenuItem i in menuItemLanguage.Items)
            {
                CultureInfo ci = (CultureInfo)i.Tag;
                i.IsChecked = ci != null && ci.Equals(currLang);
            }
            if (currLang.Name == "en-US")
            {
                imgeng.Visibility = Visibility.Visible;
                imgrus.Visibility = Visibility.Hidden;
            }
            else if (currLang.Name == "ru-RU")
            {
                imgeng.Visibility = Visibility.Hidden;
                imgrus.Visibility = Visibility.Visible;
            }
        }

        private void ChangeLanguageClick(Object sender, EventArgs e)
        {
            MenuItem mi = (MenuItem)sender;
            if (mi != null)
            {
                CultureInfo lang = (CultureInfo)mi.Tag;
                if (lang != null)
                {
                    App.Language = lang;
                }
            }

        }

        private void button1_Click(object sender, RoutedEventArgs e)
        {
            if (textbox3.Text != "")
            {
                fac.AddGroup(new Group(textbox3.Text));
                textblock3.Visibility = Visibility.Hidden;
                textbox3.Visibility = Visibility.Hidden;
                button1.Visibility = Visibility.Hidden;
                textbox3.Text = "";
                this.MaxWidth = 363;
                this.MinWidth = 363;
                this.Width = 363;
            }
            else
            {
                MessageBox.Show((string)labelMB.Content);
            }
        } // Add a group to faculty

        private void button2_Click(object sender, RoutedEventArgs e)
        {
            if (textbox1.Text != "" && textbox2.Text != "" && listbox1.SelectedItem != null)
            {
                Group selectedGroup = (Group)listbox1.SelectedItem;
                if ((bool)menuItemRandomAge.IsChecked)
                {
                    selectedGroup.AddStudent(new Student(textbox1.Text, textbox2.Text));
                }
                else
                {
                    selectedGroup.AddStudent(new Student(textbox1.Text, textbox2.Text, Convert.ToInt32(textbox4.Text)));
                }
                listbox1.UnselectAll();
                textblock1.Visibility = Visibility.Hidden;
                textblock2.Visibility = Visibility.Hidden;
                if (!(bool)menuItemRandomAge.IsChecked)
                {
                    textblock4.Visibility = Visibility.Hidden;
                    textbox4.Visibility = Visibility.Hidden;
                }
                textbox1.Visibility = Visibility.Hidden;
                textbox2.Visibility = Visibility.Hidden;
                button2.Visibility = Visibility.Hidden;
                textbox1.Text = "";
                textbox2.Text = "";
                textbox4.Text = "";
                this.MaxWidth = 363;
                this.MinWidth = 363;
                this.Width = 363;
            }
            else
            {
                MessageBox.Show((string)labelMB.Content);
            }
        } // Add a student to group

        private void button3_Click(object sender, RoutedEventArgs e)
        {
            if (textbox1.Text != "" && textbox2.Text != "" && textbox4.Text != "")
            {
                Student st = fac.SearchStudent(textbox1.Text, textbox2.Text, Convert.ToInt32(textbox4.Text));
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
                textbox4.Text = "";
                textbox1.Visibility = Visibility.Hidden;
                textbox2.Visibility = Visibility.Hidden;
                textbox4.Visibility = Visibility.Hidden;
                textblock1.Visibility = Visibility.Hidden;
                textblock2.Visibility = Visibility.Hidden;
                textblock4.Visibility = Visibility.Hidden;
                button3.Visibility = Visibility.Hidden;
                this.MaxWidth = 363;
                this.MinWidth = 363;
                this.Width = 363;
            }
            else
            {
                MessageBox.Show((string)labelMB.Content);
            }
        } // Search for a student

        private void listbox1_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (listbox1.SelectedItem != null)
            {
                Group selectedGroup = (Group)listbox1.SelectedItem;
                listbox2.ItemsSource = selectedGroup;
            }
        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            if (w1 != null)
                w1.Close();
        }

        private void menuItemNew_Click(object sender, RoutedEventArgs e)
        {
            foreach (Group gr in fac)
            {
                gr.ClearGroup();
            }
            fac.ClearFaculty();
            filename = null;
        } // Create a new collection

        private void menuItemOpen_Click(object sender, RoutedEventArgs e)
        {
            OpenFileDialog fileDialog = new OpenFileDialog();

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
        }  // Open collection from file

        private void menuItemSave_Click(object sender, RoutedEventArgs e)
        {
            if (filename == null)
            {
                SaveFileDialog fileDialog = new SaveFileDialog();

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
            else
            {
                Serializer sl = new Serializer(filename, true);
                sl.WriteObject(fac);
                sl.Close();
            }
        } // Save collection to file

        private void menuItemSaveAs_Click(object sender, RoutedEventArgs e)
        {
            SaveFileDialog fileDialog = new SaveFileDialog();

            fileDialog.DefaultExt = ".txt";
            fileDialog.Filter = "Text files (txt)|*.txt";
            if (fileDialog.ShowDialog() == true)
                filename = fileDialog.FileName;
            else
                return;
            Serializer sl = new Serializer(filename, true);
            sl.WriteObject(fac);
            sl.Close();
        } // Save collection to a specific file

        private void menuItemExit_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        } // Exit

        private void menuItemAddGroup_Click(object sender, RoutedEventArgs e)
        {
            textblock3.Visibility = Visibility.Visible;
            textbox3.Visibility = Visibility.Visible;
            button1.Visibility = Visibility.Visible;            
            this.MaxWidth = 525;
            this.MinWidth = 525;
            this.Width = 525;
            textbox3.Text = "";
        } // Add group to a faculty

        private void menuItemRemoveGroup_Click(object sender, RoutedEventArgs e)
        {
            if (listbox1.SelectedItem != null)
            {
                Group selectedGroup = (Group)listbox1.SelectedItem;
                listbox2.ItemsSource = null;
                fac.RemoveGroup(selectedGroup);
            }
        } // Remove group from faculty

        private void menuItemAddStudent_Click(object sender, RoutedEventArgs e)
        {
            textbox1.Text = "";
            textbox2.Text = "";
            textbox4.Text = "";
            button2.Visibility = Visibility.Visible;
            textblock1.Visibility = Visibility.Visible;
            textblock2.Visibility = Visibility.Visible;
            textbox1.Visibility = Visibility.Visible;
            textbox2.Visibility = Visibility.Visible;
            if (!(bool)menuItemRandomAge.IsChecked)
            {
                textblock4.Visibility = Visibility.Visible;
                textbox4.Visibility = Visibility.Visible;
            }
            this.MaxWidth = 525;
            this.MinWidth = 525;
            this.Width = 525;
        } // Add student to a group

        private void menuItemExpellStudent_Click(object sender, RoutedEventArgs e)
        {
            if (listbox1.SelectedItem != null && listbox2.SelectedItem != null)
            {
                Group selectedGroup = (Group)listbox1.SelectedItem;
                Student selectedStudent = (Student)listbox2.SelectedItem;
                selectedGroup.ExpellStudent(selectedStudent);
                listbox2.UnselectAll();
            }
        } // Expell student

        private void menuItemSearch_Click(object sender, RoutedEventArgs e)
        {
            textbox1.Text = "";
            textbox2.Text = "";
            textbox4.Text = "";
            button3.Visibility = Visibility.Visible;
            textblock1.Visibility = Visibility.Visible;
            textblock2.Visibility = Visibility.Visible;
            textbox1.Visibility = Visibility.Visible;
            textbox2.Visibility = Visibility.Visible;
            textblock4.Visibility = Visibility.Visible;
            textbox4.Visibility = Visibility.Visible;
            this.MaxWidth = 525;
            this.MinWidth = 525;
            this.Width = 525;
        } // Search for student

        private void menuItemChooseMerge_Click(object sender, RoutedEventArgs e)
        {
            if (listbox1.SelectedItem != null)
            {
                first = (Group)listbox1.SelectedItem;
                listbox1.UnselectAll();
            }
            menuItemChooseMerge.IsEnabled = false;
            menuItemMerge.IsEnabled = true;
        } // Choose the first group to merge

        private void menuItemMerge_Click(object sender, RoutedEventArgs e)
        {
            if (listbox1.SelectedItem != null)
            {
                Group second = (Group)listbox1.SelectedItem;
                if (first.Number != second.Number)
                {
                    fac.MergeGroups(first, second);
                }
                listbox1.UnselectAll();
                menuItemChooseMerge.IsEnabled = true;
                menuItemMerge.IsEnabled = false;
            }
            else
            {
                MessageBox.Show("Group must be choosed!");
            }
        } // Merge two groups

        private void menuItemChooseTransfer_Click(object sender, RoutedEventArgs e)
        {
            if (listbox1.SelectedItem != null)
            {
                first = (Group)listbox1.SelectedItem;
                listbox1.UnselectAll();
                menuItemChooseTransfer.IsEnabled = false;
                menuItemTransfer.IsEnabled = true;
            }
        } // Choose the group to transfer to

        private void menuItemTransfer_Click(object sender, RoutedEventArgs e)
        {
            if (listbox1.SelectedItem != null && listbox2.SelectedItem != null)
            {
                Student selectedStudent = (Student)listbox2.SelectedItem;
                fac.TransferStudent(selectedStudent, first.Number);
                listbox1.UnselectAll();
                listbox2.UnselectAll();
                menuItemChooseTransfer.IsEnabled = true;
                menuItemTransfer.IsEnabled = false;
            }
            else
            {
                MessageBox.Show((string)labelMB.Content);
            }
        } // Transfer student to choosen group

        private void menuItemLinq_Click(object sender, RoutedEventArgs e)
        {
            w1 = new Window1(fac);
            w1.Show();
        } // Open LINQ test window

        private void menuItemCancel_Click(object sender, RoutedEventArgs e)
        {
            textblock1.Visibility = Visibility.Hidden;
            textblock2.Visibility = Visibility.Hidden;
            textblock3.Visibility = Visibility.Hidden;
            textblock4.Visibility = Visibility.Hidden;
            textbox1.Visibility = Visibility.Hidden;
            textbox2.Visibility = Visibility.Hidden;
            textbox3.Visibility = Visibility.Hidden;
            textbox4.Visibility = Visibility.Hidden;
            button1.Visibility = Visibility.Hidden;
            button2.Visibility = Visibility.Hidden;
            button3.Visibility = Visibility.Hidden;
            this.Width = 363;
            this.MinWidth = 363;
            this.MaxWidth = 363;
            first = null;
            menuItemChooseMerge.IsEnabled = true;
            menuItemMerge.IsEnabled = false;
            menuItemChooseTransfer.IsEnabled = true;
            menuItemTransfer.IsEnabled = false;
            listbox1.UnselectAll();
            listbox2.UnselectAll();
        } //Cancel
    }
}