
import turkey


CELL_WIDTH = 16
CELL_HEIGHT = 16

def initialize_levels():

    # build some steps
    stone = turkey.load_texture("./res/stone.png")
    print "stone " + str(stone)
    #for i in range(4):
    #    print "add_game_obj " + str(turkey.add_game_obj(i, i, stone))
    
    print "add_game_obj " + str(turkey.add_game_obj(10 * CELL_WIDTH, 10 * CELL_HEIGHT, stone))
    print "add_game_obj " + str(turkey.add_game_obj(15 * CELL_WIDTH, 15 * CELL_HEIGHT, stone))

    # add the character
    dwarf = turkey.load_texture("./res/dwarf.png")    
    print "add_game_obj " + str(turkey.add_character_game_obj(3 * CELL_WIDTH,
                                                              5 * CELL_HEIGHT,
                                                              dwarf))

    return 0
