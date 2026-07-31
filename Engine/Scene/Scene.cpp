
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
            void updateMovement(std::pair<int,int> dimensions, double dt){ //do not let across the dimensions
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

class Scene {
    private:
        std::vector<unsigned int> entitiesIDs;
        std::unordered_map<unsigned int, attributes::MovementAttribute> movements;
        std::pair<double,double> dimensions;
    public:
        unsigned int createEntity(){
            unsigned int id = entitiesIDs.size();
            entitiesIDs.push_back(id);
            return id;
        }
        bool destroyEntity(unsigned int id){
            auto it = std::find(entitiesIDs.begin(), entitiesIDs.end(), id);
            if (it != entitiesIDs.end())
                entitiesIDs.erase(it);
            movements.erase(id);
        }
        void update(double dt){
            for(unsigned int id : entitiesIDs){
                auto it = movements.find(id);
                if(it!=movements.end())
                    it->second.updateMovement(dimensions, dt);
            }
        }
        bool addMovement(unsigned int id) {
            if (id >= entitiesIDs.size()) return false;
            auto[it,inserted] = movements.emplace(id, attributes::MovementAttribute{});
            return inserted;
        }
};
