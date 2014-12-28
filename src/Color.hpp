
#ifndef _IRIS_COLOR_
#define _IRIS_COLOR_

namespace com
{
	namespace toxiclabs
	{
		namespace iris
		{
			class Color
			{
				public:
				
				union
				{
					struct
					{
						float r;
						float g;
						float b;
						float w;
					};
					
					struct
					{
						float x;
						float y;
						float z;
						float w;
					};
					
					float data[4];	
				};
			};
		}
	}
}

#endif
