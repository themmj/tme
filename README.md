# TME

A small tile map editor written in C++ using OpenGL for a uni course.

# Setup

To set this project up on a debian based system using apt for package management just run the `setup` script.
For other Linux systems have a look at the `setup` script and install the equivalent dependencies.

After that a build directory should have been created inside the projects root folder. Additionally the CMake
project should have been initialized and all targets should have been built once. To execute any target navigate
inside the `build` folder and run any of the available `make` targets with `make <target>`.

target|meaning
------|-------
`run`|execute the tme executable
`test`|run the gtest executable containing unit tests
`coverage`|run `test` and generate coverage report located in tme/test/coverage
`doc-doxygen`|create doxygen documentation located in tme/doc/html/index.html
`doc-puml`|create plant uml diagrams located in tme/doc/puml
`doc-puml-svg`|create svg files from plant uml diagrams located in tme/doc/svg

# Grading criteria

## LOC

Statistics for the application code:

File|blank|comment|code
:-------|-------:|-------:|-------:
src/platform/glfw.hpp|18|60|199
src/app/layers/ui.cpp|23|4|198
src/core/graphics/batch.cpp|23|10|168
src/platform/glfw.cpp|36|16|150
src/core/key.hpp|9|59|146
src/core/graphics/shader.cpp|27|1|131
src/app/layers/editing.cpp|12|5|109
src/app/graphics/texture.cpp|17|1|106
src/core/graphics/batch.hpp|33|202|88
src/core/graphics/vertex.cpp|16|1|88
src/core/layers/imgui.cpp|12|6|87
src/core/graphics/buffer.cpp|9|2|77
src/core/storage.hpp|23|83|68
src/app/graphics/tile.cpp|19|1|66
src/core/graphics/vertex.hpp|19|63|65
src/core/graphics/texture.cpp|16|1|64
src/core/window.cpp|12|2|62
src/app/graphics/tile.hpp|23|110|60
src/core/window.hpp|28|103|60
src/app/graphics/color.cpp|14|1|59
src/core/events/mouse.hpp|19|85|58
src/app/layers/map.cpp|9|3|57
src/core/events/window.hpp|20|89|57
src/core/graphics/shader.hpp|17|83|55
src/core/events/key.hpp|16|48|53
src/app/graphics/texture.hpp|20|87|53
src/core/application.cpp|11|1|47
src/app/tilemap.cpp|12|1|45
src/app/tilemap.hpp|13|60|44
src/core/graphics/common.cpp|9|1|42
src/core/events/event.hpp|17|54|42
src/app/camera.cpp|13|3|42
src/core/graphics/texture.hpp|16|33|42
src/core/layers/layer.hpp|16|35|40
src/core/graphics/buffer.hpp|13|65|39
src/core/layers/layer.cpp|9|1|37
src/core/application.hpp|15|31|37
src/core/graphics/common.hpp|16|72|37
src/app/graphics/color.hpp|17|41|36
src/app/layers/ui.hpp|13|12|34
src/core/log.hpp|9|34|34
src/app/layers/editing.hpp|10|12|34
src/app/camera.hpp|15|64|32
src/core/layers/imgui.hpp|8|12|31
src/app/editor.cpp|9|1|29
src/core/events/dispatcher.hpp|10|24|29
src/app/layers/map.hpp|10|14|29
src/app/layers/background.cpp|5|1|25
src/core/exceptions/validation.hpp|8|25|25
src/core/exceptions/common.hpp|7|30|25
src/core/graphics/index.cpp|4|1|22
src/app/layers/background.hpp|8|16|21
src/main.cpp|7|3|20
src/core/graphics/index.hpp|7|23|19
src/app/editor.hpp|7|11|17
src/namespaces.hpp|15|11|16
src/core/loggable.hpp|6|17|16
src/core/exceptions/input.hpp|6|12|15
src/core/exceptions/graphics.hpp|6|9|15
src/platform/context.hpp|6|9|14
src/core/events/handler.hpp|5|9|14
src/core/log.cpp|4|3|13
src/core/graphics/gl.hpp|3|1|4
SUM:|855|1878|3447

