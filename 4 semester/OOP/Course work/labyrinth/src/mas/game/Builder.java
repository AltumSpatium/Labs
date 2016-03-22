//*************************************************************************************************

package mas.game;

//.................................................................................................

import java.awt.Point;
import java.util.Vector;

//*************************************************************************************************

public class Builder
{
	//.............................................................................................

	final static public int typeStatic = 0;
	final static public int typeAnimate = 1;
	final static public int typeButton = 2;
	final static public int typeLabel = 3;
	final static public int typeEarth = 4;
	final static public int typeStone = 5;
	final static public int typeInDoor = 6;
	final static public int typeOutDoor = 7;
	final static public int typeKey = 8;
	final static public int typeDiamond = 9;
	final static public int typeBomb = 10;
	final static public int typeMine = 11;
	final static public int typeBucket = 12;
	final static public int typeWood = 13;
	final static public int typePort = 14;
	final static public int typeFire = 15;
	final static public int typeHero = 16;
	final static public int typeGhost = 17;
	final static public int typeBonus = 18;
	final static public int typeStar = 19;
	final static public int typePower = 20;
	final static public int typeExplose = 21;
	final static public int typeWaiting = 22;
	final static public int typeHedge = 23;
	final static public int typeStatus = 24;
	final static public int typeLife = 25;

	//.............................................................................................

	public static Primitive[] loadObjects (String _resourcelink, String _resourcetype, Point _position)
	{
		if (_resourcetype == null || _resourcetype.trim().equals(""))
			return null;

		_resourcetype = _resourcetype.trim().toLowerCase();

		Utils.RscInfo rscinfo = Utils.parseRscLink(_resourcelink);

		if (rscinfo == null)
			return null;

		Vector<String> parameters = Utils.loadConfig(rscinfo);

		if (parameters == null)
			return null;

		if (_position == null)
			_position = new Point(0, 0);

		Vector<Primitive> objects = new Vector<Primitive>();

		try
		{
			String values[];
			Primitive primitive;

			for (String line : parameters)
			{
				if (_resourcetype != null)
				{
					if ((values = Utils.parseParameter(line)) == null || values.length != 2 ||
						!values[0].trim().equalsIgnoreCase("map.type") || 
						!values[1].trim().equalsIgnoreCase(_resourcetype))
						return null;

					_resourcetype = null;
					continue;
				}

				if ((values = Utils.parseParameter(line)) == null || values.length < 2)
					return null;
				else if (values[0].trim().equalsIgnoreCase("map.left"))
				{
					_position.x = Integer.parseInt(values[1].trim());
					continue;
				}
				else if (values[0].trim().equalsIgnoreCase("map.top"))
				{
					_position.y = Integer.parseInt(values[1].trim());
					continue;
				}

				if (values.length < 4)
					return null;

				int type, left = _position.x, top = _position.y;

				if ((values[1] = values[1].trim().toLowerCase()).equals("static"))
					type = typeStatic;
				else if (values[1].equals("animate"))
					type = typeAnimate;
				else if (values[1].equals("button"))
					type = typeButton;
				else if (values[1].equals("label"))
					type = typeLabel;
				else if (values[1].equals("status"))
					type = typeStatus;
				else return null;

				if (values.length > 4)
					left += Integer.parseInt(values[4].trim());

				if (values.length > 5)
					top += Integer.parseInt(values[5].trim());

				if (values.length > 6)
					line = values[6].trim();
				else line = "";

				if ((values[0] = values[0].trim().toLowerCase()).equals("picture"))
					primitive = new Picture(type, values[2].trim(), values[3].trim(), left, top);
				else if (values[0].equals("button"))
					primitive = new Button(type, values[2].trim(), values[3].trim(), left, top);
				else if (values[0].equals("label"))
					primitive = new Label(type, values[2].trim(), values[3].trim(), left, top, line);
				else if (values[0].equals("sprite"))
					primitive = new Sprite(type, values[2].trim(), values[3].trim(), left, top);
				else if (values[0].equals("status"))
					primitive = new Status(type, values[2].trim(), values[3].trim(), left, top);
				else primitive = null;

				if (primitive == null)
					return null;

				objects.add(primitive);
			}
		}
		catch (Exception exception)
		{
			return null;
		}

		Primitive[] result = new Primitive[objects.size()];
		objects.toArray(result);

		return result;
	}

	//.............................................................................................

