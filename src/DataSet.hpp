
#ifndef _IRIS_DATASET_
#define _IRIS_DATASET_

#include <cstdint>
#include <string>

namespace com
{
	namespace toxiclabs
	{
		namespace iris
		{
		
			enum class DataType
			{
				None,
				Integer,
				Float,
				String
			};
			
			class DataSet
			{
			
				private:
				
				union
				{
					int data_i;
					float data_f;
				};
					std::string data_s;
				
			
				public:
				
				DataType type;
				
				DataSet();
				DataSet(int data);
				DataSet(float data);
				DataSet(std::string data);
				
				
				~DataSet();
				
				void Set(int data);
				void Set(float data);
				void Set(std::string data);
				
				int GetInt();
				float GetFloat();
				std::string GetString();
				
				void operator=(const int & data);
				void operator=(const float & data);
				void operator=(const std::string & data);
			
			};
		}
	}
}

#endif