Statics for the test code:

File|blank|comment|code
:-------|-------:|-------:|-------:
test/src/core/graphics/batch_test.cpp|39|8|167
test/src/core/graphics/buffer_test.cpp|22|7|103
test/src/core/storage_test.cpp|28|1|82
test/src/core/events/key_test.cpp|14|0|76
test/src/core/layers/layer_test.cpp|22|2|75
test/src/core/graphics/vertex_test.cpp|15|0|74
test/src/platform/glfw_test.cpp|11|9|64
test/src/core/events/window_test.cpp|12|2|59
test/src/core/window_test.cpp|13|0|55
test/src/core/graphics/shader_test.cpp|8|0|43
test/src/core/events/dispatcher_test.cpp|11|0|41
test/src/sigcounter.hpp|11|0|38
test/src/core/key_test.cpp|6|0|36
test/src/core/application_test.cpp|7|3|34
test/src/core/events/event_test.cpp|10|0|33
test/src/core/events/mouse_test.cpp|9|1|28
test/src/core/graphics/texture_test.cpp|6|0|26
test/src/core/graphics/base.hpp|9|0|25
test/src/core/loggable_test.cpp|6|0|23
test/src/core/layers/imgui_test.cpp|5|0|19
test/src/core/graphics/index_test.cpp|5|0|15
test/src/core/log_test.cpp|3|0|12
SUM:|272|33|1128

Both tables were created with `cloc` (github.com/AlDanial/cloc v 1.81  T=0.04 s (2149.5 files/s, 195801.2 lines/s)).

## SOLID

The goal of solid is to create easy, reusable, flexible, and maintainable code.

### Single Responsibility

Each class has a single responsibility / provides extendable functionality for a single purpose.

The templated class tme::core::Storage is used to create, get and destroy instances of the template type. It is
used to provide a unified storage interface for all classes. That means its only responsibility is acting as a storage medium.  
The tme::core::graphics::IndexBuffer is used to transmit index data from the application to OpenGL.

### Open Closed

Classes should be open to extendability but closed for modification.

The tme::core::graphics::VertexLayout is a good example for this. It can easily be extended to accept a new data type by providing an implementation
of the templated getElement function for the new data type.  
The tme::core::graphics::Shader uses two tme::core::graphics::Shader::Stage instances (vertex and fragment stage) to be constructed. To add an additional
stage to the shader the tme::core::graphics::Shader::Stage::Type enum can be extended with another value and extending the Shader with another constructor
taking three or more stages.

### Liskov Substitution

Extended types have to be usable in place of their parent class preserving the parents functionality. 

The tme::app::graphics::TileFactory and its derived types fit this principle quite well. The derived classes construct different types of Tiles returned
as a tme::app::graphics::Tile* to preserve the functionality of its parent. The created tiles are stored in multiple tme::app::layers::MapLayer. All stored
tiles are derived types of Tile implementing its virtual methods like update, getBatchConfig etc.

### Interface Segregation

Interfaces have a single use case.

All interfaces created in this project adhere to this, for example the tme::core::graphics::Bindable interface is used to bind and unbind graphics object
whereas the tme::core::Loggable interface is used to trivialize logging.

### Dependency Inversion

Classes only depend on abstractions. That means changes in lower level classes don't affect higher level classes.

To be able to create different types of Tiles the tme::app::Cursor of a tme::app::Tilemap uses the abstract tme::app::graphics::TileFactory to construct Tiles. Depending on the
selected tile type the factory is swapped out for another derived class implementing the construct method to create the specific Tile.  
A tme::core::WindowApplication manages a abstract tme::core::Window. Its implementation depends on the injected source file providing an implementation
for the Window::create function returning a tme::core::Window*. Currently only the tme::platform::GlfwWindow is implemented but this could easily be swapped out
without changing the classes using it.

## GRASP

GRASP principles aim to simplify the software and aid in forming a mental model.

### Low Coupling

Keeping the number of dependencies between classes low.

