
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"

#include "Defs.h"
#include "Log.h"

#include <math.h>

Map::Map(bool startEnabled) : Module(startEnabled), mapLoaded(false)
{
	name.Create("map");
}

// Destructor
Map::~Map()
{}

// Ask for the value of a custom property
int Properties::GetProperty(const char* value, int defaultValue) const
{
	ListItem<Property*>* property = list.start;
	while (property != NULL)
	{
		if (property->data->name == value)
		{
			return property->data->value;
		}

		property = property->next;
	}

	return defaultValue;
}

// Set the value of a custom property
void Properties::SetProperty(const char* value, int num)
{
	ListItem<Property*>* property = list.start;
	while (property != NULL)
	{
		if (property->data->name == value)
		{
			property->data->value = num;
			break;
		}

		property = property->next;
	}
}

// Change a layer property value
void Map::ChangeLayerProperty(SString layerName, SString propertyName, int value)
{
	ListItem<MapLayer*>* layer = data.layers.start;
	while (layer != NULL)
	{
		/*if (layer->data->name == layerName)
		{
			if (layer->data->properties.GetProperty(propertyName.GetString()) == 0)
			{
				layer->data->properties.SetProperty(propertyName.GetString(), 1);
			}
			else if (layer->data->properties.GetProperty(propertyName.GetString()) == 1)
			{
				layer->data->properties.SetProperty(propertyName.GetString(), 0);
			}
		}*/

		if (layer->data->name == layerName)
		{
			layer->data->properties.SetProperty(propertyName.GetString(), value);
		}

		layer = layer->next;
	}
}

// Called before render is available
bool Map::Awake(pugi::xml_node& config)
{
    LOG("Loading Map Parser");
	bool ret = true;

    folder.Create(config.child("folder").child_value());

    return ret;
}

// Draw the map (all requried layers)
void Map::Draw()
{
	if (mapLoaded == false) return;

	// Prepare the loop to draw all tilesets + DrawTexture()
	/*MapLayer* layer = data.layers.start->data;*/

	// Make sure we draw all the layers and not just the first one
	ListItem<MapLayer*>* layer = data.layers.start;
	while (layer != NULL)
	{
		if (layer->data->properties.GetProperty("Draw") != 0)
		{
			for (int y = 0; y < data.height; ++y)
			{
				for (int x = 0; x < data.width; ++x)
				{
					int tileId = layer->data->Get(x, y);
					if (tileId > 0)
					{
						// Complete the draw function
						TileSet* tileset;
						tileset = GetTilesetFromTileId(tileId);
						SDL_Rect tileRect = tileset->GetTileRect(tileId);
						iPoint pos = MapToWorld(x, y);
						app->render->DrawTexture(tileset->texture, pos.x, pos.y, &tileRect);
					}
				}
			}
		}

		layer = layer->next;
	}
}

// Create a method that translates x,y coordinates from map positions to world positions
iPoint Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	// Add isometric map to world coordinates
	if (data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x * data.tileWidth;
		ret.y = y * data.tileHeight;
	}
	else if (data.type == MAPTYPE_ISOMETRIC)
	{
		ret.x = (x - y) * (data.tileWidth / 2);
		ret.y = (x + y) * (data.tileHeight / 2);
	}
	else
	{
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
	}

	return ret;
}

// Add orthographic world to map coordinates
iPoint Map::WorldToMap(int x, int y) const
{
	iPoint ret(0, 0);

	// Add the case for isometric maps to WorldToMap
	if (data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x / data.tileWidth;
		ret.y = y / data.tileHeight;
	}
	else if (data.type == MAPTYPE_ISOMETRIC)
	{

		float half_width = data.tileWidth * 0.5f;
		float half_height = data.tileHeight * 0.5f;
		ret.x = int((x / half_width + y / half_height) / 2);
		ret.y = int((y / half_height - (x / half_width)) / 2);
	}
	else
	{
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
	}

	return ret;
}

// Pick the right Tileset based on a tile id
TileSet* Map::GetTilesetFromTileId(int id) const
{
	ListItem<TileSet*>* item = data.tilesets.start;
	TileSet* set = item->data;

	while (item != NULL)
	{
		if (item->next == nullptr)
		{
			set = item->data;
			break;
		}
		else if (id < item->next->data->firstgid)
		{
			set = item->data;
			break;
		}

		item = item->next;
	}

	return set;
}

// Get relative Tile rectangle
SDL_Rect TileSet::GetTileRect(int id) const
{
	SDL_Rect rect = { 0 };

	// Get relative Tile rectangle
	int relativeId = id - firstgid;
	rect.w = tileWidth;
	rect.h = tileHeight;
	rect.x = margin + ((rect.w + spacing) * (relativeId % numTilesWidth));
	rect.y = margin + ((rect.h + spacing) * (relativeId / numTilesWidth));
	
	return rect;
}

// Called before quitting
bool Map::CleanUp()
{
    LOG("Unloading map");

    // Make sure you clean up any memory allocated from tilesets/map
    // Remove all tilesets
	ListItem<TileSet*>* item;
	item = data.tilesets.start;

	while (item != NULL)
	{
		app->tex->UnLoad(item->data->texture);
		RELEASE(item->data);
		
		item = item->next;
	}
	data.tilesets.clear();

	// Clean up all layer data
	// Remove all layers
	ListItem<MapLayer*>* item2;
	item2 = data.layers.start;

	while (item2 != NULL)
	{
		RELEASE(item2->data);
		item2 = item2->next;
	}
	data.layers.clear();

	// Clean up the pugui tree
	mapFile.reset();

    return true;
}

