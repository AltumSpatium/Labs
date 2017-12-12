using System;
using System.Collections.Generic;

namespace MM_Lab4
{
	public class BidSource
	{
		private double bidReceiveTime;
		private delegate IEnumerator<double> DistributionLaw(double[] sequence);
		private IEnumerator<double> DL;

		public BidSource(Func<double[], IEnumerator<double>> newDL, double[] sequence)
		{
			DistributionLaw dl = new DistributionLaw(newDL);
			DL = dl(sequence);
			bidReceiveTime = GetNextBidReceiveTime();
		}

		private double GetNextBidReceiveTime()
		{
			DL.MoveNext();
			return Math.Round(DL.Current, 2);
		}

		public int ReceiveBid(Phase phase, double modelTime)
		{
			if (bidReceiveTime > modelTime) return 0;
			bidReceiveTime = modelTime + GetNextBidReceiveTime();

			Bid bid = new Bid(phase.GetServingTime());
			bool bidReceived = false;

			foreach (Channel channel in phase.Channels)
			{
				if (channel.ChannelState != ChannelState.Free) continue;

				channel.ServeBid(bid, modelTime);
				bidReceived = true;
				break;
			}

			if (bidReceived) return 0;
			if (phase.Accumulator.Count == phase.AccumulatorCapacity) return 1;

			phase.Accumulator.Add(bid);
			return 0;
		}
	}
}