By relying on the tme::core::graphics::Renderable and tme::core::events::Handler interface the coupling can be minimized. The editor is only aware of its application Layers and
its Tilemap. Once it receives an Event from "somewhere" it can choose to process it directly or forward it to its layers. One of its layers,
the tme::app::layers::Editing Layer is aware of the different tme::app::layers::MapLayer of a tme::app::Tilemap and can choose to process or forward the Event. These MapLayers are then
aware of their individual Tiles. The same process only in reverse is applied to rendering. The individual dependencies are kept very low
because of this.

### High Cohesion

Classes should only consist of properties and functions supporting their resposibility.

For example a tme::core::graphics::Buffer contains the necessary properties to manage its corresponding OpenGL buffer and keeping track of what parts of it are
in use or free. A tme::core::graphics::Batch is only responsible for receiving tme::core::graphics::Batchable objects and propagating their vertex and index data to its Vertex- and
IndexBuffer. Both only contain the necessary logic and attributes to complete their defined tasks.

## DRY

Don't repeat yourself.

Interacting with an OpenGL buffer is almost always identical behaviour. Because of that the tme::core::graphics::Buffer class was implemented, which is able to
communicate with any OpenGL buffer given its type value. Based upon that the tme::core::graphics::VertexBuffer and tme::core::graphics::IndexBuffer were implemented. They provide the base
class with their corresponding type value and added some additional functionality if needed. That way a redundant implementation for
buffer communication was avoided.

## Design Patterns

Throughout the project various design patterns were used. The following describes two patterns with examples. The UML diagrams should have been created for the
classes in the linked documentation. Otherwise the plant uml diagrams are located in the doc/puml folder. If plantuml.jar is in the PATH then SVG files based on said plant
uml files should be located in doc/svg.

### Factory Method

A factory provides an interface for creating an object which derived classes have to implement. The created object is then referred to by a general interface.
It is used in cases where a class cannot know which specific type of object it wants to create because for example its dependent on user input.

The abstract tme::app::graphics::TileFactory has two derived types tme::app::graphics::ColorTileFactory and tme::app::graphics::TextureTileFactory implementing the construct method
to construct a tme::app::graphics::ColorTile or tme::app::graphics::TextureTile respectively. What factory is being used depends on selections in the editors ui.

### Object Pools

Object Pools are a mechanism to reuse objects which are expensive to create. This avoids expensive and redundant construction, thus increasing performance.

The templated tme::core::Storage class can be used as a Singleton to provide global access to non trivial objects. These objects include tme::core::graphics::Texture
and tme::core::graphics::Shader. Because they have to perform file io and communicate heavily with OpenGL it is best to reuse them with their respective global Storage
instances. Additionally this mechanism can avoid expensive function calls by passing the objects id around instead of the objects themselfes.

## Domain Driven Design

### Ubiquitous Language

Word|Meaning
----|-------
Tile|Representation of a square with a set size and data to render it.
Tilemap|Central point of interest. Contains Layers of Tiles and a Cursor.
Layer|Logically independent layer with a set responsibility. Can receive Events and be rendered.
Cursor|Adds additional information to the mouse cursor used for editing.
Event|Indicator that something happened containing additional data to describe what happened. 
Batch|Logical combination of different buffer to group similar objects for rendering.
Batcher|Manages variable set of Batches and forwards fitting objects into them.

### Entities

An entity is not only defined by its attributes but also by a unique identifier.

All classes implementing the tme::core::Mappable interface can in turn qualify as an Entity because it provides a pure virtual function getId.
Derived types have to implement this function which will provide a unique identifier. Implementing this interface allows classes to be used in
conjunction with tme::core::Storage.

### Value Objects

Value objects are immutable descriptions of characteristics which can be shared beween multiple instances.

The graphics abstractions makes heavy use of them. If data is added to a batch it returns a tme::core::graphics::Batch::Entry. This is a value object containing
two other value objects of type tme::core::graphics::Buffer::Space describing the position and size of the data inside the index and vertex buffers. They can be
used to update or delete data from the Batch/Buffer in the future. These value objects can then be used by the tme::core::graphics::Batcher to create mappings.
They allow do decouple the Batcher from the underlying Buffers.

### Aggregates

Aggregates are a cluster of domain objects treated like a singel unit. A single aggregate root is exposed which manages all other components.

