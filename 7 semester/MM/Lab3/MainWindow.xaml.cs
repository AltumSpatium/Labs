using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows;
using System.Windows.Input;
using System.Windows.Controls.DataVisualization.Charting;
using MathNet.Numerics.Distributions;
using System.Windows.Controls;
using System.Data;

namespace MM_Lab3
{
	public partial class MainWindow : Window
	{
		private class ChartParams
		{
			public string Title { get; set; }
			public List<List<KeyValuePair<string, double>>> FreqsLists { get; set; }
			public double[] Range { get; set; }

			public ChartParams(string title, List<List<KeyValuePair<string, double>>> freqsLists, double[] range)
			{
				Title = title;
				FreqsLists = freqsLists;
				Range = range;
			}
		}

		private List<ChartParams> charts = new List<ChartParams>();
		private int chartIndex = 0;

		private Sensor currentSensor;
		private int numbersCount;
		private double iv;
		private List<double> X = new List<double>();
		private List<double> Y = new List<double>();

		private int pmN;
		private int pmM;
		private double[,] A;
		private double[,] B;
		private double[,] PM;

		public MainWindow()
		{
			InitializeComponent();

			double[,] defaultA = new double[,] { { 2, 6, 7 } };
			double[,] defaultB = new double[,] { { 2, 3 } };
			double[,] defaultPM = new double[,] { { 0.25, 0.15, 0.15 }, { 0.15, 0.15, 0.15 } };

			SetParams(new MultiplicativeCongruentSensor(4294967296, 12344321), 1000, 12341239, 3, 2,
				defaultA, defaultB, defaultPM);
			UpdateMatrix(3, 1, defaultA, 30, dgA);
			UpdateMatrix(2, 1, defaultB, 30, dgB);
			UpdateMatrix(3, 2, defaultPM, 40, dgPM);
		}

		private double[] Generate(Sensor sensor, int count, double iv) => sensor.Generate(count, iv);

		private void UpdateMatrix(int n, int m, double[,] matrix, int cellWidth, DataGrid dataGrid)
		{
			dataGrid.ColumnWidth = cellWidth;
			dataGrid.Width = cellWidth * n;
			dataGrid.Height = 30 * m;

			DataTable dataTable = new DataTable();
			for (int i = 0; i < n; i++) dataTable.Columns.Add(new DataColumn("", typeof(double)));
			for (int j = 0; j < m; j++)
			{
				DataRow row = dataTable.NewRow();
				for (int i = 0; i < n; i++) row[i] = matrix[j,i];
				dataTable.Rows.Add(row);
			}

			dataGrid.ItemsSource = dataTable.DefaultView;
		}

		private double[,] GetMatrixData(DataGrid dataGrid)
		{
			int n = dataGrid.Columns.Count;
			int m = dataGrid.Items.Count;
			double[,] matrix = new double[m, n];

			for (int j = 0; j < m; j++)
				for (int i = 0; i < n; i++)
					matrix[j, i] = (double)((DataRowView) dataGrid.Items[j]).Row.ItemArray[i];

			return matrix;
		}

		private double[,] CopyMatrix(int fromN, int fromM, int toN, int toM, double[,] from, double[,] to)
		{
			for (int j = 0; j < toM; j++)
				for (int i = 0; i < toN; i++)
					to[j, i] = 0;

			int n = fromN > toN ? toN : fromN;
			int m = fromM > toM ? toM : fromM;

			for (int j = 0; j < m; j++)
				for (int i = 0; i < n; i++)
					to[j, i] = from[j, i];
			return to;
		}

