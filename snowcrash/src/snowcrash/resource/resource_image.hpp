#ifndef SC_RESOURCE_IMAGE_HPP
#define SC_RESOURCE_IMAGE_HPP

#include "snowcrash/container/arraylist.hpp"
#include "snowcrash/container/string.hpp"
#include "snowcrash/math/rect.hpp"
#include "snowcrash/math/vector.hpp"
#include "snowcrash/resource/resource.hpp"
#include <snowcrash/core/core.hpp>

namespace SC {

/*
 * we are reading the metadata as if its like
 * the aseprite thingy
 */
class ResourceImage : public Resource {
public:

	/*
	 * these are the png formats we might
	 * find from aseprite
	 */
	enum ColorFormat : u8 {
		ColorFormat_R8 = 1,
		ColorFormat_R8A8 = 2,
		ColorFormat_R8G8B8 = 3,
		ColorFormat_R8G8B8A8 = 4
	};

	/*
	 * these are the channels requested and returned by
	 * stbi image
	 */
	enum ColorChannels : int {
		ColorChannels_Grey = 1,
		ColorChannels_GreyAlpha = 2,
		ColorChannels_RGB = 3,
		ColorChannels_RGBA = 4
	};

	/*
	 * stores an aseprite frame, which can either be
	 * a frame of animation or a frame of a tilemap
	 */
	struct Frame {
		Rect bounds;
		bool rotated;
		bool trimmed;
		Rect sourceSize;
		u32 duration; // used for animation, defines the duration in ms
	};

	struct Meta {
		String image;
		ColorFormat format;
		Vector2Int size;
		ArrayList<Frame> frames;
	};

public:
	ResourceImage(Allocator *allocator, Engine *engine,
			 String path, String name, UUID groupID=UUID(0));

	~ResourceImage();

public:
	void load(ResourceLoadError *error) override;
	void load_metadata(ResourceLoadError *error) override;
	void save() override;
	void unload() override;

	const Meta &get_meta_data() const { return m_metadata; }

private:
	u8 *m_image;
	ColorChannels m_channels;
	Meta m_metadata;
};

}

#endif
