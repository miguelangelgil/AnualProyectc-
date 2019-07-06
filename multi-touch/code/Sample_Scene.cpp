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
#include <basics/Accelerometer>
#include <basics/Canvas>
#include <basics/Director>

using namespace basics;
using namespace std;

namespace example
{
    int listfloor_size=0;


    Sample_Scene::Sample_Scene()
    :
        brushes
        {
            { true, { 0xff,    0,    0 } },
            { true, {    0, 0xff,    0 } }

        }
    {
        canvas_width  = 1280;                   // Todavía no se puede calcular el aspect ratio, por lo que se establece
        canvas_height =  720;                   // un tamaño por defecto hasta poder calcular el tamaño final
        square_size   =  100;
        half_size     =   50;
        state         = LOADING;
        my_player = new Player({canvas_width/2,canvas_height/2},30.f,100);

    }

    bool Sample_Scene::initialize () {
        suspended = false;
        state = LOADING;
        for (auto &touch : touches) touch.id = -1;
        for (auto &brush : brushes) brush.free = true;

        //player.xfloor = 0;
        //player.yfloor = 0;
        floor tile;
        tile.x = canvas_width / 2;
        tile.y = canvas_height / 2;
        tile.x_size = canvas_width * 4;
        tile.y_size = canvas_height * 4;
        tile.x_half_size = tile.x_size / 2;
        tile.y_half_size = tile.y_size / 2;
        listfloor.push_back(tile);


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
                        if (*event[ID(x)].as<var::Float>() < canvas_width / 2) {
                            touches[0].id = *event[ID(id)].as<var::Int32>();
                            move.x = touches[0].x = *event[ID(x)].as<var::Float>();
                            move.y = touches[0].y = *event[ID(y)].as<var::Float>();
                            touches[0].brush = &brushes[0];
                            brushes[0].free = false;
                            touch_initial = true;

                        }
                        if (*event[ID(x)].as<var::Float>() > canvas_width / 2) {
                            touches[1].id = *event[ID(id)].as<var::Int32>();
                            fire.x = touches[1].x = *event[ID(x)].as<var::Float>();
                            fire.y = touches[1].y = *event[ID(y)].as<var::Float>();
                            touches[1].brush = &brushes[1];
                            brushes[1].free = false;
                            touch_to_fire = true;

                        }


                        break;
                    }

                    case ID(touch-moved):                   // El usuario arrastra el dedo/puntero por la pantalla
                    {
                        // Se busca el touch que se ha movido y se actualiza su posición:

                        int32_t id = *event[ID(id)].as<var::Int32>();

                        for (auto &touch : touches) {
                            if (touch.id == id) {
                                touch.x = *event[ID(x)].as<var::Float>();
                                touch.y = *event[ID(y)].as<var::Float>();

                                break;
                            }
                        }

                        break;
                    }

                    case ID(touch-ended):                   // El usuario deja de tocar la pantalla
                    {
                        // Se busca el touch que se ha soltado y se desactiva:

                        int32_t id = *event[ID(id)].as<var::Int32>();

                        for (auto &touch : touches) {
                            if (touch.id == id) {
                                touch.brush->free = true;
                                touch.id = -1;

                                break;
                            }
                        }
                        touch_initial = false;
                        touch_to_fire = false;

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


                bool color = true;
                /*for(auto & tile : listfloor)
                {
                    if(color)
                    {
                        canvas->set_color(1,0,1);
                        color = false;
                    }else
                    {
                        canvas->set_color(1,1,1);
                        color = true;

                    }

                }*/

                floor tile = listfloor[0];

                canvas->set_color(1,1,1);
                canvas->fill_rectangle({tile.x-tile.x_half_size,tile.y-tile.y_half_size},{tile.x_size,tile.y_size});

                canvas->set_color      (0, 0, 1);
                //canvas->fill_rectangle({my_player->x - my_player->GetHalfSize() ,my_player->y - my_player->GetHalfSize()},{90,90});
                //canvas->fill_rectangle({tile.x-tile.x_half_size,tile.y-tile.y_half_size},{tile.x_size,tile.y_size});


                for (auto & touch : touches)
                {
                    if (touch.id >= 0)
                    {
                        Color & color = touch.brush->color;

                        float r = float(color.components[0]) * (1.f / 255.f);
                        float g = float(color.components[1]) * (1.f / 255.f);
                        float b = float(color.components[2]) * (1.f / 255.f);

                        canvas->set_color      (r, g, b);
                        canvas->fill_rectangle ({ touch.x - half_size, touch.y - half_size }, { square_size, square_size });
                    }
                }

                my_player->render(*canvas);
                if(touch_initial)
                {
                    canvas->set_color      (0, 0, 0);
                    canvas->fill_rectangle ({ move.x - half_size, move.y - half_size }, { square_size, square_size });
                    //virtual void fill_rectangle  (const Point2f & where, const Size2f & size, const Texture_2D   * texture, int handling = CENTER) { }

                }





            }
        }
    }
    void Sample_Scene::load_textures()
    {
        Graphics_Context::Accessor context = director.lock_graphics_context();
        if(context)
        {
            //cargo el atlas y los slices , tambien ajusto loa animacion con la que va ha empezar
            my_player->my_current_animation = Player::RIGHT;
            state = my_player->SetSlices(context)? READY : ERROR;


        }

    }
    void Sample_Scene::run_simulation(float time)
    {
        if (touch_initial) {
            if (move.x < touches[0].x + 10)  {
                my_player->position.coordinates.x() += my_player->GetSpeed() * time;
                my_player->animate(time);
            }
            if (move.x > touches[0].x - 10) {
                my_player->position.coordinates.x() -= my_player->GetSpeed() * time;
                my_player->animate(time);
            }
            if (move.y < touches[0].y + 10) {
                my_player->position.coordinates.y() += my_player->GetSpeed() * time;
                my_player->animate(time);
            }
            if (move.y > touches[0].y - 10) {
                my_player->position.coordinates.y() -= my_player->GetSpeed() * time;
                my_player->animate(time);
            }

            for (auto &tile:listfloor) {
                //tile.x += player.xfloor;
                //tile.y += player.yfloor;
            }

        } else {
            move.x = touches[0].x;
            move.y = touches[0].y;
        }
        if (touch_to_fire) {
            bullet my_bullet;
            my_bullet.x = my_player->position.coordinates.x();
            my_bullet.y = my_player->position.coordinates.y();

            if (fire.x < touches[1].x + 10)
            {
                my_player->my_current_animation = Player::RIGHT;
            }

            if (fire.x > touches[1].x - 10)
            {
                my_player->my_current_animation = Player::LEFT;
            }

            if (fire.y < touches[1].y + 10)
            {
                my_player->my_current_animation = Player::BACK;
            }

            if (fire.y > touches[1].y - 10)
            {
                my_player->my_current_animation = Player::FRONT;
            }


        }else
            {
                fire.x = touches[1].x;
                fire.y = touches[1].y;

            }

    }

}
