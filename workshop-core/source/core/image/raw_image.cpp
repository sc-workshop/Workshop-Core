#include "core/image/raw_image.h"

#include "core/memory/memory.h"

namespace wk
{
	RawImage::RawImage(
		uint8_t* data,
		uint16_t width, uint16_t height,
		Image::PixelDepth depth,
		Image::ColorSpace space
	) : m_space(space), m_depth(depth), m_data(data)
	{
		m_width = width;
		m_height = height;
		m_type = Image::PixelDepthBaseTypeTable[(uint8_t)depth];
	};

	RawImage::RawImage(
		uint16_t width, uint16_t height,
		Image::PixelDepth depth, Image::ColorSpace space
	) : m_space(space), m_depth(depth)
	{
		m_width = width;
		m_height = height;
		m_type = Image::PixelDepthBaseTypeTable[(uint8_t)depth];

		uint8_t pixel_size = PixelDepthTable[(uint16_t)depth].byte_count;

		m_allocated_data = Memory::allocate((width * height) * pixel_size);
		m_data = m_allocated_data;
	};

	RawImage::~RawImage()
	{
		if (m_allocated_data)
		{
			free(m_allocated_data);
		}
	};

	size_t RawImage::data_length() const
	{
		return Image::calculate_image_length(m_width, m_height, m_depth);
	};

	uint8_t* RawImage::data() const
	{
		return m_data;
	};

	Image::BasePixelType RawImage::base_type() const
	{
		return m_type;
	};

	Image::ColorSpace RawImage::colorspace() const
	{
		return m_space;
	}

	Image::PixelDepth RawImage::depth() const
	{
		return m_depth;
	}

	void RawImage::copy(RawImage& image) const
	{
		size_t data_size = Image::calculate_image_length(image.width(), image.height(), image.depth());
		uint8_t* data = nullptr;

		Image::PixelDepth current_depth = m_depth;
		if (image.width() != m_width || image.height() != m_height)
		{
			// Convert image to a more convenient type to change size
			switch (m_depth)
			{
				// RGBA
			case Image::PixelDepth::RGBA4:
			case Image::PixelDepth::RGB5_A1:
				current_depth = Image::PixelDepth::RGBA8;
				break;

				// RGB
			case Image::PixelDepth::RGB565:
				current_depth = Image::PixelDepth::RGB8;
				break;

			case Image::PixelDepth::RGB8:
			case Image::PixelDepth::LUMINANCE8_ALPHA8:
			case Image::PixelDepth::LUMINANCE8:
			case Image::PixelDepth::RGBA8:
			default:
				break;
			}

			uint8_t* base_depth_data = nullptr;

			if (current_depth != m_depth)
			{
				base_depth_data = Memory::allocate(Image::calculate_image_length(m_width, m_height, current_depth));
				Image::remap(m_data, base_depth_data, m_width, m_height, m_depth, current_depth);
			}

			data = Memory::allocate(Image::calculate_image_length(image.width(), image.height(), m_depth));
			Image::resize(
				base_depth_data ? base_depth_data : m_data, data,
				m_width, m_height,
				image.width(), image.height(),
				m_type, m_space
			);

			if (base_depth_data)
			{
				Memory::free(base_depth_data);
			}
		}

		if (image.depth() != m_depth)
		{
			Image::remap(
				data ? data : m_data, image.data(),
				image.width(), image.height(),
				current_depth, image.depth()
			);
		}
		else
		{
			Memory::copy(data ? data : m_data, image.data(), data_size);
		}

		if (data)
		{
			Memory::free(data);
		}
	};

	void RawImage::write(Stream& buffer)
	{
		buffer.write(m_data, data_length());
	};
}