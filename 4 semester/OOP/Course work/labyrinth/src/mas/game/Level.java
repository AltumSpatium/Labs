//*************************************************************************************************

package mas.game;

//.................................................................................................

import java.awt.Canvas;
import java.awt.Graphics;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.awt.image.BufferStrategy;
import java.util.Random;
import java.util.Vector;

//*************************************************************************************************

public class Level
{
	//.............................................................................................

	public static class Config
	{
		//.........................................................................................

		public Picture background = new Picture(Builder.typeStatic, "background", "@level.png");
		public Sound sound = new Sound("@game.wav");
		public String link = "@objects";

		//.........................................................................................
	}

	//.............................................................................................

	public enum Result { CANCEL, LOSER, WINNER, NEXT };

	//.............................................................................................

	private static class Cell
	{
		//.........................................................................................

		public int row = 0;
		public int col = 0;

		//.........................................................................................

		public Cell (int _row, int _col)
		{
			row = _row;
			col = _col;
		}

		//.........................................................................................
	};

	//.............................................................................................

	private static Random random = new Random();

	private boolean keyup = false;
	private boolean keydown = false;
	private boolean keyleft = false;
	private boolean keyright = false;
	private boolean keyexit = false;
	private boolean keypause = false;
	private boolean keybomb = false;
	private boolean keymine = false;

	private Config config = new Config();
	private Animate pauseanimate = new Animate("@animate.map:pause");
	private Sound bonussound = new Sound("@bonus.wav");
	private Sound keysound = new Sound("@key.wav");
	private Sound firesound = new Sound("@fire.wav");
	private Sound diesound = new Sound("@die.wav");
	private Sound nextsound = new Sound("@next.wav");
	private Sound portsound = new Sound("@port.wav");
	private Sound woodsound = new Sound("@wood.wav");
	private Sound powersound = new Sound("@power.wav");
	private Sound bombsound = new Sound("@bomb.wav");
	private Sound minesound = new Sound("@mine.wav");
	private Sound explosesound = new Sound("@explose.wav");
	private Sound destroysound = new Sound("@destroy.wav");
	private Sound killsound = new Sound("@kill.wav");
	private Sound watersound = new Sound("@water.wav");
	private Sound lifesound = new Sound("@life.wav");

	private Hero hero = null;
	private Vector<Ghost> ghosts = new Vector<Ghost>();

	private Primitive[][] levelmap = null;
	private Vector<Primitive> statusmap = new Vector<Primitive>();
	private Primitive[] pausemap = null;
	private int lastobjecttype = 0;
	private int needkeycount = 0;
	private boolean ispaused = false;

	//.............................................................................................

	private KeyAdapter keyadapter = new KeyAdapter()
	{
		public void keyPressed (KeyEvent _keyevent)
		{
			dispatchKeyEvent(_keyevent, true);
		}

		//.........................................................................................

		public void keyReleased (KeyEvent _keyevent)
		{
			dispatchKeyEvent(_keyevent, false);
		}
	};

	//.............................................................................................

	private void dispatchKeyEvent (KeyEvent _keyevent, boolean _pressed)
	{
		if (_keyevent.getKeyCode() == KeyEvent.VK_SPACE && !_pressed)
			keypause = true;

		if (_keyevent.getKeyCode() == KeyEvent.VK_B && !_pressed)
			keybomb = true;

		if (_keyevent.getKeyCode() == KeyEvent.VK_M && !_pressed)
			keymine = true;

		if (_keyevent.getKeyCode() == KeyEvent.VK_ESCAPE && _pressed)
			keyexit = true;

		if (_keyevent.getKeyCode() == KeyEvent.VK_LEFT)
			keyleft = _pressed;

		if (_keyevent.getKeyCode() == KeyEvent.VK_RIGHT)
			keyright = _pressed;

		if (_keyevent.getKeyCode() == KeyEvent.VK_UP)
			keyup = _pressed;

		if (_keyevent.getKeyCode() == KeyEvent.VK_DOWN)
			keydown = _pressed;
	}

