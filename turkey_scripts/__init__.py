
import turkey

def initialize_levels():
    print "add_game_obj " + str(turkey.add_game_obj(10, 10))
    print "add_game_obj " + str(turkey.add_game_obj(9, 9))
    print "add_game_obj " + str(turkey.add_game_obj(8, 8))

    for x in range(30):
        for y in range(30):        
            turkey.add_game_obj(x, y)

    texture = turkey.load_texture("./res/dwarf.png")

    return 0