// Load new map
bool Map::Load(const char* filename)
{
    bool ret = true;
    SString tmp("%s%s", folder.GetString(), filename);

    pugi::xml_parse_result result = mapFile.load_file(tmp.GetString());

	if (result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", filename, result.description());
		ret = false;
	}

	// Load general info
	if (ret == true)
	{
		// Create and call a private function to load and fill all your map data
		ret = LoadMap();
	}

    // Create and call a private function to load a tileset
    // remember to support more any number of tilesets!
	pugi::xml_node tileset;
	for (tileset = mapFile.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if (ret == true) ret = LoadTilesetDetails(tileset, set);

		if (ret == true) ret = LoadTilesetImage(tileset, set);

		data.tilesets.add(set);
	}

	// Iterate all layers and load each of them
	// Load layer info
	pugi::xml_node layer;
	for (layer = mapFile.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		MapLayer* lay = new MapLayer();

		ret = LoadLayer(layer, lay);

		ret = LoadProperties(layer, lay->properties);

		if (ret == true) data.layers.add(lay);
	}
    
	if (ret == true)
	{
		// LOG all the data loaded iterate all tilesets and LOG everything
		LOG("Successfully parsed map XML file: %s", filename);
		LOG("width: %d", data.width);
		LOG("height: %d", data.height);
		LOG("tile width: %d", data.tileWidth);
		LOG("tile height: %d", data.tileHeight);

		if (data.type == MAPTYPE_ORTHOGONAL)
		{
			LOG("orientation: orthogonal");
		}
		else if (data.type == MAPTYPE_ISOMETRIC)
		{
			LOG("orientation: isometric");
		}

		for (int i = 0; i < data.tilesets.count(); i++)
		{
			LOG("Tileset %d", i + 1);
			LOG("name: %s", data.tilesets[i]->name.GetString());
			LOG("first gid: %d", data.tilesets[i]->firstgid);
			LOG("margin: %d", data.tilesets[i]->margin);
			LOG("spacing: %d", data.tilesets[i]->spacing);
			LOG("tile width: %d", data.tilesets[i]->tileWidth);
			LOG("tile height: %d", data.tilesets[i]->tileHeight);

			LOG("width: %d", data.tilesets[i]->texWidth);
			LOG("height: %d", data.tilesets[i]->texHeight);
		}

		// LOG the info for each loaded layer
		for (int i = 0; i < data.layers.count(); i++)
		{
			LOG("Layer %d", i + 1);
			LOG("name: %s", data.layers[i]->name.GetString());
			LOG("width: %d", data.layers[i]->width);
			LOG("height: %d", data.layers[i]->height);
		}
	}

    mapLoaded = ret;

    return ret;
}

// Load map general properties
bool Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = mapFile.child("map");

	if (map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		// Load map general properties
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tileWidth = map.attribute("tilewidth").as_int();
		data.tileHeight = map.attribute("tileheight").as_int();
		SString type = map.attribute("orientation").as_string();
		if (type == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if (type == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
	}

	return ret;
}

// Load Tileset attributes
bool Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;

	// Load Tileset attributes
	set->name = tileset_node.attribute("name").as_string();
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	set->tileWidth = tileset_node.attribute("tilewidth").as_int();
	set->tileHeight = tileset_node.attribute("tileheight").as_int();

	return ret;
}

// Load Tileset image
bool Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if (image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		// Load Tileset image
		SString tmp("%s%s", folder.GetString(), image.attribute("source").as_string());
		set->texture = app->tex->Load(tmp.GetString());
		set->texWidth = image.attribute("width").as_int();
		set->texHeight = image.attribute("height").as_int();

		set->numTilesWidth = set->texWidth / set->tileWidth;
		set->numTilesHeight = set->texHeight / set->tileHeight;
		set->offsetX = 0;
		set->offsetY = 0;
	}

	return ret;
}

// Create the definition for a function that loads a single layer
bool Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;
	
	// Load a single layer
	layer->name = node.attribute("name").as_string();
	layer->width = node.attribute("width").as_int();
	layer->height = node.attribute("height").as_int();

	size_t size = layer->width * layer->height * sizeof(uint);
	layer->data = new uint[size];
	memset(layer->data, 0, size);

	int i = 0;
	for (pugi::xml_node tile = node.child("data").child("tile"); tile; tile = tile.next_sibling("tile"))
	{
		layer->data[i] = tile.attribute("gid").as_int();
		i++;
	}

	return ret;
}

// Load a group of properties from a node and fill a list with it
bool Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
	bool ret = false;

	pugi::xml_node propertiesNode = node.child("properties");
	if (propertiesNode != NULL)
	{
		ret = true;

		for (propertiesNode = propertiesNode.child("property"); propertiesNode; propertiesNode = propertiesNode.next_sibling("property"))
		{
			Properties::Property* prop = new Properties::Property();
			prop->name = propertiesNode.attribute("name").as_string();
			prop->value = propertiesNode.attribute("value").as_int();
			properties.list.add(prop);
		}
	}

	return ret;
}