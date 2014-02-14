
import turkey


CELL_WIDTH = 16.0
CELL_HEIGHT = 16.0

def initialize_levels():

    #try:
    print 

    # # turn on drawing cell grids?
    # turkey.debug_set_draw_grid(False);
    # turkey.debug_set_draw_fps(False);

    
    for x in dir(turkey):
        print str(x)


    print "X"
    #print turkey._VIEW

    # build some steps
    print ""
    stone = turkey.load_texture("./res/stone.png")
    print "xsdfsdf" + str(stone)

    print "xxvxcv"
    turkey.add_game_obj(0.0, 0.0, stone)


    print "xxvxcvcvdv"
    for i in range(2, 12):
        print "xxvxcvcvdv" + str(i)
        turkey.add_game_obj(i * CELL_WIDTH, 20 * CELL_HEIGHT, stone)


    for i in range(15, 32):
        # print "add_game_obj " + str(turkey.add_game_obj(i, i, stone))
        turkey.add_game_obj(i * CELL_WIDTH, 20 * CELL_HEIGHT, stone)

    turkey.add_game_obj(28 * CELL_WIDTH, 19 * CELL_HEIGHT, stone)
    turkey.add_game_obj(27 * CELL_WIDTH, 19 * CELL_HEIGHT, stone)

    #turkey.add_game_obj(4 * CELL_WIDTH, 20 * CELL_HEIGHT, stone)
    #turkey.add_game_obj(5 * CELL_WIDTH, 20 * CELL_HEIGHT, stone)

    for i in range(2,35):
        # print "add_game_obj " + str(turkey.add_game_obj(i, i, stone))
        turkey.add_game_obj(i * CELL_WIDTH, 40 * CELL_HEIGHT, stone)

    ## print "add_game_obj " + str(
    ##     turkey.add_game_obj(10 * CELL_WIDTH, 10 * CELL_HEIGHT, stone))
    ## print "add_game_obj " + str(
    ##     turkey.add_game_obj(15 * CELL_WIDTH, 15 * CELL_HEIGHT, stone))

    # add the character
    dwarf = turkey.load_texture("./res/dwarf.png")
    print "add_game_obj " + str(turkey.add_character_game_obj(3 * CELL_WIDTH,
                                                              15 * CELL_HEIGHT,
                                                              dwarf))

    star = turkey.load_texture("./res/star.png")
    #print "add_particle_system " + str(turkey.add_particle_system(star))
    print "add_particle_system " + str(turkey.add_particle_system(dwarf))


    background = turkey.load_texture("./res/background.jpg")
    turkey.add_scenery(0.3, 0, 0, background)

    tree = turkey.load_texture("./res/tree.png")
    turkey.add_scenery(0.7, 100, 78, tree)

    turkey.add_scenery(0.5999, 240, 52, tree)
    turkey.add_scenery(0.6, 240, 82, tree)

    plant = turkey.load_texture("./res/plant.png")
    turkey.add_scenery(1.2, 120, 250, plant)

    # except Exception as err:
    #     print err
    #     turkey.quit()
        
    return 0
