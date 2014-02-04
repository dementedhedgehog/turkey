
import turkey


CELL_WIDTH = 16.0
CELL_HEIGHT = 16.0

def initialize_levels():

    try:
        print 
        
        # turn on drawing cell grids?
        turkey.debug_set_draw_grid(True);
        turkey.debug_set_draw_fps(False);

        # build some steps
        stone = turkey.load_texture("./res/stone.png")
        ## print "stone " + str(stone)
        ## for i in range(2,5):
        ##    # print "add_game_obj " + str(turkey.add_game_obj(i, i, stone))
        ##    turkey.add_game_obj(i * CELL_WIDTH, 20 * CELL_HEIGHT, stone)

        turkey.add_game_obj(4 * CELL_WIDTH, 20 * CELL_HEIGHT, stone)
        #turkey.add_game_obj(5 * CELL_WIDTH, 20 * CELL_HEIGHT, stone)

        ## for i in range(2,5):
        ##    # print "add_game_obj " + str(turkey.add_game_obj(i, i, stone))
        ##    turkey.add_game_obj(i * CELL_WIDTH, 40 * CELL_HEIGHT, stone)

        ## print "add_game_obj " + str(
        ##     turkey.add_game_obj(10 * CELL_WIDTH, 10 * CELL_HEIGHT, stone))
        ## print "add_game_obj " + str(
        ##     turkey.add_game_obj(15 * CELL_WIDTH, 15 * CELL_HEIGHT, stone))

        # add the character
        dwarf = turkey.load_texture("./res/dwarf.png")    
        print "add_game_obj " + str(turkey.add_character_game_obj(3 * CELL_WIDTH,
                                                                  15 * CELL_HEIGHT,
                                                                  dwarf))
    except Exception as err:
        print err
        turkey.quit()
        
    return 0
