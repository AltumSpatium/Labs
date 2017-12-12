using System;
using System.Collections.Generic;

namespace MM_Lab4
{
	public class Phase
	{
		private delegate IEnumerator<double> DistributionLaw(double[] sequence);
		private IEnumerator<double> DL;
		public int AccumulatorCapacity { get; set; }
		public List<Bid> Accumulator;
		public List<Channel> Channels;

		public Phase(int accumulatorCapacity, List<Channel> channels,
			Func<double[], IEnumerator<double>> newDL, double[] sequence)
		{
			AccumulatorCapacity = accumulatorCapacity;
			Accumulator = new List<Bid>();
			Channels = new List<Channel>(channels);
			DistributionLaw dl = new DistributionLaw(newDL);
			DL = dl(sequence);
		}

		public double GetServingTime()
		{
			DL.MoveNext();
			return Math.Round(DL.Current, 2);
		}

		public int ServePhase(double modelTime, bool isLastPhase)
		{
			int servedBidsCount = 0;

			if (isLastPhase)
			{
				foreach (Channel channel in Channels)
				{
					if (channel.ChannelState == ChannelState.Free) continue;
					if (!channel.IsServingEnded(modelTime)) continue;

					servedBidsCount++;
					channel.CurrentBid = null;
					channel.ChannelState = ChannelState.Free;
				}				
			}

			foreach (Channel channel in Channels)
			{
				if (Accumulator.Count == 0) break;

				if (channel.ChannelState == ChannelState.Free)
				{
					Bid bid = Accumulator[0];
					Accumulator.RemoveAt(0);

					channel.ServeBid(bid, modelTime);
				}
			}

			return servedBidsCount;
		}

		public void TransferToNextPhase(Phase phase, double modelTime)
		{
			foreach (Channel channel in Channels)
			{
				if (channel.ChannelState == ChannelState.Free) continue;
				if (!channel.IsServingEnded(modelTime)) continue;

				Bid bid = channel.CurrentBid;
				bid.ServingTime = GetServingTime();
				bool bidTransferred = false;

				foreach (Channel nextChannel in phase.Channels)
				{
					if (nextChannel.ChannelState == ChannelState.Free)
					{
						channel.CurrentBid = null;
						channel.ChannelState = ChannelState.Free;

						nextChannel.ServeBid(bid, modelTime);

						bidTransferred = true;
						break;
					}
				}

				if (bidTransferred) continue;

				if (phase.Accumulator.Count == phase.AccumulatorCapacity)
				{
					channel.ChannelState = ChannelState.Blocked;
					continue;
				}

				channel.CurrentBid = null;
				channel.ChannelState = ChannelState.Free;
				phase.Accumulator.Add(bid);
			}
		}
	}
}