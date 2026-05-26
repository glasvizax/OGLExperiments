#include "Camera.h"
#include <algorithm>

#include <glm/gtc/matrix_transform.hpp>

glm::mat4 Camera::getPerspectiveMatrix()
{
	if (m_perspective_dirty)
	{
		m_perspective = glm::perspective(glm::radians(m_fov_deg), m_aspect_ratio, m_near_plane, m_far_plane); // TODO: optimize
		m_perspective_dirty = false;
	}
	return m_perspective;
}

glm::mat4 Camera::getViewMatrix()
{
	if (m_view_dirty)
	{
		m_view = glm::lookAt(m_position, m_position + m_direction, m_world_up); // TODO: optimize
		m_view_dirty = false;
	}
	return m_view;
}

void Camera::update(float dt)
{
	bool update_dir = false;
	if (m_yaw_moment)
	{
		yaw_changed = true;
		update_dir = true;
		m_rotation.y += m_rotation_speed_yaw * m_yaw_moment * dt;
		m_yaw_moment = 0;
	}

	if (m_pitch_moment)
	{
		pitch_changed = true;
		update_dir = true;
		m_rotation.x += m_rotation_speed_pitch * m_pitch_moment * dt;
		m_rotation.x = std::clamp(m_rotation.x, -85.0f, 85.0f);
		m_pitch_moment = 0;
	}

	if (update_dir)
	{
		float sin_yaw, cos_yaw, sin_pitch, cos_pitch;
		if (pitch_changed)
		{
			cos_pitch = std::cos(glm::radians(m_rotation.x));
			prev_cos_pitch = cos_pitch;

			sin_pitch = std::sin(glm::radians(m_rotation.x));
			prev_sin_pitch = sin_pitch;
			pitch_changed = false;
		}
		else
		{
			cos_pitch = prev_cos_pitch;
			sin_pitch = prev_sin_pitch;
		}

		if (yaw_changed)
		{
			cos_yaw = std::cos(glm::radians(m_rotation.y));
			prev_cos_yaw = cos_yaw;

			sin_yaw = std::sin(glm::radians(m_rotation.y));
			prev_sin_yaw = sin_yaw;

		}
		else
		{
			cos_yaw = prev_cos_yaw;
			sin_yaw = prev_sin_yaw;
		}

		m_direction.x = cos_pitch * sin_yaw;
		m_direction.y = sin_pitch;
		m_direction.z = cos_pitch * cos_yaw;
		m_direction = glm::normalize(m_direction);

		if (yaw_changed)
		{
			m_right = glm::cross(m_direction, m_world_up);
			yaw_changed = false;
		}

		m_view_dirty = true;
	}

	if (m_along_moment)
	{
		glm::vec3 diff = m_direction * (m_along_moment * m_move_speed * dt);
		m_position += diff;
		m_view_dirty = true;
		m_along_moment = 0;
	}

	if (m_across_moment)
	{
		glm::vec3 diff = m_right * (m_across_moment * m_move_speed * dt);
		m_position += diff;
		m_view_dirty = true;
		m_across_moment = 0;
	}
}
