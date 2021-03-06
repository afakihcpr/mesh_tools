/*
 *  Software License Agreement (BSD License)
 *
 *  Robot Operating System code by the University of Osnabrück
 *  Copyright (c) 2015, University of Osnabrück
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   1. Redistributions of source code must retain the above
 *      copyright notice, this list of conditions and the following
 *      disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above
 *      copyright notice, this list of conditions and the following
 *      disclaimer in the documentation and/or other materials provided
 *      with the distribution.
 *
 *   3. Neither the name of the copyright holder nor the names of its
 *      contributors may be used to endorse or promote products derived
 *      from this software without specific prior written permission.
 *
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 *  TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 *  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 *  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 *  OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 *  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 *  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 *  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *
 *
 *  textured_mesh_display.h
 *
 *
 *  authors:
 *
 *    Sebastian Pütz <spuetz@uni-osnabrueck.de>
 *    Henning Deeken <hdeeken@uni-osnabrueck.de>
 *    Marcel Mrozinski
 *    Nils Oesting
 *    Kristin Schmidt <krschmidt@uni-osnabrueck.de>
 *    Jan Philipp Vogtherr <jvogtherr@uni-osnabrueck.de>
 */

#ifndef TEXTURED_MESH_DISPLAY_H
#define TEXTURED_MESH_DISPLAY_H

#include "textured_mesh_visual.h"

#include <mesh_msgs/MeshGeometryStamped.h>
#include <mesh_msgs/MeshGeometry.h>
#include <mesh_msgs/MeshVertexColorsStamped.h>
#include <mesh_msgs/MeshVertexColors.h>
#include <mesh_msgs/MeshVertexCostsStamped.h>
#include <mesh_msgs/MeshVertexCosts.h>
#include <mesh_msgs/MeshMaterialsStamped.h>
#include <mesh_msgs/MeshMaterials.h>
#include <mesh_msgs/Material.h>
#include <mesh_msgs/Texture.h>

#include <sensor_msgs/Image.h>

#include <rviz/display.h>

#ifndef Q_MOC_RUN
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <message_filters/cache.h>

#include <tf/message_filter.h>

#include <boost/circular_buffer.hpp>


#endif

namespace rviz
{

// Forward declaration
class BoolProperty;
class ColorProperty;
class FloatProperty;
class IntProperty;
class RosTopicProperty;
class EnumProperty;
class StringProperty;

} // End namespace rviz


namespace rviz_mesh_plugin
{

// Forward declaration
class TexturedMeshVisual;

/**
 * @brief Class to show options in rviz window.
 */
class TexturedMeshDisplay : public rviz::Display
{
Q_OBJECT
public:

    static size_t displayCounter;

    /**
     * @brief Constructor.
     */
    TexturedMeshDisplay();

    /**
     * @brief Destructor.
     */
    ~TexturedMeshDisplay();

protected:

    /**
     * @brief Initialises all nessessary things to get started.
     */
    void onInitialize();

    /**
     * @brief Clears whole stored data.
     */
    void reset();

    /**
     * @brief Set the topics to subscribe.
     */
    void subscribe();

    /**
     * @brief Unsubscribes all topics.
     */
    void unsubscribe();

    /**
     * @brief Calls subscribe() if display is enabled
     */
    void onEnable();

    /**
     * @brief Calls unsubscribe() and reset() if display is disabled.
     */
    void onDisable();

    /**
     * @brief Sets the fixed frame.
     */
    void fixedFrameChanged();

    /**
     * @brief Handler for incoming geometry messages. Validate data and update mesh
     * @param meshMsg The geometry
     */
    void incomingGeometry(const mesh_msgs::MeshGeometryStamped::ConstPtr& meshMsg);

    /**
     * @brief Handler for incoming vertex color messages. Validate data and update mesh
     * @param colorsStamped The vertex colors
     */
    void incomingVertexColors(const mesh_msgs::MeshVertexColorsStamped::ConstPtr& colorsStamped);

