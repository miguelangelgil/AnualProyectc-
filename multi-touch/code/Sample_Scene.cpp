/*
 * SAMPLE SCENE
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 */

#include "Sample_Scene.hpp"
#include "Menu_Scene.hpp"
#include <basics/Accelerometer>
#include <basics/Canvas>
#include <basics/Director>

using namespace basics;
using namespace std;

namespace example
{
    Sample_Scene::Sample_Scene()
    {
        canvas_width  = 1280;                   // Todavía no se puede calcular el aspect ratio, por lo que se establece
        canvas_height =  720;                   // un tamaño por defecto hasta poder calcular el tamaño final
        square_size   =  100;
        half_size     =   50;
        state         = LOADING;
        my_player = new Player({canvas_width/2,canvas_height/2},100.f,100);


    }

    bool Sample_Scene::initialize () {
        suspended = false;
        state = LOADING;
        for (auto &touch : touches) touch.id = -1;
        cadence = 0.2f;
        cadence_spawn_enemy = 2;
       /* floor tile;
        tile.x = canvas_width / 2;
        tile.y = canvas_height / 2;
        tile.x_size = canvas_width * 4;
        tile.y_size = canvas_height * 4;
        tile.x_half_size = tile.x_size / 2;
        tile.y_half_size = tile.y_size / 2;
        listfloor.push_back(tile);*/


        return true;

    }
    void Sample_Scene::suspend()
    {
        suspended = true;
    }
    void Sample_Scene::resume()
    {
        suspended = false;
    }
    void Sample_Scene::update (float time)
    {
        if(!suspended) switch(state)
        {
            case LOADING: load_textures(); break;
            case READY: run_simulation(time); break;
            case FINISHED: break;
            case ERROR: break;


        }
    }




    void Sample_Scene::handle (basics::Event & event)
    {
        if(state == READY) {
            if (!suspended)
                switch (event.id) {
                    case ID(touch-started):                 // El usuario toca la pantalla
                    {
                        // Se busca un objeto touch disponible para ser vinculado al evento que se inicia:
                        ///los touch se dividen dependiendo de en que lado de la pantalla toques
                        if (*event[ID(x)].as<var::Float>() < canvas_width / 2) {
                            touches[0].id = *event[ID(id)].as<var::Int32>();
                            move.x = touches[0].x = *event[ID(x)].as<var::Float>();
                            move.y = touches[0].y = *event[ID(y)].as<var::Float>();
                            touch_initial = true;


                        }
                        if (*event[ID(x)].as<var::Float>() > canvas_width / 2) {
                            touches[1].id = *event[ID(id)].as<var::Int32>();
                            fire.x = touches[1].x = *event[ID(x)].as<var::Float>();
                            fire.y = touches[1].y = *event[ID(y)].as<var::Float>();
                            touch_to_fire = true;


                        }


                        break;
                    }

                    case ID(touch-moved):                   // El usuario arrastra el dedo/puntero por la pantalla
                    {
                        // Se busca el touch que se ha movido y se actualiza su posición:

                        int32_t id = *event[ID(id)].as<var::Int32>();

                        for (auto &touch : touches) {
                            if (touch.id == 1 && *event[ID(x)].as<var::Float>() > canvas_width / 2)
                            {
                                touch.x = *event[ID(x)].as<var::Float>();
                                touch.y = *event[ID(y)].as<var::Float>();


                            }
                            if (touch.id == 0 && *event[ID(x)].as<var::Float>() < canvas_width / 2)
                            {
                                touch.x = *event[ID(x)].as<var::Float>();
                                touch.y = *event[ID(y)].as<var::Float>();



                            }

                        }

                        break;
                    }

                    case ID(touch-ended):                   // El usuario deja de tocar la pantalla
                    {
                        // Se busca el touch que se ha soltado y se desactiva:

                        int32_t id = *event[ID(id)].as<var::Int32>();
                        for(int i=0;i < 2;i++)
                        {
                            if(i == 0 && touches[i].id == id)
                            {
                                touch_initial = false;
                            }
                            if(i == 1 && touches[i].id == id)
                            {
                                touch_to_fire = false;
                            }

                        }



                        break;

                    }
                }
            }

        }



