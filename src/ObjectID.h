#pragma once

template<typename OwnerType>
struct ObjectID
{
	ObjectID() {};

	ObjectID(const ObjectID&) = delete;
	ObjectID& operator=(const ObjectID&) = delete;

	ObjectID(ObjectID&& other)
	{
		if (other.id != id)
		{
			id = other.id;
			other.id = 0;
		}
	}

	void operator=(ObjectID&& other)
	{
		if (other.id != id)
		{
			id = other.id;
			other.id = 0;
		}
	}

	bool bind()
	{
		if(id == bound_id)
		{
			return false;
		}
		bound_id = id;
		return true;
	}

	void clear()
	{
		if (bound_id == id)
		{
			bound_id = 0;
		}
		id = 0;
	}

	~ObjectID() 
	{
		clear();
	}

private:
	static inline GLuint bound_id = 0;
	GLuint id = 0;

	friend OwnerType;
};