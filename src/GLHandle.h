#pragma once

#include <utility>

#include <glad/glad.h>

template <typename Trait>
class GLHandleCacheDefault
{
public:
	static bool isBound(GLuint id)
	{
		return m_current_bound == id;
	}

	static void reset() 
	{
		m_current_bound = 0;
	}

	static void update(GLuint id) 
	{
		m_current_bound = id;
	}

private:
	static inline GLuint m_current_bound = 0;
};

template <typename Trait, typename GLHandleCache = GLHandleCacheDefault<Trait>>
class GLHandle
{
public:
	GLHandle() noexcept = default;

	explicit GLHandle(GLuint id) noexcept
		: m_id(id)
	{
	}

	~GLHandle()
	{
		reset();
	}

	GLHandle(const GLHandle&) = delete;

	GLHandle& operator=(const GLHandle&) = delete;

	GLHandle(GLHandle&& other) noexcept
		: m_id(std::exchange(other.m_id, 0))
	{
	}

	GLHandle& operator=(GLHandle&& other) noexcept
	{
		if (m_id != other.m_id)
		{
			reset(other.m_id);
			other.m_id = 0;
		}
		return *this;
	}

	void reset(GLuint new_id = 0)
	{
		if (m_id != 0 && m_id != new_id)
		{
			if (m_cache.isBound(m_id))
			{
				m_cache.reset();
			}
			Trait::destroy(m_id);
		}
		m_id = new_id;
	}

	GLuint get() const
	{
		return m_id;
	}

	explicit operator bool() const
	{
		return m_id != 0;
	}

	template <typename... Args>
	bool bind(Args&&... args) const
	{
		if (m_cache.isBound(m_id))
		{
			return false;
		}

		Trait::bind(m_id, std::forward<Args>(args)...);
		m_cache.update(m_id, std::forward<Args>(args)...);
		return true;
	}

private:
	GLuint m_id = 0;
	mutable GLHandleCache m_cache;
};


