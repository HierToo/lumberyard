/*
* All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
* its licensors.
*
* For complete copyright and license terms please see the LICENSE at the root of this
* distribution (the "License"). All use of this software is governed by the License,
* or, if provided, by the license below or the license accompanying this file. Do not
* remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
*/

#pragma once

#include <AzCore/std/smart_ptr/unique_ptr.h>
#include <AzCore/RTTI/TypeInfo.h>
#include <AzFramework/Physics/RigidBody.h>
#include <AzFramework/Physics/Ragdoll.h>
#include <AzFramework/Physics/Character.h>
#include <AzToolsFramework/UI/PropertyEditor/PropertyEditorAPI.h>
#include <MCore/Source/Color.h>
#include <MCore/Source/Command.h>
#include <MCore/Source/CommandManager.h>
#include <AzQtComponents/Components/Widgets/Card.h>
#include <EMotionFX/Tools/EMotionStudio/EMStudioSDK/Source/EMStudioPlugin.h>
#include <QPushButton>
#include <QWidget>


QT_FORWARD_DECLARE_CLASS(QVBoxLayout)

namespace AZ
{
    class SerializeContext;
}

namespace EMotionFX
{
    class Actor;
    class ActorInstance;
    class Node;
    class ObjectEditor;
    class ColliderWidget;

    class ColliderPropertyNotify
        : public AzToolsFramework::IPropertyEditorNotify
    {
    public:
        ColliderPropertyNotify(ColliderWidget* colliderWidget);

        void BeforePropertyModified(AzToolsFramework::InstanceDataNode* pNode) override;
        void AfterPropertyModified(AzToolsFramework::InstanceDataNode* /*pNode*/) override {}

        void SetPropertyEditingActive(AzToolsFramework::InstanceDataNode* /*pNode*/) override {}
        void SetPropertyEditingComplete(AzToolsFramework::InstanceDataNode* pNode) override;

        void SealUndoStack() override {}

    private:
        MCore::CommandGroup m_commandGroup;
        ColliderWidget* m_colliderWidget;
    };

    class ColliderWidget
        : public AzQtComponents::Card
    {
        Q_OBJECT //AUTOMOC

    public:
        ColliderWidget(QIcon* icon, QWidget* parent, AZ::SerializeContext* serializeContext);

        void Update(Actor* actor, Node* joint, size_t colliderIndex, PhysicsSetup::ColliderConfigType colliderType, const Physics::ShapeConfigurationPair& collider);
        void Update();
        void Reset();

        Actor* GetActor() const { return m_actor; };
        Node* GetJoint() const { return m_joint; }
        size_t GetColliderIndex() const { return m_colliderIndex; }
        PhysicsSetup::ColliderConfigType GetColliderType() const { return m_colliderType; }

    signals:
        void RemoveCollider(int colliderIndex);

    private slots:
        void OnCardContextMenu(const QPoint& position);
        void OnRemoveCollider();

    private:
        EMotionFX::ObjectEditor* m_editor;
        AZStd::unique_ptr<ColliderPropertyNotify> m_propertyNotify;

        Actor* m_actor = nullptr;
        PhysicsSetup::ColliderConfigType m_colliderType = PhysicsSetup::ColliderConfigType::Unknown;
        Node* m_joint = nullptr;
        size_t m_colliderIndex = MCORE_INVALIDINDEX32;
        Physics::ShapeConfigurationPair m_collider;

        QIcon* m_icon;
    };

    class AddColliderButton
        : public QPushButton
    {
        Q_OBJECT //AUTOMOC

    public:
        AddColliderButton(const QString& text, QWidget* parent = nullptr,
            const AZStd::vector<AZ::TypeId>& supportedColliderTypes = {azrtti_typeid<Physics::BoxShapeConfiguration>(),
            azrtti_typeid<Physics::CapsuleShapeConfiguration>(),
            azrtti_typeid<Physics::SphereShapeConfiguration>()});

    signals:
        void AddCollider(AZ::TypeId colliderType);

    private slots:
        void OnCreateContextMenuAddCollider();
        void OnAddColliderActionTriggered(bool checked);

    private:
        AZStd::string GetNameForColliderType(AZ::TypeId colliderType) const;

        AZStd::vector<AZ::TypeId> m_supportedColliderTypes;
    };

    class ColliderContainerWidget
        : public QWidget
    {
        Q_OBJECT //AUTOMOC

    public:
        ColliderContainerWidget(const QIcon& colliderIcon, QWidget* parent = nullptr);
        ~ColliderContainerWidget();

        void Update(Actor* actor, Node* joint, PhysicsSetup::ColliderConfigType colliderType, const Physics::ShapeConfigurationList& colliders, AZ::SerializeContext* serializeContext);
        void Update();
        void Reset();

        /**
         * Render the given colliders.
         * @param[in] colliders The colliders to render.
         * @param[in] actorInstance The actor instance from which the world space transforms for the colliders are read from.
         * @param[in] node The node to which the colliders belong to.
         * @param[in] renderInfo Needed to access the render util.
         * @param[in] colliderColor The collider color.
         */
        static void RenderColliders(const Physics::ShapeConfigurationList& colliders,
            const ActorInstance* actorInstance,
            const Node* node,
            EMStudio::EMStudioPlugin::RenderInfo* renderInfo,
            const MCore::RGBAColor& colliderColor);

        static void RenderColliders(PhysicsSetup::ColliderConfigType colliderConfigType,
            const MCore::RGBAColor& defaultColor,
            const MCore::RGBAColor& selectedColor,
            EMStudio::RenderPlugin* renderPlugin,
            EMStudio::EMStudioPlugin::RenderInfo* renderInfo);

        static int s_layoutSpacing;

    signals:
        void RemoveCollider(int index);

    private slots:
        void OnRemoveCollider(int colliderIndex);

    private:
        Actor* m_actor = nullptr;
        PhysicsSetup::ColliderConfigType m_colliderType = PhysicsSetup::ColliderConfigType::Unknown;
        Node* m_joint = nullptr;
        QVBoxLayout* m_layout = nullptr;
        AZStd::vector<ColliderWidget*> m_colliderWidgets;
        QIcon m_colliderIcon;

        class ColliderEditedCallback
            : public MCore::Command::Callback
        {
        public:
            ColliderEditedCallback(ColliderContainerWidget* parent, bool executePreUndo, bool executePreCommand = false);
            bool Execute(MCore::Command* command, const MCore::CommandLine& commandLine);
            bool Undo(MCore::Command* command, const MCore::CommandLine& commandLine);

        private:
            ColliderContainerWidget* m_widget;
        };
        ColliderEditedCallback* m_commandCallback;
    };
} // namespace EMotionFX
