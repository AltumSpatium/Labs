//*************************************************************************************************

package mas.game;

//.................................................................................................

import java.awt.Graphics;

//*************************************************************************************************

public class Status extends Sprite
{
	//.............................................................................................

	private Label label = new Label(Builder.typeLabel, "label", "@fonts.map:smallfont", "");

	//.............................................................................................

	public Status (int _type, String _id, String _resourcelink)
	{
		super(_type, _id, _resourcelink);
	}

	//.............................................................................................

	public Status (int _type, String _id, String _resourcelink, int _left, int _top)
	{
		super(_type, _id, _resourcelink, _left, _top);
	}

	//.............................................................................................

	public void setText (String _text)
	{
		super.setText(_text);
		label.setText(_text);
	}

	//.............................................................................................

	public void drawImage (Graphics _graphics)
	{
		super.drawImage(_graphics);

		if (isVisible() && label != null)
		{
			label.setPos(getLeft() + 33, getTop() + 7);
			label.drawImage(_graphics);
		}
	}

	//.............................................................................................
}

//*************************************************************************************************