The tme::core::graphics::Batcher is the top level part of the graphics abstraction. It contains a list of batches and a mapping table. It only exposes a set and unset
method which both take in a tme::core::graphics::Batchable object. Based on this the Batcher will either perform an upsert or an removal of the objects data inside
one of the Batches it sees fit. The batcher is the aggregate root exposing a set and unset method and manages the corresponding batches and mappings
accordingly. That way optimal distribution of graphics objects into buffers is completely abstracted and higher level components only have to set and unset the
tme::core::graphics::Batchable objects they want to render.

### Repository

A repository exposes methods to retrieve saved domain objects. They can be exchanged easily and support alternative storage implementations.

The tme::core::Mappable interface is currently used by the templated Storage class to provide either local or global storage of identifiable objects.
At the moment only a simple map is used as the underlying container of choice. However, this could easily be exchanged or made configurable with
extra options keeping the map as the default behaviour. This could be used to optimize performance by preferring sequential containers under certain conditions.

## Architecture

The application can be separated into three layers. All three layers are either directly or indirectly connected to the tme::core::events::Event system.

```
+-------------------------------+
|                               |
|   Presentation <-\            |
|   |              |            |
|   |              |            |
|   V              |            |
|   Domain <------ +Eventsystem |
|   |              |            |
|   |              |            |
|   V              |            |
|   Data <---------/            |
|                               |
+-------------------------------+
```

### Event System

The event system is the backbone of the architecture because it enables all layers to react to certain events like key presses or a window resize.
Classes implementing the tme::core::events::Handler are able to receive events. If they also derive from the tme::core::events::Dispatcher class
they are able to define specific reactions for destinct event types. That way every layer is able to react to events like input and are implicitly
synchronized.

### Presentation

The presentation of information includes the UI generated by ImGui and the visual representation of the tilemap. The tme::app::layers::EditingUI
creates the UI with which the user interacts. It then calls into the domain layer to update states like the tme::app::Cursor. The handling of input
is done via Events.

### Domain

Using domain objects like the tme::app::Cursor, Tiles are created and set to render with the tme::core::graphics::Batcher. In doing to it calls into
the Data layer.

### Data

The data layer contains mostly of the data contained in the Tiles and in turn Buffers. Updates of the data are triggered from the layer above.

## Unit Tests

As stated in the setup, inside the build directory a simple `make test` executes all unit tests. This will test the complete core part of the code.
Testing the creation of immediate gui (ImGui) or handling of user input had to be done manually which is why it is not covered with unit tests.

### ATRIP

Requirement|Implementation
-----------|--------------
Automatic|all unit tests can be executed with a single call of `make test`
Thourough|all core functionality is fully tested
Repeatable|all tests can be repeated without affecting their results
Independant|all tests can be executed independently from each other as long as global systems like Log are initialized
Professional|the quality standard of the tested code and test code are the same

### Coverage

As stated in the setup, inside the build directory a simple `make coverage` executes all the tests and builds the coverage report inside
test/coverage/coverage.html.

### Mocks

Mocks were used to verify expected behavior of the tested class. These include tme::core::Window tests, tme::core::layers::Stack tests etc.
They are for example used to check how member functions like onEvent are called on tme::core::layers::Layer to ensure handled tme::core::events::Event
are not passed down to lower layers. 

## Refactoring

By allowing to use the tme::core::Storage class as a Singleton, object pools can be realized. Besides that it was attempted to implement a
concept of default values. The reason was that it would provide a better user experience if default shaders were available for the different types of Tiles.
This introduced a lot of duplicate and complex code and resulted in inconsistent life time of pooled objects. The commit [2d03e20e956ad9c2b980e82499c4bdcc0e8f3524](https://github.com/themmj/tme/commit/2d03e20e956ad9c2b980e82499c4bdcc0e8f3524)
removes the explicit concept of defaults. Instead, the derived Tile types can create a default shader to be used for the tile. All pooled graphics objects are cleared
before the window is closed to properly clean up. The Codesmells identified were duplicate+complex code and inconsistent life time of objects. The refactoring removed the
code in question and unified lifetime.

