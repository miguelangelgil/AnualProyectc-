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

namespace example
{
    using basics::Texture_2D;
    using basics::Timer;
    using basics::Atlas;

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

    private:

        State    state;
        bool     suspended;                         ///< true cuando la aplicación está en segundo plano
        unsigned canvas_width;                      ///< Resolución virtual del display
        unsigned canvas_height;
        float    square_size;
        float    half_size;
        Switches my_switches[3];

        Touch    touches[2];                       ///< Se gestionan hasta 10 touches simultáneos
        Timer          timer;                               ///< Cronómetro usado para medir intervalos de tiempo
        bool    touch_initial;
        bool    touch_to_fire;
        Texture_2D * texture;




        struct floor
        {
            float x,y;
            float x_size;
            float y_size;
            float x_half_size;
            float y_half_size;


        };
        struct bullet
        {
            float x,y;
            float speed = 20.F;

        };
        std::vector<bullet> bullets;
        std::vector<floor> listfloor;
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

    };

}