	//.............................................................................................

	private Vector<Cell> findObjects (String _id, int _type, Primitive _excxprimitive)
	{
		Vector<Cell> list = new Vector<Cell>();

		for (int row = 0; row < levelmap.length; row++)
		{
			for (int col = 0; col < levelmap[row].length; col++)
			{
				Primitive primitive = levelmap[row][col];

				if (primitive == null)
					continue;

				if (_id == null || _id.equals(primitive.getId()))
					if (_type < 0 || primitive.getType() == _type)
						if (_excxprimitive == null || _excxprimitive != primitive)
							list.add(new Cell(row, col));
			}
		}

		return (lis.size() > 0 ? list : null);
	}

	//.............................................................................................

	private void destroyObject (int _row, int _col)
	{
		if (levelmap[_row][_col] != null)
		{
			if (levelmap[_row][_col].getType() == Builder.typeHedge)
			{
				levelmap[_row][_col] = new Sprite(Builder.typeWaiting, "destroy",
					config.link + ":explose", levelmap[_row][_col].getLeft(), levelmap[_row][_col].getTop());

				levelmap[_row][_col].setAnimate(true, 50, false);
				destroysound.playClip();
				hero.scores += 10;
			}
			else if (levelmap[_row][_col].getType() == Builder.typeBomb ||
				levelmap[_row][_col].getType() == Builder.typeMine ||
				(levelmap[_row][_col].getType() == Builder.typeWaiting && levelmap[_row][_col].getId().equals("mine")))
			{
				levelmap[_row][_col] = new Sprite(Builder.typeWaiting, "explose",
				config.link + ":explose", levelmap[_row][_col].getLeft(), levelmap[_row][_col].getTop());

				levelmap[_row][_col].setAnimate(true, 50, false);
				explosesound.playClip();
				hero.scores += 10;
			}
		}

		for (Ghost ghost : ghosts)
		{
			if (ghost.isActive() && ghost.getRow == _row && ghost.getCol == _col)
			{
				killsound.playClip();
				ghost.setState(Person.STATE_DIE);
				hero.scores += 15;
			}
		}

		if (hero.isActive && hero.getRow == _row && hero.getCol == _col)
		{
			hero.setState(Person.STATE_FIRE);
			firesound.playClip();
		}
	}

	//.............................................................................................

	private void clearObjects ()
	{
		for (int index = 0; index < ghosts.size(); index++)
		{
			if (!ghosts.get(index).isActive())
				ghosts.remove(index--);
		}

		for (int row = 0; row < levelmap.length; row++)
		{
			for (int col = 0; col < levelmap[row].length; col++)
			{
				Primitive primitive = levelmap[row][col];

				if (primitive == null)
					continue;

				if (primitive.getType()  == Builder.typeBonus && !primitive.isPlaying())
					levelmap[row][col] = null;
				else if (primitive.getType() == Builder.typeWaiting && !primitive.isPlaying())
				{
					Primitive nextprimitive = null;

					if (primitive.getId().equals("bomb"))
					{
						explosesound.playClip();

						nextprimitive = new Sprite(Builder.typeWaiting, "explose",
							config.link + ":explose", primitive.getLeft(), primitive.getTop());

						nextprimitive.setAnimate(true, 100, false);
					}
					else if (primitive.getId().equals("explose"))
					{
						levelmap[row][col] = null;

						destroyObject(row, col);
						destroyObject(row - 1, col);
						destroyObject(row + 1, col);
						destroyObject(row, col - 1);
						destroyObject(row, col + 1);
					}

					levelmap[row][col] = nextprimitive;
				}
			}
		}

		if (hero.getState() == Person.STATE_PORT)
		{
			Primitive primitive = levelmap[hero.getRow()][hero.getCol()];
			Vector<Cell> ports = findObjects("port", Builder.typePort, primitive);

			if (ports != null)
			{
				int numcell = (ports.size() > 1 ? random.nextInt(ports.size()) : 0);

				hero.setRowCol(ports.get(numcell).row, ports.get(numcell).col, true);
			}

			hero.setAnimate(true, 100);
			hero.setState(Person.STATE_WAIT);
		}

		for (int index = 0; index < statusmap.size(); index++)
		{
			Primitive primitive = statusmap.get(index);

			if (primitive.getType() == Builder.typPower && !hero.isPower())
				statusmap.remove(index--);
		}
	}

