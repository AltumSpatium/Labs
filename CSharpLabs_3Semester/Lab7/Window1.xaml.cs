using System;
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
using System.Windows.Shapes;

namespace Lab7
{
    /// <summary>
    /// Interaction logic for Window1.xaml
    /// </summary>
    public partial class Window1 : Window
    {
        CompositionCollection compositions;

        public Window1(CompositionCollection _compositions)
        {
            InitializeComponent();
            compositions = _compositions;
            comboboxGenre.Items.Add("Rock");
            comboboxGenre.Items.Add("Rap");
            comboboxGenre.Items.Add("Pop");
            comboboxGenre.Items.Add("Country");
            comboboxGenre.Items.Add("Jazz");
            for (int i = 1; i <= 10; i++)
                comboboxRating.Items.Add(i);
            listbox1.ItemsSource = compositions;
        }

        private void textboxMinutes_PreviewTextInput(object sender, TextCompositionEventArgs e)
        {
            if (!Char.IsDigit(e.Text, 0))
                e.Handled = true;
        }

        private void textboxSeconds_PreviewTextInput(object sender, TextCompositionEventArgs e)
        {
            if (!Char.IsDigit(e.Text, 0))
                e.Handled = true;
        }

        private void buttonAdd_Click(object sender, RoutedEventArgs e)
        {
            if (textboxTitle != null && textboxPerformer != null && textboxMinutes != null && textboxSeconds != null && comboboxRating.SelectedItem != null && comboboxGenre.SelectedItem != null)
            {
                Composition.Genres genre = Composition.Genres.No_genre;
                int rating = Convert.ToInt32(comboboxRating.SelectedItem);

                switch ((string)comboboxGenre.SelectedItem)
                {
                    case "Rock":
                        genre = Composition.Genres.Rock;
                        break;
                    case "Rap":
                        genre = Composition.Genres.Rap;
                        break;
                    case "Pop":
                        genre = Composition.Genres.Pop;
                        break;
                    case "Country":
                        genre = Composition.Genres.Country;
                        break;
                    case "Jazz":
                        genre = Composition.Genres.Jazz;
                        break;
                }
                compositions.AddComposition(new Composition((compositions.Count > 0 ? compositions.Compositions[compositions.Count - 1].ID + 1 : 1), textboxTitle.Text, "0:" + textboxMinutes.Text + ":" + textboxSeconds.Text, textboxPerformer.Text, genre, rating));
                textboxTitle.Text = "";
                textboxPerformer.Text = "";
                textboxMinutes.Text = "";
                textboxSeconds.Text = "";
                comboboxGenre.SelectedItem = null;
                comboboxRating.SelectedItem = null;
            }
        }

        private void buttonRemove_Click(object sender, RoutedEventArgs e)
        {
            if (listbox1.SelectedItem != null)
                compositions.RemoveComposition((Composition)listbox1.SelectedItem);
        }

        private void buttonUpdate_Click(object sender, RoutedEventArgs e)
        {
            if (textboxTitle != null && textboxPerformer != null && textboxMinutes != null && textboxSeconds != null && comboboxRating.SelectedItem != null && comboboxGenre.SelectedItem != null && listbox1.SelectedItem != null)
            {
                Composition selectedComposition = (Composition)listbox1.SelectedItem;
                foreach (var composition in compositions)
                {
                    if (composition == selectedComposition)
                    {
                        string title = textboxTitle.Text;
                        string performer = textboxPerformer.Text;
                        int minutes = Convert.ToInt32(textboxMinutes.Text);
                        int seconds = Convert.ToInt32(textboxSeconds.Text);
                        int rating = Convert.ToInt32(comboboxRating.SelectedItem);
                        Composition.Genres genre = Composition.Genres.No_genre;

                        switch ((string)comboboxGenre.SelectedItem)
                        {
                            case "Rock":
                                genre = Composition.Genres.Rock;
                                break;
                            case "Rap":
                                genre = Composition.Genres.Rap;
                                break;
                            case "Pop":
                                genre = Composition.Genres.Pop;
                                break;
                            case "Country":
                                genre = Composition.Genres.Country;
                                break;
                            case "Jazz":
                                genre = Composition.Genres.Jazz;
                                break;
                        }
                        compositions.UpdateComposition(composition, title, performer, minutes, seconds, genre, rating);
                        textboxTitle.Text = "";
                        textboxPerformer.Text = "";
                        textboxMinutes.Text = "";
                        textboxSeconds.Text = "";
                        comboboxGenre.SelectedItem = null;
                        comboboxRating.SelectedItem = null;
                    }
                }
            }
        }

        private void listbox1_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            if (listbox1.SelectedItem != null)
            {
                Composition selectedComposition = (Composition)listbox1.SelectedItem;
                textboxTitle.Text = selectedComposition.Title.Replace("\"", "");
                textboxPerformer.Text = selectedComposition.Performer;
                textboxMinutes.Text = selectedComposition.Length.Minutes.ToString();
                textboxSeconds.Text = selectedComposition.Length.Seconds.ToString();
                comboboxGenre.SelectedItem = selectedComposition.Genre.ToString();
                comboboxRating.SelectedItem = selectedComposition.Rating;
            }
        }
    }
}
