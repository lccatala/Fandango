#include "SceneHierarchyPanel.h"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include "Fandango/Scene/Components.h"
#include <glm/gtc/type_ptr.hpp>

namespace Fandango 
{
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow 
			| (m_SelectionContext == entity ? ImGuiTreeNodeFlags_Selected : 0);
		bool nodeExpanded = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());

		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}

		// Mark entity for deletion
		bool shouldDeleteEntity = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete entity"))
				shouldDeleteEntity = true;
				
			ImGui::EndPopup();
		}

		if (nodeExpanded)
		{
			ImGui::TreePop();
		}

		if (shouldDeleteEntity)
		{
			m_Context->DestroyEntity(entity);
			if (m_SelectionContext == entity)
				m_SelectionContext = {};
		}
	}

	// TODO: each call to this function needs a different label value, otherwise all the values will be tied and modified at the same time
	static void DrawVec3Controller(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGui::PushID(label.c_str());

		// Divide panel in 2 columns to put labels on the left
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f }); // TODO maybe 0.25f for blue??
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##x", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f }); 
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::PopStyleVar();

		ImGui::Columns(1); // Reset number of columns
		ImGui::PopID();
	}

	void SceneHierarchyPanel::DrawProperties(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str()); // Make buffer null-terminated
			if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		constexpr ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;

		if (entity.HasComponent<TransformComponent>())
		{
			bool open = ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), treeNodeFlags, "Transform");

			if (open)
			{
				auto& transformComponent = entity.GetComponent<TransformComponent>();

				DrawVec3Controller("Translation", transformComponent.Translation);

				glm::vec3 rotationDegrees = glm::degrees(transformComponent.Rotation);
				DrawVec3Controller("Rotation", rotationDegrees);
				transformComponent.Rotation = glm::radians(rotationDegrees);

				DrawVec3Controller("Scale", transformComponent.Scale);

				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<CameraComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), treeNodeFlags, "Camera"))
			{
				auto& cameraComponent = entity.GetComponent<CameraComponent>();
				auto& camera = cameraComponent.Camera;

				ImGui::Checkbox("Primary camera", &cameraComponent.Primary);

				auto projectionType = camera.GetProjectionType();
				const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
				const char* projectionTypeString = projectionTypeStrings[(int)projectionType];
				if (ImGui::BeginCombo("Projection", projectionTypeString))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = projectionTypeString == projectionTypeStrings[i];
						if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
						{
							projectionTypeString = projectionTypeStrings[i];
							camera.SetProjectionType((SceneCamera::ProjectionType)i);
						}
						if (isSelected)
						{
							ImGui::SetItemDefaultFocus();
						}
					}

					ImGui::EndCombo();
				}
				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
				{
					float fov = glm::degrees(camera.GetPerspectiveFOV());
					if (ImGui::DragFloat("FOV", &fov))
					{
						camera.SetPerspectiveFOV(glm::radians(fov));
					}

					float nearClip = camera.GetPerspectiveNearClip();
					if (ImGui::DragFloat("Near Clip", &nearClip))
					{
						camera.SetPerspectiveNearClip(nearClip);
					}

					float farClip = camera.GetPerspectiveFarClip();
					if (ImGui::DragFloat("Far Clip", &farClip))
					{
						camera.SetPerspectiveFarClip(farClip);
					}
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
				{
					float orthographicSize = camera.GetOrthographicSize();
					if (ImGui::DragFloat("Size", &orthographicSize))
					{
						camera.SetOrthographicSize(orthographicSize);
					}

					float nearClip = camera.GetOrthographicNearClip();
					if (ImGui::DragFloat("Near Clip", &nearClip))
					{
						camera.SetOrthographicNearClip(nearClip);
					}

					float farClip = camera.GetOrthographicFarClip();
					if (ImGui::DragFloat("Far Clip", &farClip))
					{
						camera.SetOrthographicFarClip(farClip);
					}

					ImGui::Checkbox("Fixed aspect ratio", &cameraComponent.FixedAspectRatio);
				}

				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			bool open = ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), treeNodeFlags, "Sprite Renderer");
			ImGui::SameLine(ImGui::GetWindowWidth() - 25.0f);
			if (ImGui::Button("+", ImVec2{20, 20}))
				ImGui::OpenPopup("ComponentSettings");
			ImGui::PopStyleVar();

			bool shouldRemoveComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove Component"))
					shouldRemoveComponent = true;
				ImGui::EndPopup();
			}
			if (open)
			{
				auto& srComponent = entity.GetComponent<SpriteRendererComponent>();
				auto& color = srComponent.Color;
				ImGui::ColorEdit4("Color", glm::value_ptr(color));
				ImGui::TreePop();
			}

			if (shouldRemoveComponent)
				entity.RemoveComponent<SpriteRendererComponent>();
		}
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		// Scene Hierarchy panel
		ImGui::Begin("Scene Hierarchy");

		m_Context->m_Registry.each([&](auto entityID)
		{
			Entity entity{ entityID, m_Context.get() };
			DrawEntityNode(entity);
		});

		if (ImGui::IsWindowHovered() && Input::IsMouseButtonPressed(FNDG_MOUSE_BUTTON_1))
		{
			m_SelectionContext = {};
		}

		// Right click on blank space
		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create empty entity"))
				m_Context->CreateEntity("Empty entity");
			ImGui::EndPopup();
		}

		ImGui::End();

		// Entity properties panel
		ImGui::Begin("Properties");
		if (m_SelectionContext)
		{
			DrawProperties(m_SelectionContext);

			if (ImGui::Button("Add component"))
			{
				ImGui::OpenPopup("AddComponent");
			}
			if (ImGui::BeginPopup("AddComponent"))
			{
				if (ImGui::MenuItem("Camera"))
				{
					m_SelectionContext.AddComponent<CameraComponent>();
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::MenuItem("Sprite Renderer"))
				{
					m_SelectionContext.AddComponent<SpriteRendererComponent>();
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}
		}
		ImGui::End();

	}
}