namespace MM_Lab4
{
	public class Bid
	{
		static public int bidsCount = 0;

		public int id;
		public double ServingTime { get; set; }

		public int GetBidId() => bidsCount++;

		public Bid(double servingTime)
		{
			ServingTime = servingTime;
			id = GetBidId();
		}

		public override string ToString()
		{
			return "Bid<" + id + ">";
		}
	}
}
