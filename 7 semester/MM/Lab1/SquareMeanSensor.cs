using System;
using System.Linq;

namespace MM_Lab1
{
	public class SquareMeanSensor : Sensor
	{
		private int rank = 0;

		private String ExtendValue(String value, int rank)
		{
			if (value.Contains(","))
			{
				value = value.Replace(",", "");
				value = value.Replace("E+", "");
				int exp = Convert.ToInt32(value.Substring(value.Length - 2));
				value = value.Remove(value.Length - 2);
				value = value + new String('0', exp - value.Length);
			}

			return new String('0', rank * 2 - value.Length) + value;
		}
			

		protected override double Method(double iv)
		{
			if (rank == 0)
				rank = iv.ToString().Length;
			String squareValue = (iv * iv).ToString();
			squareValue = squareValue.Length == rank * 2 ? 
				squareValue : ExtendValue(squareValue, rank);
			return Convert.ToDouble(squareValue.Substring(rank / 2, rank));
		}

		protected override double[] ProcessSequence(double[] sequence)
		{
			double[] newSequence = new double[sequence.Length];
			double min = sequence.Min(), max = sequence.Max();
			for (int i = 0; i < sequence.Length; i++)
				newSequence[i] = (sequence[i] - min) / max;
			return newSequence;
		}
	}
}
