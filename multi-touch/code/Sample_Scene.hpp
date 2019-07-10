/*
 * SAMPLE SCENE
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 */
#include <vector>
#include <memory>
#include <basics/Atlas>
#include <basics/Canvas>
#include <basics/Scene>
#include <basics/Timer>
#include <basics/Texture_2D>
#include "Player.hpp"
#include "Enemy.hpp"

namespace example
{
    using basics::Texture_2D;
    using basics::Timer;
    using basics::Atlas;
    using basics::Vector2f;

    class Sample_Scene : public basics::Scene
    {

        enum State
        {
            LOADING,
            READY,
            FINISHED,
            ERROR
        };



        struct Touch
        {
            int32_t id;                             ///< Un valor de ID negativo implica que el touch no está siendo usado
            float   x;
            float   y;
            const Atlas::Slice * my_slice;


        };
        std::unique_ptr< Atlas > atlas;
        struct Switches
        {
            const Atlas::Slice * slice;


        };

        struct bullet
        {
            float x,y;
            float speed = 300.F;
            float damage = 10.f;
            bool visible = true;
            const Atlas::Slice * bullet_slice;
            Vector2f direction;


        };
        std::unique_ptr< Atlas > bullet_atlas;

        std::unique_ptr< Atlas > atlas_enemy;
        std::vector< Enemy *> enemies;




    private:

        State    state;
        bool     suspended;                         ///< true cuando la aplicación está en segundo plano
        unsigned canvas_width;                      ///< Resolución virtual del display
        unsigned canvas_height;
        float    square_size;
        float    half_size;
        Switches my_switches[3];

        Touch    touches[2];                       ///< Se gestionan hasta 2 touches simultáneos
        Timer          timer;                               ///< Cronómetro usado para medir intervalos de tiempo
        bool    touch_initial;
        bool    touch_to_fire;
        std::unique_ptr< Atlas > floor;
        std::vector<bullet> bullets;
        float cadence;
        float aux_cadence=0;

        float cadence_spawn_enemy;
        float aux_cadence_spawn_enemy=0;




        struct initialTouch
        {
            float x,y;
        };

    public:

        Player * my_player;
        Sample_Scene();
        //Player player;
        initialTouch move;
        initialTouch fire;

        basics::Size2u get_view_size () override
        {
            return { canvas_width, canvas_height };
        }

        bool initialize () override;
        void suspend () override;
        void resume () override;

        void handle (basics::Event & event) override;
        void render (basics::Graphics_Context::Accessor & context) override;
        void update (float time) override;
        void load_textures();
        void run_simulation(float time);
        void spawn_enemies(float time);

    };

}
