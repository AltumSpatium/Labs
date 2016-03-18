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
		loadItems(_resourcename);
	}

	//.............................................................................................

	public Map (Type _type, String _resourcename, int _colcount, int _rowcount)
	{
		createItems(_type, _resourcename, _colcount, _rowcount);
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

	public Item[] getItems (int _layerindex)
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
				if ((item.layer = Integer.parseInt(parts[5].trim())) < 0)
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
		type = Type.UNKNOWN;
		link = null;
		items = null;
		layers = null;

		Utils.RscInfo rscinfo = Utils.parseRscLink(_resourcelink);

		if (rscinfo == null)
			return false;

		CacheInfo cacheinfo = mapcache.get(rscinfo.key);

		if (cacheinfo != null)
		{
			type = cacheinfo.type;
			link = cacheinfo.link;
			items = cacheinfo.items;
			layers = cacheinfo.layers;

			return true;
		}

		Vector<String> resdata = Utils.loadConfig(rscinfo);

		if (resdata != null)
		{
			try
			{
				Vector<Item> newitems = new Vector<Item>();
				Vector<Integer> layersid = new Vector<Integer>();

				boolean autosize = false;
				int autocols = 0, autorows = 0, autoleft = 0,
				 	autotop = 0, autowidth = 0, autoheight = 0;
				int pos;

				for (String line : resdata)
				{
					if ((pos = line.indexOf('=')) <= 0)
						return false;

					String key = line.substring(0, pos).trim().toLowerCase();
					String value = line.substring(pos + 1).trim();

					if (type == Type.UNKNOWN)
					{
						if (!key.equals("map.type"))
							return false;

						value = value.toLowerCase();

						if ((pos = value.indexOf(':')) >= 0)
						{
							String modify = value.substring(pos + 1).trim();
							value = value.substring(0, pos).trim();

							if (modify.equals("auto"))
								autosize = true;
							else return false;
						}

						if (value.equals("linear"))
							type = Type.LINEAR;
						else if (value.equals("layered"))
							type = Type.LAYERED;
						else return false;
					}
					else if (link == null)
					{
						if (key.equals("map.link"))
							link = value;
						else
						{
							type = Type.UNKNOWN;

							return false;
						}
					}
					else if (autosize)
					{
						if (key.equals("map.cols"))
							autocols = Integer.parseInt(value);
						else if (key.equals("map.rows"))
							autorows = Integer.parseInt(value);
						else if (key.equals("map.left"))
							autoleft = Integer.parseInt(value);
						else if (key.equals("map.top"))
							autotop = Integer.parseInt(value);
						else if (key.equals("map.width"))
							autowidth = Integer.parseInt(value);
						else if (key.equals("map.height"))
							autoheight = Integer.parseInt(value);
					}
					else
					{
						Item item = parseItem(key + "," + value);

						if (item == null)
						{
							type = Type.UNKNOWN;
							link = null;

							return false;
						}

						if (!layersid.contains(item.layer))
							layersid.add(item.layer);

						newitems.add(item);
					}
				}

				if (autosize)
				{
					if (autocols <= 0 || autorows <= 0 || autoleft < 0 || autotop < 0 ||
					autowidth < 0 || autoheight < 0)
					{
						type = Type.UNKNOWN;
						link = null;

						return false;
					}

					String currlink = link;

					if (!createItems(type, link, autocols, autorows,
					                 new Area(autoleft, autotop, autowidth, autoheight)))
						return false;

					link = currlink;

					cacheinfo = new CacheInfo();
					cacheinfo.type = type;
					cacheinfo.link = link;
					cacheinfo.items = items;
					cacheinfo.layers = layers;

					mapcache.put(rscinfo.key, cacheinfo);

					return true;
				}

				if (newitems.size() > 0)
				{
					if (type == Type.LAYERED && layersid.size() > 1)
					{
						newitems.sort(new Comparator<Item>()
						{
							public int compare(Item _left, Item _right)
							{
								int result = _left.layer - _right.layer;

								if (result != 0)
									return result;

								return (_left.id - _right.id);
							}
						});

						int currentlayer = -1;
						layers = new Vector<Vector<Item>>();

						for (Item item : newitems)
						{
							if (currentlayer < item.layer)
							{
								currentlayer = item.layer;
								layers.add(new Vector<Item>());
							}

							layers.get(layers.size() - 1).add(item);
						}
					}

					newitems.sort(new Comparator<Item>()
					{
						public int compare(Item _left, Item _right)
						{
							return (_left.id - _right.id);
						}
					});

					items = newitems;
				}

				cacheinfo = new CacheInfo();
				cacheinfo.type = type;
				cacheinfo.link = link;
				cacheinfo.items = items;
				cacheinfo.layers = layers;

				mapcache.put(rscinfo.key, cacheinfo);

				return true;
			}
			catch (Exception exception)
			{
				type = Type.UNKNOWN;
				link = null;
				items = null;
				layers = null;
			}
		}

		return false;
	}

	//.............................................................................................

	public boolean createItems (Type _type, String _resourcelink, int _colcount, int _rowcount)
	{
		return createItems(_type, _resourcelink, _colcount, _rowcount, new Area());
	}

	//.............................................................................................

	public boolean createItems (Type _type, String _resourcelink, int _colcount, int _rowcount, Area _area)
	{
		type = Type.UNKNOWN;
		link = null;
		items = null;
		layers = null;

		if (_type != Type.LINEAR && _type != Type.LAYERED || _colcount <= 0 || _rowcount <= 0)
			return false;

		int left = _area.getLeft(), top = _area.getTop();
		int width = _area.getWidth(), height = _area.getHeight();

		if (left < 0 || top < 0 || width < 0 || height < 0)
			return false;

		Utils.RscInfo rscinfo = Utils.parseRscLink(_resourcelink);

		if (rscinfo == null)
			return false;

		String cachekey = rscinfo.link.toLowerCase() + "?type=" + _type + 
						  ",cols=" + _colcount + ",rows=" + _rowcount;
		if (left > 0)
			cachekey += (",left=" + left);
		if (top > 0)
			cachekey += (",top=" + top);
		if (width > 0)
			cachekey += (",width=" + width);
		if (height > 0)
			cachekey += (",height=" + height);

		CacheInfo cacheinfo = mapcache.get(cachekey);

		if (cacheinfo != null)
		{
			type = cacheinfo.type;
			link = cacheinfo.link;
			items = cacheinfo.items;
			layers = cacheinfo.layers;

			return true;
		}

		Image image = Utils.loadImage(rscinfo.link);

		if (image != null)
		{
			if (width <= 0)
				width = image.getWidth(null);

			if (height <= 0)
				height = image.getHeight(null);

			if ((left + width) > image.getWidth(null) || (top + height) > image.getHeight(null))
				return false;

			width = width / _colcount;
			height = height / _rowcount;

			if (width <= 0 || height <= 0)
				return false;

			items = new Vector<Item>();

			if (_type == Type.LAYERED && _rowcount > 1)
				layers = new Vector<Vector<Item>>();

			for (int rowindex = 0, itemid = 0; rowindex < _rowcount; rowindex++, top += height)
			{
				if (layers != null)
					layers.add(new Vector<Item>());

				for (int colindex = 0, offset = left; colindex < _colcount; colindex++, itemid++, offset += height)
				{
					Item item = new Item();

					item.id = itemid;
					item.left = offset;
					item.top = top;
					item.right = offset + (width - 1);
					item.bottom = top + (height - 1);
					item.width = width;
					item.height = height;

					items.add(item);

					if (layers != null)
						layers.get(rowindex).add(item);
				}
			}

			type = _type;
			link = rscinfo.link;

			cacheinfo = new CacheInfo();
			cacheinfo.type = type;
			cacheinfo.link = link;
			cacheinfo.items = items;
			cacheinfo.layers = layers;

			mapcache.put(cachekey, cacheinfo);

			return true;
		}

		return false;
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
