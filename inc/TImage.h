template <typename T = unsigned char, 
	typename Container = std::vector<T>>
class TImage
{
	// Remove assignemnt and copy operators
	// Prevent accidental data copy (performance issues) during assignments
	TImage & operator=(const TImage&) = delete;
	TImage(const TImage&) = delete;

public:
	typedef typename T value_type;

	virtual ~TImage() {}
	TImage() {}
	/*
	* The image class constructor
	* @param width - the image width
	* @param height - the image height
	* @param stride - the offset between elements
			for example 1 - Grayscale, 3 - RGB, 4-RGBA when Type is uint8_t 
	*/
	explicit TImage(unsigned width, unsigned height, unsigned stride)
	{
		Resize(width, height, stride);
	}
	
	TImage& Resize(unsigned width, unsigned height, unsigned stride)
	{
		m_stride = stride;
		m_width = width;
		m_height = height;
		m_data.resize(m_width*m_height*m_stride);
		return *this;
	}

	TImage& From(const TImage& source)
	{
		m_stride = source.m_stride;
		m_data = source.m_data;
		return *this;
	}
	// Buffer related getters
	const std::size_t GetWidth() const { return m_width; }
	const std::size_t GetHeight() const { return m_height; }
	const std::size_t GetSize() const { return m_data.size();  }
	const std::size_t GetStride() const { return m_stride; }
	T * GetBuffer() { return m_data.data();  }
	const T * GetBuffer() const { return m_data.data(); }

	Container& GetImageData() { return m_data; }
	const Container& GetImageData() const { return m_data; }

	T * GetRow(const std::size_t row) { 
		return &m_data[row*m_stride*m_width]; 
	}
	const T * GetRow(const std::size_t row) const { 
		return &m_data[row*m_stride*m_width]; 
	}

	// Pixel related operators
	T& operator()(std::size_t row, std::size_t col) { 
		return m_data[row*m_stride*m_width + col]; 
	}
	const T& operator()(std::size_t row, std::size_t col) const { 
		return m_data[row*m_stride*m_width + col]; 
	}

	T& operator[](std::size_t idx) {
		return m_data[idx];
	}
	const T& operator[](std::size_t idx) const {
		return m_data[idx];
	}

protected:
	unsigned      m_stride{ 1 };
	unsigned      m_width{ 0 };
	unsigned      m_height{ 0 };
	Container     m_data;
};
