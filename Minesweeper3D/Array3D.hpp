template <class Type>
class Array3D
{
public:

	Array3D() = default;

	Array3D(size_t width, size_t height, size_t depth)
		: m_width{ width }
		, m_height{ height }
		, m_depth{ depth }
		, m_data(width* height* depth) {}

	Array3D(size_t width, size_t height, size_t depth, const Type& init)
		: m_width{ width }
		, m_height{ height }
		, m_depth{ depth }
		, m_data(width* height* depth, init) {}

	Type& get(size_t x, size_t y, size_t z)
	{
		const size_t index =get_index(x, y, z); // m_width * ....

		return m_data[index];
	}

	Type& get(size_t index)
	{
		return m_data[index];
	}

	const Type& get(size_t x, size_t y, size_t z) const
	{
		const size_t index = get_index(x, y, z); // m_width * ....

		return m_data[index];
	}

	const Type& get(size_t index) const
	{
		return m_data[index];
	}

	void set(size_t x, size_t y, size_t z, Type data)
	{
		const size_t index = get_index(x, y, z);
		m_data[index] = data;
	}

	void set(size_t index, Type data)
	{
		m_data[index] = data;
	}

	void clear()
	{
		//...
		m_data.clear();
	}

	void fill(const Type& value)
	{
		m_data.fill(value);
	}

	size_t width() const
	{
		return m_width;
	}

	size_t height() const
	{
		return m_height;
	}

	size_t depth() const
	{
		return m_depth;
	}


	size_t num_elements() const
	{
		return (m_width * m_height * m_depth);
	}

	size_t get_index(size_t x, size_t y, size_t z)
	{
		return m_width * m_height * z + m_width * y + x; // m_width *
	}

	Vec3 vec3FromIndex(size_t index)
	{
		size_t z = index / (m_width * m_height);
		index %= (m_width * m_height);
		size_t y = index / m_width;
		index %= m_width;

		return Vec3{index, y, z};
	}

	int32 countNeighborBomb(size_t index)
	{
		Vec3 pos = vec3FromIndex(index);
		int32 result = 0;
		for (int i = max(0, pos.x - 1); i < min(m_width, pos.x + 2); i++)
		{
			for (int j = max(0, pos.y - 1); j < min(m_height, pos.y + 2); j++)
			{
				for (int k = max(0, pos.z - 1); k < min(m_depth, pos.z + 2); k++)
				{
					if (get(i, j, k) > 0)
					{
						result++;
					}
				}
			}
		}
		return result;
	}


private:

	Array<Type> m_data;

	size_t m_width = 0;

	size_t m_height = 0;

	size_t m_depth = 0;
};
