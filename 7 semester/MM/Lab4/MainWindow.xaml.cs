using System;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Input;
using MathNet.Numerics.Distributions;

namespace MM_Lab4
{
	public partial class MainWindow : Window
	{
		private int numbersCount;

		private int uniformA;
		private int uniformB;
		private double exponentialL;
		private double normalM;
		private double normalS;

		private double experimentlength;
		private int accCapacityPhase1;
		private int accCapacityPhase2;
		private string receiveBidDL;
		private string serveBidDLPhase1;
		private string serveBidDLPhase2;
		private string serveBidDLPhase3;

		private string LogPhase(string phaseNumber, Phase phase, double modelTime)
		{
			string output = "";
			output += "Phase: " + phaseNumber + "\n";
			for (int i = 0; i < phase.Channels.Count; i++)
			{
				Channel ch = phase.Channels[i];
				output += "Channel " + (i + 1) + ": " +
					(ch.CurrentBid == null ? "" : ch.CurrentBid + " ") + ch.ChannelState + "\n";
			}
			string accInfo = "";
			foreach (var bid in phase.Accumulator) accInfo += " " + bid + " ";
			output += "[" + accInfo + "]";
			output += "\n\n";
			return output;
		}

		private string LogPhases(List<Phase> phases, double modelTime)
		{
			string output = "Model Time: " + modelTime + "\n\n";
			for (int i = 0; i < phases.Count; i++)
				output += LogPhase(i + 1 + "", phases[i], modelTime);
			return output;
		}

		public MainWindow()
		{
			InitializeComponent();
			List<string> dl = new List<string>() { "Равномерный", "Нормальный", "Экспоненциальный" };
			cbReceiveBidDL.ItemsSource = dl;
			cbServeBidDLPhase1.ItemsSource = dl;
			cbServeBidDLPhase2.ItemsSource = dl;
			cbServeBidDLPhase3.ItemsSource = dl;

			SetParams(10000, 1, 10, 1, 2, 5, 100, 5, 3, "Равномерный", "Равномерный", "Равномерный", "Равномерный");
		}

		private void SetParams(int newNumbersCount, int newUniformA, int newUniformB, double newExponentialL,
			double newNormalM, double newNormalS, double newExperimentLength, int newAccCapacityPhase1,
			int newAccCapacityPhase2, string newReceiveBidDL, string newServeBidDLPhase1,
			string newServeBidDLPhase2, string newServeBidDLPhase3)
		{
			numbersCount = newNumbersCount;
			uniformA = newUniformA;
			uniformB = newUniformB;
			exponentialL = newExponentialL;
			normalM = newNormalM;
			normalS = newNormalS;
			experimentlength = newExperimentLength;
			accCapacityPhase1 = newAccCapacityPhase1;
			accCapacityPhase2 = newAccCapacityPhase2;
			receiveBidDL = newReceiveBidDL;
			serveBidDLPhase1 = newServeBidDLPhase1;
			serveBidDLPhase2 = newServeBidDLPhase2;
			serveBidDLPhase3 = newServeBidDLPhase3;
		}

		private void btnApplySettings_Click(object sender, RoutedEventArgs e)
		{
			if (tbExperimentLength.Text == "" || tbUniformA.Text == "" || tbUniformB.Text == "" ||
				tbExponentialL.Text == "" || tbNormalM.Text == "" || tbNormalS.Text == "" ||
				tbAccCapacityPhase1.Text == "" || tbAccCapacityPhase2.Text == "")
				return;

			double experimentLength = Convert.ToDouble(tbExperimentLength.Text);
			int uniformA = Convert.ToInt32(tbUniformA.Text);
			int uniformB = Convert.ToInt32(tbUniformB.Text);
			double exponentialL = Convert.ToDouble(tbExponentialL.Text);
			double normalM = Convert.ToDouble(tbNormalM.Text);
			double normalS = Convert.ToDouble(tbNormalS.Text);
			int accCapacityPhase1 = Convert.ToInt32(tbAccCapacityPhase1.Text);
			int accCapacityPhase2 = Convert.ToInt32(tbAccCapacityPhase2.Text);
			string receiveBidDL = cbReceiveBidDL.SelectedItem.ToString();
			string serveBidDLPhase1 = cbServeBidDLPhase1.SelectedItem.ToString();
			string serveBidDLPhase2 = cbServeBidDLPhase2.SelectedItem.ToString();
			string serveBidDLPhase3 = cbServeBidDLPhase3.SelectedItem.ToString();

			SetParams(numbersCount, uniformA, uniformB, exponentialL, normalM, normalS, experimentLength,
				accCapacityPhase1, accCapacityPhase2, receiveBidDL, serveBidDLPhase1, serveBidDLPhase2,
				serveBidDLPhase3);
		}

