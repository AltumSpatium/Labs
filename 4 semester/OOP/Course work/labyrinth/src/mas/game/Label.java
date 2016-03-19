//*************************************************************************************************

package mas.game;

//.................................................................................................

import java.awt.Graphics;
import java.awt.Image;
import mas.game.Map.Item;

//*************************************************************************************************

public class Label extends Primitive
{
	//.............................................................................................

	private Image fontimage = null;
	private Map fontmap = null;
	private int fontwidth = 0;
	private int fontheight = 0;

	//.............................................................................................

	public Label (int _type, String _id, String _resourcelink, String _text)
	{
		super(_type, _id);
		setText(_text);
		setFont(_resourcelink);
	}

	//.............................................................................................

	public Label (int _type, String _id, String _resourcelink, int _left, int _top, String _text)
	{
		super(_type, _id, _left, _top);
		setText(_text);
		setFont(_resourcelink);
	}

	public boolean setFont (String _resourcelink)
	{
		fontimage = null;
		fontmap = new Map();
		fontwidth = 0;
		fontheight = 0;

		if (!fontmap.loadItems(_resourcelink) ||
			(fontimage = Utils.loadImage(fontmap.getLink())) == null)
			fontmap = null;
		else
		{
			for (Item item : fontmap.getItems())
			{
				int currwidth = (item.right - item.left) + 1;
				int currheight = (item.bottom - item.top) + 1;

				if (currwidth > fontwidth)
					fontwidth = currwidth;

				if (currheight > fontheight)
					fontheight = currheight;
			}
		}

		setText(getText());

		return (fontmap != null ? true : false);
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
	}

	//.............................................................................................

	public boolean isAnimate ()
	{
		return false;
	}

	//.............................................................................................

	public boolean isPlaying ()
	{
		return false;
	}

	//.............................................................................................

	public boolean canAnimate ()
	{
		return false;
	}

	//.............................................................................................

	public void drawImage (Graphics _graphics)
	{
		if (getText() != null && !getText().isEmpty() && isVisible() && fontmap != null)
		{
			String text = getText();
			int currleft = getLeft(), currtop = getTop();
			Item item;

			for (int index = 0, length = text.length(); index < length; index++)
			{
				int charid = text.charAt(index);

				if (charid == '\n')
				{
					currtop += fontheight;
					currleft = getLeft();
				}
				else if ((item = fontmap.findItem(charid)) != null)
				{
					int currwidth = (item.right - item.left) + 1;
					int currheight = (item.bottom - item.top) + 1;

					_graphics.drawImage(fontimage, currleft, currtop, currleft + currwidth,
										currtop + currheight, item.left, item.top, item.right + 1,
										item.bottom + 1, null);

					currleft += (currwidth + 1);
				}
				else currleft += (fontwidth + 1);
			}
		}
	}

	//.............................................................................................

	public int getLayersCount ()
	{
		return 1;
	}

	//.............................................................................................

	public int getLayerIndex ()
	{
		return 1;
	}

	//.............................................................................................

	public void setLayerIndex (int _layerindex)
	{		
	}

	//.............................................................................................

	public int getImagesCount (int _layerindex)
	{
		if (_layerindex == 0)
			return 1;

		return 0;
	}

	//.............................................................................................

	public int getImageIndex ()
	{
		return 1;
	}

	//.............................................................................................

	public void setImageIndex (int _imageindex)
	{		
	}

	//.............................................................................................

	public void animateUpdate ()
	{		
	}

	//.............................................................................................

	public void animateNext ()
	{		
	}

	//.............................................................................................

	public void stateUpdate ()
	{		
	}

	//.............................................................................................

	public void setText (String _text)
	{
		setSize(0, 0);

		String text = (_text != null ? _text : "");
		super.setText(text);

		if (!text.isEmpty() && fontmap != null)
		{
			int textwidth = 0, textheight = fontheight;
			Item item;

			for (int index = 0, currwidth = 0, length = text.length(); index < length; index++)
			{
				int charid = text.charAt(index);

				if (charid == '\n')
				{
					textheight += fontheight;
					currwidth = 0;
				}
				else if ((item = fontmap.findItem(charid)) != null)
					currwidth += ((item.right - item.left) + 2);
				else currwidth += fontwidth;

				if (textwidth < currwidth)
					textwidth = currwidth;
			}

			setSize(textwidth, textheight);
		}
	}

	//.............................................................................................
}

//*************************************************************************************************
