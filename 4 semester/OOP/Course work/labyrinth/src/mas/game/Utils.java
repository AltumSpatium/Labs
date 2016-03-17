//*************************************************************************************************

package mas.game;

//.................................................................................................

import java.awt.Image;
import java.awt.Toolkit;
import java.awt.image.ImageObserver;
import java.io.BufferedReader;
import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.StringReader;
import java.net.URL;
import java.util.HashMap;
import java.util.Properties;
import java.util.Vector;
import javax.sound.sampled.AudioInputStream;
import javax.sound.sampled.AudioSystem;
import javax.sound.sampled.Clip;

//*************************************************************************************************

public class Utils
{
	//.............................................................................................

	private static String homepath = null;
	private static String resourcepath = null;

	//.............................................................................................

	static class RscInfo
	{
		//.........................................................................................

		public String key = null;
		public String id = null;
		public String link = null;
		public String altlink = null;

		//.........................................................................................
	};

	//.............................................................................................

	public static class WaitObserver implements ImageObserver
	{
		//.........................................................................................

		private int infoflags = 0;

		//.........................................................................................

		public boolean imageUpdate (Image _image, int _infoflags, int _left,
		 							int _top, int _width, int _height)
		{
			infoflags |= _infoflags;
			return true;
		}

		//.........................................................................................

		public boolean waitEvent (int _infoflags, long _timeout)
		{
			if (_timeout >= 0)
				_timeout += System.currentTimeMillis();

			while ((infoflags & _infoflags) != _infoflags)
			{
				if (_timeout > 0 && _timeout < System.currentTimeMillis())
					return false;

				Utils.delayMillis(5);

			}

			return true;
		}

		//.........................................................................................
	};

	//.............................................................................................

	private static HashMap<String, Image> imagescache = new HashMap<String, Image>();
	private static  HashMap<String, byte[]> filescache = new HashMap<String, byte[]>();

	//.............................................................................................

	static public Image loadImage (String _resourcelink)
	{
		RscInfo rscinfo = Utils.parseRscLink(_resourcelink);

		if (rscinfo == null)
			return null;

		Image image = imagescache.get(rscinfo.link.toLowerCase());

		if (image == null)
		{
			URL url = java.lang.Class.class.getClass().getResource(rscinfo.link);

			if (url != null)
				image = Toolkit.getDefaultToolkit().getImage(url);

			if (image != null)
			{
				Utils.WaitObserver observer = new WaitObserver();

				image.getWidth(observer);
				image.getHeight(observer);

				if (observer.waitEvent(ImageObserver.WIDTH | ImageObserver.HEIGHT, 3000))
					imagescache.put(rscinfo.link.toLowerCase(), image);
				else image = null;
			}
		}

		return image;
	}

	//.............................................................................................

	static public Clip loadSound (String _resourcelink)
	{
		byte[] sound = loadData(_resourcelink);

		if (sound != null)
		{
			AudioInputStream audiostream = null;
			InputStream datastream = new ByteArrayInputStream(sound);

			try { audiostream = AudioSystem.getAudioInputStream(datastream); }
			catch (Exception exception) { }

			if (audiostream != null)
			{
				try
				{
					Clip clip = AudioSystem.getClip();
					clip.open(audiostream);
					return clip;
				}
				catch (Exception exception) { }
			}
		}

		return null;
	}

	//.............................................................................................

	static public Vector<String> readParameters (byte[] data, String _section)
	{

	}

	//.............................................................................................

	static public Vector<String> loadConfig (RscInfo _rscinfo)
	{

	}

	//.............................................................................................

	static public byte[] loadData (String _resourcelink)
	{
		RscInfo rscinfo = Utils.parseRscLink(_resourcelink);

		if (rscinfo == null)
			return null;

		byte[] data = filescache.get(rscinfo.link.toLowerCase());

		if (data == null)
		{
			InputStream instream = java.lang.Class.class.getResourceAsStream(rscinfo.link);

			if (instream == null)
				return null;

			try
			{
				try
				{
					data = new byte[instream.available()];

					if (data.length > 0)
					{
						for (int size = data.length, offset = 0, count; size > 0; )
						{
							if ((count = instream.read(data, offset, size)) <= 0)
								return null;

							offset += count;
							size -= count;
						}
					}

					filescache.put(rscinfo.link.toLowerCase(), data);
				}
				finally
				{
					instream.close();
				}
			}
			catch (Exception exception)
			{
				return null;
			}
		}

		return data;
	}

