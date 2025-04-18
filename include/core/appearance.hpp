#ifndef GWAIN_CORE_APPEARANCE_HPP
#define GWAIN_CORE_APPEARANCE_HPP

#include <memory>

namespace gwain
{
	class texture;
	class theme;

	struct appearance
	{
		appearance() {}

		appearance(const std::shared_ptr<theme>& theme)
			: theme_(theme)
		{
		}

		appearance(const std::shared_ptr<texture>& texture)
			: texture_(texture)
		{
		}

		std::shared_ptr<texture> texture_ = nullptr;		// if texture here use that...
		std::shared_ptr<theme> theme_ = nullptr;			// if theme here, use that...
	};
}

#endif // GWAIN_CORE_APPEARANCE_HPP