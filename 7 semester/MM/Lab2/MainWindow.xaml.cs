using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows;
using System.Windows.Input;
using System.Windows.Controls.DataVisualization.Charting;
using MathNet.Numerics.Distributions;

namespace MM_Lab2
{
	public partial class MainWindow : Window
	{
		private Sensor currentSensor;
		private int numbersCount;
		private double iv;

		public MainWindow()
		{
			InitializeComponent();
			setParams(new SquareMeanSensor(), 10000, 12341239);
		}

		private double[] Generate(Sensor sensor, int count, double iv) => sensor.Generate(count, iv);

		private void setParams(Sensor newSensor, int newNumbersCount, double newIV)
		{
			currentSensor = newSensor;
			numbersCount = newNumbersCount;
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

			if (tbNumsCount.Text == "" || tbIV.Text == "")
				return;

			int numbersCount = Convert.ToInt32(tbNumsCount.Text);
			double iv = Convert.ToDouble(tbIV.Text);

			setParams(currentSensor, numbersCount, iv);
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

		private double[] ReversedFunction(double[] seqX)
		{
			int n = seqX.Length;
			double[] seqY = new double[n];

			for (int i = 0; i < n; i++)
			{
				double x = seqX[i];
				double t = Math.Sqrt(-2 * Math.Log(1 - x));
				seqY[i] = t - (2.30753 + 0.27061 * t) / (1 + 0.99229 * t + 0.04481 * t * t);
			}

			return seqY;
		}

		private Dictionary<string, double> buildFrequencies(double[] sequence)
		{
			int sectionsCount;
			if (numbersCount <= 100) sectionsCount = (int)Math.Sqrt(numbersCount);
			else sectionsCount = 3 * (int)Math.Log(numbersCount);

			Dictionary<string, double> frequencies = new Dictionary<string, double>();
			double stepSize = 4.0 / sectionsCount;
			double currentStep = -2 + stepSize;
			int i = 0;

			while (currentStep <= 2)
			{
				double count = sequence.Select(x =>
					(currentStep - stepSize) <= x && x < currentStep).Count(x => x == true);
				frequencies.Add((new String(' ', i)).ToString(), count / numbersCount);
				currentStep += stepSize;
				i++;
			}

			return frequencies;			
		}

		private double calcEstimateME(double[] sequence) => sequence.Sum() / sequence.Length;

		private double calcEstimateD(double[] sequence, double estimateME) =>
			sequence.Select(x => x * x - estimateME * estimateME).Sum() / (sequence.Length - 1);

		private Tuple<double, double> calcConfIntervalME(double estimateME, double estimateD, int n)
		{
			StudentT student = new StudentT();
			double t = StudentT.InvCDF(student.Location, student.Scale, n - 1, 0.99);
			double intEstimateME_l = estimateME - Math.Sqrt(estimateD) * t / Math.Sqrt(numbersCount - 1);
			double intEstimateME_r = estimateME + Math.Sqrt(estimateD) * t / Math.Sqrt(numbersCount - 1);
			return new Tuple<double, double>(intEstimateME_l, intEstimateME_r);
		}

		private Tuple<double, double> calcConfIntervalD(double estimateD)
		{
			double intEstimateD_l = (numbersCount - 1) * estimateD / ChiSquared.InvCDF(numbersCount - 1, 1.01 / 2);
			double intEstimateD_r = (numbersCount - 1) * estimateD / ChiSquared.InvCDF(numbersCount - 1, 0.99 / 2);
			return new Tuple<double, double>(intEstimateD_l, intEstimateD_r);
		}

		private double calcChiSquare(Dictionary<string, double> frequencies, double[] seqY)
		{
			double chiSq = 0;
			double stepSize = (seqY.Max() - seqY.Min()) / frequencies.Count;
			double currentStep = seqY.Min();

			foreach (var freq in frequencies)
			{
				double p_i = Normal.CDF(0, 1, currentStep + stepSize) - Normal.CDF(0, 1, currentStep);
				double count = seqY.Select(x =>
					currentStep <= x && x < (currentStep + stepSize)).Count(x => x == true) / numbersCount;
				chiSq += Math.Pow(p_i - count, 2) / p_i;
				currentStep += stepSize;
			}

			return frequencies.Count * chiSq;
		}

		private void drawBarChart(List<KeyValuePair<string, double>> freqsList)
		{
			chart.Axes.Clear();
			LinearAxis xAxis = new LinearAxis();
			xAxis.Orientation = AxisOrientation.X;
			xAxis.Location = AxisLocation.Bottom;
			xAxis.Minimum = -2;
			xAxis.Maximum = 2;
			xAxis.Interval = 4.0 / freqsList.Count;

			if (chart.Axes.Count > 1) chart.Axes.RemoveAt(1);
			chart.Axes.Add(xAxis);
			((ColumnSeries)chart.Series[0]).ItemsSource = freqsList;
		}

		private void btnStart_Click(object sender, RoutedEventArgs e)
		{
			double[] seqX = Generate(currentSensor, numbersCount, iv);
			double[] seqY = ReversedFunction(seqX);

			var frequencies = buildFrequencies(seqY);
			List<KeyValuePair<string, double>> freqsList = new List<KeyValuePair<string, double>>();
			foreach (var freq in frequencies)
				freqsList.Add(new KeyValuePair<string, double>(freq.Key, freq.Value));
			drawBarChart(freqsList);

			double estimateME = calcEstimateME(seqY);
			double estimateD = calcEstimateD(seqY, estimateME);
			labelEstimateME.Content = "М = " + Math.Round(estimateME, 5);
			labelEstimateD.Content = "D = " + Math.Round(estimateD, 5);

			Tuple<double, double> confIntervalME = calcConfIntervalME(estimateME, estimateD, freqsList.Count);
			labelIntEstimateME.Content = "I(M): " + Math.Round(confIntervalME.Item1, 5)
				+ " <= M < " + Math.Round(confIntervalME.Item2, 5);

			Tuple<double, double> confIntervalD = calcConfIntervalD(estimateD);
			labelIntEstimateD.Content = "I(D): " + Math.Round(confIntervalD.Item1, 5)
				+ " <= D < " + Math.Round(confIntervalD.Item2, 5);

			double chiSq = calcChiSquare(frequencies, seqY);
			double chiSqTable = ChiSquared.InvCDF(frequencies.Count - 3, 0.99);
			string sign = chiSq == chiSqTable ? " = " : (chiSq < chiSqTable ? " < " : " > ");
			labelChiSquare.Content = "χ2 = " + Math.Round(chiSq, 5) + sign
				+ Math.Round(chiSqTable, 5);
		}
	}
}
