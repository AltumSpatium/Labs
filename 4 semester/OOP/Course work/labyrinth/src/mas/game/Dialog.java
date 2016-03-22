//*************************************************************************************************

package mas.game;

//.................................................................................................

import java.awt.Canvas;
import java.awt.Graphics;
import java.awt.Point;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.image.BufferStrategy;

//*************************************************************************************************

public class Dialog
{
	//.............................................................................................

	private Primitive[] elementmap = null;
	private KeyEvent lastkeyevent = null;
	private KeyEvent nextkeyevent = null;
	private Point position = new Point(0, 0);
	private Canvas canvas = null;

	private Sound clicksound = new Sound("@click.wav");
	private Sound selectsound = new Sound("@select.wav");
	private Sound cancelsound = new Sound("@cancel.wav");

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

	private MouseListener mouseclickadapter = new MouseListener()
	{
		public void mouseClicked (MouseEvent _event)
		{
			Point mp = _event.getPoint();

			for (Primitive element : elementmap)
			{
				if (element.isFocus() && element.isContaint(mp.x, mp.y))
					nextkeyevent = new KeyEvent(canvas, KeyEvent.VK_ENTER, 0, 0,
					 KeyEvent.VK_ENTER, '\n');
			}
		}

		//.........................................................................................

		public void mousePressed (MouseEvent _event)
		{			
		}

		//.........................................................................................

		public void mouseReleased (MouseEvent _event)
		{			
		}

		//.........................................................................................

		public void mouseEntered (MouseEvent _event)
		{			
		}

		//.........................................................................................

		public void mouseExited (MouseEvent _event)
		{			
		}
	};

	//.............................................................................................

	private MouseMotionListener mousemoveadapter = new MouseMotionListener()
	{
		public void mouseDragged (MouseEvent _event)
		{			
		}

		//.........................................................................................

		public void mouseMoved (MouseEvent _event)
		{
			if (_event.getID() == MouseEvent.MOUSE_MOVED)
			{
				Point mp = _event.getPoint();
				Primitive focuseditem = getFocus(), newfocuseditem = null;

				for (Primitive element : elementmap)
				{
					if (element.canFocus() && !element.isFocus() && element.isContaint(mp.x, mp.y))
						newfocuseditem = element;
				}

				if (newfocuseditem != null && newfocuseditem != focuseditem)
				{
					if (focuseditem != null)
						focuseditem.setFocus(false);

					newfocuseditem.setFocus(true);
				}
			}
		}
	};

	//.............................................................................................

	private void dispatchKeyEvent(KeyEvent _keyevent, boolean _pressed)
	{
		if (_pressed && lastkeyevent == null)
		{
			lastkeyevent = _keyevent;

			if (nextkeyevent == null)
				nextkeyevent = _keyevent;
		}
		else if (!_pressed && lastkeyevent != null)
		{
			lastkeyevent = null;
		}
	}

	//.............................................................................................

	private KeyEvent getKeyEvent ()
	{
		KeyEvent keyevent = nextkeyevent;
		nextkeyevent = null;

		return keyevent;
	}

	//.............................................................................................

	private Primitive getFocus ()
	{
		for (Primitive item : elementmap)
		{
			if (item.isFocus())
				return item;
		}

		return null;
	}

	//.............................................................................................

	private Primitive setFocus (int _direction)
	{
		int firstindex = -1, previndex = -1, nextindex = -1, focusindex = -1;

		for (int index = 0; index < elementmap.length; index++)
		{
			Primitive item = elementmap[index];

			if (item.canFocus())
			{
				if (firstindex < 0)
					firstindex = index;
			}

			if (focusindex < 0)
			{
				if (item.isFocus())
					focusindex = index;

				if (focusindex < 0 && item.canFocus())
					previndex = index;
			}
			else if (nextindex < 0 && item.canFocus())
				nextindex = index;

			if (item.isFocus())
				item.setFocus(false);
		}

		if (_direction == 0 || focusindex < 0)
			focusindex = firstindex;
		else if (_direction < 0)
			focusindex = (previndex < 0 ? focusindex : previndex);
		else focusindex = (nextindex < 0 ? focusindex : nextindex);

		if (focusindex >= 0)
			elementmap[focusindex].setFocus(true);

		if (focusindex < 0 || focusindex >= elementmap.length)
			return null;

		return elementmap[focusindex];
	}

