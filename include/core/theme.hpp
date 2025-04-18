#ifndef GWAIN_THEME_HPP
#define GWAIN_THEME_HPP

#include "texture.hpp"

namespace gwain
{
	class theme
	{
	public:
		virtual std::shared_ptr<texture> refresh(control* c, control* p = nullptr) = 0;
	};
}

#endif // GWAIN_THEME_HPP