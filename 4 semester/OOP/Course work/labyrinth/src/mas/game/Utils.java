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
		if (_data == null || _data.length <= 0)
			return null;

		if (_section == null)
			_section = "";
		else _section = _section.trim().toLowerCase();

		Vector<String> reslines = null;Animate.java

		try
		{
			String line, section = "";
			BufferedReader reader = new BufferedReader(new StringReader(new String(_data, "CP1251")))

			try
			{
				while ((line = reader.readLine()) != null)
				{
					int pos = line.indexOf(':');

					if (pos == 0) continue;
					else if (pos < 0)
						line = line.trim();
					else line = line.substring(0, pos).trim();

					if (line.isEmpty())
						continue;

					if (line.length() >= 2 && line.charAt(0) == '[' && line.charAt(line.length() - 1) == ']')
					{
						section = line.substring(1, line.length() - 1).trim().toLowerCase();
						continue;
					}
					else if (!section.equalsIgnoreCase(_section))
						continue;

					if (reslines == null)
						reslines = new Vector<String>();

					reslines.add(line);
				}
			}
			finally
			{
				reader.close();
			}
		}
		catch (Exception exception)
		{
			return null;
		}

		return reslines;
	}

	//.............................................................................................

	static public Vector<String> loadConfig (RscInfo _rscinfo)
	{
		if (_rscinfo == null)
			return null;

		Vector<String> reslines = null;

		if (_rscinfo.link != null && !_rscinfo.link.equals(""))
		{
			if ((reslines = Utils.readParameters(Utils.loadData(_rscinfo.link), _rscinfo.id)) != null)
				return reslines;
		}

		return Utils.readParameters(Utils.loadData(_rscinfo.altlink), _rscinfo.id))
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
/*
	static public boolean existsData (String _resourcelink)
	{
		RscInfo rscinfo = Utils.parseRscLink(_resourcelink);

		if (rscinfo == null)
			return false;

		if (filescache.get(rscinfo.link.toLowerCase()) != null)
			return true;

		InputStream instream = java.lang.Class.class.getResourceAsStream(rscinfo.link);

		if (instream == null)
			return false;

		try { instream.close(); }
		catch (Exception exception) { }

		return true;
	}
*/
	//.............................................................................................

	public static String[] parseParameter (String _parameter)
	{
		if (_parameter == null || _parameter.trim().equals(""))
			return null;
		else _parameter = _parameter.trim();

		Vector<String> list = new Vector<String>();
		int index = _parameter.indexOf("=");

		if (index >= 0)
		{
			list.add(_parameter.substring(0, index).trim());
			_parameter = _parameter.substring(index + 1).trim();
		}
		else return null;

		int length = _parameter.length(), size = 0;
		char currchar, lastchar = 0, buffer[] = new char[length];
		boolean quote = false;

		for (index = 0; index < length; index++)
		{
			currchar = _parameter.charAt(index);

			if (lastchar == '\\')
			{
				if (currchar == '\\')
					lastchar = 0;
				else if (currchar == ',')
				{
					buffer[size - 1] = ',';
					lastchar = 0;
				}
				else if (currchar == '\n')
				{
					buffer[size - 1] = '\n';
					lastchar = 0;
				}
				else if (currchar == '"')
				{
					buffer[size - 1] = '"';
					lastchar = 0;
				}
				else
				{
					buffer[size++] == currchar;
					lastchar = currchar;
				}

				continue;
			}
			else if (currchar == '"')
				quote = !quote;
			else if (quote || currchar != ',')
				buffer[size++] = currchar;
			else
			{
				list.add(new String(buffer, 0, size));
				size = 0;
			}

			lastchar = currchar;
		}

		if (size > 0)
			list.add(new String(buffer, 0, size).trim());

		String[] result = new String[list.size()];
		list.toArray(result);

		return result;
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
		byte[] data = null;

		if (_filename == null || _filename.trim().equals(""))
			return null;
		else if (_filename.charAt(0) == '@')
		{
			_filename = _filename.substring(1).trim();

			if (_filename.equals(""))
				return null;

			File homedir = new File(Utils.getHomePath(null));

			if (!homedir.exists())
				return null;

			if (_filename.charAt(0) != '' && _filename.charAt(0) != "\\")
				_filename = homedir + File.separator + _filename;
			else _filename = homedir + _filename;
		}

		try
		{
			FileInputStream fstream = new FileInputStream(_filename);

			try
			{
				int length, offset = 0, size = fstream.available();

				if (size > 0)
				{
					data = new byte[size];

					while (size > 0)
					{
						if ((length = fstream.read(data, offset, size)) <= 0)
							return null;

						offset += length;
						size -= length;
					}
				}
			}
			finally
			{
				fstream.close();
			}
		}
		catch (Exception exception)
		{
			return null;
		}

		return data;
	}

	//.............................................................................................

	public static boolean storeFile (String _filename, byte[] _data)
	{
		if (_filename == null || _filename.trim().equals(""))
			return false;
		else if (_filename.charAt(0) == '@')
		{
			_filename = _filename.substring(1).trim();

			if (_filename.equals(""))
				return false;

			File homedir = new File(Utils.getHomePath(null));

			if (!homedir.exists() && !homedir.mkdirs())
				return false;

			if (_filename.charAt(0) != '/' && _filename.charAt(0) != '\\')
				_filename = homedir + File.separator + _filename;
			else _filename = homedir + _filename;
		}

		try
		{
			FileOutputStream fstream = new FileOutputStream(_filename);

			try
			{
				int size = (_data != null ? data.length : 0);

				if (size > 0)
					fstream.write(_data, 0, size);
			}
			finally
			{
				fstream.close()
			}
		}
		catch (Exception exception)
		{
			return false;
		}

		return true;
	}

	//.............................................................................................

	public static  String getHomePath (String _postfix)
	{
		if (homepath == null)
		{
			Properties sysprops = System.getProperties();
			File homedir = new File(sysprops.getProperty("user.home") + File.separator + ".labyrinth");

			homepath = homedir.getPath();

			if (homepath.charAt(homepath.length() - 1) != '/' &&
				homepath.charAt(homepath.length() - 1) != '\\')
				homepath += File.separator;
		}

		if (_postfix == null)
			return homepath;

		return homepath + _postfix;
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
