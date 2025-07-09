
#ifndef SC_RESOURCE_HPP
#define SC_RESOURCE_HPP

#include "snowcrash/container/allocator.hpp"
#include "snowcrash/container/pair.hpp"
#include "snowcrash/container/string.hpp"
#include "snowcrash/core/object.hpp"
#include "snowcrash/core/uuid.hpp"
#include "snowcrash/engine/engine.hpp"
#include <snowcrash/core/core.hpp>
#include <nlohmann/json_fwd.hpp>

namespace SC {

/*
 * defines any resource that can be loaded onto or off
 * the computer
 */
class Resource : public Object {
SC_OBJECT(Resource, Object)
public:
	enum ResourceLoadState {
		ResourceLoadState_Unloaded,

		/*
		 * this means that the metadata is loaded but not the
		 * full resource
		 */
		ResourceLoadState_Partialy,
		
		/* the full resource */
		ResourceLoadState_FullyLoaded	
	};

	enum ResourceLoadError {
		ResourceLoadError_None,
		ResourceLoadError_BadPermissions,
		ResourceLoadError_PathDoesNotExist,
		ResourceLoadError_BadFormat,
		ResourceLoadError_ApiError
	};

	constexpr const char *load_error_to_string(ResourceLoadError error) {
		switch (error) {
			case ResourceLoadError_None: return "none";
			case ResourceLoadError_ApiError: return "api error";
			case ResourceLoadError_BadFormat: return "bad format";
			case ResourceLoadError_PathDoesNotExist: return "path does not exist";
			case ResourceLoadError_BadPermissions: return "bad permissions";
		}
	}

	constexpr const char *load_state_to_string(ResourceLoadState state) {
		switch(state) {
			case ResourceLoadState_Unloaded: return "unloaded";
			case ResourceLoadState_FullyLoaded: return "fully loaded";
			case ResourceLoadState_Partialy: return "partialy loaded";
		}
	}
		
public:
	Resource(Allocator *allocator, Engine *engine,
			 String path, String name, UUID groupID=UUID(0));

	~Resource();

public:
	/*
	 * loads a resource from memory from the path defined in 
	 * resource class. Children need to call this to change the resource state
	 * at the end of their load.
	 */
	virtual void load(ResourceLoadError *error);
	
	/*
	 * this partialy loads the resource such that
	 * we have the metadata of the resource but not the actual data
	 */
	virtual void load_metadata(ResourceLoadError *error);

	/*
	 * saves a resource to memory
	 * not implemented yet
	 */
	virtual void save();
	
	/*
	 * unloads a resource from memory. please call the 
	 * parent function to update the resource state
	 */
	virtual void unload();

private:
	const char *get_path() const { return m_path.c_str(); }
	const char *get_name() const { return m_name.c_str(); }
	UUID get_groupid() const { return m_groupID; }

protected:

	/*
	 * reads a file as characters and returns a char* 
	 * YOU MUST FREE THIS MEMORY YOURSELF
	 */
	Pair<char*, u32> read_file_strings(ResourceLoadError *error);
	
	/*
	 * reads a file as bytes and returns a char* 
	 * YOU MUST FREE THIS MEMORY YOURSELF
	 */
	Pair<char*, u32> read_file_bytes(ResourceLoadError *error);

	/*
	 * returns the json of the file 
	 */
	nlohmann::json read_file_json(ResourceLoadError *error);

protected:
	String m_path;
	String m_name;
	u32 m_size;
	UUID m_groupID;

	ResourceLoadState m_loadState {ResourceLoadState_Unloaded};
};

}

#endif