    void Sample_Scene::render (basics::Graphics_Context::Accessor & context)
    {
        if (!suspended)
        {
            Canvas * canvas = context->get_renderer< Canvas > (ID(canvas));

            if (!canvas)
            {
                canvas = Canvas::create (ID(canvas), context, {{ canvas_width, canvas_height }});
            }

            if (canvas)
            {

                canvas->clear ();

                //canvas->set_color(1,1,1);
                //canvas->fill_rectangle({tile.x-tile.x_half_size,tile.y-tile.y_half_size},{tile.x_size,tile.y_size});


                for(auto &bullet:bullets)
                {
                    if(bullet.visible)
                        canvas->fill_rectangle({bullet.x,bullet.y},{bullet.bullet_slice->width*2,bullet.bullet_slice->height*2},bullet.bullet_slice,1);

                }

                for(auto &enemy:enemies)
                {
                    enemy->render(*canvas);

                }
                my_player->render(*canvas);
                if(touch_initial)
                {
                    canvas->fill_rectangle ({ move.x - my_switches[0].slice->width/2, move.y - my_switches[0].slice->height/2 }, {my_switches[0].slice->width, my_switches[0].slice->height },my_switches[0].slice);
                }
                if(touch_to_fire)
                {
                    canvas->fill_rectangle ({ fire.x - my_switches[0].slice->width/2, fire.y - my_switches[0].slice->height/2 }, {my_switches[0].slice->width, my_switches[0].slice->height },my_switches[0].slice);
                }
                for (auto & touch : touches)
                {
                    if (touch.id >= 0)
                    {

                        canvas->fill_rectangle ({ touch.x - touch.my_slice -> width/2, touch.y - touch.my_slice->height/2 }, {touch.my_slice->width,touch.my_slice->height },touch.my_slice);

                    }
                }





            }
        }
    }
    void Sample_Scene::load_textures()
    {
        Graphics_Context::Accessor context = director.lock_graphics_context();
        if(context)
        {
            ///se cargan las texturas de los swiches
            atlas.reset(new Atlas("switches.sprites",context));
            my_switches[0].slice = atlas->get_slice(ID(0));
            my_switches[1].slice = atlas->get_slice(ID(r));
            my_switches[2].slice = atlas->get_slice(ID(l));
            touches[0].my_slice = my_switches[2].slice;
            touches[1].my_slice = my_switches[1].slice;
            ///se cargan las texturas de las balas
            bullet_atlas.reset(new Atlas("bullets.sprites",context));
            ///cargo el atlas y los slices , tambien ajusto loa animacion con la que va ha empezar del player
            my_player->my_current_animation = Player::RIGHT;
            state = my_player->SetSlices(context)? READY : ERROR;


        }

    }
    void Sample_Scene::run_simulation(float time)
    {
        if (touch_initial) {
            if (move.x < touches[0].x + 10)  {
                if(my_player->position.coordinates.x() < canvas_width)
                    my_player->position.coordinates.x() += my_player->GetSpeed() * time;
                my_player->animate(time);
            }
            if (move.x > touches[0].x - 10) {
                if(my_player->position.coordinates.x() > 0)
                    my_player->position.coordinates.x() -= my_player->GetSpeed() * time;
                my_player->animate(time);
            }
            if (move.y < touches[0].y + 10) {
                if(my_player->position.coordinates.y() < canvas_height)
                    my_player->position.coordinates.y() += my_player->GetSpeed() * time;
                my_player->animate(time);
            }
            if (move.y > touches[0].y - 10) {
                if(my_player->position.coordinates.y() > 0)
                    my_player->position.coordinates.y() -= my_player->GetSpeed() * time;
                my_player->animate(time);
            }



        } else {
           //move.x = touches[0].x;
           //move.y = touches[0].y;
        }
        if(aux_cadence <= cadence)
        {
            aux_cadence += time;
        }else
        {
            aux_cadence = 0;
        }
        if (touch_to_fire) {
            if (aux_cadence >= cadence) {
                if (fire.y + 10 < touches[1].y &&
                    abs(abs(fire.y) - abs(touches[1].y)) > abs(abs(fire.x) - abs(touches[1].x))) {
                    bullet my_bullet;
                    my_bullet.x = my_player->position.coordinates.x();
                    my_bullet.y = my_player->position.coordinates.y();
                    my_bullet.bullet_slice = bullet_atlas->get_slice(ID(0));
                    my_bullet.direction = {0, 1};
                    bullets.push_back(my_bullet);
                }
                if (fire.y - 10 > touches[1].y &&
                    abs(abs(fire.y) - abs(touches[1].y)) > abs(abs(fire.x) - abs(touches[1].x))) {
                    bullet my_bullet;
                    my_bullet.x = my_player->position.coordinates.x();
                    my_bullet.y = my_player->position.coordinates.y();
                    my_bullet.bullet_slice = bullet_atlas->get_slice(ID(1));
                    my_bullet.direction = {0, -1};
                    bullets.push_back(my_bullet);
                }
                if (fire.x + 10 < touches[1].x &&
                    abs(abs(fire.x) - abs(touches[1].x)) > abs(abs(fire.y) - abs(touches[1].y))) {
                    bullet my_bullet;
                    my_bullet.x = my_player->position.coordinates.x();
                    my_bullet.y = my_player->position.coordinates.y();
                    my_bullet.bullet_slice = bullet_atlas->get_slice(ID(2));
                    my_bullet.direction = {1, 0};
                    bullets.push_back(my_bullet);
                }
                if (fire.x - 10 > touches[1].x &&
                    abs(abs(fire.x) - abs(touches[1].x)) > abs(abs(fire.y) - abs(touches[1].y))) {
                    bullet my_bullet;
                    my_bullet.x = my_player->position.coordinates.x();
                    my_bullet.y = my_player->position.coordinates.y();
                    my_bullet.bullet_slice = bullet_atlas->get_slice(ID(3));
                    my_bullet.direction = {-1, 0};
                    bullets.push_back(my_bullet);
                }


            }


            if (fire.y + 10 < touches[1].y &&
                abs(abs(fire.y) - abs(touches[1].y)) > abs(abs(fire.x) - abs(touches[1].x))) {
                my_player->my_current_animation = Player::BACK;

            }

            if (fire.y - 10 > touches[1].y &&
                abs(abs(fire.y) - abs(touches[1].y)) > abs(abs(fire.x) - abs(touches[1].x))) {
                my_player->my_current_animation = Player::FRONT;

            }

            if (fire.x + 10 < touches[1].x &&
                abs(abs(fire.x) - abs(touches[1].x)) > abs(abs(fire.y) - abs(touches[1].y))) {
                my_player->my_current_animation = Player::RIGHT;

            }

            if (fire.x - 10 > touches[1].x &&
                abs(abs(fire.x) - abs(touches[1].x)) > abs(abs(fire.y) - abs(touches[1].y))) {
                my_player->my_current_animation = Player::LEFT;
            }

        }

        ///actualizo la posición de las balas
        for(auto &bullet:bullets) {
            if(bullet.visible)
            {
                bullet.x += bullet.direction.coordinates.x() * bullet.speed * time;
                bullet.y += bullet.direction.coordinates.y() * bullet.speed * time;

            }

            if (bullet.x > canvas_width || bullet.x < 0 || bullet.y > canvas_height || bullet.y < 0){bullet.visible = false;}
                //delete(bullet);

        }
        spawn_enemies(time);
        if(my_player->GetLife() <= 0)
        {
            director.run_scene (shared_ptr< Scene >(new Menu_Scene));
        }

        for(int i = 0;i < enemies.size();i++)
        {
            enemies[i]->Move(my_player->position, time);
            enemies[i]->animate(time);
            if(enemies[i]->Collision(my_player->position,my_player->GetSize()))
            {if(enemies[i]->lived){my_player -> SetLife(-(enemies[i]->GetDamage()));enemies[i]->lived = false; enemies[i]->visible = false;}};
            for(int j = 0; j < bullets.size();j++)
            {
                if(enemies[i]->Collision({bullets[j].x,bullets[j].y},{bullets[j].bullet_slice->width,bullets[j].bullet_slice->height})){if(bullets[j].visible)enemies[i]->SetLife(-(bullets[j].damage)); bullets[j].visible = false;bullets.erase(bullets.begin()+ j); }
            }
            if(enemies[i]->GetLife()<=0){enemies[i]->visible = false; enemies[i]->lived = false; enemies.erase(enemies.begin()+i);}
        }
    }
    void Sample_Scene::spawn_enemies(float time)
    {
        Graphics_Context::Accessor context = director.lock_graphics_context();

        if(aux_cadence_spawn_enemy <= cadence_spawn_enemy)
        {
            aux_cadence_spawn_enemy += time;
        }else
        {
            aux_cadence_spawn_enemy = 0;

        }
        if(aux_cadence_spawn_enemy >= cadence_spawn_enemy)
        {
            int i = rand() % 4 ;
            Enemy * my_enemy;

            switch(i)
            {
                case 0:
                    my_enemy = new Enemy({canvas_width, rand() % canvas_height}, rand() % 90 + 20, 10, 20);
                    break;
                case 1:
                    my_enemy = new Enemy({ rand() % canvas_width, canvas_height},rand() % 90 + 20, 10, 20);
                    break;
                case 2:
                    my_enemy = new Enemy({ rand() % canvas_width, 0}, rand() % 90 + 20, 10, 20);
                    break;
                case 3:
                    my_enemy = new Enemy({0,  rand() % canvas_height},rand() % 90 + 20, 10, 20);
                    break;

            }

            if(context)
                my_enemy->SetSlices(context);
            enemies.push_back(my_enemy);
        }

    }


}
