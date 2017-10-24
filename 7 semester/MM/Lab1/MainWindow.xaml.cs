using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows;
using System.Windows.Input;
using System.Windows.Controls.DataVisualization.Charting;

namespace MM_Lab1
{
	public partial class MainWindow : Window
	{
		private Sensor currentSensor;
		private int numbersCount;
		private int sectionsCount;
		private double iv;

		public MainWindow()
		{
			InitializeComponent();
			setParams(new SquareMeanSensor(), 100, 10, 12341234);
		}

		private double[] Generate(Sensor sensor, int count, double iv) => sensor.Generate(count, iv);

		private void setParams(Sensor newSensor, int newNumbersCount, int newSectionsCount, double newIV)
		{
			currentSensor = newSensor;
			numbersCount = newNumbersCount;
			sectionsCount = newSectionsCount;
			iv = newIV;
		}

		private void btnApplySettings_Click(object sender, RoutedEventArgs e)
		{
			Sensor currentSensor = null;

			if ((bool)rbSMM.IsChecked)
			{
				currentSensor = new SquareMeanSensor();
			}
			else if ((bool)rbMCM.IsChecked)
			{
				if (tbM.Text == "" || tbK.Text == "") return;

				double m = Convert.ToDouble(tbM.Text);
				double k = Convert.ToDouble(tbK.Text);
				currentSensor = new MultiplicativeCongruentSensor(m, k);
			}

			if (tbNumsCount.Text == "" || tbSectionsCount.Text == "" || tbIV.Text == "")
				return;

			int numbersCount = Convert.ToInt32(tbNumsCount.Text);
			int sectionsCount = Convert.ToInt32(tbSectionsCount.Text);
			double iv = Convert.ToDouble(tbIV.Text);

			setParams(currentSensor, numbersCount, sectionsCount, iv);
		}

		private void Preview_TextInput(object sender, TextCompositionEventArgs e)
		{
			if (!char.IsDigit(e.Text, e.Text.Length - 1))
				e.Handled = true;
		}

		private void rbMCM_Click(object sender, RoutedEventArgs e)
		{
			labelM.Visibility = Visibility.Visible;
			labelK.Visibility = Visibility.Visible;
			tbM.Visibility = Visibility.Visible;
			tbK.Visibility = Visibility.Visible;
		}

		private void rbSMM_Click(object sender, RoutedEventArgs e)
		{
			labelM.Visibility = Visibility.Hidden;
			labelK.Visibility = Visibility.Hidden;
			tbM.Visibility = Visibility.Hidden;
			tbK.Visibility = Visibility.Hidden;
		}

		private Dictionary<string, double> buildFrequencies(double[] sequence)
		{
			Dictionary<string, double> frequencies = new Dictionary<string, double>();
			double stepSize = 1.0 / sectionsCount;
			double currentStep = stepSize;
			int i = 0;

			while (currentStep <= 1)
			{
				double count = sequence.Select(x => 
					(currentStep - stepSize) <= x && x < currentStep).Count(x => x == true);
				frequencies.Add((++i).ToString(), count / numbersCount);
				currentStep += stepSize;
			}

			return frequencies;
		}

		private double calcMathExpectation(double[] sequence) => sequence.Sum() / sequence.Length;

		private double calcDispersion(double[] sequence, double mathExpectation) =>
			sequence.Select(x => x * x).Sum() / sequence.Length - mathExpectation * mathExpectation;

		private double calcCorrelationCoeff(double[] sequence, int s)
		{
			int n = sequence.Length;
			double mm = 0;
			for (int i = 0; i < n - s; i++)
				mm += sequence[i] * sequence[i + s];
			double Mxy = mm / (n - s);

			double Mx = sequence.Take(n - s).Sum() / (n - s);
			double My = sequence.Skip(s).Take(n - s).Sum() / (n - s);

			double Kxy = Mxy - Mx * My;

			double Dx = sequence.Select(x => (x - Mx) * (x - Mx)).Take(n - s).Sum() / (n - s - 1);
			double Dy = sequence.Select(x => (x - My) * (x - My)).Skip(s).Take(n - s).Sum() / (n - s - 1);

			double Rxy = Kxy / Math.Sqrt(Dx * Dy);

			return Rxy;
		}

		private void btnStartUniformityTest_Click(object sender, RoutedEventArgs e)
		{
			double[] sequence = Generate(currentSensor, numbersCount, iv);
			var frequencies = buildFrequencies(sequence);

			List<KeyValuePair<string, double>> freqsList = new List<KeyValuePair<string, double>>();
			foreach (var freq in frequencies)
				freqsList.Add(new KeyValuePair<string, double>(freq.Key, freq.Value));
			((ColumnSeries) chart.Series[0]).ItemsSource = freqsList;

			var mathExpectation = calcMathExpectation(sequence);
			labelMathExpectation.Content = "M: " + Math.Round(mathExpectation, 5);

			var dispersion = calcDispersion(sequence, mathExpectation);
			labelDispersion.Content = "D: " + Math.Round(dispersion, 5);

			var correlationCoeff = calcCorrelationCoeff(sequence, 3);
			labelCorrelation.Content = "R: " + Math.Round(correlationCoeff, 5);
		}
	}
}
