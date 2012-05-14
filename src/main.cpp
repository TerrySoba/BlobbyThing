/*
 * main.cpp
 *
 *  Created on: 02.02.2012
 *      Author: yoshi252
 */

#include "BlobbyThingGame.h"

#include "common.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

int main(int argc, char* argv[]) {

//	using boost::property_tree::ptree;
//	ptree pt;
//
//	pt.put("BlobbyThing.models.shadow", "shadow.obj");
//	pt.put("BlobbyThing.models.beach", "beach.obj");
//	pt.put("BlobbyThing.settings.resolution_x", "1280");
//	pt.put("BlobbyThing.settings.resolution_y", "720");
//
//	boost::property_tree::xml_writer_settings<char> settings(' ', 2);
//	write_xml("test.xml", pt, std::locale(), settings);
//
//	std::cout << pt.get("BlobbyThing.models.shadow", "Hallo Welt!2345");
//
//	return 0;

	BlobbyThingGame game(1280, 720, 24);
	int ret = game.run();
	return ret;
}
