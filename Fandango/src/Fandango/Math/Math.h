#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/matrix_decompose.hpp>

namespace Fandango
{
	class Math
	{
	public:

		// This is a simplification of glm::decompose in matrix_decompose.inl
		static bool DecomposeTransform(
			glm::mat4 const& transform,
			glm::vec3& outTranslation,
			glm::vec3& outRotation,
			glm::vec3& outScale)
		{
			using namespace glm;

			mat4 localMatrix(transform);

			// Normalize matrix
			if (epsilonEqual(localMatrix[3][3], static_cast<float>(0), epsilon<float>()))
				return false;

			// Isolate perspective
			if (
				epsilonEqual(localMatrix[0][3], static_cast<float>(0), epsilon<float>()) ||
				epsilonEqual(localMatrix[1][3], static_cast<float>(0), epsilon<float>()) ||
				epsilonEqual(localMatrix[2][3], static_cast<float>(0), epsilon<float>()))
			{
				// Clear perspective partition
				localMatrix[0][3] = localMatrix[1][3] = localMatrix[2][3] = static_cast<float>(0);
				localMatrix[3][3] = static_cast<float>(1);
			}

			// Ttranslation
			outTranslation = vec3(localMatrix[3]);
			localMatrix[3] = vec4(0, 0, 0, localMatrix[3].w);

			vec3 row[3];

			// Scale and shear
			for (length_t i = 0; i < 3; ++i)
				for (length_t j = 0; j < 3; ++j)
					row[i][j] = localMatrix[i][j];

			// Compute X scale factor and normalize first row
			outScale.x = length(row[0]);
			row[0] = detail::scale(row[0], static_cast<float>(1));
			outScale.y = length(row[1]);
			row[0] = detail::scale(row[1], static_cast<float>(1));
			outScale.z = length(row[2]);
			row[0] = detail::scale(row[2], static_cast<float>(1));

			outRotation.y = asin(-row[0][2]);
			if (cos(outRotation.y) != 0)
			{
				outRotation.x = atan2(row[1][2], row[2][2]);
				outRotation.z = atan2(row[0][1], row[0][0]);
			}
			else
			{
				outRotation.x = atan2(-row[2][0], row[1][1]);
				outRotation.z = 0;
			}

			return true;
		}
	};
}