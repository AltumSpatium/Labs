//*************************************************************************************************

package mas.game;

//.................................................................................................

import java.awt.Image;
import java.util.HashMap;
import java.util.Vector;
import java.util.Comparator;

//*************************************************************************************************

public class Map
{
	//.............................................................................................

	public static class Item
	{
		//.........................................................................................

		public int id;
		public int left;
		public int top;
		public int right;
		public int bottom;
		public int width;
		public int height;
		public int layer;

		//.........................................................................................
	}

	//.............................................................................................

	private class CacheInfo
	{
		//.........................................................................................

		public Vector<Item> items = null;
		public Vector<Vector<Item>> layers = null;
		public Type type = Type.UNKNOWN;
		public String link = null;

		//.........................................................................................
	}

	//.............................................................................................

	enum Type { UNKNOWN, LINEAR, LAYERED };

	//.............................................................................................

	private static HashMap<String, CacheInfo> mapcache = new HashMap<String, CacheInfo>();
	private Vector<Item> items = null;
	private Vector<Vector<Item>> layers = null;
	private Type type = Type.UNKNOWN;
	private String link = null;

	//.............................................................................................

	public Map ()
	{
	}

	//.............................................................................................

	public Map (String _resourcename)
	{

	}

	//.............................................................................................

	public Map (Type _type, String _resourcename, int _colcount, int _rowcount)
	{

	}

	//.............................................................................................

	public Type getType ()
	{
		return type;
	}

	//.............................................................................................

	public String getLink ()
	{
		return (link != null ? link : "");
	}

	//.............................................................................................

	public int getItemsCount ()
	{
		if (items != null)
			return items.size();

		return 0;
	}

	//.............................................................................................

	public int getItemsCount (int _layerindex)
	{
		if (layers != null)
		{
			if (_layerindex >= 0 && _layerindex < layers.size())
				return layers.get(_layerindex).size();
		}
		else if (items != null && _layerindex == 0)
			return items.size();

		return 0;
	}

	//.............................................................................................

	public int getLayersCount ()
	{
		if (layers != null)
			return layers.size();
		else if (items != null)
			return 1;

		return 0;
	}

	//.............................................................................................

	public Item[] getItems ()
	{
		if (items != null)
		{
			Item[] newitems = new Item[items.size()];
			items.toArray(newitems);

			return newitems;
		}

		return null;
	}

	//.............................................................................................

	public Item[] getItems (_layerindex)
	{
		if (layers != null)
		{
			if (_layerindex >= 0 && _layerindex < layers.size())
			{
				Item[] newitems = new Item[layers.get(_layerindex).size()];
				layers.get(_layerindex).toArray(newitems);

				return newitems;
			}
		}
		else if (items != null && _layerindex == 0)
		{
			Item[] newitems = new Item[items.size()];
			items.toArray(newitems);

			return newitems;
		}

		return null;
	}

	//.............................................................................................

	private Item parseItem (String _lineparams)
	{
		String[] parts = _lineparams.split(",");

		if (parts.length >= 5)
		{
			Item item = new Item();

			item.id = Integer.parseInt(parts[0].trim());
			item.left = Integer.parseInt(parts[1].trim());
			item.top = Integer.parseInt(parts[2].trim());
			item.right = Integer.parseInt(parts[3].trim());
			item.bottom = Integer.parseInt(parts[4].trim());
			item.width = (item.right - item.left) + 1;
			item.height = (item.bottom - item.top) + 1;

			if (parts.length >= 6)
			{
				if ((item.layer = Integer.parseInt(parts[4].trim())) < 0)
					return null;
			}
			else item.layer = 0;

			return item;
		}

		return null;
	}

	//.............................................................................................

	public boolean loadItems (String _resourcelink)
	{

	}

	//.............................................................................................

	public boolean createItems (Type _type, String _resourcelink, int _colcount, int _rowcount)
	{

	}

	//.............................................................................................

	public boolean createItems (Type _type, String _resourcelink,
								int _colcount, int _rowcount, Area _area)
	{

	}

	//.............................................................................................

	public Item findItem (int _id)
	{
		if (items != null)
		{
			if (items.size() > 1)
			{
				Item item;
				int centerindex, leftindex = 0, rightindex = items.size() - 1;

				if (_id < items.get(leftindex).id || _id > items.get(rightindex).id)
					return null;

				while (leftindex <= rightindex)
				{
					centerindex = leftindex + ((rightindex - leftindex) >> 1);
					item = items.get(centerindex);

					if (_id == item.id)
						return item;
					else if (_id < item.id)
						rightindex = (centerindex - 1);
					else leftindex = (centerindex + 1);
				}
			}
			else if (_id == items.get(0).id)
				return items.get(0);
		}

		return null;
	}

	//.............................................................................................

	public Item getItem (int _itemindex)
	{
		if (items != null && _itemindex < items.size())
			return items.get(_itemindex);

		return null;
	}

	//.............................................................................................

	public Item getItem (int _layerindex, int _itemindex)
	{
		if (layers != null)
		{
			if ((_layerindex >= 0 && _layerindex < layers.size()) &&
				(_itemindex >= 0 && _itemindex < items.size()))
				return layers.get(_layerindex).get(_itemindex);
		}
		else if (items != null && _layerindex == 0)
		{
			if (_itemindex >= 0 && _itemindex < items.size())
				return items.get(_itemindex);
		}

		return null;
	}

	//.............................................................................................
}

//*************************************************************************************************
