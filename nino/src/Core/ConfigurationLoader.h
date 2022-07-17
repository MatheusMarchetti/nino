#pragma once

#include "INIReader.h"

namespace nino
{
	class ConfigurationLoader
	{
	public:
		ConfigurationLoader(ApplicationDescriptor& descriptor, const std::string& fileName)
		{
			INIReader reader(fileName);

			if (reader.ParseError() != 0)
				throw std::exception("Error loading config.ini file");

			descriptor.ApplicationName = reader.Get("Application", "Name", "nino engine Application");
			descriptor.Width = reader.GetInteger("Application", "Width", 800);
			descriptor.Height = reader.GetInteger("Application", "Height", 600);
			descriptor.Maximized = reader.GetBoolean("Application", "Maximized", false);
			descriptor.vSync = reader.GetBoolean("Application", "vSync", true);
		}
	};
}