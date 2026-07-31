
#include <vector>
#include <unordered_map>

namespace scene{
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

class Scene {
    private:
        std::vector<unsigned int> entitiesIDs;
        std::unordered_map<unsigned int,scene::MovementAttribute> movements;
        std::pair<double,double> dimensions;
    public:
        void update(double dt){
            for(unsigned int id : entitiesIDs){
                auto it = movements.find(id);
                if(it!=movements.end())
                    it->second.updateMovement(dimensions, dt);
            }
        }

};