		private void Preview_TextInput(object sender, TextCompositionEventArgs e)
		{
			if (!char.IsDigit(e.Text, e.Text.Length - 1))
				e.Handled = true;
		}

		private IEnumerator<double> EnumerateDL(double[] sequence)
		{
			for (int i = 0; i < sequence.Length; i++)
				yield return sequence[i];
		}

		private double[] GetDistributionLaw(string dlStr)
		{
			double[] sequence = new double[numbersCount];

			switch (dlStr)
			{
				case "Экспоненциальный":
					Exponential.Samples(sequence, exponentialL);
					break;
				case "Нормальный":
					Normal.Samples(sequence, normalM, normalS);
					break;
				case "Равномерный":
					int[] uniformSeq = new int[numbersCount];
					DiscreteUniform.Samples(uniformSeq, uniformA, uniformB);
					for (int i = 0; i < numbersCount; i++) sequence[i] = uniformSeq[i];
					break;
			}

			return sequence;
		}

		private void btnStart_Click(object sender, RoutedEventArgs e)
		{
			Bid.bidsCount = 0;
			var DLEnumerator = new Func<double[], IEnumerator<double>>(EnumerateDL);
			string output = "";
			double modelTime = 0;

			int bidsAll = 0;
			int bidsServed = 0;
			int bidsDeclined = 0;

			List<Channel> channels1 = new List<Channel>() { new Channel(), new Channel() };
			List<Channel> channels2 = new List<Channel>() { new Channel(), new Channel() };
			List<Channel> channels3 = new List<Channel>() { new Channel() };

			Phase phase1 = new Phase(accCapacityPhase1, channels1,
				DLEnumerator, GetDistributionLaw(serveBidDLPhase1));
			Phase phase2 = new Phase(accCapacityPhase2, channels2,
				DLEnumerator, GetDistributionLaw(serveBidDLPhase2));
			Phase phase3 = new Phase(0, channels3,
				DLEnumerator, GetDistributionLaw(serveBidDLPhase3));

			BidSource bidSource = new BidSource(DLEnumerator, GetDistributionLaw(receiveBidDL));

			List<Phase> phases = new List<Phase>() { phase1, phase2, phase3 };

			while (modelTime <= experimentlength)
			{
				int bidServed = phase3.ServePhase(modelTime, true);
				phase2.TransferToNextPhase(phase3, modelTime);
				phase2.ServePhase(modelTime, false);
				phase1.TransferToNextPhase(phase2, modelTime);
				phase1.ServePhase(modelTime, false);
				int bidDeclined = bidSource.ReceiveBid(phase1, modelTime);

				output += LogPhases(phases, modelTime);

				bidsServed += bidServed;
				bidsDeclined += bidDeclined;
				bidsAll += bidServed + bidDeclined;

				output += "Bids Served: " + bidsServed + "\n";
				output += "Bids Declined: " + bidsDeclined + "\n";

				output += "-----------------------------------------\n\n";

				modelTime += 1;
			}

			output += "Bids Received: " + bidsAll + "\n";
			output += "Bids Served: " + Math.Round(bidsServed / (double)bidsAll * 100, 0) + "%\n";
			output += "Bids Declined: " + Math.Round(bidsDeclined / (double)bidsAll * 100, 0) + "%\n";

			tbOutput.Text = output;
		}
	}
}