	//.............................................................................................

	private boolean verifyMove (int _row, int _col, boolean _ishero)
	{
		if (levelmap[_row][_col] == null)
			return true;

		if (levelmap[_row][_col].getType() == Builder.typeStone ||
			levelmap[_row][_col].getType() == Builder.typeHedge ||
			levelmap[_row][_col].getType() == Builder.typeInDoor)
			return false;

		if (levelmap[_row][_col].getType() == Builder.typeOutDoor)
		{
			if (_ishero && needkeycount <= 0)
				return true;

			return false;
		}

		if (!_ishero && (levelmap[_row][_col].getType() == Builder.typeWood ||
			levelmap[_row][_col].getType() == Builder.typePort))
			return false;

		return (levelmap[_row][_col].getType() == typeWood ? _ishero : true);
	}

	//.............................................................................................

	private int lineHero (Ghost ghost)
	{
		if (!hero.isActive())
			return Person.STATE_WAIT;

		if (_ghost.getRow() == hero.getRow())
		{
			int ghostcol = _ghost.getCol(), herocol = hero.getCol();

			if (ghostcol != herocol)
			{
				int row = _ghost.getRow(), direction = (ghostcol < herocol ? 1 : -1);

				while (ghostcol != herocol)
				{
					if (verifyMove(row, ghostcol + direction, false))
						break;

					ghostcol += direction;
				}

				if (ghostcol == herocol)
				{
					if (hero.isPower())
					{
						if (verifyMove(row, _ghost.getCol() - direction, false))
							return (direction < 0 ? Person.STATE_RIGHT : Person.STATE_LEFT);
					}
					else return (direction < 0 ? Person.STATE_LEFT : Person.STATE_RIGHT);
				}
			}
		}
		else if (_ghost.getCol() == hero.getCol())
		{
			int ghostrow = _ghost.getRow(), herorow = hero.getRow();

			if (ghostrow != herorow)
			{
				int col = _ghost.getCol(), direction = (ghostrow < herorow ? 1 : -1);

				while (ghostrow != herorow)
				{
					if (!verifyMove(ghostrow + direction, col, false))
						break;

					ghostrow += direction;
				}

				if (ghostrow == herorow)
				{
					if (hero.isPower())
					{
						if (verifyMove(_ghost.getRow() - direction, col, false))
							return (direction < 0 ? Person.STATE_DOWN : Person.STATE_UP);
					}
					else return (direction < 0 ? Person.STATE_UP : Person.STATE_DOWN);
				}
			}
		}

		return Person.STATE_WAIT;
	}

	//.............................................................................................

	private Primitive crossGhosts ()
	{
		if (!hero.isActive())
			return null;

		for (Ghost ghost : ghosts)
		{
			if (ghost.isActive() && ghost.isRowCol(hero))
			{
				if (!hero.isPower())
					return ghost;

				killsound.playClip();
				ghost.setState(Person.STATE_DIE);
				hero.scores += 15;
			}
		}

		return null;
	}

	//.............................................................................................

	private void updateGhosts ()
	{

	}

	//.............................................................................................

	private void update ()
	{

	}

	//.............................................................................................

	private boolean init (Hero _hero)
	{

	}

	//.............................................................................................

	private void setStatusBar ()
	{

	}

	//.............................................................................................

	public Level ()
	{		
	}

	//.............................................................................................

	public Result runProcess (Hero _hero, Canves _canvas)
	{

	}

	//.............................................................................................
}

//*************************************************************************************************
