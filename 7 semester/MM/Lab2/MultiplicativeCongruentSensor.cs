namespace MM_Lab2
{
	public class MultiplicativeCongruentSensor : Sensor
	{
		private double m;
		private double k;

		public MultiplicativeCongruentSensor(double m, double k)
		{
			this.m = m;
			this.k = k;
		}

		protected override double Method(double iv) => ((k * iv) % m);

		protected override double[] ProcessSequence(double[] sequence)
		{
			double[] newSequence = new double[sequence.Length];
			for (int i = 0; i < sequence.Length; i++)
				newSequence[i] = sequence[i] / m;
			return newSequence;
		}
	}
}
