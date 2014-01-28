
import turkey

def initialize_levels():

    # build some steps
    stone = turkey.load_texture("./res/stone.png")
    print "stone " + str(stone)
    for i in range(10):
        print "add_game_obj " + str(turkey.add_game_obj(i, i, stone))

    # add the character
    dwarf = turkey.load_texture("./res/dwarf.png")    
    print "add_game_obj " + str(turkey.add_character_game_obj(3, 5, dwarf))

    return 0