		private void SetParams(Sensor newSensor, int newNumbersCount, double newIV, int newPmN, int newPmM,
			double[,] newA, double[,] newB, double[,] newPM)
		{
			currentSensor = newSensor;
			numbersCount = newNumbersCount;
			iv = newIV;
			pmN = newPmN;
			pmM = newPmM;
			A = newA;
			B = newB;
			PM = newPM;
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

			if (tbNumsCount.Text == "" || tbIV.Text == "" || tbPM_N.Text == "" || tbPM_M.Text == "")
				return;

			int numbersCount = Convert.ToInt32(tbNumsCount.Text);
			double iv = Convert.ToDouble(tbIV.Text);
			int pmN = Convert.ToInt32(tbPM_N.Text);
			int pmM = Convert.ToInt32(tbPM_M.Text);

			double[,] A = GetMatrixData(dgA);
			double[,] B = GetMatrixData(dgB);
			double[,] PM = GetMatrixData(dgPM);

			if (!CheckPM(PM))
			{
				labelCheckPM.Content = "Сумма вероятностей не равна 1!";
				return;
			}
			labelCheckPM.Content = "";

			SetParams(currentSensor, numbersCount, iv, pmN, pmM, A, B, PM);
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

		private ChartParams CreateChart(string titleEnd, double[] theorP, double[] empiricalP, double[] range)
		{
			string title = "Гистограмма " + titleEnd;
			List<List<KeyValuePair<string, double>>> freqsLists = new List<List<KeyValuePair<string, double>>>();
			
			List<KeyValuePair<string, double>> theorFreqList = new List<KeyValuePair<string, double>>();
			foreach (var freq in BuildFrequencies(theorP, range))
				theorFreqList.Add(new KeyValuePair<string, double>(freq.Key, freq.Value));

			List<KeyValuePair<string, double>> empiricalFreqList = new List<KeyValuePair<string, double>>();
			foreach (var freq in BuildFrequencies(empiricalP, range))
				empiricalFreqList.Add(new KeyValuePair<string, double>(freq.Key, freq.Value));

			freqsLists.Add(theorFreqList);
			freqsLists.Add(empiricalFreqList);

			return new ChartParams(title, freqsLists, range);
		}

		private ChartParams CreateChartX(string titleEnd, double[] theorPx, double[] empiricalPx)
		{
			double[] a = new double[pmN];
			for (int i = 0; i < pmN; i++) a[i] = A[0, i];
			return CreateChart(titleEnd, theorPx, empiricalPx, a);
		}

		private ChartParams CreateChartY(string titleEnd, double[] theorPyx, double[] empiricalPyx)
		{
			double[] b = new double[pmM];
			for (int i = 0; i < pmM; i++) b[i] = B[0, i];
			return CreateChart(titleEnd, theorPyx, empiricalPyx, b);
		}

		private Dictionary<string, double> BuildFrequencies(double[] P, double[] range)
		{
			double left = range.Min() - 1;
			double right = range.Max() + 1;

			Dictionary<string, double> frequencies = new Dictionary<string, double>();
			for (double i = left; i < right; i++)
			{
				double freq = range.Contains(i) ? P[range.ToList().IndexOf(i)] : 0;
				frequencies.Add(new String(' ', (int)i), freq);
			}

			return frequencies;			
		}

		private double CalcME(double[,] matrix, double[,] v, int axis)
		{
			double ME = 0;
			int outer = axis == 0 ? pmN : pmM;
			int inner = axis == 0 ? pmM : pmN;

			for (int i = 0; i < outer; i++)
				for (int j = 0; j < inner; j++)
					ME += matrix[axis == 0 ? j : i, axis == 0 ? i : j] * v[0, i];

			return ME;
		}

		private double CalcD(double[,] matrix, double[,] v, double M, int axis)
		{
			double D = 0;
			int outer = axis == 0 ? pmN : pmM;
			int inner = axis == 0 ? pmM : pmN;

			for (int i = 0; i < outer; i++)
				for (int j = 0; j < inner; j++)
					D += matrix[axis == 0 ? j : i, axis == 0 ? i : j] * Math.Pow(v[0, i] - M, 2);

			return D;
		}

		private double CalcCorrelationCoeff(double[,] matrix, double Mx, double My, double Dx, double Dy)
		{
			double Mxy = 0;
			for (int i = 0; i < pmN; i++)
				for (int j = 0; j < pmM; j++)
					Mxy += A[0, i] * B[0, j] * matrix[j, i];

			double Rxy = (Mxy - Mx * My) / Math.Pow(Dx * Dy, 0.5);
			return Rxy;
		}

		private double[] CalcDistributionSeriesX(int n, int m, double[,] matrix)
		{
			double[] Px = new double[n];

			for (int i = 0; i < n; i++)
			{
				double colSum = 0;
				for (int j = 0; j < m; j++)
					colSum += matrix[j, i];
				Px[i] = colSum;
			}

			return Px;
		}

		private double[] CalcDistributionFunctionX(int n, double[] Px)
		{
			double[] Fx = new double[n + 1];

			for (int i = 1; i <= n; i++)
				Fx[i] = Fx[i - 1] + Px[i - 1];

			return Fx;
		}

		private double[,] CalcDistributionSeriesY(int n, int m, double[,] matrix, double[] Px)
		{
			double[,] Py = CopyMatrix(n, m, n, m, matrix, new double[m, n]);

			for (int i = 0; i < n; i++)
				for (int j = 0; j < m; j++)
					Py[j, i] = Math.Round(Py[j, i] / Px[i], 5);

			return Py;
		}

		private double[,] CalcDistributionFunctionY(int n, int m, double[,] Py)
		{
			double[,] Fy = new double[m + 1, n];

			for (int i = 0; i < n; i++)
				for (int j = 1; j <= m; j++)
					Fy[j, i] = Fy[j - 1, i] + Py[j - 1, i];

			return Fy;
		}

		private double[,] CalcProbabilityMatrix(int n, int m, double[] Fx, double[,] Fy)
		{
			double[,] empiricalPM = new double[m, n];
			double[] sequence = Generate(currentSensor, numbersCount * 2, iv);
			int kx = 0, ky = 0;
			
			for (int i = 0; i < sequence.Length; i += 2)
			{
				for (int j = 0; j < n + 1; j++)
					if (sequence[i] <= Fx[j])
					{
						kx = j - 1;
						break;
					}


				for (int j = 0; j < m + 1; j++)
					if (sequence[i + 1] <= Fy[j, kx])
					{
						ky = j - 1;
						break;
					}

				empiricalPM[ky, kx] += 1.0;
				//X.Add(A[1, kx - 1]);
				//Y.Add(B[1, ky - 1]);
			}

			for (int j = 0; j < m; j++)
				for (int i = 0; i < n; i++)
					empiricalPM[j, i] /= numbersCount;

			return empiricalPM;
		}

		private double[] GetPyx(double[,] Py, int index)
		{
			double[] Pyx = new double[pmM];
			for (int i = 0; i < pmM; i++) Pyx[i] = Py[i, index];
			return Pyx;
		}

		private void DrawBarChart(ChartParams chartParams)
		{
			double left = chartParams.Range.Min() - 1;
			double right = chartParams.Range.Max() + 1;

			chart.Axes.Clear();
			LinearAxis xAxis = new LinearAxis();
			xAxis.Orientation = AxisOrientation.X;
			xAxis.Location = AxisLocation.Bottom;
			xAxis.Minimum = left;
			xAxis.Maximum = right;
			xAxis.Interval = 1.0;

			if (chart.Axes.Count > 1) chart.Axes.RemoveAt(1);
			chart.Axes.Add(xAxis);

			chart.Title = chartParams.Title;
			ColumnSeries seriesTheor = (ColumnSeries)chart.Series[0];
			ColumnSeries seriesEmpirical = (ColumnSeries)chart.Series[1];

			if ((right - left) < 5)
			{
				seriesTheor.DataPointStyle = FindResource("colStyle1") as Style;
				seriesEmpirical.DataPointStyle = FindResource("colStyle2") as Style;
			}
			else
			{
				seriesTheor.DataPointStyle = FindResource("colStyle1b") as Style;
				seriesEmpirical.DataPointStyle = FindResource("colStyle2b") as Style;
			}

			seriesTheor.ItemsSource = chartParams.FreqsLists[0];
			seriesEmpirical.ItemsSource = chartParams.FreqsLists[1];
		}

		private bool CheckPM(double[,] matrix)
		{
			double sum = 0;
			for (int i = 0; i < pmN; i++)
				for (int j = 0; j < pmM; j++)
					sum += matrix[j, i];

			return sum == 1;
		}

		private void btnStart_Click(object sender, RoutedEventArgs e)
		{
			btnNextChart.IsEnabled = true;
			btnPrevChart.IsEnabled = true;
			charts.Clear();
			chartIndex = 0;

			double[] Px = CalcDistributionSeriesX(pmN, pmM, PM);
			double[] Fx = CalcDistributionFunctionX(pmN, Px);
			double[,] Py = CalcDistributionSeriesY(pmN, pmM, PM, Px);
			double[,] Fy = CalcDistributionFunctionY(pmN, pmM, Py);
			double[,] empiricalPM = CalcProbabilityMatrix(pmN, pmM, Fx, Fy);

			double[] empiricalPx = CalcDistributionSeriesX(pmN, pmM, empiricalPM);
			charts.Add(CreateChartX("X", Px, empiricalPx));

			double[,] empiricalPy = CalcDistributionSeriesY(pmN, pmM, empiricalPM, empiricalPx);
			for (int i = 0; i < pmN; i++)
			{
				double[] Pyx = GetPyx(Py, i);
				double[] empiricalPyx = GetPyx(empiricalPy, i);
				charts.Add(CreateChartY("Y при X=" + A[0, i], Pyx, empiricalPyx));
			}

			DrawBarChart(charts[chartIndex]);

			double Mx = CalcME(empiricalPM, A, 0);
			double My = CalcME(empiricalPM, B, 1);
			labelMx.Content = "M[x] = " + Math.Round(Mx, 5);
			labelMy.Content = "M[y] = " + Math.Round(My, 5);

			double Dx = CalcD(empiricalPM, A, Mx, 0);
			double Dy = CalcD(empiricalPM, B, My, 1);
			labelDx.Content = "D[x] = " + Math.Round(Dx, 5);
			labelDy.Content = "D[y] = " + Math.Round(Dy, 5);

			double Rxy = CalcCorrelationCoeff(empiricalPM, Mx, My, Dx, Dy);
			labelRxy.Content = "R[x, y] = " + Math.Round(Rxy, 5);
		}

		private void tbPM_N_KeyUp(object sender, KeyEventArgs e)
		{
			string text = ((TextBox)sender).Text;
			if (text == "") return;

			int n = Convert.ToInt32(text);
			if (n == 0) return;

			A = CopyMatrix(pmN, 1, n, 1, A, new double[1, n]);
			PM = CopyMatrix(pmN, pmM, n, pmM, PM, new double[pmM, n]);

			UpdateMatrix(n, 1, A, 30, dgA);
			UpdateMatrix(n, pmM, PM, 40, dgPM);
			pmN = n;
		}

		private void tbPM_M_KeyUp(object sender, KeyEventArgs e)
		{
			string text = ((TextBox)sender).Text;
			if (text == "") return;

			int m = Convert.ToInt32(text);
			if (m == 0) return;

			B = CopyMatrix(pmM, 1, m, 1, B, new double[1, m]);
			PM = CopyMatrix(pmN, pmM, pmN, m, PM, new double[m, pmN]);

			UpdateMatrix(m, 1, B, 30, dgB);
			UpdateMatrix(pmN, m, PM, 40, dgPM);
			pmM = m;
		}

		private void btnNextChart_Click(object sender, RoutedEventArgs e)
		{
			if (chartIndex == charts.Count - 1) return;
			DrawBarChart(charts[++chartIndex]);
		}

		private void btnPrevChart_Click(object sender, RoutedEventArgs e)
		{
			if (chartIndex == 0) return;
			DrawBarChart(charts[--chartIndex]);
		}
	}
}
