""" file to handle all the movements of the slave. """

from mae_generator.mae import OutState, MAE, MAEState
from pathfinding.pathfinding import PathFinder
from utils.coord import Coord
from com_state_factory import ComStateFactory


class SlaveManager:
    def __init__(self, com_state_factory):
        self.pathfinder = PathFinder()
        self.pathfinder.add_circle(1500, 1050, 200)
        self.state_factory = com_state_factory
        self.current_position = Coord()
        self.movement_mae = None

    def go_to_direct_from(self, coords_from,  coords_to, precision="NEAR"):
        """ find the best way to go to the way-point given by coords.
        without using the pathfinder. 
        return a movement state machine
        """

        states = [] 
        #TODO chose different options
        
        states.append( self.state_factory.get_bf_droite(coords_to)) 
        states.append(OutState())
        states[-2].add_transition(precision, states[-1])
        return MAE(states)

    def go_to_pathfinder(self, coords):
        """complete movement using the pahtfinder to find the 
         best waypoints"""

        sub_states = []
        self.pathfinder.find_waypoints(
            self.current_position.to_tuple(), 
            coords.to_tuple())
        waypoints = self.pathfinder.get_smooth_waypoints()
        print waypoints
        print len(waypoints)
        for i, w in enumerate(waypoints[:-1]):
            if i == len(waypoints) - 2:
                # for the last part of the movement, we want to be precise
                sub_states.append(MAEState(
                    self.go_to_direct_from(w, waypoints[i + 1], "AFINI"),
                    name="final go to {}".format(waypoints[i + 1])))
            else:
                sub_states.append(MAEState(
                    self.go_to_direct_from(w, waypoints[i + 1]),
                    name="go to {}".format(waypoints[i + 1])))

            if i > 0:
                sub_states[-2].add_out_transition('out', sub_states[-1])

        #self.movement_mae = MAE(sub_states)
        return MAE(sub_states)

    def set_current_position(self, coord):
        self.current_position = coord

    def recaler(self):
        self.com.send_slave("S1")
                
            
def main():
    manager = SlaveManager(ComStateFactory("pipo com"))
    manager.set_current_position(Coord(0, 1000, 0))
    mae = manager.go_to_pathfinder(Coord(2200, 1000, 0))
    mae.draw()
    
if __name__ == "__main__":
    main()

    
    

