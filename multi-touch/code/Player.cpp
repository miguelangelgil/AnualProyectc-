/*
 * SAMPLE SCENE
 * Copyright © 2018+ Miguel Ángel Gil Martín
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 */


#include "Player.hpp"

namespace example
{
    Player::Player(Point2f position, float speed, int life)
    {
        this->position = position;
        this->speed = speed;
        this->life = life;
        animation=0;
        rate_animation = 4;
        aux_rate_animation=0;
        visible = true;
    };

    int Player::GetLife(){return life;};
    void Player::SetLife(int modifier) {life += modifier;};
    float Player::GetSpeed() {return speed;};
    void Player::SetSpeed(float modifier) {speed += modifier;};
    void Player::SetPosition(Point2f position){this->position = position;};
    Size2f Player::GetHalfSize(){return half_size;};
    bool Player::SetSlices(Graphics_Context::Accessor & context)
    {
        if(context)
        {
            atlas.reset(new Atlas("players.sprites",context));
            front_animation[0].slice = atlas->get_slice(ID(hairless_front1));
            front_animation[1].slice = atlas->get_slice(ID(hairless_front2));
            front_animation[2].slice = atlas->get_slice(ID(hairless_front3));
            left_animation[0].slice = atlas->get_slice(ID(hairless_left1));
            left_animation[1].slice = atlas->get_slice(ID(hairless_left2));
            left_animation[2].slice = atlas->get_slice(ID(hairless_left3));
            right_animation[0].slice = atlas->get_slice(ID(hairless_right1));
            right_animation[1].slice = atlas->get_slice(ID(hairless_right2));
            right_animation[2].slice = atlas->get_slice(ID(hairless_right3));
            back_animation[0].slice = atlas ->get_slice(ID(hairless_back1));
            back_animation[1].slice = atlas ->get_slice(ID(hairless_back2));
            back_animation[2].slice = atlas ->get_slice(ID(hairless_back3));


        }
        return (front_animation[0].slice->atlas->good()&&
                front_animation[1].slice->atlas->good()&&
                front_animation[2].slice->atlas->good()&&
                left_animation[0].slice->atlas->good()&&
                left_animation[1].slice->atlas->good()&&
                left_animation[2].slice->atlas->good()&&
                right_animation[0].slice->atlas->good()&&
                right_animation[1].slice->atlas->good()&&
                right_animation[2].slice->atlas->good()&&
                back_animation[0].slice->atlas->good()&&
                back_animation[1].slice->atlas->good()&&
                back_animation[2].slice->atlas->good());


    }
    void Player::render(basics::Canvas &canvas)
    {
        if(visible)
        {
            switch (my_current_animation)
            {
                case FRONT: canvas.fill_rectangle(position,{front_animation[animation].slice->width*2,front_animation[animation].slice->height*2},front_animation[animation].slice,1); break;
                case LEFT: canvas.fill_rectangle(position,{left_animation[animation].slice->width*2,left_animation[animation].slice->height*2},left_animation[animation].slice,1); break;
                case RIGHT: canvas.fill_rectangle(position,{right_animation[animation].slice->width*2,right_animation[animation].slice->height*2},right_animation[animation].slice,1); break;
                case BACK: canvas.fill_rectangle(position,{back_animation[animation].slice->width*2,back_animation[animation].slice->height*2},back_animation[animation].slice,1); break;

            }
        }
    }
    void Player::animate(float time)
    {

        if(aux_rate_animation >= rate_animation)
        {

            if(animation > 1)
            {
                animation=0;
            }else
            {
                animation++;

            }

            aux_rate_animation=0;
        }else
        {
            aux_rate_animation += speed * time;
        }

    }

}
