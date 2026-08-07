digraph "app" {
node [
  fontsize = "12"
];
    "node1" [ label = "app", shape = egg ];
    "node2" [ label = "eruptor_engine", shape = octagon ];
    "node1" -> "node2" [ style = dotted ] // app -> eruptor_engine
    "node3" [ label = "eruptor_event", shape = octagon ];
    "node2" -> "node3" [ style = dotted ] // eruptor_engine -> eruptor_event
    "node4" [ label = "project_compiler_warmings", shape = pentagon ];
    "node3" -> "node4"  // eruptor_event -> project_compiler_warmings
    "node5" [ label = "eruptor_hardware", shape = octagon ];
    "node2" -> "node5" [ style = dotted ] // eruptor_engine -> eruptor_hardware
    "node6" [ label = "Vulkan::Vulkan", shape = septagon ];
    "node5" -> "node6"  // eruptor_hardware -> Vulkan::Vulkan
    "node0" [ label = "VulkanMemoryAllocator-Hpp\n(VulkanMemoryAllocator-Hpp::VulkanMemoryAllocator-Hpp)", shape = pentagon ];
    "node5" -> "node0"  // eruptor_hardware -> VulkanMemoryAllocator-Hpp
    "node5" -> "node3" [ style = dotted ] // eruptor_hardware -> eruptor_event
    "node7" [ label = "glfw", shape = doubleoctagon ];
    "node5" -> "node7"  // eruptor_hardware -> glfw
    "node8" [ label = "glm::glm", shape = pentagon ];
    "node5" -> "node8"  // eruptor_hardware -> glm::glm
    "node9" [ label = "glm::glm-header-only", shape = pentagon ];
    "node8" -> "node9" [ style = dashed ] // glm::glm -> glm::glm-header-only
    "node5" -> "node4" [ style = dotted ] // eruptor_hardware -> project_compiler_warmings
    "node10" [ label = "eruptor_physic", shape = octagon ];
    "node2" -> "node10"  // eruptor_engine -> eruptor_physic
    "node10" -> "node3" [ style = dotted ] // eruptor_physic -> eruptor_event
    "node11" [ label = "eruptor_scene", shape = octagon ];
    "node10" -> "node11" [ style = dotted ] // eruptor_physic -> eruptor_scene
    "node11" -> "node3" [ style = dotted ] // eruptor_scene -> eruptor_event
    "node12" [ label = "eruptor_resource", shape = octagon ];
    "node11" -> "node12" [ style = dotted ] // eruptor_scene -> eruptor_resource
    "node13" [ label = "Freetype::Freetype", shape = septagon ];
    "node12" -> "node13"  // eruptor_resource -> Freetype::Freetype
    "node14" [ label = "assimp::assimp", shape = doubleoctagon ];
    "node12" -> "node14" [ style = dotted ] // eruptor_resource -> assimp::assimp
    "node15" [ label = "/usr/lib64/libz.so", shape = septagon ];
    "node14" -> "node15" [ style = dashed ] // assimp::assimp -> /usr/lib64/libz.so
    "node16" [ label = "poly2tri", shape = septagon ];
    "node14" -> "node16" [ style = dashed ] // assimp::assimp -> poly2tri
    "node17" [ label = "pugixml", shape = septagon ];
    "node14" -> "node17" [ style = dashed ] // assimp::assimp -> pugixml
    "node18" [ label = "rt", shape = septagon ];
    "node14" -> "node18" [ style = dashed ] // assimp::assimp -> rt
    "node12" -> "node3" [ style = dotted ] // eruptor_resource -> eruptor_event
    "node12" -> "node5" [ style = dotted ] // eruptor_resource -> eruptor_hardware
    "node12" -> "node10"  // eruptor_resource -> eruptor_physic
    "node12" -> "node4" [ style = dotted ] // eruptor_resource -> project_compiler_warmings
    "node11" -> "node8" [ style = dotted ] // eruptor_scene -> glm::glm
    "node11" -> "node4" [ style = dotted ] // eruptor_scene -> project_compiler_warmings
    "node10" -> "node8" [ style = dotted ] // eruptor_physic -> glm::glm
    "node10" -> "node4" [ style = dotted ] // eruptor_physic -> project_compiler_warmings
    "node19" [ label = "eruptor_renderer", shape = octagon ];
    "node2" -> "node19"  // eruptor_engine -> eruptor_renderer
    "node19" -> "node6" [ style = dotted ] // eruptor_renderer -> Vulkan::Vulkan
    "node19" -> "node3" [ style = dotted ] // eruptor_renderer -> eruptor_event
    "node19" -> "node5" [ style = dotted ] // eruptor_renderer -> eruptor_hardware
    "node19" -> "node12" [ style = dotted ] // eruptor_renderer -> eruptor_resource
    "node19" -> "node11" [ style = dotted ] // eruptor_renderer -> eruptor_scene
    "node19" -> "node4" [ style = dotted ] // eruptor_renderer -> project_compiler_warmings
    "node2" -> "node12"  // eruptor_engine -> eruptor_resource
    "node2" -> "node4" [ style = dotted ] // eruptor_engine -> project_compiler_warmings
    "node1" -> "node3" [ style = dotted ] // app -> eruptor_event
    "node1" -> "node10" [ style = dotted ] // app -> eruptor_physic
    "node1" -> "node11" [ style = dotted ] // app -> eruptor_scene
}
