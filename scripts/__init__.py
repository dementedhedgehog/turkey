
import turkey


CELL_WIDTH = 16.0
CELL_HEIGHT = 16.0

def initialize_levels():

    #
    # Debug Information Config
    #

    # turn on drawing cell grids?
    turkey.debug_set_draw_grid(False);
    turkey.debug_set_draw_fps(False);
    
    #
    # Physics Config
    #

    # y axis (+ve down)
    turkey.set_default_gravity(0.6);
    turkey.set_default_jump_velocity(8.0);

    # velocity caps
    turkey.set_default_max_x_velocity(5.0);
    turkey.set_default_max_y_velocity(10.0);

    # delta x when you hit the left/right controls
    turkey.set_default_x_acceleration(0.55)
    turkey.set_default_x_deceleration(0.80)
    

    #
    # Sound
    # (normally we won't play it from here.. we'll set up triggers to play it
    #  but we'll do it like this for now as an aid to prototyping).
    #
    music = turkey.load_music("./res/spirit_temple.ogg")
    turkey.play_music(music)

    #
    # Setup the static game objects
    #

    # build some steps
    stone = turkey.load_texture("./res/stone.png")
    turkey.add_game_obj(0.0, 0.0, stone)

    for i in range(2, 12):
        turkey.add_game_obj(i * CELL_WIDTH, 20 * CELL_HEIGHT, stone)

    for i in range(15, 32):
        turkey.add_game_obj(i * CELL_WIDTH, 20 * CELL_HEIGHT, stone)

    turkey.add_game_obj(28 * CELL_WIDTH, 19 * CELL_HEIGHT, stone)
    turkey.add_game_obj(27 * CELL_WIDTH, 19 * CELL_HEIGHT, stone)

    for i in range(2,35):
        turkey.add_game_obj(i * CELL_WIDTH, 40 * CELL_HEIGHT, stone)


    #
    # Setup the main character
    # (all this will use sprites instead of static textures at some time in the future).
    #
    
    # add the character
    dwarf = turkey.load_texture("./res/dwarf.png")
    print "add_game_obj " + str(turkey.add_character_game_obj(3 * CELL_WIDTH,
                                                              15 * CELL_HEIGHT,
                                                              dwarf))

    star = turkey.load_texture("./res/star.png")
    print "add_particle_system " + str(turkey.add_particle_system(star))


    #
    # Set up the scenery..
    #

    # the background image
    background = turkey.load_texture("./res/background.jpg")
    turkey.add_scenery(0.3, 0, 0, background)

    # some trees
    tree = turkey.load_texture("./res/tree.png")
    turkey.add_scenery(0.7, 100, 78, tree)
    turkey.add_scenery(0.5999, 240, 52, tree)
    turkey.add_scenery(0.6, 240, 82, tree)

    # a foreground plant
    plant = turkey.load_texture("./res/plant.png")
    turkey.add_scenery(1.2, 120, 250, plant)

    # except Exception as err:
    #     print err
    #     turkey.quit()
        
    return 0