	public static Primitive[][] loadLevel (String _resourcelink, Level.Config _config)
	{
		Utils.RscInfo rscinfo = Utils.parseRscLink(_resourcelink);

		if (rscinfo == null)
			return null;

		Vector<String> parameters;
		String values[];

		rscinfo.id = "config";
		parameters = Utils.loadConfig(rscinfo);

		if (parameters != null)
		{
			for (String line : parameters)
			{
				if ((values = Utils.parseParameter(line)) == null || values.length < 2 ||
					values[1].trim().length() <= 0)
					continue;
				else if (values[0].trim().equalsIgnoreCase("level.background"))
				{
					Picture picture = new Picture(Builder.typeStatic, "background", values[1].trim());

					if (picture != null)
						_config.background = picture;

					continue;
				}
				else if (values[0].trim().equalsIgnoreCase("level.sound"))
				{
					Sound sound = new Sound(values[1].trim());

					if (sound != null)
						_config.sound = sound;

					continue;
				}
				else if (values[0].trim().equalsIgnoreCase("level.map"))
				{
					_config.link = values[1].trim();

					continue;
				}
			}
		}

		rscinfo.id = "scheme";
		parameters = Utils.loadConfig(rscinfo);

		if (parameters == null)
			return null;

		if ((_config.link = _config.link.trim()).isEmpty())
			_config.link = "@objects.map";
		else if (!_config.link.equalsIgnoreCase("@objects.map"))
			_config.link = _config.link.trim() + "?@objects.map";

		int maxrow = 15, maxcol = 30, rowindex = 0;
		Primitive[][] level = new Primitive[maxrow][maxcol];
		Primitive primitive;

		for (String line : parameters)
		{
			if (line.length() != maxcol)
				return null;
			else if (rowindex >= maxrow)
				return null;

			for (int colindex = 0; colindex < maxcol; colindex++)
			{
				switch(line.charAt(colindex))
				{
					case 'X':
					case 'x':
						primitive = new Sprite(typeStone, "stone", _config.link + ":stone");
						break;
					case 'H':
					case 'h':
						primitive = new Sprite(typeHedge, "hedge", _config.link + ":hedge");
						break;
					case 'D':
					case 'd':
						primitive = new Sprite(typeDiamond, "diamond", _config.link + ":diamond");
						break;
					case 'L':
					case 'l':
						primitive = new Sprite(typeLife, "life", _config.link + ":life");
						break;
					case 'B':
					case 'b':
						primitive = new Sprite(typeBomb, "bomb", _config.link + ":bomb");
						break;
					case 'V':
					case 'v':
						primitive = new Sprite(typeBucket, "bucket", _config.link + ":bucket");
						break;
					case 'M':
					case 'm':
						primitive = new Sprite(typeMine, "mine", _config.link + ":mine");
						break;
					case 'S':
					case 's':
						primitive = new Sprite(typeStar, "star", _config.link + ":star");
						break;
					case 'K':
					case 'k':
						primitive = new Sprite(typeKey, "key", _config.link + ":key");
						break;
					case 'F':
					case 'f':
						primitive = new Sprite(typeFire, "fire", _config.link + ":fire");
						break;
					case 'W':
					case 'w':
						primitive = new Sprite(typeWood, "wood", _config.link + ":wood");
						break;
					case 'P':
					case 'p':
						primitive = new Sprite(typePower, "power", _config.link + ":power");
						break;
					case 'T':
					case 't':
						primitive = new Sprite(typePort, "port", _config.link + ":port");
						break;
					case 'I':
					case 'i':
						primitive = new Sprite(typeInDoor, "indoor", _config.link + ":closedoor");
						break;
					case 'O':
					case 'o':
						primitive = new Sprite(typeOutDoor, "outdoor", _config.link + ":closedoor");
						break;
					case '*':
						primitive = new Sprite(typeHero, "hero", _config.link + ":hero", rowindex, colindex);
						break;
					case '+':
						primitive = new Sprite(typeGhost, "ghost", _config.link + ":ghost", rowindex, colindex);
					default:
						primitive = null;
				}

				if (primitive != null)
					primitive.setPos(colindex * 32, rowindex * 32);

				level[rowindex][colindex] = primitive;
			}

			rowindex++;
		}

		if (rowindex != maxrow)
			return null;

		return level;
	}

	//.............................................................................................
}

//*************************************************************************************************
