/*
 * SAMPLE SCENE
 * Copyright © 2018+ Miguel Ángel Gil Martín
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 */
//#ifndef PLAYER_HEADER
//#define PLAYER_HEADER
#include <basics/Atlas>
#include <basics/Canvas>
#include <basics/Vector>

namespace example
{
    using basics::Atlas;
    using basics::Canvas;
    using basics::Size2f;
    using basics::Point2f;
    using basics::Graphics_Context;


    class Player
    {
    public:
        enum Current_animation
        {
            FRONT,
            LEFT,
            RIGHT,
            BACK,

        };


        Point2f position;
        std::unique_ptr< Atlas > atlas;
        struct Animation
        {
            const Atlas::Slice * slice;


        };
        Current_animation my_current_animation;



    private:
        float speed = 10.f;
        Size2f size;
        Size2f half_size;
        int life = 100;
        bool visible;

        int animation;
        float rate_animation;
        float aux_rate_animation;


        Animation front_animation[3];
        Animation left_animation[3];
        Animation right_animation[3];
        Animation back_animation[3];




    public:
        Player(Point2f position, float speed, int life);
        int GetLife();
        void SetLife (int modifier);
        float GetSpeed();
        void SetSpeed(float modifier);
        void SetPosition(Point2f position);
        Size2f GetHalfSize();
        bool SetSlices(Graphics_Context::Accessor & context);
        void render(Canvas & canvas);
        void animate(float time);



    };

}
