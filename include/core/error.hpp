#ifndef GWAIN_CORE_ERROR_HPP
#define GWAIN_CORE_ERROR_HPP

#include "types.hpp"

#define GWAIN_ERROR(err) namespace error { static const exception_id err = #err; }

namespace gwain
{
	class exception
	{
	public:
		exception(const exception_id& id) : id_(id) {}
		exception(const exception_id& id, const std::string& what) : id_(id), what_(what) {}

		std::string what_;
		exception_id id_;
	};

	GWAIN_ERROR(instanceAlreadyInitialised)
	GWAIN_ERROR(instanceNotInitialised)

	GWAIN_ERROR(viewAlreadyRegistered)
	GWAIN_ERROR(controlAlreadyRegistered)
	GWAIN_ERROR(textureAlreadyRegistered)
	GWAIN_ERROR(viewAlreadyHasControl)

	GWAIN_ERROR(viewNotFound)
	GWAIN_ERROR(sceneNotFound)
	GWAIN_ERROR(controlNotFound)
	GWAIN_ERROR(textureNotFound)

	GWAIN_ERROR(imageReadFail)
	GWAIN_ERROR(imageWriteFail)
	GWAIN_ERROR(fontReadFail)
	GWAIN_ERROR(paintBrushInvalid)
}

#endif // GWAIN_CORE_ERROR_HPP