	//.............................................................................................

	public Dialog (String _resourcelink)
	{
		elementmap = Builder.loadObjects(_resourcelink, "dialog", position);
	}

	//.............................................................................................

	public Primitive[] getElements ()
	{
		return elementmap;
	}

	//.............................................................................................

	public Primitive getPrimitive (String _id, int _type, Primitive _afterprimitive)
	{
		if (elementmap == null)
			return null;

		for (Primitive item : elementmap)
		{
			if (item == null)
				continue;

			if (_afterprimitive != null)
			{
				if (item == _afterprimitive)
					_afterprimitive = null;
			}
			else
			{
				if ((_id == null || _id.equals(item.getId())) &&
					(_type < 0 || item.getType() == _type))
					return item;
			}
		}

		return null;
	}

	//.............................................................................................

	public void setPosition (int _left, int _top)
	{
		if (elementmap != null)
		{
			for (Primitive primitive : elementmap)
			{
				if (primitive != null)
					primitive.setPos(_left + (primitive.getLeft() - position.x),
						_top + (primitive.getTop() - position.y));
			}
		}

		position.setLocation(_left, _top);
	}

	//.............................................................................................

	public String runProcess (Canvas _canvas, String _cancelid, String _focusid)
	{
		String command = _cancelid;

		if (elementmap != null)
		{
			BufferStrategy bufferstrategy = null;

			while ((bufferstrategy = _canvas.getBufferStrategy()) == null)
			{
				_canvas.createBufferStrategy(2);
				_canvas.requestFocus();
			}

			if (_focusid != null)
			{
				for (Primitive item : elementmap)
					item.setFocus(item.canFocus() && item.getId().equals(_focusid) ? true : false);
			}
			else setFocus(0);

			canvas = _canvas;
			_canvas.addKeyListener(keyadapter);
			_canvas.addMouseListener(mouseclickadapter);
			_canvas.addMouseMotionListener(mousemoveadapter);

			while (true)
			{
				KeyEvent keyevent = getKeyEvent();

				if (keyevent != null)
				{
					if (keyevent.getKeyCode() == KeyEvent.VK_ESCAPE)
					{
						if (command != null)
						{
							cancelsound.waitClip();
							break;
						}
					}
					else if (keyevent.getKeyCode() == KeyEvent.VK_ENTER)
					{
						Primitive item = getFocus();

						if (!item.getId().isEmpty())
						{
							selectsound.waitClip();
							command = item.getId();
							break;
						}
					}
					else if (keyevent.getKeyCode() == KeyEvent.VK_LEFT ||
						keyevent.getKeyCode() == KeyEvent.VK_UP)
					{
						Primitive item = getFocus();

						if (item != setFocus(-1))
							clicksound.playClip();
					}
					else if (keyevent.getKeyCode() == KeyEvent.VK_RIGHT ||
						keyevent.getKeyCode() == KeyEvent.VK_DOWN)
					{
						Primitive item = getFocus();

						if (item != setFocus(1))
							clicksound.playClip();
					}
				}
				else Utils.delayMillis(50);

				Graphics graphics = bufferstrategy.getDrawGraphics();

				for (int index = 0; index < elementmap.length; index++)
				{
					elementmap[index].animateUpdate();
					elementmap[index].drawImage(graphics);
				}

				graphics.dispose();
				bufferstrategy.show();
			}

			_canvas.removeMouseListener(mouseclickadapter);
			_canvas.removeMouseMotionListener(mousemoveadapter);
			_canvas.removeKeyListener(keyadapter);
			canvas = null;
			lastkeyevent = null;
		}

		return command;
	}

	//.............................................................................................
}

//*************************************************************************************************