    /**
     * @brief Handler for incoming vertex cost messages. Validate data and update mesh
     * @param costsStamped The vertex costs
     */
    void incomingVertexCosts(const mesh_msgs::MeshVertexCostsStamped::ConstPtr& costsStamped);


private Q_SLOTS:

    /**
     * @brief Updates material for each mesh displayed by trianglemesh_visual.
     */
    void updateMesh();

    /**
     * @brief Sets capacity of trianglemesh_visual.
     */
    void updateMeshBufferSize();

    /**
     * @brief Updates the subscribed topic.
     */
    void updateTopic();

    /**
     * @brief Initializes the used services.
     */
    void initServices();

    /**
     * @brief Updates the vertex color service.
     */
    void updateVertexColorService();

    /**
     * @brief Updates the material and texture services.
     */
    void updateMaterialAndTextureServices();

    /**
     * @brief Updates the vertex cost service.
     */
    void updateVertexCostService();

    /**
     * @brief Updates the topic synchronizer
     */
    void updateSynchronizer();

    /**
     * @brief Update the vertex costs
     */
    void updateVertexCosts();

private:

    /**
     * @brief Creates a new visual and returns shared pointer
     * @return The newly created visual
     */
    boost::shared_ptr<TexturedMeshVisual> getNewVisual();

    /**
     * @brief Creates a new visual and returns shared pointer
     * @return The current visual
     */
    boost::shared_ptr<TexturedMeshVisual> getCurrentVisual();

    /**
     * @brief Cache function for vertex cost messages.
     * @param costsStamped The vertex cost message
     */
    void cacheVertexCosts(const mesh_msgs::MeshVertexCostsStamped::ConstPtr costsStamped);

    /**
     * @brief Sets data for trianglemesh_visual and updates the mesh.
     * @param meshMsg Message containing geometry information
     */
    void processMessage(const mesh_msgs::MeshGeometryStamped::ConstPtr& meshMsg);

    /**
     * @brief Requests vertex colors from the specified service
     * @param visual Visual to which the vertex colors will be added
     * @param uuid Mesh UUID
     */
    void requestVertexColors(boost::shared_ptr<TexturedMeshVisual> visual, std::string uuid);

    /**
     * @brief Requests vertex costs from the specified service
     * @param visual Visual to which the vertex costs will be added
     * @param uuid Mesh UUID
     */
    void requestVertexCosts(boost::shared_ptr<TexturedMeshVisual> visual, std::string uuid);

    /**
     * @brief Requests materials from the specified service
     * @param visual Visual to which the materials will be added
     * @param uuid Mesh UUID
     */
    void requestMaterials(boost::shared_ptr<TexturedMeshVisual> visual, std::string uuid);

    /**
     * @brief initial service call for UUID & geometry
     */
    void initialServiceCall();

    /// Subscriber for meshMsg
    message_filters::Subscriber<mesh_msgs::MeshGeometryStamped> m_meshSubscriber;

    /// Subscriber for vertex colors
    message_filters::Subscriber<mesh_msgs::MeshVertexColorsStamped> m_vertexColorsSubscriber;

    /// Subscriber for vertex costs
    message_filters::Subscriber<mesh_msgs::MeshVertexCostsStamped> m_vertexCostsSubscriber;

    /// Messagefilter for meshMsg
    tf::MessageFilter<mesh_msgs::MeshGeometryStamped>* m_tfMeshFilter;

    /// Messagefilter for vertex colors
    tf::MessageFilter<mesh_msgs::MeshVertexColorsStamped>* m_tfVertexColorsFilter;

    /// Messagefilter for vertex costs
    tf::MessageFilter<mesh_msgs::MeshVertexCostsStamped>* m_tfVertexCostsFilter;

    /// Synchronizer for meshMsgs
    message_filters::Cache<mesh_msgs::MeshGeometryStamped>* m_meshSynchronizer;

