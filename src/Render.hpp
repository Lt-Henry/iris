
#ifndef _IRIS_RENDER_
#define _IRIS_RENDER_

#include "Color.hpp"

namespace com
{
	namespace toxiclabs
	{
		namespace iris
		{
			class RenderChunk
			{
				public:
				
				Color * image;
				
				int x;
				int y;
				int w;
				int h;
			};
		}
	}

}


#endif
