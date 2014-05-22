""" file to handle all the movements of the slave. """

from mae_generator.mae import InitState, OutState, MAE, MAEState
from pathfinding.pathfinding import PathFinder
from utils.coord import Coord
from com_state_factory import ComStateFactory


class SlaveManager:
    def __init__(self, com_state_factory):
        self.pathfinder = PathFinder()
        self.pathfinder.add_circle(1500, 1050, 200)
        self.pathfinder.add_rect(300, 0, 800, 400)
        self.pathfinder.add_rect(1650, 0, 800, 400)
        self.state_factory = com_state_factory
        self.current_position = Coord(0, 200, 0)
        self.movement_mae = None

    def go_to_direct_from(self, coords_from,  coords_to, precision="NEAR"):
        """ find the best way to go to the way-point given by coords.
        without using the pathfinder. 
        return a movement state machine
        """

        states = [] 
        #TODO chose different options
        states.append(InitState())
        coord_dep = coords_from.get_coords_to(coords_to)
        
        bfcap2 = self.state_factory.get_bf_cap(coords_to)
        if coord_dep.norm() > 10:
            #sinon petit deplacement, donc 1 seul bfcap.
            bfcap1 = self.state_factory.get_bf_cap(coord_dep)
            bfav = self.state_factory.get_bf_fw(Coord(coord_dep.norm()))
            bfcap1.add_transition(precision, bfav)
            bfav.add_transition(precision, bfcap2)
            states.append(bfcap1)
            states.append(bfav)

        states.append(bfcap2)

        #states.append(self.state_factory.get_bf_droite(coords_to)) 
        states.append(OutState())
        states[-2].add_transition(precision, states[-1])
        states[0].add_instant_transition(states[1])
        return MAE(states)

    def go_to_pathfinder(self, coords):
        """complete movement using the pahtfinder to find the 
         best waypoints"""

        sub_states = []
        coords = Coord(coords.x + 1500, coords.y, coords.cap)
        current_position = Coord(self.current_position.x + 1500, self.current_position.y, self.current_position.cap)
        self.pathfinder.find_waypoints(
            current_position.to_tuple(), 
            coords.to_tuple())
        waypoints = self.pathfinder.get_smooth_waypoints()
        print waypoints
        print len(waypoints)
        sub_states.append(InitState())
        
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

        sub_states.append(OutState("end_deplacement"))
        sub_states[-2].add_out_transition('out', sub_states[-1])

        sub_states[0].add_instant_transition(sub_states[1])

        #self.movement_mae = MAE(sub_states)
        return sub_states

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

    
    