    /// Synchronizer for vertex colors
    message_filters::Cache<mesh_msgs::MeshVertexColorsStamped>* m_colorsSynchronizer;

    /// Synchronizer for vertex costs
    message_filters::Cache<mesh_msgs::MeshVertexCostsStamped>* m_costsSynchronizer;

    /// Counter for the received messages
    uint32_t m_messagesReceived;

    /// Shared pointer to store the created objects of trianglemesh_visual
    boost::circular_buffer<boost::shared_ptr<TexturedMeshVisual> > m_meshVisuals;

    /// Counter for the meshes
    size_t m_meshCounter;

    /// DisplayID
    size_t m_displayID;

    /// Uuid of the last received message
    std::string m_lastUuid;

    /// Property to handle topic for meshMsg
    rviz::RosTopicProperty* m_meshTopic;

    /// Property to handle topic for vertex colors
    rviz::RosTopicProperty* m_vertexColorsTopic;

    /// Property to handle topic for vertex cost maps
    rviz::RosTopicProperty* m_vertexCostsTopic;

    /// Property to handle service name for vertexColors
    rviz::StringProperty* m_vertexColorServiceName;

    /// Property to handle service name for materials
    rviz::StringProperty* m_materialServiceName;

    /// Property to handle service name for textures
    rviz::StringProperty* m_textureServiceName;

    /// Property to handle service name for vertex costs
    rviz::StringProperty* m_vertexCostServiceName;

    /// Property to set meshBufferSize
    rviz::IntProperty* m_meshBufferSize;

    /// Property to set wireframe color
    rviz::ColorProperty* m_wireframeColor;

    /// Property to set wireframe transparency
    rviz::FloatProperty* m_wireframeAlpha;

    /// Property to set faces color
    rviz::ColorProperty* m_facesColor;

    /// Property to set faces transparency
    rviz::FloatProperty* m_facesAlpha;

    /// Property to use the vertex colors
    rviz::BoolProperty* m_facesVertexColors;

    /// Property to use the triangle colors
    rviz::BoolProperty* m_facesTriangleColors;

    /// Property to set the size of the normals
    rviz::FloatProperty* m_scalingFactor;

    /// Property to set the color of the normals
    rviz::ColorProperty* m_normalsColor;

    /// Property to set the transparency of the normals
    rviz::FloatProperty* m_normalsAlpha;

    /// Property to select the display type
    rviz::EnumProperty* m_displayType;

    /// Property to select the wireframe
    rviz::BoolProperty* m_showWireframe;

    /// Property to select the normals
    rviz::BoolProperty* m_showNormals;

    /// Client to request the vertex colors
    ros::ServiceClient m_vertexColorClient;

    /// Client to request the materials
    ros::ServiceClient m_materialsClient;

    /// Client to request the textures
    ros::ServiceClient m_textureClient;

    /// Client to request the vertex costs
    ros::ServiceClient m_vertexCostClient;

    /// Client to request the UUID
    ros::ServiceClient m_uuidClient;

    /// Client to request the geometry
    ros::ServiceClient m_geometryClient;

    /// Property to only show textured faces when texturizing is enabled
    rviz::BoolProperty* m_showTexturedFacesOnly;

    /// Property to select different types of vertex cost maps to be shown
    rviz::EnumProperty* m_selectVertexCostMap;

    /// Property for selecting the color type for cost display
    rviz::EnumProperty* m_costColorType;

    /// Property for using custom limits for cost display
    rviz::BoolProperty* m_costUseCustomLimits;

    /// Property for setting the lower limit of cost display
    rviz::FloatProperty* m_costLowerLimit;

    /// Property for setting the upper limit of cost display
    rviz::FloatProperty* m_costUpperLimit;

    /// Cache for received vertex cost messages
    std::map<std::string, const mesh_msgs::MeshVertexCostsStamped::ConstPtr> m_costCache;

};
} // End namespace rviz_mesh_plugin

#endif
