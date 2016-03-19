//*************************************************************************************************

package mas.game;

//.................................................................................................

import java.awt.Graphics;
import java.awt.Image;
import java.util.Random;

//*************************************************************************************************

public class Sprite extends Primitive
{
	//.............................................................................................

	private static Random random = new Random();
	private Map framemap = new Map();
	private Image frameimage = null;

	private boolean isanimate = false;
	private boolean useloop = false;
	private long deltatime = 100 + (random.nextInt(5) * 10);
	private long updatetime = System.currentTimeMillis() + random.nextInt(300) + deltatime;

	private int layerindex = 0;
	private int imageindex = 0;

	//.............................................................................................

	public Sprite (int _type, String _id, String _resourcelink)
	{
		super (_type, _id);
		loadResource(_resourcelink);
	}

	//.............................................................................................

	public Sprite ( int _type, String _id, String _resourcelink, int _left, int _top)
	{
		super(_type, _id, _left, _top);
		loadResource(_resourcelink);
	}

	//.............................................................................................

	public boolean loadResource (String _resourcelink)
	{
		isanimate = false;
		frameimage = null;
		layerindex = 0;
		imageindex = 0;

		if (!framemap.loadItems(_resourcelink))
			return false;

		if ((frameimage = Utils.loadImage(framemap.getLink())) == null)
		{
			framemap.loadItems(null);

			return false;
		}

		if (framemap.getItemsCount() > 1)
			setAnimate(true, deltatime);

		Map.Item item = framemap.getItem(0, 0);

		if (item == null)
			setSize(0, 0);
		else
			setSize(item.width, item.height);

		return true;			
	}

	//.............................................................................................

	public void setFocus (boolean _enabled)
	{		
	}

	//.............................................................................................

	public boolean isFocus ()
	{
		return false;
	}

	//.............................................................................................

	public boolean canFocus ()
	{
		return false;
	}

	//.............................................................................................

	public void setAnimate (boolean _animate, long _deltatime, boolean _useloop)
	{
		if (canAnimate())
		{
			isanimate = _animate;
			deltatime = (_deltatime > 50 ? _deltatime : 50);
			useloop = _useloop;
		}
	}

	//.............................................................................................

	public boolean isAnimate ()
	{
		return isanimate;
	}

	//.............................................................................................

	public boolean isPlaying ()
	{
		if (isAnimate() && frameimage != null)
		{
			if (useloop)
				return true;

			return ((imageindex + 1) >= framemap.getItemsCount(layerindex) ? false : true);
		}

		return false;
	}

	//.............................................................................................

	public boolean canAnimate ()
	{
		return true;
	}

	//.............................................................................................

	public void drawImage (Graphics _graphics)
	{
		if (isVisible() && frameimage != null)
		{
			Map.Item item = framemap.getItem(layerindex, imageindex);

			if (item != null)
				_graphics.drawImage(frameimage, getLeft(), getTop(), getRight() + 1, getBottom() + 1,
					item.left, item.top, item.right + 1, item.bottom + 1, null);
		}
	}

	//.............................................................................................

	public int getLayersCount ()
	{
		return framemap.getLayersCount();
	}

	//.............................................................................................

	public int getLayerIndex ()
	{
		return layerindex;
	}

	//.............................................................................................

	public void setLayerIndex (int _layerindex)
	{
		if (_layerindex != layerindex && _layerindex < framemap.getLayersCount())
		{
			layerindex = _layerindex;

			Map.Item item = framemap.getItem(layerindex, imageindex);

			if (item == null)
				setSize(0, 0);
			else
				setSize(item.width, item.height);
		}
	}

	//.............................................................................................

	public int getImagesCount (int _layerindex)
	{
		return framemap.getItemsCount(_layerindex);
	}

	//.............................................................................................

	public int getImageIndex ()
	{
		return imageindex;
	}

	//.............................................................................................

	public void setImageIndex (int _imageindex)
	{
		if (_imageindex != imageindex && _imageindex < framemap.getItemsCount(layerindex))
		{
			imageindex = _imageindex;

			Map.Item item = framemap.getItem(layerindex, imageindex);

			if (item == null)
				setSize(0, 0);
			else
				setSize(item.width, item.height);
		}
	}

	//.............................................................................................

	public void animateUpdate ()
	{
		if (isAnimate() && frameimage != null)
		{
			int itemcount = framemap.getItemsCount(layerindex);

			if (itemcount > 1 && updatetime < System.currentTimeMillis())
			{
				if ((imageindex + 1) >= itemcount)
				{
					if (useloop)
						imageindex = 0;
				}
				else imageindex++;

				Map.Item item = framemap.getItem(layerindex, imageindex);

				if (item == null)
					setSize(0, 0);
				else
					setSize(item.width, item.height);

				updatetime = System.currentTimeMillis() + deltatime;
			}
		}
	}

	//.............................................................................................

	public void animateNext ()
	{
		if (isAnimate() && frameimage != null)
		{
			int currindex = imageindex;
			int itemcount = framemap.getItemsCount(layerindex);

			if (itemcount > 1)
			{
				if ((imageindex + 1) >= itemcount)
				{
					if (useloop)					
						imageindex = 0;
				}
				else imageindex++;

				Map.Item item = framemap.getItem(layerindex, imageindex);

				if (item == null)
					setSize(0, 0);
				else
					setSize(item.width, item.height);

				if (currindex != imageindex)
					updatetime = System.currentTimeMillis() + deltatime;
			}
		}
	}

	//.............................................................................................

	public void stateUpdate ()
	{		
	}

	//.............................................................................................
}

//*************************************************************************************************