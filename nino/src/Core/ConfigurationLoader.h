#pragma once

namespace nino
{
	class ConfigurationLoader
	{
	public:
		~ConfigurationLoader() = default;
		static void LoadConfigurationFile(ApplicationDescriptor& descriptor, const std::string& fileName);

	private:

	};
}