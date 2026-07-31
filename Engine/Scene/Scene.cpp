
#include <vector>
#include <unordered_map>
#include <optional>
#include <functional>
#include <algorithm>

namespace attributes{
    class MovementAttribute{
        private:
            std::pair<double,double> position;
            std::pair<double,double> velocity;
            std::pair<double,double> acceleration;
            void updatePosition(double dt){
                position.first += velocity.first*dt;
                position.second += velocity.second*dt;
            }
            void updateVelocity(double dt){
                velocity.first += acceleration.first*dt;
                velocity.second += acceleration.second*dt;
            }
        public:
            MovementAttribute(){
                position = {0.0,0.0};
                velocity = {0.0,0.0};
                acceleration = {0.0,0.0};
            }
            void updateMovement(std::pair<double,double> dimensions, double dt){ //do not let across the dimensions
                updateVelocity(dt);
                updatePosition(dt);
            }
            std::pair<double,double> getAcceleration() const {return acceleration;}
            std::pair<double,double> getVelocity() const {return velocity;}
            std::pair<double,double> getPosition() const {return position;}
            void setAcceleration(std::pair<double,double> a){acceleration = a;}
            void setVelocity(std::pair<double,double> v){velocity = v;}
            void setPosition(std::pair<double,double> p){position = p;}
    };
}
/*
Usage:
unsigned int npc = scene.createEntity();
npc.addMovement();
npc.getMovement();
npc.destroy();
*/
#define ITERATOR(id, vector) (std::find(vector.begin(), vector.end(), id))
#define EXISTS(id, vector) (ITERATOR(id,vector)!=vector.end())

class Scene {
    private:
        unsigned int nextID;
        std::vector<unsigned int> entitiesIDs;
        std::unordered_map<unsigned int, attributes::MovementAttribute> movements;
        std::unordered_map<unsigned int, attributes::SpriteAttribute> sprites; //textura, recorte da textura, cor, camada e visibilidade
        std::unordered_map<unsigned int, attributes::HitBoxAttribute> hitboxes; //largura, altura, offset, trigger e enabled.
        std::unordered_map<unsigned int, attributes::AnimationAttribute> animations; //frame atual, total de frames, tempo por frame, looping e playing
        std::unordered_map<unsigned int, attributes::AudioAttribute> audios; //som, volume, looping e pedido para tocar
        std::unordered_map<unsigned int, attributes::CameraAttribute> cameras; //target, offset, zoom e rotação

        std::pair<double,double> dimensions;
    public:
        Scene(){nextID = 0;}

        unsigned int createEntity(){
            unsigned int id = nextID;
            nextID++;
            entitiesIDs.push_back(id);
            return id;
        }
        bool destroyEntity(unsigned int id){
            if (EXISTS(id,entitiesIDs))
                entitiesIDs.erase(ITERATOR(id,entitiesIDs));
            else return false;
            movements.erase(id);
            return true;
        }
        void update(double dt){
            for(unsigned int id : entitiesIDs){
                auto it = movements.find(id);
                if(it!=movements.end())
                    it->second.updateMovement(dimensions, dt);
            }
        }
        bool addMovement(unsigned int id) {
            if (!EXISTS(id,entitiesIDs)) return false;
            auto[it,inserted] = movements.emplace(id, attributes::MovementAttribute{});
            return inserted;
        }
};
