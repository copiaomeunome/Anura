#include "./Scene/Scene.cpp"

class Engine{
    private:
        std::vector<Scene> scenes;
    public:
        Scene& createScene(){
            Scene scene{};
            scenes.push_back(scene);
            return scene;
        }
};