/*
 * SAMPLE SCENE
 * Copyright © 2018+ Miguel Ángel Gil Martín
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 */


#include "Enemy.hpp"

namespace example {
    Enemy::Enemy(Point2f position, float speed, int life, float damage) {
        this->position = position;
        this->speed = speed;
        this->life = life;
        this->damage = damage;
        my_current_animation = LEFT;
        animation = 0;
        rate_animation = 4;
        aux_rate_animation = 0;
        visible = true;
        lived = true;
    };

    int Enemy::GetLife() { return life; };

    void Enemy::SetLife(int modifier) { life += modifier; };

    float Enemy::GetSpeed() { return speed; };

    void Enemy::SetSpeed(float modifier) { speed += modifier; };

    void Enemy::SetPosition(Point2f position) { this->position = position; };

    Size2f Enemy::GetHalfSize() { return half_size; };

    bool Enemy::SetSlices(Graphics_Context::Accessor &context) {
        if (context) {
            atlas.reset(new Atlas("enemies.sprites",context));
            left_animation[0].slice = atlas->get_slice(ID(0));
            left_animation[1].slice = atlas->get_slice(ID(1));

            right_animation[0].slice = atlas->get_slice(ID(2));
            right_animation[1].slice = atlas->get_slice(ID(3));


        }
        return (
                left_animation[0].slice->atlas->good() &&
                left_animation[1].slice->atlas->good() &&

                right_animation[0].slice->atlas->good() &&
                right_animation[1].slice->atlas->good());


    }

    void Enemy::render(basics::Canvas &canvas) {
        if (visible) {
            switch (my_current_animation) {

                case LEFT:
                    canvas.fill_rectangle(position, {left_animation[animation].slice->width * 2,
                                                     left_animation[animation].slice->height * 2},
                                          left_animation[animation].slice, 1);
                    break;
                case RIGHT:
                    canvas.fill_rectangle(position, {right_animation[animation].slice->width * 2,
                                                     right_animation[animation].slice->height * 2},
                                          right_animation[animation].slice, 1);
                    break;


            }
        }
    }

    void Enemy::animate(float time) {
        if(visible)
        {
            if (aux_rate_animation >= rate_animation) {

                if (animation > 0) {
                    animation = 0;
                } else {
                    animation++;

                }

                aux_rate_animation = 0;
            } else {
                aux_rate_animation += speed * time;
            }

        }


    }

    void Enemy::Move(Point2f meta, float time) {
        if(lived)
        {
            if (position.coordinates.x() > meta.coordinates.x()) {
                position.coordinates.x() -= speed * time;
                my_current_animation = Enemy::LEFT;
            } else {
                position.coordinates.x() += speed * time;
                my_current_animation = Enemy::RIGHT;

            }
            if (position.coordinates.y() > meta.coordinates.y()) {
                position.coordinates.y() -= speed * time;
            } else {
                position.coordinates.y() += speed * time;

            }

        }

    }

    bool Enemy::Collision(Point2f position, Size2f size) {
        if(lived)
        {
            return ((this->position.coordinates.x() >= position.coordinates.x() &&
                     this->position.coordinates.x() <= (position.coordinates.x() + size.width) &&
                     this->position.coordinates.y() >= position.coordinates.y() &&
                     this->position.coordinates.y() <= (position.coordinates.y() + size.height)) ||
                    ((this->position.coordinates.x() + this->size.width) >= position.coordinates.x() &&
                     (this->position.coordinates.x() + this->size.width) <=
                     (position.coordinates.x() + size.width) &&
                     this->position.coordinates.y() >= position.coordinates.y() &&
                     this->position.coordinates.y() <= (position.coordinates.y() + size.height)) ||
                    (this->position.coordinates.x() >= position.coordinates.x() &&
                     this->position.coordinates.x() <= (position.coordinates.x() + size.width) &&
                     (this->position.coordinates.y() + this->size.height) >= position.coordinates.y() &&
                     (this->position.coordinates.y() + this->size.height) <=
                     (position.coordinates.y() + size.height)) ||
                    ((this->position.coordinates.x() + this->size.width) >= position.coordinates.x() &&
                     (this->position.coordinates.x() + this->size.width) <=
                     (position.coordinates.x() + size.width) &&
                     (this->position.coordinates.y() + this->size.height) >= position.coordinates.y() &&
                     (this->position.coordinates.y() + this->size.height) <=
                     (position.coordinates.y() + size.height)));

        }
        return false;

    }
    float Enemy::GetDamage() {return damage;}
}
