namespace MM_Lab3
{
	public class Sensor
	{
		protected virtual double Method(double iv) => 0;

		protected virtual double[] ProcessSequence(double[] sequence) => sequence;

		private double[] GenerateSequence(int count, double iv)
		{
			double[] sequence = new double[count];
			double current = iv;

			for (int i = 0; i < count; i++)
			{
				current = Method(current);
				sequence[i] = current;
			}
			return ProcessSequence(sequence);
		}

		public double[] Generate(int count, double iv) => GenerateSequence(count, iv);
	}
}
