
import turkey

def initialize_levels():

    stone = turkey.load_texture("./res/stone.png")

    print "add_game_obj " + str(turkey.add_game_obj(10, 10, stone))
    print "add_game_obj " + str(turkey.add_game_obj(9, 9))
    print "add_game_obj " + str(turkey.add_game_obj(8, 8))


    ## dwarf = turkey.load_texture("./res/dwarf.png")
    
    ## for x in range(5):
    ##     for y in range(5):        
    ##         #turkey.add_game_obj(x, y)
    ##         print "add_game_obj " + str(turkey.add_game_obj(x, y, dwarf))
    ## #texture = turkey.load_texture("./res/dwarf.png")

    return 0
