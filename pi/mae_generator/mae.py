import pygraphviz as pgv


class State():
    def __init__(self, name="name of state"):
        self.transitions = {}  # dict of transition : "string" -> state
        self.name = name

    def in_code(self):
        """ code to be processed when arriving in the state """
        pass

    def out_code(self):
        """ code to be processed when leaving the state """
        pass

    def while_code(self):
        """ code to be processed periodicly while in the state """
        pass

    def __str__(self):
        return self.name


class InitState(State):
    def __init__(self):
        State.__init__(self)
        self.name = "init"


class OutState(State):
    def __init__(self):
        State.__init__(self)
        self.name = "out"


class MAE():
    def __init__(self, state_list=[InitState()], verbose=False):
        self.state_list = state_list
        self.verbose = verbose
        self.reinit_state()
        self.transitions_to_be_done = []

    def reinit_state(self, state=None):
        if state is None:
            self.current_state = self.state_list[0]
        else:
            raise NotImplementedError("multiple input not implemented yet")
        if self.verbose:
            print self.current_state

    def run(self):
        self.current_state.while_code()
        for t in self.transitions_to_be_done:
            new_state = self.current_state.transitions.get(t, None)
            if new_state is not None:
                if self.verbose:
                    print "transition from {} to {}".format(self.current_state.name, new_state.name)
                self.current_state.out_code()
                self.current_state = new_state
                self.current_state.in_code()
            else:
                print "no such transition"
        self.transitions_to_be_done = []

    def trigger(self, transition):
        if self.current_state.__class__.__name__ == 'MAEState':
            self.current_state.trigger(transition)
        """ trigger the transition to change state """
        self.transitions_to_be_done.append(transition)

    def draw(self, file='mae_render.png'):
        """ draw the mae for debugging purposes """
        graph = pgv.AGraph(directed=True)
        graph.add_node(self.state_list[0].name, color='red')
        for state in self.state_list:
            for t, s2 in state.transitions.iteritems():
                graph.add_edge(state.__str__(), s2.__str__(), label=t, arrowhead='diamond')
        graph.layout(prog='dot')
        graph.draw(file)


class MAEState(State):
    """ State with a MAE inside """
    def __init__(self, mae, name="name of MAE STate"):
        self.mae = mae
        self.name = name
        self.verbose = False

    def in_code(self):
        self.mae.current_state = self.mae.reinit_state()

    def while_code(self):
        self.mae.run(self.verbose)
        if self.mae.current_state.name == "out":
            self.trigger("out")

    def trigger(self, transition):
        self.mae.trigger(transition)


def debugger(mae):
    mae.draw()
    while True:
        mae.verbose = True
        mae.run()
        transition = raw_input()
        mae.trigger(transition)
    


