#ifndef GWAIN_CORE_INTERROGATOR_HPP
#define GWAIN_CORE_INTERROGATOR_HPP

#include "instance.hpp"

namespace gwain
{
	namespace core
	{
		class default_interrogation : public instance_interface::interrogation_adaptor
		{
			control* interrogate_scene(const coord& display_coordinate, const std::list<const control*>& view_controls)
			{
				for (auto sc = view_controls.rbegin(); sc != view_controls.rend(); ++sc)
				{
					if (dim_contained((*sc)->control_region(), (*sc)->control_mask(), display_coordinate))
						return const_cast<control*>(*sc);
				}
				return nullptr;
			}

		public:
			std::pair<view*, control*> interrogate(const coord& display_coordinate, const std::vector<view*>& views)
			{
				// first check the back view (top view)...
				for (auto vItr = views.rbegin(); vItr != views.rend(); ++vItr)
				{

					/*for (auto sItr = (*vItr)->view().begin(); sItr != (*vItr)->view_scenes().end(); ++sItr)
					{
						if (auto ctl = interrogate_scene(display_coordinate, instance().scene_controls(*sItr)))
							return { *vItr, ctl };
					}*/
				}
				return { nullptr, nullptr };
			}
			void interrogation_adaptor_free() {}
		};
	}
}

#endif