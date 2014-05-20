import pygraphviz as pgv


class State():
    """ class implemeting the state of a state machine 
        To create a state machine, have all state inherit from this one."""
    def __init__(self, name=""):
        self.transitions = {}  # dict of transition : "string" -> state
        if name == "":
            name = self.__class__.__name__
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

    def add_transition(self, trigger, new_state):
        self.transitions[trigger] = new_state

    def add_bloc_transition(self, new_state):
        self.add_transition("BLOC", new_state)

    def add_advd_transition(self, new_state):
        self.add_transition("ADVD", new_state)

    def add_afini_transition(self, new_state):
        self.add_transition("AFINI", new_state)

    def add_instant_transition(self, new_state):
        self.add_transition("", new_state)

    def add_near_transition(self, new_state):
        self.add_transition("NEAR", new_state)

        

    def __str__(self):
        return self.name


class InitState(State):
    def __init__(self, name="init"):
        State.__init__(self, name)


class OutState(State):
    def __init__(self, name="out"):
        State.__init__(self, name)


class MAE():
    """ implementation of state machines
        requires a set of state (inherit from State)

    """
    def __init__(self, state_list=[InitState()], verbose=True):
        self.state_list = state_list
        self.verbose = verbose
        self.reinit_state()
        self.transitions_to_be_done = [""]

    def reinit_state(self, state=None):
        if state is None:
            self.current_state = self.state_list[0]
        else:
            raise NotImplementedError("multiple input not implemented yet")
        if self.verbose:
            print "$$reinit"
            print "$$", self.current_state

    def go_to(self, new_state):
        """ check if new_state is not None and go there """
        if new_state is not None:
            if self.verbose:
                print "transition from {} to {}".format(self.current_state.name, new_state.name)
            self.current_state.out_code()
            self.current_state = new_state
            self.current_state.in_code()

    def run(self):

       # print self.current_state
        self.current_state.while_code()
        # for special out transition for MAEState
        if isinstance(self.current_state, MAEState) and self.current_state.mae.is_over():
            new_state = self.current_state.out_transitions.get(self.current_state.mae.current_state, None)
            self.go_to(new_state)
        for t in self.transitions_to_be_done:
            new_state = self.current_state.transitions.get(t, None)
            self.go_to(new_state)
            if new_state is None and self.verbose:
                pass
                #print "no transition {} in state {}".format(t, self.current_state)

        self.transitions_to_be_done = [""]

    def trigger(self, transition):
        """ the transition "" is the direct transition """
        if isinstance(self.current_state, MAEState):
            self.current_state.trigger(transition)
            if self.current_state.mae.is_over():
                self.transitions_to_be_done.append("out")
        """ trigger the transition to change state """
        self.transitions_to_be_done.append(transition)

    def draw(self, file='mae_render.png'):
        """ draw the mae for debugging purposes """
        graph = pgv.AGraph(directed=True)
        for state in self.state_list:
            for t, s2 in state.transitions.iteritems():
                graph.add_edge(state.__str__(), s2.__str__(), label=t, arrowhead='diamond')
            if isinstance(state, MAEState):
                for out_state, next_state in state.out_transitions.iteritems():
                    graph.add_edge(state, next_state, label="from_{}".format(out_state), arrowhead="diamond", color="green")
        for i, state in enumerate(self.state_list):
            if i == 0:
                init_node = graph.get_node(state.__str__())
                init_node.attr['color'] = 'red'
            if isinstance(state, MAEState):
                node = graph.get_node(state.__str__())
                node.attr['color'] = 'green'

            graph.add_node(self.state_list[0].__str__(), color='red')

        graph.layout(prog='dot')
        graph.draw(file)

    def is_over(self):
        return isinstance(self.current_state, OutState)


class MAEState(State):
    """ State with a MAE inside """
    def __init__(self, mae, name="name of MAE STate"):
        State.__init__(self, name)
        self.mae = mae
        self.name = name
        self.verbose = False
        self.out_transitions = {}

    def in_code(self):
        self.mae.reinit_state()

    def while_code(self):
        self.mae.run()

    def trigger(self, transition):
        self.mae.trigger(transition)

    def add_out_transition(self, out_state_name, next_state):
        """ add a direct transition from the out_state inside the MAEState to the next state """
        for state in self.mae.state_list:
            if isinstance(state, OutState) and state.name == out_state_name:
                self.out_transitions[out_state_name] = next_state
                return
        print "##states", [(isinstance(s, OutState), s.name) for s in self.mae.state_list]
        raise NotImplementedError("no state named {}".format(out_state_name))


def debugger(mae):
    mae.draw()
    import os
    os.system("xdg-open mae_render.png")
    while True:
        mae.verbose = True
        mae.run()
        transition = raw_input()
        mae.trigger(transition)
        
    


