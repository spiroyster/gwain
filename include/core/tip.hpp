#ifndef GWAIN_TIP_HPP
#define GWAIN_TIP_HPP

#include "control.hpp"

namespace gwain
{
	namespace core
	{
		class tip : public control
		{
		public:

			enum alignment
			{
				alignment_above,
				alignment_below,
				alignment_left,
				alignment_right
			};

			//ToolTip(const AxW::Draw::HUD* parent, const std::shared_ptr<AxW::Draw::ImageRGBA32>& image, const Alignment& alignment, int offset, bool followMouse);
			//ToolTip(const AxW::String& name, const std::shared_ptr<AxW::Draw::ImageRGBA32>& image, const Alignment& alignment, int offset, bool followMouse);

			const alignment& tip_alignment() const { return alignment_; }
			bool tip_follow_mouse() const { return followMouse_; }
			int tip_offset() const { return offset_; }

			const control* tip_parent() const { return control_parent(); }
			void tip_parent(control* parent) { control_parent(parent); }

			virtual void tip_position(const coord& mouseXY)
			{
				//int parentHeight = parent_ ? static_cast<int>(parent_->Height()) : 0;
				//int parentWidth = parent_ ? static_cast<int>(parent_->Width()) : 0;
				//AxW::Draw::ViewCoordinate xy = followMouse_ ? mouseXY : (parent_ ? parent_->Position() : AxW::Draw::ViewCoordinate({ 0, 0 }));

				//// Position the tool tip...
				//switch (GetAlignment())
				//{
				//case AlignAbove:
				//{
				//	Position({ xy.x_ + (parentWidth / 2) - (static_cast<int>(Width()) / 2),
				//		xy.y_ + parentHeight + GetOffset() });
				//	break;
				//}
				//case AlignBelow:
				//{
				//	Position({ xy.x_ + (parentWidth / 2) - (static_cast<int>(Width()) / 2),
				//		xy.y_ + (static_cast<int>(Height()) + GetOffset()) });
				//	break;
				//}
				//case AlignLeft:
				//{
				//	Position({ xy.x_ - (static_cast<int>(Width()) + GetOffset()),
				//		(xy.y_ + (parentHeight / 2)) - (static_cast<int>(Height()) / 2) });
				//	break;
				//}
				//case AlignRight:
				//{
				//	Position({ xy.x_ + GetOffset(),
				//		(xy.y_ + (parentHeight / 2)) - (static_cast<int>(Height()) / 2) });
				//	break;
				//}
				//default:
				//	throw AxW::Exception(AxW::Error::ID::ToolTipInvalidAlignment,
				//		{
				//			{ Error::Tag::Name, SceneObjectName() },
				//			{ Error::Tag::Callee, AxW::Str(__FUNCTION__)}
				//		});
			}

		private:
			int offset_ = 0;
			bool followMouse_ = false;
			alignment alignment_ = alignment_above;
		};
	}
	
}

#endif // GWAIN_TIP_HPP