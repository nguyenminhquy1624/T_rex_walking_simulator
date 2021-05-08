case SDLK_UP:
        {
            player.speed_y = 20;
            while(player.speed_y > 0 && quit == false)
            {
                quit = check_collision(player, k);
                player.y -= player.speed_y;
                player.speed_y -= 2;
                gBackgroundTexture.render(0,0,NULL);
                rng(k,enemy);
                player.animation( 7);
                SDL_Delay(50);
                cout<<player.x<<" "<<player.y<<endl;

            }
            while(player.y < 470 && quit == false)
            {
                quit = check_collision(player, k);
                player.speed_y += 2;
                player.y += player.speed_y;
                gBackgroundTexture.render(0,0,NULL);
                rng(k,enemy);
                player.animation( 7);
                SDL_Delay(50);
                cout<<player.x<<" "<<player.y<<endl;
            }
            break;
        }
