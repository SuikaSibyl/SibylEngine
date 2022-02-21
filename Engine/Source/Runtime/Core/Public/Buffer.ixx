module;
export module Core.Buffer;
import Core.MemoryManager;

namespace SIByL
{
	inline namespace Core
	{
		export class Buffer
		{
		public:
			Buffer();
			Buffer(size_t const& _size, size_t const& _alignment);
			Buffer(Buffer const& rhs);
			Buffer(Buffer && rhs);
			virtual ~Buffer();

			Buffer& operator = (Buffer const& rhs);
			Buffer& operator = (Buffer && rhs);

			auto getData() const -> char*;
			auto getpSize() ->size_t*;
			auto getSize() const ->size_t;

		private:
			char* data;
			size_t size;
			size_t alignment;
		};
	}
}