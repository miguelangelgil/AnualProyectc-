/*
 * SAMPLE SCENE
 * Copyright © 2018+ Miguel Ángel Gil Martín
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 */
//#ifndef ENEMY_HEADER
//#define ENEMY_HEADER
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


    class Enemy
    {
    public:
        enum Current_animation
        {
            LEFT,
            RIGHT,
        };


        Point2f position;
        struct Animation
        {
            const Atlas::Slice * slice;


        };
        Current_animation my_current_animation;
        std::unique_ptr< Atlas > atlas;
        bool visible;
        bool lived;




    private:
        float speed = 10.f;
        Size2f size;
        Size2f half_size;
        int life = 10;


        int animation;
        float rate_animation;
        float aux_rate_animation;
        float damage;


        Animation left_animation[2];
        Animation right_animation[2];




    public:
        Enemy(Point2f position, float speed, int life, float damage);
        int GetLife();
        void SetLife (int modifier);
        float GetSpeed();
        void SetSpeed(float modifier);
        void SetPosition(Point2f position);
        Size2f GetHalfSize();
        bool SetSlices(Graphics_Context::Accessor & context);
        void render(Canvas & canvas);
        void animate(float time);
        void Move(Point2f meta, float time);
        bool Collision(Point2f position,Size2f size);
        float GetDamage();



    };

}