	//.............................................................................................

	static public boolean existsData (String _resourcelink)
	{

	}

	//.............................................................................................

	public static String[] parseParameter (String _parameter)
	{

	}

	//.............................................................................................

	public static RscInfo parseRscLink (String _resourcelink)
	{
		if (_resourcelink == null || _resourcelink.trim().equals(""))
			return null;
		else _resourcelink = _resourcelink.trim();

		RscInfo rscinfo = new RscInfo();
		int pos = _resourcelink.indexOf("?");

		if (pos >= 0) /*Finding alternative link*/
		{
			rscinfo.altlink = _resourcelink.substring(pos + 1).trim();

			if ((_resourcelink = _resourcelink.substring(0, pos).trim()).equals(""))
				return null;

			if ((pos = rscinfo.altlink.indexOf(':')) >= 0)
			{
				_resourcelink += rscinfo.altlink.substring(pos);
				rscinfo.altlink = rscinfo.altlink.substring(0, pos).trim();
			}

			if (rscinfo.altlink.length() > 0 && rscinfo.altlink.charAt(0) == '@')
			{
				if (!(rscinfo.altlink = rscinfo.altlink.substring(1).trim()).equals(""))
				{
					String line = rscinfo.altlink;
					rscinfo.altlink = Utils.getResourcePath(null);

					if ((pos = line.lastIndexOf(".")) >= 0)
					{
						String ext = line.substring(pos).toLowerCase();

						if (ext.equals(".png") || ext.equals(".jpg") || ext.equals(".gif"))
							rscinfo.altlink += "images/";
						else if (ext.equals(".wav"))
							rscinfo.altlink += "sounds/";
						else if (ext.equals(".map"))
							rscinfo.altlink += "maps/";
						else if (ext.equals(".lev"))
							rscinfo.altlink += "levels/";
					}

					rscinfo.altlink += line;
				}
			}
		}
		else rscinfo.altlink = "";

		if ((pos = _resourcelink.indexOf(":")) >= 0) /*Finding resource id*/
		{
			rscinfo.id = _resourcelink.substring(pos + 1).trim();

			if ((_resourcelink = _resourcelink.substring(0, pos).trim()).equals(""))
				return null;
		}
		else rscinfo.id = "";

		if (_resourcelink.charAt(0) != '@') /*Finding resource link*/
			rscinfo.link = _resourcelink;
		else
		{
			if ((_resourcelink = _resourcelink.substring(1).trim()).equals(""))
				return null;

			rscinfo.link = Utils.getResourcePath(null);

			if ((pos = _resourcelink.lastIndexOf(".")) >= 0)
			{
				rscinfo.key = _resourcelink.substring(pos).toLowerCase();

				if (rscinfo.key.equals(".png") || rscinfo.key.equals(".jpg") ||
				 	rscinfo.key.equals(".gif"))
					rscinfo.link += "images/";
				else if (rscinfo.key.equals(".wav"))
					rscinfo.link += "sounds/";
				else if (rscinfo.key.equals(".map"))
					rscinfo.link += "maps/";
				else if (rscinfo.key.equals(".lev"))
					rscinfo.link += "levels/";
			}

			rscinfo.link += _resourcelink;
		}

		rscinfo.key = rscinfo.link.toLowerCase();

		if (!rscinfo.id.equals(""))
			rscinfo.key += (":" + rscinfo.id.toLowerCase());

		return rscinfo;
	}

	//.............................................................................................

	public static byte[] loadFile (String _filename)
	{

	}

	//.............................................................................................

	public static boolean storeFile (String _filename, byte[] _data)
	{

	}

	//.............................................................................................

	public static  String getHomePath (String _postfix)
	{

	}

	//.............................................................................................

	public static String getResourcePath (String _postfix)
	{
		if (resourcepath == null)
			resourcepath = "/mas/game/resource/";

		if (_postfix == null)
			return resourcepath;

		return resourcepath + _postfix;
	}

	//.............................................................................................

	public static void delayMillis (long _milliscount)
	{
		try { Thread.sleep(_milliscount); }
		catch (Exception exception) { }
	}

	//.............................................................................................
}

//*************************************************************************************************
