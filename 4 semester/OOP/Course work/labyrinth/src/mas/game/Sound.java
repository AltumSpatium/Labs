//*************************************************************************************************

package mas.game;

//.................................................................................................

import javax.sound.sampled.Clip;
import javax.sound.sampled.LineEvent;
import javax.sound.sampled.LineListener;

//*************************************************************************************************

public class Sound
{
	//.............................................................................................

	private Clip clip = null;
	private boolean isplaying = false;
	public String link;

	//.............................................................................................

	LineListener linelistener = new LineListener()
	{
		public void update (LineEvent event)
		{
			if (event.getType().equals(LineEvent.Type.START))
				isplaying = true;

			if (event.getType().equals(LineEvent.Type.STOP))
				isplaying = false;
		}
	};

	//.............................................................................................

	public Sound (String _resourcelink)
	{
		if ((clip = Utils.loadSound(_resourcelink)) != null)
		{
			clip.addLineListener(linelistener);
			link = _resourcelink;
		}
	}

	//.............................................................................................

	public void waitClip ()
	{
		if (!isPlay())
			playClip();

		while (isPlay())
			Utils.delayMillis(50);
	}

	//.............................................................................................

	public void playClip ()
	{
		if (clip != null)
		{
			clip.stop();
			clip.setFramePosition(0);
			clip.start();
			Utils.delayMillis(50);
		}
	}

	//.............................................................................................

	public void loopClip (int _loopcount)
	{
		if (clip != null)
		{
			clip.stop();
			clip.setFramePosition(0);
			clip.loop(_loopcount > 0 ? _loopcount : Clip.LOOP_CONTINUOUSLY);
			Utils.delayMillis(50);
		}
	}

	//.............................................................................................

	public boolean isPlay ()
	{
		return isplaying;
	}

	//.............................................................................................

	public void stopClip ()
	{
		if (clip != null)
		{
			clip.stop();
			clip.setFramePosition(0);
		}
	}

	//.............................................................................................
}

//*************************************************************************************************
