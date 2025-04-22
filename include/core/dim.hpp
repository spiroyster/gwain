#ifndef GWAIN_CORE_DIM_HPP
#define GWAIN_CORE_DIM_HPP

#include "types.hpp"

namespace gwain
{
	
	// Dimension...
	struct dim
	{
		bool operator==(const dim& rhs) const
		{
			return wh_.x_ == rhs.wh_.x_ && wh_.y_ == rhs.wh_.y_ &&
				xy_.x_ == rhs.xy_.x_ && xy_.y_ == rhs.xy_.y_;
		}

		float area() const
		{
			return static_cast<float>(wh_.x_ * wh_.y_) * ((wh_.x_ < 0 || wh_.y_ < 0) ? -1.0f : 1.0f);
		}

		const dim& centre(const dim& d)
		{
			return { d.xy_.x_ + (d.wh_.x_ / 2), d.xy_.y_ + (d.wh_.y_ / 2) };
		}

		bool contained(const coord& xy)
		{
			return
				(xy.x_ < (a.xy_.x_ + a.wh_.x_)) &&
				(xy.x_ > a.xy_.x_) &&
				(xy.y_ > a.xy_.y_) &&
				(xy.y_ < (a.xy_.y_ + a.wh_.y_));
		}

		static bool dim_contained(const dim& region, const dim& mask, const coord& xy)
		{
			return mask == mask_auto ? dim_contained(region, xy) :
				(xy.x_ < (region.xy_.x_ + region.wh_.x_)) && (xy.x_ < (mask.xy_.x_ + mask.wh_.x_)) &&
				(xy.x_ > region.xy_.x_) && (xy.x_ > mask.xy_.x_) &&
				(xy.y_ > region.xy_.y_) && (xy.y_ > mask.xy_.y_) &&
				(xy.y_ < (region.xy_.y_ + region.wh_.y_)) && (xy.y_ < (mask.xy_.y_ + mask.wh_.y_));
		}

		static dim dim_union(const dim& a, const dim& b)
		{
			return
			{
				(std::min)(a.xy_.x_, b.xy_.x_),
				(std::min)(a.xy_.y_, b.xy_.y_),
				(std::max)(a.xy_.x_ + a.wh_.x_, b.xy_.x_ + b.wh_.x_) - (std::min)(a.xy_.x_, b.xy_.x_),
				(std::max)(a.xy_.y_ + a.wh_.y_, b.xy_.y_ + b.wh_.y_) - (std::min)(a.xy_.y_, b.xy_.y_),
			};
		}

		static dim dim_intersection(const dim& a, const dim& b)
		{

		}

		// centre b in a...
		static coord dim_centred(const dim& child, const dim& parent)
		{
			return { child.xy_.x_ + ((parent.wh_.x_ - child.wh_.x_) / 2), child.xy_.y_ + ((parent.wh_.y_ - child.wh_.y_) / 2) };
		}

		coord wh_;
		coord xy_;
	};

	// masks...
	// auto (uses dim)			-1, -1, -1, -1 
	// custom (user defined)    w, h, x, y
	// hide						0, 0, 0, 0
	constexpr dim mask_auto = { {-1, -1 }, {-1, -1} };
	constexpr dim mask_hide = { { 0, 0 }, {0, 0} };
	constexpr dim region_auto = mask_auto;
}


#endif // GWAIN_CORE_DIM_HPP