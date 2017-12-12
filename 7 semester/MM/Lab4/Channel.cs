namespace MM_Lab4
{
	public enum ChannelState
	{
		Free = 0,
		Serving = 1,
		Blocked = 2
	}

	public class Channel
	{
		public Bid CurrentBid { get; set; }
		public ChannelState ChannelState { get; set; } = ChannelState.Free;
		private double servingEndTime = 0;

		public bool IsServingEnded(double modelTime) => servingEndTime <= modelTime;
		public void ServeBid(Bid bid, double modelTime)
		{
			ChannelState = ChannelState.Serving;
			CurrentBid = bid;
			servingEndTime = modelTime + bid.ServingTime;
		}
	}
}
