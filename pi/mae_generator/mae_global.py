from mae import MAE, State, debugger


class MAEGlobal(MAE):
    def __init__(self):
        MAE.__init__(self, [State()], True)
        pre_start = State("pre_start")
        game = State("game")
        end = State("end")
        self.state_list = [pre_start, game, end]
        pre_start.transitions["start"] = game
        game.transitions["end_game"] = end 
        self.reinit_state()

if __name__ == "__main__":
    mae = MAEGlobal()
    debugger(mae)
