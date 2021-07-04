CFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

vertSources = $(shell find ./shaders -type f -name "*.vert")
vertObjFiles = $(patsubst %.vert, %.vert.spv, $(vertSources))
fragSources = $(shell find ./shaders -type f -name "*.frag")
fragObjFiles = $(patsubst %.frag, %.frag.spv, $(fragSources))

TARGET = vk.out
$(TARGET): $(vertObjFiles) $(fragObjFiles)
$(TARGET): *.cpp *.hpp
	g++ $(CFLAGS) -o $(TARGET) *.cpp $(LDFLAGS)

# make shader targets
%.spv: %
	glslc $< -o $@

#VulkanTest: *.cpp *.hpp
#	g++ $(CFLAGS) -o VulkanTest *.cpp $(LDFLAGS)

.PHONY: test clean

test: vk.out
	DRI_PRIME=1 ./vk.out

clean:
	rm -f vk.out