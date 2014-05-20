from communication import Communication
from com_state_factory import ComStateFactory
from mae_generator.mae_global import MAEGlobal


def main():
    com = Communication("debile")
    factory = ComStateFactory(com)
    mae = MAEGlobal(factory)
    com.set_global_mae(mae)

    while True:
        mae.run()
        com.run()



if __name__ == "__main__":
    main()
