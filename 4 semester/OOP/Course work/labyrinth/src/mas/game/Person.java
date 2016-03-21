//*************************************************************************************************

package mas.game;

//*************************************************************************************************

public class Person extends Sprite
{
	//.............................................................................................

	final public static int STATE_WAIT = 0;
	final public static int STATE_UP = 1;
	final public static int STATE_RIGHT = 2;
	final public static int STATE_LEFT = 3;
	final public static int STATE_DOWN = 4;
	final public static int STATE_DIE = 5;
	final public static int STATE_FIRE = 6;
	final public static int STATE_PORT = 7;
	final public static int STATE_WIN = 8;

	private boolean ispause = false;
	private boolean isactive = true;
	private int row = 0;
	private int col = 0;

	//.............................................................................................

	public Person (int _type, String _id, String _resourcelink)
	{
		super(_type, _id, _resourcelink);
	}

	//.............................................................................................

	public Person (int _type, String _id, String _resourcelink, int _row, int _col)
	{
		super(_type, _id, _resourcelink);
		setRowCol(_row, _col, true);
	}

	//.............................................................................................

	public void setRowCol (int _row, int _col, boolean _withposition)
	{
		row = _row;
		col = _col;

		if (_withposition)
			setPos(col << 5, row << 5); // !!
	}

	//.............................................................................................

	public void moveRowCol (int _rowoffset, int _coloffset, boolean _withposition)
	{
		row += _rowoffset;
		col += _coloffset;

		if (_withposition)
			setPos(row << 5, col << 5);
	}

	//.............................................................................................

	public int getRow ()
	{
		return row;
	}

	//.............................................................................................

	public int getCol ()
	{
		return col;
	}

	//.............................................................................................

	public void setPause (boolean _ispause)
	{
		ispause = _ispause;
	}

	//.............................................................................................

	public boolean isPause ()
	{
		return ispause;
	}

	//.............................................................................................

	public void setActive (boolean _isactive)
	{
		isactive = _isactive;
	}

	//.............................................................................................

	public boolean isActive ()
	{
		return isactive;
	}

	//.............................................................................................

	public boolean isRowCol (Person _person)
	{
		return (_person != null && _person.row == row && _person.col == col ? true : false);
	}

	//.............................................................................................

	public void clearState ()
	{
		setState(STATE_WAIT);
		isactive = true;
	}

	//.............................................................................................

	public void copyState (Person _person)
	{
		setState(_person.getState());
		isactive = _person.isactive;
		row = _person.row;
		col = _person.col;
	}

	//.............................................................................................

	public int getState ()
	{
		return getLayerIndex();
	}

	//.............................................................................................

	public void nextStep (int _offset)
	{
		switch(getState())
		{
			case STATE_UP:
				addTop(-_offset);
				break;
			case STATE_DOWN:
				addTop(_offset);
				break;
			case STATE_LEFT:
				addLeft(-_offset);
				break;
			case STATE_RIGHT:
				addLeft(_offset);
				break;
		}
	}

	//.............................................................................................

	public void setState (int _state)
	{
		isactive = (_state == STATE_WAIT || _state == STATE_UP || _state == STATE_DOWN ||
			_state == STATE_LEFT || _state == STATE_RIGHT || _state == STATE_PORT ? true : false);

		if (getLayerIndex() != _state)
			setImageIndex(0);

		setLayerIndex((int) _state);
	}

	//.............................................................................................

	public void animateUpdate ()
	{
		if ((getLayerIndex() == STATE_DIE || getLayerIndex() == STATE_FIRE) &&
			getImageIndex() == (getImagesCount() - 1))
			return;

		super.animateUpdate();
	}

	//.............................................................................................

	public void animateNext ()
	{
		if ((getLayerIndex() == STATE_DIE || getLayerIndex() == STATE_FIRE) &&
			getImageIndex() == (getImagesCount() - 1))
			return;

		super.animateNext();
	}

	//.............................................................................................
}

//*************************************************************************************************
