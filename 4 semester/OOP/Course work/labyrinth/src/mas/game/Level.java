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

		return (list.size() > 0 ? list : null);
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
			if (ghost.isActive() && ghost.getRow() == _row && ghost.getCol() == _col)
			{
				killsound.playClip();
				ghost.setState(Person.STATE_DIE);
				hero.scores += 15;
			}
		}

		if (hero.isActive() && hero.getRow() == _row && hero.getCol() == _col)
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

			if (primitive.getType() == Builder.typePower && !hero.isPower())
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

		return (levelmap[_row][_col].getType() == Builder.typeWood ? _ishero : true);
	}

	//.............................................................................................

	private int lineHero (Ghost _ghost)
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
					if (!verifyMove(row, ghostcol + direction, false))
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
		for (Ghost ghost : ghosts)
		{
			if (!ghost.isActive())
				continue;

			if (levelmap[ghost.getRow()][ghost.getCol()] != null &&
				levelmap[ghost.getRow()][ghost.getCol()].getType() == Builder.typeWaiting &&
				levelmap[ghost.getRow()][ghost.getCol()].getId().equals("mine"))
			{
				levelmap[ghost.getRow()][ghost.getCol()] = new Sprite(Builder.typeWaiting, "explose",
					config.link + ":explose", ghost.getLeft(), ghost.getTop());
				levelmap[ghost.getRow()][ghost.getCol()].setAnimate(true, 100, false);
				explosesound.playClip();
				ghost.setState(Person.STATE_DIE);
				hero.scores += 15;

				continue;
			}

			int currstate = lineHero(ghost);

			if (currstate != Person.STATE_WAIT)
			{
				if (currstate == Person.STATE_UP)
					ghost.moveRowCol(-1, 0, false);
				else if (currstate == Person.STATE_DOWN)
					ghost.moveRowCol(1, 0, false);
				else if (currstate == Person.STATE_LEFT)
					ghost.moveRowCol(0, -1, false);
				else if (currstate == Person.STATE_RIGHT)
					ghost.moveRowCol(0, 1, false);
				else currstate = Person.STATE_WAIT;

				ghost.setState(currstate);

				continue;
			}

			int row = ghost.getRow(), col = ghost.getCol();
			int retrycount = 5, substep = random.nextInt(3);

			while (retrycount-- > 0)
			{
				if (ghost.getState() == Person.STATE_UP && verifyMove(row - 1, col, false))
				{
					if (verifyMove(row, col + 1, false) && substep == 1)
					{
						ghost.moveRowCol(0, 1, false);
						currstate = Person.STATE_RIGHT;
					}
					else if (verifyMove(row, col - 1, false) && substep == 2)
					{
						ghost.moveRowCol(0, -1, false);
						currstate = Person.STATE_LEFT;
					}
					else
					{
						ghost.moveRowCol(-1, 0, false);
						currstate = Person.STATE_UP;
					}

					break;
				}
				else if (ghost.getState() == Person.STATE_DOWN && verifyMove(row + 1, col, false))
				{
					if (verifyMove(row, col + 1, false) && substep == 1)
					{
						ghost.moveRowCol(0, 1, false);
						currstate = Person.STATE_RIGHT;
					}
					else if (verifyMove(row, col - 1, false) && substep == 2)
					{
						ghost.moveRowCol(0, -1, false);
						currstate = Person.STATE_LEFT;
					}
					else
					{
						ghost.moveRowCol(1, 0, false);
						currstate = Person.STATE_DOWN;
					}

					break;
				}
				else if (ghost.getState() == Person.STATE_LEFT && verifyMove(row, col - 1, false))
				{
					if (verifyMove(row + 1, col, false) && substep == 1)
					{
						ghost.moveRowCol(1, 0, false);
						currstate = Person.STATE_DOWN;
					}
					else if (verifyMove(row - 1, col, false) && substep == 2)
					{
						ghost.moveRowCol(-1, 0, false);
						currstate = Person.STATE_UP;
					}
					else
					{
						ghost.moveRowCol(0, -1, false);
						currstate = Person.STATE_LEFT;
					}

					break;
				}
				else if (ghost.getState() == Person.STATE_RIGHT && verifyMove(row, col + 1, false))
				{
					if (verifyMove(row + 1, col, false) && substep == 1)
					{
						ghost.moveRowCol(1, 0, false);
						currstate = Person.STATE_DOWN;
					}
					else if (verifyMove(row - 1, 0, false) && substep == 2)
					{
						ghost.moveRowCol(-1, 0, false);
						currstate = Person.STATE_UP;
					}
					else
					{
						ghost.moveRowCol(0, 1, false);
						currstate = Person.STATE_RIGHT;
					}

					break;
				}

				ghost.setState(random.nextInt(5));
			}

			ghost.setState(currstate);
		}
	}

	//.............................................................................................

	private void update ()
	{
		clearObjects();

		Primitive currobject = crossGhosts();

		if (currobject == null)
			currobject = levelmap[hero.getRow()][hero.getCol()];

		if (hero.getState() == Person.STATE_DIE)
			hero.status = Person.STATE_DIE;

		if (hero.getState() == Person.STATE_FIRE)
			hero.status = Person.STATE_FIRE;
		else if (currobject != null)
		{
			if (currobject.getType() == Builder.typeWaiting &&
				currobject.getId().equals("mine") && hero.isActive())
			{
				if (lastobjecttype != Builder.typeWaiting)
				{
					currobject = new Sprite(Builder.typeWaiting, "explose",
						config.link + ":explose", currobject.getLeft(), currobject.getTop());
					currobject.setAnimate(true, 100, false);
					levelmap[hero.getRow()][hero.getCol()] = currobject;
					explosesound.playClip();

					hero.setState(Person.STATE_FIRE);
					firesound.playClip();
				}
			}
			else if (currobject.getType() == Builder.typeGhost)
			{
				if (hero.getState() != Person.STATE_DIE)
				{
					diesound.playClip();
					hero.setState(Person.STATE_DIE);
				}
				else hero.status = Person.STATE_DIE;
			}
			else if (currobject.getType() == Builder.typeFire)
			{
				if (hero.getState() != Person.STATE_FIRE)
				{
					if (hero.buckets <= 0)
					{
						firesound.playClip();
						hero.setState(Person.STATE_FIRE);
					}
					else
					{
						Primitive primitive = new Sprite(Builder.typeBonus, "water", "@objects.map:water",
							currobject.getLeft(), currobject.getTop());

						primitive.setAnimate(true, 100, false);
						levelmap[hero.getRow()][hero.getCol()] = primitive;
						watersound.playClip();
						hero.buckets--;
						hero.scores += 10;
					}
				}
				else hero.status = Person.STATE_FIRE;
			}
			else if (currobject.getType() == Builder.typeOutDoor)
			{
				if (hero.getState() != Person.STATE_WIN)
				{
					nextsound.playClip();
					hero.setState(Person.STATE_WIN);
				}
				else hero.status = Person.STATE_WIN;
			}
			else if (currobject.getType() == Builder.typePort)
			{
				if (hero.getState() != Person.STATE_PORT && lastobjecttype != Builder.typePort)
				{
					portsound.playClip();
					hero.setAnimate(true, 100, false);
					hero.setState(Person.STATE_PORT);
				}
			}
			else if (currobject.getType() == Builder.typeWood)
			{
				if (lastobjecttype != Builder.typeWood)
					woodsound.playClip();
			}
			else if (currobject.getType() != Builder.typeBonus &&
				currobject.getType() != Builder.typeWaiting)
			{
				if (currobject.getType() == Builder.typeKey)
				{
					if (--needkeycount <= 0)
					{
						keysound.playClip();

						Vector<Cell> outdoors = findObjects("outdoor", Builder.typeOutDoor, null);

						if (outdoors != null)
						{
							for (Cell cell : outdoors)
								((Sprite) levelmap[cell.row][cell.col]).loadResource(config.link + ":opendoor");
						}
					}
					else bonussound.playClip();
				}
				else if (currobject.getType() == Builder.typePower)
					powersound.playClip();
				else if (currobject.getType() == Builder.typeLife)
					lifesound.playClip();
				else bonussound.playClip();

				hero.addStatistic(currobject);
				Primitive primitive = new Sprite(Builder.typeBonus, "bonus",
					config.link + ":bonus", currobject.getLeft(), currobject.getTop());
				primitive.setAnimate(true, 100, false);
				levelmap[hero.getRow()][hero.getCol()] = primitive;
			}

			lastobjecttype = currobject.getType();

			if (keybomb)
				keybomb = false;

			if (keymine)
				keymine = false;
		}
		else
		{
			if (keybomb)
			{
				keybomb = false;

				if (hero.isActive() && hero.bombs > 0)
				{
					Primitive explose = new Sprite(Builder.typeWaiting, "bomb",
						config.link + ":waitbomb", hero.getLeft(), hero.getTop());
					explose.setAnimate(true, 200, false);
					levelmap[hero.getRow()][hero.getCol()] = explose;
					hero.bombs--;

					bombsound.playClip();
					lastobjecttype = Builder.typeWaiting;
				}
			}
			else if (keymine)
			{
				keymine = false;

				if (hero.isActive() && hero.mines > 0)
				{
					Primitive mine = new Sprite(Builder.typeWaiting, "mine",
						config.link + ":warmine", hero.getLeft(), hero.getTop());

					levelmap[hero.getRow()][hero.getCol()] = mine;
					hero.mines--;

					minesound.playClip();
					lastobjecttype = Builder.typeWaiting;
				}
			}
			else lastobjecttype = Builder.typeEarth;
		}

		if (hero.isActive() && hero.status == Person.STATE_WAIT &&
			hero.getState() != Person.STATE_PORT)
		{
			if (keyup && verifyMove(hero.getRow() - 1, hero.getCol(), true))
			{
				hero.setState(Person.STATE_UP);
				hero.moveRowCol(-1, 0, false);
			}
			else if (keydown && verifyMove(hero.getRow() + 1, hero.getCol(), true))
			{
				hero.setState(Person.STATE_DOWN);
				hero.moveRowCol(1, 0, false);
			}
			else if (keyleft && verifyMove(hero.getRow(), hero.getCol() - 1, true))
			{
				hero.setState(Person.STATE_LEFT);
				hero.moveRowCol(0, -1, false);
			}
			else if (keyright && verifyMove(hero.getRow(), hero.getCol() + 1, true))
			{
				hero.setState(Person.STATE_RIGHT);
				hero.moveRowCol(0, 1, false);
			}
			else if (levelmap[hero.getRow()][hero.getCol()] == null ||
				levelmap[hero.getRow()][hero.getCol()].getType() != Builder.typePort)
				hero.setState(Person.STATE_WAIT);
		}

		updateGhosts();
	}

	//.............................................................................................

	private boolean init (Hero _hero)
	{
		keyup = false;
		keydown = false;
		keyleft = false;
		keyright = false;
		keyexit = false;
		keypause = false;
		keybomb = false;
		keymine = false;

		if (_hero == null)
			return false;

		String resourcelink = "@" + _hero.level + ".lev";

		if ((levelmap = Builder.loadLevel(resourcelink, config)) == null)
			return false;

		hero = null;
		ghosts.clear();
		statusmap.clear();
		lastobjecttype = 0;
		needkeycount = 0;
		ispaused = false;
		pausemap = null;

		for (int row = 0; row < levelmap.length; row++)
		{
			for (int col = 0; col < levelmap[row].length; col++)
			{
				if (levelmap[row][col] != null)
				{
					if (levelmap[row][col].getType() == Builder.typeKey)
						needkeycount++;
					else if (levelmap[row][col].getType() == Builder.typeHero)
					{
						hero = (Hero) levelmap[row][col];
						hero.copyState(_hero);
						hero.setPause(false);
						hero.setRowCol(row, col, true);
						hero.setState(Person.STATE_WAIT);
						hero.status = Person.STATE_WAIT;
						hero.timepower = 0;
						levelmap[row][col] = null;
					}
					else if (levelmap[row][col].getType() == Builder.typeGhost)
					{
						ghosts.add((Ghost) levelmap[row][col]);
						((Ghost) levelmap[row][col]).setState(Person.STATE_WAIT);
						levelmap[row][col] = null;
					}
				}
			}
		}

		if (hero == null)
		{
			levelmap = null;
			ghosts.clear();
			statusmap.clear();

			return false;
		}

		Primitive[] statusitems = Builder.loadObjects("@animate.map:status", "animate", null);

		if (statusitems != null)
		{
			for (Primitive item : statusitems)
				statusmap.add(item);
		}

		return true;
	}

	//.............................................................................................

	private void setStatusBar ()
	{
		int lives = hero.lives;

		if (hero.diamonds >= 50)
		{
			hero.diamonds = 0;
			hero.lives++;
		}

		if (hero.stars >= 25)
		{
			hero.stars = 0;
			hero.lives++;
		}

		if (hero.lives != lives)
			lifesound.playClip();

		for (Primitive item : statusmap)
		{
			if (item.getId().equals("diamond"))
				item.setText("" + hero.diamonds);
			else if (item.getId().equals("star"))
				item.setText("" + hero.stars);
			else if (item.getId().equals("bucket"))
				item.setText("" + hero.buckets);
			else if (item.getId().equals("bomb"))
				item.setText("" + hero.bombs);
			else if (item.getId().equals("mine"))
				item.setText("" + hero.mines);
			else if (item.getId().equals("life"))
				item.setText("" + hero.lives);
			else if (item.getId().equals("score"))
				item.setText("" + hero.scores);
			else if (item.getId().equals("level"))
				item.setText("" + hero.level);
		}
	}

	//.............................................................................................

	public Level ()
	{		
	}

	//.............................................................................................

	public Result runProcess (Hero _hero, Canvas _canvas)
	{
		Result result = Result.CANCEL;

		if (init(_hero))
		{
			BufferStrategy bufferstrategy = null;

			while ((bufferstrategy = _canvas.getBufferStrategy()) == null)
			{
				_canvas.createBufferStrategy(2);
				_canvas.requestFocus();
			}

			_canvas.addKeyListener(keyadapter);
			config.sound.loopClip(0);

			long updatetime = System.currentTimeMillis() + 50;
			int updatestep = 0;

			while (true)
			{
				if (updatestep <= 0)
				{
					if (!ispaused)
						update();

					setStatusBar();

					if (hero.status == Person.STATE_WIN)
					{
						if (!nextsound.isPlay())
						{
							_hero.copyState(hero);
							_hero.level++;

							result = (Utils.existsData("@" + _hero.level + ".lev") ? Result.NEXT : Result.WINNER);

							break;
						}
					}
					else if (hero.status == Person.STATE_FIRE)
					{
						if (!firesound.isPlay())
						{
							result = Result.LOSER;

							break;
						}
					}
					else if (hero.status == Person.STATE_DIE)
					{
						if (!diesound.isPlay())
						{
							result = Result.LOSER;

							break;
						}
					}

					if (keyexit)
						break;

					if (keypause)
					{
						keypause = false;
						ispaused = !ispaused;
						hero.setPause(ispaused);

						if (ispaused && pauseanimate != null)
							pausemap = pauseanimate.getElements();
						else pausemap = null;
					}

					if (!ispaused)
						updatestep = 8;
				}
				else if (!ispaused)
				{
					if (updatetime < System.currentTimeMillis())
					{
						hero.nextStep(4);

						for (Ghost ghost : ghosts)
							ghost.nextStep(4);

						updatetime = System.currentTimeMillis() + 40;
						updatestep--;
					}
				}

				Graphics graphics = bufferstrategy.getDrawGraphics();

				config.background.drawImage(graphics);
				hero.drawImage(graphics);

				if (!ispaused)
					hero.animateUpdate();

				for (Ghost ghost : ghosts)
				{
					ghost.drawImage(graphics);

					if (!ispaused)
						ghost.animateUpdate();
				}

				for (Primitive[] rows : levelmap)
				{
					for (Primitive primitive : rows)
					{
						if (primitive != null)
						{
							primitive.drawImage(graphics);

							if (!ispaused || primitive.getType() != Builder.typeWaiting)
								primitive.animateUpdate();
						}
					}
				}

				for (int index = 0; index < statusmap.size(); index++)
				{
					Primitive primitive = statusmap.get(index);

					primitive.animateUpdate();
					primitive.drawImage(graphics);
				}

				if (pausemap != null)
				{
					for (Primitive primitive : pausemap)
					{
						primitive.animateUpdate();
						primitive.drawImage(graphics);
					}
				}

				graphics.dispose();
				bufferstrategy.show();
			}

			_canvas.removeKeyListener(keyadapter);
		}
		else _hero.clearState();

		config.sound.stopClip();
		_hero.mark = hero.scores;

		hero = null;
		levelmap = null;
		statusmap.clear();
		lastobjecttype = 0;
		pausemap = null;

		return result;
	}

	//.............................................................................................
}

//*************************************************************************************************
