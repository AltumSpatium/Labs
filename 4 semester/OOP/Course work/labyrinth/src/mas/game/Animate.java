//*************************************************************************************************

package mas.game;

//.................................................................................................

import java.awt.Canvas;
import java.awt.Graphics;
import java.awt.Point;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.awt.image.BufferStrategy;

//*************************************************************************************************

public class Animate
{
	//.............................................................................................

	private Primitive[] elementmap = null;
	private boolean keypressed = false;
	private Point position = new Point(0, 0);

	//.............................................................................................

	private KeyAdapter keyadapter = new KeyAdapter()
	{
		public void keyPressed (KeyEvent _keyevent)
		{
			keypressed = true;
		}
	};

	//.............................................................................................

	public Animate (String _resourcelink)
	{
		elementmap = Builder.loadObjects(_resourcelink, "animate", position);
	}

	//.............................................................................................

	public Primitive[] getElements ()
	{
		return elementmap;
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
				if (item == _afterprimitive) //?
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

	public void runProcess (Canvas _canvas, long _animatetime, boolean _usescankey)
	{
		if (elementmap != null)
		{
			BufferStrategy bufferstrategy = null;

			while ((bufferstrategy = _canvas.getBufferStrategy()) == null)
			{
				_canvas.createBufferStrategy(2);
				_canvas.requestFocus();
			}

			keypressed = false;
			_animatetime = _animatetime + System.currentTimeMillis();

			if (_usescankey)
				_canvas.addKeyListener(keyadapter);

			while (_animatetime > System.currentTimeMillis() && !keypressed)
			{
				Graphics graphics = bufferstrategy.getDrawGraphics();

				for (int index = 0; index < elementmap.length; index++)
				{
					elementmap[index].animateUpdate();
					elementmap[index].drawImage(graphics);
				}

				graphics.dispose();
				bufferstrategy.show();
				Utils.delayMillis(50);
			}

			if (_usescankey)
				_canvas.removeKeyListener(keyadapter);
		}
	}

	//.............................................................................................
}

//*************************************************************************************************
