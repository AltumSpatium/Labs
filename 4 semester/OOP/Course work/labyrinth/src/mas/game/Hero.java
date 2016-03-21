//*************************************************************************************************

package mas.game;

//.................................................................................................

import java.awt.Graphics;
import java.util.Vector;

//*************************************************************************************************

public class Hero extends Person
{
	//.............................................................................................

	private Sprite poweranim = new Sprite(Builder.typeAnimate, null, "@objects.map:power");

	public String profile = "default";
	public String name = "hero";
	public int status = Person.STATE_WAIT;
	public int level = 1;
	public int mark = 0; //?
	public timepower = 0;
	public int scores = 0;
	public int lives = 5;
	public int keys = 0;
	public int diamonds = 0;
	public int stars = 0;
	public int bombs = 0;
	public int mines = 0;
	public int buckets = 0;

	//.............................................................................................

	public Hero (int _type, String _id, String _resourcelink)
	{
		super(_type, _id, _resourcelink);
	}

	//.............................................................................................

	public Hero (int _type, String _id, String _resourcelink, int _row, int _col)
	{
		super(_type, _id, _resourcelink, _row, _col);
	}

	//.............................................................................................

	public void clearState ()
	{
		super.clearState();

		status = Person.STATE_WAIT;
		level = 1;
		mark = 0;
		timepower = 0;
		scores = 0;
		lives = 5;
		keys = 0;
		diamonds = 0;
		stars = 0;
		bombs = 0;
		mines = 0;
		buckets = 0;
	}

	//.............................................................................................

	public void copyState (Hero _hero)	
	{
		super.copyState();

		mark = _hero.mark;
		status = _hero.status;
		lives  = _hero.lives;
		level = _hero.level;
		timepower = _hero.timepower;
		scores = _hero.scores;
		keys = _hero.keys;
		diamonds = _hero.diamonds;
		stars = _hero.stars;
		bombs = _hero.bombs;
		mines = _hero.mines;
		buckets = _hero.buckets;
	}

	//.............................................................................................

	public void setPause (boolean _ispause)
	{
		boolean pause = isPause();

		super.setPause(_ispause);

		if (pause != isPause())
		{
			if (!isPause())
				timepower += System.currentTimeMillis();
			else
				timepower = timepower - System.currentTimeMillis();
		}
	}

	//.............................................................................................

	public void initPower ()
	{
		if (isPause())
			timepower = 30000;
		else
			timepower = System.currentTimeMillis() + 30000;
	}

	//.............................................................................................

	public boolean isPower ()
	{
		if (isPause())
			return (timepower > 0 ? true : false);
		else
			return (timepower > System.currentTimeMillis() ? true : false);
	}

	//.............................................................................................

	public void addStatistic (Primitive _primitive)
	{
		if (_primitive == null)
			return;

		switch(_primitive.getType())
		{
			case Builder.typeDiamond:
				diamonds++;
				scores += 1;
				break;
			case Builder.typeStar:
				stars++;
				scores += 2;
				break;
			case Builder.typeBomb:
				bombs++;
				scores += 3;
				break;
			case Builder.typeMine:
				mines++;
				scores += 3;
				break;
			case Builder.typeBucket:
				buckets++;
				scores += 4;
				break;
			case Builder.typeKey:
				keys++;
				scores += 5;
				break;
			case Builder.typePower:
				initPower();
				scores += 6;
				break;
			case Builder.typeLife:
				lives++;
				scores += 25;
				break;
		}
	}

	//.............................................................................................

	public void animateUpdate ()
	{
		super.animateUpdate();

		if ((getLayerIndex() == STATE_DIE || getLayerIndex() == STATE_FIRE) &&
			getImageIndex() == (getImagesCount() - 1))
			return;

		poweranim.animateUpdate();
		super.animateUpdate();
	}

	//.............................................................................................

	public void animateNext ()
	{
		if ((getLayerIndex() == STATE_DIE || getLayerIndex() == STATE_FIRE) &&
			getImageIndex() == (getImagesCount() - 1))
			return;

		poweranim.animateNext();
		super.animateNext();
	}

	//.............................................................................................

	public void drawImage (Graphics _graphics)
	{
		super.drawImage(_graphics);

		if (isVisible() && isPower() && isActive())
		{
			poweranim.setPos(getLeft(), getTop());
			poweranim.drawImage(_graphics);
		}
	}

	//.............................................................................................

	public void saveProfile ()
	{
		String profilename = "default";

		if (profile != null && profile.trim().length() > 0)
			profilename = profile.trim();

		StringBuffer profile = new StringBuffer();

		profile.append("[" + profilename + "]");
		profile.append("\r\nhero.name=" + name);
		profile.append("\r\nhero.level=" + level);
		profile.append("\r\nhero.scores=" + scores);
		profile.append("\r\nhero.lives=" + lives);
		profile.append("\r\nhero.diamonds=" + diamonds);
		profile.append("\r\nhero.stars=" + stars);
		profile.append("\r\nhero.bombs=" + bombs);
		profile.append("\r\nhero.mines=" + mines);
		profile.append("\r\nhero.buckets=" + buckets);
		profile.append("\r\n");

		Utils.storeFile("@labyrinth.profiles", profile.toString().getBytes());
	}

	//.............................................................................................

	public void loadProfile ()
	{
		clearState();

		if (profile != null && profile.trim().length() > 0)
			profile = profile.trim();
		else profile = "default";

		Vector<String> parameters = Utils.readParameters(Utils.loadFile("@labyrinth.profiles"), profile);

		if (parameters != null)
		{
			String values[];

			for (String line : parameters)
			{
				if ((values = Utils.parseParameters(line)) == null || values.length < 2)
					continue;
				else if (values[0].trim().equalsIgnoreCase("hero.name"))
					name = values[1].trim();
				else if (values[0].trim().equalsIgnoreCase("hero.level"))
					level = Integer.parseInt(values[1].trim());
				else if (values[0].trim().equalsIgnoreCase("hero.scores"))
					scores = Integer.parseInt(values[1].trim());
				else if (values[0].trim().equalsIgnoreCase("hero.lives"))
					lives = Integer.parseInt(values[1].trim());
				else if (values[0].trim().equalsIgnoreCase("hero.diamonds"))
					diamonds = Integer.parseInt(values[1].trim());
				else if (values[0].trim().equalsIgnoreCase("hero.stars"))
					stars = Integer.parseInt(values[1].trim());
				else if (values[0].trim().equalsIgnoreCase("hero.bombs"))
					bombs = Integer.parseInt(values[1].trim());
				else if (values[0].trim().equalsIgnoreCase("hero.mines"))
					mines = Integer.parseInt(values[1].trim());
				else if (values[0].trim().equalsIgnoreCase("hero.buckets"))
					buckets = Integer.parseInt(values[1].trim());
			}
		}
	}

	//.............................................................................................
}

//*************************************************************************************************
