from communication import Communication
from com_state_factory import ComStateFactory
from mae_generator.mae_global import MAEGlobal
from slave_orders import SlaveManager
import time


def main():
    com = Communication("debile")
    factory = ComStateFactory(com)
    slave_manager = SlaveManager(factory) 
    mae = MAEGlobal(factory, slave_manager)
    com.set_global_mae(mae)

    while True:
        mae.run()
        com.run()
        time.sleep(0.0001)



if __name__ == "__main__":
    main()
