#ifndef GWAIN_TEXTURE_HPP
#define GWAIN_TEXTURE_HPP

#include "types.hpp"
#include "image.hpp"
#include "event.hpp"

#include <memory>

namespace gwain
{
	class texture
	{
	public:
		texture(const texture_id& name, const image& img)
			: texture_name_(name), texture_image_(std::make_shared<image>(img))
		{
			eos_dispatch::texture_registered(this);
		}

		~texture()
		{
			eos_dispatch::texture_unregistered(this);
		}

		void texture_image(const image& img)
		{
			texture_image_ = std::make_shared<image>(img);
			eos_dispatch::texture_image(this);
		}

		const image& texture_image() const { return *texture_image_; }

		const texture_id& texture_name() const { return texture_name_; }

	private:
		texture_id texture_name_;
		std::shared_ptr<image> texture_image_;
	};

}

#endif // GWAIN_TEXTURE_HPP