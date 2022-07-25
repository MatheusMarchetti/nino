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
			descriptor.Window.Width = reader.GetInteger("Application", "Width", 800);
			descriptor.Window.Height = reader.GetInteger("Application", "Height", 600);
			descriptor.Window.Maximized = reader.GetBoolean("Application", "Maximized", false);
			descriptor.Graphics.TextureQuality = reader.GetFloat("Graphics", "TextureQuality", 1.0f);
			descriptor.Graphics.vSync = reader.GetBoolean("Graphics", "vSync", true);

			descriptor.Window.WindowName = descriptor.ApplicationName;
		}
	};
}